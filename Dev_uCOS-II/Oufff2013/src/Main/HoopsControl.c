#include "AppIncludes.h"
#include "HoopsControl.h"

// HOOPS ---------------------------------------------------------------------------------
void HOOPS_InitReg(void)
{
	// Alarm: All alarms
	AX12_Write_Reg_Sync(AX12_LEFT_ARM_ID, AX12_REG_ALARM_SHUTDOWN, 0x7F);
	AX12_Write_Reg_Sync(AX12_RIGHT_ARM_ID, AX12_REG_ALARM_SHUTDOWN, 0x7F);

	// Temperature limit: 70�C = 0x46
	AX12_Write_Reg_Sync(AX12_LEFT_ARM_ID, AX12_REG_TEMPERATURE_LIMIT, 0x46);
	AX12_Write_Reg_Sync(AX12_RIGHT_ARM_ID, AX12_REG_TEMPERATURE_LIMIT, 0x46);
		
	// Max Torque : 768 = 0x0300
	AX12_Write_Reg_2_Datas_Sync(AX12_LEFT_ARM_ID, AX12_REG_MAX_TORQUE, 0x00, 0x03);
	AX12_Write_Reg_2_Datas_Sync(AX12_RIGHT_ARM_ID, AX12_REG_MAX_TORQUE, 0x00, 0x03);
	
	// Torque Limit : 768 = 0x0300
	AX12_Write_Reg_2_Datas_Sync(AX12_LEFT_ARM_ID, AX12_REG_TORQUE_LIMIT, 0x00, 0x03);
	AX12_Write_Reg_2_Datas_Sync(AX12_RIGHT_ARM_ID, AX12_REG_TORQUE_LIMIT, 0x00, 0x03);

	// CW limits
	AX12_Write_Reg_2_Datas_Sync(AX12_LEFT_ARM_ID, AX12_REG_CW_LIMIT, 0xFC, 0x01);
	AX12_Write_Reg_2_Datas_Sync(AX12_RIGHT_ARM_ID, AX12_REG_CW_LIMIT, 0xF0, 0x00);

	// CCW limits
	AX12_Write_Reg_2_Datas_Sync(AX12_LEFT_ARM_ID, AX12_REG_CCW_LIMIT, 0x50, 0x03);
	AX12_Write_Reg_2_Datas_Sync(AX12_RIGHT_ARM_ID, AX12_REG_CCW_LIMIT, 0x45, 0x02);
}

void HOOPS_SetSpeed(void)
{
	AX12_Write_Reg_2_Datas_Sync(AX12_LEFT_HOOP_ID, AX12_REG_SPEED, 0x90, 0x01);
	AX12_Write_Reg_2_Datas_Sync(AX12_RIGHT_HOOP_ID, AX12_REG_SPEED, 0x90, 0x01);
}

void HOOPS_Up(void)
{
	HOOP_Back_Up();
//	OSTimeDlyHMSM(0, 0, 1, 0);
}

void HOOPS_Down(void)
{
	HOOP_Back_Down();
//	OSTimeDlyHMSM(0, 0, 1, 0);
}
// HOOP Back ----------------------------------------------------------------------------
void HOOP_Back_Up(void)
{
//	AX12_Write_Torque_On_Sync (AX12_RIGHT_HOOP_ID);
// 	AX12_Write_Position_Sync(AX12_RIGHT_HOOP_ID, 0x02, 0x8F);
//	Right: 254 - 0x00FE Left: 831 - 0x0341
	AX12_Dual_Write_Torque_On_Sync (AX12_LEFT_HOOP_ID, AX12_RIGHT_HOOP_ID);
	AX12_Dual_Write_Position_Sync (AX12_LEFT_HOOP_ID, AX12_RIGHT_HOOP_ID, 0x02, 0x8F, 0x00, 0xFE);
{
}

void HOOP_Back_Down(void)
{
// 	AX12_Write_Torque_On_Sync (AX12_RIGHT_HOOP_ID);
// 	AX12_Write_Position_Sync(AX12_RIGHT_HOOP_ID, 0x01, 0x86);
//	Right: 570 - 0x023A Left: 519 - 0x0207
	AX12_Dual_Write_Torque_On_Sync (AX12_LEFT_HOOP_ID, AX12_RIGHT_HOOP_ID);
	AX12_Dual_Write_Position_Sync (AX12_LEFT_HOOP_ID, AX12_RIGHT_HOOP_ID, 0x02, 0x07, 0x02, 0x3A);
}
