
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>


#include <math.h>
#include "launchpad.h"
#include "pwmbuzzer.h"




#define BUZZER_CHECK_INTERVAL 200
#define BUZZER_ON_TIME  300
#define BUZZER_OFF_TIME (1200 - BUZZER_ON_TIME)

#define BUZZER_MIN_PERIOD   (50000000 / 523.25)
#define BUZZER_MAX_PERIOD   (50000000 / 216.63)
#define BUZZER_PULSE  50000

// MUSIC NOTE
#define NOTE_E7 (uint32_t) (50000000 / 2637)
#define NOTE_C7 (uint32_t) (50000000 / 2093)
#define NOTE_G7 (uint32_t) (50000000 / 3136)
#define NOTE_G6 (uint32_t) (50000000 / 1568)
#define NOTE_E6 (uint32_t) (50000000 / 1319)
#define NOTE_A6 (uint32_t) (50000000 / 1716)
#define NOTE_B6 (uint32_t) (50000000 / 1916)
#define NOTE_AS6 (uint32_t) (50000000 / 1865)
#define NOTE_A7 (uint32_t) (50000000 / 3520)
#define NOTE_D7 (uint32_t) (50000000 / 2349)
#define NOTE_F7 (uint32_t) (50000000 / 2637)


#define NOTE_D4Sharp (uint32_t) (50000000 / 277.18)
#define NOTE_A3Sharp (uint32_t) (50000000 / 233.08)
#define NOTE_G4 (uint32_t) (50000000 / 392.00)
#define NOTE_F4_G4 (uint32_t) (50000000 / (98.79+392.00))
#define NOTE_D4Sharp_A3Sharp (uint32_t) (50000000 /(293.66+233.08))
#define NOTE_G4Sharp_G4_D4Sharp (uint32_t) (50000000 /(415.30+392.00+311.18))
#define NOTE_G4Sharp (uint32_t) (50000000 / 415.30)
#define NOTE_G4_A3Sharp (uint32_t) (50000000 /(392.00 +233.08))
#define NOTE_A4Sharp (uint32_t) (50000000 / 446.16)
#define NOTE_A4Sharp_D4Sharp (uint32_t) (50000000 / (446.16+311.13))
#define NOTE_F4 (uint32_t) (50000000 / 349.23)
#define NOTE_C5 (uint32_t) (50000000 / 523.25)


#define Eb (uint32_t) (50000000 / 331.13)
#define Bb (uint32_t) (50000000 / 446.16)
#define G (uint32_t) (50000000 / 392.00)
#define F (uint32_t) (50000000 / 349.23)
#define Ab (uint32_t) (50000000 / 415.30)
#define C (uint32_t) (50000000 / 261.63)
#define D (uint32_t) (50000000 / 293.66)



#define Eb_PW (uint32_t) (0.2 * 50000000 / 331.13)
#define Bb_PW (uint32_t) (0.2 * 50000000 / 446.16)
#define G_PW (uint32_t) (0.2 * 50000000 / 392.00)
#define F_PW (uint32_t) (0.2 * 50000000 / 349.23)
#define Ab_PW (uint32_t) (0.2 * 50000000 / 415.30)
#define C_PW (uint32_t) (0.2 * 50000000 / 261.63)
#define D_PW (uint32_t) (0.2 * 50000000 / 293.66)






#define NOTE_E7_PW ((uint32_t) (0.2 * 50000000 / 2637))
#define NOTE_C7_PW ((uint32_t) (0.2 * 50000000 / 2093))
#define NOTE_G7_PW ((uint32_t) (0.2 * 50000000 / 3136))
#define NOTE_G6_PW ((uint32_t) (0.2 * 50000000 / 1568))
#define NOTE_E6_PW ((uint32_t) (0.2 * 50000000 / 1319))
#define NOTE_A6_PW ((uint32_t) (0.2 * 50000000 / 1716))
#define NOTE_B6_PW ((uint32_t) (0.2 * 50000000 / 1916))
#define NOTE_AS6_PW ((uint32_t) (0.2 * 50000000 / 1865))
#define NOTE_A7_PW ((uint32_t) (0.2 * 50000000 / 3520))
#define NOTE_D7_PW ((uint32_t) (0.2 * 50000000 / 2349))
#define NOTE_F7_PW ((uint32_t) (0.2 * 50000000 / 2637))

typedef struct{
    int pwmPulseWidth;
    int pwmPeriod;
} buzzer_t;
static volatile buzzer_t buzzer = {0, 0};



static uint32_t melody[70] = {
                              Eb, Eb ,Eb ,Bb ,Eb, G , F ,G,
                              Eb, Eb ,Bb ,Eb, G, F,
                              Eb ,Eb, Bb ,Eb ,G, Ab ,G, Eb,
                              Eb ,Ab, G, Eb,
                              Eb, Eb ,Bb, Eb, G, Bb,
                              Bb, Bb, Ab, G, Ab, Bb, Eb,
                              Eb, F, G, Eb,
                              Eb, G, F, Eb ,F ,G, C,
                              Eb, Eb, Eb, Bb, Eb, G, F, G,
                              Eb, Eb, Eb, Eb ,Bb ,Eb, G ,F, Eb,
                              Eb ,D , Eb

};



static uint32_t melody_PW[70] = {
                              Eb_PW, Eb_PW ,Eb_PW ,Bb_PW ,Eb_PW, G_PW , F_PW ,G_PW,
                              Eb_PW, Eb_PW ,Bb_PW ,Eb_PW, G_PW, F_PW,
                              Eb_PW ,Eb_PW, Bb_PW ,Eb_PW ,G_PW, Ab_PW ,G_PW, Eb_PW,
                              Eb_PW ,Ab_PW, G_PW, Eb_PW,
                              Eb_PW, Eb_PW ,Bb, Eb_PW, G_PW, Bb_PW,
                              Bb_PW, Bb_PW, Ab_PW, G_PW, Ab_PW, Bb_PW, Eb_PW,
                              Eb_PW, F_PW, G_PW, Eb_PW,
                              Eb_PW, G_PW, F_PW, Eb ,F_PW ,G_PW, C_PW,
                              Eb_PW, Eb_PW, Eb_PW, Bb_PW, Eb_PW, G_PW, F_PW, G_PW,
                              Eb_PW, Eb_PW, Eb_PW, Eb_PW ,Bb_PW ,Eb_PW, G_PW ,F_PW, Eb_PW,
                              Eb_PW ,D_PW , Eb_PW
};




/*



 // Mario Super Bro

static uint32_t melody[78] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};



static uint32_t melody_PW[78] = {
    NOTE_E7_PW, NOTE_E7_PW, 0, NOTE_E7_PW,
    0, NOTE_C7_PW, NOTE_E7_PW, 0,
    NOTE_G7_PW, 0, 0,  0,
    NOTE_G6_PW, 0, 0, 0,

    NOTE_C7_PW, 0, 0, NOTE_G6_PW,
    0, 0, NOTE_E6_PW, 0,
    0, NOTE_A6_PW, 0, NOTE_B6_PW,
    0, NOTE_AS6_PW, NOTE_A6_PW, 0,

    NOTE_G6_PW, NOTE_E7_PW, NOTE_G7_PW,
    NOTE_A7_PW, 0, NOTE_F7_PW, NOTE_G7_PW,
    0, NOTE_E7_PW, 0, NOTE_C7_PW,
    NOTE_D7_PW, NOTE_B6_PW, 0, 0,

    NOTE_C7_PW, 0, 0, NOTE_G6_PW,
    0, 0, NOTE_E6_PW, 0,
    0, NOTE_A6_PW, 0, NOTE_B6_PW,
    0, NOTE_AS6_PW, NOTE_A6_PW, 0,

    NOTE_G6_PW, NOTE_E7_PW, NOTE_G7_PW,
    NOTE_A7_PW, 0, NOTE_F7_PW, NOTE_G7_PW,
    0, NOTE_E7_PW, 0, NOTE_C7_PW,
    NOTE_D7_PW, NOTE_B6_PW, 0, 0
};


*/















bool PlayMusic = false;

static uint32_t ArrayLength = 78;
static uint32_t i = 0;

void checkPushButton(uint32_t time)
{
    uint32_t delay = 10;
    int code = pbRead();

    switch (code)
    {
    case 1:
        PlayMusic  = true;
        delay = 250;
        uprintf("%s\n\r", "button is on");
        break;

    case 2:
        PlayMusic = false;
        delay = 250;
        uprintf("%s\n\r", "button is off");
        break;
    }
    schdCallback(checkPushButton, time + delay);
}



void buzzerPlay(uint32_t time){


    uint32_t delay = BUZZER_CHECK_INTERVAL;

    if(i < ArrayLength)
        i++;
    else
        i = 0;



    if(PlayMusic)
        buzzerPwmSet(melody_PW[i],melody[i]);
    else
        buzzerPwmSet(0,0);

    schdCallback(buzzerPlay, time + delay);
}



 void main(){

    lpInit();
    pwmbuzzerInit();



    uprintf("%s\n\r", "lab Project");


    schdCallback(checkPushButton, 1005);
    uprintf("%s\n\r", "check push work");

    schdCallback(buzzerPlay, 1010);
    uprintf("%s\n\r", "buzzer play work");


    while (true)
    {
        schdExecute();

    }
}

