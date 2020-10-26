
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>


#include <math.h>
#include "launchpad.h"
#include "pwmbuzzer.h"
#include "ranger.h"
#include <led.c>


#define LED_PWM_PERIOD  30
#define LED_MAX_PULSE_WIDTH 5000

#define BUZZER_CHECK_INTERVAL 10
#define BUZZER_ON_TIME  200
#define BUZZER_OFF_TIME (3000 - BUZZER_ON_TIME)
#define BUZZER_MAX_PERIOD ((uint32_t) (50000000 / 261.63))
#define BUZZER_MAX_PULSE_WIDTH 50000
#define BUZZER_MIN_PERIOD ((uint32_t) (50000000 / 525.25))

typedef struct{
    enum
    {
        Off, On, SwitchOff, SwitchOn
    } state;
    bool buzzing;
    uint32_t timeLeft;
    int pwmPulseWidth;
    int pwmPeriod;
} buzzer_t;

static volatile buzzer_t buzzer = { On, false, 0, BUZZER_MAX_PERIOD, 0 };

void buzzerPlay(uint32_t time){
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
                buzzerPwmSet(0, buzzer.pwmPeriod);
                buzzer.buzzing = false;
                buzzer.timeLeft = BUZZER_OFF_TIME;
            }
        }
        else
        {

            if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
            {
                buzzerPwmSet(buzzer.pwmPulseWidth, buzzer.pwmPeriod);
                buzzer.buzzing = true;
                buzzer.timeLeft = BUZZER_ON_TIME;
            }
        }
        break;

    case SwitchOff:
        if (buzzer.buzzing)
        buzzerPwmSet(0, buzzer.pwmPeriod);
        buzzer.state = Off;
        buzzer.buzzing = Off;
        break;

    case SwitchOn:
        buzzerPwmSet(buzzer.pwmPulseWidth, buzzer.pwmPeriod);
        buzzer.state = On;
        buzzer.buzzing = true;
        buzzer.timeLeft = BUZZER_ON_TIME;
        break;
    }


    schdCallback(buzzerPlay, time + delay);
}

bool userActivated = false;


void checkPushButton(uint32_t time)
{
    uint32_t delay = 10;
    int code = pbRead();
    switch (code)
    {
    case 1:
        userActivated = true;
        buzzer.state = SwitchOn;
        delay = 250;
        uprintf("%s\n\r", "button is on");
        break;

    case 2:
        userActivated = false;
        buzzer.state = SwitchOff;
        delay = 250;
        uprintf("%s\n\r", "button is off");
        break;
    }


    schdCallback(checkPushButton, time + delay);
}


void checkRange(uint32_t time){
    uint32_t delay = 100;
    if (userActivated == true) {
        ledTurnOnOff(1,0,0);
        uint32_t distance = rangerDetect();
        uprintf("%d\n\r", distance);

        if (distance >= 500 && distance <= 600) {
            waitUs(100);
            buzzer.timeLeft = 80;
            buzzer.pwmPulseWidth = BUZZER_MAX_PULSE_WIDTH * .8;
            buzzer.pwmPeriod = BUZZER_MIN_PERIOD * .2 + BUZZER_MIN_PERIOD;

        }
        else if (distance >= 400 && distance < 500) {
            buzzer.timeLeft = 60;
            buzzer.pwmPulseWidth = BUZZER_MAX_PULSE_WIDTH * .6;
            buzzer.pwmPeriod = BUZZER_MIN_PERIOD * .4 + BUZZER_MIN_PERIOD;

        }
        else if (distance >= 300 && distance < 400) {
            buzzer.timeLeft = 40;
            buzzer.pwmPulseWidth = BUZZER_MAX_PULSE_WIDTH * .4;
            buzzer.pwmPeriod = BUZZER_MIN_PERIOD * .6 + BUZZER_MIN_PERIOD;

        }
        else if (distance >= 150 && distance < 300) {

            buzzer.timeLeft = 30;
            buzzer.pwmPulseWidth = BUZZER_MAX_PULSE_WIDTH * .2;
            buzzer.pwmPeriod = BUZZER_MIN_PERIOD * .8 + BUZZER_MIN_PERIOD;

        }
        else if (distance >= 10 && distance < 150) {
            buzzer.timeLeft = 20;
            buzzer.pwmPulseWidth = BUZZER_MAX_PULSE_WIDTH;
            buzzer.pwmPeriod = BUZZER_MAX_PERIOD;
        }
        else if (distance > 1000) {

            buzzer.pwmPulseWidth = 0;
            buzzer.pwmPeriod = 0;
        }

    }
    else {
        ledTurnOnOff(0,0,0);
    }


    schdCallback(checkRange, time + delay);
}

 void main(){

    lpInit();
    pwmbuzzerInit();
    ledInit();
    rangerInit();

    uprintf("%s\n\r", "lab8");

    schdCallback(checkPushButton, 1000);
    uprintf("%s\n\r", "check push work");
    schdCallback(checkRange, 1005);
    uprintf("%s\n\r", "check ranger work");
    schdCallback(buzzerPlay, 1010);
    uprintf("%s\n\r", "buzzer play work");

    while (true)
    {
         schdExecute();
    }
}




