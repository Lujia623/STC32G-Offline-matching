#ifndef __APP_UART_H_
#define __APP_UART_H_

#include "BSP_STC32G_system.h"
#include "BSP_STC32G_UART.h"

//#pragma pack(4)

#ifndef NUL
#define NUL                             0x00
#endif

#define COMMON_TX_BUF_SIZE              256
#define COMMON_RX_BUF_SIZE              256

#define COMMON_UART_MAX                 4

#define COMMON_NO_ERROR           0     //no error
#define COMMON_INV_CH_ERROR       1     //invalid channel
#define COMMON_TX_BUF_EMPTY       2     //tx buf is empty
#define COMMON_TX_BUF_FULL        3     //tx buf is full
#define COMMON_RX_BUF_EMPTY       4     //Rx buf is empty
#define COMMON_RX_BUF_FULL        5     //Rx buf is full
#define COMMON_PARAM_INV          6     //param invalid
#define COMMON_TX_COMPLETE        7     //tx complete

typedef enum {
    COMMON_UART1,
    COMMON_UART2,
    COMMON_UART3,
    COMMON_UART4,
    COMMON_UARTX_MAX
} eCommon_UARTx;

typedef struct CommonRingBuf {
    uint8_t is_inited;
    uint32_t RingBufTxCtr;
    uint8_t *RingBufTxInPtr;
    uint8_t *RingBufTxOutPtr;
    uint8_t RingBufTx[COMMON_TX_BUF_SIZE];

    uint32_t RingBufRxCtr;
    uint8_t *RingBufRxInPtr;
    uint8_t *RingBufRxOutPtr;
    uint8_t RingBufRx[COMMON_RX_BUF_SIZE];
} sCommonRingBuf;

void CommonInit(eCommon_UARTx eCOMM_UARTx);

void CommonConfigAll(void);

uint8_t ClearCommonRxBuf(eCommon_UARTx eCOMM_UARTx);

uint8_t ClearCommonTxBuf(eCommon_UARTx eCOMM_UARTx);

uint8_t CommonTx_put_char(eCommon_UARTx eCOMM_UARTx, uint8_t byte);

uint8_t getCommonTx_char(eCommon_UARTx eCOMM_UARTx, uint8_t *error);

uint8_t CommonSenddata(eCommon_UARTx eCOMM_UARTx, const uint8_t *databuf, uint16_t data_len);

uint32_t getCommonRxCtr(eCommon_UARTx eCOMM_UARTx, uint8_t *error);

uint8_t CommonRx_put_char(eCommon_UARTx eCOMM_UARTx, uint8_t byte);

uint8_t getCommonRx_char(eCommon_UARTx eCOMM_UARTx, uint8_t *error);

uint8_t getCommonRx_data(eCommon_UARTx eCOMM_UARTx, uint8_t *src_buf, uint16_t buf_len);

#endif /*__APP_UART_H_*/
