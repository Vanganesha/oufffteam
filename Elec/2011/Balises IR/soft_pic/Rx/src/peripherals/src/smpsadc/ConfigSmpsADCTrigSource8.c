#if defined(__dsPIC30F__) 
#include <p30fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#endif

#include "smpsadc.h"
#ifdef _ADCP8IF


/*************************************************************************
* Function Name     : ConfigSmpsADCTrigSource8
* Description       : This function enables/ disables common interrupt generation and  
*                     configures the ADC trigger source for channels AN17 and AN16                     
* Parameters        : unsigned int adc_common_intp- 0: disabled, 1: enabled
*                   ; unsigned int config1- Trigger Source
* Return Value      : None 
**************************************************************************/

void ConfigSmpsADCTrigSource8(unsigned int adc_common_intp, unsigned int config1)

 {
   	/* Enable/Disable the ADC common Interrupt request for Analog ch AN17 and AN16 */
	SetCommonIntSmpsADCPair8(adc_common_intp);

	/* Enables to select the Trigger source for conversion of Analog ch AN17 and AN16 */
	ADCPC4bits.TRGSRC8  = (config1 & 0x001F); 

 }
#else
#warning "Does not build on this target"
#endif
 
