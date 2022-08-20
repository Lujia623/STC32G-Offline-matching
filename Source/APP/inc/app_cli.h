#ifndef __APP_CLI_H_
#define __APP_CLI_H_

#include "BSP_STC32G_system.h"
#include "app_uart.h"

#define GTP_PREAMBLE2                0xD0D2C5C2UL
#define GTP_PREAMBLE                (uint32_t)0xC2C5D2D0UL
#define GTP_VERSION                 0x00
#define GTP_TYPE                    0x03
#define GTP_FC                      0x04
#define GTP_SEQ                     0x0000
#define CLI_START                   0x2323
#define CLI_NEED_ACK                0x01
#define CLI_NO_ACK                  0x00
#define CLI_TYPE_CMD                0x00
#define CLI_TYPE_RES                0x01
#define CLI_TYPE_IND                0x02
#define CLI_TAIL                    0x4040
#define CLI_EXIT_SLEEP              4

#define GTP_PREAMBLE_LENGTH         4
#define CLI_SN_LENGTH               2
#define CLI_START_LENGTH            2
#define CLI_TAIL_LENGTH             2
#define ADDR_LENGTH                 6

#define GTP_HEADER_CRC8_ERROR       -4
#define CLI_START_ERROR             -3
#define POOL_MALLOC_ERROR           -2
#define CLI_REVDATA_ERROR           -1
#define CLI_REVDATA_OK              0
#define CLI_TIMEOUT                 0x80
#define CLI_GET_ADDR_TIMEOUT        -5
#define CLI_INTERTECT_ADDR_TIMEOUT  -6
#define CLI_PROCESS_SUCCESS         1

#define CLI_PAYLOAD_LENGTH          32
#define CLI_RETRY_TIMES             50
#define CLI_SEND_TIMEOUT            10 //10s

#define CLI_DEVICEX_TRANSFER_SUCCESS(x)    BIT(x)
#define CLI_MASTER_TRANSFER_SUCCESS        BIT(1)
#define CLI_SLAVE1_TRANSFER_SUCCESS        BIT(2)
#define CLI_SLAVE2_TRANSFER_SUCCESS        BIT(3)

typedef void (*cli_msg_handle)(uint8_t *buffer, uint16_t length);

typedef enum {
    UART_MASTER,
    UART_SLAVE1,
    UART_SLAVE2,
    UART_DEVICE_MAX,
} eDeviceTypedef;

typedef struct buffer_data {
    eCommon_UARTx eCOMM_UARTx;
    uint16_t module_id;
    uint16_t msg_id;
    uint16_t length;
    uint8_t *buffer;
} sBufferData;


/**
 * @brief module_id
 * 
 */
typedef enum {
    CLI_MODULEID_DEBUGLOG,
    CLI_MODULEID_COMMUNICATION,
    CLI_MODULEID_HOSTINTERFACE,
    CLI_MODULEID_FTM,
    CLI_MODULEID_COMMON,
    CLI_MODULEID_LOWPOWER,
    CLI_MODULEID_APPLICATION,
    /** Product Specific Module ID Start */
    CLI_MODULEID_BT_APPL,
    CLI_MODULEID_AUDIO,
    /** Product Specific Module ID End */

    CLI_MODULEID_DRIVER_COMMON = 0x100,
    CLI_MODULEID_DRIVER_CHARGER = 0x101,
    CLI_MODULEID_DRIVER_LED = 0x102,

    /** This cli module is for FW of WQ Adaptor, please do not use it!*/
    CLI_MODULEID_RESERVED_WQ_ADAPTER = 0x400,

    CLI_MODULEID_MAX_NUM,
} CLI_MODULEID;

/** @defgroup app_cli_enum Enum
 * @{
 */

typedef enum {
    APP_CLI_MSGID_SEND_USER_EVENT,
    APP_CLI_MSGID_SEND_APP_MSG,
    APP_CLI_MSGID_SEND_BT_RPC_CMD,
    APP_CLI_MSGID_SEND_HCI_CMD,
    APP_CLI_MSGID_GET_BDADDR,
    APP_CLI_MSGID_TWS_PAIR,
    APP_CLI_MSGID_SEND_HCI_DATA,
    APP_CLI_MSGID_ENABLE_HCI_PASSTHROUGH,
    APP_CLI_MSGID_GET_TWS_STATE,
    APP_CLI_MSGID_GET_BATTERY_STATE,
    APP_CLI_MSGID_AT_TEST,
    APP_CLI_MSGID_GET_KEY_STATE,
    APP_CLI_MSGID_CONTROL_LED,
    APP_CLI_MSGID_SET_PEER_ADDR,
    APP_CLI_MSGID_GET_OEM_BDADDR,
    APP_CLI_MSGID_EN_BATTERY_REPORT,
    APP_CLI_MSG_ID_GET_TOUCH_KEY_INFO,
    APP_CLI_MSG_ID_GET_TOUCH_INEAR_CDC,
    APP_CLI_MSGID_EN_TONE,
    APP_CLI_MSGID_GET_NTC,
    APP_CLI_MSGID_GET_OEM_PPM,
    APP_CLI_MSGID_GET_BTN_PRESSED,
    APP_CLI_MSGID_GET_BT_NAME,
    APP_CLI_MSGID_GET_CUSTOM_VER,
    APP_CLI_MSGID_GET_BAT,
    APP_CLI_MSGID_GET_INEAR,
    APP_CLI_MSGID_GET_ANC,
    APP_CLI_MSGID_LAP_PAIR,
    APP_CLI_MSGID_GET_LAP_STATE,
    APP_CLI_MSGID_LAP_SET_PEER_ADDR,
    APP_CLI_MSGID_LAP_GET_PEER_ADDR,
    APP_CLI_MSGID_GET_GTWS_MODE,
    APP_CLI_MSGID_SET_GTWS_MODE,
    APP_CLI_MSGID_LAP_RESET_PEER_ADDR,
} app_cli_msg_id_e;
/**
 * @}
 */

typedef struct app_cli_command {
    struct app_gtp_header {
        uint8_t version;
        uint16_t length;
        uint8_t type;
        uint8_t FC;
        uint16_t seq;
    } gtp_header;
    uint8_t CRC8;

    struct app_cli_Msg {
        uint16_t module_id;
        uint16_t CRC;
        uint16_t message_id;
        uint8_t ACK:1;
        uint8_t cli_type:3;
        uint8_t Reversed:4;
        uint8_t result;
        // uint16_t SN;
    } cli_Msg;
    uint32_t CRC32;


} scli_Command;

typedef struct app_cli_revcommand {
    struct app_gtp_revheader {
        uint8_t version;
        uint16_t length;
        uint8_t type;
        uint8_t FC;
        uint16_t seq;
    } gtp_revheader;
    uint8_t CRC8;

    struct app_cli_revMsg {
        uint16_t module_id;
        uint16_t CRC;
        uint16_t message_id;
        uint8_t ACK:1;
        uint8_t cli_type:3;
        uint8_t Reversed:4;
        uint8_t result;
        // uint16_t SN[];
    } cli_revMsg;
    uint32_t CRC32;


} scli_revCommand;

uint8_t CLI_IntersectAddr(eCommon_UARTx eCOMM_UARTx, eDeviceTypedef eDevice, \ 
                            uint8_t *setAddr, uint8_t *rev_buf);

uint8_t CLIGetDeviceAddr(eCommon_UARTx eCOMM_UARTx, eDeviceTypedef eDevice);

uint8_t CLI_EVTUSR_REBOOT(eCommon_UARTx eCOMM_UARTx);

uint8_t CLIGetLAPMode(eCommon_UARTx eCOMM_UARTx);

int8_t CLI_Process(void);

#endif /*__APP_CLI_H_*/
