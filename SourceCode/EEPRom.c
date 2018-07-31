#include  "Flash.h"
#include  "EEPRom.h"
#include  "Console.h"
#include  "General.h"
#include  "keyboard.h"
#include  "Beep.h"
#include  "Mx25L1606E.h"
#include  "C_safe.h"
#include  "RF900.h"
#include  "Digital.h"
#include  "wifi_Digital.h"
#include <string.h>
//#include  "Voice.h"
#include  "WiFi_Information.h"
#include "UsbFunction.h"
//==============================================================================
#define   _Sector0_Block0_  0
#define   _Sector0_Block1_  1024
#define   _Sector0_Block2_  2048
#define   _Sector0_Block3_  3072
#define   _Sector1_Block0_  4096
#define   _Sector1_Block1_  5120
#define   _Sector1_Block2_  6144
#define   _Sector1_Block3_  7168

#define   _NumberOfBlock_     8
#define   _Sector_BlockSize_  1024
#define   _BlockIndex_        0xAA51
#define   _Sector0_StartNo_   1
#define   _Sector1_StartNo_   5
// Must be match 4 byte
union {
  struct {
    // for Check Memory  //==> x 表示为以4个byte为1单位
    unsigned short CheckNo ;                      // 2          // save number FF ==> NULL 0xAA51-0xAA54
    unsigned short CheckSum ;                     // 2    x     // 0xFFFF - Total sum = XXXX
    unsigned short MLength ;                      // 2   
    // 开始规划储存起始
    unsigned short MaxTime ;                      // 2    x
    unsigned short UserTime ;                     // 2    
    unsigned short Weight ;                       // 2    x
    unsigned short UserHeight ;                   // 2
    unsigned short WorkoutPowerSaveTime ;         // 2    x
    unsigned short HomePowerSaveTime ;            // 2
    unsigned short SleepTime ;                    // 2    x
    unsigned short PauseTime ;                    // 2
    unsigned short ClubID ;                       // 2    x   
    unsigned short ErrorList[10] ;                // 20   5x
    unsigned short Watts ;                        // 2    
    unsigned short TuneEndPointIncline1 ;         // 2    x
    unsigned short TuneEndPointIncline2 ;         // 2    
    unsigned short LowElevation ;                 // 2    x
    unsigned short MaxElevation ;                 // 2    
    unsigned short MaxRPM_Parameter ;             // 2    x
    unsigned short MinRPM_Parameter ;             // 2    
    unsigned short HoldBuffer ;                   // 2    x // 保留1个凑4byte
    //60
    unsigned char MaxSpeed ;                      // 1
    unsigned char InUser ;                        // 1
    unsigned char StartSpeed ;                    // 1
    unsigned char NAVY_Test ;                     // 1    x 
    unsigned char Level ;                         // 1
    unsigned char Age ;                           // 1
    unsigned char Gender ;                        // 1
    //67
    unsigned char Unit ;                          // 1    x
    unsigned char InclineLv ;                     // 1
    unsigned char MaxResistLv ;                   // 1
    unsigned char MinResistLv ;                   // 1
    unsigned char Sound ;                         // 1    x
    unsigned char Language ;                      // 1
    unsigned char ErrorCode ;                     // 1
    unsigned char Channel ;                       // 1 // 无用到,先拿来用更新特定新参数使用
    unsigned char Volume ;                        // 1    x
    unsigned char Logo ;                          // 1 // 2013.07.26 TimeMode => 没用到所以改名
    unsigned char OutOfOrder ;                    // 1
    unsigned char InclineBatteryVoltage ;         // 1
    unsigned char InclineRPM ;                    // 1    x
    unsigned char InclineReset ;                  // 1
    unsigned char PowerOffTime ;                  // 1 
    unsigned char RFService ;                     // 1
    unsigned char MachineType ;                   // 1    x Bike/EP/InclineEP/Stepper/Climbmill
    unsigned char ConsoleVersion ;                // 1     
    unsigned char FrameVersion ;                  // 1     
    unsigned char Audio ;                         // 1 
    unsigned char AM ;                            // 1    x
    unsigned char MaxVolume ;                     // 1      
    unsigned char OutputVolume ;                  // 1      
    unsigned char HRTSpeedBased ;                 // 1
    unsigned char TVPower ;                       // 1    x 
    unsigned char MaxDF_Elevation ;               // 1
    unsigned char Speaker ;                       // 1
    unsigned char xID ;                           // 1
    unsigned char DemoMode ;                      // 1    x
    unsigned char HJS_Switch ;                    // 1
    unsigned char Wireless ;                      // 1
    unsigned char TimeZone ;                      // 1
    unsigned char DAPI_Server ;                   // 1    x
    unsigned char USB_Protection ;                // 1
    unsigned char KeyStuck ;                      // 1
    unsigned char KeyNotifiction ;                // 1
    unsigned char SSL ;                           // 1    x // v2.1-11       //118
    char WiFi_SSID[33];                           // 33     
    char WiFi_Password[255];                      // 255  72x  
    unsigned char WiFi_Security;                  // 1  
    unsigned char MachineType_LCB ;               // 1  
    unsigned char AP_Mode ;                       // 1 
    unsigned char AutoUpdateSwitch ;              // 1     x // v2.1-13
    unsigned char AutoUpdateTime ;                // 1     // v2.1-13 
    unsigned char AutoUpdateDate ;                // 1      // v2.1-13
    unsigned char TimeMode ;                      // 1
    //399
    unsigned char InclineClibrationFlag ;         // 1

    //
    unsigned long AccumulatedTime ;               // 4    x
    unsigned long AccumulatedDistance ;           // 4    x
    unsigned long SerialNo ;                      // 4    x   
    unsigned long SerailNoFrame ;                 // 4    x 
    unsigned long CommunicationTX ;               // 4    x
    unsigned long CommunicationRX ;               // 4    x
    unsigned long HJS_total ;                     // 4    x
    unsigned long HJS_CheckNumber ;               // 4    x
    unsigned long RSCU_SoftwareVersion;           // 4    x // v2.2-7
    //
    unsigned long RunningBeltCurrentAccMiles ;    //4
    unsigned long RunningBeltCurrentAccHours ;    //4
    unsigned long RunningBeltPreviousAccMiles ;   //4
    unsigned long RunningBeltPreviousAccHours ;   //4
    unsigned short RunningBeltPreviousYear;       //2
    unsigned char RunningBeltPreviousMouth;       //1
    unsigned char RunningBeltPreviousDay;         //1   //20
    
    unsigned long RunningDeckCurrentAccMiles ;
    unsigned long RunningDeckCurrentAccHours ;
    unsigned long RunningDeckPreviousAccMiles ;
    unsigned long RunningDeckPreviousAccHours ;
    unsigned short RunningDeckPreviousYear;
    unsigned char RunningDeckPreviousMouth;
    unsigned char RunningDeckPreviousDay;             //20
    
    unsigned long MCBCurrentAccMiles ;
    unsigned long MCBCurrentAccHours ;
    unsigned long MCBPreviousAccMiles ;
    unsigned long MCBPreviousAccHours ;
    unsigned short MCBPreviousYear;
    unsigned char MCBPreviousMouth;
    unsigned char MCBPreviousDay;                 //20
    
    unsigned long MoterCurrentAccMiles ;
    unsigned long MoterCurrentAccHours ;
    unsigned long MoterPreviousAccMiles ;
    unsigned long MoterPreviousAccHours ;
    unsigned short MoterPreviousYear;
    unsigned char MoterPreviousMouth;
    unsigned char MoterPreviousDay;               //20
    
    unsigned long HeadphoneCurrentAccHours ;
    unsigned long HeadphoneCurrentInsertsTimes ;
    unsigned long HeadphonePreviousAccHours ;
    unsigned long HeadphonePreviousInsertsTimes ;
    unsigned short HeadphonePreviousYear;
    unsigned char HeadphonePreviousMouth;
    unsigned char HeadphonePreviousDay;           //20
    
    unsigned long Prev_MaintenanceCurrentAccMiles ;
    unsigned long Prev_MaintenanceCurrentAccHours ;
    unsigned long Prev_MaintenancePreviousAccMiles ;
    unsigned long Prev_MaintenancePreviousAccHours ;
    unsigned short Prev_MaintenancePreviousYear;
    unsigned char Prev_MaintenancePreviousMouth;
    unsigned char Prev_MaintenancePreviousDay;        //20
    
//    unsigned char Idle;
//    unsigned char Retained[2] ;                   // 1    x // 保留1个凑4byte

  } Parameter ;
  unsigned char Block[_Sector_BlockSize_] ;
} EEMemory ;




unsigned char EEPROM_Initial(void) ;
unsigned short EEPROM_CalculatorChecksum( unsigned char *Dptr, unsigned short size ) ;
unsigned char EEPROM_CheckEEMemoryDataStatus( unsigned long Maddr, unsigned short size, unsigned short CheckData );
unsigned char EEPROM_CheckSectorErase( unsigned long SetcorAddr ) ;
unsigned char EEPROM_VerifyData( unsigned long WriteAddress, unsigned char *Dptr, unsigned short size ) ;
unsigned char EEPROM_GetMemoryErrorStatus(void) ;



char _AccMode;
unsigned char NowMemoryBlockNo ;
unsigned char MemoryStatus ;     // 0xAA = empty , 0x55 = OK
unsigned char MemoryErrorStatus ;
#define   _Empty_           0xAA
#define   _NoEmpty_         0x55
#define   _WriteError_      0xAA
#define   _WriteOK_         0x55


// 2013.07.26
extern unsigned char _Display_Logo;
//
/*******************************************************************************
* Function Name  : EEPROM_WriteInformation()
* Description    : 外挂 EEPROM 数据写入,16bit资料转成两个8bit写入
* Input          : by_Address = 内存储存位置 , by_Data = 储存数据
* Output         : None
* Return         : None
*******************************************************************************/
void EEPROM_WriteInformation(u16 by_Address,u32 by_Data) 
{
 
  switch( by_Address )
  {
    // start v2.2-7  
  case EE_RSCU_SoftwareVersion:
    EEMemory.Parameter.RSCU_SoftwareVersion = by_Data ;
    break; 
    // end v2.2-7                              
    // start v2.1-13 
  case EE_AutoUpdateDate:
    EEMemory.Parameter.AutoUpdateDate = by_Data ;
    break ;  
  case EE_AutoUpdateSwitch:   
    EEMemory.Parameter.AutoUpdateSwitch = by_Data ;
    break ;   
  case EE_AutoUpdateTime:   
    EEMemory.Parameter.AutoUpdateTime = by_Data ;
    break ;                                 
    // end v2.1-13   
    // start v2.1-11  
  case EE_SSL:   
    EEMemory.Parameter.SSL = by_Data ;
    break ;   
    // end v2.1-11                              
  case EE_USB_Protection:   
    EEMemory.Parameter.USB_Protection = by_Data ;
    break ;   
  case EE_KeyStuck:                               
    EEMemory.Parameter.KeyStuck = by_Data ;
    break ;   
  case EE_KeyNotifiction:                               
    EEMemory.Parameter.KeyNotifiction = by_Data ;
    break ;   
  case EE_DAPI_Server: 
    EEMemory.Parameter.DAPI_Server = by_Data ;
    break ;   
  case EE_TZone: 
    EEMemory.Parameter.TimeZone = by_Data ;
    break ;     
  case EE_Wireless: 
    EEMemory.Parameter.Wireless = by_Data ;
    break ;      
  case EE_HJS_Switch: 
    EEMemory.Parameter.HJS_Switch = by_Data ;
    break ;      
  case EE_HJS_CheckNumber: 
    EEMemory.Parameter.HJS_CheckNumber = by_Data ;
    break ;   
  case EE_HJS_total: 
    EEMemory.Parameter.HJS_total = by_Data ;
    break ;                                
  case EE_xID: 
    EEMemory.Parameter.xID = by_Data ;
    break ;    
  case EE_DemoMode:           
    EEMemory.Parameter.DemoMode = by_Data ;
    break ;   
    
  case EE_Speaker:
    EEMemory.Parameter.Speaker = by_Data ;
    break;  
  case EE_MaxDF_Elevation:
    EEMemory.Parameter.MaxDF_Elevation = by_Data ;
    break;   
  case EE_Height:   
    EEMemory.Parameter.UserHeight = by_Data ;
    break;     
  case EE_TVPower:   
    EEMemory.Parameter.TVPower = by_Data ;
    break;   
  case EE_HRTSpeedBased:    
    EEMemory.Parameter.HRTSpeedBased = by_Data ;
    break;   
  case EE_CommunicationTX:    
    EEMemory.Parameter.CommunicationTX = by_Data ;
    break;   
  case EE_CommunicationRX:    
    EEMemory.Parameter.CommunicationRX = by_Data ;
    break;                                
  case EE_MaxVolume:            
    EEMemory.Parameter.MaxVolume = by_Data ;
    break ;   
  case EE_OutputVolume:         
    EEMemory.Parameter.OutputVolume = by_Data ;
    break ;   
  case EE_AM:                   
    EEMemory.Parameter.AM = by_Data ;
    break;    
  case EE_Audio:                
    EEMemory.Parameter.Audio = by_Data ;
    break;  
  case EE_ConsoleVersion:       
    EEMemory.Parameter.ConsoleVersion = by_Data ;
    break;
  case EE_FrameVersion:         
    EEMemory.Parameter.FrameVersion = by_Data ;
    break;
  case EE_FrameSerialNumber:    
    EEMemory.Parameter.SerailNoFrame = by_Data ;
    break;  
  case EE_SleepTime             :
    EEMemory.Parameter.SleepTime = by_Data ;
    break ;  
  case EE_MAX_TIME              :
    EEMemory.Parameter.MaxTime = by_Data ;
    break ;
  case EE_USER_TIME             :
    EEMemory.Parameter.UserTime = by_Data  ;
    break ;        
  case EE_WEIGHT                :
    EEMemory.Parameter.Weight = by_Data ;
    break ;        
  case EE_LEVEL                 :
    EEMemory.Parameter.Level = by_Data ;
    break ;        
  case EE_AGE                   :
    EEMemory.Parameter.Age = by_Data ;
    break ;        
  case EE_UNIT                  :
    EEMemory.Parameter.Unit = by_Data ;
    break ; 
  case EE_LOW_ELEVATION         :  
    EEMemory.Parameter.LowElevation = by_Data ;
    break;
  case EE_MAX_ELEVATION         :    
    EEMemory.Parameter.MaxElevation = by_Data ;
    break;
  case EE_ACCUMULATED_TIME      :
    EEMemory.Parameter.AccumulatedTime = by_Data;//(u32)by_Data * 0x10000 ;
    break ;        
  case EE_ACCUMULATED_DISTANCE  :
    EEMemory.Parameter.AccumulatedDistance  = by_Data;//(u32)by_Data * 0x10000 ;
    break ;        
  case EE_LANGUAGE              :
    EEMemory.Parameter.Language = by_Data  ;
    break ;    
  case EE_StartSpeed            :  
    EEMemory.Parameter.StartSpeed = by_Data  ;
    break ; 
  case EE_MaxSpeed              :  
    EEMemory.Parameter.MaxSpeed = by_Data  ;
    break ;                               
  case EE_PauseTime             :
    EEMemory.Parameter.PauseTime = by_Data ;
    break ;                                     
  case EE_CHANNEL               :
    EEMemory.Parameter.Channel = by_Data ;
    break ;        
  case EE_VOLUME                :
    EEMemory.Parameter.Volume = by_Data ;
    break ;     
  case EE_TimerMode             :
    EEMemory.Parameter.TimeMode = by_Data ;
    break ;                                     
  case EE_Logo                  :
    EEMemory.Parameter.Logo = by_Data ;
    break ; 
  case EE_GENDER                :
    EEMemory.Parameter.Gender = by_Data ;
    break ; 
  case EE_InclineCalibrationFlag		:  
    EEMemory.Parameter.InclineClibrationFlag = by_Data ;
    break ;
  case EE_OutOfOrder            :
    EEMemory.Parameter.OutOfOrder = by_Data ;
    break ;                                      
  case EE_MaxRPM_Parameter         :   
    EEMemory.Parameter.MaxRPM_Parameter = by_Data ;
    break ; 
  case EE_MinRPM_Parameter         :   
    EEMemory.Parameter.MinRPM_Parameter = by_Data ;
    break ;                               
  case EE_BEEP                 :
    EEMemory.Parameter.Sound = by_Data ;
    break ;                                     
  case EE_MachineType           :
    EEMemory.Parameter.MachineType = by_Data ;
    break ;       
  case EE_MachineType_LCB       :
    EEMemory.Parameter.MachineType_LCB = by_Data ;
    break ;                                
  case EE_SerialNumber          : // serial no
    EEMemory.Parameter.SerialNo = by_Data ;//(u32)by_Data * 0x10000 ;
    break ;        
  case EE_NAVY_Test             : 
    EEMemory.Parameter.NAVY_Test = by_Data ;
    break ;                               
  case EE_RF_SERVICE            :
    EEMemory.Parameter.RFService = by_Data ;
    break ;        
  case EE_CLUB_ID               :
    EEMemory.Parameter.ClubID = by_Data ;
    break ;    
  case EE_InUser                :
    EEMemory.Parameter.InUser = by_Data ;
    break ;                               
  case EE_ErrorCode          :
    EEMemory.Parameter.ErrorCode = by_Data ;
    break ;                                      
  case EE_DigitalError_1        :
  case EE_DigitalError_2        :
  case EE_DigitalError_3        :
  case EE_DigitalError_4        :
  case EE_DigitalError_5        :
  case EE_DigitalError_6        :
  case EE_DigitalError_7        :
  case EE_DigitalError_8        :
  case EE_DigitalError_9        :
  case EE_DigitalError_10       :
    EEMemory.Parameter.ErrorList[by_Address - EE_DigitalError_1] = by_Data ;
    break ;
  case EE_AP_Mode               :
    EEMemory.Parameter.AP_Mode = by_Data ;
    break ;        
  case EE_WiFi_Security         :
    EEMemory.Parameter.WiFi_Security = by_Data ;
    break;
  case EE_WiFi_SSID             :
    __memset( EEMemory.Parameter.WiFi_SSID, 0, 33 );
    __strncpy(EEMemory.Parameter.WiFi_SSID,wifi_settings.security.ssid,33);        
    break;
  case EE_WiFi_Password         :
    __memset( EEMemory.Parameter.WiFi_Password, 0, 255 );
    __strncpy(EEMemory.Parameter.WiFi_Password,wifi_settings.security.security_key,255);              
    break;                                    
      /*                                    RunningBelt                                      */
      case EE_RunningBeltCurrentAccMiles:
                                    EEMemory.Parameter.RunningBeltCurrentAccMiles = by_Data;
                                    break;
      case EE_RunningBeltCurrentAccHours:
                                    EEMemory.Parameter.RunningBeltCurrentAccHours = by_Data;
                                    break;
      case EE_RunningBeltPreviousAccMiles:
                                    EEMemory.Parameter.RunningBeltPreviousAccMiles = by_Data;
                                    break;
      case EE_RunningBeltPreviousAccHours:
                                    EEMemory.Parameter.RunningBeltPreviousAccHours = by_Data;
                                    break;
      case EE_RunningBeltPreviousYear:
                                    EEMemory.Parameter.RunningBeltPreviousYear = by_Data;
                                    break;
      case EE_RunningBeltPreviousMouth:
                                    EEMemory.Parameter.RunningBeltPreviousMouth = by_Data;
                                    break;
      case EE_RunningBeltPreviousDay:
                                    EEMemory.Parameter.RunningBeltPreviousDay = by_Data;
                                    break;
      /*                                    RunningDeck                                      */
      case EE_RunningDeckCurrentAccMiles:
                                    EEMemory.Parameter.RunningDeckCurrentAccMiles = by_Data;
                                    break;
      case EE_RunningDeckCurrentAccHours:
                                    EEMemory.Parameter.RunningDeckCurrentAccHours = by_Data;
                                    break;
      case EE_RunningDeckPreviousAccMiles:
                                    EEMemory.Parameter.RunningDeckPreviousAccMiles = by_Data;
                                    break;
      case EE_RunningDeckPreviousAccHours:
                                    EEMemory.Parameter.RunningDeckPreviousAccHours = by_Data;
                                    break;
      case EE_RunningDeckPreviousYear:
                                    EEMemory.Parameter.RunningDeckPreviousYear = by_Data;
                                    break;
      case EE_RunningDeckPreviousMouth:
                                    EEMemory.Parameter.RunningDeckPreviousMouth = by_Data;
                                    break;
      case EE_RunningDeckPreviousDay:
                                    EEMemory.Parameter.RunningDeckPreviousDay = by_Data;
                                    break;
      /*                                    MCB                                      */
      case EE_MCBCurrentAccMiles:
                                    EEMemory.Parameter.MCBCurrentAccMiles = by_Data;
                                    break;
      case EE_MCBCurrentAccHours:
                                    EEMemory.Parameter.MCBCurrentAccHours = by_Data;
                                    break;
      case EE_MCBPreviousAccMiles:
                                    EEMemory.Parameter.MCBPreviousAccMiles = by_Data;
                                    break;
      case EE_MCBPreviousAccHours:
                                    EEMemory.Parameter.MCBPreviousAccHours = by_Data;
                                    break;
      case EE_MCBPreviousYear:
                                    EEMemory.Parameter.MCBPreviousYear = by_Data;
                                    break;
      case EE_MCBPreviousMouth:
                                    EEMemory.Parameter.MCBPreviousMouth = by_Data;
                                    break;
      case EE_MCBPreviousDay:
                                    EEMemory.Parameter.MCBPreviousDay = by_Data;
                                    break;
      /*                                    Moter                                      */
      case EE_MoterCurrentAccMiles:
                                    EEMemory.Parameter.MoterCurrentAccMiles = by_Data;
                                    break;
      case EE_MoterCurrentAccHours:
                                    EEMemory.Parameter.MoterCurrentAccHours = by_Data;
                                    break;
      case EE_MoterPreviousAccMiles:
                                    EEMemory.Parameter.MoterPreviousAccMiles = by_Data;
                                    break;
      case EE_MoterPreviousAccHours:
                                    EEMemory.Parameter.MoterPreviousAccHours = by_Data;
                                    break;
      case EE_MoterPreviousYear:
                                    EEMemory.Parameter.MoterPreviousYear = by_Data;
                                    break;
      case EE_MoterPreviousMouth:
                                    EEMemory.Parameter.MoterPreviousMouth = by_Data;
                                    break;
      case EE_MoterPreviousDay:
                                    EEMemory.Parameter.MoterPreviousDay = by_Data;
                                    break;
      /*                                    Headphone                                      */
      case EE_HeadphoneCurrentAccHours:
                                    EEMemory.Parameter.HeadphoneCurrentAccHours = by_Data;
                                    break;
      case EE_HeadphoneCurrentInsertsTimes:
                                    EEMemory.Parameter.HeadphoneCurrentInsertsTimes = by_Data;
                                    break;
      case EE_HeadphonePreviousAccHours:
                                    EEMemory.Parameter.HeadphonePreviousAccHours = by_Data;
                                    break;
      case EE_HeadphonePreviousInsertsTimes:
                                    EEMemory.Parameter.HeadphonePreviousInsertsTimes = by_Data;
                                    break;
      case EE_HeadphonePreviousYear:
                                    EEMemory.Parameter.HeadphonePreviousYear = by_Data;
                                    break;
      case EE_HeadphonePreviousMouth:
                                    EEMemory.Parameter.HeadphonePreviousMouth = by_Data;
                                    break;
      case EE_HeadphonePreviousDay:
                                    EEMemory.Parameter.HeadphonePreviousDay = by_Data;
                                    break;
      /*                                    Prev_Maintenance                                      */
      case EE_Prev_MaintenanceCurrentAccMiles:
                                    EEMemory.Parameter.Prev_MaintenanceCurrentAccMiles = by_Data;
                                    break;
      case EE_Prev_MaintenanceCurrentAccHours:
                                    EEMemory.Parameter.Prev_MaintenanceCurrentAccHours = by_Data;
                                    break;
      case EE_Prev_MaintenancePreviousAccMiles:
                                    EEMemory.Parameter.Prev_MaintenancePreviousAccMiles = by_Data;
                                    break;
      case EE_Prev_MaintenancePreviousAccHours:
                                    EEMemory.Parameter.Prev_MaintenancePreviousAccHours = by_Data;
                                    break;
      case EE_Prev_MaintenancePreviousYear:
                                    EEMemory.Parameter.Prev_MaintenancePreviousYear = by_Data;
                                    break;
      case EE_Prev_MaintenancePreviousMouth:
                                    EEMemory.Parameter.Prev_MaintenancePreviousMouth = by_Data;
                                    break;
      case EE_Prev_MaintenancePreviousDay:
                                    EEMemory.Parameter.Prev_MaintenancePreviousDay = by_Data;
                                    break;
  default                       :
    break ;
  }
  //
  EEPROM_WriteParameter() ;
  return ;  
}


/*******************************************************************************
* Function Name  : EEPROM_ReadInformation()
* Description    : 外挂 EEPROM 数据读取,转换成16bit数据
* Input          : by_Address = 内存储存位置
* Output         : None
* Return         : 回传16bit资料
*******************************************************************************/
u32 EEPROM_ReadInformation(u16 by_Address)
{
  u32 RetData ;
  //
  switch( by_Address )
  {
    // start v2.2-7  
  case EE_RSCU_SoftwareVersion:
    RetData = EEMemory.Parameter.RSCU_SoftwareVersion;
    break;  
    // end v2.2-7                              
    // start v2.1-13 
  case EE_AutoUpdateDate:
    RetData = EEMemory.Parameter.AutoUpdateDate ;
    break ;  
  case EE_AutoUpdateSwitch:   
    RetData = EEMemory.Parameter.AutoUpdateSwitch ;
    break ;   
  case EE_AutoUpdateTime:   
    RetData = EEMemory.Parameter.AutoUpdateTime ;
    break ;                                 
    // end v2.1-13     
    // start v2.1-11  
  case EE_SSL:   
    RetData = EEMemory.Parameter.SSL ;
    break ; 
    // end v2.1-11                              
  case EE_USB_Protection:   
    RetData = EEMemory.Parameter.USB_Protection ;
    break ;   
  case EE_KeyStuck:                               
    RetData = EEMemory.Parameter.KeyStuck ;
    break ;   
  case EE_KeyNotifiction:                               
    RetData = EEMemory.Parameter.KeyNotifiction ;
    break ;   
  case EE_DAPI_Server: 
    RetData = EEMemory.Parameter.DAPI_Server ;
    break ;   
  case EE_TZone: 
    RetData = EEMemory.Parameter.TimeZone ;
    break ;   
  case EE_Wireless: 
    RetData = EEMemory.Parameter.Wireless ;
    break ;  
  case EE_HJS_Switch: 
    RetData = EEMemory.Parameter.HJS_Switch ;
    break ;      
  case EE_HJS_CheckNumber: 
    RetData = EEMemory.Parameter.HJS_CheckNumber ;
    break ;   
  case EE_HJS_total: 
    RetData = EEMemory.Parameter.HJS_total ;
    break ;   
  case EE_xID: 
    RetData = EEMemory.Parameter.xID ;
    break ;  
  case EE_DemoMode:           
    RetData = EEMemory.Parameter.DemoMode ;
    break ;   
  case EE_Speaker:
    RetData = EEMemory.Parameter.Speaker ;
    break;    
  case EE_MaxDF_Elevation:
    RetData = EEMemory.Parameter.MaxDF_Elevation ;
    break; 
  case EE_Height:   
    RetData = EEMemory.Parameter.UserHeight ;
    break;    
  case EE_TVPower:   
    RetData = EEMemory.Parameter.TVPower ;
    break;   
  case EE_HRTSpeedBased:    
    RetData = EEMemory.Parameter.HRTSpeedBased ;
    break;  
  case EE_CommunicationTX:    
    RetData = EEMemory.Parameter.CommunicationTX ;
    break;   
  case EE_CommunicationRX:    
    RetData = EEMemory.Parameter.CommunicationRX ;
    break;   
  case EE_MaxVolume:            
    RetData = EEMemory.Parameter.MaxVolume ;
    break ;   
  case EE_OutputVolume:         
    RetData = EEMemory.Parameter.OutputVolume ;
    break ;   
  case EE_AM:                   
    RetData = EEMemory.Parameter.AM ;
    break;  
  case EE_Audio:                
    RetData = EEMemory.Parameter.Audio ;
    break;
  case EE_ConsoleVersion:       
    RetData = EEMemory.Parameter.ConsoleVersion ;
    break;
  case EE_FrameVersion:         
    RetData = EEMemory.Parameter.FrameVersion ;
    break;
  case EE_FrameSerialNumber:    
    RetData = EEMemory.Parameter.SerailNoFrame ;
    break;   
  case EE_SleepTime             :
    RetData = EEMemory.Parameter.SleepTime ;
    break ;   
  case EE_MAX_TIME              :
    RetData = EEMemory.Parameter.MaxTime ;
    break ;
  case EE_USER_TIME             :
    RetData = EEMemory.Parameter.UserTime ;
    break ;        
  case EE_WEIGHT                :
    RetData = EEMemory.Parameter.Weight ;
    break ;        
  case EE_LEVEL                 :
    RetData = EEMemory.Parameter.Level ;
    break ;        
  case EE_AGE                   :
    RetData = EEMemory.Parameter.Age ;
    break ;        
  case EE_UNIT                  :
    RetData = EEMemory.Parameter.Unit ;
    break ;  
  case EE_LOW_ELEVATION         :  
    RetData = EEMemory.Parameter.LowElevation ;
    break;
  case EE_MAX_ELEVATION         :    
    RetData = EEMemory.Parameter.MaxElevation ;
    break;                              
  case EE_ACCUMULATED_TIME      :
    RetData = EEMemory.Parameter.AccumulatedTime;//(u16)(EEMemory.Parameter.AccumulatedTime >> 16) ;
    break ;        
  case EE_ACCUMULATED_DISTANCE  :
    RetData = EEMemory.Parameter.AccumulatedDistance;//(u16)(EEMemory.Parameter.AccumulatedDistance >> 16) ;
    break ;        
  case EE_LANGUAGE              :
    RetData = EEMemory.Parameter.Language ;
    break ;  
  case EE_OutOfOrder            :
    RetData = EEMemory.Parameter.OutOfOrder ;
    break ;  
  case EE_InclineCalibrationFlag		:  
    RetData = EEMemory.Parameter.InclineClibrationFlag;
    break ;
  case EE_StartSpeed            :  
    RetData = EEMemory.Parameter.StartSpeed ;
    break ; 
  case EE_MaxSpeed              :  
    RetData = EEMemory.Parameter.MaxSpeed ;
    break ;                           
  case EE_PauseTime             :
    RetData = EEMemory.Parameter.PauseTime ;
    break ;        
  case EE_CHANNEL               :
    RetData = EEMemory.Parameter.Channel ;
    break ;        
  case EE_VOLUME                :
    RetData = EEMemory.Parameter.Volume ;
    break ;   
  case EE_TimerMode             :
    RetData = EEMemory.Parameter.TimeMode ;
    break ;                                     
  case EE_Logo                  :
    RetData = EEMemory.Parameter.Logo ;
    break ;  
  case EE_GENDER                :
    RetData = EEMemory.Parameter.Gender ;
    break ;      
  case EE_MaxRPM_Parameter      :   
    RetData = EEMemory.Parameter.MaxRPM_Parameter ;
    break ; 
  case EE_MinRPM_Parameter      :   
    RetData = EEMemory.Parameter.MinRPM_Parameter ;
    break ;                               
  case EE_BEEP                 :
    RetData = EEMemory.Parameter.Sound ;
    break ;                               
  case EE_MachineType           :
    RetData = EEMemory.Parameter.MachineType ;
    break ;        
  case EE_MachineType_LCB       :
    RetData = EEMemory.Parameter.MachineType_LCB ;
    break ;                              
  case EE_SerialNumber          : // serial no
    RetData = EEMemory.Parameter.SerialNo ;
    break ;                               
  case EE_NAVY_Test             : 
    RetData = EEMemory.Parameter.NAVY_Test ;
    break ;                                 
  case EE_RF_SERVICE            :
    RetData = EEMemory.Parameter.RFService ;
    break ;        
  case EE_CLUB_ID               :
    RetData = EEMemory.Parameter.ClubID ;
    break ;                               
  case EE_InUser                :
    RetData = EEMemory.Parameter.InUser ;
    break ;                                
  case EE_DigitalError_1        :
  case EE_DigitalError_2        :
  case EE_DigitalError_3        :
  case EE_DigitalError_4        :
  case EE_DigitalError_5        :
  case EE_DigitalError_6        :
  case EE_DigitalError_7        :
  case EE_DigitalError_8        :
  case EE_DigitalError_9        :
  case EE_DigitalError_10       :
    RetData = EEMemory.Parameter.ErrorList[by_Address - EE_DigitalError_1] ;
    break ;        
  case EE_ErrorCode             :
    RetData = EEMemory.Parameter.ErrorCode ;
    break ;    
  case EE_AP_Mode               :
    RetData = EEMemory.Parameter.AP_Mode;
    break ;        
  case EE_WiFi_Security         :
    wifi_settings.security.security_type = (WIFI_SECURITY_TYPE)EEMemory.Parameter.WiFi_Security;
    break;
  case EE_WiFi_SSID             :
    __memset( wifi_settings.security.ssid, 0, 33 );
    __strncpy(wifi_settings.security.ssid,EEMemory.Parameter.WiFi_SSID,33);        
    break;
  case EE_WiFi_Password         :
    __memset( wifi_settings.security.security_key, 0, 255 );
    __strncpy(wifi_settings.security.security_key,EEMemory.Parameter.WiFi_Password,255);              
    break;                                    
      /*                                    RunningBelt                                      */
      case EE_RunningBeltCurrentAccMiles:
                                    RetData = EEMemory.Parameter.RunningBeltCurrentAccMiles;
                                    break;
      case EE_RunningBeltCurrentAccHours:
                                    RetData = EEMemory.Parameter.RunningBeltCurrentAccHours;
                                    break;
      case EE_RunningBeltPreviousAccMiles:
                                    RetData = EEMemory.Parameter.RunningBeltPreviousAccMiles;
                                    break;
      case EE_RunningBeltPreviousAccHours:
                                    RetData = EEMemory.Parameter.RunningBeltPreviousAccHours;
                                    break;
      case EE_RunningBeltPreviousYear:
                                    RetData = EEMemory.Parameter.RunningBeltPreviousYear;
                                    break;
      case EE_RunningBeltPreviousMouth:
                                    RetData = EEMemory.Parameter.RunningBeltPreviousMouth;
                                    break;
      case EE_RunningBeltPreviousDay:
                                    RetData = EEMemory.Parameter.RunningBeltPreviousDay;
                                    break;
      /*                                    RunningBelt                                      */
      case EE_RunningDeckCurrentAccMiles:
                                    RetData = EEMemory.Parameter.RunningDeckCurrentAccMiles;
                                    break;
      case EE_RunningDeckCurrentAccHours:
                                    RetData = EEMemory.Parameter.RunningDeckCurrentAccHours;
                                    break;
      case EE_RunningDeckPreviousAccMiles:
                                    RetData = EEMemory.Parameter.RunningDeckPreviousAccMiles;
                                    break;
      case EE_RunningDeckPreviousAccHours:
                                    RetData = EEMemory.Parameter.RunningDeckPreviousAccHours;
                                    break;
      case EE_RunningDeckPreviousYear:
                                    RetData = EEMemory.Parameter.RunningDeckPreviousYear;
                                    break;
      case EE_RunningDeckPreviousMouth:
                                    RetData = EEMemory.Parameter.RunningDeckPreviousMouth;
                                    break;
      case EE_RunningDeckPreviousDay:
                                    RetData = EEMemory.Parameter.RunningDeckPreviousDay;
                                    break;
      /*                                    MCB                                      */
      case EE_MCBCurrentAccMiles:
                                    RetData = EEMemory.Parameter.MCBCurrentAccMiles;
                                    break;
      case EE_MCBCurrentAccHours:
                                    RetData = EEMemory.Parameter.MCBCurrentAccHours;
                                    break;
      case EE_MCBPreviousAccMiles:
                                    RetData = EEMemory.Parameter.MCBPreviousAccMiles;
                                    break;
      case EE_MCBPreviousAccHours:
                                    RetData = EEMemory.Parameter.MCBPreviousAccHours;
                                    break;
      case EE_MCBPreviousYear:
                                    RetData = EEMemory.Parameter.MCBPreviousYear;
                                    break;
      case EE_MCBPreviousMouth:
                                    RetData = EEMemory.Parameter.MCBPreviousMouth;
                                    break;
      case EE_MCBPreviousDay:
                                    RetData = EEMemory.Parameter.MCBPreviousDay;
                                    break;
      /*                                    MCB                                      */
      case EE_MoterCurrentAccMiles:
                                    RetData = EEMemory.Parameter.MoterCurrentAccMiles;
                                    break;
      case EE_MoterCurrentAccHours:
                                    RetData = EEMemory.Parameter.MoterCurrentAccHours;
                                    break;
      case EE_MoterPreviousAccMiles:
                                    RetData = EEMemory.Parameter.MoterPreviousAccMiles;
                                    break;
      case EE_MoterPreviousAccHours:
                                    RetData = EEMemory.Parameter.MoterPreviousAccHours;
                                    break;
      case EE_MoterPreviousYear:
                                    RetData = EEMemory.Parameter.MoterPreviousYear;
                                    break;
      case EE_MoterPreviousMouth:
                                    RetData = EEMemory.Parameter.MoterPreviousMouth;
                                    break;
      case EE_MoterPreviousDay:
                                    RetData = EEMemory.Parameter.MoterPreviousDay;
                                    break;
      /*                                    Headphone                                      */
      case EE_HeadphoneCurrentAccHours:
                                    RetData = EEMemory.Parameter.HeadphoneCurrentAccHours;
                                    break;
      case EE_HeadphoneCurrentInsertsTimes:
                                    RetData = EEMemory.Parameter.HeadphoneCurrentInsertsTimes;
                                    break;
      case EE_HeadphonePreviousAccHours:
                                    RetData = EEMemory.Parameter.HeadphonePreviousAccHours;
                                    break;
      case EE_HeadphonePreviousInsertsTimes:
                                    RetData = EEMemory.Parameter.HeadphonePreviousInsertsTimes;
                                    break;
      case EE_HeadphonePreviousYear:
                                    RetData = EEMemory.Parameter.HeadphonePreviousYear;
                                    break;
      case EE_HeadphonePreviousMouth:
                                    RetData = EEMemory.Parameter.HeadphonePreviousMouth;
                                    break;
      case EE_HeadphonePreviousDay:
                                    RetData = EEMemory.Parameter.HeadphonePreviousDay;
                                    break;
      /*                                    Prev_Maintenance                                      */
      case EE_Prev_MaintenanceCurrentAccMiles:
                                    RetData = EEMemory.Parameter.Prev_MaintenanceCurrentAccMiles;
                                    break;
      case EE_Prev_MaintenanceCurrentAccHours:
                                    RetData = EEMemory.Parameter.Prev_MaintenanceCurrentAccHours;
                                    break;
      case EE_Prev_MaintenancePreviousAccMiles:
                                    RetData = EEMemory.Parameter.Prev_MaintenancePreviousAccMiles;
                                    break;
      case EE_Prev_MaintenancePreviousAccHours:
                                    RetData = EEMemory.Parameter.Prev_MaintenancePreviousAccHours;
                                    break;
      case EE_Prev_MaintenancePreviousYear:
                                    RetData = EEMemory.Parameter.Prev_MaintenancePreviousYear;
                                    break;
      case EE_Prev_MaintenancePreviousMouth:
                                    RetData = EEMemory.Parameter.Prev_MaintenancePreviousMouth;
                                    break;
      case EE_Prev_MaintenancePreviousDay:
                                    RetData = EEMemory.Parameter.Prev_MaintenancePreviousDay;
                                    break;
  default                       :
    break ;
  }
  //
  return RetData ;
}


//===============================================================================================================================================
//====> 内存内部储存所有默认值
//===================================================
unsigned char EEPROM_Save_Initial(void)
{ 
  // Set default
  EEPROM_ParameterDefault(0) ;
  MemoryErrorStatus = EEPROM_WriteParameter() ;
  if( MemoryErrorStatus == _WriteError_ )
      {
      return C_FAIL ;
      }
  //
  return C_PASS ;
}



//===============================================================================================================================================
//====> 开机预设参数
//===================================================

extern u8 by_CheckInUserTimeNumber;//==>从 Digital.c 回传资料


void EEPROM_Read_Initial(void)
{
  u8 i;
 
  _AccMode = 0;
  // 2013.07.26
  _Display_Logo = EEPROM_ReadInformation(EE_Logo);
  // S001-02
  Console_SetDefaultVolume(EEPROM_ReadInformation(EE_VOLUME));
  Console_SetMaxVolume(EEPROM_ReadInformation(EE_MaxVolume));
  Console_SetOutputVolume(EEPROM_ReadInformation(EE_OutputVolume));
  //
  Console_MaxIncline(Data_Set,EEPROM_ReadInformation(EE_MaxDF_Elevation));
  //Console_SetAM_Status(EEPROM_ReadInformation(EE_AM));
  by_CheckInUserTimeNumber = EEPROM_ReadInformation(EE_InUser);
  if( by_CheckInUserTimeNumber == 0) by_CheckInUserTimeNumber = 0xFF;
  Console_NavyTestMode(EEPROM_ReadInformation(EE_NAVY_Test));//==>980810
  Console_SpeedUnit(Data_Set,EEPROM_ReadInformation(EE_UNIT));
  //Console_Weight(Data_Set,EEPROM_ReadInformation(EE_WEIGHT));
  Console_Information(Info_HEIGHT,Data_Set,EEPROM_ReadInformation(EE_Height));
  Console_Information(Info_WEIGHT,Data_Set,EEPROM_ReadInformation(EE_WEIGHT));
  Console_MaxTime(Data_Set,EEPROM_ReadInformation(EE_MAX_TIME)*60);
  //Console_Time(Data_Set,EEPROM_ReadInformation(EE_USER_TIME)*60);
  Console_Information(Info_TIME,Data_Set,EEPROM_ReadInformation(EE_USER_TIME)*60);
  if((EEPROM_ReadInformation(EE_MAX_ELEVATION) < EEPROM_ReadInformation(EE_LOW_ELEVATION))|| \
     (EEPROM_ReadInformation(EE_MAX_ELEVATION) - EEPROM_ReadInformation(EE_LOW_ELEVATION) < (120*128)))
  {// 当储存值跑掉时需要回默认值
      EEPROM_WriteInformation(EE_LOW_ELEVATION,25 * 128);
      EEPROM_WriteInformation(EE_MAX_ELEVATION,188 * 128);
  }
  //Console_Level(Data_Set,EEPROM_ReadInformation(EE_LEVEL));
  Console_Information(Info_LEVEL,Data_Set,EEPROM_ReadInformation(EE_LEVEL));
  //Console_AGE(Data_Set,EEPROM_ReadInformation(EE_AGE));
  Console_Information(Info_AGE,Data_Set,EEPROM_ReadInformation(EE_AGE));
  Console_MaxSpeed(Data_Set,EEPROM_ReadInformation(EE_MaxSpeed));
  Console_StartSpeed(Data_Set,EEPROM_ReadInformation(EE_StartSpeed));
  //Console_Gender(Data_Set,EEPROM_ReadInformation(EE_GENDER));
  Console_Information(Info_GENDER,Data_Set,EEPROM_ReadInformation(EE_GENDER));
  Console_Language(Data_Set,EEPROM_ReadInformation(EE_LANGUAGE));
  //Voice_SetSwitch(EEPROM_ReadInformation(EE_Speaker));
  BEEP_SetSwitch(EEPROM_ReadInformation(EE_BEEP));
  Console_SleepTime(Data_Set,EEPROM_ReadInformation(EE_SleepTime));
  // Incline
  Console_HighLowADC(Data_Set,0,EEPROM_ReadInformation(EE_LOW_ELEVATION));
  Console_HighLowADC(Data_Set,1,EEPROM_ReadInformation(EE_MAX_ELEVATION));
  // RPM
  Console_RPM_Parameter(1,1,EEPROM_ReadInformation(EE_MaxRPM_Parameter));// Set MaxRPM
  Console_RPM_Parameter(1,0,EEPROM_ReadInformation(EE_MinRPM_Parameter));// Set MinRPM
  //
  Console_HJS(Data_Set,EEPROM_ReadInformation(EE_HJS_total));
  //BLX_Set_RF_Service(EEPROM_ReadInformation(EE_RF_SERVICE));
  //BLX_Get_SerialNumber();
  //BLX_Get_ClubIDNumber();
  Console_Audio(Data_Set,EEPROM_ReadInformation(EE_Audio));
  CSAFE_SetInformation(_Set_CommunicationMode,Console_Audio(Data_Get,0));
  for(i = 0;i < 10;i++)
  {
      Console_DigitalError(Data_Set,i,EEPROM_ReadInformation(EE_DigitalError_1 +i));
  }
  
}

//===============================================================================================================================================
//====> 错误码参数储存
//===================================================
void EEPROM_SaveError(void)
{
  u8 i;  
  for(i = 0;i < 10;i++)
  {
      EEMemory.Parameter.ErrorList[i] = Console_DigitalError(Data_Get,i,0) ;
  }
  EEPROM_WriteParameter() ;
}
/*******************************************************************************
* Function Name  : EEPROM_AccumulatedMode
* Description    : 设定总累计参数型态
* Input          : by_Dat : 2-> Acc Time 1-> Acc Distance
* Output         : None
* Return         : None
*******************************************************************************/
void EEPROM_AccumulatedMode(char by_Dat)
{
    _AccMode = by_Dat;
}
//===============================================================================================================================================
//====> 内存内部ㄧ般储存参数
//====> 1.Address  2.储存参数
//===================================================
void EEPROM_SaveWorkInformation(u8 by_Address,u32 by_Dat)
{
  u32 w_Main=0;
 
  if(_AccMode == 0 && by_Address==EE_ACCUMULATED_TIME)
  {//==>运动结束用使用时间总累计
     if(by_Dat > 0)
     {
        w_Main = EEPROM_ReadInformation(EE_ACCUMULATED_TIME);
        w_Main += by_Dat;
        if((w_Main / 3600) > 999999)//==>当累计快要溢位时就自动归零
        {
           w_Main=0;
        }
        EEPROM_WriteInformation(EE_ACCUMULATED_TIME,w_Main);
     }
  }
  else if(_AccMode == 0 && by_Address==EE_ACCUMULATED_DISTANCE)
  {//==>运动结束用使用距离总累计
     if(by_Dat > 0)
     {
        w_Main=EEPROM_ReadInformation(EE_ACCUMULATED_DISTANCE);
        w_Main += by_Dat;
        if((w_Main / 1000) > 999999)//==>当累计快要溢位时就自动归零 //==>Max Dis.  99999Mile/160898Km
        {
           w_Main=0;
        }
        EEPROM_WriteInformation(EE_ACCUMULATED_DISTANCE,w_Main);
     }
  }
  else if(by_Address == EE_CommunicationTX)
  {
      w_Main = EEPROM_ReadInformation(EE_CommunicationTX);
      w_Main += by_Dat;
      if(w_Main > 9999999)
      {
          w_Main = 0;
      }
      EEPROM_WriteInformation(EE_CommunicationTX,w_Main);
  }
  else if(by_Address == EE_CommunicationRX )
  {
      w_Main = EEPROM_ReadInformation(EE_CommunicationRX);
      w_Main += by_Dat;
      if(w_Main > 9999999)
      {
          w_Main = 0;
      }
      EEPROM_WriteInformation(EE_CommunicationRX,w_Main);
  }
  else
  {
     EEPROM_WriteInformation(by_Address,by_Dat);
  }

}

//------------------------------------------------------------------------------


unsigned char EEPROM_Initial(void) 
{
  unsigned short CheckData ;
  unsigned short CheckLength ;
  unsigned short CheckSum ;
  unsigned long NowAddress ;
  unsigned char i ;
  //
  IO_Set_EEPROM_CS();
  IO_Clear_EEPROM_SK();
  IO_Set_EEPROM_DI();
  //
  #ifdef      _Clear_All_
  Mx25L1606E_SectorErase(0) ;
  Mx25L1606E_SectorErase(1) ;
  #endif
  //
  //
  MemoryErrorStatus = _WriteOK_ ;
  NowMemoryBlockNo = 0 ;
  MemoryStatus = _Empty_ ; 
  // Sector 0
  // Check Block number 0~3
  for( i = 0 ; i < _NumberOfBlock_ ;i++ )
      {
      // Calculator Memory Address  
      if( i != 0 )
          NowAddress = i * _Sector0_Block1_  ;
      else
          NowAddress = _Sector0_Block0_ ;
      //------------------------------------------------------------------------
      CheckData = Mx25L1606E_ReadWord(NowAddress) ;
      if( CheckData == (_BlockIndex_+i) )
          {
          CheckSum = Mx25L1606E_ReadWord((NowAddress+EE_CheckSum)) ;
          if( EEPROM_CheckEEMemoryDataStatus((NowAddress+EE_DataStart),(_Sector_BlockSize_-EE_DataStart),CheckSum) == 0 )
              {
              CheckLength = Mx25L1606E_ReadWord(NowAddress+EE_ParameterNo) ;  
              if( CheckLength == EE_ParameterMax )
                  {
                  NowMemoryBlockNo = CheckData - _BlockIndex_ + 1 ;
                  MemoryStatus = _NoEmpty_ ;
                  }
              }
          }
      else
          {
          if(( NowMemoryBlockNo != 0 ) && (CheckData == 0xFFFF ) )
              break ;
          }
      }
  //
  if( MemoryStatus == _NoEmpty_ )
      {
      //  
      if( (NowMemoryBlockNo-1) != 0 )
          NowAddress = (NowMemoryBlockNo-1) * _Sector0_Block1_  ;
      else
          NowAddress = _Sector0_Block0_ ;
      //
      Mx25L1606E_ReadBlock(NowAddress,_Sector_BlockSize_,&EEMemory.Block[0]) ;
      EEPROM_Read_Initial() ;
      return C_PASS ;
      }
  else
      {
      // Set default
      NowMemoryBlockNo = 0 ;
      EEPROM_ParameterDefault(0) ;
      //
      if( EEPROM_CheckSectorErase(_Sector0_Block0_) == 1 )
          Mx25L1606E_SectorErase(0) ;
      
      if( EEPROM_CheckSectorErase(_Sector1_Block0_) == 1 )
          Mx25L1606E_SectorErase(1) ;
      //
      MemoryErrorStatus = EEPROM_WriteParameter() ;
      //
      }
  //
  return C_FAIL ;
}


//------------------------------------------------------------------------------------------------------      
void EEPROM_ParameterDefault(char by_D)
{
  if(by_D == 0)
  {// 非手动重置参数
      unsigned short i ;
      
      for( i = 0 ; i < _Sector_BlockSize_ ; i++ )
      {
          EEMemory.Block[i] = 0 ;
      }
      EEMemory.Parameter.AccumulatedTime = 0 ; 
      EEMemory.Parameter.AccumulatedDistance = 0 ; 
      EEMemory.Parameter.SerialNo = 0 ;
      EEMemory.Parameter.SerailNoFrame = 0;
      EEMemory.Parameter.ClubID = 63 ; //==>63:JHT 58:NARD   
      EEMemory.Parameter.MachineType = MachineType_TM ;// 自定义TM代表码
      EEMemory.Parameter.MachineType_LCB = LCB_DELTA_C3;
      for(i=0;i<10;i++)
      {
          EEMemory.Parameter.ErrorList[i] = 0 ;
      }
      //EEMemory.Parameter.RFService = RF_Service_None ;  
      // Incline
      EEMemory.Parameter.LowElevation = 31 * 128 ;//peter modify
      EEMemory.Parameter.MaxElevation = 188 * 128 ;
      // RPM
      EEMemory.Parameter.MaxRPM_Parameter = DELTA_MaxRPM;
      EEMemory.Parameter.MinRPM_Parameter = DELTA_MinRPM;
      //
      EEMemory.Parameter.ConsoleVersion = 0;// 第0版 
      EEMemory.Parameter.FrameVersion = 0;// 第0版 
      //
      EEMemory.Parameter.HJS_total = 0;
      
      EEMemory.Parameter.RunningBeltCurrentAccMiles = 0;
      EEMemory.Parameter.RunningBeltCurrentAccHours = 0;
      EEMemory.Parameter.RunningBeltPreviousAccMiles = 0;
      EEMemory.Parameter.RunningBeltPreviousAccHours = 0;
      EEMemory.Parameter.RunningBeltPreviousYear = 2017;
      EEMemory.Parameter.RunningBeltPreviousMouth = 1;
      EEMemory.Parameter.RunningBeltPreviousDay = 1;
      
      EEMemory.Parameter.RunningDeckCurrentAccMiles = 0;
      EEMemory.Parameter.RunningDeckCurrentAccHours = 0;
      EEMemory.Parameter.RunningDeckPreviousAccMiles = 0;
      EEMemory.Parameter.RunningDeckPreviousAccHours = 0;
      EEMemory.Parameter.RunningDeckPreviousYear = 2017;
      EEMemory.Parameter.RunningDeckPreviousMouth = 1;
      EEMemory.Parameter.RunningDeckPreviousDay = 1;
      
      EEMemory.Parameter.MCBCurrentAccMiles = 0;
      EEMemory.Parameter.MCBCurrentAccHours = 0;
      EEMemory.Parameter.MCBPreviousAccMiles = 0;
      EEMemory.Parameter.MCBPreviousAccHours = 0;
      EEMemory.Parameter.MCBPreviousYear = 2017;
      EEMemory.Parameter.MCBPreviousMouth = 1;
      EEMemory.Parameter.MCBPreviousDay = 1;
      
      EEMemory.Parameter.MoterCurrentAccMiles = 0;
      EEMemory.Parameter.MoterCurrentAccHours = 0;
      EEMemory.Parameter.MoterPreviousAccMiles = 0;
      EEMemory.Parameter.MoterPreviousAccHours = 0;
      EEMemory.Parameter.MoterPreviousYear = 2017;
      EEMemory.Parameter.MoterPreviousMouth = 1;
      EEMemory.Parameter.MoterPreviousDay = 1;
      
      EEMemory.Parameter.HeadphoneCurrentAccHours = 0;
      EEMemory.Parameter.HeadphoneCurrentInsertsTimes = 0;
      EEMemory.Parameter.HeadphonePreviousAccHours = 0;
      EEMemory.Parameter.HeadphonePreviousInsertsTimes = 0;
      EEMemory.Parameter.HeadphonePreviousYear = 2017;
      EEMemory.Parameter.HeadphonePreviousMouth = 1;
      EEMemory.Parameter.HeadphonePreviousDay = 1;
      
      EEMemory.Parameter.Prev_MaintenanceCurrentAccMiles = 0;
      EEMemory.Parameter.Prev_MaintenanceCurrentAccHours = 0;
      EEMemory.Parameter.Prev_MaintenancePreviousAccMiles = 0;
      EEMemory.Parameter.Prev_MaintenancePreviousAccHours = 0;
      EEMemory.Parameter.Prev_MaintenancePreviousYear = 2017;
      EEMemory.Parameter.Prev_MaintenancePreviousMouth = 1;
      EEMemory.Parameter.Prev_MaintenancePreviousDay = 1;
  }  
  // start v2.1-14
  EEMemory.Parameter.AutoUpdateSwitch = D_Enable;
  EEMemory.Parameter.AutoUpdateTime = 1;//凌晨 AM 1:00
  EEMemory.Parameter.AutoUpdateDate = 99;//初始设99作为第一次启动仪表判断基准
  // end v2.1-14  
  EEMemory.Parameter.SSL = _ON;// v2.1-11 
  EEMemory.Parameter.DAPI_Server = 3;//v2.2-8.1=> 0:Dev 1:Staging 2:QA 3:Production
  EEMemory.Parameter.USB_Protection = D_Enable;
  EEMemory.Parameter.KeyStuck = D_Enable;
  EEMemory.Parameter.KeyNotifiction = D_Enable;
  EEMemory.Parameter.TimeZone = _DefaultTimeZone;//(GMT-06:00)Central Time(US and Canada)
  EEMemory.Parameter.Wireless = D_Disable;
  EEMemory.Parameter.xID = D_Disable;
  EEMemory.Parameter.AM = D_Disable;
  EEMemory.Parameter.UserHeight = _Height_Mile_DF;
  EEMemory.Parameter.HJS_Switch = D_Enable;
  EEMemory.Parameter.HJS_CheckNumber = 30000;
  EEMemory.Parameter.MaxDF_Elevation = 150;// 15%
  EEMemory.Parameter.TVPower = _OFF;
  EEMemory.Parameter.Logo = Logo_MATRIX;
  EEMemory.Parameter.HRTSpeedBased = 0;// OFF
  EEMemory.Parameter.CommunicationTX = 0;
  EEMemory.Parameter.CommunicationRX = 0;
  EEMemory.Parameter.Channel = 1;
  EEMemory.Parameter.DemoMode = D_Disable;
  EEMemory.Parameter.Volume = 15 ;    
  EEMemory.Parameter.MaxVolume = 32 ;    
  EEMemory.Parameter.OutputVolume = 13 ; 
  EEMemory.Parameter.Audio = Audio_OFF; 
  EEMemory.Parameter.SleepTime = 0;//(分) EUPs  
  EEMemory.Parameter.MaxTime = 60;//3600 ;                      
  EEMemory.Parameter.UserTime = 20;//1200 ;                  
  EEMemory.Parameter.TimeMode = Time_UP ;
  EEMemory.Parameter.Weight = 150 ;  
  EEMemory.Parameter.Level = 1 ;  
  EEMemory.Parameter.Age = 30 ;   
  EEMemory.Parameter.Language = L_English ;  
  EEMemory.Parameter.Unit = C_MILE ;    
  EEMemory.Parameter.OutOfOrder = _OFF ; 
  EEMemory.Parameter.ErrorCode = D_Enable ;
  EEMemory.Parameter.Gender = D_Male ; 
  EEMemory.Parameter.PauseTime = 300 ;
  EEMemory.Parameter.InUser = 30 ;//==>人员侦测秒数 (0xff => Close) 
  EEMemory.Parameter.NAVY_Test = 0 ; 
  EEMemory.Parameter.MaxSpeed = Param_MaxSpeed_MPH_DF ;//==>12.0MPH   D_MaxSpeed_MPH
  EEMemory.Parameter.StartSpeed = 5 ;//==>0.5MPH 
  EEMemory.Parameter.Sound = _ON ;//==>1:ON 2:OFF  
  EEMemory.Parameter.Speaker = _OFF;
  EEMemory.Parameter.AP_Mode = _OFF;
  __memset( EEMemory.Parameter.WiFi_SSID, 0, 33 );
  __memset( EEMemory.Parameter.WiFi_Password, 0, 255 );
  __strncpy(EEMemory.Parameter.WiFi_SSID,Default_SSID,strlen(Default_SSID));//wifi SSID
  __strncpy(EEMemory.Parameter.WiFi_Password,Default_Key,strlen(Default_Key));//wifi Password
  EEMemory.Parameter.WiFi_Security = 4;// WPA2-PSK default security pattern
  EEMemory.Parameter.RSCU_SoftwareVersion = 0;// v2.2-7
  EEMemory.Parameter.InclineClibrationFlag = 1;
  if(by_D == 1)
  {// 手动重置参数
      EEPROM_WriteParameter() ;
      Console_SpeedUnit(Data_Set,C_MILE);
  }
  
  return ;
}

//------------------------------------------------------------------------------
unsigned char EEPROM_WriteParameter(void)
{
 unsigned long WriteAddress ;
 unsigned char RetryCount ;
 RetryCount = 0 ;
   if( NowMemoryBlockNo >= 8 )
      NowMemoryBlockNo = 1 ;
  else
      NowMemoryBlockNo += 1 ;
  //
  if( NowMemoryBlockNo == 0 )
      NowMemoryBlockNo = 1 ;
  //
  EEMemory.Parameter.MLength = EE_ParameterMax ;
  EEMemory.Parameter.CheckNo = _BlockIndex_ + (NowMemoryBlockNo-1) ;   
  EEMemory.Parameter.CheckSum = EEPROM_CalculatorChecksum( &EEMemory.Block[EE_DataStart],(_Sector_BlockSize_-EE_DataStart) ) ; 
 do
     {
     switch( NowMemoryBlockNo )
         {
         default  :
                  WriteAddress = _Sector0_Block0_ ;
                  NowMemoryBlockNo = 1 ;
                  //
                  if( EEPROM_CheckSectorErase(WriteAddress) == 1 )
                      Mx25L1606E_SectorErase(0) ;
                  //
                  break ;
         case 2   :
                  WriteAddress = _Sector0_Block1_ ;
                  break ;
         case 3   :
                  WriteAddress = _Sector0_Block2_ ;
                  break ;
         case 4   :
                  WriteAddress = _Sector0_Block3_ ;
                  break ;
         case 5   :
                  WriteAddress = _Sector1_Block0_ ;
                  //
                  if( EEPROM_CheckSectorErase(WriteAddress) == 1 )
                      Mx25L1606E_SectorErase(1) ;
                  //
                  break ;
         case 6   :
                  WriteAddress = _Sector1_Block1_ ;
                  break ;
         case 7   :
                  WriteAddress = _Sector1_Block2_ ;
                  break ;
         case 8   :
                  WriteAddress = _Sector1_Block3_ ;
                  break ;
         }
     //
     Mx25L1606E_WriteBlock((WriteAddress+EE_DataStart),(_Sector_BlockSize_-EE_DataStart),&EEMemory.Block[EE_DataStart]) ;
     Mx25L1606E_WriteBlock(WriteAddress,EE_BlockIDSize,&EEMemory.Block[0]) ;
     
     if( EEPROM_VerifyData(WriteAddress,&EEMemory.Block[0],_Sector_BlockSize_) == _WriteError_ )
          {
          RetryCount += 1 ;
          if( NowMemoryBlockNo >= 8 )
              NowMemoryBlockNo = 1 ;
          else
              NowMemoryBlockNo += 1 ;
          // for Check Memory
          EEMemory.Parameter.CheckNo = _BlockIndex_+ ( NowMemoryBlockNo-1 );   
          EEMemory.Parameter.CheckSum = EEPROM_CalculatorChecksum( &EEMemory.Block[EE_DataStart],(_Sector_BlockSize_-EE_DataStart) ) ;    
          EEMemory.Parameter.MLength = EE_ParameterMax ;
          //
          if( NowMemoryBlockNo == _Sector1_StartNo_ )
              {
              if( EEPROM_CheckSectorErase(_Sector1_Block0_) == 1 )
                  Mx25L1606E_SectorErase(1) ;
              }
          else
              {
              if( NowMemoryBlockNo == _Sector0_StartNo_ )
                  {
                  if( EEPROM_CheckSectorErase(_Sector0_Block0_) == 1 )
                      Mx25L1606E_SectorErase(0) ;
                  }
              }
          //
          }
     else
          {
          if( NowMemoryBlockNo > _Sector1_StartNo_ )
              {
              if( EEPROM_CheckSectorErase(_Sector0_Block0_) == 1 )
                  Mx25L1606E_SectorErase(0) ;
              }
          else
              {
              if( NowMemoryBlockNo != _Sector1_StartNo_ )
                  {
                  if( NowMemoryBlockNo > _Sector0_StartNo_ )
                      {
                      if( EEPROM_CheckSectorErase(_Sector1_Block0_) == 1 )
                          Mx25L1606E_SectorErase(1) ;
                      }
                  }
              }
          return _WriteOK_ ;
          }
     } while( RetryCount < 5 ) ;
 //
 return _WriteError_ ;
}


unsigned char EEPROM_CheckSectorErase( unsigned long SetcorAddr )
{
  //
  unsigned short i ;
  for( i = 0 ; i < _SectorBase_ ; i++ )
      {
      if( 0xFF != Mx25L1606E_ReadByte((SetcorAddr+i)) )
          return 1 ;
      }
  //
  return 0 ;
}

//
unsigned char EEPROM_VerifyData( unsigned long WriteAddress, unsigned char *Dptr, unsigned short size ) 
{
  unsigned short i ;
  for( i = 0 ; i < size ; i++ )
      {
      if( *(Dptr+i) != Mx25L1606E_ReadByte((WriteAddress+i)) )
          return _WriteError_ ;
      }
  return _WriteOK_ ;  
}


//------------------------------------------------------------------------------------------------------
unsigned short EEPROM_CalculatorChecksum( unsigned char *Dptr, unsigned short size )
{
  unsigned short CheckSum ;
  unsigned short i ;
  CheckSum = 0 ;
  for( i = 0 ; i < size ; i++ )
      {
      CheckSum += *(Dptr+i) ;
      }
  //
  CheckSum = 0xFFFF - CheckSum ;
  //
  return CheckSum ;
}

//------------------------------------------------------------------------------------------------------
unsigned char EEPROM_CheckEEMemoryDataStatus( unsigned long Maddr, unsigned short size, unsigned short CheckData )
{
  unsigned short CheckSum  ;
  unsigned short i ;
  CheckSum = 0 ;
  for( i = 0; i < size ;i++ )
      {
      CheckSum += Mx25L1606E_ReadByte((Maddr+i)) ;
      }
  //
  if( (CheckSum + CheckData) != 0xFFFF )
      return 1 ;
  //
  return 0 ;
}


//------------------------------------------------------------------------------
unsigned char EEPROM_GetMemoryErrorStatus(void)
{
  if( MemoryErrorStatus == _WriteOK_ )
      {
      return  0 ;
      }
  return 1 ;
}



//------------------------------------------------------------------------------
unsigned char EEPROM_ExportEEMemoryData(unsigned short Addr ,unsigned char *ExportData)
{
  if( Addr >= _Sector_BlockSize_ )
      return 0 ;

  *ExportData = EEMemory.Block[Addr] ;
  return 1 ;
}


//-----------------------------------------------------------------------------------
unsigned char EEPROM_InportEEMemoryData( unsigned short Addr, unsigned char InportData )
{
  if( Addr >= _Sector_BlockSize_ )
      return 0 ;

  EEMemory.Block[Addr] = InportData ;
  return 1 ;
}



