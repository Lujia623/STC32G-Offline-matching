#include "gpio_digital.h"
#include "debug.h"
#include "pins_stc32g_include.h"

void digital_write(uint8_t gpio, uint8_t val)
{
    uint8_t bits, port;

    bits = digitalPinToBitMask(gpio);
    port = digitalPinToPort(gpio);

    if(port == LOW) {
        LOG_E("%s %d\n port not exit\r\n", __FILE__, __LINE__);
        return;
    }

    switch (port) {
        case P0PORT:
            if(val == LOW) {
                P0 &= ~bits;
            } else {
                P0 |= bits;
            }
            break;
        case P1PORT:
            if(val == LOW) {
                P1 &= ~bits;
            } else {
                P1 |= bits;
            }
            break;
        case P2PORT:
            if(val == LOW) {
                P2 &= ~bits;
            } else {
                P2 |= bits;
            }
            break;
        case P3PORT:
            if(val == LOW) {
                P3 &= ~bits;
            } else {
                P3 |= bits;
            }
            break;
        case P4PORT:
            if(val == LOW) {
                P4 &= ~bits;
            } else {
                P4 |= bits;
            }
            break;
        case P5PORT:
            if(val == LOW) {
                P5 &= ~bits;
            } else {
                P5 |= bits;
            }
            break;
        case P6PORT:
            if(val == LOW) {
                P6 &= ~bits;
            } else {
                P6 |= bits;
            }
            break;
        case P7PORT:
            if(val == LOW) {
                P7 &= ~bits;
            } else {
                P7 |= bits;
            }
            break;
    
        default:
            break;
    }
}

uint8_t digital_read(uint8_t gpio)
{
    uint8_t port, bits, portlevel;


    bits = digitalPinToBitMask(gpio);
    port = digitalPinToPort(gpio);
    
    if(port == LOW) {
        LOG_E("%s %d\n port not exit\r\n", __FILE__, __LINE__);
        return LOW;
    }

    switch (port) {
        case P0PORT:
            portlevel = P0;
            break;
        case P1PORT:
            portlevel = P1;
            break;
        case P2PORT:
            portlevel = P2;
            break;
        case P3PORT:
            portlevel = P3;
            break;
        case P4PORT:
            portlevel = P4;
            break;
        case P5PORT:
            portlevel = P5;
            break;
        case P6PORT:
            portlevel = P6;
            break;
        case P7PORT:
            portlevel = P7;
            break;
        
        default:
            break;
    }

    if(portlevel & bits) return HIGHT;
    return LOW;
}
