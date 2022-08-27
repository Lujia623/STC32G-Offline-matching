#ifndef __BSP_STC32G_UART_H_
#define __BSP_STC32G_UART_H_

#include "BSP_STC32G_system.h"
#include "stdarg.h"

#define        CLR_TI2()            S2TI = 0            
#define        CLR_RI2()            S2RI = 0            
#define        CLR_TI3()            S3TI = 0            
#define        CLR_RI3()            S3RI = 0            
#define        CLR_TI4()            S4TI = 0            
#define        CLR_RI4()            S4RI = 0            

#define     XCONSOLEBUF_SIZE    128
#define     DEVICE_UART1

typedef enum {
    UART1 = 1,
    UART2,
    UART3,
    UART4,
    UART_MAX
} UARTxTypeDef;

typedef enum {
    /*串口1/2*/
    UART12_ShiftRight = 0x00,
    UART12_8bit_BRTx = 0x40,
    UART12_9bit = 0x80,
    UART12_9bit_BRTx = 0xC0,
    /*串口3/4*/
    UART34_8bit_BRTx = 0x00,
    UART34_9bit_BRTx = 0x80
} UART_ModeDef;

typedef enum {
    BAUD_RATE_TIMER1,
    BAUD_RATE_TIMER2,
    BAUD_RATE_TIMER3,
    BAUD_RATE_TIMER4
} BAUD_RATE_TimerDef;

typedef struct UART_InitStructure {
    UART_ModeDef UART_Mode;                 //模式
    BAUD_RATE_TimerDef BaudRateTimer;       //波特率发生器
    uint32_t UART_BaudRate;                 //波特率
    uint8_t Morecommunicate;                //多机通信
    uint8_t UART_RxEnable;                  //接收使能,   ENABLE,DISABLE
    uint8_t BaudRateDouble;                 //波特率倍增, ENABLE,DISABLE
} UART_InitTypeDef;

#define USE_BAUDRETE_2M_BPS             2000000UL
#define USE_BAUDRETE_115200_BPS         115200UL
#define USE_BAUDRETE_38400_BPS          38400UL
#define USE_BAUDRETE_9600_BPS           9600UL

/**
 * @brief UARTx
 * 
 */
#define IS_UARTX(UARTX)                ((UART1 == UARTX) ||\
                                        (UART2 == UARTX) ||\
                                        (UART3 == UARTX) ||\
                                        (UART4 == UARTX))

/**
 * @brief UARTx mode
 * 
 */
#define IS_UARTX_MODE(mode)            ((mode == UART12_ShiftRight) || \
                                       (mode == UART12_8bit_BRTx) || \
                                        (mode == UART12_9bit) ||\
                                        (mode == UART12_9bit_BRTx) ||\
                                        (mode == UART34_8bit_BRTx) ||\
                                        (mode == UART34_9bit_BRTx))

/**
 * @brief baud rate generator
 * 
 */
#define IS_BAUD_RATE_TIMER(timer)       ((BAUD_RATE_TIMER1 == timer) ||\
                                        (BAUD_RATE_TIMER2 == timer) ||\
                                        (BAUD_RATE_TIMER3 == timer) ||\
                                        (BAUD_RATE_TIMER4 == timer))


void UART_Init(UARTxTypeDef UARTx, UART_InitTypeDef *UART_InitStruct);

uint8_t UARTx_Inited(UARTxTypeDef UARTx);

void UART1_conifg(void);

void UART2_conifg(void);

void UART3_conifg(void);

void UART4_conifg(void);

void Receive_Ebable(UARTxTypeDef UARTx, uint8_t enable);

void SetUARTXBaudRate(UARTxTypeDef UARTx, uint32_t baudrate);

void SendByte(UARTxTypeDef UARTx, uint8_t byte);

uint8_t ReceiveByte(UARTxTypeDef UARTx);

void SendString(UARTxTypeDef UARTx, uint8_t *string);

void xprintf(const char *fmt, ...)reentrant;

#endif /*__BSP_STC32G_UART_H_*/
