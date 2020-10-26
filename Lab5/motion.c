/*
 * ECE 266, Spring 2019, Lab 4 Motion Detection Alarm
 *
 * motion.c: This file contains the initialization function for the PIR motion sensor.
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "motion.h"

// Pin usage: Grove base port J16, Tiva C PC5 (Port C, Pin 4)
#define PIR_PERIPH   SYSCTL_PERIPH_GPIOC
#define PIR_PORT     GPIO_PORTC_BASE
#define PIR_PIN      GPIO_PIN_5

// Initialize the buzzer
void pirInit()
{
    // Enable the port peripheral used by the buzzer
    SysCtlPeripheralEnable(PIR_PERIPH);

    // Configure the pin as output
    GPIOPinTypeGPIOInput(PIR_PORT, PIR_PIN);
}/*
 * motion.c
 *
 *  Created on: Feb 20, 2019
 *      Author: chuangyizhang
 */




