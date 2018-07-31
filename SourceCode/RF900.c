#include  "General.h"
#include "keyboard.h"
#include "RF900.h"
#include "C_safe.h"
#include "led.h"
#include "Show.h"
#include "Console.h"
#include "EEPRom.h"
#define CommandMax 5

char _MaxCh = 1;
char _AudioMute = 0;
char _AudioChannel = 1;
char _AudioVolume = 1;
char _ChannelOld = 1;
char _Channel = 0;
char _RF900ShowMessage = 0;
char _MessageHoldTime = 0;
char _RF900DelayTime = 0;
char _E7CommandDataList[7] = {0};
     // Byte[0] => FM Band
     // Byte[1] => Wireless Range Sensitity
     // Byte[2] => Transmitter Manufacturer
     // Byte[3] => Max Channel
     // Byte[4] => Default Volume
     // Byte[5] => Software Revision
     // Byte[6] => Handware Configuration
char _CommandBuffer[CommandMax][2];
char _ComStart = 0;
char _ComEnd = 0;

union
{
    struct
    {
        unsigned char Send : 1;
        unsigned char Recieve : 1;
        unsigned char DelayLeave : 1;
        unsigned char DelayCount : 5;
    }bit;
    unsigned char all;
}RF900_E7Command = {0};

void RF900_SetCommand(char by_Com,char by_Dat);
//===============================================================
// RF900MHZ控制击键
// Input  : key number
// Return : 信息显示代码. 
//          0 => Not yet
//          1 ~ x => display number
//===============================================================
void RF900_Key(unsigned int by_Dat)
{
  char _Buffer = by_Dat;
  
  /*
  if((_Channel / 10 == 0)&&(_Buffer >= K_TV_0 && _Buffer <= K_TV_9))
  {// 使用数字键最多设为2位数 1 ~ 99
      _Buffer -= 36;
      if(_Channel == 0) 
          _Channel = _Buffer;
      else
      {
          _Channel *= 10;
          _Channel += _Buffer;
      }
      _Buffer = 0x43;
      _RF900ShowMessage = 0x43;
      _MessageHoldTime = 1;
  }
  else 
    */
    _Buffer = 0;
  switch(by_Dat)
  {
       /*
      case K_TV_ENTER:
               if(_Channel != 0 && _Channel <= _MaxCh)
               {
                   _ChannelOld = _AudioChannel;
                   _AudioChannel = _Channel;
                   RF900_SetCommand(0x43,_Channel);
                   _Buffer = 0x43;
                   _RF900ShowMessage = 0x43;
                   _MessageHoldTime = 1;
               }
      case K_TV_SUB:
               _Channel = 0;
               break;  
               */
      case K_TV_CU | K_MASK:          
      case K_TV_CU: 
               _ChannelOld = _AudioChannel;
               _AudioChannel++;
               if(_AudioChannel > _MaxCh) _AudioChannel = _MaxCh;
                 
      case K_TV_CD | K_MASK:         
      case K_TV_CD:   
               if(by_Dat == K_TV_CD || by_Dat == (K_TV_CD | K_MASK)) 
               {
                   _ChannelOld = _AudioChannel; 
                   if(_AudioChannel > 1) _AudioChannel--;
               }
               RF900_SetCommand(0x43,_AudioChannel);
               _Buffer = 0x43;
               _RF900ShowMessage = 0x43;
               _MessageHoldTime = 1;
               _Channel = 0;
               break;
      case K_TV_VU | K_MASK:        
      case K_TV_VU:   
               if(Console_GetUserVolume() == 0)
               {
                   RF900_SetCommand(0x44,Console_GetDefaultVolume());
               }
               Console_AddUserVolume();
               //
      case K_TV_VD | K_MASK:        
      case K_TV_VD: 
               if(by_Dat == K_TV_VD || by_Dat == (K_TV_VD | K_MASK))
               {
                   Console_DecUserVolume();
               }
               if(_AudioMute == 1)
               {
                   RF900_SetCommand(0x44,Console_GetDefaultVolume());
               }
               _AudioVolume = Console_GetUserVolume();
               //
               _AudioMute = 0;
               _Buffer = 0x44;
               _RF900ShowMessage = 0x44;
               _MessageHoldTime = 1;
               _Channel = 0;
               if(Console_GetUserVolume() != 0)
                   TIM4->CCR1 = Console_GetVolumeNumber(Console_GetUserVolume());//CCR1_Val_Open;//==> PWM duty调整使用(PB.6)
               else
               {
                   RF900_SetCommand(0x44,1);
                   TIM4->CCR1 = CCR1_Val_Close; 
               }
               //
               break;
               
      case K_Mute:
               if(_AudioMute == 0)
               {
                   _AudioMute = 1;
                   RF900_SetCommand(0x44,1);
                   TIM4->CCR1 = CCR1_Val_Close;
               }
               else
               {
                   _AudioMute = 0;
                   if(Console_GetUserVolume() != 0)
                   {
                       RF900_SetCommand(0x44,Console_GetDefaultVolume());
                       TIM4->CCR1 = Console_GetVolumeNumber(Console_GetUserVolume());//==> PWM duty调整使用(PB.6)
                   }
               }
               _Buffer = 0x42;
               _RF900ShowMessage = 0x42;
               _MessageHoldTime = 1;
               _Channel = 0;
               break;
               
      case K_TV_LAST:
               RF900_SetCommand(0x43,_ChannelOld);
               _Buffer = _ChannelOld;
               _ChannelOld = _AudioChannel;
               _AudioChannel = _Buffer;
               _Buffer = 0x43;
               _RF900ShowMessage = 0x43;
               _MessageHoldTime = 1;
               _Channel = 0;
               break;
      case K_NONE:
               break;
      default: 
               //if(by_Dat >= K_TV_0 && by_Dat <= K_TV_9) ;
               //else
                   RF900_ClearMessageNumber();
               break;
  }
}

//===============================================================
// Name   : RF900_ShowMessage()
// Note   : 显示提示字符串状态
// Input  : Not
// Return : 0 => 无显示 / 1 => 显示中 
//===============================================================
char RF900_ShowMessage(void)
{
  u8 by_Out = 0;
  
  
  switch(_RF900ShowMessage)
  {
      case 0x43:// Channel
                if(_Channel == 0)
                    Show_RF900Infor(0x43,_AudioChannel);
                else
                    Show_RF900Infor(0x43,_Channel);
                by_Out = 1;
                break;
      case 0x42:// Mute    
                if(_AudioMute != 0)
                    Show_RF900Infor(0x42,0);
                else
                {
      case 0x44:// Volume
                    Show_RF900Infor(0x44,_AudioVolume);
                }
                by_Out = 1;
                break;
  }
      
  if(by_Out == 1)
  {
      if(_MessageHoldTime >= 50)
      {// 显示 5 秒后结束显示
          RF900_ClearMessageNumber();
          by_Out = 0;
      }
  }
  
  return by_Out;
}
//===============================================================
// Name   : RF900_MessageHoldTime()
// Note   : 各时间delay计数(每100ms做一次)
// Input  : Not
// Return : Not 
//===============================================================
void RF900_MessageHoldTime(void)
{
  static unsigned char by_SendCommandDelay = 0;
  static unsigned char by_NotReturn = 0;  
  static unsigned char RF900_DelayContol = 0;
  static unsigned char SendE7CommandCounter = 0;
  unsigned char by_Su = 0;
  
  if(RF900_DelayContol < 55)
  {// Delay for power-on
      RF900_DelayContol++;
      return;
  }
  if(_MessageHoldTime != 0 && _MessageHoldTime < 50)
  {
      _MessageHoldTime++;
  }
  if(_RF900DelayTime < 50)
  {
      _RF900DelayTime++;
  }
  
  by_SendCommandDelay++;
  if(by_SendCommandDelay >= 3)
  {// 每300ms
      by_SendCommandDelay = 0;
      if(RF900_E7Command.bit.Send == 1)
      {
          if(CSAFE_900M_CommandStatus() == 1)
          {// 有回复
              CSAFE_900M_CommandStatusClear();// 清除回复状态旗标
              SendE7CommandCounter = 0;
              RF900_E7Command.all = 0;
              //RF900_E7Command.bit.DelayLeave = 1;
              //RF900_E7Command.bit.DelayCount = 0;
              for(by_Su = 0;by_Su < 7;by_Su++)
              {
                  _E7CommandDataList[by_Su] = CSAFE_900M_GetControlBuffer(by_Su);
                  if(by_Su == 4) ;
                  else if(by_Su == 3) _MaxCh = CSAFE_900M_GetControlBuffer(by_Su);
              }
              // Set default channel
              if(EEPROM_ReadInformation(EE_CHANNEL) <= _MaxCh)
              {
                 _ChannelOld = _AudioChannel;
                 _AudioChannel = EEPROM_ReadInformation(EE_CHANNEL);
                 RF900_SetCommand(0x43,_AudioChannel);
              } 
          }
          //else if(RF900_E7Command.bit.DelayLeave == 1)
          //{
          //    if(++RF900_E7Command.bit.DelayCount >= 3)
          //        RF900_E7Command.all = 0;
          //}
          else
          {
              SendE7CommandCounter++;
              if(SendE7CommandCounter < 10)
              {
                  CSAFE_900M_E7command();// 下E7 cmmand
              }
              else
              {
                  SendE7CommandCounter = 0;
                  RF900_E7Command.all = 0;
              }
          }
      }
      else
      {
          if(CSAFE_900M_CommandStatus() == 1)
          {// 有回复
              by_NotReturn = 0;  
              _ComEnd++;// 指向下一笔
              if(_ComEnd >= CommandMax) _ComEnd = 0;
              if(_ComStart != _ComEnd)
              {// 还有command
                  CSAFE_900M_Cardio(_CommandBuffer[_ComEnd][0],_CommandBuffer[_ComEnd][1]); 
              }
              CSAFE_900M_CommandStatusClear();// 清除回复状态旗标
          }
          else
          {// 没有回复
              if(_ComStart != _ComEnd)
              {
                  by_NotReturn++;
                  if(by_NotReturn <= 3)
                  {// 送command
                      CSAFE_900M_Cardio(_CommandBuffer[_ComEnd][0],_CommandBuffer[_ComEnd][1]);                      
                  }
                  else
                  {// 送3次没反应就bypass command
                      _ComEnd++;
                      if(_ComEnd >= CommandMax) _ComEnd = 0;
                  }
              }
              else by_NotReturn = 0;
          }
      }
  }
  
}
//===============================================================
// Name   : RF900_ClearMessageNumber()
// Note   : 清出按键信息
// Input  : Not
// Return : Not 
//===============================================================
void RF900_ClearMessageNumber(void)
{
  _Channel = 0;
  _MessageHoldTime = 0;
  _RF900ShowMessage = 0;
  Show_RF900MessageOld();
}
//===============================================================
// Name   : RF900_DefaultChannel()
// Note   : 设定预设 Channel
// Input  : Not
// Return : Not 
//===============================================================
void RF900_DefaultChannel(char by_D1)
{
  _AudioChannel = by_D1;
}
//===============================================================
// Name   : RF900_Mute()
// Note   : 取状态
// Input  : Not
// Return : _AudioMute 
//===============================================================
char RF900_Mute(void)
{
  return _AudioMute;
}
void RF900_MuteClear(void)
{
  _AudioMute = 0;
  RF900_SetCommand(0x44,Console_GetDefaultVolume());
}
//===============================================================
// Name   : RF900_MaxChannel()
// Note   : 总台数取得
// Input  : Not
// Return : Not
//===============================================================
char RF900_MaxChannel(void)
{
  return _MaxCh;
}
//===============================================================
// Name   : RF900_Initial()
// Note   : 系统重置取各默认参数
//          如果系统都一直没有取到默认数据就采用仪表的默认值.
//          Max Channel = 1 
//              Channel = 1
//               Volume = 1
// Input  : Not
// Return : Not
//===============================================================
void RF900_Initial(void)
{
  _AudioMute = 0;
  RF900_E7Command.all = 0;
  RF900_E7Command.bit.Send = 1;
  CSAFE_900M_CommandStatusClear();// 清除回复状态旗标
  CSAFE_900M_E7command();// 下E7 cmmand
  _ComStart = 0;
  _ComEnd = 0;
  CSAFE_SetCheckAnyKeyStatus(1);// 关闭任意键判断功能
  _AudioVolume = Console_GetUserVolume();
  
}

void RF900_SetCommand(char by_Com,char by_Dat)
{
  if(_ComStart < CommandMax)
  {
      _CommandBuffer[_ComStart][0] = by_Com;
      _CommandBuffer[_ComStart][1] = by_Dat;
      _ComStart++;
      if(_ComStart >= CommandMax) _ComStart = 0;
  }
}
