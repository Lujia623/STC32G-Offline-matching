#include "BSP_STC32G_UART.h"
#include "BSP_STC32G_NVIC.h"
#include "BSP_STC32G_SWITCH_GPIO.h"
#include "app_uart.h"

bit uart1_busy = 0, uart2_busy = 0, uart3_busy = 0, uart4_busy = 0;

static uint8_t UARTx_is_Inited[UART_MAX];
static UART_InitTypeDef UARTX_InitStruct[UART_MAX];

static void SetTimerxBaudRateGenerator(UARTxTypeDef UARTx, BAUD_RATE_TimerDef eBaud_Rate, uint16_t BaudRate);

static void SetUARTxModeRXENABLE(UARTxTypeDef UARTx, UART_ModeDef eMode, uint8_t enable);

void UART_Init(UARTxTypeDef UARTx, UART_InitTypeDef *UART_InitStruct) 
{
    uint16_t counter;

    assert_param(IS_UARTX(UARTx));
    assert_param(IS_BAUD_RATE_TIMER(UART_InitStruct->BaudRateTimer));
    assert_param(IS_UARTX_MODE(UART_InitStruct->UART_Mode));

    UARTx_is_Inited[UARTx] = true;
    counter = (MAIN_Fosc / 4 / UART_InitStruct->UART_BaudRate);
    SetTimerxBaudRateGenerator(UARTx, UART_InitStruct->BaudRateTimer, counter);
    SetUARTxModeRXENABLE(UARTx, UART_InitStruct->UART_Mode, UART_InitStruct->UART_RxEnable);
}

static void SetUARTxModeRXENABLE(UARTxTypeDef UARTx, UART_ModeDef eMode, uint8_t enable) 
{
    switch (UARTx) {
        case UART1:
            SCON = (SCON & 0x3F) | eMode;
            REN = enable;
            break;
        case UART2:
            S2CON = (S2CON & 0x3F) | eMode;
            S2REN = enable;
            break;
        case UART3:
            S3CON = (S3CON & 0x7F) | eMode;
            S3REN = enable;
            break;
        case UART4:
            S4CON = (S4CON & 0x7F) | eMode;
            S4REN = enable;
            break;

        default:
            break;
    }
}

static void SetTimerxBaudRateGenerator(UARTxTypeDef UARTx, BAUD_RATE_TimerDef eBaud_Rate, uint16_t BaudRate) 
{
    assert_param(IS_UARTX(UARTx));
    assert_param(IS_BAUD_RATE_TIMER(eBaud_Rate));

    switch (eBaud_Rate) {
        case BAUD_RATE_TIMER1:
            if (UART1 == UARTx) {
                TR1 = 0;
                S1BRT = 0;
                T1_CT = 0;
                T1x12 = 1;
                TMOD &= ~0x30;
                TL1 = (uint8_t)((65536 - BaudRate) & 0x00ff);
                TH1 = (uint8_t)((65536 - BaudRate) >> 8);
                ET1 = 0;
                TR1 = 1;
                // TI = 1;
            }
            break;

        case BAUD_RATE_TIMER2: {
            T2R = 0;
            T2L = (uint8_t)((65536 - BaudRate) & 0x00ff);
            T2H = (uint8_t)((65536 - BaudRate) >> 8);
            if (UART1 == UARTx) {
                S1BRT = 1;
            } else if (UART3 == UARTx) {
                S3ST3 = 0;
            } else if (UART4 == UARTx) {
                S4ST4 = 0;
            }
            T2_CT = 0;
            T2x12 = 1;
            ET2 = 0;
            T2R = 1;
        }
            break;

        case BAUD_RATE_TIMER3:
            if (UART3 == UARTx) {
                S3ST3 = 1;
                T3R = 0;
                T3L = (uint8_t)((65536 - BaudRate) & 0x00ff);
                T3H = (uint8_t)((65536 - BaudRate) >> 8);
                T3_CT = 0;
                T3x12 = 1;
                T3R = 1;
            }
            break;

        case BAUD_RATE_TIMER4:
            if (UART4 == UARTx) {
                S4ST4 = 1;
                T4R = 0;
                T4L = (uint8_t)((65536 - BaudRate) & 0x00ff);
                T4H = (uint8_t)((65536 - BaudRate) >> 8);
                T4_CT = 0;
                T4x12 = 1;
                T4R = 1;
            }
            break;

        default :
            break;
    }
}

void SetUARTXBaudRate(UARTxTypeDef UARTx, uint32_t baudrate)
{
    uint16_t counter;

    counter = (MAIN_Fosc / 4 / baudrate);
    SetTimerxBaudRateGenerator(UARTx, UARTX_InitStruct[UARTx].BaudRateTimer, counter);
}

void Receive_Ebable(UARTxTypeDef UARTx, uint8_t enable)
{
    switch (UARTx) {
        case UART1:
            REN = enable;
            break;

        case UART2:
            S2REN = enable;
            break;

        case UART3:
            S3REN = enable;
            break;

        case UART4:
            S4REN = enable;
            break;
    
    default:
        break;
    }
}

void SendByte(UARTxTypeDef UARTx, uint8_t byte) 
{
    assert_param(IS_UARTX(UARTx));

    switch (UARTx) {
        case UART1:
            if (ES && EA) {
                SBUF = byte;
                uart1_busy = 1;
                while (uart1_busy);
            } else {
                TI = 0;
                SBUF = byte;
                while (0 == TI);
                TI = 0;
            }
            break;

        case UART2:
            if (ES2 && EA) {
                S2BUF = byte;
                uart2_busy = 1;
                while (uart2_busy);
            } else {
                S2TI = 0;
                S2BUF = byte;
                while (0 == S2TI);
                S2TI = 0;
            }
            break;

        case UART3:
            if (ES3 && EA) {
                S3BUF = byte;
                uart3_busy = 1;
                while (uart3_busy);
            } else {
                S3TI = 0;
                S3BUF = byte;
                while (0 == S3TI);
                S3TI = 0;
            }
            break;

        case UART4:
            if (ES4 && EA) {
                S4BUF = byte;
                uart4_busy = 1;
                while (uart4_busy);
            } else {
                S4TI = 0;
                S4BUF = byte;
                while (0 == S4TI);
                S4TI = 0;
            }
            break;


        default:
            break;
    }
}

/*
uint8_t ReceiveByte(UARTxTypeDef UARTx)
{
    uint8_t byte;

    assert_param(IS_UARTX(UARTx));

    switch(UARTx)
    {
        case UART1:
            if(ES == 0) {
                while (!RI);
                byte = SBUF;
                RI = 0;
            } else {
                byte = SBUF;
                while (!RI);
            }
            break;

        case UART2:
            if(ES2) {
                byte = S2BUF;
                while (!S2RI);
            } else {
                byte = S2BUF;
                while (!S2RI);
                S2RI = 0;
            }
            break;

        case UART3:
            if(ES3) {
                byte = S3BUF;
                while(!S3RI);
            } else {
                byte = S3BUF;
                while(!S3RI);
                S3RI = 0;
            }
            break;

        case UART4:
            if(ES4) {
                byte = S4BUF;
                while(!S4RI);
            } else {
                byte = S4BUF;
                while(!S4RI);
                S4RI = 0;
            }
            break;
        
        default:
            break;
    }

    return byte;
}*/

void SendString(UARTxTypeDef UARTx, uint8_t *string) 
{
    assert_param(IS_UARTX(UARTx));

    while ('\0' != *string) {
        SendByte(UARTx, *string);
        string++;
    }
}

static void xconsole_output(const uint8_t *str) 
{
#ifdef  DEVICE_UART1
    SendString(UART1, str);
#else
    (void *) str;
#endif
}

void xprintf(const char *fmt, ...) reentrant
{
    va_list args;
    uint32_t length;

    static char xlog_buf[XCONSOLEBUF_SIZE];

    va_start(args, fmt);
    length = vsprintf(xlog_buf, fmt, args);

    if (length > XCONSOLEBUF_SIZE - 1)
        length = XCONSOLEBUF_SIZE - 1;

    xconsole_output(xlog_buf);

    va_end(args);
}

uint8_t UARTx_Inited(UARTxTypeDef UARTx) 
{
    return UARTx_is_Inited[UARTx];
}

void UART1_conifg(void) 
{
    UART_InitTypeDef UART_InitStructure;

    UART_InitStructure.BaudRateTimer = BAUD_RATE_TIMER1;
    UART_InitStructure.UART_Mode = UART12_8bit_BRTx;
    UART_InitStructure.UART_BaudRate = 460800UL;
    UART_InitStructure.UART_RxEnable = ENABLE;

    UART1_SW(UART1_SW_P30_P31);
    UART_Init(UART1, &UART_InitStructure);
    UARTX_InitStruct[UART1] = UART_InitStructure;
    NVIC_UART1_Init(ENABLE,Priority_1);
}

void UART2_conifg(void) 
{
    UART_InitTypeDef UART_InitStructure;

    UART_InitStructure.BaudRateTimer = BAUD_RATE_TIMER2;
    UART_InitStructure.UART_Mode = UART12_8bit_BRTx;
    UART_InitStructure.UART_BaudRate = USE_BAUDRETE_2M_BPS;
    UART_InitStructure.UART_RxEnable = ENABLE;

    UART2_SW(UART2_SW_P46_P47);
    UART_Init(UART2, &UART_InitStructure);
    UARTX_InitStruct[UART2] = UART_InitStructure;
    NVIC_UART2_Init(ENABLE, Priority_1);
}

void UART3_conifg(void) 
{
    UART_InitTypeDef UART_InitStructure;

    UART_InitStructure.BaudRateTimer = BAUD_RATE_TIMER2;
    UART_InitStructure.UART_Mode = UART34_8bit_BRTx;
    UART_InitStructure.UART_BaudRate = USE_BAUDRETE_2M_BPS;
    UART_InitStructure.UART_RxEnable = ENABLE;

    UART3_SW(UART3_SW_P50_P51);
    UART_Init(UART3, &UART_InitStructure);
    UARTX_InitStruct[UART3] = UART_InitStructure;
    NVIC_UART3_Init(ENABLE, Priority_1);
}

void UART4_conifg(void) 
{
    UART_InitTypeDef UART_InitStructure;

    UART_InitStructure.BaudRateTimer = BAUD_RATE_TIMER2;
    UART_InitStructure.UART_Mode = UART34_8bit_BRTx;
    UART_InitStructure.UART_BaudRate = USE_BAUDRETE_2M_BPS;
    UART_InitStructure.UART_RxEnable = ENABLE;

    UART4_SW(UART4_SW_P52_P53);
    UART_Init(UART4, &UART_InitStructure);
    UARTX_InitStruct[UART4] = UART_InitStructure;
    NVIC_UART4_Init(ENABLE, Priority_1);
}
