/*
 * main.c: ECE 266 Lab 5, Fall 2018, Motion Detection Alarm (interrupt-based)
 */

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
#include "rotary.h"
#include "seg7.h"
#include "seg7digit.h"

seg7Display_t seg7Display = {
    0, 0, 0, 0, 1
};
void update (uint32_t time){
    uint32_t delay = 100;
    uint32_t ADCSamplingArray[2];

    ADCsampling(ADCSamplingArray);

    uint8_t Rotary1 = (ADCSamplingArray[0] *99) / 4095;
    uint8_t Rotary2 = (ADCSamplingArray[1] *99) / 4095;
    uprintf("%d     %d \n\r",ADCSamplingArray[0], ADCSamplingArray[1] );



    seg7Display.d1 = Rotary1 %10;
    seg7Display.d2 = Rotary1 /10;
    seg7Display.d3 = Rotary2 %10;
    seg7Display.d4 = Rotary2  /10;

    seg7Display.colon =1;

    seg7DigitUpdate(&seg7Display);

    schdCallback(update, time + delay);
}


int main(void)
{

    lpInit();
    seg7Init();
    ADCInit();
    uprintf("%s\n\r", "Lab 6");
    seg7DigitUpdate(&seg7Display);

    // Schedule the first callback events
    schdCallback(update, 200);

    // Run the callback scheduler
    while (true)
    {
        schdExecute();

        // Put Tiva C into sleep until the next interrupt happens
        __asm("    wfi");
    }
}
