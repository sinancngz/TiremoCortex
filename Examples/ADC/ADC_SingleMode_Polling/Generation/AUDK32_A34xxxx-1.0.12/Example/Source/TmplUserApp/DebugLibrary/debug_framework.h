/**
 *******************************************************************************
 * @file        debug_framework.h
 * @author      Custom Debug Framework
 * @brief       Lightweight UART debug framework (UART configured via MCUbrew32)
 *******************************************************************************/

#ifndef _DEBUG_FRAMEWORK_H_
#define _DEBUG_FRAMEWORK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "hal_uart.h"

/**
 * @brief Activate a UART channel as the debug output channel.
 * @param eUartId UART instance (e.g. UART_ID_0). Pin/baud/clock must already
 *                be configured by PRV_UART_Init() from MCUbrew32.
 * @return true on success, false if eUartId is invalid
 */
bool DebugFramework_Init(UART_ID_e eUartId);

void DebugFramework_Uninit(void);

void DebugFramework_PutChar(uint8_t ch);
void DebugFramework_Puts(const char *str);
void DebugFramework_PutsLine(const char *str);
void DebugFramework_PutDec(uint8_t num);
void DebugFramework_PutDec16(uint16_t num);
void DebugFramework_PutDec32(uint32_t num);
void DebugFramework_PutHex(uint8_t num);
void DebugFramework_PutHex16(uint16_t num);
void DebugFramework_PutHex32(uint32_t num);
void DebugFramework_Printf(const char *format, ...);

uint8_t DebugFramework_GetChar(void);
bool DebugFramework_IsDataAvailable(void);
bool DebugFramework_IsInitialized(void);
UART_ID_e DebugFramework_GetUartId(void);

#ifdef __cplusplus
}
#endif

#endif /* _DEBUG_FRAMEWORK_H_ */
