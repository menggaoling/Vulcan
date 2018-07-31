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

LSFlashConfig *machineConfig;// AM system Data
LSFlashUserConfig *UserInfo[MAX_USERS];//+1
LSFlashUserConfig guest;
//LSFlashUserConfig wifiUser1;
//LSFlashUserConfig wifiUser2;
//LSFlashUserConfig wifiUser3;
//LSFlashUserConfig wifiUser4;
// 2014.11.24
char _WorkoutFinish;
char _NowWorkoutStatus;
unsigned short _DFWorkoutTime;
unsigned short _WarmUpTime;
unsigned short _CoolDownTime;
//
const struct
{
   char* Name;        // 时区名称
}_TimeZone[78] = 
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

//==============================================================================
// to init machineConfig UserInfo 
//==============================================================================
void WiFi_Information_Initial(void)
{
  unsigned char by_Bu;
  
  machineConfig = &wifiMachineConfig;

  UserInfo[0] = &guest;    // 指定位置
  //UserInfo[1] = &wifiUser1;// 指定位置
  //UserInfo[2] = &wifiUser2;// 指定位置
  //UserInfo[3] = &wifiUser3;// 指定位置
  //UserInfo[4] = &wifiUser4;// 指定位置
   
  
  SHT1381 time;//==>HT1381时间IC Data指向
  ReadHT1381(&time);
  
  
  // xID
  for( int j = 0; j < XIDMAXLENGTH; j++ )
  {
      UserInfo[0]->xid[j] = '0';
  }
  // Passcode
  for( int j = 0; j < 4; j++ )
  {
      UserInfo[0]->passcode[j] = '0';
  }
  // DATE
  by_Bu = (time.Year / 16) * 10;
  UserInfo[0]->year = by_Bu + (time.Year % 16) ;
  UserInfo[0]->year = UserInfo[0]->year+ 2000 - 30;
  by_Bu = (time.Date / 16) * 10;
  by_Bu += (time.Date % 16);
  if( time.Month == 2 && by_Bu == 29 )
  {
      UserInfo[0]->date = 28;
  }
  else
  {
      UserInfo[0]->date = by_Bu;
  }
  UserInfo[0]->month = time.Month;
  //
  UserInfo[0]->gender = EEPROM_ReadInformation(EE_GENDER);
  UserInfo[0]->languageId = EEPROM_ReadInformation(EE_LANGUAGE);
  UserInfo[0]->isAcceptedTerms = 0;
  UserInfo[0]->isShareWorkouts = 0;
  UserInfo[0]->age = EEPROM_ReadInformation(EE_AGE);
  UserInfo[0]->isSetup = 0;
  
  WiFi_memset( machineConfig, 0 , sizeof( LSFlashConfig ));
  machineConfig->brandid = MAXTIX;
  WiFi_Information_MachineTypeDefaultCheck();
  machineConfig->csafeId = 0x02;
  machineConfig->csafeManufactureId = 0x15;
  
  machineConfig->wifiisAPMode = 1;
  
  machineConfig->maxMPHx10 = Param_MaxSpeed_MPH_Max;
  machineConfig->minMPHx10 = Param_StartSpeed_MPH_Min;
  machineConfig->maxKPHx10 = Param_MaxSpeed_KPH_Max;
  machineConfig->minKPHx10 = Param_StartSpeed_KPH_Min;
  machineConfig->maxInclinex10 = 150;
  machineConfig->minInclinex10 = 0;
  
  //machineConfig->maxMPH_RPM = D_MAX_RPM;
  //machineConfig->minMPH_RPM = D_MIN_RPM;
  //machineConfig->maxKPH_RPM = D_MAX_KM_RPM;
  //machineConfig->minKPH_RPM = D_MIN_KM_RPM;
  //machineConfig->maxInclineADC = D_ADC_MAX;
  //machineConfig->minInclineADC = D_ADC_MIN;
  AccumulatedData->accumulatedDistanceMilesX100 = EEPROM_ReadInformation(EE_ACCUMULATED_DISTANCE)/10;
  AccumulatedData->accumulatedTimeSeconds = EEPROM_ReadInformation(EE_ACCUMULATED_TIME);
  //machineConfig->demoMode = D_DEMO;
  machineConfig->isEnergySaverModeOn = 1;//D_EUP_ON;
  AccumulatedData->accumulatedLubeMilesX100 = 0;//D_LUBE_START;
  machineConfig->lubeFlag = 0;//D_LUBE_START;
  WorkoutConfig->maxWorkoutMinutes = EEPROM_ReadInformation(EE_MAX_TIME);
  WorkoutConfig->defaultWorkoutMinutes = EEPROM_ReadInformation(EE_USER_TIME);
  machineConfig->defaultAge = EEPROM_ReadInformation(EE_AGE);
  machineConfig->defaultWeightLBSX10 = EEPROM_ReadInformation(EE_WEIGHT)*10;
  WorkoutConfig->defaultWarmupMinutes = 2;
  WorkoutConfig->defaultCooldownMinutes = 2;
  WorkoutConfig->defaultPauseMinutes = EEPROM_ReadInformation(EE_PauseTime);
  machineConfig->defaultGender = EEPROM_ReadInformation(EE_GENDER);
  machineConfig->defaultVolume = EEPROM_ReadInformation(EE_OutputVolume);
  machineConfig->defaulitBrightness = 100;
  machineConfig->targetHR = 80;
  WorkoutConfig->defaultWattsTarget = 0;
  machineConfig->languageId = 0;
  machineConfig->accumulatedAgingHours = 0;
  machineConfig->displayMode = 0;
  machineConfig->isAutoLogin = 0;
  machineConfig->rtcSet = 0;
  machineConfig->aSysScale = 0;
  machineConfig->dSysScale = 0;
  machineConfig->magnetNum = 0;
  machineConfig->rfChannel = 0;
  machineConfig->rfAddress = 0X01;
  machineConfig->timeFormat = 0;
  machineConfig->lubeDistanceMilesStart = 0;
  machineConfig->unit = EEPROM_ReadInformation(EE_UNIT);

  if( machineConfig->unit == C_MILE )
  {
      UserInfo[0]->weightStandardX10 = Param_UserWeight_DF*10;
  }
  else
  {
      UserInfo[0]->weightStandardX10 = Param_UserWeight_DF_Kg*10;
  }
  WiFi_memset( machineConfig->ecbCal, 0, 30 );
  // 2015.02.02
  WiFi_Information_ChangeServerIP(_OFF);
  //strcpy( machineConfig->MarioBox, JOHNSON_WIFI_SERVERIP );
  //
  WiFi_memset( machineConfig->machineIdentification, 0, 36 );
  strcpy( machineConfig->machineIdentification, DEFAULT_MACHINEUUID );
  // 时区设定
  WiFi_memset( machineConfig->timezone, 0, 65 );
  //strcpy( machineConfig->timezone, "America/Chicago" );
  strcpy( machineConfig->timezone,_TimeZone[WiFi_Information_TimeZoneCheck(EEPROM_ReadInformation(EE_TZone))].Name);
  //
  Timer_Counter_Clear(_WiFiConnectCheckTime);
  // wifi module 重置需设Low至少300ms后再设Hi
  IO_WiFi_ResetLow();
  while(Timer_Counter(T_STEP,_WiFiConnectCheckTime,5) == 0);
  IO_WiFi_ResetHi();// WiFi Reset Pin  
  //
  Timer_Counter_Clear(_WiFiConnectCheckTime);
}

//==============================================================================
// to Delete User 
//==============================================================================
void WiFi_Information_DeleteUser(char user)
{
  SHT1381 now;//==>HT1381时间IC Data指向
  ReadHT1381(&now);//==>读取HT1381的时间值
  unsigned char by_Bu;
  int i = user;
  
  
  UserInfo[i]->userConsoleId = i;
  for( int j = 0; j < XIDMAXLENGTH; j++ )
    UserInfo[i]->xid[j] = '0';
  for( int j = 0; j < 4; j++ )
    UserInfo[i]->passcode[j] = '0';
  
  UserInfo[i]->isSetup = 0;
  UserInfo[i]->weightStandardX10 = machineConfig->defaultWeightLBSX10;
  UserInfo[i]->age = machineConfig->defaultAge;
  
  // DATE
  by_Bu = (now.Year / 16) * 10;
  UserInfo[i]->year = by_Bu + (now.Year % 16) ;
  UserInfo[i]->year = UserInfo[i]->year+ 2000 - UserInfo[i]->age;
  by_Bu = (now.Date / 16) * 10;
  by_Bu += (now.Date % 16);
  if( now.Month == 2 && by_Bu == 29 )
  {
      UserInfo[i]->date = 28;
  }
  else
  {
      UserInfo[i]->date = by_Bu;
  }
  UserInfo[i]->month = now.Month;
  //
  UserInfo[i]->gender = machineConfig->defaultGender;
  UserInfo[i]->languageId = 0;
  UserInfo[i]->lastUpdateYear = 2000;
  UserInfo[i]->lastUpdateMonth = 1;
  UserInfo[i]->lastUpdateDay = 1;
  UserInfo[i]->lastUpdateHour = 10;
  UserInfo[i]->lastUpdateMinute = 0;
  UserInfo[i]->lastUpdateSecond = 0;
  //UserInfo[i]->workoutId = 0;
  //			UserInfo[i]->autoLogin = 0;
  WiFi_memset( UserInfo[i]->name, 0, 65 );
  UserInfo[i]->isLoggedOn = 0;
  UserInfo[i]->isAcceptedTerms = 0;
  UserInfo[i]->isShareWorkouts = 0;
  UserInfo[i]->isLastUser = 0;
  UserInfo[i]->displayUnit = machineConfig->unit;
  
}
//==============================================================================
// LCB 控制驱动模式回复
//==============================================================================
unsigned char WiFi_Information_GetResistanceType( void )
{
  unsigned char type = DC_MOTOR;
  
  switch( machineConfig->machineType )
  {
  case TREADMILL:
          type = AC_MOTOR;
          break;
  case ELLIPTICAL:
          //type = ECB_RESISTANCE  ;
          //break;
  case BIKE:
          //type = ECB_RESISTANCE  ;
          //break;
  case ASCENT:
          //type = ECB_RESISTANCE ;
          //break;
  case STEPPER:
          //type = ECB_RESISTANCE  ;
          //break;
  default:
          type =  ECB_RESISTANCE;
          break;
  }
  return type;
}
//==============================================================================
// User Porgram mode
// 目前因无支持所以预设为 1
//==============================================================================
unsigned char WiFi_Information_GetCurrentUser(void)
{
  return 1;
}
//==============================================================================
// 推算使用者年龄
//==============================================================================
unsigned char WiFi_Information_GetUserAge(unsigned char userIndex )
{
  unsigned char _Age;
  unsigned char by_Bu;
  if( userIndex <= MAX_USERS )
  {
      SHT1381 today;//==>HT1381时间IC Data指向
      ReadHT1381(&today);//==>读取HT1381的时间值
      
      by_Bu = (today.Year / 16) * 10;
      by_Bu += today.Year % 16;
      _Age = by_Bu + 2000 - UserInfo[userIndex]->year;
      by_Bu = (today.Date / 16) * 10;
      by_Bu += today.Date % 16;
      if((UserInfo[userIndex]->month > today.Month)||
         (UserInfo[userIndex]->month == today.Month && UserInfo[userIndex]->date > by_Bu ))
      {
          _Age--;
      }
      //age = 35;//debug by ckm rtc error
      return _Age;
  }
  else 
      return 0;
}
//==============================================================================
// 使用WiFi进行设定动作
//==============================================================================
void WiFi_Information_SetIncline(unsigned short by_D)
{
  
}
//==============================================================================
// 使用WiFi进行设定动作
//==============================================================================
void WiFi_Information_SetResistance(unsigned char by_D)
{
  
}

unsigned char WiFi_Information_GetProgramMode(void)
{
  return Console_ProgramMode(Data_Get,0);
}


void WiFi_Information_SetWorkoutStatus(unsigned char by_D)
{
  Console_WorkoutStatus(Data_Set,by_D);
}


unsigned short WiFi_Information_GetSpeed(void)
{
  return Console_Information(Info_WorkoutSpeed,Data_Get,0)/10;//Console_Speed(Data_Get,0)
}

unsigned char WiFi_Information_GetIncline(void)
{
  return 0;
}

unsigned short WiFi_Information_GetCalories(void)
{
  return Console_Get_Calories();
}

unsigned int WiFi_Information_GetDistance(void)
{
  return Console_Get_Distance();
}

unsigned char WiFi_Information_GetHeartRate(void)
{
  return Heart_Get_Heart();
}

unsigned char WiFi_Information_GetLevel(void)
{
  return Console_Information(Info_LEVEL,Data_Get,0);//Console_Level(Data_Get,0);
}

unsigned char WiFi_Information_GetTHR(void)
{
  return Heart_Get_Target();
}

unsigned short WiFi_Information_GetConstWatts(void)
{
  return 0;//Bike_Get_ConstWatts();
}

unsigned short WiFi_Information_GetWatts(void)
{
  return ((unsigned short)((float)Console_Get_Calories_Hour() / 4.2));
}


unsigned short WiFi_Information_GetRPM(void)
{
  return Digtial_Get_RollerRpm();//Bike_Get_NowRPM();
}

unsigned short WiFi_Information_GetResistanceLV(void)
{
  return 1;//Bike_Get_ResistanceLV();
}


// 0 = Warmup, 1 = Workout, 2 = Cooldown, 3 = Pause
unsigned char WiFi_Information_GetWorkoutStatus(void)
{
  return _NowWorkoutStatus; 
}
// Workout total Time
unsigned short WiFi_Information_GetAccTime(void)
{
  return Console_Get_Time_Accumulated();
}
// Program default workout Time
unsigned short WiFi_Information_GetWorkoutTime(void)
{
  return _DFWorkoutTime;
}
// Warm Up Time
unsigned short WiFi_Information_GetWarmUpTime(void)
{
  return _WarmUpTime;
}
// Cool Down Time
unsigned short WiFi_Information_GetCoolDownTime(void)
{
  return _CoolDownTime;
}
// Workout Finish
unsigned char WiFi_Information_WorkoutFinish(void)
{
  return _WorkoutFinish;
}
// Serial Number
void WiFi_Information_SerialNumber(unsigned char mode,unsigned char *ModuleStr,unsigned long Ver,unsigned long SerialNumber)
{
  char Temp[15];
  
  if(Ver <= 1)// 不显示"A"
      sprintf((char*)&Temp[0],"%s%09d",ModuleStr,SerialNumber) ;
  else  
      sprintf((char*)&Temp[0],"%s%c%09d",ModuleStr,(Ver-1+'A'),SerialNumber) ;
  if(mode == 0)
      strcpy( machineConfig->consoleSerialNumber, Temp );
  else 
      strcpy( machineConfig->frameSerialNumber, Temp );
}
//==============================================================================
//to save machineConfig
//
//==============================================================================
void WiFi_Information_SaveMachineConfig(void)
{
  //EEPROM_SaveWorkInformation(EE_ACCUMULATED_TIME,Bike_Get_Time_Accumulated());
  //EEPROM_SaveWorkInformation(EE_ACCUMULATED_DISTANCE,Bike_Get_Distance());
}
//==============================================================================
//to save UserInfo[5] 0--->guest not save
//==============================================================================
void WiFi_Information_SaveUserConfig(void)
{
  // 用户信息加载仪表
  WiFi_Information_Set(_WiFi_Unit,UserInfo[0]->displayUnit);
  if(UserInfo[0]->displayUnit == C_KM)
  {// 因DAPI会自行转成英制回传,所以需要自行转回公制
      WiFi_Information_Set(_WiFi_Weight,((unsigned short)((float)(UserInfo[0]->weightStandardX10/2.2))/10));
  }
  else
      WiFi_Information_Set(_WiFi_Weight,UserInfo[0]->weightStandardX10/10);
  WiFi_Information_Set(_WiFi_Age,UserInfo[0]->age);
  WiFi_Information_Set(_WiFi_Gender,UserInfo[0]->gender);
  
  //
}
//==============================================================================
// WiFi参数设定至仪表
//==============================================================================
void WiFi_Information_Set(unsigned char by_Item,unsigned short by_Dat)
{
  switch(by_Item)
  {
      case _WiFi_Weight:
               Console_Information(Info_WEIGHT,Data_Set,by_Dat);
               break;
      case _WiFi_Age:
               Console_Information(Info_AGE,Data_Set,by_Dat);
               break;
      case _WiFi_Unit:
               Console_SpeedUnit(Data_Set,by_Dat); 
               break;   
      case _WiFi_Gender:         
               Console_Information(Info_GENDER,Data_Set,by_Dat);
               break;
  }
}
//==============================================================================
// WiFi 取仪表参数
//==============================================================================
unsigned short WiFi_Information_Get(unsigned char by_Item)
{
  switch(by_Item)
  {
      case _WiFi_Weight:
               return Console_Information(Info_WEIGHT,Data_Get,0); 
      case _WiFi_Age:
               return Console_Information(Info_AGE,Data_Get,0); 
      case _WiFi_Unit:
               return Console_SpeedUnit(Data_Get,0); 
      case _WiFi_Gender: 
               return Console_Information(Info_GENDER,Data_Get,0);
  }
  return 0;
}
// 2014.10.23
//==============================================================================
// 判断持续3分钟无连上时WiFi将作重置动作
//==============================================================================
void WiFi_Information_ConnectCheck(void)
{
  if(WIFISignalStrength() != 0)
      Timer_Counter_Clear(_WiFiConnectCheckTime);
  else
  {
      if(Timer_Counter(T_STEP,_WiFiConnectCheckTime,1800))
      {// every 3 minute
          Timer_Counter_Clear(_WiFiConnectCheckTime);
          // wifi module 重置需设Low至少300ms后再设Hi
          IO_WiFi_ResetLow();
          while(Timer_Counter(T_STEP,_WiFiConnectCheckTime,5) == 0);
          IO_WiFi_ResetHi();// WiFi Reset Pin  
          //
          Timer_Counter_Clear(_WiFiConnectCheckTime);
      }
  }
}
//==============================================================================
// 依据机种别进行参数预设设定
//==============================================================================
void WiFi_Information_MachineTypeDefaultCheck(void)
{
  // 2014.11.24
  WiFi_memset( machineConfig->ecbCal, 0, 30 );
  WiFi_Information_SerialNumber(0,"TM530",EEPROM_ReadInformation(EE_ConsoleVersion),EEPROM_ReadInformation(EE_SerialNumber)) ;
  WiFi_Information_SerialNumber(1,"TM501",EEPROM_ReadInformation(EE_FrameVersion),EEPROM_ReadInformation(EE_FrameSerialNumber)) ;
  machineConfig->machineType = TREADMILL;
  machineConfig->driveType = DELTA_LCB;
  machineConfig->machineModel = 22;//Csafe_Model;
}
//==============================================================================
// 时区判断
// Input : Time Zone Index 1 ~ 78
// Output: IANA Time Zone Correspond Word
//==============================================================================
unsigned char WiFi_Information_TimeZoneCheck(unsigned char by_D)
{
  if(by_D >= 1 && by_D <= 78)
      return by_D-1;
  else
      return _DefaultTimeZone;
}
//==============================================================================
// 时区时间加载RTC
// Input : Time Zone Index 1 ~ 78
//==============================================================================
void WiFi_Information_TimeZoneUpdate(unsigned char by_D)
{
  // 时区设定
  WiFi_memset( machineConfig->timezone, 0, 65 );
  strcpy( machineConfig->timezone,_TimeZone[WiFi_Information_TimeZoneCheck(by_D)].Name);
  //
  UpdateNetworkTime();
}
//==============================================================================
// Change DAPI Server IP
// _ON(1):Change _OFF(0):Not Change(Only set IP)
// 1:Dev 2:Production 3:QA 4:Staging
//==============================================================================
void WiFi_Information_ChangeServerIP(char by_D)
{// 2015.02.02
  switch(EEPROM_ReadInformation(EE_ServerIP))
  {
      case 1:
             strncpy( machineConfig->MarioBox, JOHNSON_WIFI_SERVERIP_Dev ,256);
             break;
      case 2:
             strncpy( machineConfig->MarioBox, JOHNSON_WIFI_SERVERIP_PRODUCTION ,256);
             break;
      case 3:
             strncpy( machineConfig->MarioBox, JOHNSON_WIFI_SERVERIP_QA ,256);
             break;
      case 4:
             strncpy( machineConfig->MarioBox, JOHNSON_WIFI_SERVERIP_STAGING ,256);
             break;
  }
  if(by_D == _ON)
  {
      WifiConnectionStatusChange(0);
      WifiConnectionStatusChange(1);
      wifi_ResetInit();
  }
  return;
}



