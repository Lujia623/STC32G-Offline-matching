#ifndef __BSP_STC32G_TIMER_H__
#define __BSP_STC32G_TIMER_H__

#include "BSP_STC32G_system.h"

typedef enum Timer {
    TIMER0,
    TIMER1,
    TIMER2,
    TIMER3,
    TIMER4,
    TIMER_NUM_MAX
} eTIMERx;

typedef enum TimerxSpeed {
    FOSC_12T = 0,
    FOSC_1T,
} eTimerxSpeed;

// typedef enum CLKSource {
//     CLOCK_EXTERNAL,
//     CLOCK_INTERNAL
// } eCLKSource;

typedef enum TimerMode {
    TIMx_16BitAutoReload = 0x00,
    TIMx_16BitNoReload,
    TIMx_8BitAutoReload,
    TIMx_16BitAutoReloadNoMask
} eTimerMode;

typedef enum TimerCT {
    TIMx_TIMING,
    TIMx_COUNT
} eTimerxCT;

typedef struct TimerStructTypeDef {
    eTimerMode mode;
    eTimerxSpeed speed;
    eTimerxCT Timing_Count;
    uint8_t TIMx_CLKOut;            //ENABLE or DISABLE
    uint8_t prescale;
    uint16_t TIMx_InitValue;
    uint8_t TIMx_isRun;             //ENABLE or DISABLE
} sTimerxInitTypeDef;

// #define TIMER_NUM_MAX                           5

#define TIMER_START(timer)                              StartTimerx(timer)
#define TIMER_COMPARE(timer, target)                     ((target <= getTimerx(timer)) ? 1 : 0)
#define TIMER_END(timer)                                EndTimerx(timer)
// #define TIMER_MEASURE(timer,target)                        do {
//                                                     TIMER_START(timer);

//                                                 } while (0)


/**
 * @brief timerx
 * 
 */
#define IS_TIMERX(timer)                        ((TIMER0 == timer) || \
                                                (TIMER1 == timer) || \
                                                (TIMER2 == timer) || \
                                                (TIMER3 == timer) || \
                                                (TIMER4 == timer))
/**
 * @brief timerx mode
 * 
 */
#define IS_TIMERX_MODE(mode)                    ((TIMx_16BitAutoReload == mode) || \
                                                (TIMx_16BitNoReload == mode) || \
                                                (TIMx_8BitAutoReload == mode) || \
                                                (TIMx_16BitAutoReloadNoMask == mode))
/**
 * @brief timerx speed
 * 
 */
#define IS_TIMERX_SPEED(speed)                  ((FOSC_12T == speed) || \
                                                (FOSC_1T == speed))

#define IS_TIMERX_CT(CT)                        ((TIMx_TIMING == CT) || \
                                                (TIMx_COUNT == CT))

#define IS_ISR_ENABLED                          ((ENABLE == ET0) || \
                                                (ENABLE == ET1) || \
                                                (ENABLE == ET2) || \
                                                (ENABLE == ET3) || \
                                                (ENABLE == ET4))


void StartTimerx(eTIMERx eTimerx);

void EndTimerx(eTIMERx eTimerx);

double getTimerx(eTIMERx eTimerx);

void Timer0Config(void);

void Timer3Config(void);

void delay_us(uint16_t us);

void delay_ms(uint16_t ms);

/**
 * @brief Set the Timerx object
 * 
 * @param etimerx enum eTIMERx
 * @param Value TH\TL value
 * @param prescale Pre-division frequency coefficient
 * @param Settime time by setting
 */
void SetTimerx(eTIMERx etimerx, uint16_t Value, uint8_t prescale, uint32_t Settime);

#endif /*__BSP_STC32G_TIMER_H__*/
