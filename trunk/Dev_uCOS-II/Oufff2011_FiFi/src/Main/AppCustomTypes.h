/*
*********************************************************************************************************
*                                         Association OufffTEAM
*                                     Coupe de France de Robotique
*
* File : AppCustomTypes.h
*********************************************************************************************************
*/

#ifndef APPCUSTOMTYPES_H
#define APPCUSTOMTYPES_H

// DEFINE ################################################################
#define	WAIT_FOREVER			0		// Define for Timeout

#define RIGHT_WHEEL				1		// Used for pivot mode
#define LEFT_WHEEL				-1		// Used for pivot mode

#define USE_CURRENT_VALUE		-9999	// Used into Cmd msg to indicate we want to keep current value 
										// (Thus no computation is needed for this param)

#define CURRENT_STATE__STOP     0       // Current Odo state is set to "STOP"

// ERROR CODES ###########################################################
// Global ---------------------------------------
#define ERR__NO_ERROR							0				// No error
#define ERR__INVALID_PARAM						1				// Parameter given is not valid
#define ERR__NO_MORE_DATA_AVAILABLE				2				// There is no more data


// ENUM ##################################################################
// Color ----------------------------------------
typedef enum
{
	c_NotSet = -1,
	c_ColorA,					// Blue
	c_ColorB,					// Red
}EnumColor;

// Mvt Mode -------------------------------------
typedef enum
{
	Cmd_NotSet = 0,	            // Current Cmd is not set
	Mvt_UseAngleOnly,		    // Use Asser Mode 1
	Mvt_UseDistOnly,			// Use Asser Mode 2
	Mvt_UseMixedMode,			// Use Asser Mode 3
	Mvt_UsePivotMode,			// Use Asser Mode 4
	Mvt_Simple,				    // Used a simple mvt (don't divide this mvt)
    Mvt_Stop,                   // Used to stop current mvt
	App_SetNewPos,				// Msg used to define a new position
	Wait,       		        // Wait (if all params = 0, wait for ever)
}EnumCmd;

typedef enum
{
	CmdType_NotSet = 0,	        // Command type is not set
	CmdType_Blocking,			// Command is a blocking action
	CmdType_NonBlocking,		// Command is a non-blocking action
}EnumCmdType;

typedef enum
{
    Msg_NotSet = 0,             // Current Msg is not set
}EnumMsg;

// STRUCT ################################################################
// Position -------------------------------------
typedef struct
{
	float 		x;					// Pos en x
	float 		y;					// Pos en y
	float 		angle;				// Angle
	CPU_INT16U	right_encoder;		// Right wheel position for mode 4
	CPU_INT16U	left_encoder;		// Left wheel position for mode 4
	char		CurrentState;		// Flag for storing current mvt state
}StructPos;

typedef struct
{
	EnumCmd		Cmd;		        // Mode used to go to next major point
	// MvtMode Type					   Simple			| Wait  	| UseAngleOnly	| UseDistOnly	| UseMixedMode	| UsePivotMode	| App_SetNewPos	|
	short 		Param1;				// Speed(1-100)		| Not Used  | Speed(1-100)	| Speed(1-100)	| Speed(1-100)	| Speed(1-100)	| Not Used		|
	float 		Param2;				// x				| Not Used	| Not Used		| x				| x				| Wheel to lock	| x				|
	float 		Param3;				// y				| Not Used	| Not Used		| y				| y				| Not Used		| y				|
	float		Param4;				// angle			| Not Used	| angle			| Not Used		| angle			| angle			| angle			|
	int 		ActiveSensorsFlag;	// Define which sensors we have to use for this movement (if not set, use all external sensors)
	EnumCmdType	CmdType;			// Var to define if the current action is a blocking action or not
}StructCmd;

typedef struct
{
    EnumMsg Message;
    BOOLEAN IsRead;
}StructMsg;

#endif // APPCUSTOMTYPES_H