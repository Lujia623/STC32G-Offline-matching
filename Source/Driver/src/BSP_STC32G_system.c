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
    WTST = 0;     //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXFR = 1;    //��չ�Ĵ���(XFR)����ʹ��
    CKCON = 0;    //��߷���XRAM�ٶ�

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
