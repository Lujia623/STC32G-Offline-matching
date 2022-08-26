#include "BSP_STC32G_system.h"
#include "BSP_STC32G_GPIO.h"
#include "BSP_STC32G_UART.h"
#include "BSP_STC32G_TIMER.h"
#include "app_uart.h"
#include "app_led.h"
#include "app_key.h"

extern uint8_t xdata mem_pool[1024 * 2];

#define SOFTWARE_VERSION            "1.0.0"

void assert_failed(uint8_t *file, uint32_t line) 
{
#ifdef DEVICE_UART1
    xprintf("\n\r Wrong parameter value detected on\r\n");
    xprintf("       file  %s\r\n", file);
    xprintf("       line  %d\r\n", line);

    while (1) ;
    // while(1);
#else
#error "ERROR"
#endif
}

static void version(void)
{
    xprintf("ELU Science and Technology Ltd.\r\n");
    xprintf("%s build time:%s %s\r\n", SOFTWARE_VERSION, __DATE__, __TIME__);
}

void xSystemInit(void) 
{
    WTST = 0;     //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXFR = 1;    //扩展寄存器(XFR)访问使能
    CKCON = 0;    //提高访问XRAM速度

    xinit_mempool((uint8_t *) mem_pool, sizeof(mem_pool));

    GPIO_config();
    UART1_conifg();
    UART2_conifg();
    UART3_conifg();
    UART4_conifg();
    CommonConfigAll();
    Timer0Config();
    Timer3Config();
    led_init();
    key_init();
    portENABLE_INTERRUPTS();
    version();
}
