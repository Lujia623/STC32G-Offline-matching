#include "BSP_STC32G_TIMER.h"
#include "app_key.h"
#include "gpio_digital.h"

void key_init(void)
{
    digital_write(KEY_SENSOR, 1);
}

uint8_t key_scan(ekeyMode mode)
{
    static uint8_t key_up = KEY_UP;

    if(mode == CONTINUOUS_PRESS) {
        key_up = KEY_UP;
    }
    if(key_up && digital_read(KEY_SENSOR) == LOW) {
        delay_ms(10);
        key_up = KEY_DOWN;
        if(digital_read(KEY_SENSOR) == LOW) {
            return KEY1_PRESS;
        }
    } else if(digital_read(KEY_SENSOR) == HIGHT) {
        key_up = KEY_UP;
    }

    return NOT_KEY_PRESS;
}
