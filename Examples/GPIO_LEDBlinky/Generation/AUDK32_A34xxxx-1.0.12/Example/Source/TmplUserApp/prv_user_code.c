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

#define BOARD_LED1_PORT             PCU_ID_B
#define BOARD_LED1_PIN              PCU_PIN_ID_4    /* PB4  */
#define BOARD_LED2_PORT             PCU_ID_B
#define BOARD_LED2_PIN              PCU_PIN_ID_9    /* PB9  */
#define BOARD_LED3_PORT             PCU_ID_F
#define BOARD_LED3_PIN              PCU_PIN_ID_7    /* PF7  */
#define BOARD_LED4_PORT             PCU_ID_B
#define BOARD_LED4_PIN              PCU_PIN_ID_5    /* PB5  */
#define BOARD_LED5_PORT             PCU_ID_B
#define BOARD_LED5_PIN              PCU_PIN_ID_10   /* PB10 */
#define BOARD_LED6_PORT             PCU_ID_B
#define BOARD_LED6_PIN              PCU_PIN_ID_11   /* PB11 */
#define BOARD_LED7_PORT             PCU_ID_B
#define BOARD_LED7_PIN              PCU_PIN_ID_12   /* PB12 */
#define BOARD_LED8_PORT             PCU_ID_B
#define BOARD_LED8_PIN              PCU_PIN_ID_13   /* PB13 */
#define BOARD_LED9_PORT             PCU_ID_B
#define BOARD_LED9_PIN              PCU_PIN_ID_14   /* PB14 */
#define BOARD_LED10_PORT            PCU_ID_B
#define BOARD_LED10_PIN             PCU_PIN_ID_15   /* PB15 */

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
 * @brief       Turn ON specific LED
 * @param   	LED_PIN : LED pin ID
 * @return      None
 **********************************************************************/
void LED_ON(PCU_ID_e port, PCU_PIN_ID_e pin)
{
    HAL_PCU_SetOutputBit(port, pin, PCU_OUTPUT_BIT_CLEAR);
}

/**********************************************************************
 * @brief       Turn OFF specific LED
 * @param   	LED_PIN : LED pin ID
 * @return      None
 **********************************************************************/
void LED_OFF(PCU_ID_e port, PCU_PIN_ID_e pin)
{
    HAL_PCU_SetOutputBit(port, pin, PCU_OUTPUT_BIT_SET);
}

/**********************************************************************
 * @brief       Blink all LEDs
 * @param   	None
 * @return      None
 **********************************************************************/
void LedBlink(void)
{
    typedef struct { PCU_ID_e port; PCU_PIN_ID_e pin; } LedEntry;
    LedEntry ledArray[] = {
        {BOARD_LED1_PORT,  BOARD_LED1_PIN},
        {BOARD_LED2_PORT,  BOARD_LED2_PIN},
        {BOARD_LED3_PORT,  BOARD_LED3_PIN},
        {BOARD_LED4_PORT,  BOARD_LED4_PIN},
        {BOARD_LED5_PORT,  BOARD_LED5_PIN},
        {BOARD_LED6_PORT,  BOARD_LED6_PIN},
        {BOARD_LED7_PORT,  BOARD_LED7_PIN},
        {BOARD_LED8_PORT,  BOARD_LED8_PIN},
        {BOARD_LED9_PORT,  BOARD_LED9_PIN},
        {BOARD_LED10_PORT, BOARD_LED10_PIN},
    };
    uint32_t i;

    for (i = 0; i < 10; i++)
    {
    	LED_ON(ledArray[i].port, ledArray[i].pin);
    }

    SYSTICK_Wait(500);
    for (i = 0; i < 10; i++)
    {
    	LED_OFF(ledArray[i].port, ledArray[i].pin);
    }
    SYSTICK_Wait(500);
}

/**********************************************************************
 * @brief       Configure GPIO Alternate Functions
 * @param   	None
 * @return      None
 **********************************************************************/
void GPIO_Configure()
{
    /* Configure all Test LEDs as GPIO output (ALT_0) */
    HAL_PCU_SetAltMode(BOARD_LED1_PORT, BOARD_LED1_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(BOARD_LED2_PORT, BOARD_LED2_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(BOARD_LED3_PORT, BOARD_LED3_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(BOARD_LED4_PORT, BOARD_LED4_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(BOARD_LED5_PORT, BOARD_LED5_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(BOARD_LED6_PORT, BOARD_LED6_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(BOARD_LED7_PORT, BOARD_LED7_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(BOARD_LED8_PORT, BOARD_LED8_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(BOARD_LED9_PORT, BOARD_LED9_PIN, PCU_ALT_0);
    HAL_PCU_SetAltMode(BOARD_LED10_PORT, BOARD_LED10_PIN, PCU_ALT_0);
}

/**********************************************************************
 * @brief       User Code Here
 * @param   	None
 * @return      None
 **********************************************************************/
void PRV_USER_Code(void)
{
    SysTick_Config(SystemCoreClock / SYSTICK_1MS_DIV);
    GPIO_Configure();
    /* put your code above this line. */
    while (1)
    {
    	LedBlink();
    }
}
