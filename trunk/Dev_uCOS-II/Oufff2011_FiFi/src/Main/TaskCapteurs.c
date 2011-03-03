/*
*********************************************************************************************************
*                                         Association OufffTEAM
*                                     Coupe de France de Robotique
*
*
* File : TaskCapteurs.c
*
* Suivi de version :
* 2009-02-11 | PBE | Creation de la version de base pour la coupe 2010
*********************************************************************************************************
*/

#include "TaskCapteurs.h"
#include "AppGlobalVars.h"

// ------------------------------------------------------------------------------------------------
BOOLEAN TaskCapteurs_IsStartButtonPressed()
{
	if(START_State()==0) return OS_TRUE;
	else return OS_FALSE;
}

// ------------------------------------------------------------------------------------------------
void TaskCapteurs_CheckBumpers()
{
	char uart_buffer[8];
	char * buffer_ptr;
	INT8U	Err = 0;						// Var to get error status
	CPU_INT16U  GP2Data;

	//GP2_1 : Front *************************************************
	GP2Data  = ADC_GetVal (GP2_1);
	//putsUART2("GP2_1 : ");
	//buffer_ptr = (char*) Str_FmtNbr_32 ((CPU_FP32) GP2Data, (CPU_INT08U) 4, (CPU_INT08U) 1, (CPU_BOOLEAN) DEF_YES, (CPU_BOOLEAN) DEF_YES, uart_buffer);
	//putsUART2(buffer_ptr);
	//putsUART2("  ");

	// Check Value
#ifdef APP_GP2D2_LIMIT_FRONT
	if(GP2Data > APP_GP2D2_LIMIT_FRONT)
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_GP2D2_FRONT, OS_FLAG_SET, &Err); 
	else
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_GP2D2_FRONT, OS_FLAG_CLR, &Err); 
#endif

	//GP2_2 : Back **************************************************
	GP2Data  = ADC_GetVal (GP2_2);
//	putsUART2("GP2_2 : ");
//	buffer_ptr = (char*) Str_FmtNbr_32 ((CPU_FP32) GP2Data, (CPU_INT08U) 4, (CPU_INT08U) 1, (CPU_BOOLEAN) DEF_YES, (CPU_BOOLEAN) DEF_YES, uart_buffer);
//	putsUART2(buffer_ptr);
//	putsUART2("  ");

	// Check Value
#ifdef APP_GP2D2_LIMIT_BACK
	if(GP2Data > APP_GP2D2_LIMIT_BACK)
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_GP2D2_BACK, OS_FLAG_SET, &Err); 
	else
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_GP2D2_BACK, OS_FLAG_CLR, &Err); 
#endif

	//GP2_3 : Not Used **********************************************
	//GP2Data  = ADC_GetVal (GP2_3);
	//putsUART2("GP2_3 : ");
	//buffer_ptr = (char*) Str_FmtNbr_32 ((CPU_FP32) GP2Data, (CPU_INT08U) 4, (CPU_INT08U) 1, (CPU_BOOLEAN) DEF_YES, (CPU_BOOLEAN) DEF_YES, uart_buffer);
	//putsUART2(buffer_ptr);
	//putsUART2("  ");
	
	//GP2_4 : Not Used **********************************************
	//GP2Data  = ADC_GetVal (GP2_4);
	//putsUART2("GP2_4 : ");
	//buffer_ptr = (char*) Str_FmtNbr_32 ((CPU_FP32) GP2Data, (CPU_INT08U) 4, (CPU_INT08U) 1, (CPU_BOOLEAN) DEF_YES, (CPU_BOOLEAN) DEF_YES, uart_buffer);
	//putsUART2(buffer_ptr);
	//putsUART2("  ");
		
	//GP2_5 : Not Used **********************************************
	//GP2Data  = ADC_GetVal (GP2_5);
	//putsUART2("GP2_5 : ");
	//buffer_ptr = (char*) Str_FmtNbr_32 ((CPU_FP32) GP2Data, (CPU_INT08U) 4, (CPU_INT08U) 1, (CPU_BOOLEAN) DEF_YES, (CPU_BOOLEAN) DEF_YES, uart_buffer);
	//putsUART2(buffer_ptr);
	//putsUART2("\n");

	return;
}

void TaskCapteurs_CheckClicClic()
{
	INT8U	Err = 0;						// Var to get error status

	// CLIC_1 : Front Left ******************************************
	if(CLIC_state(CLIC_1))	
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_BUMPER_CLIC1, OS_FLAG_SET, &Err); 
	else
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_BUMPER_CLIC1, OS_FLAG_CLR, &Err); 

	// CLIC_2 : Front Right *****************************************
	if(CLIC_state(CLIC_2))
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_BUMPER_CLIC2, OS_FLAG_SET, &Err); 
	else
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_BUMPER_CLIC2, OS_FLAG_CLR, &Err); 

	// CLIC_3 : Front Center ****************************************
	if(CLIC_state(CLIC_3))		
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_BUMPER_CLIC3, OS_FLAG_SET, &Err); 
	else
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_BUMPER_CLIC3, OS_FLAG_CLR, &Err); 

	// CLIC_4 : Back Center *****************************************
	if(CLIC_state(CLIC_4))		
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_BUMPER_CLIC4, OS_FLAG_SET, &Err); 
	else
		OSFlagPost(AppFlags, APP_PARAM_APPFLAG_BUMPER_CLIC4, OS_FLAG_CLR, &Err); 
}

// ------------------------------------------------------------------------------------------------
void TaskCapteurs_ReadColor()
{
	#ifdef _TARGET_STARTER_KIT
		AppCurrentColor = c_Blue;	
	#else
		// Read current color
		if(COLOR_Read() == 0)
			AppCurrentColor = c_Blue;
		else	
			AppCurrentColor = c_Yellow;
	#endif

	return;
}

// ------------------------------------------------------------------------------------------------
// TaskCapteurs_Main()
// ------------------------------------------------------------------------------------------------
void TaskCapteurs_Main(void *p_arg)
{
	INT8U	Err = 0;				// Var to get error status
	CPU_INT16U  GP2Data;

	putsUART2("OUFFF TEAM 2010 : Capteurs online\n");

	// We set the current color
	OSTimeDlyHMSM(0, 0, 0, 500);
	TaskCapteurs_ReadColor();

	#ifdef _TARGET_STARTER_KIT
		OSTimeDlyHMSM(0, 0, 0, 500);	LED_Off(1);	
		OSTimeDlyHMSM(0, 0, 0, 500);	LED_Off(2);	
		OSTimeDlyHMSM(0, 0, 0, 500);	LED_Off(3);
	#else
	// Comment this part to disable start button detection
		// We're waiting for Start button release
		while(OS_FALSE == TaskCapteurs_IsStartButtonPressed())
			OSTimeDly(1);	// Release proc

		// We're waiting for start button activation
		while(OS_TRUE == TaskCapteurs_IsStartButtonPressed())
			OSTimeDly(1);	// Release proc 
	#endif

	// StartButton has been pressed
	
	OSFlagPost(AppFlags, APP_PARAM_APPFLAG_START_BUTTON, OS_FLAG_SET, &Err); 

	while(OS_TRUE)	// Main task: we check all other sensors
	{
		OSTimeDlyHMSM(0, 0, 0, 10);	

		// FiFi - 02/01/11 : Modif pour test asser
		{
			//TaskCapteurs_CheckBumpers();			// Check for bumpers status
			//TaskCapteurs_CheckClicClic();			// Check for clic clic
		}
	}
}
