/*
 * mail.c: Starter code for Lab 7, Fall 2018
 *
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "launchpad.h"
#include "rotary.h"
#include "pwmled.h"
#include "pwmbuzzer.h"

// LED-related constant
#define LED_PWM_PERIOD          5000
#define LED_MAX_PULSE_WIDTH     40
#define BUZZER_CHECK_INTERVAL   30

#define BUZZER_ON_TIME  200
#define BUZZER_OFF_TIME (2000 - BUZZER_ON_TIME)
#define BUZZER_MAX_PERIOD   (50000000 / 216.63)
#define BUZZER_MIN_PERIOD   (50000000 / 523.25)
#define BUZZER_PULSE  50000
/*#include "pwmbuzzer.h"
 * Task 1: Play a sine pattern on LED
 */

typedef struct
{
    int pwmPeriod;              // PWM period for LED
    int maxPulseWidth;          // maximum pulse width
} LedState_t;

static LedState_t led =
        { LED_PWM_PERIOD /* PWM Period */, LED_MAX_PULSE_WIDTH /* Multiply factor */};


typedef struct
{

    enum
    {
        Off, On,
    } state;            // the running state of the buzzer system
    bool buzzing;       // if the buzzer is buzzing
    int32_t timeLeft;   // the time left for the buzzer to buzz or not buzz
    int32_t period;
    int32_t pulsewidth;
} buzzer_t;

static buzzer_t buzzer = { On, false, 0, BUZZER_MAX_PERIOD,0};

// The buzzer play callback function
void buzzerPlay(uint32_t time)
{


    uint32_t delay = BUZZER_CHECK_INTERVAL;     // the delay for next callback

    uint32_t ADCSamplingArray[2];
    ADCsampling(ADCSamplingArray);
    int volume = ADCSamplingArray[0] *100 / 4095;
    int pitch = ADCSamplingArray[1] *100 / 4095;

    uprintf("%d, %d\n\r",volume,pitch  );

    buzzer.pulsewidth = BUZZER_PULSE * volume /100;

    buzzer.period = BUZZER_MIN_PERIOD + ( BUZZER_MAX_PERIOD - BUZZER_MIN_PERIOD) * pitch /100;
    uprintf("%d, %d\n\r",buzzer.pulsewidth,buzzer.period  );

    switch (buzzer.state)
    {
    case Off:           // the buzzer system is turned off, do nothing
        break;

    case On:            // the buzzer system is active, turn buzzer on and off
        if (buzzer.buzzing)
        {

            // If the buzzer has been buzzing for enough time, turn it off
            if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
            {
                buzzerPwmSet(0,buzzer.period);
                buzzer.buzzing = false;
                buzzer.timeLeft = BUZZER_OFF_TIME;
            }
        }
        else
        {
            // If the buzzer has been silent for enough time, turn it on
            if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
            {
                buzzerPwmSet(buzzer.pulsewidth, buzzer.period);
                buzzer.buzzing = true;
                buzzer.timeLeft = BUZZER_ON_TIME;
            }
        }
        break;
    }

    // schedule the next callback
    schdCallback(buzzerPlay, time + delay);
}


static inline double sine(unsigned int degree)
{
    double radian = 2 * M_PI * ((double) (degree % 360) / 360);
    return sin(radian);
}





// LED playing callback function
void ledPlay(uint32_t time)
{
    static unsigned int angle = 0; // the degree of angle, used for calculating sine value
    int delay = 5;                    // the callback delay
    uint32_t ADCSamplingArray[2];
    ADCsampling(ADCSamplingArray);

    led.maxPulseWidth = ADCSamplingArray[0] ;

    // Calculate PWM parameters for red, blue, and green sub-LEDs using sine function.
    // Use phase shift of 60, 30, and 0 degrees for red, blue, and green
    pwm_t red, blue, green;
    red.pulseWidth = sine(angle + 60) * led.maxPulseWidth;
    blue.pulseWidth = sine(angle + 30) * led.maxPulseWidth;
    green.pulseWidth = sine(angle) * led.maxPulseWidth;
    red.period = green.period = blue.period = led.pwmPeriod;

    // Set the PWM parameters for LED
    ledPwmSet(red, blue, green);

    // Advance the angle by one degree, so a play period is 360 * 5 = 1800 ms
    angle++;

    // Schedule the next callback
    schdCallback(ledPlay, time + delay);
}

void main(void)
{
    lpInit();
    ledPwmInit();
    ADCInit();
    pwmbuzzerInit();

    schdCallback(ledPlay, 1002);
    schdCallback(buzzerPlay, 1002);
    // Loop forever
    while (true)
    {
        schdExecute();
    }
}
