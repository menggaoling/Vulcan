#include <stdio.h>
#include "EngineerNumber.h"
#include "EngineerMode.h"
#include "Console.h"
#include "RF900.h"
#include "led.h"
#include "UsbFunction.h"
#include "EEPRom_Language.h"
#include "keyboard.h"
#include "Digital.h"
#include "Show.h"
#include "timer.h"
#include "HT1381.h"
#include "Beep.h"
#include "EEPRom.h"
#include "C_safe.h"
#include "heart.h"
#include "WiFi_Information.h"
#include "wifi_Digital.h"
#include "wifi_interface.h" 
//#include "Voice.h"
#include "Show_Functions.h"
#include  "LCB_Support.h"
#include "Boot.h"

struct
{
    unsigned key       : 1;
    unsigned key1      : 1;
    unsigned key2      : 1;
    unsigned Enter     : 1;
    unsigned UseNumKey : 1;
    unsigned NumKeyStart : 1;
    unsigned Mode      : 1;
    unsigned Special   : 1;
} Engineer;

long unsigned int _Sum;
extern unsigned char _Display_Logo;
void EngineerMode_ManualUpdate(void);
void EngineerMode_SoftwareBackup(void);
/*******************************************************************************
* Function Name  : Main_SpecialMode()
* Description    : 工程模式
* Input          : 按键代码 
* Output         : None
* Return         : status
*******************************************************************************/
char EngineerMode_SpecialMode(unsigned short by_KeyInput)
{
  char by_Status = M_NONE;
  char by_D,_Run=0;
  
  if(by_KeyInput >= K_0 && by_KeyInput <= K_9)
  {
      by_D = by_KeyInput - K_0;
      if(_Sum != 0) _Sum *= 10;
      _Sum += by_D;
      if(_Sum > 999999999) _Sum = 999999999;
  }
  else if(by_KeyInput == K_ENTER)
  {       
      if(Engineer.Special == 0)
      {
          if(_Sum != 0)
              _Sum = 0;
          else
              Engineer.Special=1;
      }
      else
      {
          if(_Sum != 0)
          {
              switch(_Sum)
              {
                  case 1001:// Manage Mode
                            Engineer.Mode = 1;
                            _Run=1;
                            break;
                  case 2001:// Engineer Mode
                            _Run=2;
                            break;
                  case 3001:// Service Mode
                            _Run=3;
                            break;
                  case 5001:// Test Mode
                            EngineerMode_Test();
                            by_Status = M_RESET;
                            break;
                  case 990001:
                            // 将其wifi would设为AP模式 (JIS是Overlay有按键但1x没有所以设在这里 )
                            RestoreWifiToFactoryDefaults();
                            Timer_Counter_Clear(1);
                            while(Timer_Counter(T_STEP,1,30) == 0)
                            {
                                Led_String("Activated",0,Show_Auto);
                            }
                            by_Status = M_RESET;
                            break;
              }
              _Sum = 0;
              Engineer.Special=0;
              if(_Run != 0)
              {
                  // USB communication
                  IO_USB_Communication();
                  UsbFunction_USB(0);// HW reset
                  //
                  if(_Run == 1)
                      EngineerMode_Manager();
                  else if(_Run == 2)
                      EngineerMode_Engineer();
                  else if(_Run == 3)
                      EngineerMode_Service();
                  by_Status = M_RESET;
              }
          }
      }
  }
  else if(by_KeyInput != K_NONE)
  {
      _Sum = 0;
      Engineer.Special=0;
  }
  return by_Status;
}
/*******************************************************************************
* Function Name  : Manager_Mode()
* Input          : None
* Output         : None
* Return         : by_Status
*******************************************************************************/
char EngineerMode_Manager(void)
{
  unsigned long by_Max = _Manage_Item;
  unsigned long by_Buffer = 0;
  unsigned short by_Min = 1;
  unsigned short _KeyBuffer;
  unsigned long _NumberKeyBuffer;
  unsigned char by_Status = M_NONE;
  unsigned char by_Eng = 1;
  unsigned char _NumberRange = 1;
  unsigned char _Layer = 1;// 选项阶层
  unsigned char _SegDigits = 1;
  unsigned char EngBu[4];
  unsigned char Temp[20] = {0};
  
  Engineer.key = 0;
  Engineer.Enter = 0;
  Engineer.UseNumKey = 0;
  Engineer.NumKeyStart=0;
  Timer_Close(Time_AllClose);//==>关闭所有Time计数器
  Digital_Command(CmdGetVersion,0);
  Timer_Counter_Clear(3);
  Led_Set_Target(Display_OFF,0);
  if((Digital_GetStatus() & 0x03) != 0)
  {// 升降有动作时就需要强制停止
      Digital_Command(CmdSetInclineAction,0);//==>升降停止    
  }
  SendWifiCommand(WIFI_GET_CONNECTED_STRENGTH,0,0);
  while(by_Status == M_NONE)
  {
      UsbFunction_USB(0x500);  
      //UsbFunction_SoftwareUpdate(1);//==>支持最新版软件更新模式
      
      switch(_Layer)
      {
          case 1:// 第1层大选项
                 Led_String((unsigned char *)ManageItemName[by_Eng-1].ItemName,0,Show_Auto);
                 break;
          case 2:// 第2层小选项
                 if(by_Eng == _Manage_Item_ErP)
                 {
                     if(by_Buffer == 0)
                         Led_String(" ErP  OFF ",0,0); 
                     else  
                     {
                       Led_String(" ErP      ",0,0); 
                       Led_Window(32,1,by_Buffer); 
                     }
                 }
                 else if((by_Eng == _Manage_Item_Reset)&&(Engineer.Enter == 1))
                 {
                     Led_String("Reset WiFi Module AP To rd02",0,Show_Auto);
                     if(Led_Get_String_End() == 1)
                     {
                         Engineer.Enter = 0;
                     }
                 }
//                 else if((by_Eng == _Manage_Item_ExprotUSB) || 
//                         (by_Eng == _Manage_Item_ImportUSB) )
//                 {
//                     Show_MessageWithNumber((unsigned char *)EngineerMode[by_Eng].ItemName,99+by_Eng,0,0,Show_Auto);
//                 }
                 else
                     Led_String((unsigned char *)EngineerMode[by_Eng].ItemName,0,Show_Auto);
                 break;
          case 3:// 第3层设定值or开关or特殊功能
                 switch(EngineerMode[by_Eng].NumberDisplayWay)
                 {
                     case _String_None:// 不显示数据
                              switch(by_Eng)
                              {
                                  case _Manage_Item_USBPort:
                                              Led_String("Protection",0,Show_Auto); 
                                              break;
                                  case _Manage_Item_Type:
                                              Led_String("2015 T1x",0,Show_Auto); 
                                              break;
                                              
                                  case _Manage_Item_DefaultVolume:
                                              Led_String((unsigned char *)VolumeItem[by_Buffer].ItemName,0,Show_Auto); 
                                              break;
                                  case _Manage_Item_HeadphoneJack:
                                              Led_String((unsigned char *)HJSItem[by_Buffer].ItemName,0,Show_Auto); 
                                              break;
                                  case _Manage_Item_Keypad:
                                              Led_String((unsigned char *)KeypadItem[by_Buffer].ItemName,0,Show_Auto); 
                                              break;
                
                                  case _Manage_Item_TreadSensor:
                                              Led_String((unsigned char *)TreadSensor[by_Buffer/30].ItemName,0,Show_Auto); 
                                              break; 
                                  case _Manage_Item_NAVY:
                                              //Show_MessageParagraph((unsigned char *)NAVYItem[by_Buffer].ItemName,99,0,0);
                                              Led_String((unsigned char *)NAVYItem[by_Buffer].ItemName,0,Show_Auto);
                                              break;
                                  case _Manage_Item_SoftwareVersion:
                                  //case _Manage_Item_softwareUpdate:
                                              Led_String((unsigned char *)SoftwareItem[by_Buffer].ItemName,0,Show_Auto);
                                              break;
                                  case _Manage_Item_softwareUpdate:
                                              if(by_Buffer == 0)
                                                  Led_String((unsigned char *)SoftwareItem[1].ItemName,0,Show_Auto);
                                              else
                                                  Led_String((unsigned char *)SoftwareItem[4].ItemName,0,Show_Auto);
                                              break;
                                  case _Manage_Item_AutoUpdate:
                                              Led_String((unsigned char *)AutoUpdateItem[by_Buffer].ItemName,0,Show_Auto);
                                              break;
                                  case _Manage_Item_InputSource:
                                              Led_String((unsigned char *)AudioSource[by_Buffer].ItemName,0,Show_Auto); 
                                              break;
                                              
                                  case _Manage_Item_MAC_ID:
                                              sprintf((char*)Temp,"MAC:%02X-%02X-%02X-%02X-%02X-%02X", \
                                                       wifi_settings.mac_address[0],wifi_settings.mac_address[1], \
                                                       wifi_settings.mac_address[2],wifi_settings.mac_address[3], \
                                                       wifi_settings.mac_address[4],wifi_settings.mac_address[5]); 
                                              Led_String((unsigned char *)Temp,0,Show_Auto);
                                              break;
                                  case _Manage_Item_IP:
                                              sprintf((char*)Temp,"IP:%3d.%3d.%3d.%3d", \
                                                       wifi_settings.ip_address[0],wifi_settings.ip_address[1], \
                                                       wifi_settings.ip_address[2],wifi_settings.ip_address[3]);
                                              Led_String((unsigned char *)Temp,0,Show_Auto);
                                              break;
                                              // start v2.1-12
                                  case _Manage_Item_Signal:
                                              sprintf((char*)Temp,"%d %%",wifi_settings.ConnectedStrength);
                                              Led_String((unsigned char *)Temp,0,Show_Auto);
                                              break;
                                              /*
                                  case _Manage_Item_LogoDefault:  
                                              if(by_Buffer == Logo_MATRIX)
                                                  Led_String("MATRIX",0,Show_Auto);
                                              else
                                                  Show_MessageWithNumber(Pattern_PatternName,0,0,0,Show_Auto); 
                                              LED_MATRIX(_BigMatrix_Action);// 大Logo字符串跑马灯
                                              break;
                                    */          
                              }

                              break;
                     case _String_Number:// 显示数据
                              if(by_Eng == _Manage_Item_MaxTime || 
                                 by_Eng == _Manage_Item_Time )
                                  Led_7SEG(2,by_Buffer*100); 
                              else if(by_Eng == _Manage_Item_PauseTime)
                              {
                                  if(by_Buffer == 30)
                                      Led_7SEG(2,by_Buffer);
                                  else
                                      Led_7SEG(2,by_Buffer/60*100);
                              }
                              else if(by_Eng == _Manage_Item_MaxSpeed || 
                                      by_Eng == _Manage_Item_StartSpeed ||
                                      by_Eng == _Manage_Item_MaxElevation )
                              {
                                Led_7SEG(5,by_Buffer*10); 
                              }
                              else  
                              {
                                Led_7SEG(1,by_Buffer);
                              }
                              break;
                     case _String_DisableEnable:// 显示Disable or Enable         
                     case _String_MaleFemale:// 显示性别
                     case _String_MileKm:// 显示单位
                     case _String_OffOn:// 显示开关
                     case _String_DownUp:// 显示计时方向
                              Led_String((unsigned char *)Words_Check[EngineerMode[by_Eng].NumberDisplayWay].Words[by_Buffer],0,Show_Auto);
                              break;
                 }
                 break;
          case 4: 
                 switch(by_Eng)
                 {
                     case _Manage_Item_USBPort:
                     case _Manage_Item_Keypad:
                                 Led_String((unsigned char *)Words_Check[5].Words[by_Buffer],0,Show_Auto);
                                 break;
                     case _Manage_Item_AutoUpdate:
                                 if(EngBu[2] == 2)
                                   Led_Window(21,2,by_Buffer*100); 
                                 else if(EngBu[2] == 1)
                                   Led_String((unsigned char *)Words_Check[5].Words[by_Buffer],0,Show_Auto);
                                 break;                                                     
                     case _Manage_Item_HeadphoneJack:
                                 if(EngBu[2] == 0)
                                     Led_String((unsigned char *)Words_Check[5].Words[by_Buffer],0,Show_Auto);
                                 else
                                     Led_Window(16,6,by_Buffer); 
                                 break;
                     case _Manage_Item_DefaultVolume:
                                 Led_7SEG(1,by_Buffer);
                                 break;
                     case _Manage_Item_SoftwareVersion:
                                 if(EngBu[2] == 0)
                                     Led_String((unsigned char *)Boot_Version,0,Show_Auto);// Bootloader
                                 else if(EngBu[2] == 1)
                                     Led_String(Console_Version,0,Show_Auto);// UCB
                                 break;
                                 
                 }

                 break; 
      }
      
    
      _KeyBuffer = Key_Proce();
      if(_KeyBuffer != K_NONE)
      {
          Timer_Counter_Clear(3);
      }
      if((Engineer.UseNumKey == 1)&&
        ((_KeyBuffer >= K_0)&&(_KeyBuffer <= K_9)) )
      {
          if(_SegDigits == 1)
          {
              Engineer.NumKeyStart=1;
              // 先将原先的植备份起来
              _NumberKeyBuffer = by_Buffer;
              //
              by_Buffer = _KeyBuffer - K_0;
          }
          else if(_SegDigits <= EngineerMode[by_Eng].NumericKey.Digits) 
          {
              by_Buffer *= 10;
              by_Buffer += (_KeyBuffer - K_0);
          }
          _SegDigits++;
      }
      Engineer.key = D_ADD;
      switch(_KeyBuffer)
      {
          case K_HoldIDSD:
                   if(by_Eng == _Manage_Item_TotalDistance ||
                      by_Eng == _Manage_Item_TotalTime )
                   {
                       EEPROM_WriteInformation(EngineerMode[by_Eng].EEPRomNum,0);
                       Led_Window(Window_3,6,EEPROM_ReadInformation(EngineerMode[by_Eng].EEPRomNum)); 
                   }
                   /*
                   else if(by_Eng == _Manage_Item_InternetSwitch)
                   {// 将其wifi would设为AP模式 (JIS是Overlay有按键但1x没有所以设在这里 )
                       RestoreWifiToFactoryDefaults();// Hold
                       Timer_Counter_Clear(1);
                       while(Timer_Counter(T_STEP,1,30) == 0)
                       {
                           Led_String("Activated",0,Show_Auto);
                       }
                   }
                   */  
                   break;  
          case K_STOP | K_MASK:  
                   by_Status=M_RESET;
                   break;
          case K_SpeedD | K_MASK:
          case K_SpeedD:             
          case K_InclineD | K_MASK:         
          case K_InclineD: 
          case K_TimeD | K_MASK:
          case K_TimeD:           
                   Engineer.key = D_DEC;
          case K_SpeedU | K_MASK:
          case K_SpeedU:          
          case K_TimeU | K_MASK:
          case K_TimeU:         
          case K_InclineU | K_MASK:
          case K_InclineU:  
                   Led_Set_Reset();
                   switch(_Layer)
                   {
                   case 1:      
                   case 2://Led_Set_Reset();
                     if(Engineer.UseNumKey == 1)// 使用数字键进行设定  
                     {
                       if(Engineer.NumKeyStart == 1)
                       {
                         Engineer.NumKeyStart=0;
                         by_Buffer=_NumberKeyBuffer;
                         _SegDigits = 1;
                       }
                       by_Buffer = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Buffer);
                       break;
                     }
                     by_Eng = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Eng);
                     if(by_Eng == _Manage_Item_TreadSensor && Digital_GetMachineType() <= LCB_TopTek)
                     {// 非支持的MCB时需要隐藏此选项
                       by_Eng = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Eng);
                     }
                     Show_Message(Message_Blank,0,0,Show_Blank);
                     break;
                   case 3:Led_Set_Reset();    
                   case 4:
                     if(Engineer.UseNumKey == 1)  
                     {// 使用数字键进行设定
                       if(Engineer.NumKeyStart == 1)
                       {
                         Engineer.NumKeyStart=0;
                         by_Buffer=_NumberKeyBuffer;
                         _SegDigits = 1;
                       }
                       by_Buffer = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Buffer);
                       
                       if(_Layer == 4 && by_Eng == _Manage_Item_DefaultVolume &&
                          EngBu[2] == 2 )
                       {// Output Default
                         TIM4->CCR1 = Console_GetVolumeNumber(by_Buffer);// PWM duty调整使用(PB.6)
                       }
                       break;
                     }
                     if(by_Eng == _Manage_Item_PauseTime)
                     {
                       if(Engineer.key == D_ADD)
                       {
                         if(by_Buffer < 60)
                           by_Buffer = 60;
                         else if(by_Buffer >= 300)
                           by_Buffer = 30;
                         else
                           by_Buffer += 60;
                       }
                       else
                       {
                         if(by_Buffer == 30)
                           by_Buffer = 300;
                         else if(by_Buffer == 60)
                           by_Buffer = 30;
                         else
                           by_Buffer -= 60;
                       }
                     }
                     else
                       by_Buffer = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Buffer);
                     if(by_Eng == _Manage_Item_LanguageDefault)
                     {
                       if(Language_LanguageChecking(by_Buffer) != _Lang_Success_)
                       {
                         by_Buffer = L_English;
                       }
                       if(by_Buffer == L_English)
                         Led_String("English",0,Show_Auto);
                       else
                         Led_String(Lang_StringBuffer,0,Show_Auto);
                     }
                     /*
                     else if(by_Eng == _Manage_Item_LogoDefault)
                     {
                     if(by_Buffer != 0)
                     {
                     while(1)
                     {
                     if(Pattern_CheckingPattern(by_Buffer) != _Lang_Success_)
                     {
                     by_Buffer++;
                     if(by_Buffer > Pattern_GetPatternAmount())
                     {
                     by_Buffer = 0;
                     break;
                   }
                   }
                     else break;
                   }
                   }
                   }
                     */
                     break;
                   }
                   break;
          case K_ENTER: 
                   Engineer.NumKeyStart=0;
                   switch(_Layer)
                   {
                       case 1:
                              _Layer = 2;
                              // 取出第2层选项数量
                              by_Max = ManageItemName[by_Eng-1].ItemRange;
                              // 暂存第1层地址
                              EngBu[0] = by_Eng;
                              // 取出第2层对应值
                              by_Eng = ManageItemName[by_Eng-1].ItemTarget;
                              // 起始值
                              by_Min = by_Eng;
                              // 调整范围值
                              by_Max += by_Min;
                              if(by_Eng == _Manage_Item_ErP)
                              {
                                  Engineer.UseNumKey = 1;
                                  _SegDigits = 1;
                                  by_Min = EngineerMode[by_Eng].Range[0];
                                  by_Max = EngineerMode[by_Eng].Range[1];
                                  _NumberRange = EngineerMode[by_Eng].AdjustSize;
                                  by_Buffer = EEPROM_ReadInformation(EngineerMode[by_Eng].EEPRomNum);
                              }
                              break;
                       case 2:
                              //================================================
                              // 不进下一阶段
                              //================================================
                              if(by_Eng == _Manage_Item_ErP)
                              {
                                  Led_7SEG(4,0);
                                  if((Engineer.UseNumKey == 1)&&
                                    ((by_Buffer < by_Min)||(by_Buffer > by_Max)))
                                  {// 使用数字键输入后判断其范围,如失败则还原数值
                                      by_Buffer = _NumberKeyBuffer;
                                      _SegDigits = 1;
                                      break;
                                  }
                                  else
                                  {
                                      Engineer.UseNumKey = 0;
                                      if(by_Eng == _Manage_Item_MaxTime)
                                      {
                                          if(by_Buffer < EEPROM_ReadInformation(EE_USER_TIME))
                                          {
                                              EEPROM_SaveWorkInformation(EE_USER_TIME,by_Buffer);
                                          }
                                      }
                                  }
                                  BEEP_Proce(4,3,3);
                                  EEPROM_SaveWorkInformation(EngineerMode[by_Eng].EEPRomNum,by_Buffer);
                                  _Layer = 1;
                                  by_Eng = EngBu[0];
                                  by_Min = 1;
                                  by_Max = _Manage_Item;
                                  Show_Message(Message_Blank,0,0,Show_Blank);
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_Date ||
                                      by_Eng == _Manage_Item_RTCTime )
                              {
                                  EngineerMode_RTCSysTime(by_Eng - _Manage_Item_Date);
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_SerialNumber)
                              {
                                  EngineerMode_SerialNumber();
                                  WiFi_Information_MachineTypeDefaultCheck();
                                  UpdateMachineInfo();
                                  break;
                              }
                              else if((by_Eng == _Manage_Item_DefaultVolume)&&
                                      (EEPROM_ReadInformation(EE_Audio) == Audio_OFF) )
                              {// 音源选项OFF就不进下一阶
                                  break;  
                              }
                              else if(by_Eng == _Manage_Item_ExprotUSB ||\
                                      by_Eng == _Manage_Item_ImportUSB )
                              {// Export setting to USB
                                  EngineerMode_ExportOrImport(by_Eng);
                                  Timer_Counter_Clear(3);
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_Reset)
                              {//  Internet Reset to rd02
                                  ConnectAPForFactoryDefault();
                                  Engineer.Enter = 1;
                                  break;
                              }
                              /*
                              else if(by_Eng == _Manage_Item_LogoImport)
                              {//  Inport Logo from USB
                                  Target_SoftwareUpdate(3); 
                                  break;
                              }
                              */
                              else if(by_Eng == _Manage_Item_LanguageErase)
                              {                                  
                                  Led_Set_Reset();
                                  while(!Led_Get_String_End())
                                  {
                                    Led_String("Language Erase",0,Show_Auto);
                                  }
                                  Language_Erase_AllData();
                                  Language_LanguageChecking(L_English);
                                  BEEP_Proce(4,3,3);
                                  EEPROM_SaveWorkInformation(EE_LANGUAGE,L_English);
                                  break;
                              }
                              /*
                              else if(by_Eng == _Manage_Item_LogoErase)
                              {
                                  Led_String("  Pattern Erase   ",0,Show_Auto);
                                  Timer_Counter_Clear(0);
                                  while(!Timer_Counter(T_STEP,0,20)) ;
                                  Pattern_Erase_AllData();
                                  Pattern_CheckingPattern(Logo_MATRIX);
                                  BEEP_Proce(4,3,3);
                                  EEPROM_SaveWorkInformation(EE_Logo,Logo_MATRIX);
                                  //LED_MATRIX(0,99);// 清大Logo灯
                                  break;
                              }
                              */
                              //================================================
                              // 进下一阶段设定 
                              //================================================
                              if(EngineerMode[by_Eng].NumericKey.Used == _UseNumKey)
                              {
                                  Engineer.UseNumKey = 1;
                                  _SegDigits = 1;
                              }                              
                              EngBu[1] = by_Eng;
                              _Layer = 3;
                              by_Min = EngineerMode[by_Eng].Range[0];
                              by_Max = EngineerMode[by_Eng].Range[1];
                              _NumberRange = EngineerMode[by_Eng].AdjustSize;
                              by_Buffer = 0;
                              
                              if(by_Eng == _Manage_Item_LanguageDefault)
                              {
                                  by_Max = Language_GetLanguageAmount();
                                  by_Buffer = EEPROM_ReadInformation(EE_LANGUAGE);
                                  if(Language_LanguageChecking(by_Buffer) != _Lang_Success_) 
                                  {
                                      by_Buffer = L_English;
                                  }
                                  if(by_Buffer == L_English)
                                      Led_String("English",0,Show_Auto);
                                  else
                                      Led_String(Lang_StringBuffer,0,Show_Auto);
                              }
                              //else if(by_Eng == _Manage_Item_SoftwareVersion ||
                              //        by_Eng == _Manage_Item_softwareUpdate )
                              //{
                              //    if(by_Eng == _Manage_Item_softwareUpdate) by_Buffer = EngineerMode[by_Eng].Range[0];;
                              //}
                              else if(by_Eng == _Manage_Item_TotalDistance)
                              {
                                  //Show_Message(Message_Blank,0,0,Show_Blank);
                                  by_Buffer = EEPROM_ReadInformation(EngineerMode[by_Eng].EEPRomNum);
                                  if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                  {
                                      by_Buffer=(u32)((float)by_Buffer * 1.609);
                                  }
                                  //Led_Window(Window_3,6,by_Buffer/1000); 
                                  Led_String("          ",0,0);
                                  Led_Window(20,6,by_Buffer/1000);
                              }
                              else if(by_Eng == _Manage_Item_TotalTime )
                              {
                                  //Show_Message(Message_Blank,0,0,Show_Blank);
                                  //Led_Window(Window_3,6,EEPROM_ReadInformation(EngineerMode[by_Eng].EEPRomNum)/3600);
                                  Led_String("          ",0,0);
                                  Led_Window(20,6,EEPROM_ReadInformation(EngineerMode[by_Eng].EEPRomNum)/3600);
                              }
//                              else if(by_Eng == _Manage_Item_AP )
//                              {//if set AP on/off display its status,although its not exact
//                                by_Buffer = WIFIIsAPMode();
//                              }
                              else 
                              {
                                  by_Buffer = EEPROM_ReadInformation(EngineerMode[by_Eng].EEPRomNum);
                                  if(by_Eng == _Manage_Item_Time)
                                  {
                                      by_Max = EEPROM_ReadInformation(EE_MAX_TIME);
                                  }
                                  /*
                                  else if(by_Eng == _Manage_Item_LogoDefault)
                                  {
                                       by_Max = EEPROM_ReadInformation(EE_Logo);//Pattern_GetPatternAmount();
                                       LED_MATRIX(_BigMatrix_Clear);// 清大Logo灯
                                       if(Pattern_CheckingPattern(by_Buffer) != _Lang_Success_)
                                       {
                                           by_Buffer = 0;
                                       }
                                  }
                                  */
                                  else if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                  {
                                      if(by_Eng == _Manage_Item_Weight)
                                      {
                                          by_Min = Param_UserWeight_Min_Kg;
                                          by_Max = Param_UserWeight_Max_Kg;
                                      }
                                      else if(by_Eng == _Manage_Item_Height)
                                      {
                                          by_Min = _Height_KM_L;
                                          by_Max = _Height_KM_H;
                                      } 
                                      else if(by_Eng == _Manage_Item_MaxSpeed)
                                      {
                                           by_Min = Param_MaxSpeed_KPH_Min;
                                           by_Max = Param_MaxSpeed_KPH_Max;
                                      }
                                      else if(by_Eng == _Manage_Item_StartSpeed)
                                      {
                                           by_Min = Param_StartSpeed_KPH_Min;
                                           by_Max = Param_StartSpeed_KPH_Max;
                                      }
                                  }
                              }
                              
                              Led_Set_Reset();
                              break;
                       case 3:// SAVE or Other Layer.......
                              //================================================
                              // 其他动作阶段
                              //================================================
                              if(by_Eng == _Manage_Item_TotalDistance ||
                                 by_Eng == _Manage_Item_TotalTime)
                              {
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_USBPort)
                              {
                                  _Layer = 4;
                                  EngBu[2] = by_Buffer;
                                  by_Min = 0;
                                  by_Max = 1;
                                  by_Buffer = EEPROM_ReadInformation(EE_USB_Protection);
                                  Show_Message(Message_Blank,0,0,Show_Blank);
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_Keypad)
                              {
                                  _Layer = 4;
                                  EngBu[2] = by_Buffer;
                                  by_Min = KeypadItem[by_Buffer].Range[0];
                                  by_Max = KeypadItem[by_Buffer].Range[1];
                                  by_Buffer = EEPROM_ReadInformation(KeypadItem[by_Buffer].EEPRomNum);
                                  Show_Message(Message_Blank,0,0,Show_Blank);
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_HeadphoneJack)
                              {
                                  _Layer = 4;
                                  EngBu[2] = by_Buffer;
                                  by_Min = HJSItem[by_Buffer].Range[0];
                                  by_Max = HJSItem[by_Buffer].Range[1];
                                  by_Buffer = EEPROM_ReadInformation(HJSItem[by_Buffer].EEPRomNum);
                                  if(EngBu[2] == 1)
                                  {// 数据设定
                                      Engineer.UseNumKey = 1;
                                      _SegDigits = 1;
                                  }
                                  Show_Message(Message_Blank,0,0,Show_Blank);
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_SoftwareVersion)
                              {
                                  _Layer = 4;
                                  EngBu[2] = by_Buffer;
                                  by_Min = 0;
                                  by_Max = 0;
                                  by_Buffer = 0;
                                  Show_Message(Message_Blank,0,0,Show_Blank);
                                  switch(EngBu[2])
                                  {
                                      case 2:// LCB
                                             Led_String("    --    ",0,0);
                                             Led_MCBtype(Digital_GetMachineType());
                                             Led_Window(38,1,Digtial_Get_MCBVersion()%256);
                                             break;
                                      case 3:// WiFi
                                             Led_String(" Ver.     ",0,Show_Auto);
                                             Led_Special(0,32,wifi_settings.wifi_version.INT_VAL);
                                             break;
                                      case 4:// Language
                                             Led_String(" Ver.     ",0,Show_Auto);
                                             Led_Window(Window_2,1,Language_Version());
                                             break;
                                  }
                                  by_Buffer = 0;
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_AutoUpdate)
                              {
                                  EngBu[2] = by_Buffer;
                                  if(EngBu[2] == 0)
                                  {
                                    EngineerMode_ManualUpdate();
                                  }
                                  else
                                  {
                                    _Layer = 4;                                  
                                    if(EngBu[2] == 2)
                                    {// 数据设定
                                      Engineer.UseNumKey = 1;
                                      _SegDigits = 1;
                                    }
                                    by_Min = AutoUpdateItem[by_Buffer].Range[0];
                                    by_Max = AutoUpdateItem[by_Buffer].Range[1];
                                    by_Buffer = EEPROM_ReadInformation(AutoUpdateItem[by_Buffer].EEPRomNum);
                                  }
                                  Show_Message(Message_Blank,0,0,Show_Blank);
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_DefaultVolume)
                              {
                                  if((EEPROM_ReadInformation(EE_Audio) != Audio_RF900)&&
                                     (by_Buffer != 2))
                                  {// 非RF900且非设定支持项目
                                      break;
                                  }                                     
                                  _Layer = 4;
                                  EngBu[2] = by_Buffer;
                                  // 起始值
                                  by_Min = VolumeItem[by_Buffer].Range[0];
                                  // 调整范围值
                                  if(by_Buffer == 2)
                                      by_Max = EEPROM_ReadInformation(VolumeItem[1].EEPRomNum);
                                  else 
                                      by_Max = VolumeItem[by_Buffer].Range[1];
                                  by_Buffer = EEPROM_ReadInformation(VolumeItem[by_Buffer].EEPRomNum);
                                  // 打开音源
                                  if(EngBu[2] == 2)
                                  {// Output Default
                                      if((EEPROM_ReadInformation(EE_Audio) == Audio_TV))
                                      {
                                          IO_TV_Audio();
                                      }
                                      else
                                      {
                                          RF900_SetCommand(0x43,EEPROM_ReadInformation(EE_CHANNEL));
                                          IO_Ext_Audio();// 音源
                                      }
                                      IO_OpenAudio();//==>SHUTDOWN
                                      TIM4->CCR1 = Console_GetVolumeNumber(by_Buffer);// PWM duty调整使用(PB.6)
                                      //RF900_MuteClear();// 强制取消静音
                                  }
                                  //
                                  Engineer.UseNumKey = 1;
                                  _SegDigits = 1;
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_softwareUpdate)
                              {
                                  if(by_Buffer == 0)
                                  {// UCB software update
                                      Led_Set_Target(Display_OFF,0);
                                      Target_SoftwareUpdate(1);
                                  }
                                  else
                                  {// Language update
                                      Target_SoftwareUpdate(2);
                                  }
                                  /*
                                  switch(by_Buffer)//EngBu[2])
                                  {
                                      case 1:// UCB software update 
                                             Led_Set_Target(Display_OFF,0);
                                             Target_SoftwareUpdate(1);
                                             break;
                                      case 2:// LCB software update
                                             Led_Set_Target(Display_OFF,0);
                                             //Update_Process(_ClearDataFlash_); 
                                             break;
                                      case 3:// WiFi update
                                             break;
                                      case 4:// Language update
                                             Target_SoftwareUpdate(2);
                                             break;
                                  }
                                  */
                                  break;
                              }
                              // start v2.1-12
                              else if(by_Eng == _Manage_Item_AP)
                              {// 将其wifi module设为AP模式
                                  if(by_Buffer == _ON)
                                  {
                                      RestoreWifiToFactoryDefaults();
                                      Timer_Counter_Clear(1);
                                      while(Timer_Counter(T_STEP,1,30) == 0)
                                      {
                                          Led_String("Activated",0,Show_Auto);
                                      }
                                  }
                                  else
                                  {
                                    if(EEPROM_ReadInformation(EE_Wireless) == D_Enable)
                                    {
//                                      DisconnectWIFI();//before connect assuance disconnect status
//                                      for(unsigned int i = 0;i < 110000;i++);  // Delay  
                                      EEPROM_ReadInformation(EE_WiFi_SSID);
                                      EEPROM_ReadInformation(EE_WiFi_Password);
                                      EEPROM_ReadInformation(EE_WiFi_Security);
                                      ConnectWiFi_AP();
                                    }
                                      
                                  }
                                  EEPROM_WriteInformation(EngineerMode[by_Eng].EEPRomNum,by_Buffer);
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_TimeZone)
                              {//save Timezone to last timezone before modify it
                                EEPROM_SaveWorkInformation(EE_TZone,EEPROM_ReadInformation(EE_TZone));
                              }
                              // end v2.1-12
//                              else if(by_Eng == _Manage_Item_InternetSwitch)
//                              {
//                                if(by_Buffer == D_Enable)
//                                {
//                                  DisconnectWIFI();//before connect assuance disconnect status
//                                  for(unsigned int i = 0;i < 110000;i++);  // Delay  
//                                  EEPROM_ReadInformation(EE_WiFi_SSID);
//                                  EEPROM_ReadInformation(EE_WiFi_Password);
//                                  EEPROM_ReadInformation(EE_WiFi_Security);
//                                  ConnectWiFi_AP();
//                                }
//                              }
                              else if(by_Eng == _Manage_Item_MaxElevation)
                              {// 5为单位换算
                                  if((by_Buffer % 5)!= 0)
                                  {
                                      by_Buffer = (by_Buffer / 5)*5;
                                  }
                              }
                              //================================================
                              // 储存阶段设定 
                              //================================================
                              Led_7SEG(4,0);
                              if((Engineer.UseNumKey == 1)&&
                                ((by_Buffer < by_Min)||(by_Buffer > by_Max)))
                              {// 使用数字键输入后判断其范围,如失败则还原数值
                                  by_Buffer = _NumberKeyBuffer;
                                  _SegDigits = 1;
                                  break;
                              }
                              else
                              {
                                  Engineer.UseNumKey = 0;
                                  if(by_Eng == _Manage_Item_MaxTime)
                                  {
                                      if(by_Buffer < EEPROM_ReadInformation(EE_USER_TIME))
                                      {
                                          EEPROM_SaveWorkInformation(EE_USER_TIME,by_Buffer);
                                      }
                                  }
                              }
                              BEEP_Proce(4,3,3);
                              EEPROM_SaveWorkInformation(EngineerMode[by_Eng].EEPRomNum,by_Buffer);
                              if(by_Eng == _Manage_Item_Unit)
                              {
                                  if(by_Buffer == C_KM)
                                  {
                                      EEPROM_SaveWorkInformation(EE_WEIGHT,Param_UserWeight_DF_Kg);
                                      EEPROM_SaveWorkInformation(EE_Height,_Height_KM_DF);
                                      EEPROM_SaveWorkInformation(EE_MaxSpeed,Param_MaxSpeed_KPH_DF);
                                      EEPROM_SaveWorkInformation(EE_StartSpeed,Param_StartSpeed_KPH_DF);
                                  }
                                  else
                                  {
                                      EEPROM_SaveWorkInformation(EE_WEIGHT,Param_UserWeight_DF);
                                      EEPROM_SaveWorkInformation(EE_Height,_Height_Mile_DF);
                                      EEPROM_SaveWorkInformation(EE_MaxSpeed,Param_MaxSpeed_MPH_DF);
                                      EEPROM_SaveWorkInformation(EE_StartSpeed,Param_StartSpeed_MPH_DF);
                                  }
                                  Console_SpeedUnit(Data_Set,by_Buffer);
                              }
                              //else if(by_Eng == _Manage_Item_Speaker)
                              //{
                                  //Voice_SetSwitch(by_Buffer);
                              //}
                              else if(by_Eng == _Manage_Item_Beeper)
                              {
                                  BEEP_SetSwitch(by_Buffer);
                              }
                              else if(by_Eng == _Manage_Item_Type)
                              {
                                  WiFi_Information_MachineTypeDefaultCheck();
                              }
                              else if(by_Eng == _Manage_Item_TimeZone)
                              {
                                  WiFi_Information_TimeZoneUpdate(by_Buffer);
                              }
                              // 取出第2层选项数量
                              by_Max = ManageItemName[EngBu[0]-1].ItemRange;
                              // 起始值
                              by_Min = ManageItemName[EngBu[0]-1].ItemTarget;
                              // 调整范围值
                              by_Max += by_Min;
                              _NumberRange = 1;
                              _Layer = 2;
//                              LED_MATRIX(_BigMatrix_Clear);// 清大Logo灯
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              break;
                       case 4:// 参数设定或其他
                              Led_7SEG(4,0);
                              if(by_Eng == _Manage_Item_DefaultVolume)
                              {
                                  if((Engineer.UseNumKey == 1)&&
                                  ((by_Buffer < by_Min)||(by_Buffer > by_Max)))
                                  {// 使用数字键输入后判断其范围,如失败则还原数值
                                      by_Buffer = _NumberKeyBuffer;
                                      _SegDigits = 1;
                                      break;
                                  }
                                  else  
                                  {
                                      // 
                                      if(_SegDigits != 1 && EngBu[2] == 2)
                                      {// Output Default音源输出调整用
                                          if(_NumberKeyBuffer != by_Buffer)
                                          {// 第一次Enter切换声音大小
                                              _NumberKeyBuffer = by_Buffer;
                                              TIM4->CCR1 = Console_GetVolumeNumber(by_Buffer);// PWM duty调整使用(PB.6)
                                              _SegDigits = 1;
                                              break;
                                          }
                                          // 第二次Enter储存设定值
                                          //TIM4->CCR1 = CCR1_Val_Close;//==> PWM duty调整使用(PB.6)
                                          //IO_TV_Audio();// 音源
                                          //IO_CloseAudio();//==>SHUTDOWN
                                      }
                                      else if(EngBu[2] == 1 && EEPROM_ReadInformation(EE_OutputVolume) > by_Buffer)
                                      {//Output volume > MAX volume
                                        if(_SegDigits != 1)
                                        {
                                          by_Buffer = _NumberKeyBuffer;
                                          _SegDigits = 1;
                                        }
                                        break;
                                      }
                                      //
                                      EEPROM_SaveWorkInformation(VolumeItem[EngBu[2]].EEPRomNum,by_Buffer);
                                      Engineer.UseNumKey = 0;
                                      _SegDigits = 1;
                                  }
                              }
                              else if(by_Eng == _Manage_Item_HeadphoneJack)
                              {
                                  if(EngBu[2] == 0)
                                  {// Switch
                                      EEPROM_SaveWorkInformation(HJSItem[EngBu[2]].EEPRomNum,by_Buffer);
                                  }
                                  else
                                  {// Check Number
                                      if((Engineer.UseNumKey == 1)&&
                                      ((by_Buffer < by_Min)||(by_Buffer > by_Max)))
                                      {// 使用数字键输入后判断其范围,如失败则还原数值
                                          by_Buffer = _NumberKeyBuffer;
                                          _SegDigits = 1;
                                          break;
                                      }
                                      else  
                                      {
                                          EEPROM_SaveWorkInformation(HJSItem[EngBu[2]].EEPRomNum,by_Buffer);
                                          Engineer.UseNumKey = 0;
                                          _SegDigits = 1;
                                      }
                                  }
                              }
                              else if(by_Eng == _Manage_Item_AutoUpdate)
                              {
                                  if(EngBu[2] == 1)
                                  {// Switch
                                      EEPROM_SaveWorkInformation(AutoUpdateItem[EngBu[2]].EEPRomNum,by_Buffer);
                                  }
                                  else if(EngBu[2] == 2)
                                  {// Check Number
                                      if((Engineer.UseNumKey == 1)&&
                                      ((by_Buffer < by_Min)||(by_Buffer > by_Max)))
                                      {// 使用数字键输入后判断其范围,如失败则还原数值
                                          by_Buffer = _NumberKeyBuffer;
                                          _SegDigits = 1;
                                          break;
                                      }
                                      else  
                                      {
                                          EEPROM_SaveWorkInformation(AutoUpdateItem[EngBu[2]].EEPRomNum,by_Buffer);
                                          Engineer.UseNumKey = 0;
                                          _SegDigits = 1;
                                      }
                                  }
                              }
                              else if(by_Eng == _Manage_Item_Keypad)
                              {
                                  EEPROM_SaveWorkInformation(KeypadItem[EngBu[2]].EEPRomNum,by_Buffer);
                              }
                              else if(by_Eng == _Manage_Item_USBPort)
                              {
                                  EEPROM_SaveWorkInformation(EE_USB_Protection,by_Buffer);
                              }
                              BEEP_Proce(4,3,3);
                              _Layer = 3;
                              by_Buffer = EngBu[2];
                              by_Min = EngineerMode[by_Eng].Range[0];
                              by_Max = EngineerMode[by_Eng].Range[1];
                              _NumberRange = EngineerMode[by_Eng].AdjustSize;
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              break;
                   }
                   break;
          case K_BACK:
                   Engineer.NumKeyStart=0;
                   switch(_Layer)
                   {
                       case 1:// 回原头选项
                              by_Status = M_NEXT;
                              break;
                       case 2:// 回第一层
                              Led_7SEG(4,0);
                              _Layer = 1;
                              by_Eng = EngBu[0];
                              by_Min = 1;
                              by_Max = _Manage_Item;
                              Engineer.UseNumKey = 0;
                              _NumberRange = 1;
                              Engineer.Enter = 0;
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              break;
                       case 3:// 回第二层
                              Led_7SEG(4,0);
                              if((Engineer.UseNumKey == 1)&&(_SegDigits != 1))
                              {// 使用数字键输入后判断其范围,如失败则还原数值
                                  by_Buffer = _NumberKeyBuffer;
                                  _SegDigits = 1;
                                  break;
                              }
                              Engineer.UseNumKey = 0;
                              _NumberRange = 1;
                              _Layer = 2;
                              // 取出第2层选项数量
                              by_Max = ManageItemName[EngBu[0]-1].ItemRange;
                              // 起始值
                              by_Min = ManageItemName[EngBu[0]-1].ItemTarget;
                              // 调整范围值
                              by_Max += by_Min;
//                              LED_MATRIX(_BigMatrix_Clear);// 清大Logo灯
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              break;
                       case 4:// 回第三层   
                              if(by_Eng == _Manage_Item_DefaultVolume)
                              {
                                  if(EngBu[2] == 2)
                                  {// Output Default
                                      //TIM4->CCR1 = CCR1_Val_Close;//==> PWM duty调整使用(PB.6)
                                      //IO_TV_Audio();// 音源
                                      //IO_CloseAudio();//==>SHUTDOWN
                                      if(_SegDigits != 1)
                                      {
                                          by_Buffer = _NumberKeyBuffer;
                                          _SegDigits = 1;
                                          break;
                                      }
                                  }
                              }
                              Led_7SEG(4,0);
                              _Layer = 3;
                              by_Buffer = EngBu[2];
                              by_Min = EngineerMode[by_Eng].Range[0];
                              by_Max = EngineerMode[by_Eng].Range[1];
                              _NumberRange = EngineerMode[by_Eng].AdjustSize;
                              Engineer.UseNumKey = 0;
                              _SegDigits = 1;
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              break;
                   }
                   break;
      }
      if(Timer_Counter(T_STEP,3,1200)) by_Status = M_RESET;
      if(!Check_SafeKey()) by_Status = M_RESET;
  }
  //TIM4->CCR1 = CCR1_Val_Close;//==> PWM duty调整使用(PB.6)
  //IO_TV_Audio();// 音源
  //IO_CloseAudio();//==>SHUTDOWN
  return by_Status;
}
/*******************************************************************************
* Function Name  : Engineer_Mode()
* Input          : None
* Output         : None
* Return         : by_Status
*******************************************************************************/
char EngineerMode_Engineer(void)
{
  unsigned char by_Status=M_NONE;
  unsigned char _Layer = 1;// 选项阶层
  unsigned char _NumberRange = 1 ;
  unsigned char by_Eng = _Manage_Item+1;
  unsigned char by_Min = _Manage_Item+1;
  unsigned char by_Max = _Engineer_Item;
  unsigned char by_Buffer = 0;
  unsigned char EngBu[2];
  unsigned short _KeyBuffer;
  
  Engineer.key = 0;
  Engineer.key1 = 0;
  Timer_Counter_Clear(3);
  Led_Set_Target(Display_OFF,0);
  if((Digital_GetStatus() & 0x03) != 0)
  {// 升降有动作时就需要强制停止
      Digital_Command(CmdSetInclineAction,0);//==>升降停止    
  }
  Digital_Set_Work(0);//==>在ENG模式下
  while(by_Status == M_NONE)
  {
      switch(_Layer)
      {
          case 1:// 第1层选项
                 Led_String((unsigned char *)ManageItemName[by_Eng-1].ItemName,0,Show_Auto);
                 //Led_String(EngineerMode[by_Eng].ItemName);
                 break;
          case 2:// 第2层
                 if(by_Eng == _Eng_Item_SSL) 
                 {
                     Led_String((unsigned char *)EngineerMode[by_Eng].ItemName,0,Show_Auto);
                 }
                 else
                 {
                     switch(EngineerMode[by_Eng].NumberDisplayWay)
                     {
                         case _String_None:
                              Led_String((unsigned char *)EngineerMode[by_Eng].ItemName,0,Show_Auto);
                              break;
                         case _String_DisableEnable:// 显示Disable or Enable
                              Led_String((unsigned char *)Words_Check[EngineerMode[by_Eng].NumberDisplayWay].Words[by_Buffer],0,Show_Auto);
                              break;
                     }
                 }
                 break;
          case 3:// 第3层    
                 if(by_Eng == _Eng_Item_DAPI) 
                 {  
                     Led_String((unsigned char *)ServerIP[by_Buffer].ItemName,0,Show_Auto);
                 }
                 else if(by_Eng == _Eng_Item_SSL) 
                 {  
                     Led_String((unsigned char *)Words_Check[EngineerMode[by_Eng].NumberDisplayWay].Words[by_Buffer],0,Show_Auto);
                 }
                 else
                 {
                     Led_Window(Window_2,1,Digtial_Get_InclineLocation()/128);
                     if(Engineer.key1 && Timer_Counter(T_STEP,0,5) == 1)
                     {// 按键后500ms下停止command
                         Engineer.key1 = 0;
                         Digital_Command(CmdSetInclineAction,0);//==>升降停止    
                     }
                 }
                 break;
      }     
      _KeyBuffer = Key_Proce();
      if(_KeyBuffer != K_NONE)
      {
          Timer_Counter_Clear(3);
      } 
      Engineer.key = D_ADD;
      switch(_KeyBuffer)
      {
          case K_HoldIDSD: 
                   // RPM 参数微调
                   EngineerMode_RPMParameter();
                   //
                   _Layer = 1;
                   by_Eng = _Manage_Item+1;
                   by_Min = _Manage_Item+1;
                   by_Max = _Engineer_Item;
                   _NumberRange = 1;
                   break;
          case K_SpeedD | K_MASK:
          case K_SpeedD:             
          case K_InclineD | K_MASK:         
          case K_InclineD: 
          case K_TimeD | K_MASK:
          case K_TimeD:           
                   Engineer.key = D_DEC;
          case K_SpeedU | K_MASK:
          case K_SpeedU:          
          case K_TimeU | K_MASK:
          case K_TimeU:         
          case K_InclineU | K_MASK:
          case K_InclineU:  
                   switch(_Layer)
                   {
                       case 1:      
                              by_Eng = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Eng);
                              break;
                       case 2:      
                              if((by_Eng >= _Eng_Item_AutoCalibration && by_Eng <= _Eng_Item_EvationMax)||\
                                 (by_Eng >= _Eng_Item_DAPI && by_Eng <= _Eng_Item_SSL) )
                                  by_Eng = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Eng);           
                              else  
                                  by_Buffer = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Buffer);
                              break;
                       case 3:
                              if(by_Eng == _Eng_Item_DAPI ||\
                                 by_Eng == _Eng_Item_SSL) // v2.1-11
                              {
                                  by_Buffer = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Buffer);
                              }
                              else
                              {
                                  if(Engineer.key1 == 0)
                                  {
                                      Engineer.key1 = 1;
                                      if(Engineer.key == D_ADD)
                                          Digital_Command(CmdSetInclineAction,1);//==>升降上升
                                      else
                                          Digital_Command(CmdSetInclineAction,0xff);//==>升降下降
                                  }
                                  Timer_Counter_Clear(0);
                              }
                              break;
                   }
                   break;
          case K_ENTER:  
                   if(_Layer == 1)
                   {
                       
                       _Layer = 2;
                       // 取出第2层选项数量
                       by_Max = ManageItemName[by_Eng-1].ItemRange;
                       // 暂存第1层地址
                       EngBu[0] = by_Eng;
                       // 取出第2层对应值
                       by_Eng = ManageItemName[by_Eng-1].ItemTarget;
                       // 起始值
                       by_Min = by_Eng;
                       // 调整范围值
                       by_Max += by_Min;
                       
                       if(by_Eng == _Eng_Item_RunningTest)
                       {
                           EngineerMode_RunningTestMode();
                           Digital_Set_Work(0);//==>在ENG模式下
                           _Layer = 1;
                           by_Eng = EngBu[0];
                           by_Min = _Manage_Item+1;
                           by_Max = _Engineer_Item;
                           _NumberRange = 1;
                       }
                       else if(by_Eng == _Eng_Item_ErrorCode)// ||
                               //by_Eng == _Eng_Item_DemoMode )
                       {
                           by_Buffer = EEPROM_ReadInformation(EngineerMode[by_Eng].EEPRomNum);
                           by_Min = EngineerMode[by_Eng].Range[0];
                           by_Max = EngineerMode[by_Eng].Range[1];
                           _NumberRange = EngineerMode[by_Eng].AdjustSize;
                       }
                       
                       else if(by_Eng == _Eng_Item_BackUp)
                       {
                         EngineerMode_SoftwareBackup();
                         _Layer = 1;
                         by_Eng = EngBu[0];
                         by_Min = _Manage_Item+1;
                         by_Max = _Engineer_Item;
                         _NumberRange = 1;
                       }
                       
                   }
                   else if(_Layer == 2)
                   {
                       if(by_Eng == _Eng_Item_EvationMin ||
                          by_Eng == _Eng_Item_EvationMax )
                       {
                           _Layer = 3;
                           Engineer.key1 = 0;
                           if(by_Eng == _Eng_Item_EvationMin)
                           {
                               Led_String(" Min :   ",0,0);
                               Digital_Command(CmdSetInclineLocation,Console_HighLowADC(Data_Get,0,0));//==>升降设定
                           }
                           else
                           {
                               Led_String(" Max :   ",0,0);
                               Digital_Command(CmdSetInclineLocation,Console_HighLowADC(Data_Get,1,0));//==>升降设定 
                           }
                           break;
                       }
                       else if(by_Eng == _Eng_Item_DAPI || \
                               by_Eng == _Eng_Item_SSL ) // v2.1-11
                       {
                           _Layer = 3;
                           by_Buffer = EEPROM_ReadInformation(EngineerMode[by_Eng].EEPRomNum);
                           by_Min = EngineerMode[by_Eng].Range[0];
                           by_Max = EngineerMode[by_Eng].Range[1];
                           _NumberRange = EngineerMode[by_Eng].AdjustSize;
                           break;
                       }
                       else if(by_Eng == _Eng_Item_AutoCalibration)
                       {
                           Digital_Set_AutoCheck(1);//==>进入自动更正模式
                           Led_String(" HIGH     ",0,0);
                           EngineerMode_AutoCalibration();
                           by_Status = M_RESET;
                       }
                       else
                       {
                           BEEP_Proce(4,3,3);
                           EEPROM_SaveWorkInformation(EngineerMode[by_Eng].EEPRomNum,by_Buffer);
                       }
                       _Layer = 1;
                       by_Eng = EngBu[0];
                       by_Min = _Manage_Item+1;
                       by_Max = _Engineer_Item;
                       _NumberRange = 1;
                   }
                   else if(_Layer == 3)
                   {
                       if(by_Eng == _Eng_Item_DAPI)
                       {
                           EEPROM_SaveWorkInformation(EngineerMode[by_Eng].EEPRomNum,by_Buffer);
                           WiFi_Information_ChangeDAPI_Server(_ON);
                       }
                       // start v2.1-11
                       else if(by_Eng == _Eng_Item_SSL)
                       {
                           EEPROM_SaveWorkInformation(EngineerMode[by_Eng].EEPRomNum,by_Buffer);
                       }
                       // end v2.1-11
                       else if(by_Eng == _Eng_Item_EvationMin)
                       {
                           if(Digtial_Get_InclineLocation() < (Console_HighLowADC(Data_Get,1,0)-(120*128)))
                           {// 确保范围值
                               Console_HighLowADC(Data_Set,0,Digtial_Get_InclineLocation());
                               EEPROM_SaveWorkInformation(EngineerMode[by_Eng].EEPRomNum,Console_HighLowADC(Data_Get,0,0));  
                           }
                       }
                       else if(by_Eng == _Eng_Item_EvationMax)
                       {
                           if(Digtial_Get_InclineLocation() > (Console_HighLowADC(Data_Get,0,0)+(120*128)))
                           {// 确保范围值
                               Console_HighLowADC(Data_Set,1,Digtial_Get_InclineLocation());
                               EEPROM_SaveWorkInformation(EngineerMode[by_Eng].EEPRomNum,Console_HighLowADC(Data_Get,1,0));  
                           }
                       }
                       // start v2.1-11
                       if(by_Eng == _Eng_Item_DAPI ||\
                          by_Eng == _Eng_Item_SSL )
                       {
                           by_Min=_Eng_Item_DAPI;
                           by_Max=_Eng_Item_SSL;
                           by_Buffer=by_Eng;
                           _NumberRange = EngineerMode[by_Eng].AdjustSize;
                       }
                       // end v2.1-11
                       BEEP_Proce(4,3,3);
                       _Layer = 2;
                       
                   }
                   break;
          case K_BACK: 
                   if(_Layer == 3)
                   {
                       _Layer = 2;
                       if(by_Eng == _Eng_Item_EvationMin ||
                          by_Eng == _Eng_Item_EvationMax )
                       {
                           Digital_Command(CmdSetInclineAction,0);//==>升降停止   
                       }
                       // start v2.1-11
                       else if(by_Eng == _Eng_Item_DAPI ||\
                               by_Eng == _Eng_Item_SSL )
                       {
                           by_Min=_Eng_Item_DAPI;
                           by_Max=_Eng_Item_SSL;
                           by_Buffer=by_Eng;
                           _NumberRange = EngineerMode[by_Eng].AdjustSize;
                       }
                       // end v2.1-11
                   }
                   else if(_Layer == 2)
                   {
                       _Layer = 1;
                       by_Eng = EngBu[0];
                       by_Min = _Manage_Item+1;
                       by_Max = _Engineer_Item;
                       _NumberRange = 1;
                   }
                   else
                   {
                       by_Status = M_NEXT;
                   }
                   break;
      }
      if(Timer_Counter(T_STEP,3,1200)) by_Status = M_RESET;
      if(!Check_SafeKey()) by_Status = M_RESET;
  }
  return by_Status;
}
/*******************************************************************************
* Function Name  : Service_Mode()
* Input          : None
* Output         : None
* Return         : by_Status
*******************************************************************************/
char EngineerMode_Service(void)
{  
  unsigned char  w_TimeBuffer[6];
  unsigned short by_Min = _Engineer_Item+1;
  unsigned short by_Max = _Service_Item;
  unsigned int by_Buffer = 0;
  unsigned short _KeyBuffer;
  unsigned short _NumberKeyBuffer;
  unsigned char by_Status = M_NONE;
  unsigned char by_Eng = _Engineer_Item+1;
  unsigned char _NumberRange = 1;
  unsigned char _Layer = 1;// 选项阶层
  unsigned char EngBu[4];
  unsigned char _SegDigits = 1;
  unsigned char i = 0;  
  unsigned char  by_Bu = 0;
  
  Engineer.key = 0;
  Engineer.Enter = 0;
  Engineer.UseNumKey = 0;
  Timer_Counter_Clear(3);
  Led_Set_Target(Display_OFF,0);
  if((Digital_GetStatus() & 0x03) != 0)// S003-01
  {// 升降有动作时就需要强制停止
      Digital_Command(CmdSetInclineAction,0);//==>升降停止    
  }
  SHT1381 time;//==>HT1381时间IC Data指向
  ReadHT1381(&time);
  by_Bu = time.Seconds / 16 * 10;
  w_TimeBuffer[5] = by_Bu + (time.Seconds % 16);
  by_Bu = time.Minutes / 16 * 10;
  w_TimeBuffer[4] = by_Bu + (time.Minutes % 16);
  by_Bu = time.Hours / 16 * 10;
  w_TimeBuffer[3] = by_Bu + (time.Hours % 16);
  by_Bu = time.Date / 16 * 10;
  w_TimeBuffer[2] = by_Bu + (time.Date % 16);
  by_Bu = time.Month / 16 * 10;
  w_TimeBuffer[1] = by_Bu + (time.Month % 16);
  by_Bu = time.Year / 16 * 10;
  w_TimeBuffer[0] = by_Bu + (time.Year % 16);
  
  while(by_Status == M_NONE)
  {
      UsbFunction_USB(0x500); 
      switch(_Layer)
      {
          case 1:// 第1层大选项
                 i = 0;
                 Led_String((unsigned char *)ManageItemName[by_Eng-1].ItemName,0,Show_Auto);
                 break;
          case 2:// 第2层小选项
                 //if(by_Eng == _Service_Item_HJS) 
                 //    Show_MessageWithNumber((unsigned char *)EngineerMode[by_Eng].ItemName,99+i,0,0,Show_Auto);//by_Eng
                 //else    
                     Led_String((unsigned char *)EngineerMode[by_Eng].ItemName,0,Show_Auto);
                 break;
          case 3:
                 switch(EngineerMode[by_Eng].NumberDisplayWay)
                 {
                     case _String_Number:
                              if(by_Eng == _Service_Item_ClubID)
                              {
                                  Led_Window(26,6,by_Buffer); 
                              }
                              break;
                     case _String_None:
                              if(by_Eng == _Service_Item_Error)
                              {
                                  if(by_Buffer == 0)
                                      Led_String("Display",0,Show_Auto);
                                  else
                                      Led_String("Reset",0,Show_Auto);
                              }
                              else if(by_Eng == _Service_Item_HJS)
                              {
                                  if(by_Buffer == 0)
                                      Led_String("Insert Counts",0,Show_Auto);
                                  else
                                      Led_String("Reset",0,Show_Auto);
                              }
                              else if(by_Eng ==_Service_Item_RunningBelt)
                              {
                                  Led_String((unsigned char *)RunningBeltItem[by_Buffer].ItemName,0,Show_Auto);
                              }
                              else if(by_Eng ==_Service_Item_RunningDeck)
                              {
                                  Led_String((unsigned char *)RunningBeltItem[by_Buffer].ItemName,0,Show_Auto);
                              }
                              else if(by_Eng ==_Service_Item_MCB)
                              {
                                  Led_String((unsigned char *)RunningBeltItem[by_Buffer].ItemName,0,Show_Auto);
                              }
                              else if(by_Eng ==_Service_Item_Motor)
                              {
                                  Led_String((unsigned char *)RunningBeltItem[by_Buffer].ItemName,0,Show_Auto);
                              }
                              else if(by_Eng ==_Service_Item_Maintenance)
                              {
                                  Led_String((unsigned char *)RunningBeltItem[by_Buffer].ItemName,0,Show_Auto);
                              }
                              else if(by_Eng ==_Service_Item_HeadphoneJack)
                              {
                                  Led_String((unsigned char *)RunningBeltItem[by_Buffer].ItemName,0,Show_Auto);
                              }
                              break;
                     case _String_DisableEnable:// 显示Disable or Enable
                              Led_String((unsigned char *)Words_Check[EngineerMode[by_Eng].NumberDisplayWay].Words[by_Buffer],0,Show_Auto);
                              break;
                 }
                 break;
          case 4:
                 if(by_Eng == _Manage_Item_HeadphoneJack)
                 {
                     
                 }
                 else if(by_Eng == _Service_Item_HJS)
                 {
                     Led_Window(16,6,Console_HJS(Data_Get,Show_Auto)); 
                 }
                 break;
      }
      
      
      _KeyBuffer = Key_Proce();
      if(_KeyBuffer != K_NONE)
      {
          Timer_Counter_Clear(3);
      }   
      if((Engineer.UseNumKey == 1)&&
        ((_KeyBuffer >= K_0)&&(_KeyBuffer <= K_9)) )
      {
          if(_SegDigits == 1)
          {
              Engineer.NumKeyStart=1;
              // 先将原先的植备份起来
              _NumberKeyBuffer = by_Buffer;
              //
              by_Buffer = _KeyBuffer - K_0;
          }
          else if(_SegDigits <= EngineerMode[by_Eng].NumericKey.Digits) 
          {
              by_Buffer *= 10;
              by_Buffer += (_KeyBuffer - K_0);
          }
          _SegDigits++;
      }
      Engineer.key = D_ADD;
      switch(_KeyBuffer)
      {
          case K_SpeedD | K_MASK:
          case K_SpeedD:             
          case K_InclineD | K_MASK:         
          case K_InclineD: 
          case K_TimeD | K_MASK:
          case K_TimeD:           
                   Engineer.key = D_DEC;
          case K_SpeedU | K_MASK:
          case K_SpeedU:          
          case K_TimeU | K_MASK:
          case K_TimeU:         
          case K_InclineU | K_MASK:
          case K_InclineU:  
                   i++;  
                   Led_Set_Reset();
                   switch(_Layer)
                   {
                       case 2:
                              
                              if(by_Eng == _Service_Item_HJS) 
                                Show_Message(Message_Blank,0,0,Show_Blank);
                       case 1:   
                              by_Eng = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Eng);
                              Led_Set_Reset();
                              break;
                       case 3:    
                              if(Engineer.UseNumKey == 1) 
                              {
                                  if(Engineer.NumKeyStart == 1)
                                  {
                                      Engineer.NumKeyStart=0;
                                      by_Buffer=_NumberKeyBuffer;
                                      _SegDigits = 1;
                                  }
                                  by_Buffer = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Buffer);
                                  break;
                              }
                              by_Buffer = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Buffer);
                              //if(by_Eng == _Service_Item_Error)
                              //{
                              //    Led_String(" ERR.             ",0,0);
                              //    Led_NumberSite(31,by_Buffer);
                              //    Led_Special(0,70,EEPROM_ReadInformation(EE_DigitalError_1+(by_Buffer-1)));
                              //}
                              break;
                      case 4:    
                              by_Buffer = Keyboard_DFCount(Engineer.key,by_Min,by_Max,_NumberRange,by_Buffer);
                              if(by_Eng == _Service_Item_Error)
                              {
                                  Led_String(" ERR.     ",0,Show_Auto);
                                  Led_NumberSite(31,by_Buffer);
                                  Led_Special(2,40,EEPROM_ReadInformation(EE_DigitalError_1+(by_Buffer-1)));
                              }
                              else if(by_Eng == _Service_Item_RunningBelt)
                              {
                                if(EngBu[2]==0)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningBeltCurrentAccMiles));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningBeltCurrentAccHours));
                                  }
                                }
                                else if(EngBu[2]==2)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String("    -  -   ",0,0);
                                    Led_Window(6,1,EEPROM_ReadInformation(EE_RunningBeltPreviousYear));
                                    Led_Window(34,8,EEPROM_ReadInformation(EE_RunningBeltPreviousMouth));
                                    Led_Window(52,8,EEPROM_ReadInformation(EE_RunningBeltPreviousDay));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningBeltPreviousAccMiles));
                                  }
                                  else if(by_Buffer==3)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningBeltPreviousAccHours));
                                  }
                                }
                              }
                              else if(by_Eng == _Service_Item_RunningDeck)
                              {
                                if(EngBu[2]==0)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningDeckCurrentAccMiles));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningDeckCurrentAccHours));
                                  }
                                }
                                else if(EngBu[2]==2)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String("    -  -   ",0,0);
                                    Led_Window(6,1,EEPROM_ReadInformation(EE_RunningDeckPreviousYear));
                                    Led_Window(34,8,EEPROM_ReadInformation(EE_RunningDeckPreviousMouth));
                                    Led_Window(52,8,EEPROM_ReadInformation(EE_RunningDeckPreviousDay));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningDeckPreviousAccMiles));
                                  }
                                  else if(by_Buffer==3)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningDeckPreviousAccHours));
                                  }
                                }
                              }
                              else if(by_Eng == _Service_Item_MCB)
                              {
                                if(EngBu[2]==0)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MCBCurrentAccMiles));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MCBCurrentAccHours));
                                  }
                                }
                                else if(EngBu[2]==2)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String("    -  -   ",0,0);
                                    Led_Window(6,1,EEPROM_ReadInformation(EE_MCBPreviousYear));
                                    Led_Window(34,8,EEPROM_ReadInformation(EE_MCBPreviousMouth));
                                    Led_Window(52,8,EEPROM_ReadInformation(EE_MCBPreviousDay));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MCBPreviousAccMiles));
                                  }
                                  else if(by_Buffer==3)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MCBPreviousAccHours));
                                  }
                                }
                              }
                              else if(by_Eng == _Service_Item_Motor)
                              {
                                if(EngBu[2]==0)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MoterCurrentAccMiles));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MoterCurrentAccHours));
                                  }
                                }
                                else if(EngBu[2]==2)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String("    -  -   ",0,0);
                                    Led_Window(6,1,EEPROM_ReadInformation(EE_MoterPreviousYear));
                                    Led_Window(34,8,EEPROM_ReadInformation(EE_MoterPreviousMouth));
                                    Led_Window(52,8,EEPROM_ReadInformation(EE_MoterPreviousDay));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MoterPreviousAccMiles));
                                  }
                                  else if(by_Buffer==3)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MoterPreviousAccHours));
                                  }
                                }
                              }
                              else if(by_Eng == _Service_Item_HeadphoneJack)
                              {
                                if(EngBu[2]==0)
                                {
                                  if(by_Buffer==1)
                                  {
                                  Led_String(" I: ",0,0);
                                  Led_Window(Window_3,6,EEPROM_ReadInformation(EE_HeadphoneCurrentInsertsTimes));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_HeadphoneCurrentAccHours));
                                  }
                                }
                                else if(EngBu[2]==2)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String("    -  -   ",0,0);
                                    Led_Window(6,1,EEPROM_ReadInformation(EE_HeadphonePreviousYear));
                                    Led_Window(34,8,EEPROM_ReadInformation(EE_HeadphonePreviousMouth));
                                    Led_Window(52,8,EEPROM_ReadInformation(EE_HeadphonePreviousDay));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" I: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_HeadphonePreviousInsertsTimes));
                                  }
                                  else if(by_Buffer==3)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_HeadphonePreviousAccHours));
                                  }
                                }
                              }
                              else if(by_Eng == _Service_Item_Maintenance)
                              {
                                if(EngBu[2]==0)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_Prev_MaintenanceCurrentAccMiles));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_Prev_MaintenanceCurrentAccHours));
                                  }
                                }
                                else if(EngBu[2]==2)
                                {
                                  if(by_Buffer==1)
                                  {
                                    Led_String("    -  -   ",0,0);
                                    Led_Window(6,1,EEPROM_ReadInformation(EE_Prev_MaintenancePreviousYear));
                                    Led_Window(34,8,EEPROM_ReadInformation(EE_Prev_MaintenancePreviousMouth));
                                    Led_Window(52,8,EEPROM_ReadInformation(EE_Prev_MaintenancePreviousDay));
                                  }
                                  else if(by_Buffer==2)
                                  {
                                    Led_String(" M: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_Prev_MaintenancePreviousAccMiles));
                                  }
                                  else if(by_Buffer==3)
                                  {
                                    Led_String(" H: ",0,0);
                                    Led_Window(Window_3,6,EEPROM_ReadInformation(EE_Prev_MaintenancePreviousAccHours));
                                  }
                                }
                              }
                              break;
                   }
                   break;
          case K_ENTER:  
                   Engineer.NumKeyStart=0;
                   switch(_Layer)
                   {
                       case 1:
                              _Layer = 2;
                              // 取出第2层选项数量
                              by_Max = ManageItemName[by_Eng-1].ItemRange;
                              // 暂存第1层地址
                              EngBu[0] = by_Eng;
                              // 取出第2层对应值
                              by_Eng = ManageItemName[by_Eng-1].ItemTarget;
                              // 起始值
                              by_Min = by_Eng;
                              // 调整范围值
                              by_Max += by_Min;
                              break;
                       case 2:
                              if(by_Eng == _Service_Item_AccDistance ||
                                 by_Eng == _Service_Item_AccTime)
                              {
                                  by_Buffer = _Service_Item_AccDistance;
                                  EngineerMode_Accumulated(by_Eng - by_Buffer);
                                  break;
                              }
                              else if(by_Eng == _Service_Item_ExportUSB ||\
                                      by_Eng == _Service_Item_ImportUSB)
                              {
                                  EngineerMode_ExportOrImport(by_Eng);
                                  Timer_Counter_Clear(3);
                                  break;
                              }
                              else if(by_Eng == _Service_Item_Reset)
                              {// 恢复出厂默认值
                                  EEPROM_ParameterDefault(1) ;
                                  BEEP_Proce(6,2,2);
                                  break;
                              }
                              _Layer = 3;
                              by_Min = EngineerMode[by_Eng].Range[0];
                              by_Max = EngineerMode[by_Eng].Range[1];
                              _NumberRange = EngineerMode[by_Eng].AdjustSize;
                              by_Buffer = by_Min;
                              //if(by_Eng == _Service_Item_Error)
                              //{
                              //    Led_String(" ERR.             ",0,0);
                              //    Led_NumberSite(30,by_Buffer);
                              //    Led_Special(0,70,EEPROM_ReadInformation(EE_DigitalError_1+(by_Buffer-1)));                             
                              //}
                              //else 
                                if(by_Eng == _Service_Item_AMswitch ||
                                      by_Eng == _Service_Item_ClubID ||
                                      by_Eng == _Service_Item_xIDswitch )
                              {
                                  by_Buffer = EEPROM_ReadInformation(EngineerMode[by_Eng].EEPRomNum);
                                  if(by_Eng == _Service_Item_ClubID)
                                  {
                                      Led_String("ID :      ",0,0);
                                      Engineer.UseNumKey = 1; 
                                  }
                              }
                              break;
                       case 3:
                              if(by_Eng == _Service_Item_Error )
                              {
                                  if(by_Buffer == 0)
                                  {
                                      _Layer = 4;
                                      by_Min = 1;
                                      by_Max = 10;
                                      _NumberRange = 1;
                                      by_Buffer = by_Min;
                                      Led_String(" ERR.     ",0,0);
                                      Led_NumberSite(31,by_Buffer);
                                      Led_Special(2,40,EEPROM_ReadInformation(EE_DigitalError_1+(by_Buffer-1)));
                                  }
                                  else
                                  {
                                      // 清除次数
                                      for(_Layer=0;_Layer < 10;_Layer++)
                                      {
                                          EEPROM_SaveWorkInformation(EE_DigitalError_1+_Layer,0);
                                      }
                                      BEEP_Proce(4,3,3);
                                      _Layer = 3;
                                      Show_Message(Message_Blank,0,0,Show_Blank);
                                  }
                                  
                                  break;
                              }
                              else if(by_Eng == _Service_Item_HJS)
                              {
                                  if(by_Buffer == 0)
                                  {
                                      _Layer = 4;
                                  }
                                  else
                                  {
                                      // 清除次数
                                      Console_HJS(Data_Set,0);
                                      EEPROM_SaveWorkInformation(EE_HJS_total,Console_HJS(Data_Get,0));
                                      BEEP_Proce(4,3,3);
                                  }
                                  Show_Message(Message_Blank,0,0,Show_Blank);
                                  break;
                              }
                              else if(by_Eng == _Manage_Item_HeadphoneJack)
                              {
                                  _Layer = 4;
                                  break;
                              }
                              else if(by_Eng == _Service_Item_ClubID)
                              {
                                  if((Engineer.UseNumKey == 1)&&\
                                     (by_Buffer > by_Max) )
                                  {// 使用数字键输入后判断其范围,如失败则还原数值
                                      by_Buffer = _NumberKeyBuffer;
                                      _SegDigits = 1;
                                      Led_String("ID :      ",0,0);
                                      break;
                                  }
                                  else
                                  {
                                      Engineer.UseNumKey = 0;
                                      _SegDigits = 1;
                                  }
                                  UpdateMachineInfo();
                              }
                              else if(by_Eng == _Service_Item_AMswitch)
                              {
                                UpdateAMEnable();
                              }
                              else if(by_Eng == _Service_Item_RunningBelt)
                              {
                                if(by_Buffer == 0)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 2;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String(" M: ",0,0);
                                  Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningBeltCurrentAccMiles));
                                }
                                else if(by_Buffer==2)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 3;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String("    -  -   ",0,0);
                                  Led_Window(6,1,EEPROM_ReadInformation(EE_RunningBeltPreviousYear));
                                  Led_Window(34,8,EEPROM_ReadInformation(EE_RunningBeltPreviousMouth));
                                  Led_Window(52,8,EEPROM_ReadInformation(EE_RunningBeltPreviousDay));
                                }
                                else if(by_Buffer==1)
                                {
                                  _Layer = 4;
                                  EngBu[3] = by_Buffer;
                                  by_Min = 0;
                                  by_Max = 0;
                                  Led_String(" Confirm",0,0);
                                }
                                break;
                              }
                              else if(by_Eng == _Service_Item_RunningDeck)
                              {
                                if(by_Buffer == 0)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 2;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String(" M: ",0,0);
                                  Led_Window(Window_3,6,EEPROM_ReadInformation(EE_RunningDeckCurrentAccMiles));
                                }
                                else if(by_Buffer==2)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 3;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String("    -  -   ",0,0);
                                  Led_Window(6,1,EEPROM_ReadInformation(EE_RunningDeckPreviousYear));
                                  Led_Window(34,8,EEPROM_ReadInformation(EE_RunningDeckPreviousMouth));
                                  Led_Window(52,8,EEPROM_ReadInformation(EE_RunningDeckPreviousDay));
                                }
                                else if(by_Buffer==1)
                                {
                                  _Layer = 4;
                                  EngBu[3] = by_Buffer;
                                  by_Min = 0;
                                  by_Max = 0;
                                  Led_String(" Confirm",0,0);
                                }
                                break;
                              }
                              else if(by_Eng == _Service_Item_MCB)
                              {
                                if(by_Buffer == 0)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 2;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String(" M: ",0,0);
                                  Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MCBCurrentAccMiles));
                                }
                                else if(by_Buffer==2)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 3;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String("    -  -   ",0,0);
                                  Led_Window(6,1,EEPROM_ReadInformation(EE_MCBPreviousYear));
                                  Led_Window(34,8,EEPROM_ReadInformation(EE_MCBPreviousMouth));
                                  Led_Window(52,8,EEPROM_ReadInformation(EE_MCBPreviousDay));
                                }
                                else if(by_Buffer==1)
                                {
                                  _Layer = 4;
                                  EngBu[3] = by_Buffer;
                                  by_Min = 0;
                                  by_Max = 0;
                                  Led_String(" Confirm",0,0);
                                }
                                break;
                              }
                              else if(by_Eng == _Service_Item_Motor)
                              {
                                if(by_Buffer == 0)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 2;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String(" M: ",0,0);
                                  Led_Window(Window_3,6,EEPROM_ReadInformation(EE_MoterCurrentAccMiles));
                                }
                                else if(by_Buffer==2)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 3;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String("    -  -   ",0,0);
                                  Led_Window(6,1,EEPROM_ReadInformation(EE_MoterPreviousYear));
                                  Led_Window(34,8,EEPROM_ReadInformation(EE_MoterPreviousMouth));
                                  Led_Window(52,8,EEPROM_ReadInformation(EE_MoterPreviousDay));
                                }
                                else if(by_Buffer==1)
                                {
                                  _Layer = 4;
                                  EngBu[3] = by_Buffer;
                                  by_Min = 0;
                                  by_Max = 0;
                                  Led_String(" Confirm",0,0);
                                }
                                break;
                              }
                              else if(by_Eng == _Service_Item_HeadphoneJack)
                              {
                                if(by_Buffer == 0)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 2;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String(" I: ",0,0);
                                  Led_Window(Window_3,6,EEPROM_ReadInformation(EE_HeadphoneCurrentInsertsTimes));
                                }
                                else if(by_Buffer==2)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 3;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String("    -  -   ",0,0);
                                  Led_Window(6,1,EEPROM_ReadInformation(EE_HeadphonePreviousYear));
                                  Led_Window(34,8,EEPROM_ReadInformation(EE_HeadphonePreviousMouth));
                                  Led_Window(52,8,EEPROM_ReadInformation(EE_HeadphonePreviousDay));
                                }
                                else if(by_Buffer==1)
                                {
                                  _Layer = 4;
                                  EngBu[3] = by_Buffer;
                                  by_Min = 0;
                                  by_Max = 0;
                                  Led_String(" Confirm",0,0);
                                }
                                break;
                              }
                              else if(by_Eng == _Service_Item_Maintenance)
                              {
                                if(by_Buffer == 0)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 2;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String(" M: ",0,0);
                                  Led_Window(Window_3,6,EEPROM_ReadInformation(EE_Prev_MaintenanceCurrentAccMiles));
                                }
                                else if(by_Buffer==2)
                                {
                                  EngBu[2] = by_Buffer;
                                  _Layer = 4;
                                  by_Min = 1;
                                  by_Max = 3;
                                  _NumberRange = 1;
                                  by_Buffer = by_Min;
                                  Led_String("    -  -   ",0,0);
                                  Led_Window(6,1,EEPROM_ReadInformation(EE_Prev_MaintenancePreviousYear));
                                  Led_Window(34,8,EEPROM_ReadInformation(EE_Prev_MaintenancePreviousMouth));
                                  Led_Window(52,8,EEPROM_ReadInformation(EE_Prev_MaintenancePreviousDay));
                                }
                                else if(by_Buffer==1)
                                {
                                  _Layer = 4;
                                  EngBu[3] = by_Buffer;
                                  by_Min = 0;
                                  by_Max = 0;
                                  Led_String(" Confirm",0,0);
                                }
                                break;
                              }
                              EEPROM_SaveWorkInformation(EngineerMode[by_Eng].EEPRomNum,by_Buffer);
                              BEEP_Proce(4,3,3);
                              // 取出第2层选项数量
                              by_Max = ManageItemName[EngBu[0]-1].ItemRange;
                              // 起始值
                              by_Min = ManageItemName[EngBu[0]-1].ItemTarget;
                              // 调整范围值
                              by_Max += by_Min;
                              _NumberRange = 1;
                              _Layer = 2;
                              break;
                     case 4:
                              if(by_Eng == _Service_Item_RunningBelt)
                              {
                                if(EngBu[3] == 1)
                                {
                                  UINT32 w_miles,w_hours;
                                  w_miles = EEPROM_ReadInformation(EE_RunningBeltPreviousAccMiles) + EEPROM_ReadInformation(EE_RunningBeltCurrentAccMiles);
                                  w_hours = EEPROM_ReadInformation(EE_RunningBeltPreviousAccHours) + EEPROM_ReadInformation(EE_RunningBeltCurrentAccHours);
                                  EEPROM_WriteInformation(EE_RunningBeltPreviousAccMiles,w_miles);
                                  EEPROM_WriteInformation(EE_RunningBeltPreviousAccHours,w_hours);
                                  EEPROM_WriteInformation(EE_RunningBeltCurrentAccMiles,0);
                                  EEPROM_WriteInformation(EE_RunningBeltCurrentAccHours,0);
                                  EEPROM_WriteInformation(EE_RunningBeltPreviousYear,2000+w_TimeBuffer[0]);
                                  EEPROM_WriteInformation(EE_RunningBeltPreviousMouth,w_TimeBuffer[1]);
                                  EEPROM_WriteInformation(EE_RunningBeltPreviousDay,w_TimeBuffer[2]);
                                  _Layer = 3;
                                  by_Min=0;
                                  by_Max=2;
                                  by_Buffer=0;
                                }
                              }
                              else if(by_Eng == _Service_Item_RunningDeck)
                              {
                                if(EngBu[3] == 1)
                                {
                                  UINT32 w_miles,w_hours;
                                  w_miles = EEPROM_ReadInformation(EE_RunningDeckPreviousAccMiles) + EEPROM_ReadInformation(EE_RunningDeckCurrentAccMiles);
                                  w_hours = EEPROM_ReadInformation(EE_RunningDeckPreviousAccHours) + EEPROM_ReadInformation(EE_RunningDeckCurrentAccHours);
                                  EEPROM_WriteInformation(EE_RunningDeckPreviousAccMiles,w_miles);
                                  EEPROM_WriteInformation(EE_RunningDeckPreviousAccHours,w_hours);
                                  EEPROM_WriteInformation(EE_RunningDeckCurrentAccMiles,0);
                                  EEPROM_WriteInformation(EE_RunningDeckCurrentAccHours,0);
                                  EEPROM_WriteInformation(EE_RunningDeckPreviousYear,2000+w_TimeBuffer[0]);
                                  EEPROM_WriteInformation(EE_RunningDeckPreviousMouth,w_TimeBuffer[1]);
                                  EEPROM_WriteInformation(EE_RunningDeckPreviousDay,w_TimeBuffer[2]);
                                  _Layer = 3;
                                  by_Min=0;
                                  by_Max=2;
                                  by_Buffer=0;
                                }
                              }
                              else if(by_Eng == _Service_Item_MCB)
                              {
                                if(EngBu[3] == 1)
                                {
                                  UINT32 w_miles,w_hours;
                                  w_miles = EEPROM_ReadInformation(EE_MCBPreviousAccMiles) + EEPROM_ReadInformation(EE_MCBCurrentAccMiles);
                                  w_hours = EEPROM_ReadInformation(EE_MCBPreviousAccHours) + EEPROM_ReadInformation(EE_MCBCurrentAccHours);
                                  EEPROM_WriteInformation(EE_MCBPreviousAccMiles,w_miles);
                                  EEPROM_WriteInformation(EE_MCBPreviousAccHours,w_hours);
                                  EEPROM_WriteInformation(EE_MCBCurrentAccMiles,0);
                                  EEPROM_WriteInformation(EE_MCBCurrentAccHours,0);
                                  EEPROM_WriteInformation(EE_MCBPreviousYear,2000+w_TimeBuffer[0]);
                                  EEPROM_WriteInformation(EE_MCBPreviousMouth,w_TimeBuffer[1]);
                                  EEPROM_WriteInformation(EE_MCBPreviousDay,w_TimeBuffer[2]);
                                  _Layer = 3;
                                  by_Min=0;
                                  by_Max=2;
                                  by_Buffer=0;
                                }
                              }
                              else if(by_Eng == _Service_Item_Motor)
                              {
                                if(EngBu[3] == 1)
                                {
                                  UINT32 w_miles,w_hours;
                                  w_miles = EEPROM_ReadInformation(EE_MoterPreviousAccMiles) + EEPROM_ReadInformation(EE_MoterCurrentAccMiles);
                                  w_hours = EEPROM_ReadInformation(EE_MoterPreviousAccHours) + EEPROM_ReadInformation(EE_MoterCurrentAccHours);
                                  EEPROM_WriteInformation(EE_MoterPreviousAccMiles,w_miles);
                                  EEPROM_WriteInformation(EE_MoterPreviousAccHours,w_hours);
                                  EEPROM_WriteInformation(EE_MoterCurrentAccMiles,0);
                                  EEPROM_WriteInformation(EE_MoterCurrentAccHours,0);
                                  EEPROM_WriteInformation(EE_MoterPreviousYear,2000+w_TimeBuffer[0]);
                                  EEPROM_WriteInformation(EE_MoterPreviousMouth,w_TimeBuffer[1]);
                                  EEPROM_WriteInformation(EE_MoterPreviousDay,w_TimeBuffer[2]);
                                  _Layer = 3;
                                  by_Min=0;
                                  by_Max=2;
                                  by_Buffer=0;
                                }
                              }
                              else if(by_Eng == _Service_Item_HeadphoneJack)
                              {
                                if(EngBu[3] == 1)
                                {
                                  UINT32 w_hours,w_inserts;
                                  w_hours = EEPROM_ReadInformation(EE_HeadphonePreviousAccHours) + EEPROM_ReadInformation(EE_HeadphoneCurrentAccHours);
                                  w_inserts = EEPROM_ReadInformation(EE_HeadphonePreviousInsertsTimes) + EEPROM_ReadInformation(EE_HeadphoneCurrentInsertsTimes);
                                  EEPROM_WriteInformation(EE_HeadphonePreviousAccHours,w_hours);
                                  EEPROM_WriteInformation(EE_HeadphonePreviousInsertsTimes,w_inserts);
                                  EEPROM_WriteInformation(EE_HeadphoneCurrentAccHours,0);
                                  EEPROM_WriteInformation(EE_HeadphoneCurrentInsertsTimes,0);
                                  EEPROM_WriteInformation(EE_HeadphonePreviousYear,2000+w_TimeBuffer[0]);
                                  EEPROM_WriteInformation(EE_HeadphonePreviousMouth,w_TimeBuffer[1]);
                                  EEPROM_WriteInformation(EE_HeadphonePreviousDay,w_TimeBuffer[2]);
                                  _Layer = 3;
                                  by_Min=0;
                                  by_Max=2;
                                  by_Buffer=0;
                                }
                              }
                              else if(by_Eng == _Service_Item_Maintenance)
                              {
                                if(EngBu[3] == 1)
                                {
                                  UINT32 w_miles,w_hours;
                                  w_miles = EEPROM_ReadInformation(EE_Prev_MaintenancePreviousAccMiles) + EEPROM_ReadInformation(EE_Prev_MaintenanceCurrentAccMiles);
                                  w_hours = EEPROM_ReadInformation(EE_Prev_MaintenancePreviousAccHours) + EEPROM_ReadInformation(EE_Prev_MaintenanceCurrentAccHours);
                                  EEPROM_WriteInformation(EE_Prev_MaintenancePreviousAccMiles,w_miles);
                                  EEPROM_WriteInformation(EE_Prev_MaintenancePreviousAccHours,w_hours);
                                  EEPROM_WriteInformation(EE_Prev_MaintenanceCurrentAccMiles,0);
                                  EEPROM_WriteInformation(EE_Prev_MaintenanceCurrentAccHours,0);
                                  EEPROM_WriteInformation(EE_Prev_MaintenancePreviousYear,2000+w_TimeBuffer[0]);
                                  EEPROM_WriteInformation(EE_Prev_MaintenancePreviousMouth,w_TimeBuffer[1]);
                                  EEPROM_WriteInformation(EE_Prev_MaintenancePreviousDay,w_TimeBuffer[2]);
                                  _Layer = 3;
                                  by_Min=0;
                                  by_Max=2;
                                  by_Buffer=0;
                                }
                              }
                              break;
                   }
                   Led_Set_Reset();
                   i = 0;
                   break;
          case K_BACK:  
                   Engineer.NumKeyStart=0;
                   i = 0;
                   Led_Set_Reset();
                   switch(_Layer)
                   {
                       case 1:// 回原头选项
                              by_Status = M_NEXT;
                              break;
                       case 2:// 回第一层
                              _Layer = 1;
                              by_Eng = EngBu[0];
                              by_Min = _Engineer_Item+1;
                              by_Max = _Service_Item;
                              break;
                       case 3:
                              if(by_Eng == _Service_Item_ClubID && _SegDigits != 1)
                              {
                                  by_Buffer = _NumberKeyBuffer;
                                  _SegDigits = 1;
                                  Led_String("ID :      ",0,0);
                                  break;
                              }
                              _Layer = 2;
                              _NumberRange = 1;
                              // 取出第2层选项数量
                              by_Max = ManageItemName[EngBu[0]-1].ItemRange;
                              // 起始值
                              by_Min = ManageItemName[EngBu[0]-1].ItemTarget;
                              // 调整范围值
                              by_Max += by_Min;
                              Engineer.UseNumKey = 0;
                              _SegDigits = 1;
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              break;
                       case 4:
                              _Layer = 3;
                              if(by_Eng == _Service_Item_Error)
                              {
                                  by_Min=0;
                                  by_Max=1;
                                  by_Buffer=0;
                              }
                              else if(by_Eng == _Service_Item_RunningBelt||by_Eng == _Service_Item_RunningDeck||\
                                by_Eng == _Service_Item_MCB||by_Eng == _Service_Item_Motor||\
                                  by_Eng == _Service_Item_HeadphoneJack||by_Eng == _Service_Item_Maintenance)
                              {
                                by_Min=0;
                                by_Max=2;
                                by_Buffer=0;
                              }
                              break;
                   }
                   break;
      }
      if(Timer_Counter(T_STEP,3,1200)) by_Status = M_RESET;
      if(!Check_SafeKey()) by_Status = M_RESET;
  }
  return by_Status;
}

/*******************************************************************************
* Function Name  : Test_Mode()
* Input          : None
* Output         : None
* Return         : by_Status
*******************************************************************************/
char EngineerMode_Test(void)
{
  unsigned char  by_Status = M_NONE;
  unsigned char  by_Buffer = 1;
  unsigned char  by_Test = 1;
  unsigned char  by_Test1 = 1;
  unsigned short _KeyBuffer;
  unsigned char by_String[19] = {0};
  unsigned char CountDownTime[5] = {0,5,10,30,60};
  unsigned char by_Select = 0;
  
  Engineer.key = 0;
  
  Led_Set_Target(Display_OFF,0);
  if((Digital_GetStatus() & 0x03) != 0)// S003-01
  {// 升降有动作时就需要强制停止
      Digital_Command(CmdSetInclineAction,0);//==>升降停止    
  }
  Timer_Counter_Clear(3);
  while(by_Status == M_NONE)
  {
      
      if(Engineer.key == 0)
          Led_String((unsigned char *)TestModeItem[by_Buffer-1].ItemName,0,Show_Auto);
      else
      {
          switch(by_Buffer)
          {
              case _Test_ERP:
                       sprintf((char*)by_String, by_Select == 0? "Auto" : "%2d", CountDownTime[by_Select]);
                       Led_String(by_String, 0, Show_Auto);
                       break;
              case _Test_Keypad:
                       if(_KeyBuffer != K_NONE)
                       {
                           if(_KeyBuffer <= 40)
                           {
                               Led_String((unsigned char *)KeypadTestItem[_KeyBuffer-1].ItemName,0,Show_Auto);
                           }
                       }
                       break;
              case _Test_CSAFE:
                       switch(by_Test)
                       {
                           case 1:// CSAFE测试开始
                                  if(Timer_Counter(T_STEP,1,10))
                                  {
                                      if(CSAFE_Get_EngTestStatus() == 1)
                                      {// Return OK
                                          by_Test++;
                                      }
                                      else
                                      {
                                          if(by_Test1 > 2) 
                                          {// Fail
                                              by_Test++;
                                              CSAFE_Set_EngTest(0);
                                          } 
                                          else
                                          {// Retry
                                              CSAFE_Set_EngTest(1);
                                              CSAFE_EngTest();
                                              by_Test1++;
                                          }  
                                          Timer_Counter_Clear(1);
                                      }
                                  }
                                  break;
                           case 2:// CSAFE测试判定
                                  if(by_Test1 > 2)
                                      Led_String("CSAFE FAIL",0,Show_Auto);
                                  else
                                      Led_String("CSAFE OK",0,Show_Auto);
                                  break;
                       }
                       break;
                       
              case _Test_WiFi:                       
                       switch(by_Test)
                       {
                       case 1:// Hold 2 seconds
                              if(Timer_Counter(T_STEP,1,20))
                              {
                                   by_Test=2;
                                   Led_Set_Reset();
                              }
                              break;
                       case 2:
                              Led_String("WiFi Connect To AP,Please Wait",0,Show_Auto);
                              if(Led_Get_String_End() == 1)
                              {
                                  by_Test=3;
                                  ResetWiFi();
                                  Timer_Counter_Clear(1);
                                  Timer_Counter_Clear(3);
                              }
                              break;
                       case 3:// Hold 40 seconds
                              Led_String("WiFi Connect To AP,Please Wait",0,Show_Auto); 
                              if(Timer_Counter(T_STEP,1,400))
                              {
                                  Timer_Counter_Clear(1);
                                  Timer_Counter_Clear(3);
                                  by_Test=4;
                              }
                              break;
                       case 4:// Check Start And Hold 150 seconds
                              if(Wifi_Get_EnggineTestResult() == 0)
                                  Led_String("WiFi OK",0,Show_Auto);
                              else if(Timer_Counter(T_STEP,1,1500))
                                  by_Test=5;
                              else
                              {
                                Timer_Counter_Clear(3);
                                Led_String("WiFi Connect To AP,Please Wait",0,Show_Auto);
                              }
                              break;
                       case 5:
                              switch(Wifi_Get_EnggineTestResult())
                              {
                                  case 0:
                                         Led_String("WiFi OK",0,Show_Auto);
                                         break;
                                  case 1:
                                         Led_String("Not Have MAC Address",0,Show_Auto);
                                         break;
                                  case 2:
                                         Led_String("Not Have IP Address",0,Show_Auto);
                                         break;
                              }
                              break;
                              
                       }                       
                       break;                       
              case _Test_HJS:
                       if(IO_HJS() == 1)
                       {
                           Led_String("Plug In",0,Show_Auto);
                           Timer_Counter_Clear(1);
                       }
                       else
                       {
                           if(Timer_Counter(T_STEP,1,3))
                           {
                               Led_String("Plug Out",0,Show_Auto);
                           }
                       }
                       break;                       
          }
      }
      _KeyBuffer = Key_Proce();
      if(_KeyBuffer != K_NONE)
      {
          Timer_Counter_Clear(3);
      } 
      Engineer.key1 = D_DEC;
      switch(_KeyBuffer)
      {
          case K_SpeedU | K_MASK:
          case K_SpeedU:          
          case K_TimeU | K_MASK:
          case K_TimeU:         
          case K_InclineU | K_MASK:
          case K_InclineU:   
                   Engineer.key1 = D_ADD;
          
          case K_SpeedD | K_MASK:
          case K_SpeedD:             
          case K_InclineD | K_MASK:         
          case K_InclineD: 
          case K_TimeD | K_MASK:
          case K_TimeD:           
                   if(Engineer.key == 0)
                   {
                       if(Engineer.key1 == D_ADD)
                       {
                           if(by_Buffer < _Test_Item) 
                               by_Buffer++;
                           else
                               by_Buffer = 1;
                       }
                       else
                       {
                           if(by_Buffer == 1)
                               by_Buffer = _Test_Item;
                           else
                               by_Buffer--;
                       }
                       Led_Set_Reset();
                   }
                   else
                   {
                       if(by_Buffer == _Test_ERP)
                       {
                           by_Select=Keyboard_DFCount(Engineer.key1,0,4,1,by_Select);
                           /*
                           if(Engineer.key1 == 1)
                           {
                               by_Select++;
                               if(by_Select >= 5) by_Select=0;
                           }
                           else
                           {
                               if(by_Select == 0)
                                   by_Select=4;
                               else 
                                   by_Select--;
                           }
                           */
                       }
                   }
                   break;
          case K_ENTER:
                   
                   switch(by_Buffer)
                   {
                       case _Test_ERP:
                                if(Engineer.key == 1)
                                {
                                    by_Status = LCB_QuickERP(CountDownTime[by_Select]);
                                    Timer_Counter_Clear(3);
                                }
                                break; 
                       case _Test_Display:
                                if(Engineer.key == 0) by_Test = 0;
                                if(by_Test >= 15) 
                                {// 清显示
                                    by_Test = 0;
                                    Led_Set_Target(Display_OFF,1);
                                } 
                                else
                                {
                                    by_Test++;
                                    Led_DisplayTest(by_Test);
                                }
                                break;
                       case _Test_CSAFE:
                                CSAFE_Set_EngTest(1);
                                CSAFE_EngTest();
                                Led_String("CSAFE Test",0,Show_Auto);
                                Timer_Counter_Clear(1);
                                by_Test = 1;
                                by_Test1 = 1;
                                break;                                
                       case _Test_WiFi:
                                WIFIClearStateBuffer();
                                ConnectAPForFactoryDefault();
                                Led_String("WiFi Test",0,Show_Auto);
                                Timer_Counter_Clear(1);
                                break;                                
                       case _Test_HJS:
                                if(IO_HJS() != 1)
                                {
                                    Led_String("Plug Out",0,Show_Auto);
                                }
                                break;
                   }
                   Engineer.key = 1;
                   break;
          case K_BACK:
                   if(Engineer.key == 1)
                   {
                       Engineer.key = 0;    
//                       if(by_Buffer == _Test_WiFi)
//                       {
//                           RestoreWifiToFactoryDefaults();
//                           Timer_Counter_Clear(1);
//                           while(Timer_Counter(T_STEP,1,30) == 0)
//                           {
//                               Show_Message(Message_Activated,0,0,Show_Auto);
//                               //Led_String("Activated",0,0);
//                           }
//                       }      
                       
                       by_Test = 1;
                       by_Test1 = 1;                       
                       Led_Set_Target(Display_OFF,0);
                       Led_Set_Reset();
                   }
                   else 
                       by_Status = M_NEXT;
                   break; 
                   
      }
      // WiFi 联机状态
      WIFISignalStrength();
      //
      if(!Check_SafeKey()||Timer_Counter(T_STEP,3,1200)) by_Status = M_RESET;
  }
  return by_Status;
}
/*******************************************************************************
* Function Name  : EngineerMode_Accumulated()
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
char EngineerMode_Accumulated(char by_D)
{
  unsigned char  by_Status = M_NONE;
  unsigned char  _Seg = 1;
  unsigned short _KeyBuffer;
  unsigned long  by_Total = 0;
    
    Engineer.key = 0;
    Engineer.key1 = 0;
    if(by_D == 0)
    {
        Led_String("Dis.      ",0,0);
        if(Console_SpeedUnit(Data_Get,0) == C_KM)
            by_Total = (u32)((float)EEPROM_ReadInformation(EE_ACCUMULATED_DISTANCE) * 1.609) / 1000;
        else
            by_Total = EEPROM_ReadInformation(EE_ACCUMULATED_DISTANCE) / 1000;
    }
    else
    {
        Led_String("Time      ",0,0);
        by_Total = EEPROM_ReadInformation(EE_ACCUMULATED_TIME) / 3600;
    }
    Led_Window(27,6,by_Total);
    Timer_Counter_Clear(3);
    while(by_Status == M_NONE)
    {
        if(Engineer.key1 == 1)
            Led_Window(27,6,by_Total);
        else if(Timer_Counter(T_STEP,0,30))//==> 亮个3秒钟
        {
            Engineer.key1 = 1;
            by_Total = 0;
        }
       _KeyBuffer = Key_Proce();
       if((_KeyBuffer >= K_0 && _KeyBuffer <= K_9)&&(_Seg <= 6))
       {
           Engineer.key = 1;
           if(_Seg == 1)
           {
               Engineer.key1 = 1;
               if(_KeyBuffer != K_0)
               {
                   by_Total = _KeyBuffer - K_0;
                   _Seg++;
               }    
           }
           else
           {
               by_Total *= 10;
               by_Total += _KeyBuffer - K_0;
               _Seg++;
           }
       }
       if(_KeyBuffer != K_NONE)
       {
           Timer_Counter_Clear(3);
       }
        switch(_KeyBuffer)
        {
            case K_ENTER:
                     if(Engineer.key == 1)
                     {
                         if(by_D == 0)
                         {// Total Distance
                             if(Console_SpeedUnit(Data_Get,0) == C_KM)
                             {
                                 if(by_Total != 0) by_Total = (u32)((float)by_Total * 0.62137) + 1; 
                             }
                             EEPROM_AccumulatedMode(1);// 特殊储存用模式
                             EEPROM_SaveWorkInformation(EE_ACCUMULATED_DISTANCE,by_Total * 1000); 
                         }
                         else
                         {// Total Time
                             EEPROM_AccumulatedMode(2);// 特殊储存用模式
                             EEPROM_SaveWorkInformation(EE_ACCUMULATED_TIME,by_Total * 3600); 
                         }  
                         BEEP_Proce(4,3,3);
                     }
                     by_Status = M_NEXT;  
                     break;   
            case K_BACK:
                     if(Engineer.key == 1)
                     {
                         _Seg = 1;
                         by_Total = 0;
                         Engineer.key = 0;
                     }
                     else 
                         by_Status = M_NEXT;
                     break;
        }
        if(!Check_SafeKey() || Timer_Counter(T_STEP,3,1200)) by_Status = M_RESET;
    }
    Timer_Counter_Clear(3);
    return by_Status;
}
/*******************************************************************************
* Function Name  : EngineerMode_SerialNumber()
* Input          : None
* Output         : None
* Return         : by_Status
*******************************************************************************/
char EngineerMode_SerialNumber(void) 
{// 仪表与车架流水序号输入子程序
  unsigned char  by_Status = M_NONE;
  unsigned char  _Item = 1;
  unsigned char  _SerialMode = 1;// Console
  unsigned char  _Version ;
  unsigned char  by_Seg = 0;
  unsigned short  by_GetKeyNumber = 0;
  unsigned long  by_Number = 0;
  unsigned long  by_Buffer = 0;
  unsigned long  _Multiplier = 100000000;
  
  Engineer.key=0;
  Engineer.key1=0;
  Engineer.key2=0;
  
  while(by_Status == M_NONE)
  {
      switch(_Item)
      {
          case 1:// 选择设定项目
                 if(_SerialMode == 1 )
                     Led_String("Console",0,Show_Auto);
                 else
                     Led_String("Frame",0,Show_Auto);
                 break;
          case 2:// 设定版本
                 if( _Version > 0 )// 第B版开始显示对应码
                     Led_NumberSite(45,_Version + 10);//秀出对应字型
                 else// 第A版机型码不秀直接空格
                     Led_NumberSite(45,127);//秀出对应字型// 空格
                 
                 break;
          case 3:// 设定序号
                 if(!Engineer.key1 && by_Seg == 1)
                 {
                     Engineer.key1=1;
                 }
                 if(by_Seg > 0) Engineer.key2=1;
                 if(Engineer.key)
                 {
                     Engineer.key=0;
                     if(by_Seg > 9) 
                     {
                         by_Seg=9;
                     }
                     else
                     {
                         Led_NumberSite((by_Seg * 6),by_Number);
                         by_Number *= _Multiplier;
                         _Multiplier /= 10;
                         by_Buffer+=by_Number;
                     }            
                 }
                 break;
      }           
      by_GetKeyNumber = Key_Proce();
      if(_Item == 3 && (by_GetKeyNumber >= K_0 && by_GetKeyNumber <= K_9))
      {// 设定序号取码
          Engineer.key = 1;  
          by_Seg++;          
          by_Number = by_GetKeyNumber - K_0;   
          if(by_Seg == 1)
          {
              for(u8 i=2;i<10;i++)
              {
                  Led_NumberSite((i * 6),0);
              }
          }
      }
      if(by_GetKeyNumber != K_NONE)
       {
           Timer_Counter_Clear(3);
       }
      switch(by_GetKeyNumber)
      {
          case K_BACK:
                     if(Engineer.key2 == 1)
                     {
                         Engineer.key2 = 0;
                         if( _SerialMode == 1 )
                         {// Console
                             by_Buffer = EEPROM_ReadInformation(EE_SerialNumber);
                         }
                         else
                         {// Frame
                             by_Buffer = EEPROM_ReadInformation(EE_FrameSerialNumber);
                         }
                         _Multiplier = 100000000;
                         for(u8 i=1;i<10;i++)
                         {
                             if(i < 9)
                             {
                                 Led_NumberSite((i * 6),by_Buffer/_Multiplier);
                                 if(i < 8)
                                 {
                                     by_Buffer %= _Multiplier;
                                     _Multiplier /= 10;
                                 }
                             }
                             else
                             {
                                 Led_NumberSite((9 * 6),by_Buffer%10);
                             }
                         }
                         _Multiplier = 100000000;
                         by_Buffer = 0;
                         by_Seg = 0;
                         Engineer.key1 = 0;
                     }
                     else by_Status = M_NEXT;
                     break;
          case K_ENTER: 
                   if( _Item == 1 )
                   {
                       if( _SerialMode == 1 )
                       {// Console
                           _Version = EEPROM_ReadInformation(EE_ConsoleVersion);
                           Led_String("  TM706 ",0,0);
                       }
                       else
                       {// Frame
                           _Version = EEPROM_ReadInformation(EE_FrameVersion);
                           Led_String("  TM522 ",0,0);
                       }
                   }
                   else if( _Item == 2 )
                   {
                       if( _SerialMode == 1 )
                       {// Console
                           EEPROM_SaveWorkInformation(EE_ConsoleVersion,_Version);
                           by_Buffer = EEPROM_ReadInformation(EE_SerialNumber);
                       }
                       else
                       {// Frame
                           EEPROM_SaveWorkInformation(EE_FrameVersion,_Version);
                           by_Buffer = EEPROM_ReadInformation(EE_FrameSerialNumber);
                       }
                       BEEP_Proce(4,3,3);
                       Led_String("          ",0,0);
                       _Multiplier = 100000000;
                       for(u8 i=1;i<10;i++)
                       {
                           if(i < 9)
                           {
                               Led_NumberSite((i * 6),by_Buffer/_Multiplier);
                               if(i < 8)
                               {
                                   by_Buffer %= _Multiplier;
                                   _Multiplier /= 10;
                               }
                           }
                           else
                           {
                               Led_NumberSite((9 * 6),by_Buffer%10);
                           }
                       }
                       by_Buffer = 0;
                   }
                   else if( _Item == 3 )
                   {
                       //Engineer.key2 = 0;
                       if(Engineer.key2 == 1)
                       {
                           if( _SerialMode == 1 )
                           {// Console
                               EEPROM_SaveWorkInformation(EE_SerialNumber,by_Buffer);
                           }
                           else
                           {// Frame    
                               EEPROM_SaveWorkInformation(EE_FrameSerialNumber,by_Buffer);
                           }
                           BEEP_Proce(4,3,3);
                       }
                       by_Status = M_NEXT;
                   }
                   if( _Item <= 3 ) _Item++;
                   _Multiplier = 100000000;
                   break;
          case K_SpeedU:
          case K_InclineU:  
          case K_TimeU:
                   if( _Item == 1 )
                   {
                       if(_SerialMode == 1)
                           _SerialMode = 2;
                       else
                           _SerialMode = 1;
                   }
                   else if( _Item == 2 )
                   {
                       _Version++;
                       if( _Version > 25 ) _Version = 0;
                   }
                   break;
          case K_SpeedD:
          case K_InclineD:  
          case K_TimeD:
                   if( _Item == 1 )
                   {
                       if(_SerialMode == 1 )
                           _SerialMode = 2;
                       else
                           _SerialMode = 1;
                   }
                   else if( _Item == 2 )
                   {
                       if(_Version == 0)
                           _Version = 25;
                       else
                           _Version--;
                   }
                   break;
          case K_PAUSE | K_MASK:
                   by_Status=M_NEXT;
                   break;          
      }
      if(!Check_SafeKey() || Timer_Counter(T_STEP,3,1200)) by_Status=M_RESET;
  }
  Timer_Counter_Clear(3);
  return by_Status;
}
/*******************************************************************************
* Function Name  : EngineerMode_AutoCalibration()
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EngineerMode_AutoCalibration(void)
{
    unsigned char  by_Status=M_NONE;
    unsigned char  by_AutoCheck=1;
    unsigned char  by_AutoCheckNumber=0;
    unsigned short Old_ADC=0;
    
    Timer_Counter_Clear(0);
    Timer_Counter_Clear(1);
    
	//solve Auto Calibration incline reverse issue
    Digital_Command(CmdSetInclineLocation,Console_HighLowADC(Data_Get,0,0));//==>升降设定
    if((Digital_GetStatus() & 0x03) != 0)// S003-01
    {
        //==>> 先让升降停止动作,确保RELAY的动作正确 
        Digital_Command(CmdSetInclineAction,0);//==>升降停止
        while(Timer_Counter(T_STEP,0,20) == 0) ;
        Timer_Counter_Clear(0);
        // <=======================
    }
    Digital_Command(CmdSetInclineAction,1);//==>升降上升
    while(by_Status == M_NONE)
    {
        switch(by_AutoCheck)
        {
            case 1:if(Timer_Counter(T_STEP,1,1))
                   {
                       Digital_Command(CmdGetInclineLocation,0);
                       Timer_Counter_Clear(1);
                   }    
                   Led_Window(Window_2,1,Digtial_Get_InclineLocation()/128);
                   if(Timer_Counter(T_STEP,0,20)) 
                   {
                       if(Digtial_Get_InclineLocation() > Old_ADC+(2*128))
                       {
                           Old_ADC=Digtial_Get_InclineLocation();
                       }
                       else
                       {
                           by_AutoCheckNumber++;
                           if(by_AutoCheckNumber > 2)
                           {
                               if(Digtial_Get_InclineLocation() > Console_HighLowADC(Data_Get,0,0)+(120*128))
                               {//==>确保范围值
                                   Console_HighLowADC(Data_Set,1,Digtial_Get_InclineLocation()-12*128);//(12*128)
                                   Led_String("  LOW     ",0,0);
                                   Led_Window(Window_2,1,Digtial_Get_InclineLocation()/128);
                                   by_AutoCheck=2;
                                   by_AutoCheckNumber=0;
                               }
                               else
                               {
                                   by_AutoCheck=10;
                                   Led_Set_Reset();  
                               }   
                               Digital_Command(CmdSetInclineAction,0);//==>升降停止
                           }
                       }
                       Timer_Counter_Clear(0);
                   }
                   break;
            case 2:if(Timer_Counter(T_STEP,0,30))
                   {
                       by_AutoCheck=3;
                       Timer_Counter_Clear(0); 
                       Timer_Counter_Clear(1); 
                       Digital_Command(CmdSetInclineAction,0xff);//==>升降下降
                   }
                   break;  
            case 3:if(Timer_Counter(T_STEP,1,1))
                   { 
                       Digital_Command(CmdGetInclineLocation,0);
                       Timer_Counter_Clear(1);
                   }    
                   Led_Window(Window_2,1,Digtial_Get_InclineLocation()/128);
                   
                   if(Timer_Counter(T_STEP,0,20)) 
                   {
                       if(Digtial_Get_InclineLocation() < Old_ADC-(2*128))
                       {
                           Old_ADC=Digtial_Get_InclineLocation();
                       }
                       else
                       {
                           by_AutoCheckNumber++;
                           if(by_AutoCheckNumber > 2)
                           {
                               if(Digtial_Get_InclineLocation() < Console_HighLowADC(Data_Get,1,0)-(120*128))
                               {//==>确保范围值
                                   Console_HighLowADC(Data_Set,0,Digtial_Get_InclineLocation()+13*128);//(9*128)
                                   Led_Window(Window_2,1,Digtial_Get_InclineLocation()/128);
                                   EEPROM_SaveWorkInformation(EE_LOW_ELEVATION,Console_HighLowADC(Data_Get,0,0));
                                   EEPROM_SaveWorkInformation(EE_MAX_ELEVATION,Console_HighLowADC(Data_Get,1,0));
                                   by_AutoCheck=9;
                               }
                               else
                               {
                                   by_AutoCheck=10;
                                   Led_Set_Reset();  
                               }   
                               Led_Set_Reset();  
                               Digital_Command(CmdSetInclineAction,0);//==>升降停止
                           }
                       }
                       Timer_Counter_Clear(0);
                   }
                   break;
            case 9://==>校正OK
            case 10:
                   if(by_AutoCheck == 9)
                       Led_String("Auto Calibration PASS",0,Show_Auto);
                   else 
                       Led_String("Auto Calibration FAIL",0,Show_Auto);
                   if(Led_Get_String_End() == 1) 
                   {
                       by_Status=M_RESET;  
                       Digital_CommandBufferReset();//==>重置下控  
                       Timer_Counter_Clear(1);
                       while(!Timer_Counter(T_STEP,1,10)) ;
                   }   
                   break;                        
        }
        if(!Check_SafeKey()) by_Status=M_RESET;
    }    
}
/*******************************************************************************
* Function Name  : EngineerMode_RTCSysTime()
* Description    : 设定仪表系统上的时间IC用 (年-月-日-小时-分-秒)
* Input          : 0=Date / 1=Time
* Output         : None
* Return         : None
*******************************************************************************/
void EngineerMode_RTCSysTime(char by_D)
{
  unsigned char  w_TimeBuffer[6];
  unsigned char  by_Status=M_NONE;
  unsigned char  by_Number=0;
  unsigned char  by_Seg=0;
  unsigned char  by_KeyData=0;
  unsigned char  by_Bu = 0; 
  
  Engineer.key=0;
  Engineer.key1=0;
  Engineer.key2=1;
  Show_Message(Message_Blank,0,0,Show_Blank);
  SHT1381 time;//==>HT1381时间IC Data指向
  ReadHT1381(&time);
  by_Bu = time.Seconds / 16 * 10;
  w_TimeBuffer[5] = by_Bu + (time.Seconds % 16);
  by_Bu = time.Minutes / 16 * 10;
  w_TimeBuffer[4] = by_Bu + (time.Minutes % 16);
  by_Bu = time.Hours / 16 * 10;
  w_TimeBuffer[3] = by_Bu + (time.Hours % 16);
  by_Bu = time.Date / 16 * 10;
  w_TimeBuffer[2] = by_Bu + (time.Date % 16);
  by_Bu = time.Month / 16 * 10;
  w_TimeBuffer[1] = by_Bu + (time.Month % 16);
  by_Bu = time.Year / 16 * 10;
  w_TimeBuffer[0] = by_Bu + (time.Year % 16);
    
  if(by_D == 0)
      Led_String("          ",0,0);
  else
  {
      by_Seg = 10;
      Led_String("          ",0,0);  
  }
  Timer_Counter_Clear(0);
  Timer_Counter_Clear(3);
  while(by_Status == M_NONE)
  {
      //============== Process Key Data ==============================
      by_KeyData = Key_Proce();
      //if(by_KeyData != K_NONE) 
      //    Timer_Counter_Clear(0);
      if(by_KeyData >= K_0 && by_KeyData <= K_9)
      {//==> K_0~K_9
          if(by_Seg == 10) by_Seg = 0;
          if(by_Seg < 6)
          {
              Engineer.key = 1;
              Engineer.key1 = 1;
              Engineer.key2 = 0;
              if(by_Seg != 0 && by_Seg % 2 == 0)
              {
                  by_Number = 0;
              }
              if(by_Number != 0)
              {
                  by_Number *= 10;
              }
              by_Number += (by_KeyData - K_0);    
          }
      }
      if(by_KeyData != K_NONE)
      {
          Timer_Counter_Clear(3);
      }
      switch(by_KeyData)
      {
          case K_BACK:
                   if(Engineer.key1 == 1)
                   {
                       by_Number = 0;  
                       if(by_Seg > 2)
                       {
                          by_Seg -= 2;
                          if(by_Seg % 2 != 0) by_Seg--;
                       }
                       else 
                       {
                           by_Seg = 0;
                           Engineer.key1 = 0;
                       }
                   }
                   else 
                       by_Status=M_NEXT;  
                   break;
          case K_ENTER:
                   if(Engineer.key1 == 1)
                   {
                       Engineer.key1 = 0;
                       Engineer.key2 = 1;
                       by_Bu = w_TimeBuffer[5] / 10 * 16;
                       time.Seconds = by_Bu + (w_TimeBuffer[5] % 10);
                       by_Bu = w_TimeBuffer[4] / 10 * 16;
                       time.Minutes = by_Bu + (w_TimeBuffer[4] % 10);
                       by_Bu = w_TimeBuffer[3] / 10 * 16;
                       time.Hours = by_Bu + (w_TimeBuffer[3] % 10);
                       by_Bu = w_TimeBuffer[2] / 10 * 16;
                       time.Date = by_Bu + (w_TimeBuffer[2] % 10);
                       by_Bu = w_TimeBuffer[1] / 10 * 16;
                       time.Month = by_Bu + (w_TimeBuffer[1] % 10);
                       by_Bu = w_TimeBuffer[0] / 10 * 16;
                       time.Year = by_Bu + (w_TimeBuffer[0] % 10);
                       WriteHT1381(&time);
                       BEEP_Proce(4,3,3);
                       by_Seg = 7;
                   }
                   else
                       by_Status=M_NEXT;
                   break;
      }
      //===============================================================
      if(Engineer.key && by_Seg <= 6)
      {
          Engineer.key=0;
          if(by_D == 0)
              w_TimeBuffer[by_Seg/2] = by_Number;
          else
              w_TimeBuffer[(by_Seg/2)+3] = by_Number;
          if(w_TimeBuffer[1] > 12) w_TimeBuffer[1]=12;     // 月
          else if(w_TimeBuffer[2] > 31) w_TimeBuffer[2]=31;// 日
          else if(w_TimeBuffer[3] > 23) w_TimeBuffer[3]=23;// 小时
          else if(w_TimeBuffer[4] > 59) w_TimeBuffer[4]=59;// 分
          else if(w_TimeBuffer[5] > 59) w_TimeBuffer[5]=59;// 秒
          by_Seg++;
      }
      if((by_Seg == 10 || Engineer.key2 == 1) && Timer_Counter(T_LOOP,0,5))
      {
          ReadHT1381(&time);
          by_Bu = time.Seconds / 16 * 10;
          w_TimeBuffer[5] = by_Bu + (time.Seconds % 16);
          by_Bu = time.Minutes / 16 * 10;
          w_TimeBuffer[4] = by_Bu + (time.Minutes % 16);
          by_Bu = time.Hours / 16 * 10;
          w_TimeBuffer[3] = by_Bu + (time.Hours % 16);
          by_Bu = time.Date / 16 * 10;
          w_TimeBuffer[2] = by_Bu + (time.Date % 16);
          by_Bu = time.Month / 16 * 10;
          w_TimeBuffer[1] = by_Bu + (time.Month % 16);
          by_Bu = time.Year / 16 * 10;
          w_TimeBuffer[0] = by_Bu + (time.Year % 16);
      }
      Led_RTCTime(w_TimeBuffer,by_D,by_Seg/2);
      if(!Check_SafeKey() || Timer_Counter(T_STEP,3,1200)) by_Status = M_RESET;
  }
  Timer_Counter_Clear(3);    
}
/*******************************************************************************
* Function Name  : RunningTestMode
*                  一般测试模式 (Incline , Time , Heart Rate , Speed)
* Input          : None
* Output         : None
*******************************************************************************/                                         
void EngineerMode_RunningTestMode(void)
{
  unsigned char  by_Status = M_NONE;
  unsigned short  by_TBuffer = 0;
  unsigned short  by_KeyNumber;//==>按键代码
  
  Engineer.key = 0;
  Engineer.UseNumKey=0;
  Console_Initial();
  Show_Message(Message_Blank,0,0,Show_Blank);
  Console_Initial();
  Show_Incline();
  if(Console_SpeedUnit(Data_Get,0) == C_KM)
      Console_StartSpeed(Data_Set,8);
  else
      Console_StartSpeed(Data_Set,5);
  Show_Speed();
//  Led_Window(Window_2,1,Heart_Get_Heart());
  Led_Set_Target(Target_TimeElapsed,1);
  Led_7SEG(2,(by_TBuffer/60*100)+(by_TBuffer%60));
  
  Heart_Start();
  Timer_Close(0);
  Timer_Close(1);
  Digital_Set_Work(1);//==>在运动模式下
  
  while(by_Status == M_NONE)
  {
      Heart_Decoder();
      if(Timer_Counter(T_STEP,0,10))
      {
          by_TBuffer++;
          if(by_TBuffer > 6000) by_TBuffer = 0;// 100分钟时需归零
          Led_7SEG(2,(by_TBuffer/60*100)+(by_TBuffer%60));
//          Led_Window(Window_2,1,Heart_Get_Heart());
          Timer_Counter_Clear(0);
          if(Engineer.UseNumKey == 0) Show_Speed();
      }
      if(Engineer.UseNumKey && Timer_Counter(T_STEP,1,50))
      {//==>当设定参数后约5秒如果没确认设定的话会自动恢复参数
          Engineer.UseNumKey = 0;
          //Key_Number(K_ClearNumber,0);//==>清除内部按键值
          Keyboard_NumberCount(3,K_BACK);
          Show_Speed();
      }  
      //if(Engineer.key == 0)
          by_KeyNumber = Key_Proce();
      //else   
      //    by_KeyNumber = Key_Number(DF_WorkSpeed,0);
      switch(by_KeyNumber)
      {
          
          case K_GO:
                   if(Engineer.key == 0 && !Digital_Get_RPMStatus())
                   {//==>马达停止转动
                       Engineer.key = 1;
                       Console_WorkoutStatus(Data_Set,1);
                       Digital_Set_AutoCheck(0);//==>离开自动更正模式
                       Digital_Set_DigitalSpeedTarget(1);//==>开始追击目标速度
                       if(Digtial_ElevationVariationCheck(0) == 1)
                       {// 当升降实际值与目标值误差正负,就清除之前所设定的目标值参数然后在重新取得
                           Digital_Clear_ElevationOld();//==>先清除旧的AD暂存值
                           //Console_Incline(Data_Set,0); 
                           Console_Information(Info_WorkoutIncline,Data_Set,0);
                       }
                       Timer_Counter_Clear(0);
                   }
                   break;
                   
          case K_STOP: 
                   Digital_Initial();
                   Digital_Set_DigitalSpeedTarget(0);//==>速度归零
                   if((Digital_GetStatus() & BIT0)||(Digital_GetStatus() & BIT1))
                   {// 升降有动作时就需要强制停止
                       Digital_Command(CmdSetInclineAction,0);//==>升降停止    
                   }
                   by_Status = M_NEXT;
                   break;
          case K_SpeedU|K_MASK: 
          case K_SpeedU:    
          case K_SpeedD|K_MASK:
          case K_SpeedD:  
                   if(Engineer.key == 1)
                   {
                       // 有按数字键下需要取消
                       if(Engineer.UseNumKey == 1)
                       {
                           Engineer.UseNumKey = 0;
                           //Key_Number(K_ClearNumber,0);//==>清除内部按键值
                       }
                       //
                       if(by_KeyNumber == K_SpeedU || by_KeyNumber == (K_SpeedU|K_MASK))
                           //Console_Speed(Data_Add,1);//==>SPEED调整每次预设0.1
                           Console_Information(Info_WorkoutSpeed,Data_Add,1);
                       else
                           //Console_Speed(Data_Dec,1);//==>SPEED调整每次预设0.1
                           Console_Information(Info_WorkoutSpeed,Data_Dec,1);
                       Show_Speed();
                   }
                   break;
          case K_InclineU:
          case K_InclineU|K_MASK:
          case K_InclineD:
          case K_InclineD|K_MASK:  
                   if(Engineer.key == 1)
                   {
                       // 有按数字键下需要取消
                       if(Engineer.UseNumKey == 1)
                       {
                           Engineer.UseNumKey = 0;
                           //Key_Number(K_ClearNumber,0);//==>清除内部按键值
                       }
                       //
                       if(by_KeyNumber == K_InclineU || by_KeyNumber == (K_InclineU|K_MASK))
                           //Console_Incline(Data_Add,0);
                           Console_Information(Info_WorkoutIncline,Data_Add,0);
                       else
                           //Console_Incline(Data_Dec,0); 
                           Console_Information(Info_WorkoutIncline,Data_Dec,0); 
                       Show_Incline();
                   }
                   break;
          case K_0:
          case K_1:
          case K_2:
          case K_3:
          case K_4:
          case K_5:
          case K_6:
          case K_7:
          case K_8:
          case K_9:
                   if(Engineer.key == 1)
                   {
                       //Engineer.UseNumKey=1;
                       Timer_Counter_Clear(1);    
                       
                       if(Engineer.UseNumKey == 0)
                       {
                           Led_Window(Window_2,5,0); 
                           Keyboard_NumberCount(3,K_BACK);
                       }
                       // 数字键换算累计值
                       _Sum = Keyboard_NumberCount(3,by_KeyNumber);
                       // 
                       Led_Window(Window_2,4,_Sum);
                       Engineer.UseNumKey=1;
                       //
                   }
                   break;
          case K_BACK:     
                   if(Engineer.UseNumKey == 1)
                   {
                       Engineer.UseNumKey = 0;
                       Show_Speed();
                   }
                   break;
          case K_ENTER:
                   if(Engineer.key == 1)
                   {
                       if(Engineer.UseNumKey == 1)
                       {
                           Engineer.UseNumKey = 0;
                           Timer_Close(1);
                           if((Console_SpeedUnit(Data_Get,0) == C_KM && (_Sum >= 8 && _Sum <= Console_MaxSpeed(Data_Get,0))) || \
                              (Console_SpeedUnit(Data_Get,0) == C_MILE && (_Sum >= 5 && _Sum <= Console_MaxSpeed(Data_Get,0))))
                           {
                               Console_Information(Info_WorkoutSpeed,Data_Set,_Sum * 10);
                           }
                           Show_Speed();
                       }
                   }
                   break;        
      }
    
      if(!Check_SafeKey())
      {
          Digital_Initial();
          Digital_Set_DigitalSpeedTarget(0);//==>速度归零
          if((Digital_GetStatus() & BIT0)||(Digital_GetStatus() & BIT1))
          {// 升降有动作时就需要强制停止
              Digital_Command(CmdSetInclineAction,0);//==>升降停止    
          }
          by_Status = M_NEXT;
      }
  }
  Engineer.key = 0;
  Timer_Close(0);
  Timer_Close(1);
  Led_Set_Target(Display_OFF,0);
  Console_WorkoutStatus(Data_Set,0);
  Timer_Counter_Clear(3);
}
/*******************************************************************************
* Function Name  : EngineerMode_RPMParameter()
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EngineerMode_RPMParameter(void)
{
  unsigned char by_Status = M_NONE;
  unsigned char by_KeyBu = 0;
  unsigned char by_Mode = 0;
  unsigned short by_Buffer;
  unsigned short  by_KeyData=0;
  
  Led_Set_Target(Display_OFF,0);
  Engineer.key = 0;
  
  Timer_Counter_Clear(0);
  Led_Set_Reset();
  while(by_Status == M_NONE)
  {
      switch(by_Mode)
      {
          case 0:
                 if(Console_SpeedUnit(Data_Get,0) == C_KM)
                 {
                     Led_String("PLEASE CHECK SPEED TO 0.8Km/h",0,Show_Auto);
                 }    
                 else
                 {
                     Led_String("PLEASE CHECK SPEED TO 0.5Mile",0,Show_Auto);
                 }  
                 if(Led_Get_String_End() == 1 && Digital_Get_RPMStatus() == 0) 
                 {
                     by_Mode = 1;
                     Engineer.key = 1;
                     by_Buffer = EEPROM_ReadInformation(EE_MinRPM_Parameter);// MinPRM
                     Led_String(" Min RPM ",0,Show_Auto);
                     if(Console_SpeedUnit(Data_Get,0) == C_KM)
                         //Console_Speed(Data_Set,80);
                         Console_Information(Info_WorkoutSpeed,Data_Set,80);
                     else
                         //Console_Speed(Data_Set,50);
                          Console_Information(Info_WorkoutSpeed,Data_Set,50);
                     Digital_Set_DigitalSpeedTarget(1);//==>开始追击目标速度
                     Digital_Command(CmdSetMotorCounter, Console_Information(Info_WorkoutSpeed,Data_Get,0));//Console_Speed(Data_Get,0) 
                 }
                 break;
          case 1:
                 Led_7SEG(1,by_Buffer);
                 if(Engineer.key == 1)
                 {
                     Engineer.key = 0;
                     Console_RPM_Parameter(1,0,by_Buffer);// 放入MinRPM值
                     if(Console_SpeedUnit(Data_Get,0) == C_KM) 
                         //Console_Speed(Data_Set,80);
                         Console_Information(Info_WorkoutSpeed,Data_Set,80);
                     else
                         //Console_Speed(Data_Set,50);
                         Console_Information(Info_WorkoutSpeed,Data_Set,50);
                     Digital_Command(CmdSetMotorCounter,Console_Information(Info_WorkoutSpeed,Data_Get,0));//Console_Speed(Data_Get,0) 
                 }
                 break;
          case 2:
                 if(Console_SpeedUnit(Data_Get,0) == C_KM)
                 {
                     Led_String("PLEASE CHECK SPEED TO 10.0Km/h",0,Show_Auto);
                 }    
                 else
                 {
                     Led_String("PLEASE CHECK SPEED TO 6.0Mile",0,Show_Auto);
                 }  
                 if(Led_Get_String_End() == 1) 
                 {
                     by_Mode = 3;
                     Engineer.key = 1;
                     by_Buffer = EEPROM_ReadInformation(EE_MaxRPM_Parameter);// MaxRPM
                     Led_String(" Max RPM ",0,Show_Auto);
                 }
                 break;
          case 3:
                 Led_7SEG(1,by_Buffer);
                 if(Engineer.key)
                 {
                     Engineer.key = 0;
                     Console_RPM_Parameter(1,1,by_Buffer);// 放入MaxRPM值
                     if(Console_SpeedUnit(Data_Get,0) == C_KM)
                         //Console_Speed(Data_Set,1000);
                         Console_Information(Info_WorkoutSpeed,Data_Set,1000); 
                     else
                         //Console_Speed(Data_Set,600);
                         Console_Information(Info_WorkoutSpeed,Data_Set,600);
                     Digital_Command(CmdSetMotorCounter,Console_Information(Info_WorkoutSpeed,Data_Get,0));//Console_Speed(Data_Get,0) 
                 }
                 break;
                 
      }
      by_KeyBu = 1;
      by_KeyData=Key_Proce();
      if(by_KeyData != K_NONE)
      {
          Timer_Counter_Clear(3);
      }
      switch(by_KeyData)
          {
          case K_ENTER:  
               if(by_Mode == 1)
               {
                   EEPROM_SaveWorkInformation(EE_MinRPM_Parameter,by_Buffer); 
                   by_Mode = 2;
                   Led_7SEG(4,0);
                   BEEP_Proce(4,3,3);
                   Led_Set_Reset();
               }
               else if(by_Mode == 3)
               {
                   EEPROM_SaveWorkInformation(EE_MaxRPM_Parameter,by_Buffer); 
                   by_Status = M_RESET;
                   Led_7SEG(4,0);
                   BEEP_Proce(4,3,3);
                   while(BEEP_Get_OK() == 1) ;
                   Digital_Set_DigitalSpeedTarget(0);//==>速度归零
               }
               break;
          case K_SpeedU | K_MASK:
          case K_InclineU | K_MASK:  
          case K_TimeU | K_MASK: 
               by_KeyBu = 10; 
          case K_SpeedU:
          case K_InclineU:  
          case K_TimeU:
               if(by_Mode == 1) 
                   by_Buffer = Console_RPM_Parameter(2,0,by_KeyBu);// Add MinRPM
               else if(by_Mode == 3) 
                   by_Buffer = Console_RPM_Parameter(2,1,by_KeyBu);// Add MaxRPM
               Engineer.key=1;
               break;
          case K_SpeedD | K_MASK:
          case K_InclineD | K_MASK:  
          case K_TimeD | K_MASK: 
               by_KeyBu=10; 
          case K_SpeedD:
          case K_InclineD:  
          case K_TimeD:
               if(by_Mode == 1) 
                   by_Buffer = Console_RPM_Parameter(3,0,by_KeyBu);// Dec MinRPM
               else if(by_Mode == 3) 
                   by_Buffer = Console_RPM_Parameter(3,1,by_KeyBu);// Dec MaxRPM
               Engineer.key=1;
               break;
          }
      if(!Check_SafeKey() || Timer_Counter(T_STEP,3,1200)) by_Status=M_RESET;
      }
  Timer_Close(0);
  Timer_Counter_Clear(3);
}
/*******************************************************************************
* Function Name  : EngineerMode_SetMode()
*                  是否处于工程模式
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EngineerMode_SetMode(char by_D)
{
  if(by_D == 0)
      Engineer.Mode = 0;
  else
      Engineer.Mode = 1;
}
char EngineerMode_GetMode(void)
{
  return Engineer.Mode;
}
/*******************************************************************************
* Function Name  : EngineerMode_ExportOrImport()
*                  Export to USB or Import from USB
* Input          : Index
* Output         : None
* Return         : None
*******************************************************************************/
void EngineerMode_ExportOrImport(unsigned char Index)
{
    unsigned long Delay = 0; 
    unsigned char by_State = 0, by_Key, State = 1;
    
    //
    //IO_USB_Communication();
    //UsbFunction_USB(0);//==> Detect USB  
    while(State)
    {
        UsbFunction_USB(0x500);
        if(UsbFunction_USB_Status() == 1)
        {//==>插上随身碟
//            if(Index == _Manage_Item_ImportUSB)
//              DisconnectWIFI();//before connect assuance disconnect status
            Delay = 0;
            while((++Delay) < 110000)
                Led_String("Process...", 0, Show_Auto); 
            //
            switch(Index)
            {
                case _Service_Item_ExportUSB: // Export special Mode Parameter
                    by_State = UsbFunction_ExportEngineerParameter();
                    break;
                case _Service_Item_ImportUSB: // Import special Mode Parameter
                    by_State = UsbFunction_InportEngineerParameter(0);
                    break;
                //case _Service_Item_ImportUSB_2: // Import special Mode Parameter
                //    by_State = UsbFunction_InportEngineerParameter(1);
                //    break;
                case _Manage_Item_ExprotUSB:     // Export WiFi configuration
                    if(UsbFunction_ExportWifiConfig() == C_WriteOK)
                       by_State = 1;
                    break;
                case _Manage_Item_ImportUSB:    // Import WiFi configuration
                    if(UsbFunction_ReadWifiConfig() == C_WriteOK)
                        by_State = 1;
                    break;
            }
          
            Delay = 0;
            if( by_State == 1 )
            {
                BEEP_Proce(4,2,2);
                while((++Delay) < 110000)
                    Led_String("Success !!", 0, Show_Auto);
            }
            else
            {
                while((++Delay) < 110000)
                    Led_String("Failure !!", 0, Show_Auto);
            }
            State = 0;
        }
        else
        {
            if(++Delay < 110000)
                Led_String("Process...", 0, Show_Auto);
            else
                Led_String("No USB", 0, Show_Auto);
        }
        
        by_Key = Key_Proce();
        if(by_Key != K_NONE)
          Timer_Counter_Clear(3);
        if(by_Key == K_BACK ||\
           Check_SafeKey()==0 ||\
           Timer_Counter(T_STEP,3,1200)==1 )
        {
            State = 0;
        }
    }
    //
    //IO_USB_OnlyCharge();
    Led_Set_Reset();
}

// start v2.1-13
/*******************************************************************************
* Function Name  : EngineerMode_RSCUpdate()
* Note           : RSCU software update
* Input          : by_Mode (1:手动 0:自动)
* Output         : None
* Return         : None
*******************************************************************************/
void EngineerMode_RSCUpdate(unsigned char by_Mode)
{// from server download software file go to wifi module. 
  if(by_Mode == 1)
  {// 手動
      wifi_settings.status.UCBfileCheckOK = false ;
      Timer_Counter_Clear(1);
      while(wifi_settings.status.UCBfileCheckOK == false)
      {
          if(Timer_Counter(T_STEP,1,300)==1) return;
          Led_String("Press Enter for Update",0,Show_Auto);
          switch(Key_Proce())
          {
              case K_ENTER:
                     wifi_settings.status.UCBfileCheckOK = true ;
              default:       
                     break;
              case K_BACK:
                     return;
          }
      }
  }
  //
  Timer_Counter_Clear(1);
  while(Timer_Counter(T_STEP,1,30) == 0)
  {
      Led_String("Update...",0,Show_Auto);
  }
  UsbFunction_RSCU_SoftwareUpdate();
}
// end v2.1-13
// start v2.2-5
// RSCU
#define _RSCU_MsgNum 8
const struct
{
    unsigned char MsgIndex;
    unsigned char* MsgStrings;
}RSCU_MSG[_RSCU_MsgNum] = 
{
    {_RSCU_CheckForSoftwareOnDAPI,    "Checking for software on DAPI"},
    {_RSCU_DownloadSoftwareToModule,  "Downloading to WiFi module"},
    {_RSCU_DownloadSoftwareToEEPROM,  "Downloading to console"},
    {_RSCU_CheckForSoftwareOnModule,  "Checking for software on WiFi module"},
    {_RSCU_SoftwareDownloadOK,        "Download software completely"},
    {_RSCU_NoLatestSoftware,          "No the latest version"}, 
    {_RSCU_CanUpdate,                 "Press enter for update"},
    {_RSCU_Disconnection,             "Disconnection"},
};
void EngineerMode_ManualUpdate(void)
{
  unsigned char i = 0, Rtn, NowMsg = 0xFF;
  unsigned char by_State = 1;
  unsigned char _ClearMsg = 0;  
  
  // Clear screen
//  LED_MATRIX(_BigMatrix_Clear, 0);
  Led_Set_Target(Display_OFF,0);// Led All Off
  WiFi_RSCU_Open();
  WiFi_RSCU_Process(_RSCU_Reset);
  while(by_State)
  {
      Rtn = WiFi_RSCU_Process(_RSCU_Update);
      for(i=0; i<_RSCU_MsgNum; i++)
      {
          if(Rtn == RSCU_MSG[i].MsgIndex)
          {
              if(NowMsg != i)
                  Led_Set_Reset();
              break;
          }
      }
      NowMsg = i;
      if(_ClearMsg != NowMsg)
      {
          _ClearMsg = NowMsg;
          Show_Message(Message_Blank,0,0,Show_Blank);
          Led_Set_Reset();
      }
      if(NowMsg >= _RSCU_MsgNum)
      {
          Show_StringOneTime("RSCU failure. Try again later.",1);
          by_State = 0;
      }
      else
      {
          Led_7SEG(1, WiFi_DownloadPercentage());
          Led_String(RSCU_MSG[NowMsg].MsgStrings,0,Show_Auto);
          switch(Rtn)
          {
              case _RSCU_Disconnection:
              case _RSCU_NoLatestSoftware:
                  Show_StringOneTime(RSCU_MSG[NowMsg].MsgStrings,30);
                  by_State = 0;
                  break;
              case _RSCU_SoftwareDownloadOK:
                  break;
              case _RSCU_CanUpdate:
                  Timer_Counter_Clear(_Time_RSCUCheck);
                  while(by_State)
                  {
                      if(Timer_Counter(T_STEP,_Time_RSCUCheck,300)==1) 
                          break;
                      Led_String(RSCU_MSG[NowMsg].MsgStrings,0,Show_Auto);
                      switch(Key_Proce())
                      {
                          case K_ENTER:
                              while(BEEP_Get_OK());
                              UsbFunction_RSCU_SoftwareUpdate();
                          case K_BACK:
                              by_State = 0;
                              break;
                      }
                  }
                  Timer_Close(_Time_RSCUCheck);
                  by_State = 0;
                  break;
              default:
                  break;
          }
          
      }
      switch(Key_Proce())
      {
          case K_BACK:    
          case K_STOP:
                   by_State = 0;
                   break;
      }
      if(Check_SafeKey() == 0) by_State = 0;
  }
  // Clear screen
//  LED_MATRIX(_BigMatrix_Clear, 0);
  Led_Set_Target(Display_OFF,0);// Led All Off
}
// end v2.2-5

void EngineerMode_SoftwareBackup(void)
{
    unsigned char by_State = 1, by_Key = K_NONE;
    unsigned char BU_state = 0;
    unsigned char ConsoleVer[3] = {0};
    unsigned char by_String[80] = {0};
    
    // Read Backup Version 
    Mx25L1606E_ReadBlock( EEPRomPartition_Information_StartAddress_Backup+1, 3, ConsoleVer);
    
    if(Mx25L1606E_ReadByte(EEPRomPartition_Information_StartAddress_Backup) != _AlreadyBackUp_)
        sprintf((char *)by_String, "No backup software. Press enter for backuping software");
    else
    {
        BU_state = 1;
        sprintf((char *)by_String, "Backup version is V%d.%d Press enter for Re-backuping software", ConsoleVer[0], ConsoleVer[1]);
    }
    Timer_Counter_Clear(3);
    while(by_State)
    {
        // display
        Led_String(by_String, 0, Show_Auto);
            
        by_Key = Key_Proce();   
        
        switch(by_Key)
        {
            case K_ENTER:
                     EngineerMode_BackUpProcess(BU_state);
            case K_BACK:    
            case K_STOP:
                     by_State = 0;
                     break;
        }
        if((Check_SafeKey()==0)||(Timer_Counter(T_STEP,3,1200)==1)) by_State = 0;
    }
}
void EngineerMode_BackUpProcess(unsigned char Index)
{
    unsigned char CheckState;
  
    if(Index == 0)
        Show_StringOneTime("Backuping...", 1);
    else
        Show_StringOneTime("Re-Backuping...", 1);
    Led_String("waiting...",0,Show_Auto);
    CheckState = Boot_BackUp(UCB_Software_Ver,UCB_Software_Rel,UCB_Software_Build);
    
    if(CheckState == 1)
        Show_StringOneTime("Backup complete",1);
    else
        Show_StringOneTime("Backup failure",1);
    
    Show_Message(Message_Blank,0,0,Show_Auto);
}
