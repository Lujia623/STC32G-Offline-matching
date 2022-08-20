#include "app_uart.h"

static sCommonRingBuf CommonRingBuf[COMMON_UART_MAX];

void CommonInit(eCommon_UARTx eCOMM_UARTx) 
{
    sCommonRingBuf *pbuf = NULL;

    pbuf->is_inited = true;
    pbuf = &CommonRingBuf[eCOMM_UARTx];
    pbuf->RingBufTxCtr = 0;
    memset(&pbuf->RingBufTx[0], 0, sizeof(pbuf->RingBufTx));
    pbuf->RingBufTxInPtr = &pbuf->RingBufTx[0];
    pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];

    pbuf->RingBufRxCtr = 0;
    memset(&pbuf->RingBufRx[0], 0, sizeof(pbuf->RingBufRx));
    pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
    pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
}

void CommonConfigAll(void) 
{
    CommonInit(COMMON_UART1);
    CommonInit(COMMON_UART2);
    CommonInit(COMMON_UART3);
    CommonInit(COMMON_UART4);
}

uint8_t CommonTx_put_char(eCommon_UARTx eCOMM_UARTx, uint8_t byte) 
{
    sCommonRingBuf *pbuf = NULL;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        LOG_E("%s:\nCommonTx_put_char:\t%d", __FILE__, __LINE__);
        return COMMON_INV_CH_ERROR;
    }

    pbuf = &CommonRingBuf[eCOMM_UARTx];
    pbuf->RingBufTxCtr++;
    *pbuf->RingBufTxInPtr++ = byte;
    if (pbuf->RingBufTxInPtr == &pbuf->RingBufTx[COMMON_TX_BUF_SIZE - 1]) {
        pbuf->RingBufTxInPtr = &pbuf->RingBufTx[0];
    }

    return COMMON_NO_ERROR;
}

static uint8_t CommonTx_put_data(eCommon_UARTx eCOMM_UARTx, const uint8_t *databuf, uint16_t data_len) 
{
    uint32_t i = 0;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        LOG_E("%s:\nCommonTx_put_data:\t%d", __FILE__, __LINE__);
        return COMMON_INV_CH_ERROR;
    }

    if (data_len >= COMMON_TX_BUF_SIZE) {
        LOG_E("%s:\nCommonTx_put_data:\t%d", __FILE__, __LINE__);
        return COMMON_TX_BUF_FULL;
    }

    if (databuf && (data_len > 0)) {
        for (i = 0; i < data_len; i++) {
            CommonTx_put_char(eCOMM_UARTx, databuf[i]);
        }
        return COMMON_NO_ERROR;
    } else {
        LOG_E("%s:\nCommonTx_put_data:\t%d", __FILE__, __LINE__);
        return COMMON_PARAM_INV;
    }
}

uint8_t getCommonTx_char(eCommon_UARTx eCOMM_UARTx, uint8_t *error) 
{
    sCommonRingBuf *pbuf = NULL;
    uint8_t ichar = 0x00;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        *error = COMMON_INV_CH_ERROR;
        LOG_E("%s:\ngetCommonTx_char:\t%d", __FILE__, __LINE__);
        return (NUL);
    }

    pbuf = &CommonRingBuf[eCOMM_UARTx];

    if (pbuf->RingBufTxCtr > 0) {
        pbuf->RingBufTxCtr--;
        ichar = *pbuf->RingBufTxOutPtr++;
        if (pbuf->RingBufTxOutPtr == &pbuf->RingBufTx[COMMON_TX_BUF_SIZE - 1]) {
            pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];
        }
        *error = COMMON_NO_ERROR;
        return (ichar);
    } else {
        *error = COMMON_TX_BUF_EMPTY;
        LOG_E("%s:\ngetCommonTx_char:\t%d", __FILE__, __LINE__);
        return (NUL);
    }
}

uint8_t ClearCommonRxBuf(eCommon_UARTx eCOMM_UARTx) 
{
    sCommonRingBuf *pbuf = NULL;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        LOG_E("%s:\nClearCommonRxBuf:\t%d", __FILE__, __LINE__);
        return COMMON_INV_CH_ERROR;
    }

    pbuf = &CommonRingBuf[eCOMM_UARTx];

    memset(pbuf->RingBufRx, 0, sizeof(pbuf->RingBufRx));
    pbuf->RingBufRxCtr = 0;
    pbuf->RingBufRxInPtr = pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];

    return COMMON_NO_ERROR;
}

uint8_t ClearCommonTxBuf(eCommon_UARTx eCOMM_UARTx) 
{
    sCommonRingBuf *pbuf = NULL;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        LOG_E("%s:\nClearCommonTxBuf:\t%d", __FILE__, __LINE__);
        return COMMON_INV_CH_ERROR;
    }

    pbuf = &CommonRingBuf[eCOMM_UARTx];

    memset(pbuf->RingBufTx, 0, sizeof(pbuf->RingBufTx));
    pbuf->RingBufTxCtr = 0;
    pbuf->RingBufTxInPtr = pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];

    return COMMON_NO_ERROR;
}

static uint8_t CommonTxBuf_is_Empty(eCommon_UARTx eCOMM_UARTx) 
{
    sCommonRingBuf *pbuf = NULL;
    uint8_t is_empty = false;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        LOG_E("%s:\nCommonTxBuf_is_Empty:\t%d", __FILE__, __LINE__);
        return false;
    }

    pbuf = &CommonRingBuf[eCOMM_UARTx];
    if (pbuf->RingBufTxCtr > 0) {
        is_empty = false;
    } else {
        is_empty = true;
    }
    return is_empty;
}

void CommonSenddata(eCommon_UARTx eCOMM_UARTx, uint8_t *databuf, uint16_t data_len) 
{
    sCommonRingBuf *pbuf = NULL;
    uint8_t error = COMMON_NO_ERROR;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        error = COMMON_INV_CH_ERROR;
        LOG_E("%s:\nCommonSenddata:\t%d", __FILE__, __LINE__);
        return;
    }

    error = CommonTx_put_data(eCOMM_UARTx, databuf, data_len);
    if (COMMON_NO_ERROR != error)
        LOG_E("%s:\nCommonSenddata:\t%d", __FILE__, __LINE__);

    while (!CommonTxBuf_is_Empty(eCOMM_UARTx)) {
        SendByte((UARTxTypeDef)(eCOMM_UARTx + 1), getCommonTx_char(eCOMM_UARTx, &error));
        if (COMMON_NO_ERROR != error)
            LOG_E("%s:\nCommonSenddata:\t%d", __FILE__, __LINE__);
    }
}

static uint8_t CommonRxBuf_is_Empty(eCommon_UARTx eCOMM_UARTx) 
{
    sCommonRingBuf *pbuf = NULL;
    uint8_t is_empty = false;

    pbuf = &CommonRingBuf[eCOMM_UARTx];

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        LOG_E("%s:\nCommonRxBuf_is_Empty:\t%d", __FILE__, __LINE__);
        return false;
    }

    if (pbuf->RingBufRxCtr > 0) {
        is_empty = false;
    } else {
        is_empty = true;
    }
    return is_empty;
}

uint8_t CommonRx_put_char(eCommon_UARTx eCOMM_UARTx, uint8_t byte) 
{
    sCommonRingBuf *pbuf = NULL;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        LOG_E("%s:\nCommonRx_put_char:\t%d", __FILE__, __LINE__);
        return COMMON_INV_CH_ERROR;
    }

    pbuf = &CommonRingBuf[eCOMM_UARTx];
    pbuf->RingBufRxCtr++;
    *pbuf->RingBufRxInPtr++ = byte;
    if (pbuf->RingBufRxInPtr == &pbuf->RingBufRx[COMMON_RX_BUF_SIZE - 1]) {
        pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
    }
    return COMMON_NO_ERROR;
}

uint8_t getCommonRx_char(eCommon_UARTx eCOMM_UARTx, uint8_t *error) 
{
    sCommonRingBuf *pbuf = NULL;
    uint8_t ichar = 0;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        *error = COMMON_INV_CH_ERROR;
        LOG_E("%s:\ngetCommonRx_char:\t%d", __FILE__, __LINE__);
        return (NUL);
    }

    pbuf = &CommonRingBuf[eCOMM_UARTx];

    if (pbuf->RingBufRxCtr > 0) {
        //LOG_D("RingBufRxCtr:%0x,RingBufRxOutPtr:%p",pbuf->RingBufRxCtr,pbuf->RingBufRxOutPtr);
        pbuf->RingBufRxCtr--;
        ichar = *pbuf->RingBufRxOutPtr;
        pbuf->RingBufRxOutPtr++;
        //LOG_D("ichar:%0x\r\n",ichar);
        if (pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[COMMON_RX_BUF_SIZE - 1]) {
            pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
        }
        *error = COMMON_NO_ERROR;
        return (ichar);
    } else {
        *error = COMMON_RX_BUF_EMPTY;
        return (NUL);
    }
}

uint32_t getCommonRxCtr(eCommon_UARTx eCOMM_UARTx, uint8_t *error) 
{
    sCommonRingBuf *pbuf = NULL;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        *error = COMMON_INV_CH_ERROR;
        LOG_E("%s:\ngetCommonRx_data:\t%d", __FILE__, __LINE__);
        return (NUL);
    }

    pbuf = &CommonRingBuf[eCOMM_UARTx];
    return pbuf->RingBufRxCtr;

}

uint8_t getCommonRx_data(eCommon_UARTx eCOMM_UARTx, uint8_t *src_buf, uint16_t buf_len) 
{
    sCommonRingBuf *pbuf = NULL;
    uint8_t error = COMMON_NO_ERROR;

    if (eCOMM_UARTx >= COMMON_UARTX_MAX) {
        error = COMMON_INV_CH_ERROR;
        LOG_E("%s:\ngetCommonRx_data:\t%d", __FILE__, __LINE__);
        return (NUL);
    }

    pbuf = &CommonRingBuf[eCOMM_UARTx];

    if (src_buf && (buf_len >= pbuf->RingBufRxCtr)) {
        while ((pbuf->RingBufRxCtr) > 0) {
            *src_buf = getCommonRx_char(eCOMM_UARTx, &error);
            src_buf++;
            if (COMMON_NO_ERROR != error)
                LOG_E("%s:\ngetCommonRx_data:\t%d", __FILE__, __LINE__);
        }

    }
    return (NUL);
}
