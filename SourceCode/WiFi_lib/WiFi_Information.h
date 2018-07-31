// 使用 __WiFi_Information 宣告方式可以避免內容重複定義
#ifndef __WiFi_Information
#define __WiFi_Information

#include "wifi_define.h"

#define WIFI_SSID "T1x"//"E7"// Console name

#define _WiFi_Weight  1
#define _WiFi_Age     2
#define _WiFi_Unit    3
#define _WiFi_Gender  4
#define _WiFi_Speed   5
#define _WiFi_Incline 6
#define _WiFi_Calories 7
#define _WiFi_CaloriesPerHour 8
#define _WiFi_Distance 9
#define _WiFi_HeartRate 10
#define _WiFi_THR 11
#define _WiFi_ProgramLevel 12
#define _WiFi_TargetWatts 13
#define _WiFi_Watts 14
#define _WiFi_ResistanceLevel 15
#define _WiFi_METs 16
#define _WiFi_WorkoutState 17
#define _WiFi_TimeInState 18
#define _WiFi_AccTime 19
#define _WiFi_WorkoutTime 20
#define _WiFi_WarmUpTime 21
#define _WiFi_CoolDownTime 22
#define _WiFi_WorkoutFinish 23
#define _WiFi_WorkoutStage 24
#define _WiFi_WorkoutStageTime 25
#define _WiFi_TimeInStage 26
#define _WiFi_AveragePace 27
#define _WiFi_RPM 28
#define _WiFi_Goal 29
#define _WiFi_NextIncline 30
#define _WiFi_NextSpeed 31
#define _WiFi_NextResistance 32
#define _WiFi_UserAge 33
#define _WiFi_CurrentUser 34
#define _WiFi_ProgramID 35
#define _WiFi_ResistanceType 36
#define _WiFi_ProgramStop 37
#define _WiFi_Floors 38
#define _WiFi_TotalSteps 39
#define _WiFi_Height 40 
#define _WiFi_AvgHeartRate 41
#define _WiFi_LapIndex 42
#define _WiFi_StepperFitTestScore 43
// 2014.11.24
#define _WiFi_WorkoutState_WarmUp     0
#define _WiFi_WorkoutState_Workout    1
#define _WiFi_WorkoutState_CoolDown   2
#define _WiFi_WorkoutState_Pause      3
//
#define _DefaultTimeZone 21 //(GMT-06:00)Central Time(US and Canada)
#define _TimeZoneMax 78


// DAPI Server Index
#define _Server_Dev        0
#define _Server_Staging    1 
#define _Server_QA         2
#define _Server_Production 3


void WiFi_Set_MachineInformation(void);
void WiFi_Information_Initial(void);
void WiFi_Set_MachineInformation_Initial(void);
void WiFi_Information_SaveUserConfig(void);
void WiFi_Information_SerialNumber(unsigned char mode,unsigned char *ModuleStr,unsigned char Ver,unsigned long SerialNumber);
void WiFi_Information_Set(unsigned char by_Item,unsigned short by_Dat);
unsigned short WiFi_Information_Get(unsigned char by_Item);
void WiFi_Information_ConnectCheck(void);

unsigned char WiFi_Information_TimeZoneCheck(unsigned char by_D);
void WiFi_Information_TimeZoneUpdate(unsigned char by_D);
void WiFi_Information_ChangeDAPI_Server(char by_D);
void WiFi_Information_MachineTypeDefaultCheck(void);
#endif
