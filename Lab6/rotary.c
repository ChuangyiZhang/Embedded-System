#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/adc.h>
#include "launchpad.h"
#include "rotary.h"



void
ADCInit()
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
                              ADC_CTL_CH5);

    ADCSequenceStepConfigure(ADC0_BASE, 0, 1,
                             ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2);

    ADCSequenceEnable(ADC0_BASE, 0);
}



void
ADCsampling(uint32_t ADCSamplingArray[2])
{
    ADCProcessorTrigger(ADC0_BASE, 0);
    while(!ADCIntStatus(ADC0_BASE, 0, false))
    {
    }
    ADCIntClear(ADC0_BASE, 0);

    ADCSequenceDataGet(ADC0_BASE, 0, ADCSamplingArray);


}






