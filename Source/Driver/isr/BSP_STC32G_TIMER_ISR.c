#include "BSP_STC32G_TIMER.h"
#include "BSP_STC32G_UART.h"

uint32_t count[TIMER_NUM_MAX] = {0};

void Timer0_int (void) interrupt TMR0_VECTOR
{
    count[TIMER0]++;
}

void Timer1_int (void) interrupt TMR1_VECTOR
{
    count[TIMER1]++;
}

void Timer2_int (void) interrupt TMR2_VECTOR
{
    count[TIMER2]++;
}

void Timer3_int (void) interrupt TMR3_VECTOR
{
    count[TIMER3]++;
}

void Timer4_int (void) interrupt TMR4_VECTOR
{
    count[TIMER4]++;
}
