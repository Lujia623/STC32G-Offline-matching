#include "app_cli.h"
#include "app_uart.h"
#include "BSP_STC32G_TIMER.h"
#include "crc.h"

static uint8_t addrBuf[UART_DEVICE_MAX][CLI_PAYLOAD_LENGTH];
static uint8_t revBuf[CLI_PAYLOAD_LENGTH];
static const uint8_t exit_sleep[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00}; // FFFFFFFFFFFFFFFF00

static int8_t data_analysis(eCommon_UARTx eCOMM_UARTx, uint8_t *rev_buf, uint8_t *buffer, \
                            uint16_t *buffer_len, uint16_t *module_id, uint16_t *msg_id);

static void CLISend(eCommon_UARTx eCOMM_UARTx, uint8_t *buffer, uint16_t buffer_len, \
                    CLI_MODULEID module_id, app_cli_msg_id_e msg_id) 
{
    uint16_t i, length, sendbuf_length;
    uint8_t xdata *sendbuf = NULL;
    scli_Command command;
    uint32_t preamble = GTP_PREAMBLE2;
    static uint16_t msg_sn = 0;

    sendbuf = (uint8_t *) xmalloc(sizeof(scli_Command) + sizeof(buffer_len) + buffer_len + \
                                  GTP_PREAMBLE_LENGTH + CLI_SN_LENGTH + CLI_START_LENGTH + CLI_TAIL_LENGTH);

    if (NULL == sendbuf) {
        LOG_E("%s:%d\r\n", __FILE__, __LINE__);
        return;
    }

    xmemset(sendbuf, 0, sizeof(scli_Command) + sizeof(buffer_len) + buffer_len + GTP_PREAMBLE_LENGTH + CLI_SN_LENGTH + \
                        CLI_START_LENGTH + CLI_TAIL_LENGTH);

    // xprintf("%d\r\n",sizeof(scli_Command) + sizeof(buffer_len) + 10 + buffer_len);

    xprintf("msg_sn:%d\r\n", msg_sn);

    sendbuf[0] = ((GTP_PREAMBLE >> 0) & 0xff);
    sendbuf[1] = ((GTP_PREAMBLE >> 8) & 0xff);
    sendbuf[2] = ((GTP_PREAMBLE >> 16) & 0xff);
    sendbuf[3] = ((GTP_PREAMBLE >> 24) & 0xff);
    command.gtp_header.version = GTP_VERSION;
    xprintf("command.gtp_header.version:%0x\r\n", command.gtp_header.version);
    length = sizeof(scli_Command) + sizeof(buffer_len) + CLI_SN_LENGTH + \
             CLI_START_LENGTH + CLI_TAIL_LENGTH + buffer_len;
    length = ((length & 0xff) << 8) | ((length >> 8) & 0xff);
    command.gtp_header.length = length;
    command.gtp_header.type = GTP_TYPE;
    command.gtp_header.FC = GTP_FC;
    command.gtp_header.seq = GTP_SEQ;
    // xprintf("gtp_header:%d\r\n",sizeof(command.gtp_header));
    memcpy(&sendbuf[4], &command.gtp_header, sizeof(command.gtp_header));

    command.CRC8 = getcrc8(&sendbuf[4], sizeof(command.gtp_header));
    sendbuf[3 + sizeof(command.gtp_header) + 1] = command.CRC8;

    sendbuf[3 + sizeof(command.gtp_header) + 2] = (CLI_START >> 8) & 0xff;
    sendbuf[3 + sizeof(command.gtp_header) + 3] = (CLI_START >> 0) & 0xff;
    command.cli_Msg.module_id = ((module_id & 0xff) << 8) | ((module_id >> 8) & 0xff);
    command.cli_Msg.CRC = ((getcrc32(buffer, buffer_len) & 0xff) << 8) | ((getcrc32(buffer, buffer_len) >> 8) & 0xff);
    command.cli_Msg.message_id = ((msg_id & 0xff) << 8) | ((msg_id >> 8) & 0xff);
    command.cli_Msg.ACK = CLI_NO_ACK;
    command.cli_Msg.cli_type = CLI_TYPE_CMD;
    command.cli_Msg.Reversed = 0x00;
    command.cli_Msg.result = 0x00;
    memcpy(&sendbuf[3 + sizeof(command.gtp_header) + 4], &command.cli_Msg, sizeof(command.cli_Msg));
    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 4] = (buffer_len >> 0) & 0xff;
    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 5] = (buffer_len >> 8) & 0xff;
    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 6] = (msg_sn >> 8) & 0xff;
    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 7] = (msg_sn >> 0) & 0xff;
    memcpy((uint8_t * ) & sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 8], (uint8_t *) buffer, buffer_len);
    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 8 + buffer_len] = (CLI_TAIL >> 8) & 0xff;
    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 9 + buffer_len] = (CLI_TAIL >> 0) & 0xff;

    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 10 + buffer_len] = \
            (getcrc32(&sendbuf[4], 3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 6 + buffer_len) >> 0) & 0xff;
    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 11 + buffer_len] = \
            (getcrc32(&sendbuf[4], 3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 6 + buffer_len) >> 8) & 0xff;
    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 12 + buffer_len] = \
            (getcrc32(&sendbuf[4], 3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 6 + buffer_len) >> 16) & 0xff;
    sendbuf[3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 13 + buffer_len] = \
            (getcrc32(&sendbuf[4], 3 + sizeof(command.gtp_header) + sizeof(command.cli_Msg) + 6 + buffer_len) >> 24) & 0xff;

    xprintf("Send CLI Start:\r\n");

    sendbuf_length = sizeof(scli_Command) + sizeof(buffer_len) + buffer_len + \
                     GTP_PREAMBLE_LENGTH + CLI_SN_LENGTH + CLI_START_LENGTH + CLI_TAIL_LENGTH;

    for (i = 0; i < sendbuf_length; i++)
        xprintf("%0x\t", sendbuf[i]);

    ClearCommonTxBuf(eCOMM_UARTx);
    ClearCommonRxBuf(eCOMM_UARTx);
    CommonSenddata(eCOMM_UARTx, sendbuf, sendbuf_length);
    msg_sn++;
    if (msg_sn >= 65535)
        msg_sn = 0;

    if (NULL != sendbuf) {
        xfree(sendbuf); //只分配了2k字节
    }
    sendbuf = NULL;
    // LOG_E("end%d",5);
    xprintf("\r\nSend CLI End\r\n");
    // LOG_E("%s:\nCommonSenddata:\t%d",__FILE__,__LINE__);
}

static int8_t CLIReceive(eCommon_UARTx eCOMM_UARTx, uint8_t *buffer, uint16_t *buffer_len, uint16_t *module_id, uint16_t *msg_id) 
{
    int8_t error = CLI_REVDATA_OK;
    uint8_t i = 0;
    uint8_t clibuf[128] = {0};

    getCommonRx_data(eCOMM_UARTx, clibuf, sizeof(clibuf));

    while ((i < sizeof(clibuf))) {
        if (0xD0 == clibuf[i] && 0xD2 == clibuf[i + 1] && 0xC5 == clibuf[i + 2] && 0xC2 == clibuf[i + 3]) {
            error = data_analysis(eCOMM_UARTx, (uint8_t * ) & clibuf[i + GTP_PREAMBLE_LENGTH], (uint8_t *) buffer, \
                                  (uint16_t *) buffer_len, module_id, msg_id);
            xprintf("CLIReceive error:%d\r\n", error);

            return error;
        }
        i++;
    }

    return CLI_REVDATA_ERROR;
}

static int8_t data_analysis(eCommon_UARTx eCOMM_UARTx, uint8_t *rev_buf, uint8_t *buffer, uint16_t *buffer_len, uint16_t *module_id, uint16_t *msg_id) 
{
    uint16_t crc32length = 0;
    uint8_t i = 0;
    uint8_t *buf = buffer;
    uint16_t length = 0;
    scli_Command rev_command;

    xprintf("data_analysis rev_buf:\r\n");
    for (i = 0; i < 50; i++)
        xprintf("%0x\t", rev_buf[i]);
    xprintf("\r\n");

    memset(&rev_command, 0, sizeof(rev_command));
    memcpy((uint8_t * ) & rev_command.gtp_header, (uint8_t *) rev_buf, sizeof(rev_command.gtp_header));

    if (rev_buf[sizeof(rev_command.gtp_header)] == getcrc8(&rev_command.gtp_header, sizeof(rev_command.gtp_header))) {
        rev_command.CRC8 = rev_buf[sizeof(rev_command.gtp_header)];
        xprintf("rev_command.CRC8:%0x\r\n", rev_command.CRC8);
    } else {
        LOG_E("%s:\n%d:\tGTP_HEADER_CRC8_ERROR\r\n", __FILE__, __LINE__);
        return (int8_t) GTP_HEADER_CRC8_ERROR;
    }
    if ((rev_buf[sizeof(rev_command.gtp_header) + 1] == ((CLI_START >> 8) & 0xff)) && \
        (rev_buf[sizeof(rev_command.gtp_header) + 2] == ((CLI_START >> 0) & 0xff))) {
        memcpy((uint8_t * ) & rev_command.cli_Msg, (uint8_t *) rev_buf[sizeof(rev_command.gtp_header) + 3], \
               sizeof(rev_command.cli_Msg));

        (*module_id) = (rev_buf[sizeof(rev_command.gtp_header) + 4] << 8) | rev_buf[sizeof(rev_command.gtp_header) + 3];
        (*msg_id) = (rev_buf[sizeof(rev_command.gtp_header) + 8] << 8) | rev_buf[sizeof(rev_command.gtp_header) + 7];

        length = (uint16_t)((uint16_t) rev_buf[sizeof(rev_command.gtp_header) + CLI_START_LENGTH + sizeof(rev_command.cli_Msg) + 2]<< 8) | \
                            ((uint16_t) rev_buf[sizeof(rev_command.gtp_header) + CLI_START_LENGTH + sizeof(rev_command.cli_Msg) +1]);


        if (buf == NULL) {
            LOG_E("%s:\n%d\r\n", __FILE__, __LINE__);
            return (int8_t) POOL_MALLOC_ERROR;
        }
        if (length != 0) {
            memset((uint8_t *) buf, 0, length);
            memcpy((uint8_t *) buf, (uint8_t * ) & rev_buf[sizeof(rev_command.gtp_header) + CLI_START_LENGTH + sizeof(rev_command.cli_Msg) + CLI_SN_LENGTH + 3], length);
        }
    } else {
        LOG_E("%s:\n%d:\tCLI_START_ERROR\r\n", __FILE__, __LINE__);
        return (int8_t) CLI_START_ERROR;
    }

    crc32length = sizeof(rev_command.gtp_header) + sizeof(rev_command.cli_Msg) + sizeof(rev_command.CRC8) + sizeof(uint16_t) + \
                    CLI_START_LENGTH + CLI_SN_LENGTH + CLI_TAIL_LENGTH + length;
    xprintf("crclength:%0x\r\n", crc32length);

    rev_command.CRC32 = (uint32_t)((((uint32_t) rev_buf[crc32length + 3] << 24)) | (((uint32_t) rev_buf[crc32length + 2] << 16))) | \
                                    (((uint32_t) rev_buf[crc32length + 1] << 8)) | (((uint32_t) rev_buf[crc32length + 0] << 0));

    xprintf("rev_command.CRC32:%ld\r\n", (uint32_t) rev_command.CRC32);
    xprintf("rev_buf getcrc32:%ld\r\n", (uint32_t) getcrc32(rev_buf, crc32length));

    if ((uint32_t) getcrc32(rev_buf, crc32length) == (uint32_t) rev_command.CRC32) {
        xprintf("%s:\n:%d:CLI_REVDATA_OK\r\n", __FILE__, __LINE__);
    } else {
        return (int8_t) CLI_REVDATA_ERROR;
    }

    *buffer_len = length;

    return (int8_t) CLI_REVDATA_OK;
}

////TODO:交叉写地址
uint8_t CLI_IntersectAddr(eCommon_UARTx eCOMM_UARTx, eDeviceTypedef eDevice, uint8_t *setAddr, uint8_t *rev_buf) 
{
    uint16_t count = 0;
    uint8_t i = 0;
    int8_t error = CLI_REVDATA_OK;
    uint16_t buffer_len = 0;
    uint16_t module_id = 0, msg_id = 0;
    uint8_t result = 0;

    if ((COMMON_UART2 == eCOMM_UARTx) && (UART_MASTER == eDevice)) {
        LOG_E("%s:\nCLI_IntersectAddr:%d\r\n", __FILE__, __LINE__);
        return;
    } else if (((COMMON_UART3 == eCOMM_UARTx) || (COMMON_UART4 == eCOMM_UARTx)) && \
               ((UART_SLAVE1 == eDevice) || (UART_SLAVE2 == eDevice))) {
        LOG_E("%s:\nCLI_IntersectAddr:%d\r\n", __FILE__, __LINE__);
        return;
    }

    TIMER_START(TIMER3);

    do {
        if(TIMER_COMPARE(TIMER3, CLI_SEND_TIMEOUT)) {
            xprintf("CLI_IntersectAddr timeout:%f\r\n", getTimerx(TIMER3));
            break;
        }

        CLISend(eCOMM_UARTx, (uint8_t *) exit_sleep, sizeof(exit_sleep), CLI_MODULEID_LOWPOWER, CLI_EXIT_SLEEP);
        delay_ms(10);
        error = CLIReceive(eCOMM_UARTx, rev_buf, &buffer_len, &module_id, &msg_id);
        // LOG_E("%s %d:\nCLI_IntersectAddr ERROR:%d,then retry\r\n",__FILE__,__LINE__,error);
        if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_LOWPOWER == module_id) && (CLI_EXIT_SLEEP == msg_id)) {
            for (i = 0; i < buffer_len; i++) {
                LOG_D("CLI_IntersectAddr rev_buf[%d]:%0x\r\n", i, rev_buf[i]);
            }

            LOG_D("CLI_IntersectAddr:addrBuf:%ld,buffer_len:%d,module_id:%d,msg_id:%d\r\n", setAddr, buffer_len,
                  module_id, msg_id);

            CLISend(eCOMM_UARTx, setAddr, ADDR_LENGTH, CLI_MODULEID_APPLICATION, APP_CLI_MSGID_LAP_SET_PEER_ADDR);
            delay_ms(10);
            error = CLIReceive(eCOMM_UARTx, rev_buf, &buffer_len, &module_id, &msg_id);
            if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_APPLICATION == module_id) &&
                (APP_CLI_MSGID_LAP_SET_PEER_ADDR == msg_id)) {
                for (i = 0; i < buffer_len; i++) {
                    LOG_D("CLI_IntersectAddr rev_buf[%d]:%0x\r\n", i, rev_buf[i]);
                }
                LOG_D("CLI_IntersectAddr :addrBuf:%ld,buffer_len:%d,module_id:%d,msg_id:%d\r\n", setAddr, buffer_len,
                      module_id, msg_id);
                result = CLI_DEVICEX_TRANSFER_SUCCESS(eDevice);
                return result;
            }
        } else {
            LOG_D("CLI_IntersectAddr retry:%d\r\n", count++);
        }

    } while (1);

    TIMER_END(TIMER3);

    return CLI_TIMEOUT;
}

uint8_t CLIGetDeviceAddr(eCommon_UARTx eCOMM_UARTx, eDeviceTypedef eDevice) 
{
    uint8_t count = CLI_RETRY_TIMES;
    uint8_t i = 0;
    int8_t error = CLI_REVDATA_OK;
    uint16_t buffer_len = 0;
    uint16_t module_id = 0, msg_id = 0;
    uint8_t result = 0;

    TIMER_START(TIMER3);

    do {
        if (TIMER_COMPARE(TIMER3, CLI_SEND_TIMEOUT)) {
            xprintf("CLIGetDeviceAddr timeout:%f\r\n", getTimerx(TIMER3));
            break;
        }

        CLISend(eCOMM_UARTx, (uint8_t *) exit_sleep, sizeof(exit_sleep), CLI_MODULEID_LOWPOWER, CLI_EXIT_SLEEP);
        delay_ms(10);
        error = CLIReceive(eCOMM_UARTx, revBuf, &buffer_len, &module_id, &msg_id);

        if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_LOWPOWER == module_id) && (CLI_EXIT_SLEEP == msg_id)) {
            for (i = 0; i < buffer_len; i++) {
                LOG_D("CLIGetDeviceAddr addrBuf[%d]:%0x\r\n", i, addrBuf[eDevice][i]);
            }
            LOG_D("CLIGetDeviceAddr :addrBuf:%ld,buffer_len:%d,module_id:%d,msg_id:%d\r\n", addrBuf[eDevice],
                  buffer_len, module_id, msg_id);

            CLISend(eCOMM_UARTx, NULL, 0, CLI_MODULEID_APPLICATION, APP_CLI_MSGID_GET_BDADDR);
            delay_ms(10);
            error = CLIReceive(eCOMM_UARTx, addrBuf[eDevice], &buffer_len, &module_id, &msg_id);
            if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_APPLICATION == module_id) && (APP_CLI_MSGID_GET_BDADDR == msg_id)) {
                for (i = 0; i < buffer_len; i++) {
                    LOG_D("CLIGetDeviceAddr addrBuf[%d]:%0x\r\n", i, addrBuf[eDevice][i]);
                }
                LOG_D("CLIGetDeviceAddr :addrBuf:%ld,buffer_len:%d,module_id:%d,msg_id:%d\r\n", addrBuf[eDevice], \
                      buffer_len, module_id, msg_id);
                result = CLI_DEVICEX_TRANSFER_SUCCESS(eDevice);
                return result;
            }
        } else {
            LOG_D("CLIGetDeviceAddr retry:%d\r\n", count++);
        }

    } while (1);

    TIMER_END(TIMER3);

    return CLI_TIMEOUT;
}

uint8_t CLI_EVTUSR_REBOOT(eCommon_UARTx eCOMM_UARTx) 
{
    uint16_t count = CLI_RETRY_TIMES;
    uint8_t i = 0;
    int8_t error = CLI_REVDATA_OK;
    uint8_t CMD_buf[] = {0x20, 0x04};
    uint16_t buffer_len = 0;
    uint16_t module_id = 0, msg_id = 0;

    do {
        CLISend(eCOMM_UARTx, (uint8_t *) exit_sleep, sizeof(exit_sleep), CLI_MODULEID_LOWPOWER, CLI_EXIT_SLEEP);
        delay_ms(10);
        error = CLIReceive(eCOMM_UARTx, revBuf, &buffer_len, &module_id, &msg_id);
        if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_LOWPOWER == module_id) && (CLI_EXIT_SLEEP == msg_id)) {

            CLISend(eCOMM_UARTx, (uint8_t *) CMD_buf, sizeof(CMD_buf), CLI_MODULEID_APPLICATION, APP_CLI_MSGID_SEND_USER_EVENT);
            delay_ms(10);
            error = CLIReceive(eCOMM_UARTx, revBuf, &buffer_len, &module_id, &msg_id);
            if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_APPLICATION == module_id) && (APP_CLI_MSGID_SEND_USER_EVENT == msg_id)) {
                LOG_D("CLI_EVTUSR_REBOOT SUCCESS!\r\n", 0);
                return CLI_REVDATA_OK;
            }
        } else {
            LOG_D("CLI_EVTUSR_REBOOT retry:%d\r\n", count);
        }

    } while (count--);

    return CLI_TIMEOUT;
}

uint8_t CLI_Send_EVENT(eCommon_UARTx eCOMM_UARTx, uint16_t event) 
{
    uint16_t count = CLI_RETRY_TIMES;
    uint8_t i = 0;
    int8_t error = CLI_REVDATA_OK;
    uint8_t CMD_buf[] = {0x20, 0x04};
    uint16_t buffer_len = 0;
    uint16_t module_id = 0, msg_id = 0;

    do {
        CLISend(eCOMM_UARTx, (uint8_t *) exit_sleep, sizeof(exit_sleep), CLI_MODULEID_LOWPOWER, CLI_EXIT_SLEEP);
        delay_ms(10);
        error = CLIReceive(eCOMM_UARTx, revBuf, &buffer_len, &module_id, &msg_id);
        if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_LOWPOWER == module_id) && (CLI_EXIT_SLEEP == msg_id)) {

            CLISend(eCOMM_UARTx, (uint8_t *) CMD_buf, sizeof(CMD_buf), CLI_MODULEID_APPLICATION, APP_CLI_MSGID_SEND_USER_EVENT);
            delay_ms(10);
            error = CLIReceive(eCOMM_UARTx, revBuf, &buffer_len, &module_id, &msg_id);
            if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_APPLICATION == module_id) && (APP_CLI_MSGID_SEND_USER_EVENT == msg_id)) {
                LOG_D("CLI_EVTUSR_REBOOT SUCCESS!\r\n", 0);
                return CLI_REVDATA_OK;
            }
        } else {
            LOG_D("CLI_EVTUSR_REBOOT retry:%d\r\n", count);
        }

    } while (count--);

    return CLI_TIMEOUT;
}

uint8_t CLIGetLAPMode(eCommon_UARTx eCOMM_UARTx) 
{
    uint16_t count = CLI_RETRY_TIMES;
    uint8_t i = 0;
    uint8_t mode = 0;
    uint16_t buffer_len = 0;
    uint16_t module_id = 0, msg_id = 0;
    int8_t error = CLI_REVDATA_OK;

    do {
        CLISend(eCOMM_UARTx, (uint8_t *) exit_sleep, sizeof(exit_sleep), CLI_MODULEID_LOWPOWER, CLI_EXIT_SLEEP);
        delay_ms(10);
        error = CLIReceive(eCOMM_UARTx, revBuf, &buffer_len, &module_id, &msg_id);
        if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_LOWPOWER == module_id) && (CLI_EXIT_SLEEP == msg_id)) {
            CLISend(eCOMM_UARTx, NULL, 0, CLI_MODULEID_APPLICATION, APP_CLI_MSGID_GET_GTWS_MODE);
            delay_ms(10);
            error = CLIReceive(eCOMM_UARTx, &mode, &buffer_len, &module_id, &msg_id);
            if ((CLI_REVDATA_OK == error) && (CLI_MODULEID_APPLICATION == module_id) &&
                (APP_CLI_MSGID_GET_GTWS_MODE == msg_id)) {
                return mode;
            }
        }
    } while (count--);

    return CLI_TIMEOUT;
}

int8_t CLI_Process(void) 
{
    static uint32_t counter = 0;
    uint8_t i;
    uint8_t getAddr_result = 0, intersectAddr_result = 0, result = 0;

    xprintf("COMMON_UART2---------->UART_MASTER:START\r\n");
    getAddr_result |= CLIGetDeviceAddr(COMMON_UART2, UART_MASTER);
    xprintf("COMMON_UART2---------->UART_MASTER:END\tgetAddr_result:%0x\r\n", getAddr_result);

    xprintf("COMMON_UART3---------->UART_SLAVE1:START\r\n");
    getAddr_result |= CLIGetDeviceAddr(COMMON_UART3, UART_SLAVE1);
    xprintf("COMMON_UART3---------->UART_SLAVE1:END\tgetAddr_result:%0x\r\n", getAddr_result);

    xprintf("COMMON_UART4---------->UART_SLAVE2:START\r\n");
    getAddr_result |= CLIGetDeviceAddr(COMMON_UART4, UART_SLAVE2);
    xprintf("COMMON_UART4---------->UART_SLAVE2:END\tgetAddr_result:%0x\r\n", getAddr_result);

    if (getAddr_result & CLI_TIMEOUT) {
        LOG_E("GetAddr TIMEOUT :%0x\r\n", getAddr_result);
        return CLI_GET_ADDR_TIMEOUT;
    } else {
        xprintf("CLI_IntersectAddr START SLAVE1/SLAVE2----->MASTER:\r\n");
        intersectAddr_result |= CLI_IntersectAddr(COMMON_UART2, UART_SLAVE1, addrBuf[UART_SLAVE1], revBuf);
        xprintf("CLI_IntersectAddr SLAVE1:END\tintersectAddr_result:%0x\r\n", intersectAddr_result);
        intersectAddr_result |= CLI_IntersectAddr(COMMON_UART2, UART_SLAVE2, addrBuf[UART_SLAVE2], revBuf);
        xprintf("CLI_IntersectAddr SLAVE2:END\tintersectAddr_result:%0x\r\n", intersectAddr_result);

        xprintf("CLI_IntersectAddr MASTER----->SLAVE1 START:\r\n");
        intersectAddr_result |= CLI_IntersectAddr(COMMON_UART3, UART_MASTER, addrBuf[UART_MASTER], revBuf);
        xprintf("CLI_IntersectAddr:END\tintersectAddr_result:%0x\r\n", intersectAddr_result);

        xprintf("CLI_IntersectAddr MASTER----->SLAVE2 START:\r\n");
        intersectAddr_result |= CLI_IntersectAddr(COMMON_UART4, UART_MASTER, addrBuf[UART_MASTER], revBuf);
        xprintf("CLI_IntersectAddr:END\tintersectAddr_result:%0x\r\n", intersectAddr_result);

        if (intersectAddr_result & CLI_TIMEOUT) {
            LOG_E("intersectAddr TIMEOUT :%0x\r\n", intersectAddr_result);
            return CLI_INTERTECT_ADDR_TIMEOUT;
        }
    }

    for (i = 0; i < ADDR_LENGTH; i++) {
        LOG_D("MASTER:%0x\r\n", addrBuf[UART_MASTER][i]);
    }
    for (i = 0; i < ADDR_LENGTH; i++) {
        LOG_D("SLAV1:%0x\r\n", addrBuf[UART_SLAVE1][i]);
    }
    for (i = 0; i < ADDR_LENGTH; i++) {
        LOG_D("SLAV2:%0x\r\n", addrBuf[UART_SLAVE2][i]);
    }

    if ((0x07 == getAddr_result) && (0x07 == intersectAddr_result)) {
        result |= CLI_EVTUSR_REBOOT(COMMON_UART2);
        result |= CLI_EVTUSR_REBOOT(COMMON_UART3);
        result |= CLI_EVTUSR_REBOOT(COMMON_UART4);

        if (result & CLI_TIMEOUT) {
            LOG_E("REBOOT FAIL\r\n", 0);
        }

        xprintf("Pared SUCCESS: %ld\r\n", ++counter);
    }

    return CLI_PROCESS_SUCCESS;
}
