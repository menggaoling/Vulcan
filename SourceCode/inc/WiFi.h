

#ifndef __WIFI_H__
#define __WIFI_H__

// 20110704
#define _WiFi_UpdateUCBLCB_
#define _WiFi_AM_
//

#define   _MaxDataLength_       128
#define   _MaxBufferLength_     (_MaxDataLength_*2+10)

#define   _WiFiGetStatus_               0 
#define   _WiFiGetDemoData_             1 
#define   _WiFiInitialAP_               2
#define   _WiFiManualMode_              4
#define   _WiFiFWupdate_                5
#define   _WiFiLCBupdateCheck_          6
#define   _WiFiLCBupdateEnd_            7
#define   _WiFiUCBupdateCheck_          8
#define   _WiFiUCBupdateEnd_            9
#define   _WiFiAMMode_                  10
// 20120117
#define   _WiFiDelayComm_               11  
#define   _WiFiDelayAPConnect_          12
//
typedef union {
  struct {
    unsigned short APConnect:1 ;	//0
    unsigned short Error:1 ;	//1
    unsigned short Unuse:1 ;	//2
    unsigned short Resend:1 ;	//3
    unsigned short BufferFull:1 ;	//4
    unsigned short DataReady:1 ;	//5
    unsigned short TcpIpConnect:1 ;	//6
    unsigned short Rev1:1 ;		//7
    //
#ifdef  _WiFi_UpdateUCBLCB_
    unsigned short RemoteWiFiUpdate: 1;	//8
		unsigned short RemoteUCBUpdate: 1;	//9
		unsigned short RemoteLCBUpdate: 1;	//10
    unsigned short Rev2:5 ;
#else
    unsigned short Rev2:8 ;     
#endif
    
  } b ;
  unsigned short All ;
} WiFi_RxStatus ;


typedef struct {
  unsigned short Command ;
  unsigned short Length ;
  unsigned char *pData ;
} WiFiPacket ;


typedef struct {
  unsigned short  Command ;
  unsigned short  Length ;
  WiFi_RxStatus   Status ;
  unsigned short  ErrorCode ;
  unsigned char   *pData ;
} WiFiReponsePacket ;


typedef union {
  struct {
    unsigned long WiFiInitalReady:1 ;
    unsigned long WiFiHWReady:1 ;
    unsigned long WiFiPowerOnOnlineDelay:1 ;   // 20120130
    unsigned long Tx:1 ;
    unsigned long TxEnd:1 ;
    unsigned long TxStart:1 ;
    unsigned long Rx:1 ;
    unsigned long RxStartByte:1 ;  
    unsigned long RxEnd:1 ;
    unsigned long RxTimeoutOccur:1 ;    
    unsigned long RxCrcOccur:1 ;
    unsigned long Error:1 ;
    unsigned long WiFiProgrammerMode:1 ;
    unsigned long WiFiProgrammerBusy:1 ;
    //
    #ifdef  _WiFi_UpdateUCBLCB_
    unsigned long WiFiUpdateUCB:1 ;
    unsigned long WiFiUpdateLCB:1 ;
    #endif
    //
    unsigned long SetSSID:1 ;
    unsigned long SetSecurityType:1 ;
    unsigned long SetSecurityKey:1 ;
    unsigned long SetConnectAP:1 ;
    unsigned long SetDisConnectAP:1 ;
    unsigned long SetTCPIPAddress:1 ;
    unsigned long SetTCPIPPort:1 ;
    unsigned long SetConnectTCPIP:1 ;
    unsigned long SetDisConnectTCPIP:1 ;
    unsigned long GetSSID:1 ;
    unsigned long GetSecurityType:1 ;
    unsigned long GetSecurityKey:1 ;
    //
    #ifdef  _WiFi_AM_
    unsigned long SendDataToWiFiAM:1 ;
    unsigned long SendAMtoWeb:1 ;
    unsigned long SendAMtoWebStart:1 ;
    #endif
    //
  } b;
  unsigned long Status ;
} WiFiControlStatus ;



typedef struct {
  // WiFi Access point setting
  unsigned char SSID[33] ;
  unsigned char SecurityType ;
  unsigned char SecurityKey[64] ;
  // Server Setting
  unsigned char TCPIP[129] ;
  unsigned short TCPIPPort ;
  // Client Setting
  unsigned char MAC[6] ;
  unsigned char IP[6] ;
  //  
  unsigned char WiFiModuleVer[2] ;
  unsigned char WiFiUartVer[2] ;
  // 2012011
  unsigned char APSSID[33] ;
  unsigned char APSecurityType ;
  unsigned char APSecurityKey[64] ;
  //
} WiFiModule ;



typedef struct {
  unsigned short Count ;
  unsigned short TotalLength ;
  unsigned char Buffer[_MaxBufferLength_] ;
//  unsigned char Status ;
} WiFi_Data ;



typedef struct {
  unsigned short Command ;
  unsigned short Length ;
  WiFi_RxStatus Status ;
  unsigned short ErrorCode ;
  unsigned char Buffer[_MaxDataLength_] ;
} WiFiProtocolDataMapping ;



typedef enum {
  MemoryError = 0,
  WiFiReceiveStop,
  WiFiWaitting,
  WiFiWaittingTimeout,
  WiFiReceiveCrcError ,
  WiFiReceiveSuccess,  
} ReponseStatus ;




//------------------------------------------------------------------------------
// Database type define
typedef struct {
  unsigned char Gender ;    //(0:Female, 1:Male)
  unsigned char Age ;
  struct  {
    unsigned short Data:15 ;//
    unsigned short Unit:1 ; //0: Metric Unit (kg), 1:Imperial Unit (lbs)
  } Weigth ;
} WiFiUserInfoDB ;


typedef struct {
  unsigned short Speed:15 ; //Resolution = 0.1
  unsigned short Unit:1 ;   //0: Metric Unit (km), 1:Imperial Unit (mile)
} WiFiWorkoutSpeedDB ;


typedef struct {
  unsigned short ID ;
  unsigned short Level ;
} WiFiWorkoutProgramLevelDB ;

typedef struct {            //Workout Distance/ Workout floor
  unsigned short DistancePerFloor:15 ; //Resolution = 0.1
  unsigned short Unit:1 ;   //0: Metric Unit (km), 1:Imperial Unit (mile) 
} WiFiWorkoutDistancePerFloorDB ;

typedef struct {
  unsigned short InclinePercent:15 ; //Resolution = 0.1%
  unsigned short Sign:1 ;           //0: Positive , 1: Negative
} WiFiWorkoutInclineDB ;


/*
typedef struct {
  unsigned char                   ProgramName[128] ;
  WiFiWorkoutProgramLevelDB       WorkoutProgram ;
  WiFiUserInfoDB                  UserInfo ;
  WiFiWorkoutSpeedDB              Speed ;
  WiFiWorkoutDistancePerFloorDB   DistancePerFloor ;
  WiFiWorkoutInclineDB            Incline ;
  unsigned short                  RPM ;
  unsigned long                   Time ;
  unsigned short                  Calories ;
  unsigned short                  METs ;
  unsigned short                  Watts ;
  unsigned char                   HR ;
  unsigned short                  ErrorCode ;
  unsigned short                  ServiceCode ;
  
} WiFiInformationDataDB ;

*/

//------------------------------------------------------------------------------
// for test
/*
typedef union {
  struct {
	unsigned short Incline;
	unsigned short Speed;
	unsigned short Distance;
	unsigned short Time;
	unsigned short Calories;
	unsigned short HR;
  } Member ;
  unsigned char All[12] ;
}SDemo_TM;	
*/

#ifdef  _WiFi_UpdateUCBLCB_

#define   _LCB_           0
#define   _UCB_           1


typedef union {
  struct {
    unsigned char HW[6] ;
    unsigned char Mask ;
    unsigned char Ver[4] ;
    unsigned char Rev[21] ;
    unsigned char CheckSum[16] ;
    unsigned char InfoOK ;
  } Member ;
  unsigned char Data[49] ;
} LCBInfo ;

typedef union {
  struct {
    unsigned char McuType[20] ;
    unsigned char ModuleName[20] ;
    unsigned char ModuleNo[20] ;
    unsigned char Product[20] ;
    unsigned char Version[20] ;
    unsigned char InfoOK ;    
  } Member ;
  unsigned char Data[101] ;
} UCBInfo ;

#endif

//------------------------------------------------------------------------------
void WiFi_Initial(void) ;
void WiFi_SetModulePowerReady(void) ;
void WiFi_Process(void) ;
void WiFi_UartTxRx_Information(void) ;


void WiFi_DefaultSSID( unsigned char *pData ) ;
void WiFi_DefaultSecurityType( unsigned char Data ) ;
void WiFi_DefaultSecurityKey( unsigned char *pData ) ;
void WiFi_DefaultTCPIPAddress( unsigned char *pData ) ;
void WiFi_DefaultTCPIPPort( unsigned short Data ) ;
void WiFi_SetConnectionAP( void ) ;
void WiFi_SetConnectionTCPIP( void ) ;
void WiFi_SetCommWorkMode( unsigned char Mode ) ;
unsigned char WiFi_GetCommWorkMode(void);
void WiFi_SendEntryProgrammingMode(unsigned char *ReturnStatus ) ;
void WiFi_ReadProgrammingStatus(unsigned char *ReturnStatus ) ;
void WiFi_SendProgrammingEraseMode(void) ;
void WiFi_SendLeaveProgrammingMode(void) ;
void WiFi_ReadProgrammingStatus(unsigned char *ReturnStatus ) ;
void WiFi_SendProgrammingAddress(unsigned char *Address ) ;
void WiFi_SendProgrammingData(unsigned char *pData, unsigned short length ) ;
WiFiControlStatus *WiFi_GetWiFiControlStatus(void) ;
void WiFi_GetModuleVer(void) ;
void WiFi_GetUartVer(void) ;
// 20110704
#ifdef  _WiFi_UpdateUCBLCB_
void WiFi_UCBLCB_LeaveProgramming(unsigned char Mode ) ;
void WiFi_Get_UCBLCBProgrammingStatus(unsigned char *ReturnStatus ) ;
unsigned char WiFi_UCBLCB_ReadProgrammingData( unsigned char Mode ,unsigned long Address, unsigned short Length, unsigned char *ReturnStatus ) ;
void WiFi_ClearLCBProgrammingStatus(void) ;
void WiFi_ClearUCBProgrammingStatus(void) ;
unsigned char WiFi_CheckUCBInfo( unsigned char *Source,unsigned char *Target,unsigned char Mode ) ;
#endif
//
// 20111212 --------------------------------------------------------------------
#ifdef  _WiFi_AM_
void WiFi_SendAM_DataToModule(unsigned long Address, unsigned short Length , unsigned char *pData ) ;
void WiFi_SetAM_DataToWeb(void) ;
void __WiFimemset(void *s, char c, long n);
long __WiFistrlen(const char *s) ; 
#endif
// 20120111---------------------------------------------------------------------
void WiFi_ReadSSID(void) ;
void WiFi_ReadSecurityType(void) ;
void WiFi_ReadSecurityKey(void) ;
void WiFi_SendEraseAllFlash(void) ;
// 20120130---------------------------------------------------------------------
unsigned char WiFi_GetAPConnectStatus(void);





#endif /* __WIFI_H__ */


