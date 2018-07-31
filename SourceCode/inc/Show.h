#ifndef _Show_H_
#define _Show_H_

#include "General.h"

#define Window_1 2
#define Window_2 43
#define Window_3 56
#define Window_4 82

#define _Message_Keep   0 // 定字符串
#define _Message_Loop   1 // 卷动字符串
#define _Message_NotYet 2 // 无动作
//
//
#define _Message_AddNumber 1
#define _Message_AddNumber_DF 2
#define _Message_AddNumber_Time_DF 3
#define _Message_AddNumber_Time 4
unsigned char Show_Message(u8 by_Dat,u8 by_D1,u16 by_D2,u8 Motion);
//
#define _MsgAddNum_Fixed_Time 1
#define _MsgAddNum_Fixed_Time_WithOff 2
#define _MsgAddNum_Time  3
#define _MsgAddNum_Fixed_2Int 4  
#define _MsgAddNum_Fixed_3Int 5   
#define _MsgAddNum_Fixed_4Int 6   
#define _MsgAddNum_Fixed_5Int 7   
#define _MsgAddNum_Fixed_6Int 8   
#define _MsgAddNum_Fixed_6Int1Dec 9 
unsigned char Show_MessageWithNumber(unsigned char* string, unsigned char stringNumber, unsigned long by_Dat, unsigned char by_Mode, unsigned char Motion);
//
void Show_Time(u8 by_D);
void Show_HeartRate(void);
void Show_Speed(void);
void Show_Incline(void);
void Show_Distance(u8 by_Data);
void Show_Calories(void);
void Show_CaloriesHour(void);
void Show_GerkinInformation(u8 by_Dat);
void Show_FITInformation(u8 by_Dat);
void Show_NavyInformation(u8 by_Dat);
void Show_WFIinInformation(u8 by_Dat);
//void Show_MessageParagraph(u8 by_MessageNumber,u8 by_Paragraph);
//unsigned char Show_MessageParagraph(uc8 *pby_ShowNumber,u8 by_MessageNumber,u8 by_D1,u32 by_D2);
void Show_SetSummaryNumber(u8 by_Dat);
//void Show_Summary(u8 by_Dat);
void Show_AveragePace(void);
void Show_Pace(u8 by_D);
void Show_METs(void);
void Show_Watts(void);
void Show_Clock(unsigned int by_Dat);

void Show_WorkoutSummary(void);

//void USB_SaveWorkoutSummary(void);

// 100.09.28
void Show_RF900Infor(u8 by_Com,u8 by_D);
void Show_RF900MessageOld(void);
//
void Show_StringOneTime(unsigned char *String, unsigned char ShowTime);
#endif // _Show_H_
