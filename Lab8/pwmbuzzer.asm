;
 ; pwmbuzzer.asm
 ;
 ;  Created on: Nov 2, 2018
 ;      Author: chuangyizhang
 ;
; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmbuzzer.h"

                .text
; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmbuzzer.h"

                .text

; Field and any other declarations
TIMER0_PERIPH   .field  SYSCTL_PERIPH_WTIMER0

TIMER1_PERIPH   .field	SYSCTL_PERIPH_GPIOC

TIMER0          .field  WTIMER0_BASE

PORTC           .field  GPIO_PORTC_BASE

PIN_ROUTE_4     .field  GPIO_PC4_WT0CCP0

                .asmfunc
buzzerInit      PUSH    {lr}

                LDR     r0, TIMER0_PERIPH
                BL      SysCtlPeripheralEnable

                LDR     r0, TIMER1_PERIPH
                BL      SysCtlPeripheralEnable

                LDR     r0, PORTC
                MOV     r1, #GPIO_PIN_4
                BL      GPIOPinTypeTimer

                LDR     r0, PIN_ROUTE_4
                BL      GPIOPinConfigure

                LDR     r0, TIMER0
                MOV     r1, #TIMER_CFG_SPLIT_PAIR
                ORR     r1, #TIMER_CFG_A_PWM
                BL      TimerConfigure

                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                MOV     r2, #true
                BL      TimerControlLevel

                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                MOV     r2, #200
                BL      TimerLoadSet

                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                MOV     r2, #0
                BL      TimerMatchSet

                LDR     r0, TIMER0,
                MOV     r1, #TIMER_A
                BL      TimerEnable


                POP     {pc}
                .endasmfunc

; void buzzerPwmSet(uint32_t pulse, uint32_t period)
                .asmfunc
buzzerPwmSet       PUSH    {lr, r0, r1}

                MOV		r3, r0
                MOV     r2, r1
                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                BL      TimerLoadSet

                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                MOV     r2, r3
                BL      TimerMatchSet


                ADD     sp, #8
                POP     {pc}
                .endasmfunc
