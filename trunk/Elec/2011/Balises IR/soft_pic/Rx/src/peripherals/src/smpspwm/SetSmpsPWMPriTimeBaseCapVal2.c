#if defined(__dsPIC30F__) 
#include <p30fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#endif

#if defined(__dsPIC33F__)

#include "smpspwm.h"
#ifdef _ADCP0IF
#ifdef _PWM2IF 


/*************************************************************************
* Function Name     : SetSmpsPWMPriTimeBaseCapVal2
* Description       : This function captures the independent time base value when a 
*					  leading edge is detected on the current-limit input and LEB processing 
*					  on the current-limit input signal is completed                       
* Parameters        : unsigned int capture_val
* Return Value      : None 
**************************************************************************/

void SetSmpsPWMPriTimeBaseCapVal2(unsigned int capture_val )
{
    PWMCAP2 = capture_val ; 
	
}
#else
#warning "Does not build on this target"
#endif
#endif 

#endif