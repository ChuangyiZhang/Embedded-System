;
; Lab 4, ECE 266, Spring 2019
;
; Assembly functions related to buzzer operations, using direct I/O register access (bypassing TivaWare).
; This version also uses the "bit banding" feature of ARM GPIO ports.
;
; Created by Zhao Zhang
;

; To include names declared in C
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","inc/hw_gpio.h","driverlib/gpio.h","buzzer.h"

                .text
                .global buzzerOn
                .global buzzerOff

; Calculate the bit banding address for the BUZZER's data pin
; The buzzer is assumed to be connected to J17, and then the buzzer's
; signal pin is connected to PC4.
BUZZER_PORT             .equ    GPIO_PORTC_BASE
BUZZER_PIN              .equ    GPIO_PIN_5
BUZZER_PIN_DATA_PTR     .field  BUZZER_PORT + GPIO_O_DATA + (BUZZER_PIN << 2)

;
; void buzzOn(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
buzzerOn        ; Write 1 to the GPIO pin that the buzzer uses:
                ;   In C: BUZZER_PIN_DATA = BUZZER_PIN
                LDR     r0, BUZZER_PIN_DATA_PTR     ; load the memory address for the data bin used by the buzzer
                MOV     r1, #BUZZER_PIN     ; In this mask, the pin's data is 1
                STRB    r1, [r0]            ; Write 1 to the bin
                BX      lr                  ; return

;
; void buzzOff(): Turn off the buzzer. It alls GPIOPinWrite() to write 0 to the pin
;
buzzerOff       ; Write 0 to the GPIO pin that the buzzer uses
                ;   In C: BUZZER_DATA &= ~BUZZER_BIN
                LDR     r0, BUZZER_PIN_DATA_PTR     ; load the memory address for the Port's data register
                MOV     r1, #0              ; In this mask, the pin's data is 0
                STRB    r1, [r0]            ; Write 1 to the bin
                BX      lr
