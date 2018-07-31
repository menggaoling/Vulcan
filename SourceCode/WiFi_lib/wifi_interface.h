/*
 * wifi_interface.h
 *
 *  Created on: May 2, 2013
 *      Author: Brenden.Capps
 */

#ifndef WIFI_INTERFACE_H_
#define WIFI_INTERFACE_H_

#include "wifi_api.h"
//RFID
#include  "RFID_GATRM310.h"
//typedef unsigned short int UINT16;
//typedef unsigned char UINT8;
//typedef unsigned long int UINT32;

#define WIFI_DAPI_VERSION 10 //20150803
#define WIFI_PROTOCOL_VERSION 5//E------>5
#define WIFI_LINGO_MODULE_SETTINGS_VERSION 1
#define WIFI_LINGO_GENERAL_VERSION 1
#define WIFI_LINGO_REMOTE_VERSION 1
#define WIFI_LINGO_SETTINGS_VERSION 1
#define WIFI_LINGO_TEST_VERSION 1

#define WIFI_LINGO_WORKOUT_TRACKING_VERSION 1
#define WIFI_LINGO_ASSET_MANAGEMENT_VERSION 1
#define WIFI_LINGO_NETWORK_TIME_VERSION 1
#define WIFI_LINGO_USER_MANAGEMENT_VERSION 1

#define RFTagLength 16

#ifndef WIFI_SUCCESS
#define WIFI_SUCCESS 1
#endif
#ifndef WIFI_FAIL
#define WIFI_FAIL    2
#endif

enum WIFI_COMMAND_STATES
{
	WCS_IDLE,  // 0
	WCS_INIT_WIFI,
	WCS_GET_WIFI_INFO,
	WCS_GET_WIFI_SETTINGS,
	WCS_CONNECT_TO_AP,
	WCS_GET_LINGO_INFO,
	WCS_GET_CONNECTION_INFO,
	WCS_SET_DATA_SERVER,
	WCS_GET_DATA_SERVER,
	WCS_SET_AM_SERVER,
	WCS_GET_AM_SERVER,
	WCS_GET_MACHINE_INFO,  // 11
	WCS_GET_WORKOUT_DATA,
	WCS_ACK,
	WCS_GET_STATUS,
	WCS_GET_SPEED_CALIBRATION,
	WCS_GET_PWM_CALIBRATION,
	WCS_GET_ECB_CALIBRATION,
	WCS_GET_WATTS_TABLE,
	WCS_GET_MACHINE_STATE,
	WCS_GET_USER_DATA,
	WCS_GET_CURRENT_USER,  // 21
	WCS_GET_DEFAULTS,
	WCS_GET_MACHINE_TYPE,
	WCS_GET_REGION,
	WCS_CHECK_FOR_UPDATE,  // RSCU
	WCS_POST_WORKOUT_DATA,
	WCS_UPDATE_USER,
	WCS_COMPLETE_WORKOUT,
	WCS_UPDATE_TIME,
	WCS_GET_REMOTE_DATA,
	WCS_UPDATE_USER_INFO,   // 31
	WCS_UPDATE_MACHINE_INFO,
	WCS_SELECT_PROGRAM,
	WCS_KEY_PRESS,
	WCS_REGISTER_USER,
	WCS_CHECK_XID,
	WCS_LOGIN_USER,
	WCS_ENABLE_AP,
	WCS_DISABLE_AP,
	WCS_CONNECT_TO_ROUTER,
	WCS_DISCONNECT_TO_ROUTER,  // 41
	WCS_SYNC_CONSOLE_INFO,
	WCS_REGISTER_MACHINE,
	WCS_GET_DAPI_LOCATION,
	WCS_SET_MARIO_BOX,
	WCS_UNLINK_USER,
	WCS_GET_SIGNAL_STRENGTH,
	WCS_RESTORE_FACTORY,
	WCS_SET_AP_FOR_ENGTEST,
	WCS_GET_LATEST_VERSION, //for RSCU 
	WCS_SET_UPDATE_URL,//for RSCU 
	WCS_WORKOUT_SNAPSHOT_EX,
	WCS_WORKOUT_COMPLETE_EX,
	WCS_RFID_LOGIN,
	WCS_RFID_ASSOCIATE,
	WCS_RFID_DISASSOCIATE,
	WCS_UPDATE_MACHINE_STATS,
	WCS_OCCURRED_ERROR_CODE,
	WCS_OCCURRED_SERVICE_CODE,
        WCS_SEND_HTTP, // 0502
        WCS_AM_ENABLE,
    WCS_DOWNLOADSOFTWARE,
};

typedef enum
{
	WIFI_LINGO_MODULE_SETTINGS = 0x01,
	WIFI_LINGO_GENERAL,//0x02
	WIFI_LINGO_REMOTE,//0x03
	WIFI_LINGO_SETTINGS,//0x04
	WIFI_LINGO_TEST,//0x05
        
	WIFI_LINGO_WORKOUT_TRACKING = 0x80,
	WIFI_LINGO_ASSET_MANAGEMENT,//0x81
	WIFI_LINGO_NETWORK_TIME,//0x82
	WIFI_LINGO_USER_MANAGEMENT,//0x83
} WIFI_COMMAND_LINGO;

typedef enum
{
	WIFI_MODULE_SETTINGS_ACKNOWLEDGMENT,
	WIFI_GET_WIFI_SETTINGS,
	WIFI_CONNECT_TO_AP,
} WIFI_LINGO_MODULE_SETTINGS_COMMANDS;//0x01

typedef enum
{
	WIFI_GENERAL_ACKNOWLEDGMENT,
	WIFI_GET_LINGO_VERSION,
	WIFI_GET_MACHINE_INFO,
	WIFI_SET_SPEED_CALIBRATION,
	WIFI_GET_SPEED_CALIBRATION,
	WIFI_SET_PWM_CAL,
	WIFI_GET_PWM_CAL,
	WIFI_SET_ECB_CAL,
	WIFI_GET_ECB_CAL,
	WIFI_SET_WATTS_TABLE,
	WIFI_GET_WATTS_TABLE,
	WIFI_SET_DATA_SERVER,
	WIFI_GET_DATA_SERVER,
	WIFI_SET_MACHINE_STATE,
	WIFI_GET_MACHINE_STATE,
	WIFI_SET_USER_DATA,
	WIFI_GET_USER_DATA,
	WIFI_SET_WORKOUT_DATA_RATE,
	WIFI_GET_WORKOUT_DATA,
	WIFI_GET_CURRENT_USER,
	WIFI_STOP,
	WIFI_CHECK_FOR_UPDATE,

} WIFI_LINGO_GENERAL_COMMANDS;//0x02

typedef enum
{
	WIFI_REMOTE_ACKNOWLEDGMENT,
	WIFI_SET_CURRENT_USER,
	WIFI_START,
	WIFI_PAUSE,
	WIFI_RESTART,
	WIFI_CHANGE_PROGRAM,
	WIFI_SET_SPEED,
	WIFI_SET_INCLINE,
	WIFI_SET_RESISTANCE,
	WIFI_SET_RAW_ECB,
	WIFI_SET_RAW_PWM,
	WIFI_SELECT_PROGRAM,
	WIFI_KEY_PRESS,
} WIFI_LINGO_REMOTE_COMMANDS;//0x03

typedef enum
{
	WIFI_SETTINGS_ACKNOWLEDGMENT,
	WIFI_SET_DEFAULTS,
	WIFI_GET_DEFAULTS,
	WIFI_SET_MACHINE_TYPE,
	WIFI_GET_MACHINE_TYPE,
	WIFI_SET_REGION,
	WIFI_GET_REGION,
} WIFI_LINGO_SETTINGS_COMMANDS;//0x04

typedef enum
{
	WIFI_TEST_ACKNOWLEDGMENT,
	WIFI_SET_INCLINE_ADC,
	WIFI_SET_RPM,
	WIFI_SET_ECB,
	WIFI_SET_PWM,
	WIFI_BEEP,
	WIFI_SIM_SAFETY_KEY,
} WIFI_LINGO_TEST_COMMANDS;//0x05

typedef enum
{
	WIFI_ACK_OK,
	WIFI_ACK_CHECKSUM_ERROR,
	WIFI_ACK_UNABLE_TO_CONNECT,
	WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT,
	WIFI_ACK_ANOTHER_DEVICE_HAS_CONTROL,
	WIFI_ACK_MACHINE_STATE_CHANGE_NOT_CONFIRMED,
	WIFI_ACK_INVALID_USER_ID,
	WIFI_ACK_NOT_IN_REMOTE_CONTROL,
	WIFI_ACK_INVALID_SPEED,
	WIFI_ACK_INVALID_INCLINE,
	WIFI_ACK_INVALID_RESISTANCE,
	WIFI_ACK_START_INVALID_WORKOUT_IN_PROGRESS,
	WIFI_ACK_STOP_INVALID_NO_WORKOUT_IN_PROGRESS,
	WIFI_ACK_PAUSE_INVALID_NO_WORKOUT_IN_PROGRESS,
	WIFI_ACK_CHANGE_WORKOUT_INVALID_NO_WORKOUT_IN_PROGRESS,
	WIFI_ACK_DEFAULT_OUT_OF_RANGE,
	WIFI_ACK_INVALID_TIME_SET,
	WIFI_ACK_NOT_IN_TEST_MODE,
	WIFI_ACK_ADC_OUT_OF_RANGE,
	WIFI_ACK_RPM_OUT_OF_RANGE,
	WIFI_ACK_ECB_OUT_OF_RANGE,
	WIFI_ACK_PWM_OUT_OF_RANGE,
	WIFI_ACK_NOT_IN_SETTINGS_MODE,
	WIFI_ACK_MACHINE_STATE_OUT_OF_RANGE,
	WIFI_ACK_INVALID_LINGO,
	WIFI_ACK_UNKNOWN_COMMAND,
	WIFI_ACK_NO_PWM_RESISTANCE,
	WIFI_ACK_NO_ECB_RESISTANCE,
	WIFI_ACK_NO_INCLINE,
	WIFI_ACK_NO_WATTS_TABLE,
	WIFI_ACK_MACHINE_TYPE_OUT_OF_RANGE,
	WIFI_ACK_MODEL_TYPE_OUT_OF_RANGE,
	WIFI_ACK_START_INVALID_BELT_MOVING,
	WIFI_ACK_RESTART_INVALID_WORKOUT_NOT_IN_PROGRESS,
	WIFI_ACK_RESTART_INVALID_WORKOUT_NOT_PAUSED,
	WIFI_ACK_WORKOUT_NOT_IN_PROGRESS,
	WIFI_ACK_UNSUPPORTED_COMMAND,
	WIFI_ACK_SPEED_CALIBRATION_OUT_OF_RANGE,
	WIFI_ACK_INVALID_USER_DATA,
	WIFI_ACK_START_PARAMS_OUT_OF_RANGE,
	WIFI_ACK_CHANGE_PROGRAM_PARAMS_OUT_OF_RANGE,
	WIFI_ACK_INVALID_DATE_TIME,
	WIFI_ACK_INVALID_DATA_SIZE,
	WIFI_ACK_SET_MACHINE_STATE_REJECTED,
} WIFI_ACK_RESPONSE;

typedef enum
{
	WIFI_WORKOUT_TRACKING_ACKNOWLEDGMENT,//0
	WIFI_WORKOUT_TRACKING_START_WORKOUT,//1
	WIFI_WORKOUT_TRACKING_WORKOUT_DATA,//2
	WIFI_WORKOUT_TRACKING_WORKOUT_COMPLETE,//3

	WIFI_WORKOUT_TRACKING_DATA_EX = 0x06,
	WIFI_WORKOUT_TRACKING_COMPLETE_EX= 0x07,
	WIFI_WORKOUT_TRACKING_SPRINT8_COMPLETE_EX = 0x08,
	WIFI_WORKOUT_TRACKING_FITNESS_COMPLETE_EX = 0x09,
} WIFI_LINGO_WORKOUT_TRACKING_MESSAGES;//0x80

typedef enum
{
	WIFI_ASSET_MANAGEMENT_ACKNOWLEDGMENT,//0
	WIFI_ASSET_MANAGEMENT_MACHINE_INFO,//1
	WIFI_ASSET_MANAGEMENT_MACHINE_REGISTRATION,//2
	WIFI_ASSET_MANAGEMENT_GET_DAPI_LOCATION,
        
	WIFI_ASSET_MANAGEMENT_GET_LATEST_VERSION = 0x05,
	WIFI_ASSET_MANAGEMENT_MACHINE_INFO_EX = 0x07,
	WIFI_ASSET_MANAGEMENT_UPDATE_MACHINE_STATE = 0x08,
	WIFI_ASSET_MANAGEMENT_ERROR_CODE_OCCURRED = 0x09,
	WIFI_ASSET_MANAGEMENT_SERVICE_CODE_OCCURRED = 0x0a,
	WIFI_ASSET_MANAGEMENT_GET_CONSOLE_CONFIG_URL = 0x0B,
	WIFI_ASSET_MANAGEMENT_GET_MESSAGE_CENTER_URL = 0x0c,
	WIFI_ASSET_MANAGEMENT_ASSET_MANAGEMENT_ENABLED = 0x0f,
} WIFI_LINGO_ASSET_MANAGEMENT_MESSAGES;//0x81

typedef enum
{
	WIFI_NETWORK_TIME_ACKNOWLEDGMENT,//0
	WIFI_NETWORK_TIME_GET_TIME,//1
} WIFI_LINGO_NETWORK_TIME_MESSAGES;//0x82

typedef enum
{
	WIFI_USER_MANAGEMENT_ACKNOWLEDGMENT,//0
	WIFI_SYNC_USER_INFO,//1
	WIFI_USER_LOGIN,//2
	WIFI_USER_REGISTRATION,//3
	WIFI_USER_CHECK_XID,//4
	WIFI_USER_CLEAR_USERS,//5
	WIFI_USER_RESET_PASSWORD,//6
	WIFI_USER_SET_KEY_STORE,//7
	WIFI_USER_GET_KEY_STORE,//8
	WIFI_USER_GET_CALORIE_INFO,//9 
	WIFI_USER_UNLINK_USER,//0x0a
	WIFI_USER_GET_SOCAIL_MEDIA_CONNECTED=0x0e,
	WIFI_USER_SEND_SOCAIL_MEDIA = 0x0F,
	WIFI_USER_CHECK_UNIQUE_EMAIL=0x10,
	WIFI_USER_HAS_PERSONAL_TRAINER = 0x11,
	WIFI_USER_RFID_LOGIN = 0x12,
	WIFI_USER_RFID_ASSOCIATE = 0x13,
	WIFI_USER_RFID_DISASSOCIATE =0x14,
	WIFI_USER_GET_NUMBER_LEADER_BOARDS = 0x15,
} WIFI_LINGO_USER_MANAGEMENT_MESSAGES;//0x83

#pragma pack( push, 1 )

typedef struct
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
}DateAndTime;

typedef struct
{
	UINT8 messageType;
	UINT8 error;
} WIFIDataAck;  // 0x--, 0x00

// 0x01  WIFI_LINGO_MODULE_SETTINGS_COMMANDS

typedef struct
{
	char ssid[32];
	char securityKey[256];
} WIFIDataAPConnection;  // 0x01, 0x01 response (0x02 request)

// 0x02  WIFI_LINGO_GENERAL_COMMANDS

typedef struct
{
	UINT8 lingo;
	UINT8 version;
} WIFIDataLingoVersion;  // 0x02, 0x01 response (request: lingo)

typedef struct
{
	UINT8 brand;
	UINT8 machineType;
	UINT8 modelType;
	UINT8 driveType;
	UINT8 csafeManufacture;
	UINT8 csafeId;
	UINT8 csafeModelId;
	char serialNumber[_WIFI_Length_SerialNumber];
	char frameSerialNumber[_WIFI_Length_SerialNumber];
	char machineIdentification[_WIFI_Length_MachineID];
	UINT16 clubId;
	UINT16 minMPHX10;
	UINT16 maxMPHX10;
	UINT16 minKPHX10;
	UINT16 maxKPHX10;
	UINT16 minInclineX10;
	UINT16 maxInclineX10;
	UINT16 minLevel;
	UINT16 maxLevel;
	UINT32 accumulatedDistanceMilesX100;
	UINT32 accumulatedSeconds;
	char softwareVersion[_WIFI_Length_SoftwareVer];  // 20 ?
	UINT16 DapiVersion;
	UINT16 wifiProtocolVersion;
	UINT16 wifiFirmwareVersion;
	UINT16 wifiUARTVersion;
	UINT16 btProtocolVersion;
	UINT16 btUARTVersion;
	UINT8 lcbVendor;
	UINT16 lcbVersion;
	UINT16 ioVersion;
	char osVersion[_WIFI_Length_SoftwareVer];  // 20 ?
	UINT8 numUsers;
	char macAddress[_WIFI_Length_MacAddress];
	char ipAddress[_WIFI_Length_IPAddress];
	UINT16 errorCode;
	UINT16 serviceCode;
	UINT16 motorTemperature;
	UINT16 busCurrent;
	UINT16 busVoltage;
	UINT32 lubeMiles;
	UINT8  AutoLogin;
} WIFIDataMachineInfo;  // 0x02, 0x02 response

typedef struct
{
	UINT16 maxMPHx10;
	UINT16 minMPHx10;
	UINT16 maxKPHx10;
	UINT16 minKPHx10;
	UINT16 maxInclinex10;
	UINT16 minInclinex10;
	UINT16 maxADC;
	UINT16 minADC;
	UINT16 rpmMPHHigh;
	UINT16 rpmMPHLow;
	UINT16 rpmKPHHigh;
	UINT16 rpmKPHLow;
} WIFIDataSpeedCalibration;  // 0x02, 0x03 request (0x04 response)

typedef struct
{
	PWM_CAL_POINT calPoint[5];
} WIFIDataPWMCal;  // 0x02, 0x05 request (0x06 response)

typedef struct
{
	UINT16 ecbLevel[30];
} WIFIDataECBCal;  // 0x02, 0x07 request (0x08 response)

typedef struct
{
	UINT8 level;
	UINT16 wattsX10[26];
} WIFIDataWattsTable;  // 0x02, 0x09 request (0z0a response)

typedef struct
{
	char server[256];
} WIFIDataDataServer;  // 0x02, 0x0b request (0x0c response)

typedef struct
{
	UINT8 state;
        UINT8 priority;
} WIFIMachineStateRequest;  // 0x02, 0x0d request (0x0e response: state)

typedef struct
{
	UINT8 userConsoleId;
	unsigned char xid[XIDMAXLENGTH];
	unsigned char passcode[_XidPassWordLength];
	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 langauge;
        UINT8 unit;
	char name[65];
        UINT8 accepted;
        UINT8 share;
} WIFIDataUserData;  // 0x02, 0x0f request (0x10 response)

typedef struct
{
        char userExternalId[_WIFI_Length_UserExternalID];
	UINT8 programType;
	UINT8 workoutState;
	UINT8 stage;
	UINT16 stageTimeSeconds;
	UINT16 workoutTimeSeconds;
	UINT16 warmupTimeSeconds;
	UINT16 cooldownTimeSeconds;
	UINT16 timeInStateSeconds;
	UINT16 timeInStageSeconds;
	UINT16 mphx10;
	UINT16 inclinex10;
	UINT16 averagePace;
	UINT16 totalCalories;
	UINT16 caloriesPerHour;
	UINT16 metsX10;
	UINT16 watts;
	UINT16 milesx100;
	UINT16 heartRate;
	UINT16 averageHeartRate;
	UINT16 rpm;
	UINT16 programLevel;
	UINT16 resistanceLevel;
	UINT16 thr;
	UINT16 targetWattsX10;
	UINT16 nextIncline;
	UINT16 nextSpeed;
	UINT8 nextResistance;
} WIFIWorkoutDataRemote;  // 0x02, 0x12 response

// 0x03  WIFI_LINGO_REMOTE_COMMANDS

typedef struct
{
	UINT16 programType;
	UINT16 workoutTime;
	UINT16 warmupTime;
	UINT16 cooldownTime;
	UINT8 units;
	UINT16 startSpeedX10;
	UINT16 startInclineX10;
	UINT8 startLevel;
} WIFIDataProgramStart;  // 0x03, 0x02 

typedef struct
{
	UINT16 programType;
	UINT16 workoutTime;
	UINT16 warmupTime;
	UINT16 cooldownTime;
} WIFIDataProgramChange;  // 0x03, 0x04

typedef struct
{
        UINT8 metric;
	UINT16 speedX10;
} WIFISetSpeed;  // 0x03, 0x05 request

// 0x04  WIFI_LINGO_SETTINGS_COMMANDS

typedef struct
{
	UINT16 maxWorkoutTime;
	UINT16 defaultWorkoutTime;
	UINT16 defaultAge;
	UINT16 defaultWeightX10;
	UINT16 defaultWarmupTime;
	UINT16 defaultCooldownTime;
	UINT16 pauseTime;
	UINT8 gender;
	UINT8 volume;
	UINT8 brightness;
	UINT16 targetHR;
	UINT16 constantWatts;
	UINT8 energySaver;
	UINT8 demoMode;
	UINT8 languageId;
	UINT8 units;
	UINT16 maxMPHX10;
	UINT16 minMPHX10;
	UINT16 maxKPHX10;
	UINT16 minKPHX10;
	UINT16 maxInclineX10;
	UINT16 minInclineX10;
	UINT8 timeFormat;
        UINT32 LubebeltMilex10;
        UINT8 autologin;
} WIFIDataDefaults;  // 0x04, 0x01 requeat (0x02 response)

typedef struct
{
	UINT8 machineType;
	UINT8 modelType;
	UINT8 CSAFEManufacture;
	UINT8 CSAFEId;
	UINT8 CSAFEModelId;
	char ConsoleSerialNumber[_WIFI_Length_SerialNumber];
	char FrameSerialNumber[_WIFI_Length_SerialNumber];
	char machineIdentification[_WIFI_Length_MachineID];
} WIFIMachineTypeData;  // 0x04, 0x03 request (0x04 response)

typedef struct
{
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
	UINT8 units;
	UINT8 timeFormat;
	char timeZone[_WIFI_Length_TimeZone];
} WIFIDataRegion;  // 0x04, 0x05 request (0x06 response)

// 0x80  WIFI_LINGO_WORKOUT_TRACKING_MESSAGES

typedef struct
{
    char macAddress[_WIFI_Length_MacAddress];
    char machineIdentification[_WIFI_Length_MachineID];
    char userExternalId[_WIFI_Length_UserExternalID];
    UINT16 programType;
    char programUUID[_WIFI_Length_ProgramUUID];
    UINT16 goalType;
    UINT32 goalValueX100;
    UINT32 goalRemainingX100;
    UINT8   stageNumber;
    UINT16 stageGoalValueX100;
    UINT16 stageGoalRemainingX100;
    UINT8   workoutState;
    UINT16  timeInStateSeconds;
    UINT16  timeInStage;
    UINT16  lapIndex;
    UINT16  splitIndex;
    UINT16  mphx10;
    UINT16  inclinex10;
    UINT16  averagePace;
    UINT16  totalCalories;
    UINT16  caloriesPerHour;
    UINT16  metsX10;
    UINT16  watts;
    UINT16  milesx100;
    UINT16  heartRate;
    UINT16  averageHeartRate;
    UINT16  rpm;
    UINT16  programLevel;
    UINT16  resistanceLevel;
    UINT16  thr;
    UINT16  targetWattsX10;
    UINT32  floorsX10;
    UINT32  totalSteps;
    UINT32  elevationX100;
    UINT16  VO2X10;
    DateAndTime   StartTime;
} WIFIWorkoutDataEx;  // 0x80, 0x06 request

typedef struct{
    char macAddress[_WIFI_Length_MacAddress];
    char machineIdentification[_WIFI_Length_MachineID];
    char userExternalId[_WIFI_Length_UserExternalID];
    UINT16 programType;
    char programUUID[_WIFI_Length_ProgramUUID];
    char description[256];
    UINT16  workoutTimeSeconds;
    UINT16  warmupTimeSeconds;
    UINT16  cooldownTimeSeconds;
    UINT16  totalCalories;
    UINT16  milesx100;
    UINT16  averageHeartRate;
    UINT32  totalFloorsX10;
    UINT32  totalSteps;
    UINT32 totalElevationX100;
    DateAndTime StartTime;
}WIFIWorkoutDataCompleteEx;  // 0x80, 0x07 request

typedef struct{
    char macAddress[_WIFI_Length_MacAddress];
    char machineIdentification[_WIFI_Length_MachineID];
    char userExternalId[_WIFI_Length_UserExternalID];
    UINT16 programType;
    char programUUID[_WIFI_Length_ProgramUUID];
    char description[256];
    UINT16 sprint1SweatScoreX10;    
    UINT16 sprint2SweatScoreX10;
    UINT16 sprint3SweatScoreX10;    
    UINT16 sprint4SweatScoreX10;
    UINT16 sprint5SweatScoreX10;    
    UINT16 sprint6SweatScoreX10;
    UINT16 sprint7SweatScoreX10;    
    UINT16 sprint8SweatScoreX10;
    UINT16 totalSweatScoreX10;
    DateAndTime StartTime;
}WIFIWorkoutDataSprint8CompleteEx;  // 0x80, 0x08 request

typedef struct{
    char macAddress[_WIFI_Length_MacAddress];
    char machineIdentification[_WIFI_Length_MachineID];
    char userExternalId[_WIFI_Length_UserExternalID];
    UINT16 programType;
    char programUUID[_WIFI_Length_ProgramUUID];
    char description[256];
    UINT16 fitnessTestScore;
    DateAndTime StartTime;
}WIFIWorkoutDataFitnessTestCompleteEx;  // 0x80, 0x09 request

// 0x81  WIFI_LINGO_ASSET_MANAGEMENT_MESSAGES

typedef struct
{
	UINT8 macAddress[_WIFI_Length_MacAddress];
} WIFIMachineRegisterRequest;  // 0x81, 0x02 request

typedef struct
{
	char machineIdentification[_WIFI_Length_MachineID];
} WIFIMachineRegisterResponse;  // 0x81, 0x02 response

typedef struct
{
	char machineIdentification[_WIFI_Length_MachineID];
	UINT8 macAddress[_WIFI_Length_MacAddress];
	UINT16 DapiVersion;
}WIFIDAPILocation;  // 0x81, 0x03 request

typedef struct{
	UINT8 Secure;
	char Server[256];
	char Url[256];
	UINT16 Port;
}WIFIDAPIResponse;  // 0x81, 0x03 response

typedef struct{
    char  MachineUUid[36];
    UCHAR  MacAddress[_WIFI_Length_MacAddress];
    UINT16 BrandID;
    UINT16 MachineType;
    UINT16 ModelType;
    UINT16 ConsoleType;
    UINT8  Deployment;
    char  SoftwareType[256];
    char  CurrentVersion[256];
}WIFIDATAGetLatestVersion;  // 0x81, 0x05 request

typedef struct{
    char  SoftwareType[256];
    char  CurrentVersion[256];
    char  SoftwareUrl[256];
    char  MD5[32];
}WIFIDATAResponseLatestVersion;  // 0x81, 0x05 response

typedef struct
{
    char machineIdentification[_WIFI_Length_MachineID];
    UINT8 macAddress[_WIFI_Length_MacAddress];
    UINT16 brand;
    UINT16 machineType;
    UINT16 modelType;
    UINT16 driveType;
    UINT16 consoleType;
    UINT8   csafeManufacture;
    UINT8   csafeId;
    UINT8   csafeModelId;
    char serialNumber[_WIFI_Length_SerialNumber];
    char frameSerialNumber[_WIFI_Length_SerialNumber];
    UINT16  clubId;
    UINT16  minMPHX10;
    UINT16 maxMPHX10;
    UINT16 minKPHX10;
    UINT16 maxKPHX10;
    UINT16 minInclineX10;
    UINT16 maxInclineX10;
    UINT16 minLevel;
    UINT16 maxLevel;
    char softwareVersion[_WIFI_Length_SoftwareVer];
    UINT16 DapiVersion;
    UINT8  lcbVendor;
    UINT16 lcbVersion;
    char osVersion[_WIFI_Length_SoftwareVer];
    UINT8  numUsers;
    char ipAddress[_WIFI_Length_IPAddress];
    char timeZone[_WIFI_Length_TimeZone];
    UINT8  AutoLogin;
    DateAndTime lastChangeTime;
    char lastChangetimeZone[_WIFI_Length_TimeZone];
}WIFIDataRequestMachineInfo_EX;  // 0x81, 0x07 request

typedef struct{
    char machineIdentification[_WIFI_Length_MachineID];
    UINT16 brandID;
    UINT16 machineType;
    UINT16 modelType;
    UINT16 driveType;
    UINT16 consoleType;
    UINT8   csafeManufacture;
    UINT8   csafeID;
    UINT8   csafeModelID;
    char serialNumber[_WIFI_Length_SerialNumber];
    char frameSerialNumber[_WIFI_Length_SerialNumber];
    UINT16 clubID;
    UINT16 minMPHX10;
    UINT16 maxMPHX10;
    UINT16 minKPHX10;
    UINT16 maxKPHX10;
    UINT16 minInclineX10;
    UINT16 maxInclineX10;
    UINT16 minResistanceLevel;
    UINT16 maxResistanceLevel;
    UINT8  maxUsers;
    char timeZone[_WIFI_Length_TimeZone];
    UINT8  AutoLogin;
    UINT32 accumulatedDistanceMilesX100;
    UINT32 accumulatedTime;//seconds
    UINT32 lubeBeltMilesX10;
    UINT32 headphoneInsertions;
    UINT32 headphoneRemovals;
}WIFIMachineInfoEXResponse;  // 0x81, 0x07 response

typedef struct{
    char machineIdentification[_WIFI_Length_MachineID];
    char MacAddress[_WIFI_Length_MacAddress];
    UINT32  accumulatedDistanceMilesX100;
    UINT32  accumulatedTime;
    UINT16  motorTemperature;
    UINT16  busCurrent;
    UINT16  busVoltage;
    UINT32  lubeBeltMilesX10;
    UINT32  headphoneInsertions;
    UINT32  headphoneRemovals;
}WIFIUpdateMachineStats;  // 0x81, 0x08 request

typedef struct{
    char machineIdentification[_WIFI_Length_MachineID];
    char MacAddress[_WIFI_Length_MacAddress];
    UINT16  errorCode;
    DateAndTime    OccurredTime;    
}WIFIErrorCodeOccurred;  // 0x81, 0x09 request

typedef struct{    
    char machineIdentification[_WIFI_Length_MachineID];
    char  MacAddress[_WIFI_Length_MacAddress];
    UINT16 serviceCode;
    DateAndTime  OccurredTime;
    char      serviceNote[1024];
}WIFIServiceCodeOccurred;  // 0x81, 0x10 request

typedef struct{    
    char machineIdentification[_WIFI_Length_MachineID];
    char MacAddress[_WIFI_Length_MacAddress];
    char      isAMsystem;
}WIFIServiceAM;

// 0x82  WIFI_LINGO_NETWORK_TIME_MESSAGES

typedef struct
{
	char timeZone[_WIFI_Length_TimeZone];
} WIFITimeRequestRegion;  // 0x82, 0x01 request

typedef struct
{
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
	UINT16 ms;
} WIFITimeResponseRegion;  // 0x82, 0x01 response

// 0x83  WIFI_LINGO_USER_MANAGEMENT_MESSAGES

typedef struct
{
	UINT8 userConsoleId;
	UINT8 UserSetup;
	unsigned char xid[XIDMAXLENGTH];
    unsigned char passcode[_XidPassWordLength];
    char macAddress[_WIFI_Length_MacAddress];
    char machineIdentification[_WIFI_Length_MachineID];
    DateAndTime lastChangeTime;
	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 language;
	UINT8 DisplayUnits;
	char name[65];
	UINT8 AcceptedTerms;
	UINT8 WorkoutDataShare;
} WIFIExternalUserRequestData;  // 0x83, 0x01 request

typedef struct
{
	UINT8 userConsoleId;
	UINT8 UserSetup;
	unsigned char xid[XIDMAXLENGTH];
    unsigned char passcode[_XidPassWordLength];
	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 language;
	UINT8 DisplayUnits;
	char name[65];
	UINT8 AcceptedTerms;
	UINT8 WorkoutDataShare;
} WIFIExternalUserResponseData;  // 0x83, 0x01 response

typedef struct
{
	UINT8 userId;
	unsigned char xid[XIDMAXLENGTH];
    unsigned char passcode[_XidPassWordLength];
    char macAddress[_WIFI_Length_MacAddress];
    char MachineID[_WIFI_Length_MachineID];
} WIFIUserLoginRequest;  // 0x83, 0x02 request

typedef struct
{
	UINT8 userId;
    char externalUserId[_WIFI_Length_UserExternalID];
	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
    UINT16 birth_year;
    UINT8 birth_month;
    UINT8 birth_day;
	UINT8 language;
	UINT8 DisplayUnits;
	char userName[65];
        UINT8 AcceptedTerms;
	UINT8 WorkoutDataShare;
} WIFIUserLoginResponse;  // 0x83, 0x02 response

typedef struct
{
	UINT8 userId;
    char macAddress[_WIFI_Length_MacAddress];
    char machineIdentification[_WIFI_Length_MachineID];
	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 language;
	UINT8 DisplayUints;
	char userName[65];
	char email[65];
	unsigned char xid[XIDMAXLENGTH];
    unsigned char passcode[_XidPassWordLength];
	UINT8 AcceptedTerms;
	UINT8 WorkoutDataShare;
} WIFIUserRegisterRequest;  // 0x83, 0x03 request

typedef struct
{
	UINT8 userId;
    char externalUserId[_WIFI_Length_UserExternalID];
} WIFIUserRegisterResponse;  // 0x83, 0x03 response

typedef struct
{
	unsigned char xid[XIDMAXLENGTH];
} WIFIUserCheckXid;  // 0x83, 0x04 request (0x06 request)

typedef struct
{
    UINT8 macAddress[_WIFI_Length_MacAddress];
    char machineIdentification[_WIFI_Length_MachineID];
} WIFIClearUsers;  // 0x83, 0x05 request

typedef struct
{
	UINT8 UserIndex;
    char machineIdentification[_WIFI_Length_MachineID];
}WIFIUnlinkUser;  // 0x83, 0x0a request

typedef struct{
    char MachineID[_WIFI_Length_MachineID];
    char macAddress[_WIFI_Length_MacAddress];
    UINT8 userID;
    char RFIDTag[RFTagLength];
    UINT8  RFIDCarrier;
}WIFIRFidLoginReq;  // 0x83, 0x12 request

typedef struct{
    UINT8 userID;
    char externalUserID[_WIFI_Length_UserExternalID];
    UINT8 gender;
    UINT16 userWeightX10;
    UINT8   userAge;
    UINT16 userBirthYear;
    UINT8 userBirthMonth;
    UINT8 userBirthDay;
    UINT8   userLanguageId;
    UINT8   displayUnits;
    char      userName[65];
    UINT8   acceptedTerms;
    UINT8   shareWorkoutData;
}WIFIRFidLoginResp;  // 0x83, 0x12 response

typedef struct{
    char MachineID[_WIFI_Length_MachineID];
    char macAddress[_WIFI_Length_MacAddress];
    UINT8 userID;
    char externalUserID[_WIFI_Length_UserExternalID];
    char RFIDTag[16];
    UINT8  RFIDCarrier;
}WIFIRFidAssociateReq;  // 0x83, 0x13 request

typedef struct{
    char MachineID[_WIFI_Length_MachineID];
    char macAddress[_WIFI_Length_MacAddress];
    char RFIDTag[16];
    UINT8  RFIDCarrier;
}WIFIRFidDisassociateReq;  // 0x83, 0x14 request

typedef union
{
	UINT8 UByte;
	char Byte;
	INT16 Word;
	UINT16 UWord;
	INT32 DoubleWord;
	UINT32 UDoubleWord;
        //Acknowledge 
	WIFIDataAck Ack;
        //0x01
        WIFIDataAPConnection APConnection;
	//0x02
        WIFIDataLingoVersion LingoVersion;
	WIFIDataMachineInfo MachineInfo;
	WIFIDataSpeedCalibration SpeedCalibration;
	WIFIDataPWMCal PWMCal;
	WIFIDataECBCal ECBCal;
	WIFIDataWattsTable WattsTable;
	WIFIDataDataServer DataServer;
	WIFIMachineStateRequest MachineStateRequest;
	WIFIDataUserData UserData;
	WIFIWorkoutDataRemote WorkoutDataRemote;
	//0x03
        WIFIDataProgramStart ProgramStart;
	WIFIDataProgramChange ProgramChange;
	WIFISetSpeed SetSpeed;
	//0x04
        WIFIDataDefaults Defaults;
	WIFIMachineTypeData MachineType;
	WIFIDataRegion Region;
	//0x80
        WIFIWorkoutDataEx    WorkoutData_EX;
        WIFIWorkoutDataCompleteEx WorkoutTrackingDataComplete_EX;
        WIFIWorkoutDataSprint8CompleteEx WorkoutSprint8CompleteEx;
        WIFIWorkoutDataFitnessTestCompleteEx WorkoutFitnessTestCompleteEx;
        //0x81
        WIFIMachineRegisterRequest RegisterRequest;
	WIFIMachineRegisterResponse RegisterResponse;
	WIFIDAPILocation   DAPILocation;
        WIFIDAPIResponse AMWIFIDAPIResponse;
        WIFIDATAGetLatestVersion getlatestVersion;
        WIFIDATAResponseLatestVersion responseLatestVersion;
        WIFIDataRequestMachineInfo_EX   MachineInfo_EX;
        WIFIMachineInfoEXResponse MachineInfoExResponse;
        WIFIUpdateMachineStats  MachineStats;
        WIFIErrorCodeOccurred    ErrorCodeOccurred;
        WIFIServiceCodeOccurred ServiceCodeOccurred;
        WIFIServiceAM   ServiceAM;
        //0x82
	WIFITimeRequestRegion TimeRequest;
	WIFITimeResponseRegion TimeResponse;
        //0x83
        WIFIExternalUserRequestData UserDataRequest;
	WIFIExternalUserResponseData UserDataResponse;
	WIFIUserLoginRequest UserLoginRequest;
	WIFIUserLoginResponse UserLoginResponse;
	WIFIUserRegisterRequest UserRegisterRequest;
	WIFIUserRegisterResponse UserRegisterResponse;
	WIFIUserCheckXid UserCheckXid;
	WIFIClearUsers ClearUsers;
	WIFIUnlinkUser UnlinkUserData;
        WIFIRFidLoginReq  rfidLoginReq;
        WIFIRFidLoginResp rfidLoginResp;
        WIFIRFidAssociateReq rfidAssociateReq;
        WIFIRFidDisassociateReq rfidDisassociateReq;
} WifiCommandData;

typedef struct
{
	UINT16 syncWord;
	UINT16 transactionId;
	UINT8 lingoId;
	UINT8 messageId;
	UINT16 dataSize;
	UINT16 checksum;
} WifiHeader;

typedef struct _WIFI_COMMAND
{
	WifiHeader header;
	WifiCommandData data;
} WifiCommand;

typedef enum{
    WIFI_STATUS_START,
    WIFI_STATUS_IDLE= WIFI_STATUS_START,
    WIFI_STATUS_OFFLINE,
    WIFI_STATUS_LOGIN,
    WIFI_STATUS_SYNC_USER,
    WIFI_STATUS_SUCCESS,
    WIFI_STATUS_FAIL,
    WIFI_STATUS_WAITTING,
    WIFI_STATUS_TIMEOUT,
    WIFI_STATUS_ACTIVATED,
    WIFI_STATUS_AP_MODE,
    WIFI_STATUS_RFID_ASSOCIATE_SUCCESS,
    WIFI_STATUS_RFID_ASSOCIATE_FAIL,    
    WIFI_STATUS_RFID_DISASSOCIATE_SUCCESS,
    WIFI_STATUS_RFID_DISASSOCIATE_FAIL,
    WIFI_STATUS_RSCU_CHECKING,
    WIFI_STATUS_DOWNLOADING_SOFTWARE,
    WIFI_STATUS_DOWNLOAD_COMPLETED,
    WIFI_STATUS_END
}WIFI_STATUS_TYPE;

typedef enum{
    RFIDCARRIERSTART,
    LEGIC_RF_STANARD = RFIDCARRIERSTART,
    ISO_15693,
    ISO_14443_A,
    ISO_14443_B,
    INSIDE_SECURE,
    INSIDE_SECURE_1,
    RFIDCARRIEREND,
    EM4001,
    EM4100,
}RFIDCARRIER;

typedef struct{
    UINT8 xidCheckResponse :2;
    UINT8 registrationCheckResponse:2;
    UINT8 loginCheckResponse:2;
    UINT8 DeleteUserCheckResponse:2;
    UINT8 userInfoUpdated:2;
    UINT8 RfidAssociateflag:2;
    UINT8 RfidDisassociateflag:2;
    UINT8 RfidLoginflag:2;
    UINT8 _feature:2;
}UserResponse;

typedef struct{
    UINT8 userIndex;
    UINT8 rfidTag[RFTagLength];
    RF_Standard rfidCarrier;  //  RFIDCARRIER
}RFIDData;

#pragma pack(pop)


void ConnectWiFi_AP(void);
void ProcessTCPData( int port, char* data, int data_size );
void RunWiFiInterface( UINT16 timer );
void ProcessHTTPPost( char* message, int length, UINT16 timer );
void WifiConnectionStatusChange( char wifiConnected );
void CheckXidUnique( unsigned char* xid );
UCHAR Wifi_Get_EnggineTestResult(void);
void AddWifiResponseState( UINT8 state, UINT16 command, UINT16 error, UINT8 port );
//UINT8 WIFIGetDeleteResult(void);
//void ConnectAPForFactoryTest( void );
void ConnectAPForFactoryDefault( void );
void wifi_ResetInit(void);
// WiFi 
void WiFi_memset(void *s, char c, long n);
void WiFi_memcpy(void *s1, const void *s2, long n);
char WiFi_memcmp(const void *s1, const void *s2, long n);
long WiFi_CountChar(const void *s1, long MaxLength);
// 2015.01.21
void WIFIClearStateBuffer(void);
void WiFi_Interface_ClearUserExternalID(void);
//RFID 
void wifiSetRFidLogin(UINT8 userIndex, char* rfidTag, RF_Standard rfidCarrier);  //  RFIDCARRIER rfidCarrier
UINT8 CheckRFIDLoginResult(void);
//void wifiSetRFidAssociate(UINT8 userIndex, char* rfidTag,RF_Standard rfidCarrier);  //  RFIDCARRIER rfidCarrier
//void wifiSetRFidDisassociate(UINT8* rfidTag, RF_Standard rfidCarrier);  //  RFIDCARRIER rfidCarrier
UINT8 CheckRFIDAssociate(void);
void SetRFIDAssociate(char AF);
void wifi_interface_ClearLoginFlag(void);
//RSCU
typedef union
{
    char data[1+256+32];
    struct
    {
        char softwaretype;
        char URL[256];
        char MD5[32];
    }DatatoModule;
}RSCUData;
//
enum
{
    //--- Input
    _RSCU_Reset,
    _RSCU_State,
    _RSCU_Update,
    //--- Output
    // status
    _RSCU_Begin,
    _RSCU_Pause,
    _RSCU_CheckForSoftwareOnDAPI,
    _RSCU_DownloadSoftwareToModule,
    _RSCU_DownloadSoftwareToEEPROM,
    _RSCU_CheckForSoftwareOnModule,
    _RSCU_SoftwareDownloadOK,   
    _RSCU_CanUpdate,
    // error
    _RSCU_NoLatestSoftware,
    _RSCU_CanNotUpdate,
    _RSCU_BinCheckError,
    _RSCU_EEPRomError,
    _RSCU_Disconnection,
};
unsigned char WiFi_RSCU_Process(unsigned char by_Mode);
void WiFi_RSCU_Open(void);
void WiFi_RSCU_Close(void);
unsigned char WiFi_CheckRSCUVersion(char* RSCUVersionString);
void UpdateMachineStats(void);
void WiFi_OccurredErrorCode(unsigned short by_Dat);
void WiFi_Timer_Int(void);
unsigned short WiFi_DownloadPercentage(void);
bool WIFICanLoginUser(void);

typedef union 
{
    unsigned char All[264];
    struct Info
    {
      unsigned char Name[32];
      unsigned char Signal;
    }AP_Infos[8];
}AP_Information;

#endif /* WIFI_INTERFACE_H_ */
