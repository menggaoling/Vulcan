//##############################################################################
//
//
//##############################################################################
/* Includes ------------------------------------------------------------------*/
#include  "WiFi.h"
#include  "WiFi_Command.h"
#include  "stm32f10x_conf.h"
#include  "stdio.h"
#include  "string.h"
#include  "stm32_reg.h"
#include  "General.h"
#include  "UsbFunction.h"
#include  "Boot.h"
//20111213
#include  "HT1381.h"
#include  "WebFile.h"
//

#define   _LookUpTable                  1
#define   _CRC16_NotInclude_Stuffing_   1
//#define   _DebugWiFiComm_               1

//
unsigned short WiFi_CommandTime ;
//
WiFiPacket        WiFiSendBuffer ;
WiFiReponsePacket WiFiReadBuffer ;

WiFi_Data WiFi_Rx ;
WiFi_Data WiFi_Tx ;
WiFiPacket  *WiFiSendPacket ;
WiFiReponsePacket  *WiFiReceivePacket ;
WiFiProtocolDataMapping *WiFiReceiverData ;
WiFiModule  *WiFiDataBuffer ;
unsigned short WiFi_RxTimeout ;
unsigned char WiFi_WorkMode  ;
unsigned char WiFi_NormalCheckStep ; // 20120131
volatile WiFiControlStatus   WiFiProcessStatus ;
unsigned short TxCounter ;
unsigned short RxCounter ;
WiFiModule WiFiModuleData ;
#ifdef    _DebugWiFiComm_
unsigned char ReturnData[256] ;
#endif
unsigned char SendDataBuffer[256] ;
//

#ifdef  _WiFi_UpdateUCBLCB_
extern const unsigned char User_McuType[20] ;
extern const unsigned char User_ModuleName[20] ;
extern const unsigned char User_ModuleNo[20] ;
extern const unsigned char User_Product[20] ;
extern const unsigned char User_Version[20] ;

LCBInfo WiFi_LCBInfoData ;
UCBInfo WiFi_UCBInfoData ;
unsigned char UCBLCBUpdateStep ;
#define   _Check_LCB_Info_        0
#define   _Check_LCB_Data1_       1 
#define   _Check_LCB_Data2_       2
#define   _Check_LCB_Data3_       3
#define   _Check_LCB_Data4_       4
#define   _Check_LCB_Data5_       5 
#define   _Check_LCB_Data6_       6
#define   _Check_LCB_Data7_       7
#define   _Check_LCB_Data8_       8
#define   _Check_LCB_Data9_       9 
#define   _Check_LCB_Data10_      10
#define   _Check_LCB_Data11_      11
#define   _Check_LCB_Data12_      12
#define   _Check_LCB_Data13_      13
#define   _Check_LCB_Data14_      14
#define   _Check_LCB_Data15_      15
//
#define   _Check_UCB_Info_        16


#define   _LCB_DATA1_ADDR_        0x4030
#define   _LCB_DATA2_ADDR_        (_LCB_DATA1_ADDR_+2200)
#define   _LCB_DATA3_ADDR_        (_LCB_DATA2_ADDR_+2200)
#define   _LCB_DATA4_ADDR_        (_LCB_DATA3_ADDR_+2200)
#define   _LCB_DATA5_ADDR_        (_LCB_DATA4_ADDR_+2200)
#define   _LCB_DATA6_ADDR_        (_LCB_DATA5_ADDR_+2200)
#define   _LCB_DATA7_ADDR_        (_LCB_DATA6_ADDR_+2200)
#define   _LCB_DATA8_ADDR_        (_LCB_DATA7_ADDR_+2200)
#define   _LCB_DATA9_ADDR_        (_LCB_DATA8_ADDR_+2200)
#define   _LCB_DATA10_ADDR_       (_LCB_DATA9_ADDR_+2200)
#define   _LCB_DATA11_ADDR_       (_LCB_DATA10_ADDR_+2200)
#define   _LCB_DATA12_ADDR_       (_LCB_DATA11_ADDR_+2200)
#define   _LCB_DATA13_ADDR_       (_LCB_DATA12_ADDR_+2200)
#define   _LCB_DATA14_ADDR_       (_LCB_DATA13_ADDR_+2200)
#define   _LCB_DATA15_ADDR_       (_LCB_DATA14_ADDR_+2200)


const unsigned long LcbDataAddr[15] = {
                                        _LCB_DATA1_ADDR_,
                                        _LCB_DATA2_ADDR_,
                                        _LCB_DATA3_ADDR_,
                                        _LCB_DATA4_ADDR_,
                                        _LCB_DATA5_ADDR_,
                                        _LCB_DATA6_ADDR_,
                                        _LCB_DATA7_ADDR_,
                                        _LCB_DATA8_ADDR_,
                                        _LCB_DATA9_ADDR_,
                                        _LCB_DATA10_ADDR_,
                                        _LCB_DATA11_ADDR_,
                                        _LCB_DATA12_ADDR_,
                                        _LCB_DATA13_ADDR_,
                                        _LCB_DATA14_ADDR_,
                                        _LCB_DATA15_ADDR_
                                      } ;
const unsigned short LcbDataLength[15] = {
                                          2200,     //1
                                          2200,     //2
                                          2200,     //3
                                          2200,     //4
                                          2200,     //5
                                          2200,     //6
                                          2200,     //7
                                          2200,     //8
                                          2200,     //9
                                          2200,     //10
                                          2200,     //11
                                          2200,     //12
                                          2200,     //13
                                          2200,     //14
                                          1920      //15
                                        } ;

unsigned char DataBuffer[100] ;
unsigned long DataAddr = 0;
unsigned short DataLength = 0 ;
unsigned short DataCount = 0 ;
unsigned char DataCRC8 = 0 ;
unsigned short DataReadSize = 0 ;
unsigned short WiFi_DelayTimeCount = 0 ; // 20120117
#endif
// 20111213
#ifdef  _WiFi_AM_
unsigned char WebDataStep ;
unsigned char WebDataRtyCount ;
// 20120214 Add
const unsigned char WebWorkoutPacket[22] = { 10,11,12,13,14,15,16,19,20,21,22,23,24,25,26,27,28,29,30,31,32,0 } ;
const unsigned char WebErrorPacket[14] = { 10,11,12,13,14,15,16,17,19,20,21,31,32,0 } ;
const unsigned char WebServicePacket[14] = { 10,11,12,13,14,15,16,18,19,20,21,31,32,0 } ;
unsigned char const *WebSendDataPacketPtr ;
//
union {
  struct {
    unsigned long WaitWiFiMacReady: 1 ;
    unsigned long WiFiMacReady:1 ;
  } b ;
  unsigned long All ;
} UploadToWiFiStatus ;
#endif
//
//------------------------------------------------------------------------------
void WiFi_ConvertSendPacket(void) ;
unsigned char WiFi_ConvertSpecialData( unsigned char *pData, unsigned char InData ) ;
unsigned short WiFi_CRC_CITT( unsigned char *pData, unsigned short Length ) ;
ReponseStatus WiFi_ReponseProcess(void) ;
void WiFi_RemoveSCF(void) ;
ReponseStatus WiFi_ReveiveAnalysis(void) ;
void WiFi_SetSendPacket( unsigned short Cmd, unsigned char *pData , unsigned short Sendlen, unsigned char *rData, unsigned short Readlen ) ;


//void WiFi_TestCommand(void) ;

void WiFi_ReadStatus(void) ;
void WiFi_ReadIPAddress(void) ;
void WiFi_ReadMACAddress(void) ;

void WiFi_SendTCPIPAddress(void) ;
void WiFi_SendTCPIPPort(void) ;
void WiFi_SendSSID(void) ;
void WiFi_SendSecurityType(void) ;
void WiFi_SendSecurityKey(void) ;
//
#define   _WiFiCmdTime_             100
#define   _PowerOnDelayTime_        ( 5000 / _WiFiCmdTime_ )
#define   _DelayReConnectTime_      ( 1000 / _WiFiCmdTime_ )
#define   _PowerOnOnlineDelayTime_  ( 1000 / _WiFiCmdTime_ )

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_Initial(void)
{
  USART_InitTypeDef 	USART_InitStructure;
  NVIC_InitTypeDef 	NVIC_InitStructure;
  //
  WiFiProcessStatus.b.WiFiInitalReady = 0 ;
  /* Enable the USART5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//Channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);	
  //

  //
  USART_Cmd(UART5, DISABLE);	
  USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
  USART_ITConfig(UART5, USART_IT_TXE,DISABLE);
  USART_ITConfig(UART5, USART_IT_TC,DISABLE);
  //
  WebFile_Initial();
  // Initial RAM Parameter 
  WiFiProcessStatus.Status = 0 ;
  WiFiSendPacket = 0 ;        // address
  WiFiReceivePacket = 0 ;     // address 
  WiFiReceiverData = 0 ;      // address 
  WiFiDataBuffer = 0 ;        // address
  WiFi_RxTimeout = 0 ;
  WiFi_WorkMode = _WiFiDelayComm_ ; // 20120117
  WiFi_DelayTimeCount = _PowerOnDelayTime_ ;
  WiFi_Rx.Count = 0 ;
  WiFi_Rx.TotalLength = 0 ;
  WiFi_Tx.Count = 0 ;
  WiFi_Tx.TotalLength = 0 ;
  TxCounter = 0 ;
  RxCounter = 0 ;
  WiFi_CommandTime = 0 ;
  WiFi_NormalCheckStep = 0 ;
  WebSendDataPacketPtr = &WebWorkoutPacket[0] ;
  UploadToWiFiStatus.All = 0 ;
  //
  //----------------------------------------------------------------------------
  //
  WiFiSendPacket = &WiFiSendBuffer ;
  WiFiReceivePacket = &WiFiReadBuffer ;
  WiFiDataBuffer = &WiFiModuleData ;
  /*
  TMDataToWiFi.Member.Incline = 0 ;
  TMDataToWiFi.Member.Speed = 0 ;
  TMDataToWiFi.Member.Distance = 0 ;
  TMDataToWiFi.Member.Time = 0 ;
  TMDataToWiFi.Member.Calories = 0 ;
  TMDataToWiFi.Member.HR = 0 ;
  */
  // 20110704 Clear Buffer
  #ifdef  _WiFi_UpdateUCBLCB_
  __memset(&WiFi_LCBInfoData.Data[0],0,sizeof(WiFi_LCBInfoData.Data)) ;
  __memset(&WiFi_UCBInfoData.Data[0],0,sizeof(WiFi_UCBInfoData.Data)) ;
  #endif
  __memset(&WiFiModuleData.MAC[0],0,sizeof(WiFiModuleData.MAC)) ;
  __memset(&WiFiModuleData.IP[0],0,sizeof(WiFiModuleData.IP)) ;
  //
  WiFiReceivePacket->Status.All = 0 ;
  //
  USART_InitStructure.USART_BaudRate = 115200 ;//19200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure USART5 */
  USART_Init(UART5, &USART_InitStructure);
  /* Enable USART5 Receive interrupt */
  USART_ITConfig(UART5,USART_IT_TC,ENABLE);
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
  //
  USART_Cmd(UART5, ENABLE);	
  //
  /* Enable the USART5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//Channel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
  //
  WiFiProcessStatus.b.WiFiInitalReady = 1 ;
  WiFiProcessStatus.b.WiFiPowerOnOnlineDelay = 1 ;
  //
  return ;
}


void WiFi_SetModulePowerReady()
{
  WiFiProcessStatus.b.WiFiInitalReady = 1 ;
  return ;
}
/*
void WiFi_TestCommand()
{
  WiFiSendPacket->Command = DDemo_GetAllData ;
  WiFiSendPacket->Length = 12 ;
  WiFiSendPacket->pData = &TMDataToWiFi.All[0] ;
  WiFiReceivePacket->Command = WiFiSendPacket->Command ;
  WiFiReceivePacket->Length = 0 ;
  WiFiReceivePacket->pData = &ReturnData[0] ;
  return ;
}
*/


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ReadStatus()
{
  // Set send packet
#ifdef    _DebugWiFiComm_
  WiFi_SetSendPacket(WiFi_GetModuleStatus,0,0,&ReturnData[0],2) ;
#else
  WiFi_SetSendPacket(WiFi_GetModuleStatus,0,0,0,0);
#endif 
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ReadIPAddress()
{
  WiFi_SetSendPacket(WiFi_GetIPAddress,0,0,&WiFiModuleData.IP[0],6) ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ReadMACAddress()
{
  WiFi_SetSendPacket(WiFi_GetMACAddress,0,0,&WiFiModuleData.MAC[0],6) ;
  return ;
}






//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SetConnectionAP( )
{
  WiFiProcessStatus.b.SetConnectAP = 1 ;
  WiFiProcessStatus.b.SetConnectTCPIP = 1 ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SetConnectionTCPIP( ) 
{
  WiFiProcessStatus.b.SetConnectTCPIP = 1 ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_DefaultTCPIPAddress( unsigned char *pData ) 
{
  unsigned short Len ;
  Len = __strlen((const char*)pData) ;
  if( Len > 128 )
      Len = 128 ;
  // Copy Data To Memory
  __memset(&WiFiModuleData.TCPIP[0],0,sizeof(WiFiModuleData.TCPIP)) ;
  __memcpy(&WiFiModuleData.TCPIP[0],pData,Len) ;
  // Mask by Kunlung 20120111
  //WiFiProcessStatus.b.SetTCPIPAddress = 1 ;
  //
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendTCPIPAddress( )
{
  unsigned short Len ;
  Len = __strlen((const char*)&WiFiModuleData.TCPIP[0]) ;
  if( Len > 128 )
      Len = 128 ;
  // Set send packet
#ifdef    _DebugWiFiComm_
  WiFi_SetSendPacket(WiFi_SetTcpIpAddress,&WiFiModuleData.TCPIP[0],Len,&ReturnData[0],Len) ;
#else
  WiFi_SetSendPacket(WiFi_SetTcpIpAddress,&WiFiModuleData.TCPIP[0],Len,0,Len) ; 
#endif 
  return ;
}



//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_DefaultTCPIPPort( unsigned short Data ) 
{
  if( Data <= 9999 )
      {
      WiFiModuleData.TCPIPPort = Data ;
      // Mask by Kunlung 20120111
      //WiFiProcessStatus.b.SetTCPIPPort = 1 ;
      //
      }
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendTCPIPPort( )
{
#ifdef    _DebugWiFiComm_
  WiFi_SetSendPacket(WiFi_SetTcpIpPort,(unsigned char*)&WiFiModuleData.TCPIPPort,2,&ReturnData[0],0) ;
#else
  WiFi_SetSendPacket(WiFi_SetTcpIpPort,(unsigned char*)&WiFiModuleData.TCPIPPort,2,0,0) ;
#endif   
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_DefaultSSID( unsigned char *pData ) 
{
  unsigned short Len ;
  Len = __strlen((const char*)pData) ;
  if( Len > 32 )
      Len = 32 ;
  // Copy Data To Memory
  __memset(&WiFiModuleData.SSID[0],0,sizeof(WiFiModuleData.SSID)) ;
  __memcpy(&WiFiModuleData.SSID[0],pData,Len) ;
  //
  // Mask by Kunlung 20120111
  //WiFiProcessStatus.b.SetSSID = 1 ;
  //
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendSSID( )
{
  unsigned short Len ;
  Len = __strlen((const char*)&WiFiModuleData.SSID[0]) ;
  if( Len > 32 )
      Len = 32 ;
  // Set send packet
#ifdef    _DebugWiFiComm_
  WiFi_SetSendPacket(WiFi_SetSSID,&WiFiModuleData.SSID[0],Len,&ReturnData[0],Len) ;
#else
  WiFi_SetSendPacket(WiFi_SetSSID,&WiFiModuleData.SSID[0],Len,0,Len) ; 
#endif  
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ReadSSID()
{
  __memset(&WiFiModuleData.APSSID[0],0,sizeof(WiFiModuleData.APSSID)) ;
  WiFi_SetSendPacket(WiFi_GetSSID,0,0,&WiFiModuleData.APSSID[0],32) ;
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_DefaultSecurityType( unsigned char Data )
{
  if( Data <= 8 )
      {
      WiFiModuleData.SecurityType = Data ;
      // Mask by Kunlung 20120111
      //WiFiProcessStatus.b.SetSecurityType = 1 ;
      }
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendSecurityType( )
{
#ifdef    _DebugWiFiComm_
  WiFi_SetSendPacket(WiFi_SetSecurityType,&WiFiModuleData.SecurityType,1,&ReturnData[0],1) ;
#else
  WiFi_SetSendPacket(WiFi_SetSecurityType,&WiFiModuleData.SecurityType,1,0,1) ; 
#endif
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ReadSecurityType()
{
  WiFiModuleData.APSecurityType = 0 ;
  WiFi_SetSendPacket(WiFi_GetSecurityType,0,0,&WiFiModuleData.APSecurityType,1) ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_DefaultSecurityKey( unsigned char *pData ) 
{
  unsigned short Len ;
  Len = __strlen((const char*)pData) ;
  if( Len > 63 )
      Len = 63 ;
  __memset(&WiFiModuleData.SecurityKey[0],0,sizeof(WiFiModuleData.SecurityKey)) ;
  __memcpy(&WiFiModuleData.SecurityKey[0],pData,Len) ;
  // Mask by Kunlung 20120111
  //WiFiProcessStatus.b.SetSecurityKey = 1 ;
  //
  return ;
}



//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendSecurityKey( )
{
  unsigned short Len ;
  Len = __strlen((const char*)&WiFiModuleData.SecurityKey[0]) ;
  if( Len > 63 )
      Len = 63 ;
#ifdef    _DebugWiFiComm_
  WiFi_SetSendPacket(WiFi_SetSecurityKey,&WiFiModuleData.SecurityKey[0],Len,&ReturnData[0],Len) ;
#else
  WiFi_SetSendPacket(WiFi_SetSecurityKey,&WiFiModuleData.SecurityKey[0],Len,0,Len) ; 
#endif
  
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ReadSecurityKey()
{
  __memset(&WiFiModuleData.APSecurityKey[0],0,sizeof(WiFiModuleData.APSecurityKey)) ;
  WiFi_SetSendPacket(WiFi_GetSecurityKey,0,0,&WiFiModuleData.APSecurityKey[0],63) ;
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendEntryProgrammingMode(unsigned char *ReturnStatus )
{
  __memset(&SendDataBuffer[0],0,256) ;
  //JHTPgr
  SendDataBuffer[0] = 'J' ;
  SendDataBuffer[1] = 'H' ;
  SendDataBuffer[2] = 'T' ;
  SendDataBuffer[3] = 'P' ;
  SendDataBuffer[4] = 'g' ;
  SendDataBuffer[5] = 'r' ;

  WiFi_SetSendPacket(WiFi_EntryProgrammingMode,&SendDataBuffer[0],6,ReturnStatus,1) ;

  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ReadProgrammingStatus(unsigned char *ReturnStatus )
{
  WiFi_SetSendPacket(WiFi_GetProgrammingStatus,0,0,ReturnStatus,1) ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendLeaveProgrammingMode(void )
{
  WiFi_SetSendPacket(WiFi_LeaveProgrammingMode,0,0,0,0) ;
  return ;
}




//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendProgrammingEraseMode(void )
{
  WiFi_SetSendPacket(WiFi_ProgrammingErase,0,0,0,0) ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendProgrammingAddress(unsigned char *Address )
{
  SendDataBuffer[0] = *(Address) ;
  SendDataBuffer[1] = *(Address+1) ;
  SendDataBuffer[2] = *(Address+2) ;
  SendDataBuffer[3] = *(Address+3) ;
  WiFi_SetSendPacket(WiFi_SetProgrammingAddress,&SendDataBuffer[0],4,0,0) ;
  return ;
}



// 20110704 --------------------------------------------------------------------
#ifdef  _WiFi_UpdateUCBLCB_
//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_UCBLCB_LeaveProgramming(unsigned char Mode )
{
  SendDataBuffer[0] = Mode ;
  WiFi_SetSendPacket(WiFi_UCBLCB_LeaveProgrammingMode,&SendDataBuffer[0],1,0,0) ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_Get_UCBLCBProgrammingStatus(unsigned char *ReturnStatus )
{
  WiFi_SetSendPacket(WiFi_UCBLCB_GetProgrammingStatus,0,0,ReturnStatus,1 ) ;
  return ;
}



//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
unsigned char WiFi_UCBLCB_ReadProgrammingData( unsigned char Mode ,unsigned long Address, unsigned short Length, unsigned char *ReturnStatus )
{
  union {
    struct {
    unsigned long LL:8 ;
    unsigned long LH:8 ;
    unsigned long HL:8 ;
    unsigned long HH:8 ;
    } DT ;
    unsigned long All ;
  } Temp ;    
  
  Temp.All = Address ;
  //
  if( Length > 100 )
      return 0 ;
  //
  SendDataBuffer[0] = Length % 0x100 ;
  SendDataBuffer[1] = Length / 0x100 ;
  SendDataBuffer[2] = Temp.DT.LL ;
  SendDataBuffer[3] = Temp.DT.LH ;
  SendDataBuffer[4] = Temp.DT.HL ;
  SendDataBuffer[5] = Temp.DT.HH ;
  //
  if( Mode == 1 )
      WiFi_SetSendPacket(WiFi_UCB_ProgrammingData,&SendDataBuffer[0],6,ReturnStatus, Length ) ;
  else    
      WiFi_SetSendPacket(WiFi_LCB_ProgrammingData,&SendDataBuffer[0],6,ReturnStatus, Length ) ;
  return 1 ;
}



extern const unsigned char Update_CrcTab[16] ;
unsigned char WiFi_UpdateCRC8(unsigned char CHECKDATA, unsigned char Data )
{
    unsigned char crchalf;
    crchalf =(CHECKDATA/16);
    CHECKDATA<<=4;
    CHECKDATA^=Update_CrcTab[crchalf ^( Data /16)];
    crchalf =(CHECKDATA/16);
    CHECKDATA<<=4;
    CHECKDATA^=Update_CrcTab[crchalf ^( Data &0x0f)];
    return CHECKDATA;
}

#endif

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendEraseAllFlash()
{
  WiFi_SetSendPacket(WiFi_EraseAllFlash,0,0,0,0);
  return ;
}

// 20110704 --END---------------------------------------------------------------
// 20111212 --------------------------------------------------------------------
#ifdef  _WiFi_AM_
//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendAM_DataToModule(unsigned long Address, unsigned short Length , unsigned char *pData )
{
  unsigned short i ;
  unsigned short Len = 0 ;
  union {
    struct {
    unsigned long LL:8 ;
    unsigned long LH:8 ;
    unsigned long HL:8 ;
    unsigned long HH:8 ;
    } DT ;
    unsigned long All ;
  } Temp ;    
  
  Temp.All = Address ;
  //
  SendDataBuffer[0] = Length % 0x100 ;
  SendDataBuffer[1] = Length / 0x100 ;
  SendDataBuffer[2] = Temp.DT.LL ;
  SendDataBuffer[3] = Temp.DT.LH ;
  SendDataBuffer[4] = Temp.DT.HL ;
  SendDataBuffer[5] = Temp.DT.HH ;
  Len = 6 ;
  for( i = 0 ; ( i < Length && i < 256 ) ;i++ )
      {
      Len += 1 ;
      SendDataBuffer[i+6] = *(pData+i) ;
      }
  WiFi_SetSendPacket(WiFi_AM_DataToModule,&SendDataBuffer[0],Len,0,0) ;
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SetAM_DataToWeb(void)
{
  WiFi_SetSendPacket(WiFi_AM_DataToWeb,0,0,0,0 ) ;
  return ;
}

#endif
// 20111212 --END---------------------------------------------------------------
//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SendProgrammingData(unsigned char *pData, unsigned short length )
{
  unsigned short i ;
  unsigned short Len = 0 ;
  for( i = 0 ; ( i < length && i < 256 ) ;i++ )
      {
      Len += 1 ;
      SendDataBuffer[i] = *(pData+i) ;
      }
  WiFi_SetSendPacket(WiFi_ProgrammingData,&SendDataBuffer[0],Len,0,0) ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_GetModuleVer( void )
{
  WiFi_SetSendPacket(WiFi_GetModuleSoftwareVersion,0,0,(unsigned char*)&WiFiModuleData.WiFiModuleVer[0],2) ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_GetUartVer( void )
{
  WiFi_SetSendPacket(WiFi_GetUartSoftwareVersion,0,0,(unsigned char*)&WiFiModuleData.WiFiUartVer[0],2) ;
  return ;
}






//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SetSendPacket( unsigned short Cmd, unsigned char *pData , unsigned short Sendlen, unsigned char *rData, unsigned short Readlen )
{
  WiFiSendPacket->Command = Cmd ;
  WiFiSendPacket->Length = Sendlen ;
  WiFiSendPacket->pData = pData ;
  WiFiReceivePacket->Command = WiFiSendPacket->Command ;
  WiFiReceivePacket->Length = Readlen ;
  WiFiReceivePacket->pData = rData ;
  //
  WiFiProcessStatus.b.Tx = 1 ;
  WiFiProcessStatus.b.TxStart = 1 ; 
  WiFiProcessStatus.b.RxTimeoutOccur = 0 ;
  WiFiProcessStatus.b.RxCrcOccur = 0 ;
  //
  return ;
}






/*
#ifdef  _WiFi_UpdateUCBLCB_
extern unsigned char Updata_CheckLCBVersion(unsigned char *LcbPtr ) ;
extern void Update_SaveCheckCode(unsigned char idx,unsigned char Code) ;
#endif 
*/


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_Process()
{
  ReponseStatus Status ;
  //#ifdef  _WiFi_UpdateUCBLCB_
  //unsigned char crcDataCounter = 0 ;
  //#endif
  //----------------------------------------------------------------------------
  if( WiFiProcessStatus.b.WiFiInitalReady == 0 )
      return ;
  //----------------------------------------------------------------------------
  // Transmit Process
  if( WiFiProcessStatus.b.Tx == 1  )
      {
      WiFi_CommandTime = 0 ;
      if( WiFiProcessStatus.b.Rx == 0 )
          {
          if( WiFiSendPacket != 0 )
              {
              if( WiFiProcessStatus.b.TxStart == 1 )
                  {
                  WiFiProcessStatus.b.TxStart = 0 ;
                  WiFi_ConvertSendPacket() ;
                  //
                  WiFiProcessStatus.b.TxEnd = 0 ;
                  WiFi_Tx.Count = 1 ;
                  USART_SendData(UART5,WiFi_Tx.Buffer[0]);
                  //
                  }
              }
          }
      }
  else
      {
      //----------------------------------------------------------------------------
      // Receive Process
      if( WiFiProcessStatus.b.Rx == 1 )
          {
          Status = WiFi_ReponseProcess() ;
          if( Status != WiFiWaitting  )
              {
              switch( Status )
                  {
                  case  MemoryError         : // Memory Pointer Error
                                            WiFiProcessStatus.b.Rx = 0 ;
                                            WiFiProcessStatus.b.Tx = 0 ;
                                            break ;
                  //case  WiFiWaitting        :
                  //                          break ;
                  case  WiFiWaittingTimeout : // Retry   
                                            WiFiProcessStatus.b.RxTimeoutOccur = 1 ;
                                            WiFiProcessStatus.b.Tx = 0 ;
                                            WiFiProcessStatus.b.Rx = 0 ; 
                                            // 20111213
                                            WiFiReceivePacket->Status.All = 0 ;
                                            if( WiFi_WorkMode == _WiFiAMMode_ )
                                                {
                                                WebDataRtyCount += 1 ;
                                                if( WebDataRtyCount > 2 )
                                                    {
                                                    WiFiProcessStatus.b.SendAMtoWeb = 0 ;
                                                    WiFi_WorkMode = _WiFiGetStatus_ ;                                                
                                                    }
                                                }
                                            /*
                                            // 20120202 for Debug No SYNC
                                            USART_SendData(UART5,0x5A);
                                            USART_SendData(UART5,0x5A);
                                            WiFi_WorkMode = _WiFiDelayComm_ ; 
                                            WiFi_DelayTimeCount = _PowerOnOnlineDelayTime_ ;
                                            //
                                            */
                                            break ;                
                  case  WiFiReceiveCrcError :                                
                                            WiFiProcessStatus.b.Tx = 0 ;
                                            WiFiProcessStatus.b.Rx = 0 ;
                                            WiFiProcessStatus.b.RxCrcOccur = 1 ;
                                                                                    
                                            // 20111213
                                            WiFiReceivePacket->Status.All = 0 ;
                                            if( WiFi_WorkMode == _WiFiAMMode_ )
                                                {
                                                WebDataRtyCount += 1 ;
                                                if( WebDataRtyCount > 2 )
                                                    {
                                                    WiFiProcessStatus.b.SendAMtoWeb = 0 ;
                                                    WiFi_WorkMode = _WiFiGetStatus_ ;                                                
                                                    }
                                                }
                                            //
                                            break ;
                  case  WiFiReceiveSuccess  : 
                                            WiFiProcessStatus.b.Error = WiFiReceivePacket->Status.b.Error ;
                                            WiFiProcessStatus.b.Tx = 0 ;
                                            WiFiProcessStatus.b.Rx = 0 ;
                                            //
                                            switch(WiFi_WorkMode)
                                                {
                                                //                                                                          
                                                case  _WiFiFWupdate_          :
                                                                              break ;
                                                case  _WiFiAMMode_            : 
                                                                              WebDataRtyCount = 0 ;
                                                                              DataAddr += DataLength ;                                                  
                                                                              if( *(WebSendDataPacketPtr+WebDataStep) == 0 )
                                                                                  {
                                                                                  WiFiProcessStatus.b.SendAMtoWeb = 0 ;
                                                                                  WiFi_WorkMode = _WiFiGetStatus_ ;  
                                                                                  WebDataStep = 0 ;
                                                                                  }
                                                                              else
                                                                                  WebDataStep += 1 ;
                                                                              break ;
                                                #ifdef  _WiFi_UpdateUCBLCB_                                                                          
                                                case  _WiFiUCBupdateCheck_    :
                                                                              if( WiFiReceivePacket->Command == WiFi_UCB_ProgrammingData )
                                                                                  {
                                                                                  WiFi_UCBInfoData.Member.InfoOK = 0xAA ;
                                                                                  WiFiProcessStatus.b.WiFiUpdateUCB = 0 ;
                                                                                  if( WiFi_CheckUCBInfo((unsigned char*)&User_McuType[0],&WiFi_UCBInfoData.Member.McuType[0],0) == 1 )
                                                                                      {
                                                                                      if( WiFi_CheckUCBInfo((unsigned char*)&User_ModuleName[0],&WiFi_UCBInfoData.Member.ModuleName[0],0) == 1 )
                                                                                          {
                                                                                          if( WiFi_CheckUCBInfo((unsigned char*)&User_ModuleNo[0],&WiFi_UCBInfoData.Member.ModuleNo[0],0) == 1 )
                                                                                              {
                                                                                              if( WiFi_CheckUCBInfo((unsigned char*)&User_Product[0],&WiFi_UCBInfoData.Member.Product[0],0) == 1 )
                                                                                                  {
                                                                                                  if( WiFi_CheckUCBInfo((unsigned char*)&User_Version[0],&WiFi_UCBInfoData.Member.Version[0],1) == 1 )
                                                                                                      {
                                                                                                      WiFi_WorkMode = _WiFiGetStatus_ ;
                                                                                                      WiFiProcessStatus.b.WiFiUpdateUCB = 1 ;
                                                                                                      }
                                                                                                  }
                                                                                              }
                                                                                          }
                                                                                      }
                                                                                  //
                                                                                  if( WiFiProcessStatus.b.WiFiUpdateUCB == 0 )
                                                                                      {
                                                                                      WiFi_WorkMode = _WiFiUCBupdateEnd_ ;
                                                                                      }
                                                                                  }
                                                                              break ;
                                                                              /*
                                                case  _WiFiLCBupdateCheck_    :
                                                                              switch( UCBLCBUpdateStep )
                                                                                  {
                                                                                  case  _Check_LCB_Info_  :
                                                                                                          if( WiFiReceivePacket->Command == WiFi_LCB_ProgrammingData )
                                                                                                                {
                                                                                                                WiFi_LCBInfoData.Member.InfoOK = 0xAA ;
                                                                                                                if( Updata_CheckLCBVersion(&WiFi_LCBInfoData.Member.Ver[0]) == _UserAPIOk_ )
                                                                                                                    {
                                                                                                                    UCBLCBUpdateStep = _Check_LCB_Data1_ ;  
                                                                                                                    WiFi_CommandTime = _WiFiCmdTime_ ;
                                                                                                                    DataAddr = LcbDataAddr[UCBLCBUpdateStep-_Check_LCB_Data1_] ;
                                                                                                                    DataLength = LcbDataLength[UCBLCBUpdateStep-_Check_LCB_Data1_] ;
                                                                                                                    DataCRC8 = 0 ;
                                                                                                                    }
                                                                                                                else
                                                                                                                    {
                                                                                                                    WiFi_WorkMode = _WiFiLCBupdateEnd_ ;
                                                                                                                    }
                                                                                                                }
                                                                                                          break ;
                                                                                  case _Check_LCB_Data1_  :
                                                                                  case _Check_LCB_Data2_  :
                                                                                  case _Check_LCB_Data3_  :
                                                                                  case _Check_LCB_Data4_  :
                                                                                  case _Check_LCB_Data5_  :
                                                                                  case _Check_LCB_Data6_  :
                                                                                  case _Check_LCB_Data7_  :
                                                                                  case _Check_LCB_Data8_  :
                                                                                  case _Check_LCB_Data9_  :
                                                                                  case _Check_LCB_Data10_ :
                                                                                  case _Check_LCB_Data11_ :
                                                                                  case _Check_LCB_Data12_ :
                                                                                  case _Check_LCB_Data13_ :
                                                                                  case _Check_LCB_Data14_ :
                                                                                  case _Check_LCB_Data15_ :  
                                                                                                          //WiFi_CommandTime = 200 ;
                                                                                                          // Calculator Crc
                                                                                                          for( crcDataCounter =0; crcDataCounter < DataReadSize; crcDataCounter++)
                                                                                                              {
                                                                                                              DataCRC8 = WiFi_UpdateCRC8(DataCRC8,DataBuffer[crcDataCounter]) ;
                                                                                                              }
                                                                                                          
                                                                                                          if( DataLength == 0 )
                                                                                                              {
                                                                                                              if(DataCRC8!= WiFi_LCBInfoData.Member.CheckSum[UCBLCBUpdateStep-_Check_LCB_Data1_])
                                                                                                                  {
                                                                                                                  WiFi_WorkMode = _WiFiLCBupdateEnd_ ;
                                                                                                                  }
                                                                                                              else
                                                                                                                  {
                                                                                                                  //  
                                                                                                                  Update_SaveCheckCode((UCBLCBUpdateStep-_Check_LCB_Data1_),DataCRC8) ;
                                                                                                                  //
                                                                                                                  if( UCBLCBUpdateStep == _Check_LCB_Data15_ )
                                                                                                                      {
                                                                                                                      WiFi_WorkMode = _WiFiGetStatus_ ;
                                                                                                                      WiFiProcessStatus.b.WiFiUpdateLCB = 1 ;
                                                                                                                      //WiFi_CommandTime = 0 ;
                                                                                                                      }
                                                                                                                  else  
                                                                                                                      {
                                                                                                                      UCBLCBUpdateStep += 1 ;
                                                                                                                      DataAddr = LcbDataAddr[UCBLCBUpdateStep-_Check_LCB_Data1_] ;
                                                                                                                      DataLength = LcbDataLength[UCBLCBUpdateStep-_Check_LCB_Data1_] ;
                                                                                                                      }
                                                                                                                  }
                                                                                                              DataCount = 0  ;
                                                                                                              DataCRC8 = 0 ;
                                                                                                              }
                                                                                                          
                                                                                                          //else
                                                                                                          //    WiFi_CommandTime = _WiFiCmdTime_ / 2 ;
                                                                                                          
                                                                                                          
                                                                                                          break ;
                                                                                  default                 :
                                                                                                          WiFi_WorkMode = _WiFiGetStatus_ ;
                                                                                                          UCBLCBUpdateStep = 0 ;
                                                                                                          break ;
                                                                                  }
                                                                              break ;
                                                                              */
                                                case  _WiFiLCBupdateEnd_      :
                                                                              WiFi_WorkMode = _WiFiGetStatus_ ;
                                                                              WiFiProcessStatus.b.WiFiUpdateLCB = 0 ;
                                                                              break ;
                                                case  _WiFiUCBupdateEnd_      :
                                                                              WiFi_WorkMode = _WiFiGetStatus_ ;
                                                                              WiFiProcessStatus.b.WiFiUpdateUCB = 0 ;
                                                                              break ;
                                                // 20120117                       
                                                /*
                                                case  _WiFiDelayAPConnect_    :
                                                                              WiFi_WorkMode = _WiFiDelayComm_ ;
                                                                              WiFi_DelayTimeCount = _DelayReConnectTime_ ;
                                                                              break ;
                                                */                                                                          
                                                //                                                                          
                                                #endif                                                                          
                                                default                       :
                                                                              // Set Disconnect AP Add by kunlung 20120111
                                                                              if( WiFiProcessStatus.b.SetDisConnectAP == 1 )
                                                                                  {
                                                                                  if( WiFiReceivePacket->Command != WiFi_DisconnectToAP )
                                                                                      {
                                                                                      WiFi_SetSendPacket(WiFi_DisconnectToAP,0,0,0,0);
                                                                                      break ;
                                                                                      }
                                                                                  else
                                                                                      {
                                                                                      WiFiProcessStatus.b.SetDisConnectAP = 0 ;
                                                                                      }
                                                                                  }
                                                                              // Modify by kunlung 20120111
                                                                              if(  WiFiReceivePacket->Status.b.APConnect == 0 )
                                                                                  {
                                                                                  // Check Set SSID
                                                                                  if( WiFiProcessStatus.b.SetSSID == 1 )
                                                                                      {
                                                                                      if( WiFiReceivePacket->Command != WiFi_SetSSID )
                                                                                          {
                                                                                          WiFi_SendSSID() ; 
                                                                                          break ;
                                                                                          }
                                                                                      else
                                                                                          {
                                                                                          WiFiProcessStatus.b.SetSSID = 0 ;
                                                                                          // Mask by Kunlung 20120111
                                                                                          if( WiFiProcessStatus.b.SetConnectAP == 0 )
                                                                                              WiFiProcessStatus.b.SetConnectAP = 1 ;
                                                                                          }
                                                                                      }
                                                                                  // Check Set Security Type
                                                                                  if( WiFiProcessStatus.b.SetSecurityType == 1 )
                                                                                      {
                                                                                      if( WiFiReceivePacket->Command != WiFi_SetSecurityType )
                                                                                          {
                                                                                          WiFi_SendSecurityType() ;
                                                                                          break ;
                                                                                          }
                                                                                      else
                                                                                          {
                                                                                          WiFiProcessStatus.b.SetSecurityType = 0 ;
                                                                                          // Mask by Kunlung 20120111
                                                                                          if( WiFiProcessStatus.b.SetConnectAP == 0 )
                                                                                              WiFiProcessStatus.b.SetConnectAP = 1 ;
                                                                                          }
                                                                                      }
                                                                                  // Check Set Security Key  
                                                                                  if( WiFiProcessStatus.b.SetSecurityKey == 1 )
                                                                                      {
                                                                                      if( WiFiReceivePacket->Command != WiFi_SetSecurityKey )
                                                                                          {
                                                                                          WiFi_SendSecurityKey() ;
                                                                                          break ;
                                                                                          }
                                                                                      else
                                                                                          {
                                                                                          WiFiProcessStatus.b.SetSecurityKey = 0 ;
                                                                                          WiFiProcessStatus.b.GetSSID = 1 ;
                                                                                          WiFiProcessStatus.b.GetSecurityType = 1 ;
                                                                                          WiFiProcessStatus.b.GetSecurityKey = 1 ;
                                                                                          if( WiFiProcessStatus.b.SetConnectAP == 0 )
                                                                                              WiFiProcessStatus.b.SetConnectAP = 1 ;
                                                                                          }
                                                                                      }
                                                                                  }
                                                                              //------------------------------------------------------------------------------
                                                                              // Check Get SSID
                                                                              if( WiFiProcessStatus.b.GetSSID == 1 )
                                                                                  {
                                                                                  if( WiFiReceivePacket->Command != WiFi_GetSSID )
                                                                                      {
                                                                                      WiFi_ReadSSID() ; 
                                                                                      break ;
                                                                                      }
                                                                                  else
                                                                                      {
                                                                                      WiFiProcessStatus.b.GetSSID = 0 ;
                                                                                      // Mask by Kunlung 20120111
                                                                                      //if( WiFiProcessStatus.b.SetConnectAP == 0 )
                                                                                      //    WiFiProcessStatus.b.SetConnectAP = 1 ;
                                                                                      }
                                                                                  }
                                                                              // Check Get Security Type
                                                                              if( WiFiProcessStatus.b.GetSecurityType == 1 )
                                                                                  {
                                                                                  if( WiFiReceivePacket->Command != WiFi_GetSecurityType )
                                                                                      {
                                                                                      WiFi_ReadSecurityType() ;
                                                                                      break ;
                                                                                      }
                                                                                  else
                                                                                      {
                                                                                      WiFiProcessStatus.b.GetSecurityType = 0 ;
                                                                                      // Mask by Kunlung 20120111
                                                                                      //if( WiFiProcessStatus.b.SetConnectAP == 0 )
                                                                                      //    WiFiProcessStatus.b.SetConnectAP = 1 ;
                                                                                      }
                                                                                  }
                                                                              // Check Get Security Key  
                                                                              if( WiFiProcessStatus.b.GetSecurityKey == 1 )
                                                                                  {
                                                                                  if( WiFiReceivePacket->Command != WiFi_GetSecurityKey )
                                                                                      {
                                                                                      WiFi_ReadSecurityKey() ;
                                                                                      break ;
                                                                                      }
                                                                                  else
                                                                                      {
                                                                                      WiFiProcessStatus.b.GetSecurityKey = 0 ;
                                                                                      //if( WiFiProcessStatus.b.SetConnectAP == 0 )
                                                                                      //    WiFiProcessStatus.b.SetConnectAP = 1 ;
                                                                                      }
                                                                                  }
                                                                              //------------------------------------------------------------------------------
                                                                              // Check Connect to AP  
                                                                              if( WiFiProcessStatus.b.SetConnectAP == 1 )
                                                                                  {
                                                                                  WiFi_SetSendPacket(WiFi_DisconnectToAP,0,0,0,0);
                                                                                  WiFi_DelayTimeCount = _DelayReConnectTime_ ; // sec.
                                                                                  WiFi_WorkMode = _WiFiDelayAPConnect_ ;
                                                                                  WiFiProcessStatus.b.SetConnectAP = 0 ; 
                                                                                  break ;
                                                                                  /*
                                                                                  if( WiFiReceivePacket->Status.b.APConnect == 1 )
                                                                                      {
                                                                                      WiFi_SetSendPacket(WiFi_DisconnectToAP,0,0,0,0);
                                                                                      break ;
                                                                                      }
                                                                                  else
                                                                                      {
                                                                                      if( WiFiReceivePacket->Command != WiFi_ConnectToAP )
                                                                                          {
                                                                                           
                                                                                         
                                                                                          WiFi_SetSendPacket(WiFi_ConnectToAP,0,0,0,0);
                                                                                          // 20120111
                                                                                          WiFiModuleData.IP[0] = 0 ;
                                                                                          WiFiModuleData.IP[1] = 0 ;
                                                                                          WiFiModuleData.IP[2] = 0 ;
                                                                                          WiFiModuleData.IP[3] = 0 ;
                                                                                          WiFiModuleData.IP[4] = 0 ;
                                                                                          WiFiModuleData.IP[5] = 0 ;
                                                                                          
                                                                                          //
                                                                                          break ;
                                                                                          }
                                                                                      else
                                                                                          {
                                                                                          WiFiProcessStatus.b.SetConnectAP = 0 ;
                                                                                          }
                                                                                      }
                                                                                  */
                                                                                  }
                                                                              //
                                                                              if( WiFiReceivePacket->Status.b.APConnect == 1 )
                                                                                  {
                                                                                  // Check Set TCPIP Address  
                                                                                  if( WiFiProcessStatus.b.SetTCPIPAddress == 1 )
                                                                                      {
                                                                                      if( WiFiReceivePacket->Command != WiFi_SetTcpIpAddress )
                                                                                          {
                                                                                          WiFi_SendTCPIPAddress() ;
                                                                                          break ;
                                                                                          }
                                                                                      else
                                                                                          {
                                                                                          WiFiProcessStatus.b.SetTCPIPAddress = 0 ;
                                                                                          if( WiFiProcessStatus.b.SetConnectTCPIP == 0 )
                                                                                              WiFiProcessStatus.b.SetConnectTCPIP = 1 ;
                                                                                          }
                                                                                      }
                                                                                  // Check Set TCPIP port number    
                                                                                  if( WiFiProcessStatus.b.SetTCPIPPort == 1 )
                                                                                      {
                                                                                      if( WiFiReceivePacket->Command != WiFi_SetTcpIpPort )
                                                                                          {
                                                                                          WiFi_SendTCPIPPort() ;
                                                                                          break ;
                                                                                          }
                                                                                      else
                                                                                          {
                                                                                          WiFiProcessStatus.b.SetTCPIPPort = 0 ;
                                                                                          if( WiFiProcessStatus.b.SetTCPIPPort == 0 )
                                                                                              WiFiProcessStatus.b.SetTCPIPPort = 1 ;
                                                                                          }
                                                                                      }
                                                                                  // Check Connect to TCPIP  
                                                                                  if( WiFiProcessStatus.b.SetConnectTCPIP == 1 )
                                                                                      {
                                                                                      if( WiFiReceivePacket->Status.b.TcpIpConnect == 1 )
                                                                                          {
                                                                                          WiFi_SetSendPacket(WiFi_TcpIpDisconnect,0,0,0,0);
                                                                                          }
                                                                                      else 
                                                                                          {
                                                                                          if( WiFiReceivePacket->Command != WiFi_TcpIpConnect )
                                                                                              {
                                                                                              WiFi_SetSendPacket(WiFi_TcpIpConnect,0,0,0,0);
                                                                                              break ;
                                                                                              }
                                                                                          else
                                                                                              {
                                                                                              WiFiProcessStatus.b.SetConnectTCPIP = 0 ;
                                                                                              }
                                                                                          }
                                                                                      }
                                                                                  //
                                                                                  }
                                                                              // 20120130
                                                                              if( WiFiProcessStatus.b.WiFiPowerOnOnlineDelay == 1 )
                                                                                  {
                                                                                  WiFi_WorkMode = _WiFiDelayComm_ ; // 20120117
                                                                                  WiFi_DelayTimeCount = _PowerOnOnlineDelayTime_ ;
                                                                                  WiFiProcessStatus.b.WiFiPowerOnOnlineDelay = 0 ;
                                                                                  }
                                                                              //                                                                          
                                                                              break ;
                                                }
                                            // 20120220
                                            if( WiFiReceivePacket->Command == WiFi_GetMACAddress )
                                                {
                                                UploadToWiFiStatus.b.WiFiMacReady = 1 ;
                                                if( UploadToWiFiStatus.b.WaitWiFiMacReady == 1 )
                                                    {
                                                    WiFi_WorkMode = _WiFiAMMode_ ;
                                                    WiFiProcessStatus.b.SendAMtoWeb = 1 ;
                                                    }
                                                }
                                            //
                                            break ;
                  default                   :
                                            break ;
                  }
              //WiFiProcessStatus.b.Rx = 0 ;
              //WiFiProcessStatus.b.Tx = 0 ;
              }
          WiFi_CommandTime = 0 ;
          }
      else
          {
          WiFi_CommandTime += 1 ;
          if( WiFi_CommandTime > _WiFiCmdTime_ ) 
              {  
              WiFi_CommandTime = 0 ;
              // 20111213
              if( WiFi_WorkMode != _WiFiAMMode_ )
                  {
                  WiFiProcessStatus.b.SendAMtoWebStart = 1 ;
                  WiFiProcessStatus.b.SendDataToWiFiAM = 1 ;
                  }
              //
              switch( WiFi_WorkMode )
                  {
                  case  _WiFiManualMode_      :
                                              break ;
                  case  _WiFiFWupdate_        :
                                              break ;
                  // 20111213
                  case  _WiFiAMMode_          : 
                                              //
                                              WiFi_LCBInfoData.Member.InfoOK = 0 ;
                                              if( UploadToWiFiStatus.b.WiFiMacReady != 1 )
                                                  {
                                                  UploadToWiFiStatus.b.WaitWiFiMacReady = 1 ;
                                                  WiFiProcessStatus.b.SendAMtoWeb = 0 ;
                                                  WiFi_WorkMode = _WiFiGetStatus_ ;  
                                                  break ;
                                                  }
                                             
                                              UploadToWiFiStatus.b.WaitWiFiMacReady = 0 ;
                                              
                                              if( WiFiProcessStatus.b.SendAMtoWeb == 1 )
                                                  {
                                                  if( WiFiProcessStatus.b.SendAMtoWebStart == 1 )
                                                      {
                                                      DataAddr = 0 ;
                                                      WebDataStep = 0 ; // 10->0
                                                      WiFiProcessStatus.b.SendAMtoWebStart = 0 ;
                                                      WebDataRtyCount = 0 ;
                                                      WiFi_CommandTime = _WiFiCmdTime_ ;
                                                      }
                                                  // 20120214
                                                  if( *(WebSendDataPacketPtr+WebDataStep) >= 10 )
                                                      {
                                                      __WiFimemset(&DataBuffer[0],0,sizeof(DataBuffer)) ;
                                                      WebFile_CreateStream(*(WebSendDataPacketPtr+WebDataStep),&DataBuffer[0]) ;
                                                      DataLength = __WiFistrlen((char const*)DataBuffer) ;//==>计算数据长度
                                                      WiFi_SendAM_DataToModule(DataAddr,DataLength,&DataBuffer[0]) ;
                                                      }
                                                  else
                                                      {
                                                      if( *(WebSendDataPacketPtr+WebDataStep) == 0 )
                                                          {
                                                          WiFi_SetAM_DataToWeb() ;
                                                          }
                                                      }
                                                  /* Modify by Kunlung 20120214
                                                  if( WebDataStep >= 10 && WebDataStep <= 31 ) // Modify by Kunlung 20120214 30-->31
                                                      {
                                                      __WiFimemset(&DataBuffer[0],0,sizeof(DataBuffer)) ;
                                                      WebFile_CreateStream(WebDataStep,&DataBuffer[0]) ;
                                                      DataLength = __WiFistrlen((char const*)DataBuffer) ;//==>计算数据长度
                                                      WiFi_SendAM_DataToModule(DataAddr,DataLength,&DataBuffer[0]) ;
                                                      }
                                                  else
                                                      {
                                                      if( WebDataStep == 32 )// Modify by Kunlung 20120214 31-->32
                                                          {
                                                          WiFi_SetAM_DataToWeb() ;
                                                          }
                                                      else
                                                          {
                                                          WiFiProcessStatus.b.SendAMtoWeb = 0 ;
                                                          WiFi_WorkMode = _WiFiGetStatus_ ;  
                                                          }
                                                      }
                                                  */
                                                  //
                                                  }
                                              else
                                                  WiFi_WorkMode = _WiFiGetStatus_ ;  
                                              //
                                              break ;
                  #ifdef  _WiFi_UpdateUCBLCB_                                              
                  case  _WiFiLCBupdateCheck_  :
                                              switch( UCBLCBUpdateStep )
                                                  {
                                                  case _Check_LCB_Info_   :
                                                                          WiFi_UCBLCB_ReadProgrammingData(_LCB_,0x4000,48,&WiFi_LCBInfoData.Data[0]) ;
                                                                          break ;
                                                  case _Check_LCB_Data1_  :
                                                  case _Check_LCB_Data2_  :
                                                  case _Check_LCB_Data3_  :
                                                  case _Check_LCB_Data4_  :
                                                  case _Check_LCB_Data5_  :
                                                  case _Check_LCB_Data6_  :
                                                  case _Check_LCB_Data7_  :
                                                  case _Check_LCB_Data8_  :
                                                  case _Check_LCB_Data9_  :
                                                  case _Check_LCB_Data10_ :
                                                  case _Check_LCB_Data11_ :
                                                  case _Check_LCB_Data12_ :
                                                  case _Check_LCB_Data13_ :
                                                  case _Check_LCB_Data14_ :
                                                  case _Check_LCB_Data15_ :
                                                                          if( DataLength > 100 )
                                                                              {
                                                                              WiFi_UCBLCB_ReadProgrammingData(_LCB_,DataAddr,100,&DataBuffer[0]) ;
                                                                              DataLength -= 100 ;
                                                                              DataCount += 100 ;
                                                                              DataReadSize = 100 ;
                                                                              }
                                                                          else
                                                                              {
                                                                              WiFi_UCBLCB_ReadProgrammingData(_LCB_,DataAddr,DataLength,&DataBuffer[0]) ;
                                                                              DataCount += DataLength ;
                                                                              DataReadSize = DataLength ;
                                                                              DataLength = 0 ;
                                                                              }
                                                                          DataAddr += DataReadSize ;
                                                                          break ;
                                                  default                 :
                                                                          WiFi_WorkMode = _WiFiGetStatus_ ;
                                                                          break ;
                                                  }
                                              break ;
                  case  _WiFiLCBupdateEnd_    :
                                              WiFi_ClearLCBProgrammingStatus();
                                              break ;
                  case  _WiFiUCBupdateCheck_  :
                                              WiFi_UCBLCB_ReadProgrammingData(_UCB_,0x0003FE34,100,&WiFi_UCBInfoData.Data[0]) ;
                                              break ;
                  case  _WiFiUCBupdateEnd_    :
                                              WiFi_ClearUCBProgrammingStatus();
                                              break ;                                                              
                  #endif                                                                  
                  case  _WiFiGetStatus_       :
                                              #ifdef  _WiFi_UpdateUCBLCB_
                                              if( WiFiReceivePacket->Status.b.RemoteLCBUpdate == 1 )
                                                  {
                                                  if( WiFi_LCBInfoData.Member.InfoOK != 0xAA )
                                                      {
                                                      //WiFi_UCBLCB_ReadProgrammingData(_LCB_,0x4000,48,&WiFi_LCBInfoData.Data[0]) ;
                                                      WiFi_WorkMode = _WiFiLCBupdateCheck_ ;
                                                      UCBLCBUpdateStep = _Check_LCB_Info_ ;
                                                      WiFi_CommandTime = _WiFiCmdTime_ ;
                                                      }
                                                  }
                                              else
                                                  {
                                                  WiFi_LCBInfoData.Member.InfoOK = 0 ;
                                                  //
                                                  if( WiFiReceivePacket->Status.b.RemoteUCBUpdate == 1 )
                                                      {
                                                      if( WiFi_UCBInfoData.Member.InfoOK != 0xAA )
                                                          {
                                                          WiFi_WorkMode = _WiFiUCBupdateCheck_ ;
                                                          WiFi_CommandTime = _WiFiCmdTime_ ;
                                                          }
                                                      }
                                                  else
                                                      {
                                                      WiFi_UCBInfoData.Member.InfoOK = 0 ;
                                                      /* 20120131 Modify
                                                      if( WiFiReceivePacket->Status.b.APConnect == 1 )
                                                          {
                                                          WiFi_ReadIPAddress() ;
                                                          WiFi_WorkMode = 0xFF ;
                                                          }
                                                      else
                                                          {
                                                          WiFi_ReadMACAddress() ; 
                                                          }
                                                      */
                                                      if( WiFiProcessStatus.b.WiFiHWReady == 0 ) 
                                                          WiFi_GetModuleVer() ;
                                                      else
                                                          {
                                                          switch(WiFi_NormalCheckStep)
                                                              {
                                                              case  1   :
                                                                        WiFi_ReadIPAddress() ;
                                                                        WiFi_NormalCheckStep = 0 ;
                                                                        break ;
                                                              case  0   :
                                                              default   :  
                                                                        WiFi_ReadMACAddress() ; 
                                                                        WiFi_NormalCheckStep = 1 ;
                                                                        break ;
                                                              }
                                                          }
                                                      }
                                                  }
                                              //
                                              #else   
                                              if( WiFiReceivePacket->Status.b.APConnect == 1 )
                                                  {
                                                  WiFi_ReadIPAddress() ;
                                                  WiFi_WorkMode = 0xFF ;
                                                  }
                                              else
                                                  {
                                                  WiFi_ReadMACAddress() ; 
                                                  }
                                              #endif
                                              //WiFiProcessStatus.b.Tx = 1 ;
                                              //WiFiProcessStatus.b.TxStart = 1 ; 
                                              break ;
/*                                              
                  case  _WiFiGetDemoData_     :
                                              if( WiFiReceivePacket->Status.b.APConnect == 1 )
                                                  {
                                                  TMDataToWiFi.Member.Incline += 1 ;
                                                  TMDataToWiFi.Member.Speed += 1 ;
                                                  TMDataToWiFi.Member.Distance += 1 ;
                                                  TMDataToWiFi.Member.Time += 1 ;
                                                  TMDataToWiFi.Member.Calories += 1 ;
                                                  TMDataToWiFi.Member.HR += 1 ;
                                                  //
                                                  WiFi_TestCommand() ;
                                                  WiFiProcessStatus.b.Tx = 1 ;
                                                  WiFiProcessStatus.b.TxStart = 1 ; 
                                                  }
                                              else
                                                  {
                                                  WiFi_WorkMode = _WiFiGetStatus_ ;
                                                  WiFi_ReadMACAddress() ; 
                                                  WiFiProcessStatus.b.Tx = 1 ;
                                                  WiFiProcessStatus.b.TxStart = 1 ; 
                                                  }
                                              //
                                              break ;
*/                 
                  case  _WiFiDelayAPConnect_  :
                                              if( WiFi_DelayTimeCount != 0 )
                                                  WiFi_DelayTimeCount -= 1 ;
                                              else
                                                  {
                                                  WiFi_SetSendPacket(WiFi_ConnectToAP,0,0,0,0);
                                                  // 20120111
                                                  WiFiModuleData.IP[0] = 0 ;
                                                  WiFiModuleData.IP[1] = 0 ;
                                                  WiFiModuleData.IP[2] = 0 ;
                                                  WiFiModuleData.IP[3] = 0 ;
                                                  WiFiModuleData.IP[4] = 0 ;
                                                  WiFiModuleData.IP[5] = 0 ;
                                                  //
                                                  WiFiModuleData.MAC[0] = 0 ;
                                                  WiFiModuleData.MAC[1] = 0 ;
                                                  WiFiModuleData.MAC[2] = 0 ;
                                                  WiFiModuleData.MAC[3] = 0 ;
                                                  WiFiModuleData.MAC[4] = 0 ;
                                                  WiFiModuleData.MAC[5] = 0 ;
                                                  //
                                                  WiFi_WorkMode = _WiFiDelayComm_ ;
                                                  WiFi_DelayTimeCount = _DelayReConnectTime_ ;
                                                  //
                                                  }
                                              break ;
                  case  _WiFiDelayComm_       :   
                                              if( WiFi_DelayTimeCount != 0 )
                                                  WiFi_DelayTimeCount -= 1 ;
                                              else
                                                  WiFi_WorkMode = _WiFiGetStatus_ ;
                                              break ;
                  default                     :
                                              //if( WiFiReceivePacket->Status.b.APConnect == 1 )
                                              //    WiFi_WorkMode = _WiFiGetDemoData_ ;
                                              //else
                                                  WiFi_WorkMode = _WiFiGetStatus_ ;
                                              break ;                                                
                  }

              }
          }
      }
  //
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ConvertSendPacket()
{
  union {
    struct {
      unsigned char Low:8 ;
      unsigned char High:8 ;
    } byte ;
    unsigned short word ;
  } Data,CrcData ;
  unsigned short i ;
  unsigned short DataIndex ;
  //
  DataIndex = 1 ;
  #ifdef  _CRC16_NotInclude_Stuffing_
  // Calculator crc16
  Data.word = WiFiSendPacket->Command ;
  WiFi_Tx.Buffer[0] = Data.byte.Low ;
  WiFi_Tx.Buffer[1] = Data.byte.High ;
  //
  if( WiFiSendPacket->pData != 0 )
      Data.word = WiFiSendPacket->Length ;
  else
      Data.word = 0 ;
  //
  WiFi_Tx.Buffer[2] = Data.byte.Low ;
  WiFi_Tx.Buffer[3] = Data.byte.High ;
  
  for( i = 0 ; i < Data.word ; i++ )
      {
      if( WiFiSendPacket->pData != 0 )
          WiFi_Tx.Buffer[i+4] = *(WiFiSendPacket->pData+i) ;
      }
  CrcData.word = WiFi_CRC_CITT( &WiFi_Tx.Buffer[0], (Data.word+4)) ;
  #endif
  // Start Byte
  WiFi_Tx.Buffer[WiFi_StartPacket] = WiFi_START ;
  
  // Command
  Data.word = WiFiSendPacket->Command ;
  DataIndex += WiFi_ConvertSpecialData(&WiFi_Tx.Buffer[DataIndex],Data.byte.Low) ;
  DataIndex += WiFi_ConvertSpecialData(&WiFi_Tx.Buffer[DataIndex],Data.byte.High) ;
  
  // Data 
  if( WiFiSendPacket->pData != 0 )
      Data.word = WiFiSendPacket->Length ;
  else
      Data.word = 0 ;
  //Data.word = WiFiSendPacket->Length ;
  DataIndex += WiFi_ConvertSpecialData(&WiFi_Tx.Buffer[DataIndex],Data.byte.Low) ;
  DataIndex += WiFi_ConvertSpecialData(&WiFi_Tx.Buffer[DataIndex],Data.byte.High) ;

  for( i = 0 ; i < Data.word ; i++ )
      {
      if( WiFiSendPacket->pData != 0 )
          DataIndex += WiFi_ConvertSpecialData(&WiFi_Tx.Buffer[DataIndex],*(WiFiSendPacket->pData+i)) ;
      }
  
  // CRC16 : The data not include start byte
  #ifndef  _CRC16_NotInclude_Stuffing_
  Data.word = WiFi_CRC_CITT( &WiFi_Tx.Buffer[WiFi_CommandPacket], (DataIndex-1)) ;
  #endif
  DataIndex += WiFi_ConvertSpecialData(&WiFi_Tx.Buffer[DataIndex],CrcData.byte.Low) ;
  DataIndex += WiFi_ConvertSpecialData(&WiFi_Tx.Buffer[DataIndex],CrcData.byte.High) ;

  // End
  WiFi_Tx.Buffer[DataIndex] = WiFi_END ;
  DataIndex += 1 ;
  //
  WiFi_Tx.TotalLength = DataIndex ;
  WiFi_Tx.Count = 0 ;
  WiFi_Rx.Count = 0 ;
  WiFi_RxTimeout = 0 ;
  //
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
ReponseStatus WiFi_ReponseProcess()
{
  ReponseStatus Result = MemoryError ;
  //
  if( WiFiReceivePacket !=  0 )
      {
      if( WiFiProcessStatus.b.RxEnd == 1 )
          {
          WiFi_RxTimeout = 0 ;
          // Analysis Receive Data 
          Result = WiFi_ReveiveAnalysis() ;
          //
          }
      else
          {
          WiFi_RxTimeout += 1 ;
          Result = WiFiWaitting ;
          if( WiFi_RxTimeout >= 5000 )
              {
              WiFi_RxTimeout = 0 ;
              Result = WiFiWaittingTimeout ;
              // Set WiFi hardware error
              WiFiProcessStatus.b.WiFiHWReady = 0 ;
              }
          }
      } 
  else
      {
      WiFi_RxTimeout = 0 ;
      }
  //
  return Result ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
ReponseStatus WiFi_ReveiveAnalysis() 
{
  ReponseStatus Result = WiFiReceiveCrcError ;
  unsigned short i ;
  //WiFiDataMapping *WiFiReceiverData ;
  //
  union {
    struct {
      unsigned char Low:8 ;
      unsigned char High:8 ;
    } byte ;
    unsigned short Word ;
  } TempData ;
  #ifdef  _CRC16_NotInclude_Stuffing_
  // Shift Combine SCF+Data  
  WiFi_RemoveSCF() ; 
  #endif
  // Check CRC16
  TempData.Word = WiFi_CRC_CITT( &WiFi_Rx.Buffer[WiFi_CommandPacket], (WiFi_Rx.TotalLength-4)) ;
  #ifndef  _CRC16_NotInclude_Stuffing_
  // Shift Combine SCF+Data  
  WiFi_RemoveSCF() ; 
  #endif
  if( (TempData.byte.Low == WiFi_Rx.Buffer[WiFi_Rx.TotalLength-3]) && (TempData.byte.High == WiFi_Rx.Buffer[WiFi_Rx.TotalLength-2]) )
      {
      // Mapping Receive Data Memory Address
      WiFiReceiverData = (WiFiProtocolDataMapping *)&WiFi_Rx.Buffer[WiFi_CommandPacket] ;
      //
      if( WiFiReceivePacket != 0 )
          {
          //--------------------------------------------------------------------  
          WiFiReceivePacket->Command = WiFiReceiverData->Command ;
          WiFiReceivePacket->Length = WiFiReceiverData->Length ;
          WiFiReceivePacket->Status.All = WiFiReceiverData->Status.All ;
          WiFiReceivePacket->ErrorCode = WiFiReceiverData->ErrorCode ;
          // Set WiFi hardware OK
          WiFiProcessStatus.b.WiFiHWReady = 1 ;
          //--------------------------------------------------------------------
          switch(WiFiReceiverData->Command)
              {
              case  WiFi_EntryProgrammingMode           : // 6	1	  Entry Programming Mode
                                                          //        DataT: JHTPgr
                                                          //        DataR: 1 = Ready
              case  WiFi_GetProgrammingStatus           : // 0	1	  Get Programming Status
                                                          //        Bit0: 1 = Programming Mode
                                                          //        Bit1: 7 = Busy
              case  WiFi_GetModuleSoftwareVersion       :	// 0	2	  Get WIFI Module WIFI Software Version
              case  WiFi_GetUartSoftwareVersion         :	// 0	2	  Get WIFI Module UART Software Version
              case  WiFi_GetMACAddress                  : // 0	6	  Get MAC Address
              case  WiFi_GetIPAddress                   : // 0	6	  Get WIFI IP
                                                          //        For IPV4: 0x00, 0x00, IP
              case  WiFi_SetSecurityType                : // 1	1	  Set WIFI Security Type (DataT = DataR)
                                                                      //        0: None
                                                                      //        1: WEP_40Bits
                                                                      //        2: WEP_104Bits
                                                                      //        3: WPA_KEY
                                                                      //        4: WPA_PHRASE
                                                                      //        5: WPA2_KEY
                                                                      //        6: WPA2_PHRASE
                                                                      //        7: WPA_AUTO_KEY
                                                                      //        8: WPA_AUTO_PHRASE
              case  WiFi_SetSSID                        : // N	N	  Set WIFI SSID
              case  WiFi_SetSecurityKey                 : // N	N	  Set WIFI Security Key (DataT = DataR)  
              case  WiFi_GetTcpIpBufferSize             : // 0  4   Get TCP/IP Buffer Size ( TX(2)+RX(2) )
              case  WiFi_GetTcpIpAddress                :	// 0	N	  Get TCP/IP address
              case  WiFi_GetTcpIpPort                   :	// 0	2	  Get TCP/IP port
              case  WiFi_GetTcpIpData                   :	// 0	N	  Get TCP/IP data (Max 256Bytes)     
              case  WiFi_GetModuleStatus                : // 0	2	  Get WIFI Module Status
              #ifdef  _WiFi_UpdateUCBLCB_
              case  WiFi_UCB_ProgrammingData            :
              case  WiFi_LCB_ProgrammingData            :  
              case  WiFi_UCBLCB_GetProgrammingStatus    :
              #endif
              // 20120111
              case WiFi_GetSSID                         :
              case WiFi_GetSecurityType                 :
              case WiFi_GetSecurityKey                  :  
              //
                                                        if( WiFiReceivePacket->pData != 0 )
                                                            {
                                                            if( WiFiReceiverData->Length > _STATUS_ERROR_DATA_SIZE_ )
                                                                {
                                                                for( i = 0 ; i < (WiFiReceiverData->Length-_STATUS_ERROR_DATA_SIZE_) ; i++ )
                                                                    WiFiReceivePacket->pData[i] = WiFiReceiverData->Buffer[i] ;
                                                                }
                                                            }
                                                        break ;
              case  WiFi_SetTcpIpAddress                : // N	0	  (Max 128Bytes with 0x00 terminator)
              case  WiFi_SetTcpIpPort                   :	// 2	0	  Set TCP/IP port
              case  WiFi_TcpIpConnect                   :	// 0	0	  TCP/IP connect
              case  WiFi_TcpIpDisconnect                :	// 0	0	  TCP/IP disconnect
              case  WiFi_SetTcpIpData                   :	// N	0	  Set TCP/IP data (Max 256Bytes)
 
              case  WiFi_ConnectToAP                    : // 0	0	  Connect to WIFI AP
              case  WiFi_DisconnectToAP                 : // 0	0	  Disconnect to WIFI AP        
              case  WiFi_LeaveProgrammingMode           : // 0	0	  Leave Programming Mode
              case  WiFi_SetProgrammingAddress          : // 4	0	  Set Programming Address  
              case  WiFi_ProgrammingData                : // N	0	  Programming Data
                                                          //        N size must be less than 128.                 
              case  WiFi_WorkoutProgramLevel            : // 4	0	  Workout Program & Level
              case  WiFi_WorkoutName                    : // N	0	  Workout Name
              case  WiFi_UserInfo                       : // 4	0	  User Info
              case  WiFi_WorkoutSpeed                   : // 2	0	  Workout Speed
              case  WiFi_WorkoutDistancePerFloor        : // 2	0	  Workout Distance/ Workout floor
              case  WiFi_WorkoutIncline                 : // 2	0	  Workout Incline
              case  WiFi_WorkoutMotorRPM                : // 2	0 	Workout Motor RPM/RPM/SPM
              case  WiFi_WorkoutTime                    : // 4	0	  Workout Time
              case  WiFi_WorkoutCalories                : // 2	0	  Workout Calories
              case  WiFi_WorkoutMETs                    : // 2	0	  Workout METs
              case  WiFi_WorkoutPower                   : // 2	0	  Workout Power
              case  WiFi_WorkoutHR                      : // 1	0	  Workout HR
              case  WiFi_Info                           : // 16 + (Version Count * 4)	0	Info
              case  WiFi_Info1                          : // 25	0	  Serial Number
              case  WiFi_Version                        : // 8		  Version
              case  WiFi_RecordTotalHorizontalDistance  : // 4	0	  Record Total Horizontal Distance
              case  WiFi_RecordTotalVerticalDistance    : // 4	0	  Record Total Vertical Distance
              case  WiFi_RecordTotalUtilizationTime     : // 4	0	  Record Total Utilization Time
              case  WiFi_RecordErrorCode                : // 2	0	  Record Error Code
              case  WiFi_RecordServiceCode              : // 2	0	  Record Service Code
              #ifdef  _WiFi_UpdateUCBLCB_
              case  WiFi_UCBLCB_LeaveProgrammingMode    :                                                        
              #endif     
              // 20120111
              case  WiFi_EraseAllFlash                  :
              //
                                                        break ;
              #ifdef  _WiFi_AM_
              case  WiFi_AM_DataToWeb                   :
                                                        WiFiProcessStatus.b.SendAMtoWeb = 1 ;
                                                        break ;
              case  WiFi_AM_DataToModule                :
                                                        if( WiFiReceiverData->Buffer[0] == 0x55 )
                                                            {
                                                            WiFiProcessStatus.b.SendDataToWiFiAM = 1 ;
                                                            }
                                                        break ;
              #endif
              default                                   :
                                                        // Command not define
                                                        break ;
              }
          Result = WiFiReceiveSuccess ;
          }
      else
          Result = MemoryError ;
      }
  return Result ;
}

//##############################################################################
// Function : Convert 0xf1,0xf2,0xf0 data to 0xf0+0x00,0xf0+0x01,0xf0+0x02
// Input : 
//        pData: data save memory address
//        InData: Convert Data
// Return : data count
//##############################################################################
unsigned char WiFi_ConvertSpecialData( unsigned char *pData, unsigned char InData )
{
  unsigned char Result ;
  Result = 1 ;
  if( InData == WiFi_START || InData == WiFi_END || InData == WiFi_SCF )
      { 
      *pData = WiFi_SCF ;
      *(pData+1) = InData & 0x0f ;
      Result = 2 ;
      }
  else
      {
      *pData = InData ;
      }
  return Result ;
}


//##############################################################################
// Function : Convert 0xf0+0x00,0xf0+0x01,0xf0+0x02 to 0xf0,0xf1,0xf2
// Input : none
// Return : none
//##############################################################################
void WiFi_RemoveSCF()
{
  unsigned short i ;
  unsigned short j ;
  //
  i = 1 ;
  while( i < WiFi_Rx.TotalLength && WiFi_Rx.Buffer[i] != WiFi_END )
      {
      if( WiFi_Rx.Buffer[i] == WiFi_SCF )
          {
          j = i+1 ;
          WiFi_Rx.Buffer[i] += WiFi_Rx.Buffer[j] ;
          while( j < WiFi_Rx.TotalLength && WiFi_Rx.Buffer[j] != WiFi_END )
              {
              WiFi_Rx.Buffer[j] = WiFi_Rx.Buffer[j+1] ;
              j += 1 ;
              }
          WiFi_Rx.Buffer[j] = 0 ;
          WiFi_Rx.TotalLength -= 1 ;
          }
      i += 1 ;
      }
  //
  return ;
}




//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_UartTxRx_Information()
{
  unsigned char TempData ;
  //if( (UART5->SR & 0x00000040 ) != 0 )
  if(USART_GetITStatus(UART5, USART_IT_TC) != RESET)
      {
      /* Clear the UART5 transmit interrupt */
      USART_ClearITPendingBit(UART5, USART_IT_TC);
      //UART5->SR &= 0xFFFFFFBF ;
      //
      if( WiFiProcessStatus.b.Tx == 1 )
          {
          if( WiFiProcessStatus.b.TxEnd == 1 )
              {
              WiFiProcessStatus.b.RxEnd = 0 ;
              WiFiProcessStatus.b.Rx = 1 ;
              WiFiProcessStatus.b.Tx = 0 ;
              //WiFiProcessStatus.b.RxStartByte = 0 ;
              WiFi_Rx.Count = 0 ;
              TxCounter += 1;
              }
          else
              {
              if( WiFi_Tx.Buffer[WiFi_Tx.Count] == WiFi_END )
                  {
                  WiFiProcessStatus.b.TxEnd = 1 ;
                  }
              USART_SendData(UART5,WiFi_Tx.Buffer[WiFi_Tx.Count]);
              //UART5->DR = (Data & (uint16_t)0x01FF);
              WiFi_Tx.Count++;
              }
          }
      }
  //
  //if( (UART5->SR & 0x00000020 ) != 0 )
  if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) 
      {//==>判断是否接收中断
      //TempData = (uint16_t)(UART5->DR & (uint16_t)0x01FF);
      TempData = USART_ReceiveData(UART5);
      /* Clear the USART5 Receive interrupt */
      //UART5->SR &= 0xFFFFFFDF ;
      USART_ClearITPendingBit(UART5, USART_IT_RXNE);
      if( WiFiProcessStatus.b.Rx == 1 )
          {
          if( WiFiProcessStatus.b.RxEnd == 1 )
              return ;
          //
          if( TempData == WiFi_START )
              {
              WiFi_Rx.Count = 0 ;
              WiFiProcessStatus.b.RxStartByte = 1 ;
              }
          //
          WiFi_Rx.Buffer[WiFi_Rx.Count] = TempData ;
          WiFi_Rx.Count += 1 ;
          if( WiFi_Rx.Count >= _MaxBufferLength_ )
              {
              WiFi_Rx.Count = 0 ;
              }
          //
          if( TempData == WiFi_END )
              {
              if( WiFiProcessStatus.b.RxStartByte == 1 )
                  {
                  WiFi_Rx.TotalLength = WiFi_Rx.Count ;
                  WiFiProcessStatus.b.RxStartByte = 0 ;
                  WiFi_Rx.Count = 0 ;
                  RxCounter += 1 ;
                  WiFiProcessStatus.b.RxEnd = 1 ;
                  }
              }
          //
          }
      else
          {
          WiFiProcessStatus.b.RxStartByte = 0 ;
          }
      }
  //
  return ;
}







//##############################################################################
// Function : CRC16_CITT
// Initial CRC : 0xFFFF
// Input : pData ( Data address )
//         Length ( Data Length )
// Return : CRC Value
//##############################################################################
#ifdef    _LookUpTable
const unsigned short CRC16_Table[256]={ 
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0 
};

unsigned short WiFi_CRC_CITT( unsigned char *pData, unsigned short Length )
{
  unsigned short crc = 0xFFFF; // Initial CRC = 0xFFFF
  unsigned char nBit = 0 ;
  unsigned short i ;
 
  for( i = 0 ; i < Length ; i++ )
      {        
      nBit = (crc&0xff00) >> 8 ;
      crc <<= 8 ;
      crc ^= CRC16_Table[nBit^pData[i]];
      }
  
  //crc = ~crc ;
  return crc ;
}

#else
unsigned short WiFi_CRC_CITT( unsigned char *pData, unsigned short Length )
{
  unsigned short crc = 0xFFFF; // Initial CRC = 0xFFFF
  unsigned char B = 0 ;
  unsigned short i ;
  unsigned char j ;
  
  for( i = 0 ; i< Length ; i++ )
      {        
      for( j = 0 ; j < 8 ; j++ )
          {
          B = ((pData[i]<<j)&0x80) ^ ((crc&0x8000)>>8) ;
          crc <<= 1 ;
          if( B != 0)        
              crc ^= 0x1021 ;
          }
      }
  //crc = ~crc;
  return crc ;
}
#endif


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_SetCommWorkMode( unsigned char Mode )
{
  WiFi_WorkMode = Mode ;
  return ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
unsigned char WiFi_GetCommWorkMode(void)
{
  return WiFi_WorkMode ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
WiFiControlStatus *WiFi_GetWiFiControlStatus(void)
{
  return((WiFiControlStatus *)&WiFiProcessStatus) ;
}


//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ClearLCBProgrammingStatus(void)
{
  WiFi_UCBLCB_LeaveProgramming(0x22) ;
  //WiFi_LCBInfoData.Member.InfoOK = 0 ;
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
void WiFi_ClearUCBProgrammingStatus(void)
{
  WiFi_UCBLCB_LeaveProgramming(0x11) ;
  //WiFi_UCBInfoData.Member.InfoOK = 0 ;
  return ;
}

//##############################################################################
// Function : 
// Input : none
// Return : none
//##############################################################################
unsigned char WiFi_CheckUCBInfo( unsigned char *Source,unsigned char *Target,unsigned char Mode )
{
  unsigned char RetValue = 1 ;
  unsigned char idx = 0 ;
  while( *(Source+idx) != 0 && idx < 20 )
      {
      if(Mode == 0)
          {
          if( *(Source+idx) != *(Target+idx) )
              {
              return 0 ;
              }
          }
      else
          {
          if( *(Source+idx) < *(Target+idx) )
              {
              return 1 ;
              }
          else
              {
              if( *(Source+idx) > *(Target+idx) )
                  return 0 ;
              else
                  RetValue = 0 ;    
              }
          }
      idx += 1 ;
      }
  
  // 20110727
  if( idx < 20 && *(Target+idx) != 0 )
      RetValue = 0 ;
  //
  
  return RetValue ;
}




// 20120130---------------------------------------------------------------------
unsigned char WiFi_GetAPConnectStatus(void)
{
  if( WiFiReceivePacket->Status.b.APConnect == 1 )
      return 1 ;
  else
      return 0 ;
}




void __WiFimemset(void *s, char c, long n)
{       /* store c throughout unsigned char s[n] */
  const unsigned char uc = c;
  unsigned char *su = (unsigned char *)s;
  
  for (; 0 < n; ++su, --n)
  *su = uc;
  return ;
}


long __WiFistrlen(const char *s)
{       /* find length of s[] */
  const char *sc;
  
  for (sc = s; *sc != '\0'; ++sc)
    ;
  return (sc - s);
}




