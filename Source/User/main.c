#include "BSP_STC32G_UART.h"
#include "BSP_STC32G_GPIO.h"
#include "BSP_STC32G_TIMER.h"
#include "debug.h"
#include "assert.h"
#include "app_uart.h"
#include "app_cli.h"
#include "app_led.h"
#include "app_key.h"

// extern uint8_t Rxbuf[128];
uint8_t xdata mem_pool[1024 * 2];

void Delay500ms()        //@24.000MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 61;
    j = 225;
    k = 62;
    do {
        do {
            while (--k);
        } while (--j);
    } while (--i);
}

int main(void) 
{
    int8_t error = 0;
    uint8_t key = 0;

    xSystemInit();

    while (1) {
        
        key = key_scan(CLICK);
        
        if (key == KEY1_PRESS) {
            led_on();
            error = CLI_Process();
        } 
        
        if(error == CLI_PROCESS_SUCCESS) {
            led_dim(0);
        } else {
            led_blink(500);
        }
    }
}
