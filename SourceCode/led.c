#include "led.h"
//#include "LetterCodeTable.h"
#include "Console.h"
#include "Show.h"
#include "EEPRom_Language.h"
#include "wifi_Digital.h"
#include "Show_Functions.h"


//
struct {
       unsigned Blink          : 1;
       unsigned Blink1         : 1;//==>跑马灯流畅度用
       unsigned Blink2         : 1;
       unsigned Blink3         : 1;
       unsigned Reset          : 1;//==>控制跑马灯是否重头跑
       unsigned Loop           : 1;//==>判断跑马灯是否循环
       unsigned TargetLoop     : 1;
       //unsigned FanStart       : 1;//==>风扇控制判断是否在运动模式下
       unsigned ProfileStatus  : 1;
       }Led;
u8 aw_7SEG[4];
u8 Led_Buffer[10];// Matrix LED
u8 Led_Target[5]={0}; // Overlay LED
//                Bit  7 6 5 4
//                Bit  3 2 1 0
//Led_Target[0] : Manual       / Training Workouts  / N              / N  
//                Calories     / Time Elapased      / Distance_M     / HR_LED
//Led_Target[1] : HR Program   / N                  / Fitness Test   / Fat Burn
//                Watts        / N                  / PACE           / Incline
//Led_Target[2] : N            / N                  / N              / Rolling Hills
//                Calories_H   / Time_R             / Distance_KM    / METs
//Led_Target[3] : N            / N                  / N              / CLOCK
//                SPEED        / Total_P_Time       / Average_Pace   / METs(Below)
//Led_Target[4] : N            / N                  / N              / N
//                N            / N                  / TV Power Green / TV Power Red
#define _8x8DotMatrix 8*8
#define _MaxWord 17
u8 aw_DotMatrix_Buffer[_StringWindow_Width];
u8 aw_ProfileMap[16];
u8 by_TargetLED;
//u8 by_Fan;
u8 by_Show_End=0;
u8 _MessageSpeed;
//u16 by_FanDelayTime=0;
u8 aw_16x16DotMatrix[16];

//char Led_CheckWordNumber(char by_D);
//========================================================================================================================
//====>MCB TYPE显示
//=============================================================================================
void Led_MCBtype(u8 by_Dat)
{
  u8 w_ShowBuffer=12;//88;
  Led_NumberSite(w_ShowBuffer,by_Dat/16);
  Led_NumberSite(w_ShowBuffer+6,by_Dat%16);
}
//========================================================================================================================
//====>重置
//=============================================================================================
void Led_Initial(void)
{
  Led.Blink=0;
  Led.Reset=1;
  Led.Blink1=0;//控制米字流畅度用
  Led.Blink3=0;
  Led.TargetLoop=0;
  Led.ProfileStatus=0;
  by_TargetLED=1;
  by_Show_End=0;
  _MessageSpeed = 39;
}

//=======================================================================================================================================
//====> Profile map table 资料暂存
// by_Dat : Program
// by_Dat1: Level
// by_Dat2: 取图资起始点 0-依据排数 1-从头
//========================================================
void Led_ProfileMapBuffer(u8 by_Dat,u8 by_Dat1,u8 by_Dat2)
{
  u8 i,by_Bu = 0;
  
  if(by_Dat2 == 0) by_Bu = Console_Get_Time_Seg() - 1;
  for(i = by_Bu;i < 16;i++)
  {
      aw_ProfileMap[i] = Console_GetInclinePfofile(by_Dat,i,by_Dat1-1);
  }
}
//=======================================================================================================================================
//====> Profile map 显示于incline切换时存入
//========================================================
void Led_InclineChangeSetProfileMap(void)
{
  u8 i;
  
  if((Console_ProgramMode(Data_Get,0) <= TM_Manual)|| \
     (Console_ProgramMode(Data_Get,0) == TM_TargetHR)|| \
     (Console_ProgramMode(Data_Get,0) >= TM_Gerkin))
  {  // 2015.04.13
     if((Console_ProgramMode(Data_Get,0) == TM_TargetHR)&&\
        (Console_GetHRControlMode() == 1) )
     {// THR program and Speed change 
         if(aw_ProfileMap[Console_Get_Time_Seg() - 1] != (Console_Information(Info_WorkoutSpeed,Data_Get,0)/10)) 
         {// 当有改变Incline时
             for(i = Console_Get_Time_Seg() - 1;i < 16;i++)
             {
                 aw_ProfileMap[i] = Console_Information(Info_WorkoutSpeed,Data_Get,0)/10;
                 Led_Line(i,aw_ProfileMap[i]);
             }
         }
     }
     else if(aw_ProfileMap[Console_Get_Time_Seg() - 1] != Console_Information(Info_WorkoutIncline,Data_Get,0)) 
     {// 当有改变Incline时
         for(i = Console_Get_Time_Seg() - 1;i < 16;i++)
         {
             aw_ProfileMap[i] = Console_Information(Info_WorkoutIncline,Data_Get,0);
             Led_Line(i,aw_ProfileMap[i]);
         }
     }
  }
  
}
//=======================================================================================================================================
//====> Profile map 显示
// 0=秀图形 
// 1=单排闪烁 
// 2=图形数据放入Buffer+秀图形
// 3=往左位移一排
//========================================================
void Led_ProfileMap(u8 by_Dat)
{
  u8 i;
  u16 by_Dot;
    
  // Program Profile
  if(by_Dat == 0 || by_Dat == 2)
  {
      for(i = 0;i < 16;i++)
      {
          by_Dot = aw_ProfileMap[i];
          if(by_Dat == 0)//==>show 对应图形
              Led_Line(i,by_Dot);
          else//==>将对应图形数据放入Buffer
              Led_Line(i,by_Dot);
      }    
  }
  else if(by_Dat == 3)
  {// 往左位移一排
      for(i = 0;i < 16;i++)
      { 
          if(i == 15)
          {
              aw_ProfileMap[i] = 1;
              by_Dot = 1;
          }
          else  
          {
              aw_ProfileMap[i] = aw_ProfileMap[i+1];
              by_Dot = aw_ProfileMap[i];
          }
          Led_Line(i,by_Dot);
      }
  }
  else
  {
      i = Console_Get_Time_Seg()-1;        
      by_Dot = aw_ProfileMap[i];
      if(Led.Blink2)
          Led_Line(i,by_Dot);
      else 
          Led_Line(i,0xfe);
  }
}

//=======================================================================================================================================
//====> 单排显示灯数给 Profile map 显示用
//========================================================
void Led_Line(u8 by_Dat,u8 by_Dat1)
{
    u16 by_Data = 0xff;
    
    if(by_Dat1 == 0xfe)// 灭整排
        by_Data = 0; 
    else // 单排点 
    {
        // 2015.04.13
        if((Console_ProgramMode(Data_Get,0) == TM_TargetHR)&&\
           (Console_GetHRControlMode() == 1) )
        {
            if(Console_SpeedUnit(Data_Get,0) == C_KM)
            {
                by_Dat1=(u8)((float)by_Dat1*0.62137);
            }
            by_Dat1 /= 20;
            if(by_Dat1 > 7) by_Dat1=7;
        }
        else
        {
            if(by_Dat1 < 5)
                by_Dat1=0;
            else if(by_Dat1 < 9)
                by_Dat1=1;
            else if(by_Dat1 < 13)
                by_Dat1=2;
            else if(by_Dat1 < 17)
                by_Dat1=3;
            else if(by_Dat1 < 21)
                by_Dat1=4;
            else if(by_Dat1 < 25)
                by_Dat1=5;
            else if(by_Dat1 < 29)
                by_Dat1=6;
            else
                by_Dat1=7;
        }
        by_Data = by_Data << 7 - by_Dat1;
    }
    aw_16x16DotMatrix[by_Dat] = by_Data;
}
//=======================================================================================================================================
//====> 清除图形
//========================================================
void Led_ClearProfileMap(void)
{
  u8 i;
  
  for(i = 0;i < 16;i++)
  {
      aw_16x16DotMatrix[i] = 0;
  }
}
//========================================================================================================================
//====>LED闪烁旗标
//=============================================================================================
void Led_Blink_Flag_Int(void)
{
  static u8 by_Loop1=0;
  
  if(by_Loop1 >= 1)
  {
     by_Loop1=0;
     if(Led.Blink2) Led.Blink2=0;
     else Led.Blink2=1;
  }
  else by_Loop1++;
  //if(!Led.FanStart)
  //{
  //    if(by_Fan != Fan_Close)
  //    {
  //        if(by_FanDelayTime >= 300)
  //        {
  //            Led_FanControlReset();              
  //        }
  //       else by_FanDelayTime++;
  //    }
  //    else by_FanDelayTime=0;
  //}
  //else by_FanDelayTime=0;
  if(Led.Blink3 == 0)
  {// 大距阵跑马灯
      Led.Blink3 = 1;
  }
}
//=================================
// 特殊显示用显示
// by_D = 0:MCB Version 
//        1:MCB type 
//        2:Error Code (平时)
//        3:Total Time xxxxxxx 
//        4:Total Dis. xxxxxx.x 
//        5:Serial number xxxxxxxxx 
//        6:Error Code (工程模式)
//        7:RF Radio
//        8:xxxxxx
//        9:LCBA xxx B xxx 
// by_D1 = Number
//==================================
void Led_Special(u8 by_Mode,u8 by_S,u16 by_Dat)
{
  if(by_Mode == 0)
  {
    Led_NumberSite(by_S,(by_Dat/256)/16);
    Led_NumberSite(by_S+6,(by_Dat/256)%16);
    Led_NumberSite(by_S+12,120);// .
    by_Dat %= 256;
    Led_NumberSite(by_S+18,by_Dat/16);
    Led_NumberSite(by_S+24,by_Dat%16);
  }
  else if(by_Mode == 2)
  {
    Led_NumberSite(by_S,(by_Dat/256));
    Led_NumberSite(by_S+6,126);// -
    by_Dat %= 256;
    Led_NumberSite(by_S+12,by_Dat/16);
    Led_NumberSite(by_S+18,by_Dat%16);    
  }
}

//========================================================================================================================
//====>指针 LED 灯显示循环
//=============================================================================================
void Led_TargetLED_Loop(void)
{
  if(Led.Blink2)
  {
      if(Led.TargetLoop)
      {
          Led.TargetLoop=0;
          Led_Set_Target(by_TargetLED,0);
          by_TargetLED++;
          if(by_TargetLED > 6) by_TargetLED=1;
          Led_Set_Target(by_TargetLED,1);
      }
  }
  else
  {
      Led.TargetLoop=1;
  }
}
//========================================================================================================================
//====>指针 LED 灯显示
//====> 1.灯号  2. 0->灭 / 1->点亮 / 2->闪烁
//=============================================================================================
const unsigned char _TargetLedList[23]=
          {_Led_Manual       ,_Led_Training    ,_Led_HR,
           _Led_FatBurn      ,_Led_RollingHills,_Led_Fitness,
           _Led_Calories     ,_Led_HRInfor     ,_Led_Incline,
           _Led_DistanceMiles,_Led_Pace        ,_Led_Speed,
           _Led_AveragePace  ,_Led_TimeElapased,_Led_TimeRemaining,
           _Led_TotaTime     ,_Led_Clock       ,_Led_METs,
           _Led_DistanceKM   ,_Led_CaloriesHour,_Led_Watts,
           _Led_TV_ON        ,_Led_TV_OFF
          };
void Led_Set_Target(u8 by_Number,u8 by_Mode)
{
  
  u8 _Blink = 0;
  u8 by_Buffer = 0;
  u8 _Bu = 0;
  
  if(by_Mode == 1 || (by_Mode == 2 && Led.Blink2))
  {
      _Blink = 1;
  }
  by_Buffer = _TargetLedList[by_Number - 1];
  
  switch(by_Number)
  {
      case Target_WiFiSignal:
                  if(by_Mode >= 1 && by_Mode <= 4)
                  {
                      Led_Target[2] |= _Led_Watts;
                      if(by_Mode >= 2)
                          Led_Target[2] |= _Led_Speed;
                      if(by_Mode >= 3)  
                          Led_Target[2] |= _Led_Pace;
                      if(by_Mode == 4)  
                          Led_Target[2] |= _Led_DistanceMiles;
                  }
                  else
                  {
                      Led_Target[2] &= (~_Led_Pace);
                      Led_Target[2] &= (~_Led_Watts);
                      Led_Target[2] &= (~_Led_DistanceMiles);
                      Led_Target[2] &= (~_Led_Speed);
                  }
                  return ;
      case Target_TV_ON:
      case Target_TV_OFF:
                  _Bu = 4;
                  break;
      case Target_FatBurn:            
      case Target_RollingHills:            
      case Target_FitnessTest:            
      case Target_Clock:     
      case Target_TotalTime: 
      case Target_AveragePace: 
                  _Bu = 3;
                  break;
      case Target_DistanceMiles:
      case Target_Pace:            
      case Target_Speed:
      case Target_CaloriesHour:  
      case Target_DistanceKM: 
      case Target_METs:   
                  _Bu = 2;
                  break;
      case Target_Calories: 
      case Target_HRInfor:
      case Target_Incline:
                  _Bu = 1;
                  break; 
  }
 
  if(by_Number == Target_Program)
  {
      Led_Target[0] &= ~0x98;
      Led_Target[3] &= ~0xE0;
  }
  else if(by_Number == Display_ON || by_Number == Display_OFF)
  {
     for(_Bu = 0;_Bu < _8x8DotMatrix;_Bu++)
     {
         if(_Bu < 5) Led_Target[_Bu] = by_Number;
         //7 seg number led all on
         if(_Bu < 4) aw_7SEG[_Bu] = by_Number;
         if(_Bu < 16)
         {
           aw_16x16DotMatrix[_Bu] = by_Number;
         }
         //aw_DotMatrix_Buffer[_Bu] = by_Number;
         DotMatrix_Window[_Bu] = by_Number;
     }
  }
  else
  {
      if(_Blink == 1)
          Led_Target[_Bu] |= by_Buffer;
      else
          Led_Target[_Bu] &= ~by_Buffer;
  }
  // 2015.01.22
  Digital_Command_Proce_wifi();
  //
}
//========================================================================================================================
//====>LED TEST
//=============================================================================================
void Led_DisplayTest(u8 by_Dat)
{
   u8 w_Data = 0x01;
   u8 i;
   if(by_Dat <= 8)
   {
       if(by_Dat > 1) w_Data = 0x01 << by_Dat-1;
       for(i = 0;i < _8x8DotMatrix;i++)
       {
           //aw_DotMatrix_Buffer[i] = w_Data;
           DotMatrix_Window[i]=w_Data;
       }
       Led_7SEG(1,by_Dat*1111);
   }
   if(by_Dat < 15)
   {
     if(by_Dat == 13)       
       Led_Set_Target(Target_TV_OFF,1);   
     else if(by_Dat == 14)
       Led_Set_Target(Target_TV_ON,1); 
     else
       Led_Set_Target(by_Dat,1); 
   }
   if(by_Dat == 15)
   {
       Led_Set_Target(Display_ON,1);
   }
  // 2015.01.22
  Digital_Command_Proce_wifi();
  //
}
//===================================================================================================================================
//====> 控制跑马灯是否重头跑可避免有错误的字符串产生
//=====================================================
//void Led_Set_Reset(void)
//{
//  Led.Reset=1;
//  by_Show_End=0;
//}
//===================================================================================================================================
//====> 判断跑马灯字符串是否跑完
//=====================================================
//u8 Led_Get_String_End(void)
//{
//  return by_Show_End;
//}
//void Led_Set_Strig_End(void)
//{
//  by_Show_End++;
//}
//=====================================================================================================================================
//====> show 字符串或跑马灯
//====> 1.显示内容
//====> 2.是否在跑马灯后加上数值显示 0:NO 1:YES
//====> 3.跑马灯后的显示值 <最大值999>
//==================================================================
/*
void Led_String(uc8 *pby_Dat,u8 by_D1,u16 by_D2)
{
  u8 by_Loop=0;
  u8 by_Word=0;
  u8 by_Target;//==>字库对照位置
  u8 by_Dot=0;//==>间隔排数用
  u8 by_TargetSeg;
  u8 W_Buffer[120];//==>显示位移数据暂存
  u8 W_Buffer1[170]={0};//==>接收数据暂存
  u8 i=0;
  u8 by_StringStart=0;
  u8 by_StartLine = 0;
  static u8 by_Seg=0;
  static u8 by_TargetStart=0;//==>显示字符串起始地址
  static u8 by_OldString=0;
  
  
  while(*(pby_Dat+by_Word)!=0)
  {//==>判断是否为跑马灯用
     if(by_Word > _MaxWord+2) break;
     W_Buffer1[by_Word]=*(pby_Dat+by_Word);
     by_Word++;
  }
  if(Led.Reset)
  {//==>当下RESET指令时就让字符串重跑
     Led.Reset=0;
     by_Seg=0;
     by_TargetStart=1;
  }
  if(by_Word > _MaxWord)
  {//==>使用跑马灯
    
     if(Led.Blink1)
     {
         by_StringStart=1;
         Led.Blink1=0;
         Led.Loop=1;
         by_TargetSeg=_MaxWord+2;// ㄧ次取_MaxWord个字做备份
         by_Word=0;
         while(*(pby_Dat+by_Word)!=0)
         {
             W_Buffer1[by_Word+_MaxWord+1]=*(pby_Dat+by_Word);
             by_Word++;
             if(by_Word > 250) break;
         }
         if(by_D1 == 1)
         {//==>最大显示值 999
             if(by_D2 > 999) by_D2=999;
             if(by_D2/100 != 0)
             {
                 W_Buffer1[by_Word+_MaxWord+1]=by_D2/100+'0';
                 by_Word++;
                 by_D2%=100;
             }
             W_Buffer1[by_Word+_MaxWord+1]=by_D2/10+'0';
             by_Word++;
             W_Buffer1[by_Word+_MaxWord+1]=by_D2%10+'0';
             by_Word++;
         }
         for(i=0;i<_MaxWord+1;i++)
         {
             W_Buffer1[i]=0x20;
             W_Buffer1[by_Word+_MaxWord+1+i]=0x20;
         }
         W_Buffer1[by_Word+((_MaxWord+1)*2)]=0;
         if(by_TargetStart < 6)
         {//==>位移前面一组字用 6排
             by_TargetStart++;
         }
         else
         {//==>前ㄧ笔字型位移完成后,准备下ㄧ笔位移数据
             by_TargetStart=1;
             if(W_Buffer1[by_Seg+_MaxWord+1]==0)//==>当字符串结束时
             {
                  by_Seg=0;
                  by_Show_End=1;
             }
             else by_Seg++;
         }
     }
     by_OldString = 0;
  }
  else
  {//==>单一句子
     Led.Loop=0;
     by_TargetSeg=_MaxWord;
     by_Seg=0;
     by_Show_End=1;
     by_StringStart=2;
     // 不足_MaxWord字需居中显示
     by_StartLine = 0;
     if(by_Word < _MaxWord)
     {
         by_TargetSeg = by_Word;
         by_StartLine = Led_WordCount(W_Buffer1);
     }
     if(by_OldString != by_Word)
     {
         by_OldString = by_Word;
         for(i=0;i<_8x8DotMatrix;i++)
         {
             aw_DotMatrix_Buffer[i]=0x00;
         }
     }
     //
  }
  if(by_StringStart == 1 || by_StringStart == 2)
  {
      while(by_Loop < by_TargetSeg)
      {
          by_Target = W_Buffer1[by_Loop + by_Seg];
          
          by_Target = Led_CheckWordNumber(by_Target);
          for(i = 0;i < 5;i++)
          {
              if(!Led.Loop)
              {//==>非跑马灯
                  aw_DotMatrix_Buffer[by_StartLine+by_Loop*5+i+by_Dot+2]=aw_Code[by_Target][i];// +2为了居中用
              }
              else
              {//==>跑马灯
                  W_Buffer[by_Loop*5+i+by_Dot]=aw_Code[by_Target][i];
              }
          }
          by_Dot++;
          if(!Led.Loop)
          {//==>非跑马灯
              aw_DotMatrix_Buffer[by_StartLine+by_Loop*5+4+by_Dot+2]=0x00;// +2为了居中用
          }
          else
          {//==>跑马灯
              W_Buffer[by_Loop*5+4+by_Dot]=0x00;
          }
          by_Loop++;
      }
      if(Led.Loop)
      {
          for(i=0;i<_8x8DotMatrix;i++)
          {
              aw_DotMatrix_Buffer[i]=W_Buffer[i+by_TargetStart];
          }
      }
      else
      {// 遗漏排数清除
          aw_DotMatrix_Buffer[0]=0x00;
          aw_DotMatrix_Buffer[1]=0x00;
          aw_DotMatrix_Buffer[102]=0x00;
          aw_DotMatrix_Buffer[103]=0x00;
      }
  }
  // 2015.01.22
  Digital_Command_Proce_wifi();
  //
}
*/
/*******************************************************************************
* Function Name  : Led_WordCount()
* Description    : 显示起始排数计算
* Input          : *pby_Dat : 对应字符串
* Output         : None
* Return         : 起始排数
*******************************************************************************/
u8 Led_WordCount(uc8 *pby_Dat)
{
  u8 by_Word=0;
  while(*(pby_Dat+by_Word)!=0) 
  {
      by_Word+=1;
      if(by_Word > 250) return 1;
  }
  by_Word*=6;
  by_Word=_8x8DotMatrix-by_Word;
  return (by_Word/2)+1;
}
/*******************************************************************************
* Function Name  : Led_StringParagraph()
* Description    : 显示提示字符串 (由下往上移动版)
* Input          : by_Line : 显示起始排数 ; *pby_Dat : 对应字符串
* Output         : None
* Return         : by_Return -> 当完成一次上移显示就设为 1
*******************************************************************************/
/*
u8 Led_StringParagraph(u8 by_Line,uc8 *pby_Dat)
{
  u8 i,by_Word=0,by_Target,by_Return=0; 
  static u8 w_Sum=0,w_Chang=0,w_Line=0;
  
  // 2015.01.22
  Digital_Command_Proce_wifi();
  //
  if(by_Line == 255)
  {//==>参数归零且显示暂存清除无显示
      w_Sum=0;
      w_Chang=0;
      w_Line=0;
      for(i=0;i < _8x8DotMatrix;i++)
      {                      
          aw_DotMatrix_Buffer[i]=0x00;
      }
      return 0;
  }
  by_Line--;
  if(Led.Blink1)
  {
      Led.Blink1=0;
      switch(w_Chang)
      {
          case 0://==>由下往上浮起 (渐渐出现)
                 w_Line++;
                 if(w_Line <= 8)
                 {
                     while(*(pby_Dat+by_Word)!=0) 
                     {
                         if(by_Word > 250) break;
                         by_Target = *(pby_Dat+by_Word);
                         by_Target = DotMatrix_TableSearch(by_Target);
                         by_Word++;
                         for(i=0;i<5;i++)
                         {
                             aw_DotMatrix_Buffer[by_Line]=LetterTable[by_Target][i] << (8-w_Line);
                             by_Line++;
                         }
                         by_Line++;
                     }
                 }
                 else if(w_Line >= 10)
                 {
                     w_Chang=1;
                     w_Line=0;
                     w_Sum=0;
                 }    
                 break;
          case 1://==>字符串完全浮起后停留约1.5秒
                 w_Sum++;
                 if(w_Sum > 45)
                 {
                     w_Chang=2;
                     w_Line=0;
                 }    
                 break;
          case 2://==>由全画面往上浮 (渐渐消失)
                 w_Line++;
                 if(w_Line <= 8)
                 {
                     for(i=0;i < _8x8DotMatrix;i++)
                     {                      
                         aw_DotMatrix_Buffer[i]=aw_DotMatrix_Buffer[i] >> 1;
                     }
                 }
                 else if(w_Line >= 10)
                 {
                     w_Chang=0;
                     w_Line=0;
                     by_Return=1;
                 }    
                 break;       
      }
  }
  return by_Return;
}
*/
//-------------------------------------------------------------------------------------------------------------------
//========================================================================================================================
//====>设定数值显示位置设定 1.第几排 2.数值 (5x7字库)
//=============================================================================================
/*
void Led_NumberSite(u8 by_Seg,u8 by_Dat)
{
   u8 i;
   if(by_Seg > 99) by_Seg=99;//==>避免超出显示范围
   for(i=0;i < 5;i++)
   {
      aw_DotMatrix_Buffer[by_Seg-1+i]=aw_Code[by_Dat][i];
   }
}
*/
//========================================================================================================================
//====>设定数值显示位置设定 1.第几排 2.数值 (运动显示值用 4x8字库)
//=============================================================================================
/*
void Led_NumberSite_4x8(u8 by_Seg,u8 by_Dat)
{
   u8 i;
   if(by_Seg > 99) by_Seg=99;//==>避免超出显示范围
   for(i=0;i < 5;i++)
   {
      if(i == 4) 
          aw_DotMatrix_Buffer[by_Seg-1+i]=0x00;
      else 
          aw_DotMatrix_Buffer[by_Seg-1+i]=aw_Code1[by_Dat][i];
   }
   
}
*/
/*******************************************************************************
* Function Name  : Led_HT1381_Information
* Description    : 将HT381的各项时间信息秀出 (24H status)
* Input          : by_Year,by_Month,by_Date,by_Hours,by_Min,by_Sec
* Output         : None
* Return         : None
*******************************************************************************/
void Led_RTCTime(unsigned char *by_RTC,char by_Mode,char by_Seg)
{
  char i;
  char by_Line = 6;
  
  if(by_Mode == 0)
  {// Date
      // Year
      Led_NumberSite_4x8(by_Line,2);
      Led_NumberSite_4x8(by_Line+5,0);
      for(i=0;i<5;i++)
      {//==>显示 =
          aw_DotMatrix_Buffer[by_Line+20+i]=0x10;//==> -
          aw_DotMatrix_Buffer[by_Line+38+i]=0x10;//==> -
      }
  }
  else 
  {// Time
      for(i=0;i<2;i++)
      {
          aw_DotMatrix_Buffer[by_Line+22+i]=0x6C;//==> : 
          aw_DotMatrix_Buffer[by_Line+40+i]=0x6C;//==> :
      }
  }
  for(i=0;i<3;i++)
  {
      if(Led.Blink2 == 0 && by_Seg < 6 && by_Seg == i)
      {
          Led_NumberSite_4x8(by_Line+10+(18*i),10);// 空格
          Led_NumberSite_4x8(by_Line+15+(18*i),10);
      }
      else
      {
          if(by_Mode == 0)
          {// Date Year / Month / Day
              Led_NumberSite_4x8(by_Line+10+(18*i),*(by_RTC+i)/10);
              Led_NumberSite_4x8(by_Line+15+(18*i),*(by_RTC+i)%10);
          }
          else
          {// Time Hour / Minute / Second
              Led_NumberSite_4x8(by_Line+10+(18*i),*(by_RTC+i+3)/10);
              Led_NumberSite_4x8(by_Line+15+(18*i),*(by_RTC+i+3)%10);
          }
      }
  }
  // 2015.01.22
  Digital_Command_Proce_wifi();
  //
}

/*******************************************************************************
* Function Name  : Led_SetData
* Description    : 将LED所要点亮的数据依序传出 (使用IC为 MBI5026CF * 6颗)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Led_SetData(void)
{
  u8 by_Loop,by_Check,by_CheckA;
  
  IO_Clear_CLK();
  IO_Clear_LE();
  
  for(by_Loop = 0;by_Loop < 10;by_Loop++)  
  {
     by_CheckA = Led_Buffer[by_Loop];
     for(by_Check = 0;by_Check < 8;by_Check++)
     {
        if(by_CheckA & BIT7)
        {
            IO_Set_SDI();
        }
        else
        {
            IO_Clear_SDI();
        }
        by_CheckA = by_CheckA << 1;
        IO_Set_CLK();
        IO_Clear_CLK();
     }
  }
  IO_Set_LE();
  IO_Clear_OE();
  IO_Clear_LE();
}
/*******************************************************************************
* Function Name  : Led_ComScan
* Description    : LED扫描脚个别对应传输数据暂存放入
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Led_ComScan(void)
{
  static u8 by_Index=0;
  static u8 by_Paragraph=0;
  u8 i;
  
  for(i = 0;i < 10;i++)
    Led_Buffer[i] = 0x00;
  Led_SetData();// 先清一次显示器
  
  
    if(by_Index < 8)
    {     // COM 1 ~ 8
      Led_Buffer[9] = 0x01 << by_Index;
      Led_Buffer[8] = 0x00;
      // TV Power LED (Green/Red)
      if(by_Index == 1) 
        Led_Buffer[0] = Led_Target[4];                  
      
      Led_Buffer[5] = aw_DotMatrix_Buffer[by_Index];     
      Led_Buffer[4] = aw_DotMatrix_Buffer[16 + by_Index];
      Led_Buffer[3] = aw_DotMatrix_Buffer[32 + by_Index];
      Led_Buffer[2] = aw_DotMatrix_Buffer[48 + by_Index];
      Led_Buffer[1] = 0x00;// 
    }
    else if(by_Index < 16)
    {    // COM 9 ~ 16
        Led_Buffer[9] = 0x00;
        Led_Buffer[8] = 0x01 << by_Index - 8;
 
        Led_Buffer[7] = 0x00;
        Led_Buffer[6] = 0x00;
        Led_Buffer[5] = aw_DotMatrix_Buffer[by_Index];     
        Led_Buffer[4] = aw_DotMatrix_Buffer[16 + by_Index];
        Led_Buffer[3] = aw_DotMatrix_Buffer[32 + by_Index];
        Led_Buffer[2] = aw_DotMatrix_Buffer[48 + by_Index];
        Led_Buffer[1] = 0x00;
        // Overlay LED
        if( by_Index >= 9 && by_Index <= 12 )
        {
          Led_Buffer[0] = Led_Target[(by_Index-9)] ;   
        }         
    }
    else
    {
      // COM 17 ~ 18
      Led_Buffer[1] = 0x01 << by_Index - 16;
      //
      if(by_Index == 16)
      {
          Led_Buffer[7] = aw_7SEG[0];
          Led_Buffer[6] = aw_7SEG[1];
      }
      else if(by_Index == 17)
      {
          Led_Buffer[7] = aw_7SEG[2];
          Led_Buffer[6] = aw_7SEG[3];
      }
  }

  //
  Led_SetData();

  if(++by_Index > 17) 
  {
      by_Index = 0;
      for(i=0;i<_StringWindow_Width;i++)
      {
          aw_DotMatrix_Buffer[i] = DotMatrix_Window[i];
      }
  }
  

  //
  by_Paragraph++;
  if(by_Paragraph >= _MessageSpeed)//39
  {
      by_Paragraph = 0;
      if(!Led.Blink1) Led.Blink1=1;
  }
}  	
// 调整字符串卷动速度
void Led_MessageSpeed(u8 by_D)
{
  _MessageSpeed = by_D;
}
//========================================================================================================================
//====>风扇控制
//=============================================================================================
/*
void Led_FanControlReset(void)
{
    Led.FanStart=0;
    by_Fan = Fan_Close;
    IO_FAN(Fan_Close);
}
void Led_FanControl(void)
{
    by_Fan++;
    if(by_Fan > Fan_HSP)
    {
        by_Fan=Fan_Close;
    }
    switch(by_Fan)
    {
        case Fan_Close:
                 IO_FAN(Fan_Close);
                 break;
        case Fan_LSP:
                 IO_FAN_LSP();
                 break;
        case Fan_MSP:
                 IO_FAN_MSP();
                 break;
        case Fan_HSP:
                 IO_FAN_HSP() 
                 break;
    }
}
*/
/*用来判断风扇是否在运动模式下启动
  0=Yes , 1=NO*/
//void Led_FanStart(u8 by_Dat)
//{
//    Led.FanStart=by_Dat;
//}
/*清除风扇启动累计时间*/
//void Led_FanDelayTimeClear(void)
//{
//    if(!Led.FanStart) by_FanDelayTime=0;
//}
/*******************************************************************************
* Function Name  : Led_CheckWordNumber()
* Description    : 字型代码转换
* Input          : 字码
* Return         : 对应字库代码
*******************************************************************************/
/*
char Led_CheckWordNumber(char by_D)
{
  u8 by_Out = 127;// 空格
  
  if(by_D >= 'A' && by_D <= 'Z') 
       by_Out = by_D - 'A' + 10;
  else if(by_D >= 'a' && by_D <= 'z') 
       by_Out = by_D - 'a' + 36;
  else if(by_D >= '0' && by_D <= '9') 
       by_Out = by_D - '0';
  else if(by_D >= 160 && by_D <= 215) 
       by_Out = by_D - 160 + 62;
  else if(by_D == 216 || by_D == 217) 
       by_Out = by_D - 88;
  else if(by_D == '"') by_Out = 117;
  else if(by_D == '*') by_Out = 118;
  else if(by_D == '`') by_Out = 119;
  else if(by_D == '.') by_Out = 120;
  else if(by_D == ',') by_Out = 121;
  else if(by_D == ':') by_Out = 122;
  else if(by_D == '/') by_Out = 123;
  else if(by_D == '!') by_Out = 124;
  else if(by_D == '%') by_Out = 125;
  else if(by_D == '-') by_Out = 126;
  
  return by_Out;
}
*/
/*******************************************************************************
* Function Name  : Led_Window()
* Description    : 显示窗口用数据显示
* Input          : 1.显示起始排
*                  2.显示模式 1 => xxxx 
*                             2 => xx:xx 
*                             3 => xx.xx 
*                             4 => xxx.x
*                             5 => 清空显示范围
*                             6 => xxxxxxx
*                             7 => xxxxxx.x  
                              8 => xx
*                  3.显示值
* Return         : Not
*******************************************************************************/
void Led_Window(char by_Line,char by_Mode,long unsigned int by_Sum)
{
  char _LineBuffer = 0;
  unsigned char TempData[7] ; 
  
  if(by_Line > 63) by_Line = 63;// 配合显示位数,所以需限制最大显示起始范围
  _LineBuffer = by_Line;
  if((by_Mode == 6 || by_Mode == 7)&&(by_Sum > 9999999))
  {
      by_Sum = 9999999;
  }
  else if(by_Mode < 6 && by_Sum > 9999) 
  {
      by_Sum = 9999;
      if(by_Mode == 2)
      {
          by_Sum = 9959;
      }
  }
  
  long unsigned int _BB = 1000000;
  char i;
  for(i=0;i<6;i++)
  {
      TempData[i] = by_Sum / _BB ;
      by_Sum %= _BB;
      _BB /= 10;
  }
  TempData[6] = by_Sum % 10 ;
  switch( by_Mode )
      {
      default :
      case 1  ://xxxx
              Led_NumberSite_4x8(_LineBuffer,(( TempData[3] != 0 )?TempData[3]:10)); 
              Led_NumberSite_4x8(_LineBuffer+5,(( TempData[4] != 0 || TempData[3] != 0 )?TempData[4]:10)); 
              Led_NumberSite_4x8(_LineBuffer+10,(( TempData[5] != 0 || TempData[4] != 0 || TempData[3] != 0 )?TempData[5]:10)); 
              Led_NumberSite_4x8(_LineBuffer+15,TempData[6]);
              break ;
      case 2  ://xx:xx
      case 3  ://xx.xx  
              Led_NumberSite_4x8(_LineBuffer,(( TempData[3] != 0 )?TempData[3]:10));
              Led_NumberSite_4x8(_LineBuffer+5,TempData[4]); 
              //aw_DotMatrix_Buffer[_LineBuffer+9] = (( by_Mode == 2 )?0x24:0x80);// 0x24=: / 0x80=.
              DotMatrix_Window[_LineBuffer+9] = (( by_Mode == 2 )?0x24:0x80);// 0x24=: / 0x80=.
              Led_NumberSite_4x8(_LineBuffer+12,TempData[5]);
              Led_NumberSite_4x8(_LineBuffer+17,TempData[6]);
              break ;
      case 4  ://xxx.x
              Led_NumberSite_4x8(_LineBuffer,(( TempData[3] != 0 )?TempData[3]:10)); 
              Led_NumberSite_4x8(_LineBuffer+5,(( TempData[4] != 0 || TempData[3] != 0 )?TempData[4]:10)); 
              Led_NumberSite_4x8(_LineBuffer+10,TempData[5]);
              //aw_DotMatrix_Buffer[_LineBuffer+14] = 0x80;// .
              DotMatrix_Window[_LineBuffer+14] = 0x80;// .
              Led_NumberSite_4x8(_LineBuffer+17,TempData[6]);
              break ;
      case 5  :
              Led_NumberSite_4x8(_LineBuffer,10);//==>秀空格
              Led_NumberSite_4x8(_LineBuffer + 5,10);//==>秀空格
              Led_NumberSite_4x8(_LineBuffer + 10,10);//==>秀空格
              Led_NumberSite_4x8(_LineBuffer + 15,10);//==>秀空格
              //aw_DotMatrix_Buffer[_LineBuffer + 19] = 0x00;
              DotMatrix_Window[_LineBuffer + 19] = 0x00;
              break ;
      case 6  :// xxxxxxx
      case 7  :// xxxxxx.x  
              Led_NumberSite_4x8(_LineBuffer,   (( TempData[0] != 0 )?TempData[0]:10)); 
              Led_NumberSite_4x8(_LineBuffer+5, (( TempData[0] != 0 || TempData[1] != 0 )?TempData[1]:10)); 
              Led_NumberSite_4x8(_LineBuffer+10,(( TempData[0] != 0 || TempData[1] != 0 || TempData[2] != 0)?TempData[2]:10));
              Led_NumberSite_4x8(_LineBuffer+15,(( TempData[0] != 0 || TempData[1] != 0 || TempData[2] != 0 || TempData[3] != 0)?TempData[3]:10)); 
              Led_NumberSite_4x8(_LineBuffer+20,(( TempData[0] != 0 || TempData[1] != 0 || TempData[2] != 0 || TempData[3] != 0 || TempData[4] != 0 )?TempData[4]:10)); 
              Led_NumberSite_4x8(_LineBuffer+25,(( TempData[0] != 0 || TempData[1] != 0 || TempData[2] != 0 || TempData[3] != 0 || TempData[4] != 0 || TempData[5] != 0 )?TempData[5]:10));
              if(by_Mode == 6)
                  Led_NumberSite_4x8(_LineBuffer+30,TempData[6]);
              else
              {
                  //aw_DotMatrix_Buffer[_LineBuffer+29] = 0x80;// .
                  DotMatrix_Window[_LineBuffer+29] = 0x80;// .
                  Led_NumberSite_4x8(_LineBuffer+32,TempData[6]);
              }
              break ;    
      case 8:
              Led_NumberSite_4x8(_LineBuffer,TempData[5]);
              Led_NumberSite_4x8(_LineBuffer+5,TempData[6]);
              break ;
      }
  // 2015.01.22
  Digital_Command_Proce_wifi();
  //
}
//=================================
// 矩阵显示器图形显示 MATRIX 字符串
// 0 : 重新启动
// by_D1 : 0=>MATRIX
//==================================
uc16 aw_MATRIX[70]=
{
    0xFF,0xFF,0x06,0x0C,0x38,0x38,0x0C,0x06,
    0xFF,0xFF,0x00,0xC0,0xF0,0x3C,0x0E,0x63,
    0x63,0x0E,0x3C,0xF0,0xC0,0x03,0x03,0x03,
    0xFF,0xFF,0x03,0x03,0x03,0x00,0xFF,0xFF,
    0x03,0x33,0x33,0x73,0xDF,0x9F,0x00,0x00,
    0xFF,0xFF,0x00,0x00,0x81,0xC3,0x66,0x3C,
    0x18,0x3C,0x66,0xC3,0x81,0x00,0x00,0xFF,
    0xFF,0x06,0x0C,0x18,0x30,0x60,0x60,0x30,
    0x18,0x0C,0x06,0xFF,0xFF,0x00
};
/*
void LED_MATRIX(unsigned char by_D)
{
    unsigned char i;
    unsigned char _Size;
    static unsigned char by_Line = 0;
    static unsigned char by_Old = 0;
    unsigned short *pby_Map;
    unsigned char by_Action = 0;
    unsigned short by_MASK = (1<<BigMatrix_Height) - 1;
    unsigned char LogoNum = 0;//Bike_Get_Parameter(_Bike_CustomLogo);
    
    //
    if(by_D == _BigMatrix_Clear || by_Old != LogoNum )
    {// 清除资料
        by_Line = 0;
        by_Old = LogoNum;
        for(i = 0;i < BigMatrix_Width;i++)
            aw_16x16DotMatrix[i] = 0x00;
        Led.Blink3 = 0;
        return;
    }  
    
    if(Led.Blink3 == 1)
    {
        Led.Blink3 = 0;
        if(LogoNum == Logo_MATRIX)
        {
            pby_Map = (unsigned short*)aw_MATRIX;
            _Size = 70;
            by_Action = _Pattern_MovedStop;
        }
        else
        {
            pby_Map = Pattern_PatternInfo;
            _Size = Pattern_GetPatternColumnAmount();
            by_Action = Pattern_GetPatternMotion();
        }

        switch(by_Action)
        {
            default:
            case _Pattern_Fixed:
                for(i = 0;i < BigMatrix_Width; i++)
                    aw_16x16DotMatrix[i] = (*(pby_Map+i))&by_MASK;
                break;
            case _Pattern_Moved:
                for(i = 0;i < (BigMatrix_Width - 1); i++)
                    aw_16x16DotMatrix[i] = aw_16x16DotMatrix[i + 1]; 
                aw_16x16DotMatrix[BigMatrix_Width-1] = ((by_Line < _Size)?(*(pby_Map+by_Line)):0x0000)&by_MASK;
                if((++by_Line) >= (_Size + BigMatrix_Width))
                    by_Line = 0;
                break;
            case _Pattern_MovedStop:
                if(by_Line < _Size)
                {
                    for(i = 0;i < (BigMatrix_Width - 1); i++)
                        aw_16x16DotMatrix[i] = aw_16x16DotMatrix[i + 1];
                    aw_16x16DotMatrix[BigMatrix_Width-1] = (*(pby_Map+by_Line))&by_MASK;   
                }
                if((++by_Line) > (_Size + 30)) // 30*0.12 = 3.6 sec.
                    by_Line = 0;
                break;    
        }
    }
}*/
//=================================
// 矩阵显示器图形显示 END 字符串
//==================================
//uc8 aw_END[16]=
//{
// 0x00,0xcff3,0xc893,0xc893,0xc893,0xc003,0xcff3,0xc083,0xc103,0xcff3,0xc003,0xcff3,
// 0xc813,0xc813,0xc7e3,0xc003 
//};

void LED_END(void)
{
  u8 i;
  
  for(i = 0;i < 16;i++)
  {
     aw_16x16DotMatrix[i] = 0;//aw_END[i];
  }
}
/*******************************************************************************
* Function Name  : Led_7SEG()
* Description    : Big 7 窗口用数据显示
* Input          : 1.显示起始排
*                  2.显示模式 1 => xxxx 
*                             2 => xx:xx 
*                             3 => xx.xx 
*                             4 => 清空显示范围
*                             5 => xx.x 
*                  3.显示值
* Return         : None
*******************************************************************************/
extern unsigned char aw_7SEG_Code1[10];
void Led_7SEG(unsigned char by_Mode,unsigned short by_Sum)
{
  unsigned char TempData[4] ; 
     
  if(by_Sum > 9999) 
  {
      by_Sum = 9999;
      if(by_Mode == 2)
      {
          by_Sum = 9959; // 99:59
      }
  }
  TempData[0] = by_Sum / 1000 ;
  by_Sum %= 1000;
  TempData[1] = by_Sum / 100 ;
  by_Sum %= 100;
  TempData[2] = by_Sum / 10 ;
  TempData[3] = by_Sum % 10 ;
  switch( by_Mode )
      {
       default :
       case 1://xxxx
              if( TempData[0] != 0 )
                  aw_7SEG[0] = aw_7SEG_Code1[TempData[0]];
              else
                  aw_7SEG[0] = 0x00;
              if( TempData[1] != 0 || TempData[0] != 0)
                  aw_7SEG[1] = aw_7SEG_Code1[TempData[1]];
              else
                  aw_7SEG[1] = 0x00;
              if( TempData[2] != 0 || TempData[1] != 0 || TempData[0] != 0)
                  aw_7SEG[2] = aw_7SEG_Code1[TempData[2]];//2
              else
                  aw_7SEG[2] = 0x00;
              aw_7SEG[3] = aw_7SEG_Code1[TempData[3]];//2
              break;
       case 2://xx:xx
              if( TempData[0] != 0 ) 
                  aw_7SEG[0] = aw_7SEG_Code1[TempData[0]]; 
              else
                  aw_7SEG[0] = 0x00;
              aw_7SEG[1] = aw_7SEG_Code1[TempData[1]]|0x80; 
              //aw_7SEG[1] |= 0x80;
              aw_7SEG[2] = aw_7SEG_Code1[TempData[2]]|0x80;//2
              //aw_7SEG[2] |= 0x80;
              aw_7SEG[3] = aw_7SEG_Code1[TempData[3]];
              break ;
       case 3://xx.xx
              if( TempData[0] != 0 )
                  aw_7SEG[0] = aw_7SEG_Code1[TempData[0]]; 
              else
                  aw_7SEG[0] = 0x00;//==>秀空格
              aw_7SEG[1] = aw_7SEG_Code1[TempData[1]]|0x80; 
              //aw_7SEG[1] |= 0x80;
              aw_7SEG[2] = aw_7SEG_Code1[TempData[2]];
              aw_7SEG[3] = aw_7SEG_Code1[TempData[3]];
              break ;   
       case 4:// Clear
              aw_7SEG[0] = 0x00;//==>秀空格
              aw_7SEG[1] = 0x00;
              aw_7SEG[2] = 0x00;
              aw_7SEG[3] = 0x00;
              break ;
       case 5://xx.x
              if( TempData[0] != 0 )
                  aw_7SEG[0] = aw_7SEG_Code1[TempData[0]]; 
              else
                  aw_7SEG[0] = 0x00;//==>秀空格
              aw_7SEG[1] = aw_7SEG_Code1[TempData[1]]|0x80; 
              //aw_7SEG[1] |= 0x80;
              aw_7SEG[2] = aw_7SEG_Code1[TempData[2]];
              aw_7SEG[3] = 0x00;//==>秀空格
//              aw_7SEG[3] = aw_7SEG_Code1[TempData[3]];
              break ;       
              /*
       case 6:// ----
              aw_7SEG[0] = 0x40;// -
              aw_7SEG[1] = 0x40;
       case 7://   --       
              aw_7SEG[2] = 0x40;// -
              aw_7SEG[3] = 0x40;
              break ;   
              */
    }
}
// 2014.11.24
/*******************************************************************************
* Function Name  : Led_xIDNumber
* Description    : xID 输入显示
* Input          : 1. 0=xID;1=PassCode 2. 位数 3. 对应数据
* Output         : None
* Return         : None
*******************************************************************************/
void Led_xIDNumber(u8 by_Mode,u8 by_D,char *by_D1)
{
  u8 by_B1,by_B2;
  u8 _Line = 51;//98;
  
  if(by_Mode == 0)
  {// xID
      by_B1 = 9;
      by_B2 = 14;
  }
  else
  {// Pass Code
      by_B1 = 3;
      by_B2 = 4;
  }
  if(by_D == 0)
  {// 清除"*"仅留"_"即可
      Led_NumberSite(_Line,127);// 空格
  }
  else
  {
      if(by_D >= 2 && by_D <= by_B1)
      {// 数据位移并显示"*"
          if(by_D < by_B1)
          {// 确保"*"的显示前一位没有其它显示
              Led_NumberSite(_Line-(by_D*6),127);// 空格
          }
          Led_NumberSite(_Line-((by_D-1)*6),134);// *
      }
      if(by_D == 1)
      {// 清除"*"仅留第一位显示即可
          Led_NumberSite(_Line-6,127);// 空格
      }
      if(by_D < by_B2)
          Led_NumberSite(_Line,*(by_D1+(by_D-1))-'0');
      else
      {// 达到最大位数时将不显示"_"
          Led_NumberSite(_Line,134);// *
          Led_NumberSite(_Line+6,*(by_D1+(by_D-1))-'0');
      }
  }
  if(by_D < by_B2)
  {// 最大位数内"_"符号闪烁
      if(Led.Blink2 == 0)
          Led_NumberSite(_Line+6,127);// 空格
      else
          Led_NumberSite(_Line+6,135);// _
  }

}
//




//=================================
// 通讯累计次数显示用
//==================================
/*
void Led_CommunicationTestNumber(u8 by_D,u32 by_D1)
{
  u8 _NumberBu[7];
  u8 _Line = 2;
  u8 i;
  
  _NumberBu[6] = by_D1 / 1000000;
  by_D1 %= 1000000;
  _NumberBu[5] = by_D1 / 100000;
  by_D1 %= 100000;
  _NumberBu[4] = by_D1 / 10000;
  by_D1 %= 10000;
  _NumberBu[3] = by_D1 / 1000;
  by_D1 %= 1000;
  _NumberBu[2] = by_D1 / 100;
  by_D1 %= 100;
  _NumberBu[1] = by_D1 / 10;
  _NumberBu[0] = by_D1 % 10;
  
  if(by_D != 0) _Line = 64;
  for(i=0;i<=6;i++)
  {
      Led_NumberSite_4x8(_Line,_NumberBu[6 - i]);
      if(by_D == 0)
      {// TX
          _Line+=6;
      }
      else
      {// RX
          _Line+=6;
      }
  }
  
}
*/
