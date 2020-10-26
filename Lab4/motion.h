/*
 * ECE 266, Spring 2019, Lab 4 Buzzer
 *
 * motion.h: Solution code, head file for the PIR motion sensor functions
 *
 * Created by Zhao Zhang
 */

#ifndef MOTION_H_
#define MOTION_H_

// Initialize the PIR motion sensor
void pirInit();

// Detect motion
bool pirDetect();

#endif /* MOTION_H_ */
