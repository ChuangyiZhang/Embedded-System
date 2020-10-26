/*
 * pwmbuzzer.h
 *
 *  Created on: Oct 30, 2018
 *      Author: chuangyizhang
 */

#ifndef PWMBUZZER_H_
#define PWMBUZZER_H_

void pwmbuzzerInit();

void buzzerPwmSet(int pulsewidth, int period);

#endif /* PWMBUZZER_H_ */
