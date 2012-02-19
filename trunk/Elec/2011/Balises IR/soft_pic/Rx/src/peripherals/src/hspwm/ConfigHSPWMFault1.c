#include <hspwm.h>

#if (defined (__PIC24E__) || defined (__dsPIC33E__)) && defined (_PWM1IF)
/*************************************************************************
* Function Name     : ConfigHSPWMFault1
* Description       : This function configures the different PWM1 pair faults by writing 
*                     into the FCLCON1 register. The current limit control signal and Fault 
*                     control signal sources the generation of fault.                        
* Parameters        : unsigned int config1
* Return Value      : None 
**************************************************************************/

void ConfigHSPWMFault1(unsigned int config1)
{
    FCLCON1 = config1;
}
#else
#warning "Does not build on this target"
#endif
 