/**
 *******************************************************************************
 * @file        prv_user_code.c
 * @author      ABOV R&D Division
 * @brief       Dummy User Application Main
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

#define SYSTICK_1MS_DIV         1000U

extern uint32_t SystemCoreClock;
static volatile uint32_t s_un32SysTimerVal=0;

/**********************************************************************
 * @brief		ARM System Timer Interrupt Handler.
 * @param   	None
 * @return	    None
 **********************************************************************/
void SysTick_Handler (void)
{
    if (s_un32SysTimerVal)
    {
        s_un32SysTimerVal--;
    }
}

/**********************************************************************
 * @brief		Waiting by time(ms)
 * @param   	un32TimeMS : Milisecond time to wait.
 * @return	    None
 **********************************************************************/
void SYSTICK_Wait (uint32_t un32TimeMS)
{
    s_un32SysTimerVal = un32TimeMS;
    while (s_un32SysTimerVal);
}

/**********************************************************************
 * @brief       Turn ON LED (active low)
 * @param   	port : PCU port ID
 * @param   	pin  : PCU pin ID
 * @return      None
 **********************************************************************/
void LED_ON(PCU_ID_e port, PCU_PIN_ID_e pin)
{
    HAL_PCU_SetOutputBit(port, pin, PCU_OUTPUT_BIT_CLEAR);
}

/**********************************************************************
 * @brief       Turn OFF LED (active low)
 * @param   	port : PCU port ID
 * @param   	pin  : PCU pin ID
 * @return      None
 **********************************************************************/
void LED_OFF(PCU_ID_e port, PCU_PIN_ID_e pin)
{
    HAL_PCU_SetOutputBit(port, pin, PCU_OUTPUT_BIT_SET);
}

/**********************************************************************
 * @brief       Read user button and control LED accordingly
 * @param   	None
 * @return      None
 **********************************************************************/
void GPIO_inputRun(void)
{
    PCU_PORT_e eBtnState;
    PCU_PORT_e ePrevBtnState = PCU_PORT_MAX;

    while (1)
    {
        HAL_PCU_GetInputValue(BOARD_USER_BTN_PORT, BOARD_USER_BTN_PIN, &eBtnState);

        if (eBtnState != ePrevBtnState)
        {
            if (eBtnState == PCU_PORT_LOW)
            {
                LED_ON(BOARD_LED6_PORT, BOARD_LED6_PIN);
            }
            else
            {
                LED_OFF(BOARD_LED6_PORT, BOARD_LED6_PIN);
            }
            ePrevBtnState = eBtnState;
        }

        SYSTICK_Wait(50);
    }
}

/**********************************************************************
 * @brief       User Code Here
 * @param   	None
 * @return      None
 **********************************************************************/
void PRV_USER_Code(void)
{
    SysTick_Config(SystemCoreClock / SYSTICK_1MS_DIV);
    GPIO_inputRun();
}
