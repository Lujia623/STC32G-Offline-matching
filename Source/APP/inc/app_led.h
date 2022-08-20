#ifndef __APP_LED_H_
#define __APP_LED_H_

#include "BSP_STC32G_system.h"
#include "BSP_STC32G_GPIO.h"

typedef enum Ledfrom_on {
    led_from_on,
    led_from_off
} eLedfrom_on;

typedef struct LedModuleTypeDef {
    uint8_t gpio;
    eGPIO_Mode mdoe;
    eLedfrom_on from_on;
} sLedModuleTypeDef;

#define LED_GPIO            P72

void led_init(void);
void led_off(void);
void led_on(void);
void led_blink(uint16_t ms);
void led_dim(uint16_t ms);
#endif /*__APP_LED_H_*/
