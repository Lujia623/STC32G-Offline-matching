#ifndef __BSP_STC32G_SYSTEM_H__
#define __BSP_STC32G_SYSTEM_H__

#include "STC32G.H"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
//#include "FreeRTOSConfig.h"

#define MAIN_Fosc               (24000000UL)          //系统频率

#define POINT_LOW_ADDRESS       (uint8_t volatile*)0x000000
#define POINT_HIGHT_ADDRESS     (uint8_t volatile*)0xFFFFFF


#define BIT(x)                  (1 << (x))
#define IS_POINT_PARAM(point)   ((POINT_LOW_ADDRESS < point) && (POINT_HIGHT_ADDRESS> point))

// typedef _Bool                bool;
#define true                (uint8_t)1
#define false               (uint8_t)!true
#define	Priority_0			0	//中断优先级为 0 级（最低级）
#define	Priority_1			1	//中断优先级为 1 级（较低级）
#define	Priority_2			2	//中断优先级为 2 级（较高级）
#define	Priority_3			3	//中断优先级为 3 级（最高级）

#define ENABLE        1
#define DISABLE        0

#define SUCCESS        0
#define FAIL        -1

#ifndef OS
#define portDISABLE_INTERRUPTS()        EA = 0
#define portENABLE_INTERRUPTS()         EA = 1

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
#endif

#define USE_FULL_ASSERT     1
#ifdef  USE_FULL_ASSERT
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

void assert_failed(uint8_t *file, uint32_t line);

#else
#define assert_param(expr) ((void)0)
#endif


void xSystemInit(void);

#endif /*__BSP_STC32G_SYSTEM_H_*/
