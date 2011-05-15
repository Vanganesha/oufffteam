/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                             (c) Copyright 2006, Micrium, Inc., Weston, FL
*                                           All Rights Reserved
*
*                                          Board Support Package
*                                               Microchip
*                                              Explorer 16
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/

#define  BSP_CLK_FREQ               80E6

// TMR2 --> Odometer
#define  TMR2_TICK					10E-3
#define  TMR2_PRESCALER				256
#define  TMR2_RELOAD				TMR2_TICK*BSP_CLK_FREQ/TMR2_PRESCALER

// TMR3 --> PWM generation
#define  TMR3_FREQUENCY				30E3
#define  TMR3_TICK					1/TMR3_FREQUENCY
#define  TMR3_PRESCALER				1
#define  TMR3_RELOAD				TMR3_TICK*BSP_CLK_FREQ/TMR3_PRESCALER

#define  UART2_BAUDRATE				115200

#ifdef _TARGET_440H
#define SW1							10		// Switch 1
#define SW2							11		// Switch 2
#define SW3							12		// Switch 3
#define LED_USB						1		// Led USB
#define LED_SD						2		// Led SD
#define LED_FLASH					3		// Led Flash
#define LED_ERR						4		// Led Error
#else
#define CLIC_1						0		// connecteur 1
#define CLIC_2						1		// connecteur 2
#define CLIC_3						2		// connecteur 3
#define CLIC_4						3		// connecteur 4

#define GP2_1						11
#define GP2_2						10
#define GP2_3						8
#define GP2_4						6
#define GP2_5						4
#define GP2_6						2		//ne fonctionne pas pour le moment !! CBE
#endif

/*
*********************************************************************************************************
*                                             PROTOTYPES
*********************************************************************************************************
*/    

void        LED_Toggle          (CPU_INT08U led);
void        LED_Off             (CPU_INT08U led);
void        LED_On              (CPU_INT08U led);
void        LED_Init            (void);

void        BSP_ADCHandler      (void);
void        BSP_UARTHandler     (void);
void        BSP_CNHandler       (void);
void        BSP_TickISR_Handler (void);
void        BSP_Except_Handler  (void);

void        BSP_InitIO          (void);
void        BSP_IntDisableALL   (void);

void 		PWM_M0_SetDC(INT16U Duty);
void 		PWM_M1_SetDC(INT16U Duty);

void        BSP_Dly             (CPU_INT32U  us);

void PMP_Write(CPU_INT16U address, CPU_INT08U data);
CPU_INT08U PMP_Read(CPU_INT16U address);


void TMR2_Init(void);

void  ADC_Init (void);
CPU_INT16U  ADC_GetVal (CPU_INT08U channel_to_convert);