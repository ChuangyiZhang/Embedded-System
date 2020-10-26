;
; ECE 266, Lab 4, Spring 2019
;
; Function related to PIR motion sensor operations.
;
; Created by Zhao Zhang

; To include names declared in C
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/gpio.h","driverlib/sysctl.h","motion.h"

                .text

; GPIO peripheral, port base and pin mask value for the PIR motion sensor.
; The motion sensor is assumed to be connected to J16, and then the
; signal pin is connected to PC4.
PIR_PERIPH      .field  SYSCTL_PERIPH_GPIOC
PIR_PORT        .field  GPIO_PORTC_BASE
PIR_PIN         .equ    GPIO_PIN_4

;
; void pirDetect(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
pirDetect       PUSH		{LR}

                ; Read GPIO pin that the PIR motion sensor uses:
                ;   Call GPIOPinRead(BUZZ_PORT, BUZZ_PIN)
                LDR     r0, PIR_PORT
                MOV     r1, #PIR_PIN
                BL      GPIOPinRead

                POP     {PC}
