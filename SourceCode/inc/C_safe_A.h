#include "General.h"

#define Csafe_Ready    0x01//0x81
#define Csafe_Offline  0x09//0x89
#define Csafe_IDle     0x02//0x82
#define Csafe_Manual   0x08
#define Csafe_HaveID   0x03//0x83
#define Csafe_In_Use   0x05//0x85
#define Csafe_Paused   0x06//0x86
#define Csafe_Finished 0x07//0x87


#define cmdGetOdometer 0x9b
#define cmdGetPace     0xa6
#define cmdGetMETs     0xb3
#define cmdGetPower    0xb4

void CSAFE_TotalTime(u32 by_D);
void CSAFE_TotalDistance(u32 by_D);

u8 CSAFE_AvgHR(void);
void CSAFE_RX1(void);

void CSAFE_NetPulse(u8 by_D);
u8 CSAFE_GetNetPulse(void);

void CSAFE_SetWorkout(u8 by_D);
void CSAFE_Counter_Int(void);
void CSAFE_RX_Buffer_Decision(void);
void CSAFE_Number_Check(void);
u8 CSAFE_TX_XOR(u8 by_Dat);
void CSAFE_RX_Answer_Buffer(u8 by_Dat);
u8 CSAFE_Get_Mode(void);
void CSAFE_Set_Mode(u8 by_Mode);
void CSAFE_Initial(void);
void CSAFE_Power_On(void);
//void CSAFE_Set_Enable(u8 by_Dat);
//u8 CSAFE_Get_Enable(void);
void CSAFE_Set_ID_Number(u32 by_Dat);
void CSAFE_Set_AnyKey(void);
void CSAFE_Answer(u8 by_Dat);
void CSAFE_Error_Answer(u8 by_Dat);
u8 CSAFE_Mode_Check_Answer(u8 by_Dat);
void CSAFE_Work_Exit(void);
u8 CSAFE_Mode_Judge(void);
void CSAFE_Set_Start(void);
u8 CSAFE_Get_Program(void);//==>取得CSAFE所设定的 Program
u16 CSAFE_Get_Target_Distance(void);//==>取得C-SAFE目标值
u16 CSAFE_Get_Tartet_Calories(void);//==>取得C-SAFE目标值
//u8 CSAFE_Get_HR_Max(void);//==>取得C-SAFE是否有做设定的侦测
u8 CSAFE_Get_Work_END(void);//==>判断目标值是否达到用
void CSAFE_Reset(void);//==>将C-SAFE系统参数重置
void CSAFE_Heart_Scan(void);//==>针对心跳的各项数据取得用
u8 CSAFE_Get_Level(void);//==>回传是否有设定level参数用
u8 CSAFE_Get_Default_Level(void);//==>回传LEVEL设定值
u8 CSAFE_Get_Time(void);//==>回传是否有设定目标时间用
//void CSAFE_Set_Time(void);//==>当有设定时间时
u8 CSAFE_Get_Default_Time(void);//==>回传C-SAFE所设定预设时间值
u8 CSAFE_Get_Distance(void);//==>回传是否有设定目标距离用
u8 CSAFE_Get_Calories(void);//==>回传是否有设定目标卡路里用
//u8 CSAFE_Get_Heart(void);//==>回传是否有设定目标心跳值用
void CSAFE_Set_Stop(u8 by_Dat);//==>暂时停止运动与否(1->yes;0->No)
void CSAFE_EngTest(void);
void CSAFE_Clear_Target_DIS_CAL(void);
u8 CSAFE_Get_HR(void);

// 100.09.26
void CSAFE_900M_Cardio(char by_Command,char by_Num); 
void CSAFE_SetCheckAnyKeyStatus(char by_Dat);
//
// 100.10.04
void CSAFE_900M_E7command(void);
char CSAFE_900M_CommandStatus(void);
void CSAFE_900M_CommandStatusClear(void);
void CSAFE_900M_E7DataAnalysis(void);
//

