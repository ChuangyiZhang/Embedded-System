;
; ECE 266, Lab 4, Spring 2019
;
; Function related to PIR motion sensor operations.
;
; Created by Zhao Zhang

; To include names declared in C
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","inc/hw_gpio.h","driverlib/pin_map.h","driverlib/gpio.h","driverlib/sysctl.h","motion.h"

                .text

; Calculate the address for the PIR motion sensor's data register
; The sensor is assumed to be connected to J16, and then the buzzer's
; signal pin is connected to PC4.
PIR_PORT            .equ    GPIO_PORTC_BASE
PIR_PIN             .equ    GPIO_PIN_4
PIR_PORT_DATA_PTR   .field  PIR_PORT + GPIO_O_DATA + (0xFF << 2)

;
; void pirDetect(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
pirDetect       .asmfunc
                ; Read GPIO pin that the PIR motion sensor uses:
                ;   In C: return PIR_PORT_DATA & PIR_PIN
                LDR     r0, PIR_PORT_DATA_PTR       ; load the address pointer
                LDRB    r0, [r0]                    ; load the 8-bit port data
                AND     r0, #PIR_PIN                ; clear the other bits
                BX      lr                          ; return
                .endasmfunc
