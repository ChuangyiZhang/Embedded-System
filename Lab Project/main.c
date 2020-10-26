
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
uint8_t NumOfUpdate = 0;
uint8_t hour = 0;
uint8_t NumOfHour = 0;
bool Wanted_Update = true;



static uint8_t seg7Coding[10] = {
        0b00111111,         // digit 0
        0b00000110,         // digit 1
        0b01011011,         // digit 2
        0b01001111,         // digit 3
        0b01100110,         // digit 4
        0b01101101,         // digit 5
        0b01111101,         // digit 6
        0b00000111,         // digit 7
        0b01111111,         // digit 8
        0b01101111,         // digit 9
        // MORE CODINGS
};




static uint8_t colon = 0;
/*
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
            hour = 1;
            dig4++;
        }
        if(hour == 1 && dig3 == 1)
            dig3 = 1;
        if(hour == 1 && dig3 == 2)
            dig3 = 2;

        if(hour == 1 && dig3 == 3)
            dig3 = 3;

        if(hour == 1 && dig3 == 4)
            dig3 = 4;

        if( dig4 == 2 && dig3 == 4){
            dig4 = 0;
            dig3 = 0;
        }


}

*/
// Update the clock display

void
clockUpdate(uint32_t time)                              // pointer to a 4-byte array
{
    uint8_t code[4];  // The 7-segment code for the four clock digits
    if(Wanted_Update == true){
        NumOfUpdate++;
    }

    if (NumOfUpdate ==60){
        dig1 ++;
    }


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
        NumOfHour++;
    }

    if( dig4 == 3){
        dig4 = 0;
    }

    if(dig4 == 2 && dig3 ==4){
        dig3 = 0;
        dig4 = 0;
    }

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


        delay = 250;
        break;

    case 2:
        dig1 ++;
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


    schdCallback(clockUpdate, 1000);
    schdCallback(checkPushButton, 1000);


    // Run the callback scheduler
    while (true)
    {
        schdExecute();

    }
}
