/*
 * main.c
 *
 *  Created on: Aug 23, 2016
 *      Author: zzhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>
#include "launchpad.h"
#include "seg7.h"
uint8_t dig1 = 0;
uint8_t dig2 = 0;
uint8_t dig3 = 0;
uint8_t dig4 = 0;
bool Wanted_Update = true;
// 7-segment coding table. See https://en.wikipedia.org/wiki/Seven-segment_display. The segments
// are named as A, B, C, D, E, F, G. In this coding table, segments A-G are mapped to bits 0-7.
// Bit 7 is not used in the coding. This display uses active high signal, in which '1' turns ON a
// segment, and '0' turns OFF a segment.
static uint8_t seg7Coding[10] = {
		0b00111111, 		// digit 0
		0b00000110, 		// digit 1
		0b01011011,			// digit 2
		0b01001111,	        // digit 3
		0b01100110,         // digit 4
		0b01101101,         // digit 5
		0b01111101,         // digit 6
		0b00000111,         // digit 7
		0b01111111,         // digit 8
		0b01101111,         // digit 9
		// MORE CODINGS
};

// The colon status: if colon == 0b10000000, then the colon is on,
// otherwise it is off.
static uint8_t colon = 0;

void displayUpdate(){

        if( dig1 == 10 ){
            dig1 = 0;
            dig2++;
        }
        if( dig2 == 6){
            dig2 = 0;
            dig3++;
        }
        if( dig3 == 10){
            dig3 = 0;
            dig4++;
        }
        if( dig4 == 6){
            dig4 = 0;
        }

}


// Update the clock display

void
clockUpdate(uint32_t time)								// pointer to a 4-byte array
{
    uint8_t code[4];  // The 7-segment code for the four clock digits
    if(Wanted_Update == true){
        dig1++;
    }

    displayUpdate();
    code[0] = seg7Coding[dig1] + colon;
    code[1] = seg7Coding[dig2] + colon;
    code[2] = seg7Coding[dig3] + colon;
    code[3] = seg7Coding[dig4] + colon;


    // Calculate the display digits and colon setting for the next update
    if (colon == 0b00000000) {
        colon = 0b10000000;
    }
    else {
        colon = 0b00000000;
    }


    if(Wanted_Update == true){
        Wanted_Update = false;
    }else{
        Wanted_Update = true;
    }

    // Call back after 1 second

    seg7Update(code);
    schdCallback(clockUpdate, time+500);

}
checkPushButton(uint32_t time)
{
    // Read the pushbutton state; see pbRead() in launchpad.h
    int code = pbRead();
    uint32_t delay = 10;

    switch (code) {
    case 1:
        dig3++;
        displayUpdate();

        delay = 250;
        break;

    case 2:
        dig1 ++;
        displayUpdate();
        // Use an inertia for soft de-bouncing
        delay = 250;
        break;
    }

    schdCallback(checkPushButton, time + delay);
}


int main(void)
{
	lpInit();
	seg7Init();

	uprintf("%s\n\r", "Lab 2: Wall clock");

	// Schedule the first callback events for LED flashing and push button checking.
	// Those trigger callback chains. The time unit is millisecond.
	schdCallback(clockUpdate, 1000);
	schdCallback(checkPushButton, 1000);
	// Loop forever
	while (true) {
		schdExecute();
	}
}
