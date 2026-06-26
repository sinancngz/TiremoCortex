/**
 *******************************************************************************
 * @file        user_adc_isr.c
 * @author      ABOV R&D Division
 * @brief       ADC interrupt handlers — VBAT demo on ADC_ID_2
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

#ifndef True
#define True true
#endif

#ifndef False
#define False false
#endif

/* Generated Code */
/* >>> Placeholder for code generation */

void ADC_IRQHandler_ADC_ID_0(uint32_t un32Event, void *pContext)
{
    (void)un32Event;
    (void)pContext;
}

void ADC_IRQHandler_ADC_ID_1(uint32_t un32Event, void *pContext)
{
    (void)un32Event;
    (void)pContext;
}

void ADC_IRQHandler_ADC_ID_2(uint32_t un32Event, void *pContext)
{
    extern volatile uint8_t g_un8BatteryAdcConvDone;

    (void)pContext;

    if ((un32Event & ADC_EVENT_SINGLE_CAPTURED) != 0U)
        g_un8BatteryAdcConvDone = 1U;
}

/* <<< End of Placeholder */
