#include "wifi_define.h"
#include "HT1381.h"
//#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "Console.h"
#include "heart.h"
#include "EEPRom.h"
#include "UsbFunction.h"
#include "wifi_interface.h"
#include "WiFi_Information.h"
#include "Timer.h"
#include "General.h"
#include "Digital.h"
#include "C_safe.h"

LSFlashConfig *machineConfig;// AM system Data
LSFlashUserConfig *userFlashConfig[MAX_USERS];//+1

const struct
{
   char* Name;        // 時區名稱
}_TimeZone[_TimeZoneMax] = 
{
  "Asia/Kabul",         // 1
  "America/Anchorage",
  "Asia/Kuwait",
  "Asia/Muscat",
  "Asia/Baghdad",
  "America/Halifax",    // 6
  "Australia/Darwin",
  "Australia/Sydney",
  "Asia/Baku",
  "Atlantic/Azores",
  "America/Edmonton",   // 11
  "Atlantic/Cape_Verde",
  "Asia/Yerevan",
  "Australia/Adelaide",
  "America/Costa_Rica",
  "Asia/Almaty",        // 16
  "America/Cuiaba",
  "Europe/Belgrade",
  "Europe/Belgrade",
  "Pacific/Guadalcanal",
  "America/Chicago",    // 21 
  "America/Monterrey",
  "Asia/Shanghai",
  "Africa/Nairobi",
  "Australia/Brisbane",
  "Europe/Minsk",       // 26
  "America/Sao_Paulo",
  "America/New_York",
  "Africa/Cairo",
  "Asia/Yekaterinburg",
  "Pacific/Fiji",       // 31
  "Europe/Riga",
  "Asia/Tbilisi",
  "Europe/London",
  "America/Godthab",
  "Africa/Monrovia",    // 36 
  "Europe/Istanbul",
  "Pacific/Honolulu",
  "Asia/Calcutta",
  "Asia/Tehran",
  "Asia/Jerusalem",     // 41
  "Asia/Seoul",
  "Atlantic/South_Georgia",
  "America/Denver",
  "America/Chihuahua",
  "Asia/Rangoon",       // 46
  "Asia/Novosibirsk",
  "Africa/Windhoek",
  "Asia/Kathmandu",
  "Pacific/Auckland",
  "America/St_Johns",   // 51
  "Asia/Irkutsk",
  "Asia/Krasnoyarsk",
  "America/Santiago",
  "America/Tijuana",
  "Europe/Paris",       // 56
  "Europe/Moscow",
  "America/Argentina/Buenos_Aires",
  "America/Bogota",
  "America/La_Paz",
  "Pacific/Apia",       // 61
  "Asia/Bangkok",
  "Asia/Singapore",
  "Africa/Harare",
  "Asia/Colombo",
  "Asia/Taipei",        // 66
  "Australia/Hobart",
  "Asia/Tokyo",
  "Pacific/Tongatapu",
  "America/Indianapolis",
  "America/Phoenix",    // 71 
  "Asia/Vladivostok",
  "Australia/Perth",
  "Africa/Lagos",
  "Europe/Berlin",
  "Asia/Tashkent",      // 76
  "Pacific/Guam",
  "Asia/Yakutsk"
};

LSFlashConfig wifiMachineConfig;
LSFlashUserConfig wifiUser;

char _NowWorkoutStatus;
char _WorkoutFinish;
//==============================================================================
// to init machineConfig userFlashConfig 
//==============================================================================
void WiFi_Set_MachineInformation(void)
{
  SHT1381_RealTime now;
  HT1381_RealTime( &now );
    
  machineConfig = &wifiMachineConfig;
  userFlashConfig[0] = &wifiUser;
  machineConfig->brandid = MAXTIX;
  WiFi_Information_MachineTypeDefaultCheck();// 依據機種別進行參數預設設定
  machineConfig->csafeId = (unsigned char)CSAFE_GetInformation(_Get_CID);
  machineConfig->csafeManufactureId = (unsigned char)CSAFE_GetInformation(_Get_Manufacturer);
  machineConfig->csafeModelId = CSAFE_Model;
  machineConfig->clubId = EEPROM_ReadInformation(EE_CLUB_ID);
  machineConfig->accumulatedDistanceMilesX100 = EEPROM_ReadInformation(EE_ACCUMULATED_DISTANCE)/10;
  machineConfig->accumulatedSeconds = EEPROM_ReadInformation(EE_ACCUMULATED_TIME);
  machineConfig->headphoneInsertion = EEPROM_ReadInformation(EE_HJS_total);
  machineConfig->maxWorkoutTimeMinutes = EEPROM_ReadInformation(EE_MAX_TIME);
  machineConfig->defaultWorkoutTimeMinutes = EEPROM_ReadInformation(EE_USER_TIME);
  machineConfig->defaultAge = EEPROM_ReadInformation(EE_AGE);
  machineConfig->defaultWeightX10 = EEPROM_ReadInformation(EE_WEIGHT)*10; // 0.1 lbs
  machineConfig->pauseTimeMinutes = EEPROM_ReadInformation(EE_PauseTime);
  machineConfig->gender = EEPROM_ReadInformation(EE_GENDER);
  machineConfig->targetHR = Heart_Get_Target();
  machineConfig->units = EEPROM_ReadInformation(EE_UNIT);
  machineConfig->isAMsystem = EEPROM_ReadInformation(EE_AM);
  machineConfig->LcbVersion = Digtial_Get_MCBVersion();
  machineConfig->volume = EEPROM_ReadInformation(EE_OutputVolume);
  
  WiFi_memset( machineConfig->timeZone, 0, 65 );
  strcpy( machineConfig->timeZone,_TimeZone[WiFi_Information_TimeZoneCheck(EEPROM_ReadInformation(EE_TZone))].Name); //strcpy( machineConfig->timeZone, "America/Chicago" );
  
  //
    userFlashConfig[0]->lastUpdateYear = 2000 + now.Year;
    userFlashConfig[0]->lastUpdateMonth = now.Month;
    userFlashConfig[0]->lastUpdateDay = now.Day;
    userFlashConfig[0]->lastUpdateHour = now.Hour;
    userFlashConfig[0]->lastUpdateMinute = now.Minute;
    userFlashConfig[0]->lastUpdateSecond = now.Second;
  //
}
void WiFi_Set_MachineInformation_Initial(void)
{
  //unsigned char by_Bu;
  SHT1381_RealTime now;
  HT1381_RealTime( &now );
  machineConfig = &wifiMachineConfig;
  userFlashConfig[0] = &wifiUser;
  // machine  
  WiFi_memset( machineConfig, 0 , sizeof( LSFlashConfig ));
  machineConfig->maxMPHx10 = Param_MaxSpeed_MPH_Max;
  machineConfig->minMPHx10 = Param_StartSpeed_MPH_Min;
  machineConfig->maxKPHx10 = Param_MaxSpeed_KPH_Max;
  machineConfig->minKPHx10 = Param_StartSpeed_KPH_Min;
  machineConfig->maxInclinex10 = 150;
  machineConfig->maxUser = 1;
  machineConfig->demoMode = 0;//EEPROM_ReadInformation(EE_DemoMode);
  machineConfig->energySaver = 0;//(EEPROM_ReadInformation(EE_DemoMode) != 0 ? 1:0);
  WiFi_memset( machineConfig->ecbCal, 0, 30 );
  machineConfig->defaultWarmupTimeMinutes = 2;
  machineConfig->defaultCooldownTimeMinutes = 2;
  machineConfig->brightness = 100;
  machineConfig->languageId = 0;
  machineConfig->timeFormat = 0;
  machineConfig->Autologin = 0;
  machineConfig->wifiisAPMode = 1;
  machineConfig->constantWatts = 0;
  WiFi_memset( machineConfig->machineIdentification, 0, 36 );
  strcpy( machineConfig->machineIdentification, DEFAULT_UUID );
  WiFi_memset( machineConfig->programUUID, 0, 36 );
  strcpy( machineConfig->programUUID, DEFAULT_UUID );
  // software version
  sprintf(machineConfig->softwareVersion, "%d%d%d.%d%d%d.000", UCB_Software_Ver/100, (UCB_Software_Ver%100)/10, UCB_Software_Ver%10, UCB_Software_Rel/100, (UCB_Software_Rel%100)/10, UCB_Software_Rel%10);
  WiFi_memset( (machineConfig->softwareVersion+WiFi_CountChar(machineConfig->softwareVersion, 20)), ' ', 20 - WiFi_CountChar(machineConfig->softwareVersion, 20) );
  // OS version
  WiFi_memset( machineConfig->osVersion, 0, 20 );
  strcpy( machineConfig->osVersion, DEFAULT_OSVERSION );
  // user
  userFlashConfig[0]->userConsoleId = 0;
  userFlashConfig[0]->UserSetup = 0;
  WiFi_memset( userFlashConfig[0]->xid, 0, XIDMAXLENGTH );
  WiFi_memset( userFlashConfig[0]->passcode, 0, 4 );
  userFlashConfig[0]->weightX10 = machineConfig->defaultWeightX10; // unit: 0.1lbs
  userFlashConfig[0]->age = EEPROM_ReadInformation(EE_AGE);
  userFlashConfig[0]->birth_year = 2000 + now.Year - userFlashConfig[0]->age;
  userFlashConfig[0]->birth_month = now.Month;
  userFlashConfig[0]->birth_day = now.Day;
  userFlashConfig[0]->gender = EEPROM_ReadInformation(EE_GENDER);
  userFlashConfig[0]->language = EEPROM_ReadInformation(EE_LANGUAGE);
  
  WiFi_memset( userFlashConfig[0]->name, 0, 65 );
  userFlashConfig[0]->DisplayUnits = 0;
  userFlashConfig[0]->AcceptedTerms = 0;
  userFlashConfig[0]->WorkoutDataShare = 0;
  userFlashConfig[0]->LoggedOn = 0;
  userFlashConfig[0]->isLastUser = 0;
  // AM
  machineConfig->LubeBeltMilesX10 = 0;
  machineConfig->MotorTemperature = 50;
  machineConfig->BusCurrent = 5;
  machineConfig->BusVoltage = 12;
}

void WiFi_Information_Initial(void)
{
  WiFi_Set_MachineInformation_Initial();
  WiFi_Set_MachineInformation();
  // 2015.02.02
  WiFi_Information_ChangeDAPI_Server(_OFF);
  //
  Timer_Counter_Clear(_Time_WiFiConnectCheck);
  // wifi module 重置需設Low至少300ms後再設Hi
  IO_WiFi_ResetLow();
  while(Timer_Counter(T_STEP,_Time_WiFiConnectCheck,5) == 0);
  IO_WiFi_ResetHi();// WiFi Reset Pin  
  //
  Timer_Counter_Clear(_Time_WiFiConnectCheck);
}


//==============================================================================
//to save userFlashConfig[5] 0--->guest not save
//==============================================================================
void WiFi_Information_SaveUserConfig(void)
{
  // 使用者資訊載入儀表
  WiFi_Information_Set(_WiFi_Unit,userFlashConfig[0]->DisplayUnits);
  if(userFlashConfig[0]->DisplayUnits == C_KM)
  {// 因DAPI會自行轉成英制回傳,所以需要自行轉回公制
      WiFi_Information_Set(_WiFi_Weight,((unsigned short)((float)(userFlashConfig[0]->weightX10/2.2))/10));
  }
  else
  {
      WiFi_Information_Set(_WiFi_Weight,userFlashConfig[0]->weightX10/10);
  }
  WiFi_Information_Set(_WiFi_Age,userFlashConfig[0]->age);
  WiFi_Information_Set(_WiFi_Gender,userFlashConfig[0]->gender);
  
  //
}

//==============================================================================
// WiFi參數設定至儀表
//==============================================================================
void WiFi_Information_Set(unsigned char by_Item,unsigned short by_Dat)
{
  switch(by_Item)
  {
      case _WiFi_Height:
               Console_Information(Info_HEIGHT,Data_Set,by_Dat);
               break;
      case _WiFi_Weight:
               Console_Information(Info_WEIGHT,Data_Set,by_Dat);
               break;
      case _WiFi_Age:
               Console_Information(Info_AGE,Data_Set,by_Dat);
               break;
      case _WiFi_Unit:
               Console_SpeedUnit(Data_Set,by_Dat); 
               if(by_Dat == C_KM)
               {
                   Console_StartSpeed(Data_Set,8);
                   Console_MaxSpeed(Data_Set,Param_MaxSpeed_KPH_DF);
               }
               else  
               {
                   Console_StartSpeed(Data_Set,5);
                   Console_MaxSpeed(Data_Set,Param_MaxSpeed_MPH_DF);
               }
               break;   
      case _WiFi_Gender:         
               Console_Information(Info_GENDER,Data_Set,by_Dat);
               break;
      case _WiFi_Incline:
               //Console_Information(Info_WorkoutIncline,Data_Set,by_Dat*2/10);
               break; 
      case _WiFi_WorkoutState:
               // 運動狀態設定
               // 0=Not Workout ; 1=Workout
               Console_WorkoutStatus(Data_Set,by_Dat);
               break;
      case _WiFi_ResistanceLevel:
               break; 
  }
}

//==============================================================================
// WiFi 取儀表參數
// 如果沒有支援請回 0
//==============================================================================
unsigned short WiFi_Information_Get(unsigned char by_Item)
{
  unsigned char _Age;
  unsigned short by_Bu;
  
  SHT1381_RealTime today;//==>HT1381時間IC Data指向
  
  switch(by_Item)
  {
      case _WiFi_AvgHeartRate:
               return CSAFE_GetInformation(_Get_HRAvg);
      case _WiFi_Height:
               return Console_Information(Info_HEIGHT,Data_Get,0); 
      case _WiFi_Weight:
               return Console_Information(Info_WEIGHT,Data_Get,0); 
      case _WiFi_Age:
               return Console_Information(Info_AGE,Data_Get,0); 
      case _WiFi_Unit:
               return Console_SpeedUnit(Data_Get,0); 
      case _WiFi_Gender: 
               return Console_Information(Info_GENDER,Data_Get,0);
      case _WiFi_Speed:
               // unit : 0.1 mile/hr
               return Console_Information(Info_WorkoutSpeed,Data_Get,0)/10;
      case _WiFi_Incline:
               // unit: 0.1%
               return Console_Information(Info_WorkoutIncline,Data_Get,0) * 10 / 2;
      case _WiFi_Calories:
               // Unit : Calorie
               return Console_Get_Calories();
      case _WiFi_CaloriesPerHour:
               // Unit : Calorie/Hr
               return Console_Get_Calories_Hour(); 
      case _WiFi_Distance:
               // Unit : 0.01 mile
               return (Console_Get_Distance()+5)/10;//rounding 
      case _WiFi_HeartRate:
               // unit: BPM
               return Heart_Get_Heart();
      case _WiFi_THR:    
               return Heart_Get_Target();
      case _WiFi_ProgramLevel:
               // unit: Level 
               return Console_Information(Info_LEVEL,Data_Get,0);
      case _WiFi_Watts:         
               return ((unsigned short)((float)Console_Get_Calories_Hour() / 4.2));
      case _WiFi_METs:
               // unit : 0.1 METs   
               return Console_Get_METs();
      case _WiFi_WorkoutState:
               // 0 = Warmup, 1 = Workout, 2 = Cooldown, 3 = Pause
               return _NowWorkoutStatus; 
      case _WiFi_TimeInState:
               // the elapsed time for every state
               if( _NowWorkoutStatus == _WiFi_WorkoutState_WarmUp)
                   return Console_Parameter(Info_WarmUpAccTime);
               else if( _NowWorkoutStatus == _WiFi_WorkoutState_CoolDown)
                   return Console_Parameter(Info_CoolDwonAccTime);
               else if( _NowWorkoutStatus == _WiFi_WorkoutState_Workout)
                   return Console_Parameter(Info_WorkoutAccTime);
               else if( _NowWorkoutStatus == _WiFi_WorkoutState_Pause) 
                   return EEPROM_ReadInformation(EE_PauseTime); 
               break;
      case _WiFi_AccTime:
               // Workout total Time   
               return Console_Parameter(Info_AccumulatedTime);
      case _WiFi_WorkoutTime:
               // Program default workout Time,Elapsed Workout Time
               return Console_Parameter(Info_WorkoutAccTime);
      case _WiFi_WarmUpTime:
               // Warm Up Time,Elapsed Warm Up Time
               return Console_Parameter(Info_WarmUpAccTime);
      case _WiFi_CoolDownTime:
               // Cool Down Time,Elapsed Cool Down Time
               return Console_Parameter(Info_CoolDwonAccTime);
      case _WiFi_WorkoutFinish:
               // Workout Finish         
               return _WorkoutFinish;
      case _WiFi_AveragePace:         
               // unit: Seconds/Mile
               return Console_Get_AvgPace();
      case _WiFi_RPM:
               // RPM (TM: Motor, Bike:Generator,flywheel)         
               // unit: RPM
               return Digtial_Get_RollerRpm();
      case _WiFi_UserAge:  
               // 推算使用者年齡
               HT1381_RealTime(&today);//==>讀取HT1381的時間值 
               _Age = 2000 + today.Year - userFlashConfig[0]->birth_year;
               return _Age;
      case _WiFi_ProgramID: 
               switch(Console_ProgramMode(Data_Get,0))
               {
                   case TM_Rolling:
                             by_Bu = ROLLING_HILLS_SPEED_INCLINE;
                             break;
                   case TM_FatBurn:
                             by_Bu = FATBURN; 
                             break;
                   case TM_Gerkin:
                             by_Bu = GERKIN_FITNESS_TEST; 
                             break;
                   case TM_PEB:
                             by_Bu = PEB_FITNESS_TEST;
                             break;
                   case TM_Army:
                             by_Bu = ARMY_FITNESS_TEST;
                             break;
                   case TM_Navy:
                             by_Bu = NAVY_FITNESS_TEST;
                             break;
                   case TM_Airforce:
                             by_Bu = AIR_FORCE_FITNESS_TEST;
                             break;
                   case TM_Marine:
                             by_Bu = MARINES_FITNESS_TEST;
                             break; 
                   case TM_WFI:
                             by_Bu = WFI_FITNESS_TEST;
                             break;
                   case TM_TargetHR:
                             by_Bu = THR_ZONE;
                             break;
                   case TM_5K:
                             by_Bu = GOAL_5K;
                             break;
                   case TM_10K:
                             by_Bu = GOAL_10K;
                             break;                             
//                   case TM_Sprint8:
//                             by_Bu = SPRINT_8;
//                             break;
                   default:
                             by_Bu = MANUAL;
                             break;
               }
               return by_Bu;
      case _WiFi_ResistanceType:
               // LCB 控制驅動模式回復
               return AC_MOTOR;
      case _WiFi_WorkoutStageTime:
               // Workout Stage(interval) Time,The time of one stage
               return Console_GetTimeSegData();
      case _WiFi_TimeInStage:
               // Every workout stage(interval) elasped time 
               if( _NowWorkoutStatus == _WiFi_WorkoutState_WarmUp)
                   return Console_Parameter(Info_WarmUpAccTime);
               else if( _NowWorkoutStatus == _WiFi_WorkoutState_CoolDown)
                   return Console_Parameter(Info_CoolDwonAccTime);
               else if( _NowWorkoutStatus == _WiFi_WorkoutState_Workout)
                   return Console_Parameter(Info_WorkoutAccTime);
               else if( _NowWorkoutStatus == _WiFi_WorkoutState_Pause) 
                   return EEPROM_ReadInformation(EE_PauseTime); 
      case _WiFi_CurrentUser:         
               return 1;
      case _WiFi_NextIncline:  
               // unit: 0.1%
      case _WiFi_NextSpeed:  
               // unit: 0.1 mile/hr
      case _WiFi_NextResistance:  
               // unit: level
      case _WiFi_ResistanceLevel:
               // unit: Level          
      case _WiFi_WorkoutStage:
               // Workout Stage(interval),For current interval# or stage#  
               //Bike_Get_Time_Seg(); 
               break; 
      case _WiFi_Goal:
               // Time/Distance/Calorie/Steps/Custom goal 
               // unit: seconds/km/K.Calorie/../..
               {
                 char programID = Console_ProgramMode(Data_Get,0);
                 if(programID == TM_5K)
                   return 5;
                 else if(programID == TM_10K)
                   return 10;
                 else
                   return Console_UserTime(Data_Get,0);
               }
      case _WiFi_TargetWatts:
      case _WiFi_Floors: // 0.1 Floors
      case _WiFi_TotalSteps:
               break;
  }
  return 0;
}

    
// 2014.10.23
//==============================================================================
// 判斷持續3分鐘無連上時WiFi將作重置動作
//==============================================================================
void WiFi_Information_ConnectCheck(void)
{
  if(WIFISignalStrength() != 0)
      Timer_Counter_Clear(_Time_WiFiConnectCheck);
  else
  {
      if(Timer_Counter(T_LOOP,_Time_WiFiConnectCheck,1800))
      {// every 3 minute
          Timer_Counter_Clear(_Time_WiFiConnectCheck);
          // wifi module 重置需設Low至少300ms後再設Hi
          IO_WiFi_ResetLow();
          while(Timer_Counter(T_STEP,_Time_WiFiConnectCheck,5) == 0);
          IO_WiFi_ResetHi();// WiFi Reset Pin  
          //
          Timer_Counter_Clear(_Time_WiFiConnectCheck);
      }
  }
}
//==============================================================================
// 時區判斷
// Input : Time Zone Index 1 ~ _TimeZoneMax
// Output: IANA Time Zone Correspond Word
//==============================================================================
unsigned char WiFi_Information_TimeZoneCheck(unsigned char by_D)
{
  if(by_D >= 1 && by_D <= _TimeZoneMax)
      return by_D-1;
  else
      return _DefaultTimeZone;
}
//==============================================================================
// 時區時間載入RTC
// Input : Time Zone Index 1 ~ _TimeZoneMax
//==============================================================================
void WiFi_Information_TimeZoneUpdate(unsigned char by_D)
{
  // 時區設定
  WiFi_memset( machineConfig->timeZone, 0, _WIFI_Length_TimeZone );
  strcpy( machineConfig->timeZone,_TimeZone[WiFi_Information_TimeZoneCheck(by_D)].Name);
  //
  UpdateNetworkTime();
}
//==============================================================================
// Change DAPI Server IP
// _ON(1):Change _OFF(0):Not Change(Only set IP)
// 0:Dev 1:Production 2:QA 3:Staging
//==============================================================================
void WiFi_Information_ChangeDAPI_Server(char by_D)
{// 2015.02.02
  switch(EEPROM_ReadInformation(EE_DAPI_Server))
  {   
        case _Server_Dev:
          strncpy( machineConfig->MarioBox, (EEPROM_ReadInformation(EE_SSL) == _ON ? JOHNSON_WIFI_SERVERIP_Dev_SSL : JOHNSON_WIFI_SERVERIP_Dev) ,256);
             break;
        case _Server_Production: 
             strncpy( machineConfig->MarioBox, JOHNSON_WIFI_SERVERIP_PRODUCTION ,256);
             break;
        case _Server_QA: 
          strncpy( machineConfig->MarioBox, (EEPROM_ReadInformation(EE_SSL) == _ON ? JOHNSON_WIFI_SERVERIP_QA_SSL : JOHNSON_WIFI_SERVERIP_QA) ,256);
             break;
        case _Server_Staging: 
          strncpy( machineConfig->MarioBox, (EEPROM_ReadInformation(EE_SSL) == _ON ? JOHNSON_WIFI_SERVERIP_STAGING_SSL : JOHNSON_WIFI_SERVERIP_STAGING) ,256);
             break;
  }
  if(by_D == _ON)
  {
      WiFi_Information_Initial();
      WifiConnectionStatusChange(0);
      WifiConnectionStatusChange(1);
      wifi_ResetInit();
  }
  return;
}

// Serial Number
void WiFi_Information_SerialNumber(unsigned char mode,unsigned char *ModuleStr,unsigned char Ver,unsigned long SerialNumber)
{// mode ==> 0:console, 1:frame
  char Temp[_WIFI_Length_SerialNumber] = {0};
  
  if( Ver != 0 )
      sprintf((char*)&Temp[0],"%c%s%c%09d", (mode == 0)?'C':'F', ModuleStr,(Ver+'A'),SerialNumber) ;
  else  
      sprintf((char*)&Temp[0],"%c%s%09d", (mode == 0)?'C':'F', ModuleStr, SerialNumber);
//  __memset(&Temp[0]+strlen(Temp), '0', WIFI_SerialNumberSize - strlen(Temp)) ;
  WiFi_memcpy( (mode == 0)?machineConfig->serialNumber:machineConfig->frameSerialNumber, Temp, _WIFI_Length_SerialNumber );
}
//==============================================================================
// 依據機種別進行參數預設設定
//==============================================================================
void WiFi_Information_MachineTypeDefaultCheck(void)
{
  // 2014.11.24
  WiFi_memset( machineConfig->ecbCal, 0, 30 );
  // 2015.05.28
  WiFi_Information_SerialNumber(0,"TM706",EEPROM_ReadInformation(EE_ConsoleVersion),EEPROM_ReadInformation(EE_SerialNumber)) ;
  WiFi_Information_SerialNumber(1,"TM522",EEPROM_ReadInformation(EE_FrameVersion),EEPROM_ReadInformation(EE_FrameSerialNumber)) ;
  //
  machineConfig->machineType = TREADMILL;
  machineConfig->driveType = EEPROM_ReadInformation(EE_MachineType_LCB);
  machineConfig->consoleType = D_CONSOLE_TYPE;
  machineConfig->modelType = D_MACHINE_MODEL;// E for US only
}
  



