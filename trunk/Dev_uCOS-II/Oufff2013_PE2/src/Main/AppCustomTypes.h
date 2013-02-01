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
#define	WAIT_FOREVER							0		// Define for Timeout

#define RIGHT_WHEEL								1		// Used for pivot mode
#define LEFT_WHEEL								-1		// Used for pivot mode

#define USE_CURRENT_VALUE						-9999	// Used into Cmd msg to indicate we want to keep current value 
														// (Thus no computation is needed for this param)

#define CURRENT_STATE__MOVING					0		// Current Odo state is moving
#define CURRENT_STATE__STOP    					1       // Current Odo state is set to "STOP"

#define HOLDER_CLOSE							0		// default value, holder is fully closed
#define HOLDER_OPEN_LEFT_ONLY					1		// Left Holder is open, right is closed
#define HOLDER_OPEN_RIGHT_ONLY					2		// Right Holder is open, left is closed
#define HOLDER_OPEN								3		// All is opened
#define HOLDER_GRAB								4		// Holder is grabbing an object (try to lock the object)
#define HOLDER_GRIP								5		// Holder is holding an object to hang it

#define HOLDER_LEVEL_LOW						0		// default value, holder is on Low position
#define HOLDER_LEVEL_MIDDLE						1		// Holder is on Middle position
#define HOLDER_LEVEL_HIGH						2		// Holder is on High position

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
	c_ColorA,					// Red
	c_ColorB,					// Purple
}EnumColor;

// Mvt Mode -------------------------------------
typedef enum
{
	NotSet							= 0,	// Current Cmd is not set
	// Command MVT ______________________
	Mvt_UseAngleOnly				= 10,	// Use Asser Mode 1
	Mvt_UseDistOnly					= 11,	// Use Asser Mode 2
	Mvt_UseMixedMode				= 12,	// Use Asser Mode 3
	Mvt_UsePivotMode				= 13,	// Use Asser Mode 4
	MvtSimple_MoveInMM				= 14,	// Use a simple mvt for moving in MM (don't divide this mvt)
	MvtSimple_RotateInDeg			= 15,	// Use a simple mvt for rotating in deg (don't divide this mvt)
	MvtSimple_RotateToAngleInDeg	= 16,	// Use a simple mvt for rotating to a specified angle in deg (don't divide this mvt)
	Mvt_Stop						= 17,   // Used to stop current mvt
	Mvt_UseSpline					= 18,	// Send the expected point directly to the task asser (without computation)
	// Command APP ______________________
	App_Wait						= 20,	// Wait (if all params = 0, wait for ever)
	App_IfGoto_System				= 21,	// Go to a specific step based on System Flags
	App_IfGoto_Strategy				= 22,	// Go to a specific step based on Strategy Flags
	App_SetNewPos					= 23,	// Msg used to define a new position
	App_SetStrategyFlags			= 24,	// Set the stratgey flags
	// Command SENSORS __________________
	Sensors_ArmsOpenDown			= 30,	// Open arms (Use for grabbing objects on floor)
	Sensors_ArmsOpenUp				= 31,	// Open arms (Use for grabbing objects on totem)
	Sensors_ArmsDeployment			= 32,	// Open arms with delay 
	Sensors_ArmsOpenTotem			= 33,	// Open arms (Use for the arrival on the totem) 
	Sensors_ArmsOpenOneCD			= 34,	// Open arms (Use for grabbing only one CD)
	Sensors_ArmsClose				= 35,	// Close arms (Use for keeping objects into arms)
	Sensors_ArmsCloseTotem			= 36,	// Close arms (Use for keeping objects into arms on totem (elevator is up))
	Sensors_ArmsUngrab				= 37,	// Open arms for putting down the objects
	Sensors_ElevatorLow				= 38,	// Set the elevator level to low
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
	Msg_Sensor_OpenClamp,		// Msg to TaskSensor to open Clamp
}EnumMsg;

// STRUCT ################################################################
// Position -------------------------------------
typedef struct
{
	float 			x;					// Pos en x
	float 			y;					// Pos en y
	float 			angle;				// Angle
	CPU_INT16U		right_encoder;		// Right wheel position for mode 4
	CPU_INT16U		left_encoder;		// Left wheel position for mode 4
	unsigned char	CurrentState;		// Flag for storing current mvt state
}StructPos;

typedef struct
{
	EnumCmd		Cmd;			        // Mode used to go to next major point
	int 			Param1;				
	float 			Param2;				
	float 			Param3;				
	float			Param4;				
	unsigned int 	ActiveSensorsFlag;	// Define which sensors we have to use for this movement (if not set, use all external sensors)
	EnumCmdType	CmdType;				// Var to define if the current action is a blocking action or not
}StructCmd;

typedef struct
{
    BOOLEAN		IsRead;				// Read Flag
    EnumCmd		Cmd;				// Command for the current Msg
	EnumCmdType	CmdType;			// Var to define if the current action is a blocking action or not
	INT8U		Param1;						
}StructMsg;

#endif // APPCUSTOMTYPES_H
