/*
 * wifi_define.c
 *
 *  Created on: 2014/06/13
 *      Author: Peter.fu
 */


#ifndef __WIFI_DEFINE
#define __WIFI_DEFINE

//for wifi feature
#include "wifi_port.h"
#include "General.h"


#define JOHNSON_WIFI_SERVERIP_Dev           "dev.dls.jfit.co"
#define JOHNSON_WIFI_SERVERIP_QA            "qa.dls.jfit.co"
#define JOHNSON_WIFI_SERVERIP_STAGING       "staging.dls.jfit.co"
#define JOHNSON_WIFI_SERVERIP_PRODUCTION    "dapi-ls.jfit.co"

#define JOHNSON_WIFI_SERVERIP_Dev_SSL        "dev-dls.jfit.co"
#define JOHNSON_WIFI_SERVERIP_QA_SSL         "qa-dls.jfit.co"
#define JOHNSON_WIFI_SERVERIP_STAGING_SSL    "staging-dls.jfit.co"

//#define JOHNSON_WIFI_SERVERIP               JOHNSON_WIFI_SERVERIP_STAGING
//#define JOHNSON_WIFI_SERVERIP "qa.dms.jfit.co"
#define DEFAULT_UUID "00000000-0000-0000-0000-000000000000"
#define DEFAULT_OSVERSION ""//"000.000.000         " // "OS : V1.000         "   20 letters
//#define DEFAULT_MACHINEUUID  "f81d4fae-7dec-11d0-a765-00a0c91e6bf6"
#define DEFAULT_DESCRIPTION "No description"

// WiFi Default AP
#define Default_SSID "rd02"
#define Default_Key  "QvnDjg8PZodeN34LZSpl"
#define Default_Type  WIFI_SECURITY_WPA2_PHRASE

#define DEFAULTXID "00000000000000"
#define XIDMAXLENGTH 14
#define _XidPassWordLength 4

//
#define FLASH_SEGMENT_SIZE 1024
#define NUM_FLASH_SEGMENTS 2

// String length
#define _WIFI_Length_SerialNumber    36
#define _WIFI_Length_SoftwareVer     20
#define _WIFI_Length_MachineID       36
#define _WIFI_Length_MacAddress      6
#define _WIFI_Length_IPAddress       6
#define _WIFI_Length_UserExternalID  36
#define _WIFI_Length_ProgramUUID     36
#define _WIFI_Length_TimeZone        65

//for data type
typedef signed   int  INT;
typedef unsigned   int  UINT;

typedef unsigned char     UCHAR;
typedef unsigned char       BYTE;

typedef unsigned short      WORD;
typedef unsigned long       DWORD;


typedef char     SCHAR;
typedef void              VOID;
typedef void *            PVOID;

typedef signed char       INT8;
typedef signed char *     PINT8;
typedef unsigned char     UINT8;
typedef unsigned char *   PUINT8;


typedef signed   int*  PINT;

typedef unsigned   int*  PUINT;

typedef signed char *     PCHAR;

typedef unsigned char *   PUCHAR;
typedef signed char *            PSTR;
typedef const signed char *      PCSTR;

typedef short             INT16;
typedef short *           PINT16;
typedef unsigned short    UINT16;
typedef unsigned short *  PUINT16;
typedef signed long       INT32;
typedef signed long *     PINT32;
typedef unsigned long     UINT32;
typedef unsigned long *   PUINT32;
typedef char   CHAR;
#define       true                (bool)1
#define       false               (bool)0

//for data struct
typedef struct
{
	UCHAR Brand;
	UCHAR MachineType;
	UCHAR ModeType;
	UCHAR DriveType;
	UCHAR CSafeManu;
	UCHAR CSafeID;
	UCHAR CSafeModel;
    UCHAR SerialNumber[_WIFI_Length_SerialNumber];//15 // 2015.05.28
    UCHAR FrameNumber[_WIFI_Length_SerialNumber];//15 // 2015.05.28
}MachineInfor;
// 机种别宣告
typedef enum
{
	TREADMILL = 0,
	ELLIPTICAL = 1,
	BIKE =2,
	ASCENT = 3,
	STEPPER = 4,
	CLIMB_MILL = 5,
	ROWER = 6,
	MACHINETYPEEND
} MACHINE_TYPE;
// 品牌宣告
typedef enum
{
	MAXTIX = 0,
	VISION = 1,
	HORIZON = 2,
	SPRINT8 = 3,
        JOHNSON = 4,
	BRANDIDEND
}BRANDID;
// 下控制器各型号宣告
typedef enum
{
	AC_LCB_JIS = 1,
	LCB1_PWM_JHT = 2,
	LCB3_PWM_JHT = 3,
	DC_LCB_JIS = 4,
	LCB_CD_JHT = 5,
	TM_LCB_PANATTA = 6,
	LCB1_CURRENT_JHT = 7,
	LCB3_CURRENT_JHT = 8,
	LCB1_PANATTA = 9,
	LCB2_JHT = 10,//0x0A,
	CLIMBMILL_JHT = 11,//0x0B,
	EP_JIS = CLIMBMILL_JHT ,
	EP_LCB_INCLINE_JIS = EP_JIS, 
	BIKE_INDUCTIONBRAKE_JIS = 12,//0x0C,
	EP_INDUCTIONBRAKE_JIS = BIKE_INDUCTIONBRAKE_JIS,
	DC_LCB_1_JIS = 13,//0x0D,/*2~~~2.75 HP*/
	DC_LCB_2_JIS = 14,//0x0E,/*3~~~3.25 HP*/
	AC_LCB_1_JIS = 15,//0x0F,/*2~~~2.75 HP*/
	AC_LCB_2_JIS = 16,//0x10,/*3~~~3.25 HP*/
	A2D_BOARD_JIS = 17,//0x11,
	ECB_BOARD_JIS = 18,//0x12,
	ECB_BOARD_OUT_JIS = ECB_BOARD_JIS,
	LCBA          = 19,//0x13,
        _1xBikeLCB_2014 = 20,//0x14,
	EP_INDUCTIONBRAKE_INCLINE_JIS = 21,//0x15,
	DCI_DAUGHTER_BOARD = 192,//0xC0,
	TOPTECK_LCB = 193,//0xC1,
	DELTA_LCB  = 194,//0xC2,
	DELTA_AC = 195,//0xC3,
}DRIVE_TYPE;

// 阻力控制型态宣告
typedef enum
{
	DC_MOTOR,
	AC_MOTOR,
	ECB_RESISTANCE,
	PWM_RESISTANCE,
	GENERATOR,
} RESISTANCE_TYPE;

// All Program 代码宣告
typedef enum
{
  MANUAL = 0,
  WEIGHT_LOSS = 1,
  WEIGHT_LOSS_PLUS = 2,
  SPEED_INTERVALS = 3,
  ENDURANCE_INTERVALS = 4,
  PEAK_INTERVALS = 5,
  HR_INTERVALS = 6,
  FOOT_HILLS = 7,
  ROLLING_HILLS_SPEED  = 8,
  ROLLING_HILLS_SPEED_INCLINE = 9,
  MOUNTAIN_CLIMB = 10,//0x0A,
  GOAL_5K = 11,//0x0B,
    GOAL_10K = 12,//0x0c
    GOLF_COURSES = 13,//0x0D,
    DISTANCE_GOAL = 14,//0x0E,
    CALORIES_GOAL = 15,//0x0F,
    STEP_GOAL = 16,//0x10,
    TARGET_HEART_RATE = 17,//0x11,
    SPRINT_8 = 18,//0x12,
    CUSTOM = 19,//0x13,
    CONSTANT_WATTS = 20,//0x14,
    INTERVALS_WATTS = 21,//0x15,
    RUN_WALK = 22,//0x16,
    HILL_TRAINING = 23,//0x17,
    GAME = 24,//0x18,
    CANDENCE = 25,//0x19,
    RANDOM = 26,//0x1A,
    STRENGTH_BUILDER = 27,//0x1B,
    //FIT_TEST = 28,//0x1C,
    //PERCENT_THR = 29,//0x1D,
    GERKIN_FITNESS_TEST = 30,//0x1E,
    //HRC = 31,//0x1F,
    THR_ZONE = 32,//0x20,
  ARMY_FITNESS_TEST = 33,
  NAVY_FITNESS_TEST = 34,
  AIR_FORCE_FITNESS_TEST = 35,
  MARINES_FITNESS_TEST = 36,
  PEB_FITNESS_TEST = 37,
  COOPER_FITNESS_TEST = 38,
    THR_SPEED = 39,
    INTERVALS = 40,
    GOAL_20K = 41,
    GOAL_30K = 42,
    THR_RESISTANCE_BASED = 43,
    PERCENT_THR_SPEED = 44,
    PERCENT_THR_INCLINE = 45,
    PERCENT_THR_RESISTANCE = 46,
    VIRTUAL_ACTIVE = 47,
    COACH_JENNY = 48,
    FATBURN = 49,
    GLUTE_TRAINING = 50,
    CPAT_FITNESS_TEST = 51,
    WFI_FITNESS_TEST = 52,
    SUB_MAX_FITNESS_TEST = 53,
    PERSONAL_TRAINER_WORKOUT = 54,
    LANDMARK = 55,
    FITNESS_TEST = 56,
    INTERVAL_SPEED = 57,
	PROGRAMIDEND
}PROGRAMID;

typedef struct
{
	UINT8 level;
	UINT16 cal;
} PWM_CAL_POINT;

typedef struct
{
    UINT32 ID;
    UINT16 Year;
    UINT8 Month;
    UINT8 Day;
} WorkoutResponse;

// 综合信息设定
typedef struct
{
	UINT16 brandid;
        UINT8 csafeId;
	UINT8 csafeManufactureId;
	UINT8 csafeModelId;
        UINT8 Console;
	UINT16 clubId;
	UINT16 machineType;
	UINT16 modelType;
        UINT16 consoleType;
        UINT16 driveType;
	UINT16 maxMPHx10;
	UINT16 minMPHx10;
	UINT16 maxKPHx10;
	UINT16 minKPHx10;
	UINT16 maxInclinex10;
	UINT16 minInclinex10;
	UINT16 maxMPH_RPM;
	UINT16 minMPH_RPM;
	UINT16 maxKPH_RPM;
	UINT16 minKPH_RPM;
        UINT16 maxLevel;
        UINT16 minLevel;
	//UINT16 maxInclineADC;
	//UINT16 minInclineADC;
	UINT32 accumulatedDistanceMilesX100;
	UINT32 accumulatedSeconds;
        UINT32 headphoneInsertion;
	UINT32 headphoneRemoval;
        UINT8 maxUser;
	UINT8 demoMode;
        UINT8 energySaver;
	UINT16 maxWorkoutTimeMinutes;
	UINT16 defaultWorkoutTimeMinutes;
	UINT16 defaultAge;
	UINT16 defaultWeightX10;
	UINT16 defaultWarmupTimeMinutes;
	UINT16 defaultCooldownTimeMinutes;
	UINT16 pauseTimeMinutes;
	UINT8 gender;
	UINT8 volume;
	UINT8 brightness;
	UINT16 targetHR;
	UINT16 constantWatts;
	UINT8 languageId;
	UINT8 units;
	UINT8 timeFormat;
	UINT16 ecbCal[30];
    char serialNumber[_WIFI_Length_SerialNumber];//15 // 2015.05.28
    char frameSerialNumber[_WIFI_Length_SerialNumber];//15 // 2015.05.28
    char machineIdentification[_WIFI_Length_MachineID];//UINT8 // 2014.06.17
    char programUUID[_WIFI_Length_ProgramUUID];
    char softwareVersion[_WIFI_Length_SoftwareVer];
    char osVersion[_WIFI_Length_SoftwareVer];
    char timeZone[_WIFI_Length_TimeZone];
    char MarioBox[256];
    char Autologin;
    UINT8   wifiisAPMode;
    char isAMsystem;
    UINT16 LcbVersion;
    UINT16 MotorTemperature;  // For Future
    UINT16 BusCurrent;        // For Future
    UINT16 BusVoltage;        // For Future
    UINT32 LubeBeltMilesX10;
    UINT16 ErrorCode;
    UINT16 ServiceCode;
    
} LSFlashConfig;

typedef struct
{
  UINT8 userConsoleId;
  UINT8 UserSetup;
  char xid[XIDMAXLENGTH];
  char passcode[_XidPassWordLength];
  UINT16 weightX10;
  UINT8 age;
  UINT16 birth_year;
  UINT8 birth_month;
  UINT8 birth_day;
  UINT8 gender;
  UINT8 language;
  UINT16 lastUpdateYear;
  UINT8 lastUpdateMonth;
  UINT8 lastUpdateDay;
  UINT8 lastUpdateHour;
  UINT8 lastUpdateMinute;
  UINT8 lastUpdateSecond;
  UINT8 DisplayUnits;
  char name[65];
  UINT8 AcceptedTerms;
  UINT8 WorkoutDataShare;
  UINT8 LoggedOn;//bool
  UINT8 isLastUser;//bool
} LSFlashUserConfig;

typedef struct
{
	char xid[XIDMAXLENGTH];
    char passcode[_XidPassWordLength];
} LSFlashWorkoutId;

typedef struct
{
	UINT8 incline;
	UINT8 speedOrResistance;
	UINT16 segmentTime;
	UINT8 segmentType;
} CustomWorkoutSegment;

typedef struct
{
	UINT8 userId;
	UINT8 numSegments;
} CustomWorkoutHeader;

#define MAX_USERS 1//4
#define MAX_SEGMENTS 15
#define MAX_WORKOUTS 5

extern LSFlashConfig *machineConfig;
extern LSFlashUserConfig *userFlashConfig[MAX_USERS]; // + 1 user 0 is Guest
//extern LSFlashWorkoutId* userWorkoutId;//peterfu delete

//extern  UINT8* numUserWorkoutId;
//__no_init char WiFi_flashBuffer[FLASH_SEGMENT_SIZE*NUM_FLASH_SEGMENTS];


#endif /*__WIFI_DEFINE*/



 
