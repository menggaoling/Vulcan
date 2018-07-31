/******************** (C) COPYRIGHT 2009 Johnson Fitness Inc. ******************
* File Name          : platform_config.h
* Author             : 
* Version            : V1.0.0
* Date               : 01/13/2009
* Description        : VFS7700 UCB board specific configuration file.
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H



#define       _HARDWARE_A3_                       1 
//#define     _MECHINE_LIFE_TEST_                 1
//#define     _MECHINE_LIFE_TEST_NOTIMELIMIT_     1
#define       _FITNESS800_                        1

//#define     _BIKE_LIFE_TEST_                      1

#ifdef  _MECHINE_LIFE_TEST_
    #define   _MECHINE_LIFE_TEST_NOTIMELIMIT_     1
#endif


//#define       _DisableGetInclineLocation_          1

//#define     _TEST_POWER_TABLE_                  1
//#define     _RANDOM_RPM_                        1

//#define     VFS7700       1
/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x_lib.h"
/* Exported types ------------------------------------------------------------*/
struct LONG_WORD
{
  unsigned long		LowWord:16 ;
	unsigned long		HighWord:16 ;
} ;

struct	WORD_BYTE	{
	unsigned short		LowByte:8 ;
	unsigned short		HighByte:8 ;
} ;


struct	BIT16_BITS	{
	unsigned short		Bit0:1 ;
	unsigned short		Bit1:1 ;
	unsigned short		Bit2:1 ;
	unsigned short		Bit3:1 ;
	unsigned short		Bit4:1 ;
	unsigned short		Bit5:1 ;
	unsigned short		Bit6:1 ;
	unsigned short		Bit7:1 ;
	unsigned short		Bit8:1 ;
	unsigned short		Bit9:1 ;
	unsigned short		Bit10:1 ;
	unsigned short		Bit11:1 ;
	unsigned short		Bit12:1 ;
	unsigned short		Bit13:1 ;
	unsigned short		Bit14:1 ;
	unsigned short		Bit15:1 ;
} ;

struct	BIT8_BITS	{
	unsigned char	Bit0:1 ;
	unsigned char	Bit1:1 ;
	unsigned char	Bit2:1 ;
	unsigned char	Bit3:1 ;
	unsigned char	Bit4:1 ;
	unsigned char	Bit5:1 ;
	unsigned char	Bit6:1 ;
	unsigned char	Bit7:1 ;
} ;

struct	BIT4_BITS	{
	unsigned char	LowNibble:4 ;
	unsigned char	HighNibble:4 ;
} ;

//------------------------------------------------------------------------------
typedef	union		BIT8_REG	{
	unsigned char				all ;
	struct 	BIT8_BITS		Bits ;
  struct  BIT4_BITS   Nibbles ;
} Bu8 ;

// User Typedef Bit
typedef	union		BIT16_REG	{
	unsigned short				all ;
  //
  Bu8     BYTES[2] ;
  //
  struct	WORD_BYTE			Bytes ;	
  //
	struct 	BIT16_BITS		Bits ;
  //
} Bu16 ;


typedef	union		BIT32_REG	{
	unsigned long				all ;
  struct  LONG_WORD   WORDS ;
  //struct	WORD_BYTE   WORDS[2] ;
  struct 	BIT16_BITS	BIT_BYTES[2] ;
  Bu8                 BYTES[4] ;
} Bu32 ;

typedef	union		WORDTOBYTE_REG	{
	unsigned short				all ;
	struct	WORD_BYTE			Bytes ;	
  struct 	BIT16_BITS		Bits ;
  
} SReg ;




//
/* Exported constants --------------------------------------------------------*/
// Keyboard Scan Port
#define SKx_MODE              GPIO_Mode_Out_PP
#define SKx_SPEED             GPIO_Speed_10MHz
#define SK1_GPIOx             GPIOB
#define SK1                   GPIO_Pin_2  
#define SK2_GPIOx             GPIOB
#define SK2                   GPIO_Pin_1
#define SK3_GPIOx             GPIOB
#define SK3                   GPIO_Pin_0
#define SK4_GPIOx             GPIOC
#define SK4                   GPIO_Pin_5
#define SK5_GPIOx             GPIOC
#define SK5                   GPIO_Pin_4
#define SK6_GPIOx             GPIOA
#define SK6                   GPIO_Pin_7
#define SK7_GPIOx             GPIOA
#define SK7                   GPIO_Pin_5  
             
// Keyboard Data Port
#define DAx_MODE              GPIO_Mode_IN_FLOATING
#define DAx_SPEED             GPIO_Speed_10MHz
#define DA1_GPIOx             GPIOC
#define DA1                   GPIO_Pin_10  
#define DA2_GPIOx             GPIOC
#define DA2                   GPIO_Pin_11
#define DA3_GPIOx             GPIOC
#define DA3                   GPIO_Pin_12
#define DA4_GPIOx             GPIOD
#define DA4                   GPIO_Pin_2
#define DA5_GPIOx             GPIOB
#define DA5                   GPIO_Pin_5


// Heart Rate Input
#define HR1_GPIOx             GPIOB
#define HR1                   GPIO_Pin_8  
#define HR1_EXTI_LINE         EXTI_Line8
#define HR1_MODE              GPIO_Mode_IN_FLOATING
#define HR1_SPEED             GPIO_Speed_10MHz
#define HR1_PORT_SOURCE       GPIO_PortSourceGPIOB
#define HR1_PIN_SOURCE        GPIO_PinSource8
#define HR2_GPIOx             GPIOB
#define HR2                   GPIO_Pin_9
#define HR2_EXTI_LINE         EXTI_Line9
#define HR2_MODE              GPIO_Mode_IN_FLOATING
#define HR2_SPEED             GPIO_Speed_10MHz
#define HR2_PORT_SOURCE       GPIO_PortSourceGPIOB
#define HR2_PIN_SOURCE        GPIO_PinSource9
// C-SAFE Digital Communication
#define CSAFE_TX_GPIOx        GPIOB
#define CSAFE_TX              GPIO_Pin_10
#define CSAFE_TX_MODE         GPIO_Mode_AF_PP
#define CSAFE_TX_SPEED        GPIO_Speed_10MHz
#define CSAFE_RX_GPIOx        GPIOB
#define CSAFE_RX              GPIO_Pin_11
#define CSAFE_RX_MODE         GPIO_Mode_IN_FLOATING
#define CSAFE_RX_SPEED        GPIO_Speed_10MHz
#define CSAFE_CTS_GPIOx       GPIOC
#define CSAFE_CTS             GPIO_Pin_1
#define CSAFE_CTS_MODE        GPIO_Mode_IPU
#define CSAFE_CTS_SPEED       GPIO_Speed_10MHz

// JHT Digital Communication
#define JHT_DIR_GPIOx         GPIOA
#define JHT_DIR               GPIO_Pin_1
#define JHT_DIR_MODE          GPIO_Mode_Out_PP
#define JHT_DIR_SPEED         GPIO_Speed_10MHz
#define JHT_TX_GPIOx          GPIOA
#define JHT_TX                GPIO_Pin_2
#define JHT_TX_MODE           GPIO_Mode_AF_PP
#define JHT_TX_SPEED          GPIO_Speed_10MHz
#define JHT_RX_GPIOx          GPIOA
#define JHT_RX                GPIO_Pin_3
#define JHT_RX_MODE           GPIO_Mode_IN_FLOATING
#define JHT_RX_SPEED          GPIO_Speed_10MHz

// External TV Communication
#define TV_TX_GPIOx           GPIOA
#define TV_TX                 GPIO_Pin_9
#define TV_TX_MODE            GPIO_Mode_AF_PP
#define TV_TX_SPEED           GPIO_Speed_10MHz
#define TV_RX_GPIOx           GPIOA
#define TV_RX                 GPIO_Pin_10
#define TV_RX_MODE            GPIO_Mode_IN_FLOATING
#define TV_RX_SPEED           GPIO_Speed_10MHz
// Beep
#define BEEP_GPIOx            GPIOC
#define BEEP                  GPIO_Pin_3
#define BEEP_MODE             GPIO_Mode_Out_PP
#define BEEP_SPEED            GPIO_Speed_10MHz

// EEPROM
#define EEPROM_SPEED          GPIO_Speed_10MHz
#define EEPROM_DO_GPIOx       GPIOC
#define EEPROM_DO             GPIO_Pin_7
#define EEPROM_DO_MODE        GPIO_Mode_IPU
#define EEPROM_DI_GPIOx       GPIOC
#define EEPROM_DI             GPIO_Pin_8
#define EEPROM_DI_MODE        GPIO_Mode_Out_PP
#define EEPROM_SK_GPIOx       GPIOC
#define EEPROM_SK             GPIO_Pin_9
#define EEPROM_SK_MODE        GPIO_Mode_Out_PP
#define EEPROM_CS_GPIOx       GPIOA
#define EEPROM_CS             GPIO_Pin_8
#define EEPROM_CS_MODE        GPIO_Mode_Out_PP

// Battery
#define BATTERY_SPEED         GPIO_Speed_10MHz
#define BATTERY_CHECK_GPIOx   GPIOC
#define BATTERY_CHECK         GPIO_Pin_13
#define BATTERY_CHECK_MODE    GPIO_Mode_IN_FLOATING
#define BATTERY_POWER_GPIOx   GPIOC
#define BATTERY_POWER         GPIO_Pin_2
#define BATTERY_POWER_MODE    GPIO_Mode_Out_PP
#define BATTERY_PWM_GPIOx     GPIOB
#define BATTERY_PWM           GPIO_Pin_7
#define BATTERY_PWM_MODE      GPIO_Mode_Out_PP
#define BATTERY_ADC_GPIOx     GPIOC
#define BATTERY_ADC           GPIO_Pin_0
#define BATTERY_ADC_MODE      GPIO_Mode_AIN
// Analog Communication Control
#define ANALOG_PWM_GPIOx      GPIOA
#define ANALOG_PWM            GPIO_Pin_0
#define ANALOG_PWM_MODE       GPIO_Mode_AF_PP
#define ANALOG_PWM_SPEED      GPIO_Speed_10MHz
#define ANALOG_RPM_GPIOx      GPIOA
#define ANALOG_RPM            GPIO_Pin_6
#define ANALOG_RPM_MODE       GPIO_Mode_IN_FLOATING;
#define ANALOG_RPM_SPEED      GPIO_Speed_10MHz
// Burnin Test
#define BURNIN_TEST_GPIOx     GPIOA
#define BURNIN_TEST           GPIO_Pin_4
#define BURNIN_TEST_MODE      GPIO_Mode_IPU
#define BURNIN_TEST_SPEED     GPIO_Speed_10MHz
// ST2225A
#define ST2225A_SPEED         GPIO_Speed_10MHz
#define ST2225A_MODE          GPIO_Mode_Out_PP
#define ST2225A_EN_GPIOx      GPIOB
#define ST2225A_EN            GPIO_Pin_12
#define ST2225A_EN1_GPIOx     GPIOC
#define ST2225A_EN1           GPIO_Pin_6
#define ST2225A_CK_GPIOx      GPIOB
#define ST2225A_CK            GPIO_Pin_13
#define ST2225A_DA_GPIOx      GPIOB
#define ST2225A_DA            GPIO_Pin_14
#define ST2225A_DA1_GPIOx     GPIOB
#define ST2225A_DA1           GPIO_Pin_15
#define LED_SCAN89_MODE       GPIO_Mode_Out_OD
#define LED_SCAN8_GPIOx       GPIOA
#define LED_SCAN8             GPIO_Pin_12
#define LED_SCAN9_GPIOx       GPIOA
#define LED_SCAN9             GPIO_Pin_11
//
#define LED_SCAN07_SPEED      GPIO_Speed_10MHz
#define LED_SCAN07_MODE       GPIO_Mode_Out_PP
#define LED_SCAN07_GPIOx      GPIOB
#define LED_SCAN07            GPIO_Pin_6
//

//
#define GPIO_BIT_HI           Bit_SET
#define GPIO_BIT_LO           Bit_RESET

//

/* Exported macro ------------------------------------------------------------*/
// GPIO
#define SK1_Output(s)         {(s == 1)?GPIO_WriteBit(SK1_GPIOx,SK1,GPIO_BIT_HI):GPIO_WriteBit(SK1_GPIOx,SK1,GPIO_BIT_LO);}  
#define SK2_Output(s)         {(s == 1)?GPIO_WriteBit(SK2_GPIOx,SK2,GPIO_BIT_HI):GPIO_WriteBit(SK2_GPIOx,SK2,GPIO_BIT_LO);}
#define SK3_Output(s)         {(s == 1)?GPIO_WriteBit(SK3_GPIOx,SK3,GPIO_BIT_HI):GPIO_WriteBit(SK3_GPIOx,SK3,GPIO_BIT_LO);}
#define SK4_Output(s)         {(s == 1)?GPIO_WriteBit(SK4_GPIOx,SK4,GPIO_BIT_HI):GPIO_WriteBit(SK4_GPIOx,SK4,GPIO_BIT_LO);}
#define SK5_Output(s)         {(s == 1)?GPIO_WriteBit(SK5_GPIOx,SK5,GPIO_BIT_HI):GPIO_WriteBit(SK5_GPIOx,SK5,GPIO_BIT_LO);}
#define SK6_Output(s)         {(s == 1)?GPIO_WriteBit(SK6_GPIOx,SK6,GPIO_BIT_HI):GPIO_WriteBit(SK6_GPIOx,SK6,GPIO_BIT_LO);}
#define SK7_Output(s)         {(s == 1)?GPIO_WriteBit(SK7_GPIOx,SK7,GPIO_BIT_HI):GPIO_WriteBit(SK7_GPIOx,SK7,GPIO_BIT_LO);}     
#define DA1_Input(s)          s = GPIO_ReadInputDataBit(DA1_GPIOx,DA1)
#define DA2_Input(s)          s = GPIO_ReadInputDataBit(DA2_GPIOx,DA2)
#define DA3_Input(s)          s = GPIO_ReadInputDataBit(DA3_GPIOx,DA3)
#define DA4_Input(s)          s = GPIO_ReadInputDataBit(DA4_GPIOx,DA4)
#define DA5_Input(s)          s = GPIO_ReadInputDataBit(DA5_GPIOx,DA5)
//#define DA6_Input(s)          s = GPIO_ReadInputDataBit(DA6_GPIOx,DA6)
#define CSAFE_CTS_Input(s)    s = GPIO_ReadInputDataBit(CSAFE_CTS_GPIOx,CSAFE_CTS)
#define JHT_DIR_TXD           GPIO_SetBits(JHT_DIR_GPIOx,JHT_DIR)
#define JHT_DIR_RXD           GPIO_ResetBits(JHT_DIR_GPIOx,JHT_DIR)
#define JHT_DIR_Output(s)     {(s == 1)?GPIO_SetBits(JHT_DIR_GPIOx,JHT_DIR):GPIO_ResetBits(JHT_DIR_GPIOx,JHT_DIR);}
#define BEEP_ON               GPIO_SetBits(BEEP_GPIOx,BEEP)
#define BEEP_OFF              GPIO_ResetBits(BEEP_GPIOx,BEEP)
#define BEEP_Output(s)        {(s == 1)?GPIO_SetBits(BEEP_GPIOx,BEEP):GPIO_ResetBits(BEEP_GPIOx,BEEP,GPIO_BIT_LO);}
#define EEPROM_CS_Output(s)   {(s == 1)?GPIO_SetBits(EEPROM_CS_GPIOx,EEPROM_CS):GPIO_ResetBits(EEPROM_CS_GPIOx,EEPROM_CS);}
#define EEPROM_SK_Output(s)   {(s == 1)?GPIO_SetBits(EEPROM_SK_GPIOx,EEPROM_SK):GPIO_ResetBits(EEPROM_SK_GPIOx,EEPROM_SK);}
#define EEPROM_DI_Output(s)   {(s == 1)?GPIO_SetBits(EEPROM_DI_GPIOx,EEPROM_DI):GPIO_ResetBits(EEPROM_DI_GPIOx,EEPROM_DI);}
//#define EEPROM_DO_Input(s)    s = GPIO_ReadInputDataBit(EEPROM_DO_GPIOx,EEPROM_DO)
#define EEPROM_DO_Input       GPIO_ReadInputDataBit(EEPROM_DO_GPIOx,EEPROM_DO)

#define ST2225A_EN_Output(s)  {(s == 1)?GPIO_SetBits(ST2225A_EN_GPIOx,ST2225A_EN):GPIO_ResetBits(ST2225A_EN_GPIOx,ST2225A_EN);}
#define ST2225A_EN1_Output(s) {(s == 1)?GPIO_SetBits(ST2225A_EN1_GPIOx,ST2225A_EN1):GPIO_ResetBits(ST2225A_EN1_GPIOx,ST2225A_EN1);}
#define ST2225A_CK_Output(s)  {(s == 1)?GPIO_SetBits(ST2225A_CK_GPIOx,ST2225A_CK):GPIO_ResetBits(ST2225A_CK_GPIOx,ST2225A_CK);}
#define ST2225A_DA_Output(s)  {(s == 1)?GPIO_SetBits(ST2225A_DA_GPIOx,ST2225A_DA):GPIO_ResetBits(ST2225A_DA_GPIOx,ST2225A_DA);}
#define ST2225A_DA1_Output(s) {(s == 1)?GPIO_SetBits(ST2225A_DA1_GPIOx,ST2225A_DA1):GPIO_ResetBits(ST2225A_DA1_GPIOx,ST2225A_DA1);}
#define LED_SCAN8_Output(s)   {(s == 1)?GPIO_SetBits(LED_SCAN8_GPIOx,LED_SCAN8):GPIO_ResetBits(LED_SCAN8_GPIOx,LED_SCAN8);}
#define LED_SCAN9_Output(s)   {(s == 1)?GPIO_SetBits(LED_SCAN9_GPIOx,LED_SCAN9):GPIO_ResetBits(LED_SCAN9_GPIOx,LED_SCAN9);}
#define LED_SCAN07_Output(s)  {(s == 1)?GPIO_ResetBits(LED_SCAN07_GPIOx,LED_SCAN07):GPIO_SetBits(LED_SCAN07_GPIOx,LED_SCAN07);}

#define BURNIN_TEST_Input     GPIO_ReadInputDataBit(BURNIN_TEST_GPIOx,BURNIN_TEST)
//
#define POWERDOWN_Input       GPIO_ReadInputDataBit(BATTERY_CHECK_GPIOx,BATTERY_CHECK)
#define BATTERY_Output(s)     {(s == 1)?GPIO_SetBits(BATTERY_POWER_GPIOx,BATTERY_POWER):GPIO_ResetBits(BATTERY_POWER_GPIOx,BATTERY_POWER);}
#define BATTERY_PWM_Output(s) {(s == 1)?GPIO_SetBits(BATTERY_PWM_GPIOx,BATTERY_PWM):GPIO_ResetBits(BATTERY_PWM_GPIOx,BATTERY_PWM);}
//

#define SYSTEM_KICK_TIME                1       // 1 ms


//
//
#define       true                (bool)1
#define       false               (bool)0
//


#define TESTIO_SPEED          GPIO_Speed_10MHz
#define TESTIO_MODE           GPIO_Mode_Out_PP
#define TESTIO_GPIOx          GPIOA
#define TESTIO                GPIO_Pin_9
#define TESTIO_Output(s)     {(s == 1)?GPIO_SetBits(TESTIO_GPIOx,TESTIO):GPIO_ResetBits(TESTIO_GPIOx,TESTIO);}
// Hardware A3
#define MECHINE_TYPE_MODE       GPIO_Mode_IN_FLOATING;
#define MECHINE_TYPE_SPEED      GPIO_Speed_10MHz
#define MECHINE_TYPE_GPIOx      GPIOB
#define MECHINE_TYPE            GPIO_Pin_12
#define MECHINE_TYPE_Input      GPIO_ReadInputDataBit(MECHINE_TYPE_GPIOx,MECHINE_TYPE)

//
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 Johnson Fitness Inc. ***END OF FILE****/

