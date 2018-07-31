#ifndef _treadmill_H_
#define _treadmill_H_


#include "General.h"

#define I_NONE    0
#define I_COMP    1
#define I_OFFSET  2
#define I_NO_MOV  3
#define P_NONE    0
#define P_COMP    1

// Info 这些设定项与 General.h 的设定(各流程参数设定状态代码)是相关联的
#define Info_TIME         0
#define Info_WEIGHT       1
#define Info_HEIGHT       2
#define Info_StartIncline 3
#define Info_AGE          4
#define Info_GENDER       5
#define Info_LEVEL        6
#define Info_PERCENT      7 
#define Info_StartSpeed   8
#define Info_WorkoutSpeed 9
#define Info_WorkoutIncline 10
//
enum{
    Info_WarmUpAccTime = 1,
    Info_WorkoutAccTime,
    Info_CoolDwonAccTime,
    Info_AccumulatedTime,
    Info_FitTestACCTime
};
//
unsigned short Console_Information(unsigned char by_Item,unsigned char by_Handle,unsigned short by_Dat);
unsigned short Console_Parameter(char by_Item);

u16 Console_GetVolumeNumber(u8 by_D);
void Console_AddUserVolume(void);
void Console_DecUserVolume(void);
u8 Console_GetUserVolume(void);
void Console_SetDefaultVolume(u8 by_Dat);
u8 Console_GetDefaultVolume(void);
void Console_SetMaxVolume(u8 by_Dat);
u8 Console_GetMaxVolume(void);
void Console_SetOutputVolume(u8 by_Dat);
u8 Console_GetOutputVolume(void);

void Console_ChangeTimeCul(void);
void Console_BMI(void);
u16 Console_Get_BMI(void);
void Console_VO2(void);
u16 Console_Get_VO2(void);
void Console_Set_FitTestTimeAcc(u8 by_Dat);
//u16 Console_Get_FitTestAccTime(void);
u16 Console_Get_CsafeAccTime(void);
u16 Console_Get_AvgPace(void);
u32 Console_Get_AvgSpeed(void);
u16 Console_Get_METs(void);
void Console_ClearDistance(void);
u8 Console_Time_Counter_Proce(void);
// Time Seg (Dotmatrix Line)
u8 Console_Get_Time_Seg(void);
// Calories
u8 Console_Calories_Counter_Proce(void);
u16 Console_Get_Calories(void);
u16 Console_Get_Calories_Hour(void);
// Distance
u8 Console_Distance_Counter_Proce(void);
u16 Console_Get_Distance(void);
//
//u16 Console_Get_Time_Accumulated(void);
void Console_Time_Cul(void);
void Console_Set_StartAccumulated(u8 by_Dat);
//
u8 Console_DistanceGoal_Over(void);
u16 Console_Get_GoalDistance(void);
void Console_GetInclineInformation(void);
u8 Console_Time_Seg_Proce(void);
u8 Console_5K10K_Seg_Proce(void);
void Console_Initial(void);
void Console_SEC_Counter_Int(void);
u8 Console_Get_TimeDIR(void);
u8 Check_SafeKey(void);
u8 Console_GetInclinePfofile(u8 by_Dat,u8 by_Dat1,u8 by_Dat2);
// New Time
u16 Console_NewTime(u8 by_D);
// User Time
u16 Console_UserTime(u8 by_D,u16 by_D1);
// Max Time
u16 Console_MaxTime(u8 by_D,u16 by_D1);
// Time Start
void Console_TimeStart(u8 by_D);
// Time Over
u8 Console_TimeOver(u8 by_D,u8 by_D1);
// Distance Show target
u8 Console_ShowDistanceTarget(u8 by_D,u8 by_D1);
// Language
u8 Console_Language(u8 by_D,u8 by_D1);
// Display Speed Change
u8 Console_DisplaySpeedChange(u8 by_D,u8 by_D1);
// Digital Error
u16 Console_DigitalError(u8 by_D,u8 by_D1,u16 by_D2);
// Navy Test
void Console_NavyTestMode(u8 by_D);
// Speed Unit
u8 Console_SpeedUnit(u8 by_D,u8 by_D1);
// Max Speed
u8 Console_MaxSpeed(u8 by_D,u8 by_D1);
// Start Speed 
u8 Console_StartSpeed(u8 by_D,u16 by_D1);
// Sleep Time
u16 Console_SleepTime(u8 by_D,u16 by_D1);
// ADC
u16 Console_HighLowADC(u8 by_D,u8 by_D1,u16 by_D2);
// RPM Parameter
u16 Console_RPM_Parameter(u8 by_D1,u8 by_D2,u16 by_D3);
// Gender
//u8 Console_Gender(u8 by_D,u8 by_D1);
// Workout Status
u8 Console_WorkoutStatus(u8 by_D,u8 by_D1);
// Audio
u8 Console_Audio(u8 by_D,u8 by_D1);
// Power On 
u16 Console_PowerOn(u8 by_D,u16 by_D1);
// Program Mode
u8 Console_ProgramMode(u8 by_D,u8 by_D1);
// Fitness Test
u8 Console_GerkinConversion(void);
u8 Console_Gerkin(void);
u16 Console_AirForce(void);
u8 Console_Navy(void);
u8 Console_PEB(void);
u8 Console_DistanceEND(void);
u8 Console_Army(void);
u8 Console_ArmyScore(u16 by_TimeStart,u16 by_TimeEnd);
u8 Console_Marine(void);
//
u8 Console_CsafGoalEND(void);
void Console_CsafGoalDisable(void);
//u16 Console_ChangeDistanceBuffer(char by_D);

void Console_SetHRControlModel(u8 by_D);
void Console_HRControlMax(u8 by_D);
void Console_HRControlChange(u8 by_D1,u16 by_D2);
u8 Console_GetHRControlMax(void);
u8 Console_GetHRControlMode(void);
u8 Console_MaxIncline(u8 by_D,u8 by_D1);
long unsigned int Console_HJS(unsigned char by_Mo,long unsigned int by_D);
unsigned char Console_AutoSoftwareUpdateStatus(unsigned char by_Mode);

unsigned short Console_GetTimeSegData(void);

#endif //_treadmill_H_
