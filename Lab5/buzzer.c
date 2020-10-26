/*
 * buzzer.c
 *
 *  Created on: Feb 19, 2019
 *      Author: chuangyizhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "buzzer.h"

// Pin usage: Grove base port J17, Tiva C PC5 (Port C, Pin 5)
#define BUZZER_PERIPH   SYSCTL_PERIPH_GPIOC
#define BUZZER_PORT     GPIO_PORTC_BASE
#define BUZZER_PIN      GPIO_PIN_4

// Initialize the buzzer
void buzzerInit()
{
    // Enable the port peripheral used by the buzzer
    SysCtlPeripheralEnable(BUZZER_PERIPH);

    // Configure the pin as output
    GPIOPinTypeGPIOOutput(BUZZER_PORT, BUZZER_PIN);
}


