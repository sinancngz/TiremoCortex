/**
 *******************************************************************************
 * @file        prv_user_code.c
 * @author      ABOV R&D Division
 * @brief       ADC Sequential Mode — mic (ch0) + AVDD (ch23) on ADC_ID_0
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

#define SEQ_ADC_ID                  ADC_ID_0
#define SEQ_ADC_RESOLUTION          4096UL
#define SEQ_ADC_WAIT_TIMEOUT        100000UL

#define SYSTICK_1MS_DIV             1000U
#define VCORE_SETTLE_MS             10U
#define MIC_RECORD_MS               1000U
#define MIC_SYSTICK_HANDLER_DIV     16U

#define AUDIO_BUFFER_SIZE           1024U
#define AUDIO_SAMPLE_RATE           MP23ABS1_FREQ_16KHZ
#define AUDIO_VOLUME                80U

#define DEBUG_UART_CHANNEL          UART_ID_0

extern uint32_t SystemCoreClock;

static volatile uint32_t s_un32SysTimerVal = 0U;
static uint16_t s_aun16AudioBuffer[AUDIO_BUFFER_SIZE];
static uint16_t s_un16LastVcoreRaw = 0U;

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

void SYSTICK_Wait(uint32_t un32TimeMS)
{
    s_un32SysTimerVal = un32TimeMS;
    while (s_un32SysTimerVal != 0U);
}

/**
 * @brief One sequential conversion (referans Tiremo/main.c ADC_SequentialModeRun).
 *        Start → wait EOC → GetData(seq0) → wait EOC → GetData(seq1) → Stop.
 */
static uint8_t SeqAdc_RunOnce(uint16_t *pun16MicRaw, uint16_t *pun16VcoreRaw)
{
    HAL_ERR_e eErr;
    ADC_SEQ_DATA_t tAdcData;

    eErr = HAL_ADC_Start(SEQ_ADC_ID);
    if (eErr != HAL_ERR_OK)
        return 2U;

    eErr = HAL_ADC_SetWaitComplete(SEQ_ADC_ID, SEQ_ADC_WAIT_TIMEOUT);
    if (eErr != HAL_ERR_OK)
    {
        (void)HAL_ADC_Stop(SEQ_ADC_ID);
        return 3U;
    }

    tAdcData.bReadDDR = false;
    eErr = HAL_ADC_GetData(SEQ_ADC_ID, 0U, &tAdcData);
    if (eErr != HAL_ERR_OK)
    {
        (void)HAL_ADC_Stop(SEQ_ADC_ID);
        return 4U;
    }

    if (pun16MicRaw != NULL)
        *pun16MicRaw = tAdcData.un16Result;

    eErr = HAL_ADC_SetWaitComplete(SEQ_ADC_ID, SEQ_ADC_WAIT_TIMEOUT);
    if (eErr != HAL_ERR_OK)
    {
        (void)HAL_ADC_Stop(SEQ_ADC_ID);
        return 5U;
    }

    tAdcData.bReadDDR = false;
    eErr = HAL_ADC_GetData(SEQ_ADC_ID, 1U, &tAdcData);
    if (eErr != HAL_ERR_OK)
    {
        (void)HAL_ADC_Stop(SEQ_ADC_ID);
        return 6U;
    }

    if (pun16VcoreRaw != NULL)
        *pun16VcoreRaw = tAdcData.un16Result;

    (void)HAL_ADC_Stop(SEQ_ADC_ID);

    return 0U;
}

/**
 * @brief Called from MP23ABS1_TimerHandler — runs full sequence, returns mic sample.
 */
uint8_t SeqAdc_ReadMicSample(uint16_t *pun16MicRaw)
{
    uint16_t un16VcoreRaw = 0U;
    uint8_t result;

    if (pun16MicRaw == NULL)
        return 1U;

    result = SeqAdc_RunOnce(pun16MicRaw, &un16VcoreRaw);
    if (result == 0U)
        s_un16LastVcoreRaw = un16VcoreRaw;

    return result;
}

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

static uint8_t Mic_Init(void)
{
    MP23ABS1_Init_t tInit;

    tInit.SampleRate = AUDIO_SAMPLE_RATE;
    tInit.Volume = AUDIO_VOLUME;
    tInit.pBuffer = s_aun16AudioBuffer;
    tInit.BufferSize = AUDIO_BUFFER_SIZE;

    if (MP23ABS1_Init(&tInit) != MP23ABS1_OK)
        return 1U;

    return 0U;
}

static void Print_MicAndAvdd(uint32_t un32Rms, uint32_t un32Count)
{
    uint32_t un32AvddMv = 0U;

    if (s_un16LastVcoreRaw > 0U)
        un32AvddMv = (1000UL * SEQ_ADC_RESOLUTION) / (uint32_t)s_un16LastVcoreRaw;

    DebugFramework_Printf("Mic RMS: %lu  samples: %lu\r\n",
        (unsigned long)un32Rms,
        (unsigned long)un32Count);

    DebugFramework_Printf("AVDD: %lu.%03lu V (Vcore raw: %u)\r\n",
        (unsigned long)(un32AvddMv / 1000UL),
        (unsigned long)(un32AvddMv % 1000UL),
        (unsigned int)s_un16LastVcoreRaw);
}

static void ADC_SequentialMode_Run(void)
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
        Print_MicAndAvdd(un32Rms, un32Count);
    }
}

void PRV_USER_Code(void)
{
    SysTick_Config(SystemCoreClock / SYSTICK_1MS_DIV);

    if (!DebugFramework_Init(DEBUG_UART_CHANNEL))
    {
        while (1);
    }

    SYSTICK_Wait(VCORE_SETTLE_MS);

    if (Mic_Init() != 0U)
    {
        DebugFramework_Puts("MP23ABS1 init failed\r\n");
        while (1);
    }

    ADC_SequentialMode_Run();
}
