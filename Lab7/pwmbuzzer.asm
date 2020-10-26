;
 ; pwmbuzzer.asm
 ;
 ;  Created on: Nov 2, 2018
 ;      Author: chuangyizhang
 ;
; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmbuzzer.h"

                .text

TIMER0_PERIPH		.field	SYSCTL_PERIPH_WTIMER0
PORTC_PERIPH		.field	SYSCTL_PERIPH_GPIOC
TIMER0				.field	WTIMER0_BASE

PORTC				.field	GPIO_PORTC_BASE
PIN_ROUTE_1			.field  GPIO_PC4_WT0CCP0


                .asmfunc
pwmbuzzerInit		PUSH	{lr}

					LDR		r0, TIMER0_PERIPH
					BL		SysCtlPeripheralEnable

					LDR		r0, PORTC_PERIPH
					BL		SysCtlPeripheralEnable

					LDR		r0, PORTC
					MOV		r1, #(GPIO_PIN_4)
					BL		GPIOPinTypeTimer

					LDR		r0 ,PIN_ROUTE_1
					BL		GPIOPinConfigure

					LDR		r0, TIMER0
					MOV		r1, #TIMER_CFG_SPLIT_PAIR
					ORR		r1, #TIMER_CFG_A_PWM
					BL		TimerConfigure

					LDR		r0, TIMER0
					MOV		r1, #TIMER_A
					MOV		r2, #true
					BL		TimerControlLevel

					LDR		r0, TIMER0
					MOV		r1, #TIMER_A
					MOV		r2, #200
					BL		TimerLoadSet

					LDR		r0, TIMER0
					MOV		r1, #TIMER_A
					MOV		r2, #0
					BL		TimerMatchSet

					LDR		r0, TIMER0
					MOV		r1, #TIMER_A
					BL		TimerEnable

					POP		{pc}
					.endasmfunc


					.asmfunc
buzzerPwmSet		PUSH	{lr,r0}

					MOV		r2, r1
					LDR		r0, TIMER0
					MOV		r1, #TIMER_A
					BL		TimerLoadSet


					LDR		r0, TIMER0
					MOV		r1, #TIMER_A
					POP		{r2}
					BL		TimerMatchSet

					POP		{pc}
					.endasmfunc






