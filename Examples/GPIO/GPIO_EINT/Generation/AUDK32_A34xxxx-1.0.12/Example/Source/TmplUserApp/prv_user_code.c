/**
 *******************************************************************************
 * @file        prv_user_code.c
 * @author      ABOV R&D Division
 * @brief       GPIO External Interrupt — user button on PC9
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
#include "hal_pcu.h"

#if (CONFIG_DEBUG == 1)
#include "debug_log.h"
#include "debug.h"
#endif

/* Include HAL header files for your target modules */

#define BOARD_LED6_PORT             PCU_ID_B
#define BOARD_LED6_PIN              PCU_PIN_ID_11   /* PB11 */
#define BOARD_USER_BTN_PORT         PCU_ID_C
#define BOARD_USER_BTN_PIN          PCU_PIN_ID_9    /* PC9 */
#define BOARD_USER_BTN_INTR_MASK    (0x3UL << ((uint32_t)BOARD_USER_BTN_PIN * 2U))

#define SYSTICK_1MS_DIV             1000U

extern uint32_t SystemCoreClock;

static volatile uint32_t s_un32SysTimerVal = 0U;
static volatile uint8_t s_btnPressed = 0U;
static volatile uint8_t s_btnEvent = 0U;

/*-------------------------------------------------------------------------*//**
 * @brief         ARM System Timer Interrupt Handler
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
    if (s_un32SysTimerVal != 0U)
        s_un32SysTimerVal--;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Waiting by time(ms)
 * @param         un32TimeMS : Milisecond time to wait
 * @return        None
 *//*-------------------------------------------------------------------------*/
void SYSTICK_Wait(uint32_t un32TimeMS)
{
    s_un32SysTimerVal = un32TimeMS;
    while (s_un32SysTimerVal != 0U);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Turn ON LED (active low)
 * @param         port : PCU port ID
 * @param         pin  : PCU pin ID
 * @return        None
 *//*-------------------------------------------------------------------------*/
static void LED_ON(PCU_ID_e port, PCU_PIN_ID_e pin)
{
    HAL_PCU_SetOutputBit(port, pin, PCU_OUTPUT_BIT_CLEAR);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Turn OFF LED (active low)
 * @param         port : PCU port ID
 * @param         pin  : PCU pin ID
 * @return        None
 *//*-------------------------------------------------------------------------*/
static void LED_OFF(PCU_ID_e port, PCU_PIN_ID_e pin)
{
    HAL_PCU_SetOutputBit(port, pin, PCU_OUTPUT_BIT_SET);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Read button state
 * @param         None
 * @return        1 if pressed, 0 if released
 *//*-------------------------------------------------------------------------*/
static uint8_t Button_IsPressed(void)
{
    PCU_PORT_e level = PCU_PORT_HIGH;

    if (HAL_PCU_GetInputValue(BOARD_USER_BTN_PORT, BOARD_USER_BTN_PIN, &level) != HAL_ERR_OK)
        return 0U;

    return (level == PCU_PORT_LOW) ? 1U : 0U;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Update LED according to button state
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
static void Button_UpdateLed(void)
{
    if (Button_IsPressed() != 0U)
        LED_ON(BOARD_LED6_PORT, BOARD_LED6_PIN);
    else
        LED_OFF(BOARD_LED6_PORT, BOARD_LED6_PIN);
}

/*-------------------------------------------------------------------------*//**
 * @brief         Port C PCU interrupt handler (PC9 button)
 * @param         un32Event : PCU interrupt event flags
 * @param         pContext  : Handler context
 * @return        None
 *//*-------------------------------------------------------------------------*/
void PCU_IRQHandler_PCU_ID_C(uint32_t un32Event, void *pContext)
{
    (void)pContext;

    if ((un32Event & BOARD_USER_BTN_INTR_MASK) == 0U)
        return;

    s_btnPressed = Button_IsPressed();
    Button_UpdateLed();
    s_btnEvent = 1U;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure button external interrupt
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
static void ButtonIntr_Init(void)
{
    PCU_IRQ_CFG_t irqCfg;

    (void)HAL_PCU_SetIntrPort(BOARD_USER_BTN_PORT, BOARD_USER_BTN_PIN,
                              PCU_INTR_MODE_EDGE,
                              PCU_INTR_TRG_BOTH_LEVEL_EDGE,
                              0U);

    irqCfg.eId         = BOARD_USER_BTN_PORT;
    irqCfg.eOps        = PCU_OPS_INTR;
    irqCfg.pfnHandler  = PCU_IRQHandler_PCU_ID_C;
    irqCfg.pContext    = NULL;
    irqCfg.un32IRQPrio = 3U;
    irqCfg.un8IntNum   = 0U;
    (void)HAL_PCU_SetIRQ(&irqCfg);
}

/*-------------------------------------------------------------------------*//**
 * @brief         User Code Here
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void PRV_USER_Code(void)
{
    SysTick_Config(SystemCoreClock / SYSTICK_1MS_DIV);
    ButtonIntr_Init();

    while (1)
    {
        if (s_btnEvent != 0U)
        {
            s_btnEvent = 0U;
        }

        SYSTICK_Wait(10U);
    }
}
