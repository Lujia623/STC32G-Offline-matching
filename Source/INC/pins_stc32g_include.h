#ifndef __PINS_STC32G_INCLUDE_H_
#define __PINS_STC32G_INCLUDE_H_

#include "BSP_STC32G_system.h"

#ifndef _BV
#define _BV(X)              (1 << (X))
#endif

#define NOT_A_PORT              0       //port not exit
#define NOT_A_PIN               0       //pin not exit

#define P0PORT                  1
#define P1PORT                  2
#define P2PORT                  3
#define P3PORT                  4
#define P4PORT                  5
#define P5PORT                  6
#define P6PORT                  7
#define P7PORT                  8

const uint8_t digital_pin_to_port_VPM[] = {
    P0PORT,     //PIN00
    P0PORT,
    P0PORT,
    P0PORT,
    P0PORT,
    P0PORT,
    P0PORT,
    P0PORT,
    NOT_A_PORT,     //not exit
    NOT_A_PORT,     //not exit

    P1PORT,     //PIN10
    P1PORT,
    P1PORT,
    P1PORT,
    P1PORT,
    P1PORT,
    P1PORT,
    P1PORT,
    NOT_A_PORT,     //not exit
    NOT_A_PORT,     //not exit
    
    P2PORT,     //PIN20
    P2PORT,
    P2PORT,
    P2PORT,
    P2PORT,
    P2PORT,
    P2PORT,
    P2PORT,
    NOT_A_PORT,     //not exit
    NOT_A_PORT,     //not exit

    P3PORT,     //PIN30
    P3PORT,
    P3PORT,
    P3PORT,
    P3PORT,
    P3PORT,
    P3PORT,
    P3PORT,
    NOT_A_PORT,     //not exit
    NOT_A_PORT,     //not exit

    P4PORT,     //PIN40
    P4PORT,
    P4PORT,
    P4PORT,
    P4PORT,
    P4PORT,
    P4PORT,
    P4PORT,
    NOT_A_PORT,     //not exit
    NOT_A_PORT,     //not exit

    P5PORT,     //PIN50
    P5PORT,
    P5PORT,
    P5PORT,
    P5PORT,
    P5PORT,
    P5PORT,
    P5PORT,
    NOT_A_PORT,     //not exit
    NOT_A_PORT,     //not exit

    P6PORT,     //PIN60
    P6PORT,
    P6PORT,
    P6PORT,
    P6PORT,
    P6PORT,
    P6PORT,
    P6PORT,
    NOT_A_PORT,     //not exit
    NOT_A_PORT,     //not exit

    P7PORT,     //PIN70
    P7PORT,
    P7PORT,
    P7PORT,
    P7PORT,
    P7PORT,
    P7PORT,
    P7PORT,
    NOT_A_PORT,     //not exit
    NOT_A_PORT,     //not exit
};

const uint8_t digital_pin_to_bit_mask_VPM[] = {
    _BV(0), //PIN00
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    NOT_A_PIN,     //not exit
    NOT_A_PIN,     //not exit

    _BV(0), //PIN10
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    NOT_A_PIN,     //not exit
    NOT_A_PIN,     //not exit

    _BV(0), //PIN20
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    NOT_A_PIN,     //not exit
    NOT_A_PIN,     //not exit

    _BV(0), //PIN30
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    NOT_A_PIN,     //not exit
    NOT_A_PIN,     //not exit

    _BV(0), //PIN40
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    NOT_A_PIN,     //not exit
    NOT_A_PIN,     //not exit

    _BV(0), //PIN50
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    NOT_A_PIN,     //not exit
    NOT_A_PIN,     //not exit

    _BV(0), //PIN60
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    NOT_A_PIN,     //not exit
    NOT_A_PIN,     //not exit

    _BV(0), //PIN70
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    NOT_A_PIN,     //not exit
    NOT_A_PIN,     //not exit
};

#define digitalPinToPort(port)              ( digital_pin_to_port_VPM[port] )
#define digitalPinToBitMask(port)           ( digital_pin_to_bit_mask_VPM[port] )

#endif /*__PINS_STC32G_INCLUDE_H_*/
