#ifndef _led_H_
#define _led_H_

#include "General.h"

#define Led_Elevation   0
#define Led_Speed       1

#define Show_Left       0 //==>左侧显示信息
#define Show_Right      1 //==>右侧显示信息
// Big Dot Matrix LED
#define BigMatrix_Width  16 // 8*2  unit:Dot
#define BigMatrix_Height 8  // 8*2  Dot:Dot
#define _BigMatrix_Clear 0
#define _BigMatrix_Action 1
//

#define Display_ON         0xff //==>显示全亮
#define Display_OFF        0x00 //==>显示全灭
// Program LED 
// 此Program排列与General.h中的排列有对等关系
#define Target_Manual        1
#define Target_Training      2 
#define Target_TargetHR      3
#define Target_FatBurn       4
#define Target_RollingHills  5
#define Target_FitnessTest   6
// Information LED
#define Target_Calories      7
#define Target_HRInfor       8
#define Target_Incline       9
#define Target_DistanceMiles 10
#define Target_Pace          11
#define Target_Speed         12
#define Target_AveragePace   13
#define Target_TimeElapsed   14
#define Target_TimeRemaining 15
#define Target_TotalTime     16
#define Target_Clock         17
#define Target_METs          18
#define Target_DistanceKM    19
#define Target_CaloriesHour  20
#define Target_Watts         21
#define Target_TV_ON         22
#define Target_TV_OFF        23
#define Target_WiFiSignal    24

#define Target_Program       60


//0
#define _Led_Manual         0x80
#define _Led_Training       0x10
#define _Led_HR             0x08
#define _Led_TimeElapased   0x04
#define _Led_TimeRemaining  0x02
#define _Led_CaloriesHour   0x01
//1
#define _Led_Calories       0x08
#define _Led_HRInfor        0x01
#define _Led_Incline        0x02
//2
#define _Led_DistanceMiles  0x40
#define _Led_Pace           0x20
#define _Led_Speed          0x10
#define _Led_Watts          0x08
#define _Led_DistanceKM     0x02
#define _Led_METs           0x01
//3
#define _Led_FatBurn        0x80
#define _Led_RollingHills   0x20
#define _Led_Fitness        0x40
#define _Led_Clock          0x10
#define _Led_TotaTime       0x04
#define _Led_AveragePace    0x02
//4
#define _Led_TV_ON          0x02
#define _Led_TV_OFF         0x01
/*
#ifdef WiFi_Module
    void Led_WiFiVersion(u8 by_Dat, u8 by_Dat1);
#endif
*/
void Led_MCBtype(u8 by_Dat);
void Led_Initial(void);
void Led_Blink_Flag_Int(void);
void Led_Set_Target(u8 by_Dat,u8 by_Blink);
void Led_ComScan(void);
//void Led_Set_Reset(void);
//void Led_String(uc8 *pby_Dat,u8 by_D1,u16 by_D2);
u8 Led_Get_DotmatrixEnd(void);
//u8 Led_Get_String_End(void);
//void Led_Set_Strig_End(void);
//void Led_NumberSite(u8 by_Seg,u8 by_Dat);
//void Led_NumberSite_4x8(u8 by_Seg,u8 by_Dat);
void Led_TargetLED_Loop(void);
//void Led_FanControlReset(void);
//void Led_FanControl(void);
void Led_DisplayTest(u8 by_Dat);
void Led_Special(u8 by_Mode,u8 by_S,u16 by_Dat);
void Led_RTCTime(unsigned char *by_RTC,char by_Mode,char by_Seg);


//u8 Led_StringParagraph(u8 by_Line,uc8 *pby_Dat);
u8 Led_WordCount(uc8 *pby_Dat);
//void Led_FanStart(u8 by_Dat);
//void Led_FanDelayTimeClear(void);
void Led_ProfileMap(u8 by_Dat);
void Led_Line(u8 by_Dat,u8 by_Dat1);
void Led_ProfileMapBuffer(u8 by_Dat,u8 by_Dat1,u8 by_Dat2);
void Led_SetProfileMapStatus(u8 by_D);
void Led_ClearProfileMap(void);
void Led_InclineChangeSetProfileMap(void);
void Led_Window(char by_Line,char by_Mode,long unsigned int by_Sum);
//void LED_MATRIX(unsigned char by_D);
void LED_END(void);
void Led_MessageSpeed(u8 by_D);
void Led_xIDNumber(u8 by_Mode,u8 by_D,char *by_D1);
void Led_7SEG(unsigned char by_Mode,unsigned short by_Sum);
#endif // _led_H_
