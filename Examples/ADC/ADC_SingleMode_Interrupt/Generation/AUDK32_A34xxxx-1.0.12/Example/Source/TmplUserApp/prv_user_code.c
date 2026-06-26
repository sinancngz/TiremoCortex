/**
 *******************************************************************************
 * @file        prv_user_code.c
 * @author      ABOV R&D Division
 * @brief       ADC Single Mode Interrupt — VBAT (AVDD) measurement demo
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

#define BATTERY_ADC_ID              ADC_ID_2
#define BATTERY_VCORE_CH            23U
#define BATTERY_ADC_RESOLUTION      4096UL
#define BATTERY_VCORE_SETTLE_LOOPS  500000UL

#define SYSTICK_1MS_DIV             1000U
#define BATTERY_MEASURE_INTERVAL_MS 1000U

#define DEBUG_UART_CHANNEL          UART_ID_0

extern uint32_t SystemCoreClock;
volatile uint8_t g_un8BatteryAdcConvDone = 0U;

static volatile uint32_t s_un32SysTimerVal = 0U;
static uint8_t s_bBatteryReady = 0U;

void SysTick_Handler(void)
{
    if (s_un32SysTimerVal != 0U)
        s_un32SysTimerVal--;
}

void SYSTICK_Wait(uint32_t un32TimeMS)
{
    s_un32SysTimerVal = un32TimeMS;
    while (s_un32SysTimerVal != 0U);
}

static uint8_t BatteryReading_Init(void)
{
    for (volatile uint32_t i = 0U; i < BATTERY_VCORE_SETTLE_LOOPS; i++)
    {
        ;
    }

    s_bBatteryReady = 1U;
    return 0U;
}

/**
 * @brief Single-mode interrupt read (Start → wait ISR flag → GetData).
 * @note  ADC_MODE_SINGLE + ADC_OPS_INTR on ADC_ID_2 (user_adc.c).
 *        ISR flag is set in user_adc_isr.c (like referans main.c fflag).
 */
static uint8_t BatteryReading_ReadChannel(uint8_t un8Channel, uint16_t *pun16Value)
{
    HAL_ERR_e eErr;
    ADC_SEQ_DATA_t tAdcData;
    ADC_SEQ_TRG_CFG_t tSeqCfg;

    if ((s_bBatteryReady == 0U) || (pun16Value == NULL))
        return 1U;

    (void)HAL_ADC_Stop(BATTERY_ADC_ID);

    tSeqCfg.eType = ADC_TRG_TYPE_INDEPENDENT;
    tSeqCfg.eTrgSrc = ADC_TRG_SRC_ADST;
    tSeqCfg.un8TrgNum = 0U;
    tSeqCfg.utCfg.tInd.un8SeqNum = 0U;
    tSeqCfg.utCfg.tInd.un8ChNum = un8Channel;
    tSeqCfg.utCfg.tInd.un8SamplingTime = 10U;

    eErr = HAL_ADC_SetSeqConfig(BATTERY_ADC_ID, &tSeqCfg);
    if (eErr != HAL_ERR_OK)
        return 2U;

    g_un8BatteryAdcConvDone = 0U;

    eErr = HAL_ADC_Start(BATTERY_ADC_ID);
    if (eErr != HAL_ERR_OK)
        return 3U;

    while (g_un8BatteryAdcConvDone == 0U)
    {
        ;
    }

    tAdcData.bReadDDR = false;
    eErr = HAL_ADC_GetData(BATTERY_ADC_ID, 0U, &tAdcData);
    if (eErr != HAL_ERR_OK)
    {
        (void)HAL_ADC_Stop(BATTERY_ADC_ID);
        return 4U;
    }

    *pun16Value = tAdcData.un16Result;
    (void)HAL_ADC_Stop(BATTERY_ADC_ID);

    return 0U;
}

static uint8_t BatteryReading_ReadSupplyVoltage(uint16_t *pun16VcoreRaw, uint32_t *pun32AvddMillivolts)
{
    uint8_t result;
    uint16_t un16VcoreRaw = 0U;

    if ((s_bBatteryReady == 0U) || (pun16VcoreRaw == NULL) || (pun32AvddMillivolts == NULL))
        return 1U;

    result = BatteryReading_ReadChannel(BATTERY_VCORE_CH, &un16VcoreRaw);
    if (result != 0U)
        return result;

    *pun16VcoreRaw = un16VcoreRaw;

    if (un16VcoreRaw > 0U)
        *pun32AvddMillivolts = (1000UL * BATTERY_ADC_RESOLUTION) / un16VcoreRaw;
    else
    {
        *pun32AvddMillivolts = 0U;
        return 6U;
    }

    return 0U;
}

static void BatteryReading_Print(void)
{
    uint16_t un16VcoreRaw = 0U;
    uint32_t un32AvddMv = 0U;

    if (s_bBatteryReady == 0U)
    {
        DebugFramework_Puts("AVDD: Not initialized\r\n");
        return;
    }

    if (BatteryReading_ReadSupplyVoltage(&un16VcoreRaw, &un32AvddMv) != 0U)
    {
        DebugFramework_Puts("AVDD: Read error\r\n");
        return;
    }

    DebugFramework_Printf("AVDD: %lu.%03lu V (Vcore raw: %u)\r\n",
        (unsigned long)(un32AvddMv / 1000UL),
        (unsigned long)(un32AvddMv % 1000UL),
        (unsigned int)un16VcoreRaw);
}

static void ADC_SingleMode_Interrupt(void)
{
    while (1)
    {
        BatteryReading_Print();
        SYSTICK_Wait(BATTERY_MEASURE_INTERVAL_MS);
    }
}

void PRV_USER_Code(void)
{
    SysTick_Config(SystemCoreClock / SYSTICK_1MS_DIV);

    if (!DebugFramework_Init(DEBUG_UART_CHANNEL))
    {
        while (1);
    }

    if (BatteryReading_Init() != 0U)
    {
        DebugFramework_Puts("Battery init failed\r\n");
        while (1);
    }

    ADC_SingleMode_Interrupt();
}
