#if defined(__dsPIC30F__) 
#include <p30fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#endif

#include "smpscmp.h"
#ifdef _AC3IF


/*************************************************************************
* Function Name     : SetSmpsCMPRef3
* Description       : This function sets the comparator 3 reference voltage 
*                     (CMREF * INTREF/1024) or (CMREF * (AVDD/2)/1024) depending on Range bit or 
*                     (CMREF * EXTREF/1024) volts depending on EXTREF bit
* Parameters        : unsigned int config
* Return Value      : None 
**************************************************************************/

void SetSmpsCMPRef3(unsigned int config)
{
    CMPDAC3   = config ; 
	
}
#else
#warning "Does not build on this target"
#endif
 
