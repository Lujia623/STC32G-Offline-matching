#include "BSP_STC32G_UART.h"
#include "BSP_STC32G_GPIO.h"
#include "BSP_STC32G_TIMER.h"
#include "debug.h"
#include "assert.h"
#include "app_uart.h"
#include "app_cli.h"
#include "app_led.h"
#include "app_key.h"

uint8_t xdata mem_pool[1024 * 2];

int main(void) 
{
    int8_t error = 0;
    uint8_t key = 0, ichar, rx_error = 0;

    xSystemInit();

    while (1) {
        
        key = key_scan(CLICK);
        
        if (key == KEY1_PRESS) {
            led_on();
            error = CLI_Process();
        } 
        ichar = getCommonRx_char(COMMON_UART1, &rx_error);
        if ((ichar == 'S') && (rx_error == COMMON_NO_ERROR)) {
            led_on();
            error = CLI_Process();
        } 
        // ichar = getCommonRx_char(COMMON_UART2, &rx_error);
        // if(rx_error == COMMON_NO_ERROR) {
        //     xprintf("%0x\r\n", ichar);
        // }
        if(error == CLI_PROCESS_SUCCESS) {
            led_dim(0);
        } else {
            led_blink(500);
        }
    }
}
