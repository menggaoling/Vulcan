#include "General.h"
#include "keyboard.h"
#include "Console.h"
#include "EEPRom.h"
#include "C_safe.h"
#include "TV.h"
//#include "Voice.h"
//#include "cab.h"
#include "RF900.h" 
//#include "UsbFunction.h"
#include  "KeyFunction.h"
#include  "KeyFunctionInfor.h"
#include  "i2c_key.h" 

#define MAX_HOLD_TIME     3000
#define TURE_TIME         1
#define HOLD_TIME         45//15
#define REPLAY_TIME_FAST  35//17//10
#define REPLAY_TIME_QUICK 8//17
#define HOLD_3secTIME     428//153
#define HOLD_5secTIME     714//256

struct {
         unsigned Flag      : 1;
         unsigned SetNumber : 1;
         unsigned Enter     : 1;   // ==> Enter键
         unsigned Back      : 1;   // ==> Back键
         unsigned Key       : 1;   // ==> 判断是否有按按键 
       } KeyBit;

union {
      u16 WORD;
      u8 BYTE[2];
      }Key_Dat;

u8 Key_Data[8]={0};//==>搭配所使用 8x8 的扫描方式所定义
u8 by_AccMode=0;
u16 by_KeyHoldTime = 0;
u16 by_KeypadTestTime = 0;

JHTKeyFunctionLib KeyInformation;// 按键结构宣告

void Keyboard_Initial(void)
{
  by_AccMode=0;
  _Key_Data = NULL;// 传址对应使用子程序
  KeyScan_Initial( &KeyInformation, &_KeyTable[0], &_SpecialKey[0][0], 
                   &_HoldKeyTime[0], &_HoldKey[0][0]);
  KeyInformation.KeyDataNumber = 8;// Data pin total 8 pin
  KeyInformation.SpecialKeyGroup = 2;// 设为2组组合键
  KeyInformation.HoldKeyTimeGroup = 4;// 设为4组
  KeyInformation.HoldKeyColseTime = 8000;// 持续按住 8`s 后将不在承认按键
  KeyScan_SetKeyIntSwitch(_KeyIntOpen);// 允许执行中断内子程序

  i2cInit();
}

//=============================================================================
// Key value read模块用户需自行处理的硬件分位
//=============================================================================
  
u8 Key_Value_Read( u8 * Buffer)
{
  u8 key_count = 0;
  
  if(!INT_STATUS())//Key int pin low active
  {
    i2cWriteBytes(ADDR,0x10,0,NULL);
    i2cReadBytes(ADDR,0,1,&key_count);
    if( key_count > 4)
      key_count = 4;
    i2cWriteBytes(ADDR,0x20,0,NULL);
    i2cReadBytes(ADDR,0,key_count,Buffer);   
  }
  return key_count;
}
/*******************************************************************************
* Function Name  : Key_Counter_Int
* Description    : 按键读取中断,每7m`s做ㄧ次
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Key_Counter_Int(void)
{
  KeyBit.Flag=1;
}
/*******************************************************************************
* Function Name  : Key_Decode
* Description    : 所按按键成立与加速条件成立与否判断
* Input          : 0-不支援Password 1-支援Password 
* Output         : None
* Return         : by_Out ; 对应按键代码
*******************************************************************************/
u16 Key_Proce(void)
{
  u16 by_Out = KeyScan_Proce();
  
  switch(Console_Audio(Data_Get,0))// 100.09.26
  {
      //case Audio_CAB:
      //         CAB_SendCommand(by_Out) ;
      //         CAB_CommandProcess();
      //         break ;
      case Audio_TV:
               TV_Key(by_Out);// TV控制
               break;
      case Audio_RF900:
               if(Console_WorkoutStatus(Data_Get,0) == 1)
               {// 运动模式
                   RF900_Key(by_Out);// RF900MHZ控制
               }
               break;
  }
  return by_Out;
}

/*******************************************************************************
* Function Name  : Console_NumberCount
* Description    : 数字键数字计数用
* Input          : 数字位数(max.=5)/ 按键代码
* Output         : None
* Return         : None
*******************************************************************************/
u16 _NumberSum;
u16 Keyboard_NumberCount(u8 by_Range,u16 by_KeyInput)
{
  u8 by_D;
  static u8 Number_Seg = 0;

  if(by_KeyInput >= K_0 && by_KeyInput <= K_9)
  {
      if(Number_Seg < by_Range)
      {
          Number_Seg++;
          by_D = by_KeyInput - K_0;
          if(_NumberSum != 0) _NumberSum *= 10;
          _NumberSum += by_D;
          if(_NumberSum > 9999) _NumberSum = 9999;
      }
  }
  else if(by_KeyInput != K_NONE)
  {
      _NumberSum = 0;
      Number_Seg = 0;
  }
  return _NumberSum;
}
//===============================================================================================================================
//==> 参数计算使用
//==> by_D  : 0-Add or 1-Dec 
//==> by_D1 : Min
//==> by_D2 : Max
//==> by_D3 : 调整累计值
//==> by_D4 : 目前值
//=========================================================================================
u32 Keyboard_DFCount(u8 by_D,u16 by_D1,u32 by_D2,u8 by_D3,u32 by_D4)
{
  if(by_D == 0)
  {
      by_D4 += by_D3;
      if(by_D4 > by_D2) by_D4 = by_D1;
  }
  else
  {
      if(by_D4 >= (by_D1 + by_D3) )
          by_D4 -= by_D3;
      else
          by_D4 = by_D2;
  }
  return by_D4;
}
