/*
 * pwmbuzzer.h
 *
 *  Created on: Oct 30, 2018
 *      Author: chuangyizhang
 */

#ifndef PWMBUZZER_H_
#define PWMBUZZER_H_

void pwmbuzzerInit();

void buzzerPwmSet(uint32_t pulse, uint32_t period);

#endif /* PWMBUZZER_H_ */
