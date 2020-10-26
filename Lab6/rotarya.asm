

;rotary.c
 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h","driverlib/adc.h", "launchpad.h", "rotary.h"


                 .text
                .global ADCInit
                .global ADCsampling


***************************************************
ROTARY_PERIPH	.field		SYSCTL_PERIPH_ADC0
ROTARY_PORT		.field		ADC0_BASE
ROTARY_TRIG		.field		ADC_TRIGGER_PROCESSOR

ADCInit		PUSH		{LR}

			LDR			r0, ROTARY_PERIPH
			BL			SysCtlPeripheralEnable

			LDR			r0, ROTARY_PORT
			MOV			r1, #0
			LDR			r2, ROTARY_TRIG
			MOV			r3, #0
			BL			ADCSequenceConfigure

			LDR			r0,  ROTARY_PORT
			MOV			r1, #0
			MOV			r2, #0
			MOV			r3, #ADC_CTL_CH5
			BL			ADCSequenceStepConfigure

			LDR			r0,  ROTARY_PORT
			MOV			r1, #0
			MOV			r2, #1
			MOV			r3, #(ADC_CTL_IE  | ADC_CTL_END  | ADC_CTL_CH2)
			BL			ADCSequenceStepConfigure

			LDR			r0,	ROTARY_PORT
			MOV			r1, #0
			BL			ADCSequenceEnable

			POP			{PC}

ADCsampling		PUSH	{r0, LR}

				LDR		r0, ROTARY_PORT
				MOV		r1, #0
				BL		ADCProcessorTrigger

loop			LDR		r0, ROTARY_PORT
				MOV 	r1, #0
				MOV		r2, #false
				BL		ADCIntStatus
				CMP		r0, #0
				BEQ		loop

				LDR		r0, ROTARY_PORT
				MOV		r1, #0
				BL		ADCIntClear


				LDR		r0, ROTARY_PORT
				MOV		r1, #0
				POP		{r2}
				BL		ADCSequenceDataGet

				POP		{PC}
