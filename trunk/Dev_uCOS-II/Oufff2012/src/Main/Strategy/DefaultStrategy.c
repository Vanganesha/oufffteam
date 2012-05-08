/*
*********************************************************************************************************
*                                         Association OufffTEAM
*                                     Coupe de France de Robotique
*
*
* File : DefaultStrategy.c
*
* Suivi de version :
* 2011-05-10 | PBE | Creation de la version de base pour la coupe 2011
*********************************************************************************************************
*/

#include "../Strategy.h"

#ifdef DEFAULT_STRATEGY_ENABLED

// ------------------------------------------------------------------------------------------------
INT8U Strategy_GetInitCmd(EnumColor CurrentColor, StructCmd *InitCmd)
{
	if(NULL == InitCmd)
		return ERR__INVALID_PARAM;

	switch(CurrentColor)
	{
	case c_ColorA:	// Red ############################################# 
		InitCmd->Cmd				= App_SetNewPos;
		InitCmd->CmdType			= CmdType_NonBlocking;
		InitCmd->Param2				= 065.0;
		InitCmd->Param3				= 233.0;
		InitCmd->Param4				= AppConvertDegInRad(0.0);
		InitCmd->ActiveSensorsFlag	= APP_PARAM_APPFLAG_NONE;
		break;

	case c_ColorB:	// Purple ##############################################
		InitCmd->Cmd				= App_SetNewPos;
		InitCmd->CmdType			= CmdType_NonBlocking;
		InitCmd->Param2				= 1000.0;
		InitCmd->Param3				= 1000.0;
		InitCmd->Param4				= AppConvertDegInRad(180.0);
		InitCmd->ActiveSensorsFlag	= APP_PARAM_APPFLAG_NONE;
		break;

	default:		// ##################################################
		InitCmd = NULL;
		return ERR__INVALID_PARAM;
		break;
	}

	return ERR__NO_ERROR;
}



// ------------------------------------------------------------------------------------------------
INT8U Strategy_GetNextAction(EnumColor CurrentColor, StructCmd *NextAction)
{
	static unsigned int 	CurrentActionID = 0;
	INT8U 					Err = 0;
	OS_FLAGS				CurrentFlag = 0;
	StructCmd 				*p = NextAction;

	if(NULL == p)
		return ERR__INVALID_PARAM;

	// Set all actions as blocking actions
	p->CmdType = CmdType_Blocking;

	switch(CurrentColor)
	{
	case c_ColorA:	// Red #############################################################
		switch(CurrentActionID)
		{
		default:
			return ERR__NO_MORE_DATA_AVAILABLE;
			break;
		}

	break;

	case c_ColorB:	// Purple ##############################################################
		switch(CurrentActionID)
		{

		default:
			return ERR__NO_MORE_DATA_AVAILABLE;
			break;
		}

	break;

	default:		// Not Set ##########################################################
	break;
	}

	// Check for Wait command --------------------------------------------
	if(App_Wait == p->Cmd)
	{
		// Execute the wait command
		OSTimeDlyHMSM(p->Param1, p->Param2, p->Param3, p->Param4);
		return Strategy_GetNextAction(CurrentColor, p);
	}

	// Check for conditionnal command ------------------------------------
	if(App_IfGoto == p->Cmd)
	{
		if(p->Param1)
			CurrentActionID = p->Param2;
		else
			CurrentActionID = p->Param3;

		return Strategy_GetNextAction(CurrentColor, p);
	}
	
	// Create the MvtSimple Command --------------------------------------
	if(MvtSimple_MoveInMM == p->Cmd) 
		LibMoving_MoveInMM(p->Param2, p->Param1, p);

	if(MvtSimple_RotateInDeg == p->Cmd)
		LibMoving_RotateInDeg(p->Param2, p->Param1, p);
	
	if(MvtSimple_RotateToAngleInDeg == p->Cmd)
		LibMoving_RotateToAngleInDeg(p->Param2, p->Param1, p);
	

	return ERR__NO_ERROR;
}

#endif