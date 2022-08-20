#ifndef __GPIO_DIGITAL_H_
#define __GPIO_DIGITAL_H_

#include "BSP_STC32G_system.h"

#define LOW                 0
#define HIGHT               1

void digital_write(uint8_t gpio, uint8_t val);
uint8_t digital_read(uint8_t gpio);

#endif /*__GPIO_DIGITAL_H_*/
