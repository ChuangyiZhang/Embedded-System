

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"

// Buzzer-related constants
#define BUZZER_CHECK_INTERVAL 30
#define BUZZER_ON_TIME  30
#define BUZZER_OFF_TIME (3000 - BUZZER_ON_TIME)



typedef struct
{
    enum
    {
        Off, On, SwitchOff, SwitchOn
    } state;
    bool buzzing;
    int32_t timeLeft;
} buzzer_t;
static volatile buzzer_t buzzer = { Off, false, 0 };


static volatile uint32_t lastMotionDetectedTime = 0;


void buzzerPlay(uint32_t time)
{
    uint32_t delay = BUZZER_CHECK_INTERVAL;

    switch (buzzer.state)
    {
    case Off:
        break;

    case On:
        if (buzzer.buzzing)
        {

            if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
            {
                buzzerOff();
                buzzer.buzzing = false;
                buzzer.timeLeft = BUZZER_OFF_TIME;
            }
        }
        else
        {
            if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
            {
                buzzerOn();
                buzzer.buzzing = true;
                buzzer.timeLeft = BUZZER_ON_TIME;
            }
        }
        break;

    case SwitchOff:             // De-activate the buzzer system
        if (buzzer.buzzing)
            buzzerOff();
        buzzer.state = Off;
        buzzer.buzzing = Off;
        break;

    case SwitchOn:              // Activate the buzzer system
        buzzerOn();
        buzzer.state = On;
        buzzer.buzzing = true;
        buzzer.timeLeft = BUZZER_ON_TIME;
        break;
    }

    // schedule the next callback
    schdCallback(buzzerPlay, time + delay);
}

static volatile bool userActivated = false;


void pbIntrHandler()
{

    static uint32_t lastTime = 0;

    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    int code = pbRead();

    uint32_t time = sysTimeGet();

    if (time < lastTime + 150)
        return;

    switch (code)
    {
    case 1:
        userActivated = true;

        break;

    case 2:
        userActivated = false;

        break;
    }


    lastTime = time;
}

void pirIntrHandler()
{

       GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_5);

    switch (userActivated)
      {
      case true:
          if(motionDetect() > 0){
              buzzer.state = SwitchOn;
              ledTurnOnOff(true, false, false);
          }
          else{
              buzzer.state = SwitchOff;
              ledTurnOnOff(false, false, true);
          }
          break;

      case false:
          buzzer.state = SwitchOff;
          ledTurnOnOff(false, false, false);
          break;
      }

}

void setInterrupts()
{
    // Set interrupt on Port F, pin 0 (SW1) and pin 4 (SW2)
    GPIOIntRegister(GPIO_PORTF_BASE, pbIntrHandler); // register the interrupt handler
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, // interrupt on falling edge, note that SW1 and SW2 are active low
                   GPIO_FALLING_EDGE);
    IntPrioritySet(INT_GPIOF, 1); // set interrupt level to 0 (0 is the highest for programmable interrupts)
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // enable interrupts on SW1 and SW2 input

    // Set interrupt on Port C, pin 5 (motion sensor)
    GPIOIntRegister(GPIO_PORTC_BASE, pirIntrHandler); // register the interrupt handler
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_5,  // interrupt on falling edge, note that motion sensor is active high
                      GPIO_BOTH_EDGES);

    IntPrioritySet(INT_GPIOC, 0); // set interrupt level to 0 (0 is the highest for programmable interrupts)
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_5); // enable interrupts on MOTION SENSOR
}

/*
 * The main function
 */
int main(void)
{
    lpInit();
    buzzerInit();
    motionInit();
    setInterrupts();

    // Print out a start message
    uprintf("%s\n\r", "Lab 5 starts");

    // Schedule the first callback events
    schdCallback(buzzerPlay, 200);


    // Run the callback scheduler
    while (true)
    {
        schdExecute();

        // Put Tiva C into sleep until the next interrupt happens
        __asm("    wfi");
    }
}
