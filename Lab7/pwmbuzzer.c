

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "pwmbuzzer.h"

#define TIMER0          WTIMER0_BASE
#define PORTC           GPIO_PORTC_BASE
#define PIN             GPIO_PIN_4

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 *    BUZZER:      PF1, T0CCP1, Timer 0-B

 */



void pwmbuzzerInit()
{

    // Enable Timer 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Connect pins to timers
    GPIOPinTypeTimer(PORTC, PIN);
    GPIOPinConfigure(GPIO_PC4_WT0CCP0);

    // Select PWM for Timer 0 sub-Timer B, and Timer 1 sub-Timers A & B
    TimerConfigure(TIMER0, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM));


    TimerControlLevel(TIMER0, TIMER_A, true /* output inversion */);


    // Turn off the LEDs initially
    TimerLoadSet(TIMER0, TIMER_A, 200);
    TimerMatchSet(TIMER0, TIMER_A, 0);


    // Enable the Timer 0's TimerB and Timer 1's TimerA and TimerB
    TimerEnable(TIMER0, TIMER_A);

}


void buzzerPwmSet(int pulsewidth, int period)
{
    TimerLoadSet(TIMER0, TIMER_A, period);
    TimerMatchSet(TIMER0, TIMER_A, pulsewidth);
}

