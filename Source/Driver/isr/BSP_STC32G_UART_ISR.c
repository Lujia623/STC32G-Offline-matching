#include "BSP_STC32G_UART.h"
#include "app_uart.h"

extern bit uart1_busy, uart2_busy, uart3_busy, uart4_busy;

void UART1_int (void) interrupt UART1_VECTOR
{
    uint8_t Rxbyte = 0x00, Txbyte = 0x00;
    uint8_t error = COMMON_NO_ERROR;

    if(RI)
    {
        Rxbyte = SBUF;
        CommonRx_put_char(COMMON_UART1, Rxbyte);
        RI = 0;
    }

    if(TI)
    {
        TI = 0;
        uart1_busy = 0;
    }
}

void UART2_int (void) interrupt UART2_VECTOR
{
    uint8_t byte = 0x00;

    if(S2RI)
    {
        byte = S2BUF;
        CommonRx_put_char(COMMON_UART2, byte);
        S2RI = 0;
    }

    if(S2TI)
    {
        S2TI = 0;
        uart2_busy = 0;
    }
}

void UART3_int (void) interrupt UART3_VECTOR
{
    uint8_t byte = 0x00;

    if(S3RI)
    {
        byte = S3BUF;
        CommonRx_put_char(COMMON_UART3, byte);
        S3RI = 0;
    }

    if(S3TI)
    {
        S3TI = 0;
        uart3_busy = 0;
    }
}

void UART4_int (void) interrupt UART4_VECTOR
{
    uint8_t byte = 0x00;

    if(S4RI)
    {
        byte = S4BUF;
        CommonRx_put_char(COMMON_UART4, byte);
        S4RI = 0;
    }

    if(S4TI)
    {
        S4TI = 0;
        uart4_busy = 0;
    }
}
