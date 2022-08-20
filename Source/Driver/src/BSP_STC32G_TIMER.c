#include "BSP_STC32G_TIMER.h"
#include "BSP_STC32G_NVIC.h"
#include "debug.h"

extern uint32_t count[TIMER_NUM_MAX];
static uint8_t isInit[TIMER_NUM_MAX] = {DISABLE};
static uint16_t TimerxInitValue[TIMER_NUM_MAX];

static void TimerxRegisterConfig(eTIMERx eTimerx) 
{
    switch (eTimerx) {
        case TIMER0:
            TMOD &= ~0x0C;
            break;

        case TIMER1:
            /* code */
            break;

        case TIMER2:
            /* code */
            break;

        case TIMER3:
            /* code */
            break;

        case TIMER4:
            /* code */
            break;

        default:
            break;
    }
}

void Timerx_Init(eTIMERx eTimerx, sTimerxInitTypeDef *Timer_InitStruct) 
{
    assert_param(IS_TIMERX(eTimerx));
    assert_param(IS_TIMERX_MODE(Timer_InitStruct->mode));
    assert_param(IS_TIMERX_CT(Timer_InitStruct->Timing_Count));

    if (ENABLE == isInit[eTimerx])
        return;

    switch (eTimerx) {
        case TIMER0:
            TR0 = 0;
            TMOD &= ~0x0C;
            TMOD = (TMOD & 0xFC) | Timer_InitStruct->mode;
            IT0 = 1;
            // IE0 = 1;
            T0_CT = Timer_InitStruct->Timing_Count;
            TH0 = (Timer_InitStruct->TIMx_InitValue >> 8) & 0xFF;
            TL0 = Timer_InitStruct->TIMx_InitValue & 0xFF;
            T0x12 = Timer_InitStruct->speed;
            T0CLKO = Timer_InitStruct->TIMx_CLKOut;
            TM0PS = Timer_InitStruct->prescale;
            TR0 = Timer_InitStruct->TIMx_isRun;
            break;

        case TIMER1:
            TR1 = 0;
            TMOD &= 0x3F;
            TMOD = (TMOD & 0xCF) | Timer_InitStruct->mode;
            IT1 = 1;
            // IE1 = 1;
            T1_CT = Timer_InitStruct->Timing_Count;
            TH1 = (Timer_InitStruct->TIMx_InitValue >> 8) & 0xFF;
            TL1 = Timer_InitStruct->TIMx_InitValue & 0xFF;
            T1x12 = Timer_InitStruct->speed;
            T1CLKO = Timer_InitStruct->TIMx_CLKOut;
            TM1PS = Timer_InitStruct->prescale;
            TR1 = Timer_InitStruct->TIMx_isRun;
            break;

        case TIMER2:
            T2R = 0;
            T2_CT = Timer_InitStruct->Timing_Count;
            T2H = (Timer_InitStruct->TIMx_InitValue >> 8) & 0xFF;
            T2L = Timer_InitStruct->TIMx_InitValue & 0xFF;
            T2x12 = Timer_InitStruct->speed;
            T2CLKO = Timer_InitStruct->TIMx_CLKOut;
            TM2PS = Timer_InitStruct->prescale;
            T2R = Timer_InitStruct->TIMx_isRun;
            break;

        case TIMER3:
            T3R = 0;
            T3_CT = Timer_InitStruct->Timing_Count;
            T3H = (Timer_InitStruct->TIMx_InitValue >> 8) & 0xFF;
            T3L = Timer_InitStruct->TIMx_InitValue & 0xFF;
            T3x12 = Timer_InitStruct->speed;
            T3CLKO = Timer_InitStruct->TIMx_CLKOut;
            TM3PS = Timer_InitStruct->prescale;
            T3R = Timer_InitStruct->TIMx_isRun;
            break;

        case TIMER4:
            T4R = 0;
            T4_CT = Timer_InitStruct->Timing_Count;
            T4H = (Timer_InitStruct->TIMx_InitValue >> 8) & 0xFF;
            T4L = Timer_InitStruct->TIMx_InitValue & 0xFF;

            T4x12 = Timer_InitStruct->speed;
            T4CLKO = Timer_InitStruct->TIMx_CLKOut;
            TM4PS = Timer_InitStruct->prescale;
            T4R = Timer_InitStruct->TIMx_isRun;
            break;

        default:
            break;
    }
    isInit[eTimerx] = ENABLE;
    TimerxInitValue[eTimerx] = Timer_InitStruct->TIMx_InitValue;
}

void Timer0Config(void) 
{
    sTimerxInitTypeDef TimerxInitStructure;

    TimerxInitStructure.mode = TIMx_16BitAutoReload;
    TimerxInitStructure.speed = FOSC_1T;
    TimerxInitStructure.Timing_Count = TIMx_TIMING;
    TimerxInitStructure.TIMx_CLKOut = ENABLE;
    TimerxInitStructure.TIMx_InitValue = 65535;
    TimerxInitStructure.prescale = 23;
    TimerxInitStructure.TIMx_isRun = ENABLE;

    Timerx_Init(TIMER0, &TimerxInitStructure);
    // NVIC_Timer0_Init(ENABLE, Priority_2);
}

void Timer3Config(void) 
{
    sTimerxInitTypeDef TimerxInitStructure;

    TimerxInitStructure.mode = TIMx_16BitAutoReload;
    TimerxInitStructure.speed = FOSC_1T;
    TimerxInitStructure.Timing_Count = TIMx_TIMING;
    TimerxInitStructure.TIMx_CLKOut = ENABLE;
    TimerxInitStructure.TIMx_InitValue = 18661;
    TimerxInitStructure.prescale = 255;
    TimerxInitStructure.TIMx_isRun = ENABLE;

    Timerx_Init(TIMER3, &TimerxInitStructure);
    NVIC_Timer3_Init(ENABLE, Priority_2);
}

void SetTimerx(eTIMERx eTimerx, uint16_t Value, uint8_t prescale, uint32_t Settime) 
{
    switch (eTimerx) {
        case TIMER0:

            break;

        case TIMER1:
            break;

        case TIMER2:
            break;

        case TIMER3:
            break;

        case TIMER4:
            break;

        default:
            break;
    }
}

void StartTimerx(eTIMERx eTimerx) 
{
    switch (eTimerx) {
        case TIMER0:
            TR0 = 0;
            TH0 = 0;
            TL0 = 0;
            ET0 = 1;
            TR0 = 1;
            break;

        case TIMER1:
            TR1 = 0;
            TH1 = 0;
            TL1 = 0;
            ET1 = 1;
            TR1 = 1;
            break;

        case TIMER2:
            T2R = 0;
            T2H = 0;
            T2L = 0;
            ET2 = 1;
            T2R = 1;
            break;

        case TIMER3:
            T3R = 0;
            T3H = 0;
            T3L = 0;
            ET3 = 1;
            T3R = 1;
            break;

        case TIMER4:
            T4R = 0;
            T4H = 0;
            T4L = 0;
            ET4 = 1;
            T4R = 1;
            break;

        default:
            break;
    }

    count[eTimerx] = 0;
    TimerxInitValue[eTimerx] = 0;
}

void EndTimerx(eTIMERx eTimerx) 
{
    switch (eTimerx) {
        case TIMER0:
            TR0 = 0;
            break;

        case TIMER1:
            TR1 = 0;
            break;

        case TIMER2:
            T2R = 0;
            break;

        case TIMER3:
            T3R = 0;
            break;

        case TIMER4:
            T4R = 0;
            break;

        default:
            break;
    }
}

void delay_us(uint16_t us) 
{
    if (DISABLE == isInit[TIMER0]) {
        LOG_E("%s %d:\nTIMER0 not init!\r\n", __FILE__, __LINE__);
        return;
    }
    TR0 = 0;
    TH0 = ((65536 - us) >> 8) & 0xff;
    TL0 = (65536 - us) & 0xff;
    ET0 = 0;
    TF0 = 0;
    TR0 = 1;
    while (!TF0);
    TR0 = 0;
}

void delay_ms(uint16_t ms) 
{
    while (ms--) {
        delay_us(1000);
    }
}

double getTimerx(eTIMERx eTimerx) 
{
    uint16_t value;
    uint8_t prescale;
    double time = 0;
    double TimerxCLK = 0, TimerxPeriod = 0;

    assert_param(IS_TIMERX(eTimerx));
    assert_param(IS_ISR_ENABLED);
    assert_param(isInit[eTimerx]);

    switch (eTimerx) {
        case TIMER0:
            value = ((uint16_t)(TH0 << 8) | TL0) - TimerxInitValue[eTimerx];
            prescale = TM0PS;
            break;

        case TIMER1:
            value = ((uint16_t)(TH1 << 8) | TL1) - TimerxInitValue[eTimerx];
            prescale = TM1PS;
            break;

        case TIMER2:
            value = ((uint16_t)(T2H << 8) | T2L) - TimerxInitValue[eTimerx];
            prescale = TM2PS;
            break;

        case TIMER3:
            value = ((uint16_t)(T3H << 8) | T3L) - TimerxInitValue[eTimerx];
            prescale = TM3PS;

            break;

        case TIMER4:
            value = ((uint16_t)(T4H << 8) | T4L) - TimerxInitValue[eTimerx];
            prescale = TM4PS;
            break;

        default:
            break;
    }

    TimerxCLK = MAIN_Fosc / (prescale + 1);
    TimerxPeriod = (65536 - TimerxInitValue[eTimerx]) / TimerxCLK;
    time = 1.0 * count[eTimerx] * TimerxPeriod + (value / TimerxCLK);

    // xprintf("T3H:%d,T3L:%d\r\n",T3H,T3L);
    // LOG_D("%s:\n%d,count :%d\r\n",__FILE__,__LINE__,count[eTimerx]);
    // LOG_E("%s:\n%d,value :%0x,prescale:%d\r\n",__FILE__,__LINE__,value,prescale);
    // xprintf("TimerxCLK :%f\r\n",TimerxCLK);
    // xprintf("TimerxPeriod :%d\r\n",TimerxPeriod);
    // xprintf("before :%f\r\n",1.0 * count[eTimerx] * TimerxPeriod);
    xprintf("in time:%f\r\n", time);
    // xprintf("TimerxInitValue :%0x\r\n",TimerxInitValue[eTimerx]);

    return time;
}
