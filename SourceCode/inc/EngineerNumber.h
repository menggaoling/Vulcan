#ifndef _EngineerNumber_H_
#define _EngineerNumber_H_

#include "General.h"
#include "C_safe.h"
#include "EEPRom.h"

// 数字键是否使用
#define _NonUseNumKey 0
#define _UseNumKey    1
//---------------------------------------------------------------------
// 设定参数时，窗口显示的方式
#define _String_None          10
#define _String_Number        11
#define _String_YesNo         0
#define _String_OffOn         1
#define _String_MaleFemale    2
#define _String_DownUp        3
#define _String_MileKm        4
#define _String_DisableEnable 5
//---------------------------------------------------------------------
const struct
{
  char* Words[2];
}Words_Check[6] = 
{// 0       1
  {"YES" ,   "NO"},
  {"OFF" ,   "ON"},
  {"Male",   "Female"},
  {"Time Remaining",   "Time Elapsed"},
  {"Standard","Metric"},
  {"Disable","Enable"}
};
//---------------------------------------------------------------------
const struct
{
  char* ItemName;  // 选项名称
  unsigned char EEPRomNum;  // EEPRom 储存代号
  unsigned short Range[2];  // 数值范围
}VolumeItem[3] = 
{
  {"Input", EE_VOLUME,      {1,15}},
  {"Max",   EE_MaxVolume,   {1,32}},
  {"Output",EE_OutputVolume,{1,32}}
};
//---------------------------------------------------------------------
const struct
{
  char* ItemName;          // 选项名称
  unsigned char EEPRomNum; // EEPRom 储存代号
  unsigned int Range[2];   // 数值范围
}HJSItem[2] = 
{
  {"Notification"   ,EE_HJS_Switch     ,{0,1}},        
  {"Times to warning",EE_HJS_CheckNumber,{1000,1000000}}
};
//---------------------------------------------------------------------
const struct
{
  char* ItemName;          // 选项名称
  unsigned char EEPRomNum; // EEPRom 储存代号
  unsigned int Range[2];   // 数值范围
}KeypadItem[2] = 
{
  {"Stuck Check" ,EE_KeyStuck      ,{0,1}},        
  {"Notification",EE_KeyNotifiction,{0,1}}
};

//---------------------------------------------------------------------
const struct
{
    char* ItemName;
}AudioSource[4] = 
{
  "OFF",
  "TV",
  "PCTV",
  "REMOTE TV"
};
//---------------------------------------------------------------------
const struct
{
  char* ItemName;  // 选项名称
}SoftwareItem[5] = 
{
  "Bootloader",        
  "UCB",                
  "LCB",                    
  "WiFi",           
  "Language"
};
//---------------------------------------------------------------------
const struct
{
  char* ItemName;          // 选项名称
  unsigned char EEPRomNum; // EEPRom 储存代号
  unsigned int Range[2];   // 数值范围
}AutoUpdateItem[3] = 
{
  {"Check Update"   ,0,                  {0,1}},        
  {"Disable/Enable" ,EE_AutoUpdateSwitch,{0,1}},
  {"Time"           ,EE_AutoUpdateTime,  {0,23}}
};
//---------------------------------------------------------------------
const struct
{
  char* ItemName;  // 选项名称
}NAVYItem[2] = 
{
  "NAVY Test Site:0",//"TEST SITE ELEVATION GREATER THAN 5000 FT ABOVE SEA LEVEL",                
  "NAVY Test Site:1",//"TEST SITE ELEVATION LESS THAN 5000 FT ABOVE SEA LEVEL"                    
};
//---------------------------------------------------------------------
const struct
{
  char* ItemName;  // 选项名称
}ServerIP[4] = 
{
  "Dev",
  "Staging",                
  "QA",
  "Production"
};
//---------------------------------------------------------------------
const struct
{
  char* ItemName;  // 选项名称
}TreadSensor[4] = 
{
  "Time : OFF",                
  "Time : 30 ",
  "Time : 60 ",
  "Time : 90 "
};

//---------------------------------------------------------------------
const struct
{
   char* ItemName;  // 选项名称
}TestModeItem[6] = 
{
  "Display",        
  "Keypad",                
  "CSAFE",                    
  "WiFi",           
  "Headphone Jack",
  "Erp"
};

const struct
{    
    char* ItemName;          // 选项名称
//    unsigned char EEPRomNum; // EEPRom 储存代号
//    unsigned int Range[2];   // 数值范围
}RunningBeltItem[3] =
{
    "Current Wear",
    "Replace Item",
    "Previous Service"
};
//---------------------------------------------------------------------
// Manuage Mode各名称代码    
#define _Manage_Item                 12 // 大组数
enum{
// Workouts
  _Manage_Item_MaxTime,
  _Manage_Item_Time,
  _Manage_Item_TimerMode,
  _Manage_Item_Level,
  _Manage_Item_HRTSpeedBased,
  _Manage_Item_PauseTime,
  _Manage_Item_NAVY,
// User
  _Manage_Item_Age,
  _Manage_Item_Weight,
  _Manage_Item_Height,
  _Manage_Item_Gender,
// Data & Time
  _Manage_Item_Date,
  _Manage_Item_RTCTime,
  _Manage_Item_TimeZone,
// Speed
  _Manage_Item_Unit,
  _Manage_Item_MaxSpeed,
  _Manage_Item_StartSpeed,
// Elevation
  _Manage_Item_MaxElevation,
// Software
  _Manage_Item_SoftwareVersion,
  _Manage_Item_softwareUpdate,
  _Manage_Item_AutoUpdate,
// General
  _Manage_Item_TotalDistance,
  _Manage_Item_TotalTime,
// Language
  _Manage_Item_LanguageDefault,
  _Manage_Item_LanguageErase,
// Logo
//  _Manage_Item_LogoDefault,
//  _Manage_Item_LogoImport,
//  _Manage_Item_LogoErase,
// Machine
  _Manage_Item_Type,
  _Manage_Item_SerialNumber,
  _Manage_Item_OutOfOrder,
  _Manage_Item_TreadSensor,
//  _Manage_Item_Speaker,
  _Manage_Item_Beeper,
  _Manage_Item_HeadphoneJack,
  _Manage_Item_USBPort,
  _Manage_Item_Keypad,
// TV
  _Manage_Item_InputSource,
//  _Manage_Item_DefaultChannel,
  _Manage_Item_DefaultVolume,
  _Manage_Item_Power,
// Internet (WiFi)
  _Manage_Item_InternetSwitch,
  _Manage_Item_MAC_ID,
  _Manage_Item_IP,
// start v2.1-12
//  _Manage_Item_SiteSurvey,
  _Manage_Item_AP,
  _Manage_Item_Signal,
// end v2.1-12
  _Manage_Item_ExprotUSB,
  _Manage_Item_ImportUSB,
  _Manage_Item_Reset,
// ErP
  _Manage_Item_ErP, 
//END
  _Manage_Item_END,
//
  _Manage_Item_Max = _Manage_Item_END - 1
};

//---------------------------------------------------------------------
#define _Engineer_Item                _Manage_Item+5 // 大组数
// Calibration
#define _Eng_Item_AutoCalibration    _Manage_Item_Max+1
#define _Eng_Item_EvationMin         _Manage_Item_Max+2
#define _Eng_Item_EvationMax         _Manage_Item_Max+3
// Running Test
#define _Eng_Item_RunningTest        _Manage_Item_Max+4
// Error Code
#define _Eng_Item_ErrorCode          _Manage_Item_Max+5
// Demo Mode
//#define _Eng_Item_DemoMode           _Manage_Item_Max+6
// DAPI
#define _Eng_Item_DAPI               _Manage_Item_Max+6
#define _Eng_Item_SSL                _Manage_Item_Max+7 // v2.1-11
// Back UP
#define _Eng_Item_BackUp             _Manage_Item_Max+8

#define _Eng_Item_Max                _Manage_Item_Max+8
//---------------------------------------------------------------------
#define _Service_Item                _Engineer_Item+7 // 大组数
// Accumulate
#define _Service_Item_AccDistance    _Eng_Item_Max+1
#define _Service_Item_AccTime        _Eng_Item_Max+2
// Log
#define _Service_Item_Error          _Eng_Item_Max+3
#define _Service_Item_HJS            _Eng_Item_Max+4
// Configuration (EEPROM)
#define _Service_Item_ExportUSB      _Eng_Item_Max+5
#define _Service_Item_ImportUSB      _Eng_Item_Max+6
// Factory Default
#define _Service_Item_Reset          _Eng_Item_Max+7
// Asset Management
#define _Service_Item_AMswitch       _Eng_Item_Max+8
#define _Service_Item_ClubID         _Eng_Item_Max+9
// xID Login
#define _Service_Item_xIDswitch      _Eng_Item_Max+10
//Replacement Parts
#define _Service_Item_RunningBelt    _Eng_Item_Max+11
#define _Service_Item_RunningDeck    _Eng_Item_Max+12
#define _Service_Item_MCB            _Eng_Item_Max+13
#define _Service_Item_Motor          _Eng_Item_Max+14
#define _Service_Item_HeadphoneJack  _Eng_Item_Max+15
#define _Service_Item_Maintenance    _Eng_Item_Max+16

#define _Service_Item_Max            _Eng_Item_Max+16
//---------------------------------------------------------------------
#define _Test_Item                   6

#define _Test_Display                1
#define _Test_Keypad                 2
#define _Test_CSAFE                  3
#define _Test_WiFi                   4
#define _Test_HJS                    5
#define _Test_ERP                    6

//---------------------------------------------------------------------
const struct
{
    unsigned char  ItemTarget;  // 对应项目起始位置
    unsigned char  ItemRange;   // 对应项目数 (0~n)
             char* ItemName;    // 工程模式选项名称
}ManageItemName[_Service_Item]=
{
  {_Manage_Item_MaxTime,        6,"Workouts"},
  {_Manage_Item_Age,            3,"User"},
  {_Manage_Item_Date,           2,"Date/Time"},
  {_Manage_Item_Unit,           2,"Speed"},
  {_Manage_Item_MaxElevation,   0,"Elevation"},
  {_Manage_Item_SoftwareVersion,2,"Software"},
  {_Manage_Item_TotalDistance,  1,"General"},
  {_Manage_Item_LanguageDefault,1,"Language"},
  //{_Manage_Item_LogoDefault,    2,"Logo"},
  {_Manage_Item_Type,           7,"Machine"},
  {_Manage_Item_InputSource,    2,"TV"},
  {_Manage_Item_InternetSwitch, 7,"Internet"},
  {_Manage_Item_ErP,            0,"ErP"},
  {_Eng_Item_AutoCalibration,   2,"Calibration"},
  {_Eng_Item_RunningTest,       0,"Running Test"},
  {_Eng_Item_ErrorCode,         0,"Error Code"},
  //{_Eng_Item_DemoMode,          0,"Demo Mode"},
  {_Eng_Item_DAPI,              1,"DAPI"},// v2.1-11
  {_Eng_Item_BackUp,            1,"Back Up"},
  {_Service_Item_AccDistance,   1,"Accumulate"},
  {_Service_Item_Error,         1,"Log"},
  {_Service_Item_ExportUSB,     1,"Configuration"},
  {_Service_Item_Reset,         0,"Factory Default"},
  {_Service_Item_AMswitch,      1,"Asset Management"},
  {_Service_Item_xIDswitch,     0,"xID Login"},
  {_Service_Item_RunningBelt,   5,"Replacement Parts"}
};
//---------------------------------------------------------------------
const struct
{
    char* ItemName;        // 工程模式参数选项名称
    struct
    {
        unsigned char Used : 1;     // 0:不使用数字键, 1:使用
        unsigned char IsTime : 1;   // 0:非时间时间, 1:时间格式
        unsigned char Digits : 6;   // 数字位数
    }NumericKey;    
    unsigned char AdjustSize;       // 数值调整的倍数
    unsigned char NumberDisplayWay; // 数字显示的方式
    unsigned char EEPRomNum;        // EEPRom 储存代号
    unsigned short Range[2];        // 数值范围
}EngineerMode[_Service_Item_Max+1] = 
{
  // Manager Mode
  // Workouts 
  {"Max Time",        {_UseNumKey,   1,2},1, _String_Number,    EE_MAX_TIME,   {Param_Time_Min, Param_Time_Max}},
  {"Time",            {_UseNumKey,   1,2},1, _String_Number,    EE_USER_TIME,  {Param_Time_Min, Param_Time_Max}},
  {"Timer Mode",      {_NonUseNumKey,0,0},1, _String_DownUp,    EE_TimerMode,  {Time_DOWN, Time_UP}},
  {"Level",           {_UseNumKey,   0,2},1, _String_Number,    EE_LEVEL,      {Param_ProgramLv_Min, Param_ProgramLv_Max}},
  {"HRT Speed Based", {_NonUseNumKey,0,0},1, _String_OffOn,     EE_HRTSpeedBased,{0, 1}},
  {"Pause Time",      {_NonUseNumKey,1,3},1, _String_Number,    EE_PauseTime,  {Param_PauseTime_Min, Param_PauseTime_Max}},
  {"NAVY Test Site",  {_NonUseNumKey,0,0},1, _String_None,      EE_NAVY_Test,  {0,1}},
  // User
  {"Age",             {_UseNumKey,   0,3},1, _String_Number,    EE_AGE,        {Param_UserAge_Min, Param_UserAge_Max}},
  {"Weight",          {_UseNumKey,   0,3},1, _String_Number,    EE_WEIGHT,     {Param_UserWeight_Min, Param_UserWeight_Max}},  
  {"Height",          {_UseNumKey,   0,3},1, _String_Number,    EE_Height,     {_Height_Mile_L, _Height_Mile_H}},  
  {"Gender",          {_NonUseNumKey,0,0},1, _String_MaleFemale,EE_GENDER,     {D_Male, D_Female}}, 
  // Date & Time
  {"Date",            {_NonUseNumKey,0,0},0, _String_None,      0,             {0, 0}}, 
  {"Time",            {_NonUseNumKey,0,0},0, _String_None,      0,             {0, 0}}, 
  {"Time Zone",       {_UseNumKey   ,0,2},1, _String_Number,    EE_TZone   ,   {1, 78}},
  // Speed
  {"Unit",            {_NonUseNumKey,0,0},1, _String_MileKm,    EE_UNIT,       {0, 1}},
  {"Max",             {_UseNumKey,   0,3},1, _String_Number,    EE_MaxSpeed,   {Param_MaxSpeed_MPH_Min,Param_MaxSpeed_MPH_Max}},
  {"Start",           {_UseNumKey,   0,2},1, _String_Number,    EE_StartSpeed, {Param_StartSpeed_MPH_Min,Param_StartSpeed_MPH_Max}},
  // Elevation
  {"Max",             {_UseNumKey,   0,3},5, _String_Number,    EE_MaxDF_Elevation, {0, 150}},
  // Software
  {"Version",         {_NonUseNumKey,0,0},1, _String_None,      0,             {0, 4}},
  {"Update",          {_NonUseNumKey,0,0},1, _String_None,      0,             {0, 1}}, 
  {"Auto Update",     {_NonUseNumKey,0,2},1, _String_None,      0,             {0, 2}},
  // General
  {"Total Distance",  {_NonUseNumKey,0,4},0, _String_None,      EE_ACCUMULATED_DISTANCE,{0, 0}},
  {"Total Time",      {_NonUseNumKey,0,3},0, _String_None,      EE_ACCUMULATED_TIME,    {0, 0}},
  // Language
  {"Default Language",{_NonUseNumKey,0,0},1, _String_None,      EE_LANGUAGE,   {0, 0}}, 
  {"Erase EEPROM",    {_NonUseNumKey,0,0},0, _String_None,      0,             {0, 0}},
  // Logo
  //{"Default Logo",    {_NonUseNumKey,0,0},1, _String_None,      EE_Logo,       {0, 0}}, 
  //{"Import Logo",     {_NonUseNumKey,0,0},0, _String_None,      0,             {0, 0}},
  //{"Erase EEPROM",    {_NonUseNumKey,0,0},0, _String_None,      0,             {0, 0}},
  // Machine
  {"Type",            {_NonUseNumKey,0,0},1, _String_None,      EE_MachineType,{0, 2}}, 
  {"Serial Number",   {_NonUseNumKey,0,0},0, _String_None,      0,             {0, 0}}, 
  {"Out Of Order",    {_NonUseNumKey,0,0},1, _String_OffOn,     EE_OutOfOrder, {0, 1}},
  {"Tread Sensor",    {_NonUseNumKey,0,0},30,_String_None,      EE_InUser,     {0, 90}},
  //{"Speaker",         {_NonUseNumKey,0,0},1, _String_OffOn,     EE_Speaker,    {0, 1}},
  {"Beeper",          {_NonUseNumKey,0,0},1, _String_OffOn,     EE_BEEP,       {0, 1}},
  {"Headphone Jack",  {_NonUseNumKey,0,7},1, _String_None,      0,             {0, 1}},
  {"USB Port",        {_NonUseNumKey,0,0},1, _String_None,      0,             {0, 1}},
  {"Keypad",          {_NonUseNumKey,0,0},1, _String_None,      0,             {0, 1}},
  // TV
  {"Input Source",    {_NonUseNumKey,0,0},1, _String_None,      EE_Audio,      {0, 3}},  
//  {"Channel",         {_UseNumKey,   0,2},1, _String_Number,    EE_CHANNEL,    {1, 99}}, 
  {"Volume",          {_NonUseNumKey,0,2},1, _String_None,      0,             {0, 2}}, 
  {"Power",           {_NonUseNumKey,0,0},1, _String_OffOn,     EE_TVPower,    {0, 1}},
  // Internet
  {"Disable/Enable",  {_NonUseNumKey,0,0},1, _String_DisableEnable,EE_Wireless,{0, 1}},
  {"MAC ID",          {_NonUseNumKey,0,0},0, _String_None,      0,             {0, 0}},
  {"IP",              {_NonUseNumKey,0,0},0, _String_None,      0,             {0, 0}},
  {"AP mode",         {_NonUseNumKey,0,0},1, _String_OffOn,     EE_AP_Mode,    {0, 1}},// v2.1-12
  {"Signal Strength", {_NonUseNumKey,0,0},1, _String_None,       0,            {0, 0}},// v2.2-4
  {"Export setting to USB",  {_NonUseNumKey,0,0},0, _String_None,0,            {0, 0}},
  {"Import setting from USB",{_NonUseNumKey,0,0},0, _String_None,0,            {0, 0}},
  {"Reset",           {_NonUseNumKey,0,0},0, _String_None,      0,             {0, 0}},
  // Erp   
  {"ErP Time",        {_UseNumKey,   1,2},1, _String_Number,    EE_SleepTime,  {Param_ErPTime_Min, Param_ErPTime_Max}},
  
  // Engineer Mode
  // Calibration
  {"Auto Calibration",{_NonUseNumKey,0,0},1, _String_None,      0,             {0, 0}},
  {"Elevation Min",   {_NonUseNumKey,0,0},1, _String_None,      EE_LOW_ELEVATION,{0, 0}},
  {"Elvation Max",    {_NonUseNumKey,0,0},1, _String_None,      EE_MAX_ELEVATION,{0, 0}},
  // Running Test
  {"Running Test",    {_NonUseNumKey,0,0},1, _String_None,      0,             {0, 0}},
  // Error Code
  {"Error Code",      {_NonUseNumKey,0,0},1,_String_DisableEnable,EE_ErrorCode,{0, 1}},
  // DAPI
  {"Server",          {_NonUseNumKey,0,0},1, _String_None,      EE_DAPI_Server,   {0, 3}},
  {"SSL",             {_NonUseNumKey,0,0},1, _String_OffOn,     EE_SSL,        {0, 1}},// v2.1-11
  {"Back Up",         {_NonUseNumKey,0,0},0, _String_None,       0,            {0, 1}},
  // Service Mode 
  // Accumulate
  {"Distance",        {_NonUseNumKey,0,0},0,_String_None,       0,              {0, 1}},
  {"Time",            {_NonUseNumKey,0,0},1,_String_None,       0,              {0, 1}},
  // Log
  {"Error",           {_NonUseNumKey,0,0},1,_String_None,       0,              {0, 1}},
  {"Headphone Jack Sensor",{_NonUseNumKey,0,0},1,_String_None,  EE_HJS_total,   {0, 1}},
  // Configuration
  {"Export to USB",   {_NonUseNumKey,0,0},1,_String_None,       0,              {0, 0}},
  {"Import from USB",   {_NonUseNumKey,0,0},1,_String_None,       0,              {0, 0}},
  // Factory Default
  {"Reset",           {_NonUseNumKey,0,0},1,_String_None,       0,              {0, 0}},
  // Asset Management
  {"Disable/Enable",  {_NonUseNumKey,0,0},1,_String_DisableEnable,EE_AM,        {0, 1}},
  {"Club ID",         {_NonUseNumKey,0,5},1,_String_Number,       EE_CLUB_ID,   {1, 0xffff}},
  // xID Login
  {"Disable/Enable",  {_NonUseNumKey,0,0},1,_String_DisableEnable,EE_xID,       {0, 1}},
  //Replacement Parts
  {"Running Belt",    {_NonUseNumKey,0,0},1,_String_None,         0,       {0, 2}},
  {"Running Deck",    {_NonUseNumKey,0,0},1,_String_None,         0,       {0, 2}},
  {"MCB",             {_NonUseNumKey,0,0},1,_String_None,         0,       {0, 2}},
  {"Motor",           {_NonUseNumKey,0,0},1,_String_None,         0,       {0, 2}},
  {"Headphone Jack",  {_NonUseNumKey,0,0},1,_String_None,         0,       {0, 2}},
  {"Prev.Maintenance",{_NonUseNumKey,0,0},1,_String_None,         0,       {0, 2}}
};

//---------------------------------------------------------------------
const struct
{
   char* ItemName;  // ??????3?
}KeypadTestItem[40] = 
{
  "BACK",
  "ENTER",
  "MANUAL",
  "TRAINING",
  "HEART RATE",
  "FAT BURN",
  "ROLLING",
  "FITNESS",
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "SELECT1",
  "SELECT2",
  "ToggleDisplay 3",
  "ToggleDisplay 4",
  "PAUSE",
  "TIME UP",
  "TIME DOWN",
  "SPEED UP",
  "SPEED DOWN",
  "INCLINEU",
  "INCLINED",
  "GO",
  "STOP",
  "TV POWER",
  "VOL UP",
  "VOL DOWN",
  "TV CH UP",
  "TV CH DOWN",
  "TV LAST",
  "TV MUTE",
  "TV CC",
  "COOL DOWN"
  
};






#endif
