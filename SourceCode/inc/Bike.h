#include "General.h"

#define I_NONE    0
#define I_COMP    1
#define I_OFFSET  2
#define I_NO_MOV  3
#define P_NONE    0
#define P_COMP    1


typedef struct
{
	u8 Level;
	u16 Vo2;
	u16 MaxVo2;
	u16 MET;
	u16 Watts;
}FitTestResult;



void Bike_SetLanguage(u8 by_D);
void Bike_SetPowerSaveTime(u8 by_D);

u16 Bike_FiTtestScore(void);
u16 Bike_Vo2Percentile(void);

u16 Bike_Get_TimeDirNumber(void);
u16 Bike_Get_ProgramTime(void);
void Bike_Set_ProgramTime(u16 by_D);

void Bike_Set_MaxResist(u8 by_Dat);
u8 Bike_Get_MaxResist(void);
void Bike_Add_MaxResist(void);
void Bike_Dec_MaxResist(void);


void Bike_Set_MinResist(u8 by_Dat);
u8 Bike_Get_MinResist(void);
void Bike_Add_MinResist(void);
void Bike_Dec_MinResist(void);

void Bike_Set_Machine_Type(u8 by_Dat);
u8 Bike_Get_Machine_Type(void);

void Bike_Set_Incline_TuneEndPoint(u16 by_Dat);
u16 Bike_Get_Incline_TuneEndPoint(void);
void Bike_Set_Incline_NowTuneDir(u8 by_Dat);
u8 Bike_Get_Incline_NowTuneDir(void);


void Bike_Set_TvCH(u8 by_Dat);
u8 Bike_Get_TvCH(void);
void Bike_Set_TvVOL(u8 by_Dat);
u8 Bike_Get_TvVOL(void);


u8 Bike_Get_TimeDIR(void);
u16 Bike_Get_TimeModeBuffer(void);
void Bike_Set_TimeMode(u8 by_Dat);
u8 Bike_Get_TimeMode(void);
void Bike_Set_AddDecNewTime(u8 w_Dat);

void Bike_Set_Error1(u16 by_Dat);
u16 Bike_Get_Error1(void);
void Bike_Set_Error2(u16 by_Dat);
u16 Bike_Get_Error2(void);
void Bike_Set_Error3(u16 by_Dat);
u16 Bike_Get_Error3(void);

void Bike_Set_DigitalError(u8 by_D,u16 by_Dat);
u16 Bike_Get_DigitalError(u8 by_D);




u8 Bike_Get_Level(void);
void Bike_Set_Level(u8 by_Dat);
void Bike_Add_Level(void);
void Bike_Dec_Level(void);

void Bike_Set_AGE(u8 by_Dat);
u8 Bike_Get_AGE(void);
void Bike_Add_AGE(void);
void Bike_Dec_AGE(void);



void Bike_Add_Watts(void);
void Bike_Dec_Watts(void);
void Bike_Set_Watts(u16 by_Dat);
u16 Bike_Get_ConstWatts(void);



void Bike_METs_Cul(void);
u16 Bike_Get_METs(void);


void Bike_Set_Heart(u8 by_Dat);
u8 Bike_Get_Heart(void);
void Bike_Add_Heart(void);
void Bike_Dec_Heart(void);

void Bike_Add_Weight(void);
void Bike_Dec_Weight(void);
u16 Bike_Get_Weight(void);
void Bike_Set_Weight(u16 by_Dat);

void Bike_Add_Time(void);
void Bike_Dec_Time(void);
void Bike_Set_Time(u16 w_Dat);
u16 Bike_Get_Time(void);

void Bike_Set_User_Time(u16 by_Dat);
u16 Bike_Get_User_Time(void);
void Bike_Add_User_Time(void);
void Bike_Dec_User_Time(void);

u8 Bike_Get_Program_Mode(void);
void Bike_Set_Program_Mode(u8 by_Dat);

void Bike_Set_TotalTimeSegment(u8 Val);
u8 Bike_Get_TotalTimeSegment(void);


void Bike_Set_Time_Seg(u8 by_Dat);
void Bike_Set_PauseTime(u16 by_Dat);
u16 Bike_Get_PauseTime(void);
//void Bike_Add_PauseTime(void);
//void Bike_Dec_PauseTime(void);
void Bike_Set_Gender(u8 by_Dat);
u8 Bike_Get_Gender(void);
void Bike_Check_TargetSpeed(void);

u8 Bike_Get_THRPercent(void);
void Bike_Set_THRPercent(u8 by_Dat);
void Bike_Add_THRPercent(void);
void Bike_Dec_THRPercent(void);

void Bike_Initial(void);
void Bike_Set_Power_On(u16 by_Dat);
u16 Bike_Get_Power_On(void);
void Bike_SEC_Counter_Int(void);


u8 Bike_Get_Start(void);
void Bike_Set_Start(void);
void Bike_Start_Stop(void);

u16 Bike_Get_NowSpeed(void);
u16 Bike_Get_NowWatt(void);
void Bike_Speed_and_Watt_cul(void);

void Bike_Calories_Cul(void);
u8 Bike_Calories_Counter_Proce(void);
u32 Bike_Get_Calories(void);

void Bike_Distance_Cul(void);
u8 Bike_Distance_Counter_Proce(void);
u32 Bike_Get_Distance(void);

void Bike_Time_Cul(void);
void Bike_Time_CulToWorkout(void);
void Bike_Set_StartAccumulated(u8 by_Dat);
void Bike_Set_RPMMatch(u8 by_Dat);

u16 Bike_Get_Max_Time(void);
void Bike_Set_Max_Time(u16 by_Dat);

void Bike_Set_Time_Start(void);
void Bike_Set_Time_Stop(void);
u8 Bike_Get_Time_Over(void);
void Bike_Set_Time_Over(u8 Val);
void Bike_Clear_Time_Over(void);
u8 Bike_Time_Counter_Proce(u8 CoolDown);
u16 Bike_Get_Time_Accumulated(void);


void Bike_Set_Speed_Mode(u8 by_Dat);
u8 Bike_Get_Speed_Mode(void);

u16 Bike_Get_Speed(void);
//void Bike_Calc_FitTest_Result(FitTestResult *TestResult);


void Bike_SetInclineInformation(void);
u8 Bike_Get_BatteryVolt(void);
void Bike_Set_BatteryVolt(u8 Val);
u8 Bike_Get_BatteryCapacity(void);
void Bike_Set_BatteryCapacity(u8 Val);

u16 Bike_GetResisDacTable(u8 num);
u16 Bike_Get_NowRPM(void);
void Bike_Set_NowRPM(u16 Val);

void Bike_SetResistance(u8 CoolDown);
u8 GetNumResisTab(void);
void Bike_Set_PWM(u16 w_Dat);
void Bike_Constant_Watts_Proce(void);

void Bike_Add_Incline(void);
void Bike_Dec_Incline(void);
u8 Bike_Get_Incline(void);
void Bike_Set_Incline(u8 by_Dat);
void Bike_InclineDisable(void);
void InclineBatteryOverLow(void);
u8 InclinePositionOffset(void);

u8 Bike_Get_StartIncline(void);
void Bike_Set_StartIncline(u8 by_Dat);

void Bike_System_Reset_TimeCounter(void);
void Bike_Set_Incline_Reset(u8 by_Dat);
u8 Bike_Get_System_Reset(void);

u8 Bike_Get_Time_Seg(void);
void Bike_Clear_Time_Seg(void);
u8 Bike_Time_Seg_Proce(void);

void Bike_Set_Sound(u8 by_Dat);
u8 Bike_Get_Sound(void);

void Bike_Set_EngParameter(u16 by_Dat);
u16 Bike_Get_EngParameter(void);

void Bike_Set_ProfileTable(void);
