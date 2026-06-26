/**
 *******************************************************************************
 * @file        prv_user_code.c
 * @author      ABOV R&D Division
 * @brief       ADC Single Mode Polling — MP23ABS1 (Tiremo-style)
 *
 * Copyright 2024 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include "abov_config.h"
#include "abov_module_config.h"
#include "hal_adc.h"
#include "DebugLibrary/debug_framework.h"
#include "MP23ABS1/mp23abs1_sensor.h"

#define SYSTICK_1MS_DIV             1000U
#define MIC_RECORD_MS               1000U
#define MIC_SYSTICK_HANDLER_DIV     16U

#define AUDIO_BUFFER_SIZE           1024U
#define AUDIO_SAMPLE_RATE           MP23ABS1_FREQ_16KHZ
#define AUDIO_VOLUME                80U

#define DEBUG_UART_CHANNEL          UART_ID_0

extern uint32_t SystemCoreClock;

static volatile uint32_t s_un32SysTimerVal = 0U;
static uint16_t s_aun16AudioBuffer[AUDIO_BUFFER_SIZE];

/*-------------------------------------------------------------------------*//**
 * @brief         SysTick: ms delay + MP23ABS1 sampling (same as Tiremo prv_user_code)
 *//*-------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
    static uint32_t s_un32TickCounter = 0U;

    if (s_un32SysTimerVal != 0U)
        s_un32SysTimerVal--;

    s_un32TickCounter++;
    if (s_un32TickCounter >= MIC_SYSTICK_HANDLER_DIV)
    {
        s_un32TickCounter = 0U;
        MP23ABS1_TimerHandler();
    }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Waiting by time(ms)
 *//*-------------------------------------------------------------------------*/
void SYSTICK_Wait(uint32_t un32TimeMS)
{
    s_un32SysTimerVal = un32TimeMS;
    while (s_un32SysTimerVal != 0U);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure ADC sequence for PA0 / AN0 (once at init)
 *//*-------------------------------------------------------------------------*/
static HAL_ERR_e MicAdc_ConfigureSequence(void)
{
    ADC_SEQ_TRG_CFG_t tSeqCfg;

    tSeqCfg.eType = ADC_TRG_TYPE_INDEPENDENT;
    tSeqCfg.eTrgSrc = ADC_TRG_SRC_ADST;
    tSeqCfg.un8TrgNum = 0U;
    tSeqCfg.utCfg.tInd.un8SeqNum = 0U;
    tSeqCfg.utCfg.tInd.un8ChNum = MP23ABS1_ADC_CHANNEL;
    tSeqCfg.utCfg.tInd.un8SamplingTime = 10U;

    return HAL_ADC_SetSeqConfig(MP23ABS1_ADC_ID, &tSeqCfg);
}

/*-------------------------------------------------------------------------*//**
 * @brief         RMS from audio buffer (same as Tiremo sensor.c)
 *//*-------------------------------------------------------------------------*/
static uint32_t Compute_RMS(const uint16_t *pBuffer, uint32_t un32Len)
{
    uint64_t un64SumSq = 0U;
    uint32_t i;

    if ((pBuffer == NULL) || (un32Len == 0U))
        return 0U;

    for (i = 0U; i < un32Len; i++)
    {
        int32_t n32Sample = (int32_t)pBuffer[i] - 32768;
        un64SumSq += (uint64_t)((int64_t)n32Sample * n32Sample);
    }

    {
        uint64_t un64Mean = un64SumSq / un32Len;
        uint64_t un64X = un64Mean;
        uint64_t un64Y = (un64X + 1U) / 2U;

        if (un64Mean > 0U)
        {
            while (un64Y < un64X)
            {
                un64X = un64Y;
                un64Y = (un64X + un64Mean / un64X) / 2U;
            }

            return (uint32_t)un64X;
        }
    }

    return 0U;
}

void MP23ABS1_Error_Callback(void)
{
    DebugFramework_Puts("Mic: driver error\r\n");
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize microphone (Tiremo Sensor_TestAll / sensor.c style)
 *//*-------------------------------------------------------------------------*/
static uint8_t Mic_Init(void)
{
    MP23ABS1_Init_t tInit;

    tInit.SampleRate = AUDIO_SAMPLE_RATE;
    tInit.Volume = AUDIO_VOLUME;
    tInit.pBuffer = s_aun16AudioBuffer;
    tInit.BufferSize = AUDIO_BUFFER_SIZE;

    if (MP23ABS1_Init(&tInit) != MP23ABS1_OK)
        return 1U;

    if (MicAdc_ConfigureSequence() != HAL_ERR_OK)
        return 2U;

    return 0U;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Record 1 s via SysTick polling, then print RMS + sample count
 * @note          Polling runs inside MP23ABS1_TimerHandler (ADC_OPS_POLL)
 *//*-------------------------------------------------------------------------*/
static void ADC_SingleMode_Polling(void)
{
    uint32_t un32Count;
    uint32_t un32CalcLen;
    uint32_t un32Rms;

    while (1)
    {
        if (MP23ABS1_StartRecording() != MP23ABS1_OK)
        {
            DebugFramework_Puts("Mic: start failed\r\n");
            SYSTICK_Wait(MIC_RECORD_MS);
            continue;
        }

        SYSTICK_Wait(MIC_RECORD_MS);
        (void)MP23ABS1_StopRecording();

        un32Count = MP23ABS1_GetSampleCount();
        if (un32Count == 0U)
        {
            DebugFramework_Puts("Mic: no samples\r\n");
            continue;
        }

        un32CalcLen = (un32Count < AUDIO_BUFFER_SIZE) ? un32Count : AUDIO_BUFFER_SIZE;
        un32Rms = Compute_RMS(s_aun16AudioBuffer, un32CalcLen);

        DebugFramework_Printf("Mic RMS: %lu  samples: %lu\r\n",
            (unsigned long)un32Rms,
            (unsigned long)un32Count);
    }
}

/*-------------------------------------------------------------------------*//**
 * @brief         User application entry (called from main.c)
 *//*-------------------------------------------------------------------------*/
void PRV_USER_Code(void)
{
    SysTick_Config(SystemCoreClock / SYSTICK_1MS_DIV);

    if (!DebugFramework_Init(DEBUG_UART_CHANNEL))
    {
        while (1);
    }

    if (Mic_Init() != 0U)
    {
        DebugFramework_Puts("MP23ABS1 init failed\r\n");
        while (1);
    }

    ADC_SingleMode_Polling();
}
