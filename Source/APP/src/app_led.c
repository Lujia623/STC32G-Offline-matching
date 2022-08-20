#include "app_led.h"
#include "BSP_STC32G_TIMER.h"
#include "BSP_STC32G_UART.h"
#include "gpio_digital.h"

static sLedModuleTypeDef LedModule;
static sLedModuleTypeDef* pLedModule;

void led_init(void)
{
    LedModule.gpio = 72;
    LedModule.mdoe = pulldown;          //led on
    LedModule.from_on = led_from_off;   //led from on/off
    pLedModule = &LedModule;
    digital_write(LedModule.gpio, (uint8_t)LedModule.mdoe);
}

void led_blink(uint16_t ms)
{
    digital_write(LedModule.gpio, HIGHT);
    delay_ms(ms);
    digital_write(LedModule.gpio, LOW);
    delay_ms(ms);
}


void led_off(void)
{
    uint8_t level;

    level = (uint8_t)LedModule.mdoe;
    if(digital_read(LedModule.gpio) == level) {
        digital_write(LedModule.gpio, !level);
    }
}

void led_on(void)
{
    uint8_t level;

    level = (uint8_t)LedModule.mdoe;
    if(digital_read(LedModule.gpio) != level) {
        digital_write(LedModule.gpio, level);
    }
}

void delay(uint16_t i)
{
    while (i--);
    
}

void led_dim(uint16_t ms)
{
    uint16_t i, hight_level, low_level;
    for(i = 1; i < 100; i++) {
        hight_level = i * 100UL;
        low_level = 10000UL - hight_level;
        digital_write(LedModule.gpio, LOW);
        delay_us(low_level);
        digital_write(LedModule.gpio, HIGHT);
        delay_us(hight_level);
        // xprintf("before hight_level :%d,low_level :%d,duty_cycle :%f\r\n",hight_level,low_level,1.0 * hight_level / (hight_level + low_level));
    }

    for(i = 1; i < 100; i++) {
        low_level = i * 100UL;
        hight_level = 10000UL - low_level;
        digital_write(LedModule.gpio, LOW);
        delay_us(low_level);
        digital_write(LedModule.gpio, HIGHT);
        delay_us(hight_level);
        // xprintf("after hight_level :%d,low_level :%d,duty_cycle :%f\r\n",hight_level,low_level,1.0 * hight_level / (hight_level + low_level));
    }
    delay_ms(ms);
}
