#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "pwmbuzzer.h"

#define WTIMER0         WTIMER0_BASE
#define PORTC           GPIO_PORTC_BASE

void pwmbuzzerInit()
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);


    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinConfigure(GPIO_PC4_WT0CCP0);

    TimerConfigure(WTIMER0, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM));

    TimerControlLevel(WTIMER0, TIMER_A, true );

    TimerEnable(WTIMER0, TIMER_A);

    TimerLoadSet(WTIMER0, TIMER_A, 200);
    TimerMatchSet(WTIMER0, TIMER_A, 0);
;
}


void buzzerPwmSet(uint32_t pulse, uint32_t period)
{

    TimerLoadSet(WTIMER0, TIMER_A, period);
    TimerMatchSet(WTIMER0, TIMER_A, pulse);
}
