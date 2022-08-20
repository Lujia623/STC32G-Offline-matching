#ifndef __APP_KEY_H_
#define __APP_KEY_H_

#include "BSP_STC32G_system.h"
// #include ""
#define NOT_KEY_PRESS               0
#define KEY1_PRESS                  1

#define KEY_UP                      1
#define KEY_DOWN                    0
#define KEY_SENSOR                  17

typedef enum key_mode {
    CLICK,
    CONTINUOUS_PRESS
} ekeyMode;

void key_init(void);
uint8_t key_scan(ekeyMode mode);

#endif /*__APP_KEY_H_*/
