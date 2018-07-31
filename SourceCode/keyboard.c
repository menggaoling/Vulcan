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
         unsigned Enter     : 1;   // ==> Enter��
         unsigned Back      : 1;   // ==> Back��
         unsigned Key       : 1;   // ==> �ж��Ƿ��а����� 
       } KeyBit;

union {
      u16 WORD;
      u8 BYTE[2];
      }Key_Dat;

u8 Key_Data[8]={0};//==>������ʹ�� 8x8 ��ɨ�跽ʽ������
u8 by_AccMode=0;
u16 by_KeyHoldTime = 0;
u16 by_KeypadTestTime = 0;

JHTKeyFunctionLib KeyInformation;// �����ṹ����

void Keyboard_Initial(void)
{
  by_AccMode=0;
  _Key_Data = NULL;// ��ַ��Ӧʹ���ӳ���
  KeyScan_Initial( &KeyInformation, &_KeyTable[0], &_SpecialKey[0][0], 
                   &_HoldKeyTime[0], &_HoldKey[0][0]);
  KeyInformation.KeyDataNumber = 8;// Data pin total 8 pin
  KeyInformation.SpecialKeyGroup = 2;// ��Ϊ2����ϼ�
  KeyInformation.HoldKeyTimeGroup = 4;// ��Ϊ4��
  KeyInformation.HoldKeyColseTime = 8000;// ������ס 8`s �󽫲��ڳ��ϰ���
  KeyScan_SetKeyIntSwitch(_KeyIntOpen);// ����ִ���ж����ӳ���

  i2cInit();
}

//=============================================================================
// Key value readģ���û������д����Ӳ����λ
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
* Description    : ������ȡ�ж�,ÿ7m`s�����
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
* Description    : �����������������������������ж�
* Input          : 0-��֧ԮPassword 1-֧ԮPassword 
* Output         : None
* Return         : by_Out ; ��Ӧ��������
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
               TV_Key(by_Out);// TV����
               break;
      case Audio_RF900:
               if(Console_WorkoutStatus(Data_Get,0) == 1)
               {// �˶�ģʽ
                   RF900_Key(by_Out);// RF900MHZ����
               }
               break;
  }
  return by_Out;
}

/*******************************************************************************
* Function Name  : Console_NumberCount
* Description    : ���ּ����ּ�����
* Input          : ����λ��(max.=5)/ ��������
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
//==> ��������ʹ��
//==> by_D  : 0-Add or 1-Dec 
//==> by_D1 : Min
//==> by_D2 : Max
//==> by_D3 : �����ۼ�ֵ
//==> by_D4 : Ŀǰֵ
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
