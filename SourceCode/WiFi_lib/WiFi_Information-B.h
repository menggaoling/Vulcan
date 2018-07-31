// 使用 __WiFi_Information 宣告方式可以避免内容重复定义
#ifndef __WiFi_Information
#define __WiFi_Information

#include "wifi_define.h"

#define _WiFi_Weight 1
#define _WiFi_Age    2
#define _WiFi_Unit   3
#define _WiFi_Gender 4
// 2014.11.24
#define _WorkoutMode_Warmup     0
#define _WorkoutMode_Workot     1
#define _WorkoutMode_CoolDown   2
#define _WorkoutMode_Pause      3
//
#define _DefaultTimeZone 21 //(GMT-06:00)Central Time(US and Canada)



extern LSFlashConfig* machineConfig;
extern LSFlashUserConfig *UserInfo[MAX_USERS]; // + 1 user 0 is Guest
extern LSFlashUserConfig guest;




void WiFi_Information_Initial(void);
void WiFi_Information_DeleteUser(char user);
void WiFi_Information_SaveMachineConfig(void);
void WiFi_Information_SaveUserConfig(void);

unsigned char WiFi_Information_GetResistanceType( void );
unsigned char WiFi_Information_GetCurrentUser(void);
unsigned char WiFi_Information_GetUserAge(unsigned char userIndex );
void WiFi_Information_SetIncline(unsigned short by_D);
void WiFi_Information_SetResistance(unsigned char by_D);
unsigned char WiFi_Information_GetProgramMode(void);
unsigned char WiFi_Information_GetWorkoutStatus(void);
void WiFi_Information_SetWorkoutStatus(unsigned char by_D);
unsigned short WiFi_Information_GetAccTime(void);
unsigned short WiFi_Information_GetWorkoutTime(void);
unsigned short WiFi_Information_GetSpeed(void);
unsigned char WiFi_Information_GetIncline(void);
unsigned short WiFi_Information_GetCalories(void);
unsigned int WiFi_Information_GetDistance(void);
unsigned char WiFi_Information_GetHeartRate(void);
unsigned char WiFi_Information_GetLevel(void);
unsigned char WiFi_Information_GetTHR(void);
unsigned short WiFi_Information_GetConstWatts(void);
unsigned short WiFi_Information_GetWatts(void);
unsigned short WiFi_Information_GetWarmUpTime(void);
unsigned short WiFi_Information_GetCoolDownTime(void);
unsigned char WiFi_Information_WorkoutFinish(void);
//void WiFi_Information_SetWeight(unsigned short by_D);
//unsigned short WiFi_Information_GetWeight(void);
unsigned short WiFi_Information_GetRPM(void);
unsigned short WiFi_Information_GetResistanceLV(void);
void WiFi_Information_SerialNumber(unsigned char mode,unsigned char *ModuleStr,unsigned long Ver,unsigned long SerialNumber);
void WiFi_Information_MachineTypeDefaultCheck(void);

void WiFi_Information_Set(unsigned char by_Item,unsigned short by_Dat);
unsigned short WiFi_Information_Get(unsigned char by_Item);
void WiFi_Information_ConnectCheck(void);
unsigned char WiFi_Information_TimeZoneCheck(unsigned char by_D);
void WiFi_Information_TimeZoneUpdate(unsigned char by_D);
void WiFi_Information_ChangeServerIP(char by_D);

#endif
