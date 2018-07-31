#ifndef _EEPRom_H_
#define _EEPRom_H_

#include "General.h"

enum
{
  EE_MAX_TIME = 1,
  EE_USER_TIME,
  EE_WEIGHT,
  EE_LEVEL,
  EE_AGE,
  EE_UNIT,
  EE_LOW_ELEVATION,
  EE_MAX_ELEVATION,
  EE_ACCUMULATED_TIME,
  EE_ACCUMULATED_TIMEA,
  EE_ACCUMULATED_DISTANCE,
  EE_ACCUMULATED_DISTANCEA,
  EE_LANGUAGE,
  EE_OutOfOrder,
  EE_StartSpeed,
  EE_MaxSpeed,
  EE_PauseTime,
  EE_CHANNEL,
  EE_VOLUME,
  EE_TimerMode,
  EE_Logo,
  EE_GENDER,
  EE_MaxRPM_Parameter,
  EE_BEEP,
  EE_MachineType,
  EE_MachineType_LCB,
  EE_CHECK,
  EE_ACCUMULATED_TIME_L,
  EE_SerialNumber,
  EE_NAVY_Test,
  EE_RF_SERVICE,
  EE_CLUB_ID,
  EE_InUser,
  EE_ErrorCode,
  EE_DigitalError_1,
  EE_DigitalError_2,
  EE_DigitalError_3,
  EE_DigitalError_4,
  EE_DigitalError_5,
  EE_DigitalError_6,
  EE_DigitalError_7,
  EE_DigitalError_8,
  EE_DigitalError_9,
  EE_DigitalError_10,
  EE_SleepTime,
  EE_ConsoleVersion,
  EE_FrameVersion,
  EE_FrameSerialNumber,
  EE_Audio,
  EE_MinRPM_Parameter,
  EE_AM,
  EE_MaxVolume,
  EE_OutputVolume,
  EE_CommunicationTX,
  EE_CommunicationRX,
  EE_HRTSpeedBased,
  EE_TVPower,
  EE_Height,
  EE_MaxDF_Elevation,
  EE_Speaker,
  EE_Wireless,
  EE_HJS_Switch,
  EE_HJS_CheckNumber,
  EE_HJS_total,
  EE_DemoMode,
  EE_xID,
  EE_TZone,
  EE_DAPI_Server,
  EE_USB_Protection,
  EE_KeyStuck,
  EE_KeyNotifiction,
  EE_SSL,
  EE_AP_Mode,
  EE_WiFi_SSID,
  EE_WiFi_Password,
  EE_WiFi_Security,
  EE_AutoUpdateSwitch,
  EE_AutoUpdateTime,
  EE_AutoUpdateDate,
  EE_RSCU_SoftwareVersion,
  
  EE_RunningBeltCurrentAccMiles,
  EE_RunningBeltCurrentAccHours,
  EE_RunningBeltPreviousAccMiles,
  EE_RunningBeltPreviousAccHours,
  EE_RunningBeltPreviousYear,
  EE_RunningBeltPreviousMouth,
  EE_RunningBeltPreviousDay,
  
  EE_RunningDeckCurrentAccMiles ,
  EE_RunningDeckCurrentAccHours ,
  EE_RunningDeckPreviousAccMiles ,
  EE_RunningDeckPreviousAccHours ,
  EE_RunningDeckPreviousYear,
  EE_RunningDeckPreviousMouth,
  EE_RunningDeckPreviousDay,
  
  EE_MCBCurrentAccMiles,
  EE_MCBCurrentAccHours,
  EE_MCBPreviousAccMiles,
  EE_MCBPreviousAccHours,
  EE_MCBPreviousYear,
  EE_MCBPreviousMouth,
  EE_MCBPreviousDay,
  
  EE_MoterCurrentAccMiles,
  EE_MoterCurrentAccHours,
  EE_MoterPreviousAccMiles,
  EE_MoterPreviousAccHours,
  EE_MoterPreviousYear,
  EE_MoterPreviousMouth,
  EE_MoterPreviousDay,
  
  EE_HeadphoneCurrentAccHours,
  EE_HeadphoneCurrentInsertsTimes,
  EE_HeadphonePreviousAccHours,
  EE_HeadphonePreviousInsertsTimes,
  EE_HeadphonePreviousYear,
  EE_HeadphonePreviousMouth,
  EE_HeadphonePreviousDay,

  EE_Prev_MaintenanceCurrentAccMiles,
  EE_Prev_MaintenanceCurrentAccHours,
  EE_Prev_MaintenancePreviousAccMiles,
  EE_Prev_MaintenancePreviousAccHours,
  EE_Prev_MaintenancePreviousYear,
  EE_Prev_MaintenancePreviousMouth,
  EE_Prev_MaintenancePreviousDay,
  
  EE_InclineCalibrationFlag,
//  EE_Idle,

  // END
  EE_END,
  EE_ParameterMax = EE_END-1 // 目前最大储存数 (随储存数增减)不可随意调整,一修改后
                             // 软件更新完后会将其参数都设回默认值.
};


/*
#define EE_MAX_TIME               1
#define EE_USER_TIME              2
#define EE_WEIGHT                 3
#define EE_LEVEL                  4
#define EE_AGE                    5
#define EE_UNIT                   6
#define EE_LOW_ELEVATION          7
#define EE_MAX_ELEVATION          8
#define EE_ACCUMULATED_TIME       9
#define EE_ACCUMULATED_TIMEA      10
#define EE_ACCUMULATED_DISTANCE   11
#define EE_ACCUMULATED_DISTANCEA  12
#define EE_LANGUAGE               13
#define EE_OutOfOrder             14
#define EE_StartSpeed             15
#define EE_MaxSpeed               16
#define EE_PauseTime              17
#define EE_CHANNEL                18 
#define EE_VOLUME                 19
#define EE_Logo                   20
#define EE_GENDER                 21
#define EE_MaxRPM_Parameter       23
#define EE_BEEP                   24
#define EE_MachineType            25
#define EE_MachineType_L          26
#define EE_CHECK                  27
#define EE_ACCUMULATED_TIME_L     28
#define EE_SerialNumber           29
#define EE_NAVY_Test              30
#define EE_RF_SERVICE             31
#define EE_CLUB_ID                32
#define EE_InUser                 33
#define EE_ErrorCode              34
#define EE_DigitalError_1         35
#define EE_DigitalError_2         36
#define EE_DigitalError_3         37
#define EE_DigitalError_4         38
#define EE_DigitalError_5         39
#define EE_DigitalError_6         40
#define EE_DigitalError_7         41
#define EE_DigitalError_8         42
#define EE_DigitalError_9         43
#define EE_DigitalError_10        44
#define EE_SleepTime              45
#define EE_ConsoleVersion         46 
#define EE_FrameVersion           47 
#define EE_FrameSerialNumber      48 
#define EE_Audio                  49 
#define EE_MinRPM_Parameter       50
#define EE_AM                     51
#define EE_MaxVolume              52 
#define EE_OutputVolume           53 
#define EE_CommunicationTX        54
#define EE_CommunicationRX        55
#define EE_HRTSpeedBased          56
#define EE_TVPower                57 
#define EE_Height                 58
#define EE_MaxDF_Elevation        59
#define EE_Speaker                60
#define EE_Wireless               61
#define EE_HJS_Switch             62
#define EE_HJS_CheckNumber        63
#define EE_HJS_total              64 
#define EE_DemoMode               65
#define EE_xID                    66
#define EE_TZone                  67
#define EE_ServerIP               68

#define EE_ParameterMax           68 //==>目前最大储存数 (随储存数增减)不可随意调整,一修改后
                                     //   软件更新完后会将其参数都设回默认值.
*/
#define EE_BlockMask              0
#define EE_CheckSum               2
#define EE_ParameterNo            4
#define EE_BlockIDSize            6
#define EE_DataStart              6

#define MachineType_TM  0x39 // 自定义编号


u32 EEPROM_ReadInformation(u16 by_Address);
void EEPROM_WriteInformation(u16 by_Address,u32 by_Data);

unsigned char  EEPROM_Save_Initial(void);
void EEPROM_Read_Initial(void);
void EEPROM_SaveError(void);
void EEPROM_SaveWorkInformation(u8 by_Address,u32 by_Dat);
unsigned char EEPROM_GetMemoryErrorStatus(void) ;
unsigned char EEPROM_Initial(void) ;
//void EEPROM_DFNumber_Initial(void);
unsigned char EEPROM_WriteParameter(void) ;
// Add by Kunlung 20110127  
unsigned char EEPROM_ExportEEMemoryData(unsigned short Addr ,unsigned char *ExportData) ; // Return = 0 is END
unsigned char EEPROM_InportEEMemoryData( unsigned short Addr, unsigned char InportData ) ; // Return = 0 is END
void EEPROM_ParameterDefault(char by_D) ;
void EEPROM_AccumulatedMode(char by_Dat);


#endif //_EEPRom_H_
