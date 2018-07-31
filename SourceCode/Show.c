#include "General.h"
#include "led.h"
#include "Message.h"
#include "Console.h"
#include "heart.h"
#include "EEPRom.h"
#include "timer.h"
#include "C_safe.h"
#include "Digital.h"
#include "Beep.h"
#include "Show.h"
#include "keyboard.h"
#include "UsbFunction.h"
#include <stdio.h> // 使用 sprintf() command加入
#include "EEPRom_Language.h"
#include "Show_Functions.h"
//
//

u8 by_SummaryShow;
// 100.09.28
u8 _RF900MessageOld = 0;
//
u8 _WindowsClear = 0;
/*******************************************************************************
* Function Name  : Show_WorkoutSummary()
* Description    : 显示运动结果信息
* Input          : Nome                      
* Output         : None
*******************************************************************************/
void Show_WorkoutSummary(void)
{
  u8 _Status = 0;
  u8 i;
  u8 _Show = 0;
  u16 _Buffer = Console_Get_CsafeAccTime();
  
  Led_Set_Target(Display_OFF,0);
  //LED_END();
  Timer_Counter_Clear(2);
  
  if((_Buffer/60*100)+(_Buffer%60) > 9959)// 超出显示范围
      _Buffer = 9959;
  else
      _Buffer = (_Buffer/60*100)+(_Buffer%60);
  Show_Message(Message_Blank,0,0,Show_Blank);
  while(_Status == 0)
  {
      //UsbFunction_USB(0x500);
      //USB_SaveWorkoutSummary(); 
      if(Timer_Counter(T_STEP,2,50) == 1)
      {
          Timer_Counter_Clear(2);
          for(i=0;i<3;i++)
          {
//              if(i < 3) Led_Set_Target(Target_HRInfor+i,0);
              Led_Set_Target(Target_DistanceMiles+i,0);
              Led_Set_Target(Target_Calories+i,0);
          }
          _Show++;
          if(_Show > 5) _Status = 1;
          Led_Window(Window_1,5,0);// 窗口清除
          Led_Window(Window_2,5,0);
          Led_Window(Window_4,5,0);
      }
      Led_7SEG(2,_Buffer);
      switch(_Show)
      {
          case 0:
          case 3:
//                 Show_HeartRate();
                 Show_Distance(C_MILE);
                 Show_Calories();
                 break;
          case 1:
          case 4:
//                 Show_Incline();
                 Show_HeartRate();
                 Show_Pace(Target_Pace);
                 break;
          case 2:
          case 5:
                 Show_Incline();
//                 Show_Pace(Target_Pace);
                 Show_Speed();
                 break;
          case 6:
          case 7:
                 Show_METs();
                 Show_Pace(Target_AveragePace);
                 Show_Watts();
                 break;
                 
      }
      if(Key_Proce() == K_STOP || Check_SafeKey() == 0) _Status = 1;
  }
  
  
}
/*******************************************************************************
* Function Name  : Show_SetSummaryNumber()
* Description    : 设定显示字符串后数据的显示方式
* Input          : by_Dat = 显示方式 0(xxxx)1(xx:xx)2,3,4(xx.x)                       
* Output         : None
* Return         : None
*******************************************************************************/
void Show_SetSummaryNumber(u8 by_Dat)
{
    by_SummaryShow=by_Dat;
}
/*******************************************************************************
* Function Name  : Show_MessageParagraph()
* Description    : 显示提示字符串 (由下往上移动版)
* Input          : 1.*pby_ShowNumber : 对应字符串  
*                  2.by_MessageNumber : 字符串代码
*                  3.by_D1 : 是否在跑马灯后加上数值显示 0:NO 1:YES 2:重置参数后离开
*                  4.by_D2 : 跑马灯后的显示值 <最大值99999>                         
* Output         : None
* Return         : 0=非跑马灯 / 1=跑马灯 
*******************************************************************************/
/*
#define MessageBuffer 120
#define AddInformation 50

unsigned char Show_MessageParagraph(uc8 *pby_ShowNumber,u8 by_MessageNumber,u8 by_D1,u32 by_D2)
{
  static u8 by_OldMessage=0;
  static u8 w_TargetMessage[19];
  static u8 by_TargetMessage=0;
  static u8 by_ShowLine=0;
  static u8 by_MessageCheck=0;
  static u8 by_MessageEND=0;
  unsigned char by_Buffer = _Message_Keep;
  char _MessageBuffer[MessageBuffer] = {0}; // 暂存区
  char _AddInformation[AddInformation] = {0}; // 暂存区
  
  u8 i,j;
  uc8 *pby_Message;
  
  

  pby_Message = w_TargetMessage;
  if(by_OldMessage != by_MessageNumber || by_D1 == 2)
  {//==>当输入不同字符串代码时就需更新代码状态
      if(by_D1 != 2)
      {
          //Led_StringParagraph(255,0);//==>清除画面
          Show_Message(Message_Blank,0,0,Show_Blank);
          by_OldMessage = by_MessageNumber;
      }
      by_TargetMessage=0;
      by_MessageCheck=1;
      by_MessageEND=0;
      Led_Set_Reset();//==>第一次显示时需要清除相关旗标
      if(by_D1 == 2)
      {
          by_OldMessage=0;
          return _Message_NotYet;
      }
      
  }
  if(by_MessageCheck == 1)
  {
      if(by_SummaryShow == 1)
      {
          if(by_D2 > 99999) by_D2=99999;
      }
      else
      {
          if(by_D2 > 9999) by_D2=9999;
      }    
      by_MessageCheck=0;
      
      // 字符串放入暂存区
      j = 0;
      for(i = 0;i < MessageBuffer;i++)
      {
          _MessageBuffer[i] = *(pby_ShowNumber + i);
          if(by_D1 == 1 && *(pby_ShowNumber + i) == '%')
          {// 为避免在字符串加入数据出错所以先移除
              _MessageBuffer[i] = ' ';
              j = i;
          }
          if(*(pby_ShowNumber + i) == 0)
          {
              break;
          }
      }
      //
      if(by_D1 == 1)
      {// 字符串后面加入数据
          _MessageBuffer[i] = '%';
          i++;
          switch(by_SummaryShow)
          {
              case 2:// xx.x
              case 3:// xx.x
              case 4:// xx.x  
              case 5:// xx.xx  
                     _MessageBuffer[i] = 'd';
                     i++;
                     _MessageBuffer[i] = '.';
                     i++;
                     _MessageBuffer[i] = '%';
                     i++;
                     _MessageBuffer[i] = '0';
                     i++;
                     if(by_SummaryShow == 5)
                         _MessageBuffer[i] = '2';
                     else
                         _MessageBuffer[i] = '1';
                     i++;
                     break;
              case 1:// xxx:xx  
                     _MessageBuffer[i] = 'd';
                     i++;
                     _MessageBuffer[i] = ':';
                     i++;
                     _MessageBuffer[i] = '%';
                     i++;
                     _MessageBuffer[i] = '0';
                     i++;
                     _MessageBuffer[i] = '2';
                     i++;
              //case 0://==>xxxx       
                     break;
          }
          _MessageBuffer[i] = 'd';
          i++;
          _MessageBuffer[i] = 0;// end
          // 合并字符串与数据
          if(by_SummaryShow == 0)
              sprintf(_AddInformation,_MessageBuffer,by_D2);
          else if(by_SummaryShow == 1 || by_SummaryShow == 5)
              sprintf(_AddInformation,_MessageBuffer,by_D2 / 100,by_D2 % 100); 
          else  
              sprintf(_AddInformation,_MessageBuffer,by_D2 / 10,by_D2 % 10);   
          //
          for(i = 0;i < AddInformation;i++)
          {// 放入显示用暂存区
              _MessageBuffer[i] = _AddInformation[i];
              if(j != 0 && i == j) _MessageBuffer[i] = '%';// 还原 % 字码
              if(_AddInformation[i] == 0) break;
          }
      }
      for(i=0;i < 18;i++)
      {//==>一次取18个字
          w_TargetMessage[i] = _MessageBuffer[by_TargetMessage];  
          if(_MessageBuffer[by_TargetMessage] == 0)
          {//==>当字符串结束时
              i=100;            
              by_TargetMessage=0;
              by_MessageEND=1;
          }   
          else
          {
              by_TargetMessage++;
              if(i == 17)
              {//==>取完字数后判断显示字句长度
                  
                  w_TargetMessage[18] = _MessageBuffer[by_TargetMessage];
                  if(by_TargetMessage == 18 && w_TargetMessage[18] == 0) 
                  {//==>当有跑马灯加数字显示或单一字符串非跑马灯显示时
                      by_MessageCheck=2;
                      by_TargetMessage=0;
                      
                  }
                  else if(w_TargetMessage[17] != 0x20 && w_TargetMessage[18] != 0x20)
                  {//==>当还有下一单字存在
                      j=16;
                      while(w_TargetMessage[j] != 0x20) 
                      {//==>位移至前一单字
                          by_TargetMessage--;
                          j--;
                          if(j == 0) break;
                      }
                      by_TargetMessage--;
                      w_TargetMessage[j]=0;  
                  }
                  else
                  {
                      w_TargetMessage[18]=0;
                  } 
                  // 字符串居中前处理
                  j = 0;
                  if(w_TargetMessage[0] == 0x20)
                  {// 第1个字为空格时字符串往前移一位
                      
                      while(w_TargetMessage[j+1] != 0) 
                      {
                          w_TargetMessage[j] = w_TargetMessage[j+1];
                          j++;
                      }
                      w_TargetMessage[j] = 0;
                  }
                  else
                  {// 最后一个字为空格时结束位往前移一位
                      while(w_TargetMessage[j] != 0)
                      {
                          j++;
                      }
                      if(w_TargetMessage[j-1] == 0x20)
                      {
                          w_TargetMessage[j-1] = 0;
                      }
                  }
                  //
              }
          }
      }
      by_ShowLine = Led_WordCount(pby_Message);//==>计算显示居中字符串起始排数
  }
  if(by_MessageCheck == 2)
  {//==>一般正常跑马灯(由右至左)或单一字符串非跑马灯
      Led_String((unsigned char *)pby_ShowNumber,by_D1,by_D2);
  }
  else
  {
      // 字符串卷动与否判断
      //if(__strlen((const char *)_MessageBuffer) != 18)
      //{
          by_Buffer = _Message_Loop;
      //}
      //
      if(Led_StringParagraph(by_ShowLine,pby_Message))
      {//==>当字符串移动完成一次后就准备显示下一段落
          by_MessageCheck=1;
          if(by_MessageEND == 1)
          {//==>当字符串跑完一次时就将结束旗标 +1
              Led_Set_Strig_End(); 
              by_MessageEND=0;
          }
      }    
  }
  return by_Buffer;  
}
*/
/*******************************************************************************
* Function Name  : Show_Message()
* Description    : 显示提示字符串
* Input          : 1.by_Dat:字符串代码;
                   2.是否在跑马灯后加上数值显示 0:NO 1:YES;
                   3.跑马灯后的显示值 <最大值999> 
                   4.字符串显示方式
* Output         : None
* Return         : 0=单字符串 / 1=跑马灯 / 2=不作动
*******************************************************************************/
unsigned char Show_Message(u8 by_Dat,u8 by_D1,u16 by_D2,u8 Motion)
{
  unsigned char by_Out=0;
  unsigned char by_DispalyMode = 0;

  if(by_Dat == Message_StartingSpeedGrade)
  {//==>此字符串有分公英制
      if(Console_SpeedUnit(Data_Get,0) == C_KM) by_Dat+=1;
  }

  if(by_D1 != 0)
  {
      switch(by_D1)
      {
          default: by_DispalyMode = by_D1;break;
          case _Message_AddNumber:
              by_DispalyMode = 0;
              break;
          case _Message_AddNumber_DF:
              by_DispalyMode = _MsgAddNum_Fixed_3Int;
              break;
          case _Message_AddNumber_Time_DF:
              by_DispalyMode = _MsgAddNum_Fixed_Time;
              break;
          case _Message_AddNumber_Time:
              by_DispalyMode = _MsgAddNum_Time;
              break;    
      } 
  }
  
  if(Language_GetString(_Lang_Language_Data_,by_Dat) == _Lang_Success_)
  {
      if(by_D1 == 0)
          by_Out = Led_String(Lang_StringBuffer,by_Dat,Motion);
      else
          by_Out = Show_MessageWithNumber(Lang_StringBuffer, by_Dat, by_D2, by_DispalyMode, Motion);
  }
  else
  {
      if(by_D1 == 0)
          by_Out = Led_String((unsigned char*)Message_English[by_Dat],by_Dat,Motion);
      else
          by_Out = Show_MessageWithNumber((unsigned char*)Message_English[by_Dat], by_Dat, by_D2, by_DispalyMode, Motion);
  }
  if(by_Out == 0)
      return 0;// 单字符串
  else if(by_Out == 1)
      return 1;// 跑马灯
  else
      return 2;// 不作动
}
/*******************************************************************************
* Function Name  : Show_MessageWithNumber()
* Description    : 显示 (提示字符串 + 数字)
* Input          : 1.string:提示字符串 
                   2.stringNumber:字符串代码
                   3.by_Dat:数字
                   4.by_Mode:显示方式
                   5.字符串显示动作
* Output         : None
* Return         : 0=单字符串 / 1=跑马灯
*******************************************************************************/    
unsigned char Show_MessageWithNumber(unsigned char* string, unsigned char stringNumber, unsigned long by_Dat, unsigned char by_Mode, unsigned char Motion)
{
    unsigned char by_Out=0;
    unsigned char by_Display[_TheWordsOfOneString_] = {0};
    unsigned char i = 0, j = 0;
    unsigned char by_Buff[10] = {0};    
      
    switch(by_Mode)
    {
        case _MsgAddNum_Fixed_Time: /* 时间，固定位数 */
            sprintf((char*)by_Buff, "%2d:%1d%1d", by_Dat/60, (by_Dat%60)/10, (by_Dat%60)%10);
            break;
        case _MsgAddNum_Fixed_Time_WithOff:  /* 时间，固定位数, 0:00 = Off*/
            if(by_Dat == 0)
                sprintf((char*)by_Buff, " OFF ");
            else  
                sprintf((char*)by_Buff, "%2d:%1d%1d", by_Dat/60, (by_Dat%60)/10, (by_Dat%60)%10);
            break;
        case _MsgAddNum_Time: /* 时间 */
            sprintf((char*)by_Buff, "%d:%1d%1d", by_Dat/60, (by_Dat%60)/10, (by_Dat%60)%10);
            break;    
        case _MsgAddNum_Fixed_2Int: /* 固定位数，2整数 */
            sprintf((char*)by_Buff, "%2d", by_Dat);
            break;
        case _MsgAddNum_Fixed_3Int: /* 固定位数，3整数 */
            sprintf((char*)by_Buff, "%3d", by_Dat);
            break;
        case _MsgAddNum_Fixed_4Int: /* 固定位数，4整数 */
            sprintf((char*)by_Buff, "%4d", by_Dat);
            break;
        case _MsgAddNum_Fixed_5Int: /* 固定位数，5整数 */
            sprintf((char*)by_Buff, "%5d", by_Dat);
            break;
        case _MsgAddNum_Fixed_6Int: /* 固定位数，6整数 */
            sprintf((char*)by_Buff, "%6d", by_Dat);
            break;
        case _MsgAddNum_Fixed_6Int1Dec: /* 固定位数，6整数+1位小数 */
            sprintf((char*)by_Buff, "%6d.%1d", by_Dat/10, by_Dat%10);
            break;
        default:
            sprintf((char*)by_Buff, "%d", by_Dat);
            break;
    }
    
    i = 0;
    while(*(string+i) != 0)
    {
        by_Display[i] = *(string + i);
        i++;
    }
    
    j = 0;
    while(by_Buff[j] != 0)
    {
        by_Display[i] = by_Buff[j];
        i++;
        j++;
    }
    by_Display[i] = 0;
      
    by_Out = Led_String(by_Display, stringNumber, Motion);
  return by_Out;  
}
/*******************************************************************************
* Function Name  : Show_Time
* Description    : 显示运动时间计数
* Input          : 0 -> Time_DOWN 
*                  1 -> Time_UP
*                  2 -> Time_Program
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Time(u8 by_D)
{
    u16 by_TBuffer;
    
    if(by_D == Time_UP)
    {
//        Led_Set_Target(Target_TimeElapsed,1);
        if(Console_Get_TimeDIR() == 1)
        {//==>正数
            by_TBuffer = Console_Information(Info_TIME,Data_Get,0);//Console_Time(Data_Get,0);
        }
        else
        {//==>倒数
            by_TBuffer = Console_UserTime(Data_Get,0) - Console_Information(Info_TIME,Data_Get,0);//Console_Time(Data_Get,0);
        }
    }
    else if(by_D == Time_DOWN)
    {
        Led_Set_Target(Target_TimeRemaining,1);
        if(Console_Get_TimeDIR() == 1)
        {//==>正数
            by_TBuffer = Console_MaxTime(Data_Get,0) - Console_Information(Info_TIME,Data_Get,0);//Console_Time(Data_Get,0);
        }
        else
        {//==>倒数
            by_TBuffer = Console_Information(Info_TIME,Data_Get,0);//Console_Time(Data_Get,0); 
        }   
    }
    else
    {
        Led_Set_Target(Target_TotalTime,1);
        //by_TBuffer = Console_GetNewTime(); 
        by_TBuffer = Console_NewTime(Data_Get);
    }
    Led_7SEG(2,(by_TBuffer/60*100)+(by_TBuffer%60));
}
/*******************************************************************************
* Function Name  : Show_Clock
* Description    : 显示时钟
* Input          : 时间
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Clock(unsigned int by_Dat)
{
  Led_Set_Target(Target_Clock,1);
  Led_Window(Window_3,2,by_Dat);  
}
/*******************************************************************************
* Function Name  : Show_HeartRate
* Description    : 显示心跳值
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_HeartRate(void)
{
    if(Heart_Get_Heart() > 40)
        Led_Set_Target(Target_HRInfor,2);
    else
        Led_Set_Target(Target_HRInfor,1);
    Led_Window(Window_1,1,Heart_Get_Heart());
}
/*******************************************************************************
* Function Name  : Show_Speed
* Description    : 显示目前设定速度
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Speed(void)
{
  Led_Set_Target(Target_Speed,1);
#ifdef _DebugClose
    u32 by_Sum;
    if(Console_DisplaySpeedChange(Data_Get,0) == 0)
    {
        Led_Window(Window_2,4,Console_Information(Info_WorkoutSpeed,Data_Get,0)/ 10);//Console_Speed(Data_Get,0)
    }    
    else
    {
        by_Sum = Digtial_Get_MotorRpm() /10;
        if((by_Sum <= (Console_Information(Info_WorkoutSpeed,Data_Get,0)/10)+1)&& \
           (by_Sum >= (Console_Information(Info_WorkoutSpeed,Data_Get,0)/10)-1))//Console_Speed(Data_Get,0)
        {
            Console_DisplaySpeedChange(Data_Set,0);
            Led_Window(Window_2,4,Console_Information(Info_WorkoutSpeed,Data_Get,0) / 10);//Console_Speed(Data_Get,0)
        }
        else 
        {
            Led_Window(Window_2,4,by_Sum);
        }
    }
#else  
    Led_Window(Window_2,4,Console_Information(Info_WorkoutSpeed,Data_Get,0)/ 10);//Console_Speed(Data_Get,0)
#endif    
}
/*******************************************************************************
* Function Name  : Show_Incline
* Description    : 显示目前设定升降
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Incline(void)
{
  Led_Set_Target(Target_Incline,1);
  Led_Window(Window_1,4,Console_Information(Info_WorkoutIncline,Data_Get,0) * 10 / 2);//Console_Incline(Data_Get,0)
}
/*******************************************************************************
* Function Name  : Show_Distance
* Description    : 显示距离
* Input          : 1 -> C_KM
*                  0 -> C_MILE 
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Distance(u8 by_Data)
{
    u16 by_Dat = Console_Get_Distance();
    u16 by_D1,by_Bu;//by_D,
    u8 by_x=1;
    if(Console_SpeedUnit(Data_Get,0) == C_KM)
      by_Data = C_KM;
    else
      by_Data = C_MILE;
    if(Console_ShowDistanceTarget(Data_Get,0) == 0 &&\
      (Console_ProgramMode(Data_Get,0) == TM_5K ||\
       Console_ProgramMode(Data_Get,0) == TM_10K))
    {//==> 5K/10K Run mode
        if(Console_ProgramMode(Data_Get,0) == TM_5K)
            by_D1=500;
        else
            by_D1=1000;
        if(Console_Get_GoalDistance() >= by_D1) 
            by_Bu = (u16)((float)by_Dat * 1.609) / 10;
        else
            by_Bu = by_D1 - Console_Get_GoalDistance();
        if(by_Data == C_MILE)
        {
            by_Bu = (u16)((float)by_Bu * 0.62137);
        }
    }
    else if(Console_ShowDistanceTarget(Data_Get,0) == 0 && \
           (Console_ProgramMode(Data_Get,0) == TM_PEB ||  
            Console_ProgramMode(Data_Get,0) == TM_Army || 
            Console_ProgramMode(Data_Get,0) == TM_Airforce || 
            Console_ProgramMode(Data_Get,0) == TM_Marine ))  
    {//==>Fitness test mode
        if(Console_ProgramMode(Data_Get,0) == TM_PEB || 
           Console_ProgramMode(Data_Get,0) == TM_Airforce )
        {
            by_D1 = 150;
        }
        else if(Console_ProgramMode(Data_Get,0) == TM_Army) 
        {
            by_D1 = 200;
        }
        else if(Console_ProgramMode(Data_Get,0) == TM_Marine) 
        {
            by_D1 = 300;
        }        
        if(by_Data == C_KM)
        {
            by_D1 = (u16)((float)by_D1 * 1.609);
            by_Bu = (u16)((float)Console_Get_Distance() * 1.609);
        }
        else
            by_Bu = Console_Get_Distance();
        by_Bu = by_D1 - (by_Bu / 10);
    }
    else
    {//==> Other
        by_x=0;
        if(by_Data == C_KM)
        {
            by_Dat = (u16)((float)by_Dat * 1.609);
        }
        if((by_Dat / 100) < 100)
        {
            by_x = 1;
            by_Bu = by_Dat / 10;
        }    
        else 
        {
            by_Bu = by_Dat / 100;
        } 
        
        if((Console_CsafGoalEND() == 0) && 
           (CSAFE_GetInformation(_Get_TargetDistance) == 1))
        {//==>当C-SAFE有设定目标距离且实际距离未达目标值时
            by_Dat = CSAFE_GetInformation(_Get_DFdistnce);//10;
            if(Console_SpeedUnit(Data_Get,0) == C_KM)
            {// KPH
                if(by_Data == C_MILE)
                {
                    by_Dat = (u16)((float) by_Dat * 0.62137); 
                }
            }
            else
            {// MPH
                if(by_Data == C_KM)
                {
                    by_Dat = (u16)((float) by_Dat * 1.609);
                }  
            } 
            if(by_x == 1)
            {
                if(by_Dat > by_Bu) 
                {
                    by_Bu = by_Dat-by_Bu;
                    if(by_Bu >= 1000)
                    {
                        by_Bu /= 10;
                        by_x=0;
                    }
                }
            }  
            else
            {
                if(by_Dat > by_Bu)
                {
                    by_Bu = by_Dat - by_Bu;
                }
            }    
        }
    } 
//    if(by_Data == C_KM)
//        Led_Set_Target(Target_DistanceKM,1);
//    else
    Led_Set_Target(Target_DistanceMiles,1);
    if(by_x == 1)
    {
        if(_WindowsClear == 0)
        {
            _WindowsClear = 1;
            Led_Window(Window_2,5,0);
        }
        Led_Window(Window_2,3,by_Bu);
    }
    else
    {
        if(_WindowsClear == 1)
        {
            _WindowsClear = 0;
            Led_Window(Window_2,5,0);
        }
        Led_Window(Window_2,4,by_Bu);
    }
}
/*******************************************************************************
* Function Name  : Show_Calories
* Description    : 显示卡路里
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Calories(void)
{
  Led_Set_Target(Target_Calories,1);
  if((Console_CsafGoalEND() == 0) && \
     (CSAFE_GetInformation(_Get_TargetCalories) == 1) && \
     (CSAFE_GetInformation(_Get_DFcalories) > Console_Get_Calories()))
  {//==>当C-SAFE有设定目标卡路里且实际卡路里未达目标值时
      Led_Window(Window_1,1,CSAFE_GetInformation(_Get_DFcalories) - Console_Get_Calories());
  }
  else
  {
      Led_Window(Window_1,1,Console_Get_Calories());
  }
}
void Show_CaloriesHour(void)
{
  Led_Set_Target(Target_CaloriesHour,1);
  Led_Window(Window_4,1,Console_Get_Calories_Hour());
}
/*******************************************************************************
* Function Name  : Show_Watts()
* Description    : 显示平均
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Watts(void)
{
  Led_Set_Target(Target_Watts,1);
  Led_Window(Window_4,1,(u16)((float)Console_Get_Calories_Hour() / 4.2));
}


/*******************************************************************************
* Function Name  : Show_WFIinInformation
* Description    : 显示 WFI 信息并持续到按紧急开关为止
* Input          : by_Dat ; 0=正常结束 : 1=非正常结束
* Output         : None
* Return         : None
*******************************************************************************/
void Show_WFIinInformation(u8 by_Dat)
{
    u8 by_Status = M_NONE;
    u8 by_Stage = 0;
    u8 by_Show = 1;
    u16 by_Buffer = 0;
    
    if(Console_Parameter(Info_FitTestACCTime) > 180)
    {
      by_Stage=(Console_Parameter(Info_FitTestACCTime) - 180)/60;
      if((Console_Parameter(Info_FitTestACCTime)-180)%60 != 0) by_Stage+=1; 
    }    
    Console_VO2();
    by_Buffer = Console_Parameter(Info_AccumulatedTime);
    if(by_Dat == 0)
    {
        by_Show=2;
    }
    Led_Set_Reset(); 
    while(by_Status == M_NONE)
    {
        switch(by_Show)
        {
            case 1:
                   Show_Message(Message_TestTerminated,0,0,Show_Auto);
                   break;
            case 2:
                   Show_Message(Message_GoalAchieved,0,0,Show_Auto);
                   break;       
            case 3: 
                   //Show_SetSummaryNumber(1);//==>显示信息值模式 xx:xx
                   Show_Message(Message_TotalTime_2,_Message_AddNumber_Time_DF,by_Buffer,Show_Auto);
                   break;
            case 4://==>VO2
                   by_Show++;
                   Led_String(" VO2 :    ",0,0);
                   Led_Window(Window_2,4,Console_Get_VO2());
                   Timer_Counter_Clear(1);
                   break;
        }
        if(by_Show <= 3 && Led_Get_String_End())
        {
            by_Show++;
            Led_Set_Reset(); 
            Show_Message(Message_Blank,0,0,Show_Blank);
        }
        if(by_Show > 3 && Timer_Counter(T_STEP,1,50))
        {
            by_Status=M_RESET;
        }
        
        if(!Check_SafeKey() || Key_Proce() == K_STOP) 
          by_Status=M_RESET;
    }
    return ;
}
/*******************************************************************************
* Function Name  : Show_GerkinInformation
* Description    : 显示 GERKIN 信息并持续到按紧急开关为止
* Input          : by_Dat ; 0=正常结束 : 1=非正常结束
* Output         : None
* Return         : None
*******************************************************************************/
void Show_GerkinInformation(u8 by_Dat)
{
    u8 by_Status=M_NONE;
    u8 by_Stage=0;
    u8 by_Level=0;
    u8 by_Show=1;
    u16 by_Buffer=0;
    
    if(Console_Parameter(Info_FitTestACCTime) > 180)
    {
        by_Stage=(Console_Parameter(Info_FitTestACCTime) - 180)/60;
        if((Console_Parameter(Info_FitTestACCTime)-180)%60 != 0) by_Stage+=1; 
    }    
    by_Level = Console_Gerkin();  
    by_Buffer = Console_Parameter(Info_AccumulatedTime);
    if(by_Dat == 0)
    {
        by_Show=2;
    }
    Timer_Counter_Clear(1);
    while(by_Status == M_NONE)
    {
        switch(by_Show)
        {
            case 1:Show_Message(Message_TestTerminated,0,0,Show_Auto);
                   break;
            case 2://Show_SetSummaryNumber(1);//==>显示信息值模式 xx:xx
                   Show_Message(Message_TotalTime_2,_Message_AddNumber_Time_DF,by_Buffer,Show_Auto);
                   break;
            case 3://Show_SetSummaryNumber(0);//==>显示信息值模式 xxxx
                   Show_Message(Message_StageNumber,_MsgAddNum_Fixed_4Int,by_Stage,Show_Auto);
                   break;
            case 4://Show_SetSummaryNumber(0);//==>显示信息值模式 xxxx
                   Show_Message(Message_CaloriesBurned,_MsgAddNum_Fixed_4Int,Console_Get_Calories(),Show_Auto);
                   break;
            case 5:// 状态等级
                   Show_Message(Message_VERY_POOR - by_Level,0,0,Show_RightToLeft);
                   break;
        }
        if(Led_Get_String_End())
        {
            by_Show++;
            if(by_Show > 5)
            {//==>总累计时间
                if(by_Dat == 1) by_Show=1;
                else by_Show=2;
            }    
        }
        if(Timer_Counter(T_STEP,1,600) || !Check_SafeKey() || Key_Proce()==K_STOP ) 
          by_Status=M_RESET;
    }
}
/*******************************************************************************
* Function Name  : Show_FITInformation
* Description    : 显示特定 Fitness Test 信息并持续到按紧急开关为止
* Input          : by_Dat; 0=正常结束 : 1=非正常结束
* Output         : None
* Return         : None
*******************************************************************************/
void Show_FITInformation(u8 by_Dat)
{
  u8 by_Status=M_NONE;
  u8 by_Show=1;
  u16 by_Score=0;
  u16 by_Buffer=0;
  
  if(Console_ProgramMode(Data_Get,0) == TM_PEB) 
    by_Score = Console_PEB();
  else if(Console_ProgramMode(Data_Get,0) == TM_Army) 
    by_Score = Console_Army();
  else if(Console_ProgramMode(Data_Get,0) == TM_Airforce) 
    by_Score = Console_AirForce();
  else if(Console_ProgramMode(Data_Get,0) == TM_Marine) 
    by_Score = Console_Marine();
  by_Buffer = Console_Parameter(Info_AccumulatedTime);
  Timer_Counter_Clear(0);
  Timer_Counter_Clear(1);
  while(by_Status == M_NONE)
  {
    if(by_Show == 1)
    {
      Show_Message(Message_TotalTime_2,_Message_AddNumber_Time_DF,by_Buffer,Show_Auto);
      if(Led_Get_String_End())
      {
        by_Show++;
        Led_Set_Reset(); 
      }        
    }
    else if(by_Show == 2)
    {//==>状态层级
      if(by_Dat == 0)
      {       
        if(Console_ProgramMode(Data_Get,0) == TM_Airforce)
          Show_Message(Message_Score,_MsgAddNum_Fixed_6Int1Dec,by_Score,Show_Auto);
        else
          Show_Message(Message_Score,_MsgAddNum_Fixed_4Int,by_Score,Show_RightToLeft);
        if(Led_Get_String_End())
        {
          by_Show++;
          Led_Set_Reset();
        } 
      }              
      
      else if(by_Dat == 1)
      {
        Show_Message(Message_TestTerminated,0,0,Show_Auto);
        if(Led_Get_String_End())
        {
          by_Show++;
          Led_Set_Reset(); 
        }
      }
    }
    else if(by_Show == 3) 
    {
      Show_Message(Message_CaloriesBurned,_MsgAddNum_Fixed_4Int,Console_Get_Calories(),Show_Auto);
      if(Led_Get_String_End())
      {
        by_Show = 1;
        Led_Set_Reset(); 
      } 
    }
    if(Timer_Counter(T_STEP,1,600) || \
      !Check_SafeKey() || \
        Key_Proce() == K_STOP) 
      by_Status=M_RESET;
  }
}
/*******************************************************************************
* Function Name  : Show_NavyInformation
* Description    : 显示Navy Fitness Test 信息并持续到按紧急开关为止
* Input          : by_Dat; 0=正常结束 : 1=非正常结束
* Output         : None
* Return         : None
*******************************************************************************/
void Show_NavyInformation(u8 by_Dat)//==>980810
{
    u8 by_Status=M_NONE;
    u8 by_Show=1;
    u8 by_Level=0;
    u8 by_PointsStatus=0;
    u16 by_Buffer=0;
    
    if(by_Dat == 0)
    {
        by_Level=Console_Navy();
        by_PointsStatus=(by_Level*5)+40;
    }  
    by_Buffer=Console_Parameter(Info_AccumulatedTime);
    Timer_Counter_Clear(1);
    while(by_Status == M_NONE)
    {
        //UsbFunction_USB(0x500);
        //USB_SaveWorkoutSummary();//==>USB储存运动后信息
        switch(by_Show)
        {
            case 1://Show_SetSummaryNumber(1);//==>显示信息值模式 xx:xx
                   Show_Message(Message_TotalTime_2,_Message_AddNumber_Time_DF,by_Buffer,Show_Auto);
                   break;
            case 2://Show_SetSummaryNumber(0);//==>显示信息值模式 xxxx
                   Show_Message(Message_Points,_Message_AddNumber,by_PointsStatus,Show_RightToLeft);
                   break;
            case 3:// 状态等级
                   if(by_Level == 0)
                       Show_Message(Message_TestTerminated,0,0,Show_Auto);
                   else
                       Show_Message(Message_SATISFACTORY_MARGINAL-(by_Level-1),0,0,Show_Auto);
                   /*
                   switch(by_Level)
                   {
                        case 0:// 78
                               Show_Message(Message_TestTerminated,0,0);
                               break;
                        case 1://60
                               Show_Message(Message_SATISFACTORY_MARGINAL,0,0);
                               break;
                        case 2:// 59
                               Show_Message(Message_SATISFACTORY_MEDIUM,0,0);
                               break;
                        case 3:// 58
                               Show_Message(Message_SATISFACTORY_HIGH,0,0);
                               break;
                        case 4:// 57
                               Show_Message(Message_GOOD_LOW,0,0);
                               break;
                        case 5:// 56
                               Show_Message(Message_GOOD_MEDIUM,0,0);
                               break;
                        case 6:// 55
                               Show_Message(Message_GOOD_HIGH,0,0);
                               break; 
                        case 7:// 54
                               Show_Message(Message_EXCELLENT_LOW,0,0);
                               break; 
                        case 8:// 53
                               Show_Message(Message_EXCELLENT_MEDIUM,0,0);
                               break; 
                        case 9:// 52
                               Show_Message(Message_EXCELLENT_HIGH,0,0);
                               break; 
                        case 10:// 51
                               Show_Message(Message_OUTSTANDING_LOW,0,0);
                               break; 
                        case 11:// 50
                               Show_Message(Message_OUTSTANDING_MEDIUM,0,0);
                               break; 
                        case 12:// 49
                               Show_Message(Message_OUTSTANDING_HIGH,0,0);
                               break; 
                    }
                   */
                   break;
        }
        if(Led_Get_String_End())
        {
            by_Show++;
            if(by_Show > 3)
            {//==>总累计时间
                by_Show=1;
            }    
        }
      
        if(Timer_Counter(T_STEP,1,600) || \
           !Check_SafeKey() || \
           Key_Proce() == K_STOP) 
          by_Status=M_RESET;
    }
}
/*******************************************************************************
* Function Name  : Show_AveragePace()
* Description    : 显示平均
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_AveragePace(void)
{
  Led_Set_Target(Target_AveragePace,1);
  Led_Window(Window_2,2,Console_Get_AvgPace());
}
void Show_Pace(u8 by_D)
{
  u16 by_Dat;
  u8 by_Mod;
  static u8 by_WindowsClear = 0;
 
  if(by_D == Target_Pace)
  {// Pace
      Led_Set_Target(Target_Pace,1);
      by_Dat = 36000 / (Console_Information(Info_WorkoutSpeed,Data_Get,0) / 10);//(Console_Speed(Data_Get,0)
  }
  else 
  {// Average Pace
      Led_Set_Target(Target_AveragePace,1);
      by_Dat = 36000 / Console_Get_AvgSpeed();
  }
  by_Mod = by_Dat % 60;
  by_Dat = ((by_Dat / 60)* 100)+ by_Mod;    
  if(by_Dat > 9959) 
  {
    if(by_WindowsClear == 0)
    {
      by_WindowsClear = 1;
      Led_Window(Window_2,5,0);
    }
    Led_Window(Window_2,1,by_Dat/100);
  }
  else
  {
    if(by_WindowsClear == 1)
    {
      by_WindowsClear = 0;
      Led_Window(Window_2,5,0);
    }
    Led_Window(Window_2,2,by_Dat);
  }
}
/*******************************************************************************
* Function Name  : Show_METs()
* Description    : 显示METs
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_METs(void)
{
  Led_Set_Target(Target_METs,1);
  Led_Window(Window_1,4,Console_Get_METs());
}

// 100.09.28
/*******************************************************************************
* Function Name  : Show_RF900Infor()
* Description    : RF900MHz显示信息用
* Input          : by_Com => Command / by_D => 台数
* Return         : None
*******************************************************************************/
void Show_RF900Infor(u8 by_Com,u8 by_D)
{
  u8 _Com;
  u8 _Line = 52;
  //uc8 *_Message;
  
  switch(by_Com)
  {
      case 0x42:// Mute
                _Com = Message_Mute;
                break;
      case 0x43:// Channel
                _Com = Message_Channel;
                break;
      case 0x44:// Volume
                _Com = Message_Volume;
                break;
  }
  /*
  _Message = Message_English[_Com];
  
  switch(Console_Language(Data_Get,0))
  {
      default:
             if(Language_GetString(_Lang_Language_Data_,_Com) == _Lang_Success_)
                _Message = Lang_StringBuffer;
            else 
                _Message = Message_English[_Com];
               break;
      case L_English:
               _Message = Message_English[_Com];
               break;
  }
  */
  if(_RF900MessageOld != by_Com)
  {
      _RF900MessageOld = by_Com;
      //Led_String((unsigned char *)_Message,0,0);
      Show_Message(_Com,0,0,Show_Auto);
  }
  if(by_Com != 0x42)
  {
      Led_NumberSite(_Line,by_D / 10);
      Led_NumberSite(_Line + 6,by_D % 10);
  }
}

void Show_RF900MessageOld(void)
{
  _RF900MessageOld = 0;
}
//====================================
// String   : 字符串
// ShowTime : 单字符串显示Hold time
//====================================
void Show_StringOneTime(unsigned char *String, unsigned char ShowTime)
{
    Show_Message(Message_Blank,0,0,Show_Blank);
    Led_Set_Reset();
    while(1)
    {
        Led_String(String,0,Show_Auto);
        if(Led_Get_String_End() == 1)
        {
            Timer_Delay(ShowTime); // 1 = 0.1s
            break; 
        }
    }
}
//
/*
// 秀数字通讯次数累计
void Show_CommunicationTotal(void)
{
  Led_Set_Target(Display_OFF,0);
  Led_CommunicationTestNumber(0,EEPROM_ReadInformation(EE_CommunicationTX));
  Led_CommunicationTestNumber(1,EEPROM_ReadInformation(EE_CommunicationRX));
  while(1)
  {
      if(!Check_SafeKey()) break;
  }
}
*/

