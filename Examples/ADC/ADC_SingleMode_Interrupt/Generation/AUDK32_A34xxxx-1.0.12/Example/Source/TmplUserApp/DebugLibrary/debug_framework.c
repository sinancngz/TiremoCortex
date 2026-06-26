/**
 *******************************************************************************
 * @file        debug_framework.c
 * @author      Custom Debug Framework
 * @brief       Lightweight UART debug framework implementation
 *******************************************************************************/

#include "debug_framework.h"
#include <stdarg.h>
#include <stdio.h>

static bool s_bInitialized = false;
static UART_ID_e s_eUartId = UART_ID_MAX;

static void PRV_SendByte(uint8_t data)
{
    if (!s_bInitialized)
    {
        return;
    }

    (void)HAL_UART_Transmit(s_eUartId, &data, 1, true);
}

bool DebugFramework_Init(UART_ID_e eUartId)
{
    if (eUartId >= UART_ID_MAX)
    {
        return false;
    }

    s_eUartId = eUartId;
    s_bInitialized = true;
    return true;
}

void DebugFramework_Uninit(void)
{
    s_bInitialized = false;
    s_eUartId = UART_ID_MAX;
}

void DebugFramework_PutChar(uint8_t ch)
{
    PRV_SendByte(ch);
}

void DebugFramework_Puts(const char *str)
{
    if ((str == NULL) || (!s_bInitialized))
    {
        return;
    }

    while (*str != '\0')
    {
        PRV_SendByte((uint8_t)(*str));
        str++;
    }
}

void DebugFramework_PutsLine(const char *str)
{
    DebugFramework_Puts(str);
    DebugFramework_Puts("\r\n");
}

void DebugFramework_PutDec(uint8_t num)
{
    uint8_t c1 = (uint8_t)(num % 10U);
    uint8_t c2 = (uint8_t)((num / 10U) % 10U);
    uint8_t c3 = (uint8_t)((num / 100U) % 10U);

    PRV_SendByte((uint8_t)('0' + c3));
    PRV_SendByte((uint8_t)('0' + c2));
    PRV_SendByte((uint8_t)('0' + c1));
}

void DebugFramework_PutDec16(uint16_t num)
{
    char buffer[6];
    int i = 0;

    if (num == 0U)
    {
        PRV_SendByte((uint8_t)'0');
        return;
    }

    while ((num > 0U) && (i < 5))
    {
        buffer[i++] = (char)('0' + (num % 10U));
        num = (uint16_t)(num / 10U);
    }

    while (i > 0)
    {
        PRV_SendByte((uint8_t)buffer[--i]);
    }
}

void DebugFramework_PutDec32(uint32_t num)
{
    char buffer[11];
    int i = 0;

    if (num == 0UL)
    {
        PRV_SendByte((uint8_t)'0');
        return;
    }

    while ((num > 0UL) && (i < 10))
    {
        buffer[i++] = (char)('0' + (num % 10UL));
        num /= 10UL;
    }

    while (i > 0)
    {
        PRV_SendByte((uint8_t)buffer[--i]);
    }
}

void DebugFramework_PutHex(uint8_t num)
{
    static const char hex[] = "0123456789ABCDEF";

    PRV_SendByte((uint8_t)hex[(num >> 4) & 0x0FU]);
    PRV_SendByte((uint8_t)hex[num & 0x0FU]);
}

void DebugFramework_PutHex16(uint16_t num)
{
    DebugFramework_PutHex((uint8_t)(num >> 8));
    DebugFramework_PutHex((uint8_t)(num & 0xFFU));
}

void DebugFramework_PutHex32(uint32_t num)
{
    DebugFramework_PutHex16((uint16_t)(num >> 16));
    DebugFramework_PutHex16((uint16_t)(num & 0xFFFFUL));
}

void DebugFramework_Printf(const char *format, ...)
{
    char buffer[160];
    int len;
    va_list args;

    if ((format == NULL) || (!s_bInitialized))
    {
        return;
    }

    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (len <= 0)
    {
        return;
    }

    if (len >= (int)sizeof(buffer))
    {
        len = (int)sizeof(buffer) - 1;
    }

    for (int i = 0; i < len; i++)
    {
        PRV_SendByte((uint8_t)buffer[i]);
    }
}

uint8_t DebugFramework_GetChar(void)
{
    uint8_t ch = 0;

    if (!s_bInitialized)
    {
        return 0;
    }

    (void)HAL_UART_Receive(s_eUartId, &ch, 1, true);
    return ch;
}

bool DebugFramework_IsDataAvailable(void)
{
    uint8_t lineStatus = 0;

    if (!s_bInitialized)
    {
        return false;
    }

    if (HAL_UART_GetLineStatus(s_eUartId, &lineStatus) != HAL_ERR_OK)
    {
        return false;
    }

    return ((lineStatus & (uint8_t)UART_LINE_STATUS_RX_DONE) != 0U);
}

bool DebugFramework_IsInitialized(void)
{
    return s_bInitialized;
}

UART_ID_e DebugFramework_GetUartId(void)
{
    return s_eUartId;
}
