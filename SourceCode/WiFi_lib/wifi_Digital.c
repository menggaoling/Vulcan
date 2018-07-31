
#include "wifi_define.h"
#include "wifi_Digital.h"

#include "WiFi_Information.h"
#include "wifi_interface.h"
#include "EEPRom_Language.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system_config.h"

#define DRVE_START 0xF1
#define DRVE_END   0xF2

#define Null            0xff

extern unsigned char RTSFlag;
bool flag_502_t = false;  // 20150901
__no_init bool delay_get_status; //20160302
__no_init char cmd503_response_state; //0:fail 1:success 2:in progress 20160304
void Process_Message();
void WIFI_UART_TXDATA(unsigned char by_D) ;
unsigned char WiFi_CanSendCommmand(void);

__no_init WIFI_SETTINGS wifi_settings;// WiFi information

static UINT16 by_10ms_timer = 0;
static UINT16 by_10ms_timer2 = 0;

static unsigned char  wifiPoweronFinish = 0;

__no_init static UINT16 by_xMs_Timer_wifi;

// 2016 RSCU
static unsigned char SoftwareReadyToDownload = 0;
static unsigned char SoftwareDownloadFinish = 0;
extern bool WiFi_BinGetFromModulle;
extern unsigned long ProgramDataIndex;
extern unsigned char ProgramData[_Bin_1_time_size];

// 2016 WiFi AP
//extern unsigned char AP_Num;
//extern AP_Information AP;

// 2016 WiFi connect
unsigned char wifi_connect = 0;

//--- Test WiFi
struct
{
    unsigned long SendingTimes;
    unsigned long RecievingTimes;
    unsigned short OnePackageTime;  // unit: 10ms
    unsigned char Flag;
}WiFi_Test = {0};

// kmy
//static MSG_RECV _MSG_BUFFER;

//--- Send and receive for wifi communication
#define MSG_RECV_MAX_DATALENGTH 1024+5
#define MSG_PACKAGE_SIZE MSG_RECV_MAX_DATALENGTH+8

// Data Index
#define  MSG_STARTBYTE   0 
#define  MSG_COMMAND     MSG_STARTBYTE+1
#define  MSG_DATALENGTH  MSG_COMMAND+2
#define  MSG_DATA        MSG_DATALENGTH+2
//#define  MSG_CRC         MSG_DATA+MSG_RECV_MAX_DATALENGTH
//#define  MSG_ENDBYTE     MSG_CRC+2


// Variables
unsigned char WiFi_TxData[MSG_PACKAGE_SIZE] = {0};   // The Tx buffers which are ready to send data to Wifi module.
unsigned char WiFi_RxData[MSG_PACKAGE_SIZE] = {0};   // The Rx buffers which receive date from wifi module.
unsigned short WiFi_TxCounter = 0, WiFi_TotalTx = 0;
unsigned short WiFi_RxCounter = 0, WiFi_TotalRx = 0;
union
{
    struct
    {
        unsigned char RX_Finish : 1;
        unsigned char CommandDelay : 1;
        unsigned char NotReconnect : 1;
        unsigned char Non : 5;
    }bit;
    unsigned char all;
}WiFi_State = {0};



// kmy
volatile char send_ready = 0;

void Digital_Initial_Data_wifi( void )
{
    WiFi_memset(&wifi_settings,0,sizeof(WIFI_SETTINGS));

    by_xMs_Timer_wifi = 0;
    wifiPoweronFinish = 0;
    by_10ms_timer = 0;
    by_10ms_timer2 = 0;

    // Clear Tx, Rx counter
    WiFi_TxCounter = 0;
    WiFi_TotalTx = 0;
    WiFi_RxCounter = 0;
    WiFi_TotalRx = 0;
    WiFi_State.all = 0;

    send_ready = 0;
    delay_get_status = false;
    cmd503_response_state = 2;
    wifi_ResetInit();
}

void Digital_Command_Proce_wifi( void )
{
            if( wifiPoweronFinish > 4 )
            {
                RunWiFiInterface( by_10ms_timer );
            }
}

void Digital_10ms_Timeout_Int_wifi( void )
{
    static unsigned char ReadyFlag = 0, CommDelayTime = 0;
    _UINT16 CRCValue = {0};
  
    by_10ms_timer++;
  by_10ms_timer2++;
  if(ReadyFlag != IO_WiFi_Ready())
  {
      ReadyFlag = IO_WiFi_Ready();
      by_10ms_timer2 = 0;
      if( wifiPoweronFinish < 20)
      {
          wifiPoweronFinish ++;
      }
  }
  if(by_10ms_timer2 > 1000 ) //|| (WiFi_Information_Get(_WiFi_ProgramStop)))//400
    {// 10 seconds
      if(wifiPoweronFinish != 0)
      {
          wifiPoweronFinish = 0;
          wifi_ResetInit();  
      }
  }
  
  //
    if(send_ready == 1 && WiFi_State.bit.CommandDelay == 1)
    {// Command delays 20ms
        if(++CommDelayTime >= 2)  // 20ms
  {
            WiFi_State.bit.CommandDelay = 0;
            CommDelayTime = 0;
            send_ready = 0;
        }
  }
  else
        CommDelayTime = 0;

    //---WiFi communication timeout!!!
  if( send_ready == 1 && WiFi_State.bit.RX_Finish == 0)
  {
      if( ++by_xMs_Timer_wifi >= 400)  
      {// 4 seconds
          WiFi_Test.OnePackageTime = by_xMs_Timer_wifi;
          //
          send_ready = 0;
          by_xMs_Timer_wifi = 0;
          //
          __HAL_UART_CLEAR_FLAG(&huart5,UART_FLAG_ORE) ;
          __HAL_UART_DISABLE_IT(&huart5, UART_IT_RXNE);
      }
  }
  else
      by_xMs_Timer_wifi = 0;

    // Rx Completed and analyze rx data
    if(send_ready == 1 && WiFi_State.bit.RX_Finish == 1)
    {
        if(WiFi_RxCounter > 3)
        {
            CRCValue.INT_VAL = GenerateCRC_CCITT_F1( &WiFi_RxData[0], (WiFi_RxCounter-3));
            if( CRCValue.Dat.L_CHAR_VAL ==  WiFi_RxData[WiFi_RxCounter - 3] && CRCValue.Dat.H_CHAR_VAL ==  WiFi_RxData[WiFi_RxCounter - 2])
                Process_Message();
        }
        WiFi_State.bit.RX_Finish = 0;
        WiFi_State.bit.CommandDelay = 1;
    }
}

unsigned char StartSendingMessage(unsigned short Command, unsigned short DataLength)
{
    unsigned short i = 0, by_CRC = 0;
    
    if( WiFi_CanSendCommmand() == 1 )
	{
        i = MSG_STARTBYTE;
        // Start byte
        WiFi_TxData[i++] = DRVE_START;
        
        // Command
        WiFi_TxData[i++] = Command&0x00FF;
        WiFi_TxData[i++] = Command>>8;
        
        // DataLength
        WiFi_TxData[i++] = DataLength&0x00FF;
        WiFi_TxData[i++] = DataLength>>8;

        // Data
        i += DataLength;
        
        // CRC
        by_CRC = GenerateCRC_CCITT_F1( WiFi_TxData, DataLength + 5 );
        WiFi_TxData[i++] = by_CRC&0x00FF;
        WiFi_TxData[i++] = by_CRC>>8;
        
        // End Byte
        WiFi_TxData[i++] = DRVE_END;
        WiFi_TotalTx = i;
        WiFi_TxCounter = 0;
        //
        WiFi_State.bit.RX_Finish = 0; 
        WiFi_State.bit.CommandDelay = 0;
        WiFi_memset( WiFi_RxData, 0, MSG_PACKAGE_SIZE ); // clear rx buffer
        send_ready = 1;
        //
        by_xMs_Timer_wifi = 0;
        //
        
        __HAL_UART_DISABLE_IT(&huart5, UART_IT_RXNE);
        return 1;
	}
	return 0;
}


unsigned char SendWifiCommand( UINT16 message_id, char* data, int data_size )
{
    if(WiFi_CanSendCommmand() == 0)
		return 0;
	
    WiFi_Set_WiFiTx_Data(0, data_size, (unsigned char*)data);
    return StartSendingMessage(message_id, data_size);
}

unsigned char SendSetMachineInfo( char *string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_MACHINE_INFO, string, string_length );
}


unsigned char  SendGetWifiVersion()
{
	return SendWifiCommand( WIFI_GET_WIFI_VERSION, NULL, 0 );
}
// WIFI_GET_UART_VERSION
unsigned char  SendGetUartVersion()
{
	return SendWifiCommand( WIFI_GET_UART_VERSION, NULL, 0 );
}

// WIFI_GET_STATUS
unsigned char  SendGetStatus()
{
	return SendWifiCommand( WIFI_GET_STATUS, NULL, 0 );
}
// WIFI_GET_MAC_ADDRESS
unsigned char  SendGetMacAddress()
{
	return SendWifiCommand( WIFI_GET_MAC_ADDRESS, NULL, 0 );
}
// WIFI_GET_IP
unsigned char  SendGetIP()
{
	return SendWifiCommand( WIFI_GET_IP, NULL, 0 );
}
// WIFI_CONNECT_AP
unsigned char  SendConnectAP()
{
	return SendWifiCommand( WIFI_CONNECT_AP, NULL, 0 );
}
// WIFI_DISCONNECT_AP
unsigned char  SendDisconnectAP()
{
	return SendWifiCommand( WIFI_DISCONNECT_AP, NULL, 0 );
}
// WIFI_SET_SSID
unsigned char  SendSetSSID( char*string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_SSID, string, string_length );
}
// WIFI_SET_SECURITY_TYPE
unsigned char  SendSetSecurityType( UINT8 type )
{
	return SendWifiCommand( WIFI_SET_SECURITY_TYPE, (char*)&type, sizeof(UINT8) );
}
// WIFI_SET_SECURITY_KEY
unsigned char  SendSetSecurityKey( char *string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_SECURITY_KEY, string, string_length );
}
// WIFI_GET_SSID
unsigned char  SendGetSSID()
{//0x0011
	return SendWifiCommand( WIFI_GET_SSID, NULL, 0 );
}
// WIFI_GET_SECURITY_TYPE
unsigned char  SendGetSecurityType()
{
	return SendWifiCommand( WIFI_GET_SECURITY_TYPE, NULL, 0 );
}
// WIFI_GET_SECURITY_KEY
unsigned char  SendGetSecurityKey()
{
	return SendWifiCommand( WIFI_GET_SECURITY_KEY, NULL, 0 );
}
// WIFI_GET_TCP_BUFFER_SIZE
unsigned char  SendGetTCPBufferSize( UINT8 index )
{
	return SendWifiCommand( WIFI_GET_TCP_BUFFER_SIZE, (char*)&index, sizeof(UINT8) );
}
// WIFI_SET_TCP_ADDRESS
unsigned char SendSetTCPAddress( UINT8 index, char *string, UINT16 string_length )
{
    if(WiFi_CanSendCommmand() == 0)
		return 0;
    
    WiFi_Set_WiFiTx_Data(0, 1, &index);
    WiFi_Set_WiFiTx_Data(2, string_length, (unsigned char*)string);
   
    return StartSendingMessage(WIFI_SET_TCP_ADDRESS, string_length + 1);
}
// WIFI_GET_TCP_ADDRESS
unsigned char  SendGetTCPAddress( UINT8 index )
{
	return SendWifiCommand( WIFI_GET_TCP_ADDRESS, (char*)&index, sizeof(UINT8) );
}
// WIFI_SET_TCP_PORT
unsigned char SendSetTCPPort( UINT8 index, UINT16 port, UINT8 mode )
{
    unsigned char PortData[4] = {0};
    
    if(WiFi_CanSendCommmand() == 0)
		return 0;
    
    PortData[0] = index;
    PortData[1] = port&0x00FF;
    PortData[2] = port>>8;
    PortData[3] = mode;
    
    WiFi_Set_WiFiTx_Data(0, 4, &PortData[0]);
    return StartSendingMessage(WIFI_SET_TCP_PORT, 4);
}
// WIFI_GET_TCP_PORT
unsigned char  SendGetTCPPort( UINT8 index )
{
	return SendWifiCommand( WIFI_GET_TCP_PORT, (char*)&index, sizeof(UINT8) );
}
// WIFI_SET_UDP_PORT
unsigned char  SendSetUDPPort( UINT16 port )
{
	return SendWifiCommand( WIFI_SET_UDP_PORT, (char*)&port, sizeof(UINT16) );
}
// WIFI_GET_UDP_PORT
unsigned char  SendGetUDPPort()
{
	return SendWifiCommand( WIFI_GET_UDP_PORT, NULL, 0 );
}
// WIFI_TCP_CONNECT
unsigned char  SendTCPConnect( UINT8 index )
{
	return SendWifiCommand( WIFI_TCP_CONNECT, (char*)&index, sizeof(UINT8) );
}
// WIFI_TCP_DISCONNECT
unsigned char  SendTCPDisconnect( UINT8 index )
{
	return SendWifiCommand( WIFI_TCP_DISCONNECT, (char*)&index, sizeof(UINT8) );
}

unsigned char  SendGetMachineUUID()
{//0x0015
	return SendWifiCommand( WIFI_GET_MACHINE_UUID, NULL, 0 );
}

unsigned char  SendGetSignalStrength()
{
    //0x0014
    //return SendWifiCommand( WIFI_GET_SIGNAL_STRENGTH, NULL, 0 );
    //0x0019
    return SendWifiCommand( WIFI_GET_CONNECTED_STRENGTH, NULL, 0 );  
}

// WIFI_SET_TCP_DATA
unsigned char SendSetTCPData( UINT8 index, char *data, UINT16 data_size )
{
    if(WiFi_CanSendCommmand() == 0)
		return 0;
 
    WiFi_Set_WiFiTx_Data(0, 1, &index);
    WiFi_Set_WiFiTx_Data(1, data_size, (unsigned char*)data);

    return StartSendingMessage(WIFI_SET_TCP_DATA, data_size + 1);
}
// WIFI_GET_TCP_DATA
unsigned char  SendGetTCPData( UINT8 index )
{
	return SendWifiCommand( WIFI_GET_TCP_DATA, (char*)&index, sizeof(UINT8) );
}

// WIFI_SET_SSID_AP
unsigned char  SendSetSSID_AP( char *string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_SSID_AP, string, string_length );
}
// WIFI_GET_SSID_AP
unsigned char  SendGetSSID_AP()
{
	return SendWifiCommand( WIFI_GET_SSID_AP, NULL, 0 );
}
// WIFI_SET_AP_SECURE_TYPE
unsigned char  SendSetSecurityType_AP( UINT8 type )
{
	return SendWifiCommand( WIFI_SET_AP_SECURE_TYPE, (char*)&type, sizeof(UINT8) );
}
// WIFI_SET_AP_SECURE_KEY
unsigned char  SendSetSecurityKey_AP( char *string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_AP_SECURE_KEY, string, string_length );
}
// WIFI_GET_AP_SECURE_TYPE
unsigned char  SendGetSecurityType_AP()
{
	return SendWifiCommand( WIFI_GET_AP_SECURE_TYPE, NULL, 0 );
}
// WIFI_GET_AP_SECURE_KEY
unsigned char  SendGetSecurityKey_AP()
{
	return SendWifiCommand( WIFI_GET_AP_SECURE_KEY, NULL, 0 );
}
// WIFI_SET_BROADCAST_DATA
unsigned char  SendSetBroacastData( char *string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_BROADCAST_DATA, string, string_length );
}
// WIFI_SET_AP_STA_MODE
unsigned char  SendSetApStaMode( UINT8 type )
{
	return SendWifiCommand( WIFI_SET_AP_STA_MODE, (char*)&type, sizeof(UINT8) );
}

unsigned char SendSetWorkoutIP( char *string, UINT16 string_length )
{//0x0504
    if(WiFi_CanSendCommmand() == 0)
		return 0;

    WiFi_Set_WiFiTx_Data(0, string_length, (unsigned char*)string);
    WiFi_Set_WiFiTx_Data(string_length, strlen( DATA_SERVER_LOCATION ), DATA_SERVER_LOCATION);
    WiFi_Set_WiFiTx_Data(string_length+strlen( DATA_SERVER_LOCATION ), 1, 0);
    return StartSendingMessage(WIFI_SET_WORKOUT_DATA_IP, string_length+strlen( DATA_SERVER_LOCATION ) + 1);

}

char SendRemoteUpdate( char* console, char* equipment, char* version, UINT16 lcbVersion, UINT16 wifiVersion, int clubId, char* serialNumber )
{
    if(WiFi_CanSendCommmand() == 0)
            return false;

    char data[512]={0};
    sprintf( (char*)data, "%s%s?c=%s&e=%s&usv=%s&lsv=%d&wsv=%d&cid=%d&cs=%s", machineConfig->MarioBox, UPDATE_SERVER_LOCATION, console, equipment,
		version, lcbVersion, wifiVersion, clubId, serialNumber );

    return SendWifiCommand( WIFI_SET_REMOTE_UPDATE, data, strlen( data ) + 1 );
}

unsigned char ResetWiFi()
{//0x0900
	return SendWifiCommand( WIFI_RESET_WIFI, NULL, 0 );
}

unsigned char  SetHTTPData( char *string, UINT16 string_length )
{//0x0500
  delay_get_status = true;//delay send Get status cmd flag
	return SendWifiCommand( WIFI_SET_HTTP_DATA, string, string_length );
}

unsigned char SendHTTPData( HTTP_METHOD command, HTTP_DATA_TYPE dataType, char *url, UINT16 url_length, UINT8 Secure)
{//0x0502
    unsigned char Other[3] = {0};
    if(WiFi_CanSendCommmand() == 0)
		return 0;

    Other[0] = Secure;
    Other[1] = command;
    Other[2] = dataType;
    
    WiFi_Set_WiFiTx_Data(0, 3, &Other[0]);
    WiFi_Set_WiFiTx_Data(3, url_length, (unsigned char*)url);
    
    
    cmd503_response_state = 2;//wait 0503 response
    return StartSendingMessage(WIFI_SEND_HTTP_DATA, url_length + 3);
}

unsigned char GetHTTPResponseData()
{//0x0503
	return SendWifiCommand( WIFI_GET_HTTP_RESPONSE, NULL, 0 );
}

// 2014.06.05
/*******************************************************************************
* Function Name  : wifi_Digital_UartTxRx_Information()
* Description    : wifi UART TX / RX 資料傳輸與接收 <放置在USART中斷中使用>
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char wifi_Digital_UartTxRx_Information(void)
{
    unsigned char Rtn = 0;
    
  if(__HAL_UART_GET_IT_SOURCE(&huart5, UART_IT_RXNE ) != RESET && __HAL_UART_GET_FLAG(&huart5, UART_FLAG_RXNE) != RESET)
  {//==>判斷是否接收中斷
      /* Read one byte from the receive data register */
      Digital_Recv_Int_wifi((uint8_t)(huart5.Instance->DR & (uint16_t)0x00FF));
      /* Clear the UART5 Receive interrupt */
      //USART_ClearITPendingBit(UART5, USART_IT_RXNE);  
        //
        Rtn =  1;
  }
  
  if(__HAL_UART_GET_IT_SOURCE(&huart5, UART_IT_TXE) != RESET && __HAL_UART_GET_FLAG(&huart5, UART_FLAG_TXE) != RESET)
  {//==>判斷是否發送中斷
      /* Write one byte to the transmit data register */
      Digital_Send_FIFO_wifi();
      /* Clear the USART5 transmit interrupt */
        Rtn = 1;
  }
    
    return Rtn;
}

// When we detect an F0, F1, or F2 byte in the data stream we must convert it to
// F0F0, F001, and F002
unsigned char SendByte( unsigned char *stupidByteShift, UCHAR data )
{
  if( !*stupidByteShift && ( data == 0xF0 || data == 0xF1 || data == 0xF2 ) )
  {
      *stupidByteShift = 1;
      WIFI_UART_TXDATA( 0xF0 );
      return 0;
  }
  else if( *stupidByteShift )
  {
      WIFI_UART_TXDATA( data - 0xF0 );
      *stupidByteShift = 0;
      return 1;
  }
  else
  {
      *stupidByteShift = 0;
      WIFI_UART_TXDATA( data );
      return 1;
  }
}
// wifi TX int
void Digital_Send_FIFO_wifi( void )
{
  static unsigned char stupidByteShift = 0;
  
  __HAL_UART_CLEAR_FLAG(&huart5, UART_FLAG_TXE);
    
    if(WiFi_TxCounter == 0 || WiFi_TxCounter == (WiFi_TotalTx - 1))
              {
        WIFI_UART_TXDATA( WiFi_TxData[WiFi_TxCounter++]);
              }
                  else
              {
        if(WiFi_TxCounter > WiFi_TotalTx)
                  {
            __HAL_UART_DISABLE_IT(&huart5, UART_IT_TXE);
            __HAL_UART_DISABLE_IT(&huart5, UART_IT_RXNE);
            //
            WiFi_TxCounter = 0;
            WiFi_TotalTx = 0;
                  }
        else if(WiFi_TxCounter == WiFi_TotalTx)
        {// Complete Tx
            __HAL_UART_DISABLE_IT(&huart5, UART_IT_TXE);
            __HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
            
            // Clear RX counter
            WiFi_RxCounter = 0;
            WiFi_TotalRx = 0;
            WiFi_State.bit.RX_Finish = 0;
            
            // WiFi Test
            WiFi_Test.SendingTimes++;
            if(WiFi_Test.Flag == 0)
                WiFi_Test.SendingTimes = 0;
              }
              else
              {
            if( SendByte( &stupidByteShift, WiFi_TxData[WiFi_TxCounter]))
                WiFi_TxCounter++;
                  }
              }
}


// kmy
// Copy the strings "src" to the strings "dest"
void strcpy_s( char* dest, char* src, int destSize, int srcSize )
{
	WiFi_memset( dest, 0, destSize );
    strncpy( dest, src, ( srcSize > destSize - 1 ) ? (destSize - 1) : srcSize );
}

UCHAR Digital_GetError_wifi(void)
{
 return  wifi_settings.status.error; 
}

void Process_Message()
{
    //
    _UINT8BIT Data = {0};
    _UINT16 DataLength = {0};
    _UINT16 Command = {0};
    long_temp Addr_temp = {0};

    unsigned char *Rx_Data;

    // Command
    Command.Dat.L_CHAR_VAL = WiFi_RxData[MSG_COMMAND];
    Command.Dat.H_CHAR_VAL = WiFi_RxData[MSG_COMMAND+1];

    // Data Length
    DataLength.Dat.L_CHAR_VAL = WiFi_RxData[MSG_DATALENGTH];
    DataLength.Dat.H_CHAR_VAL = WiFi_RxData[MSG_DATALENGTH+1];

    // Analyze data
    Rx_Data = &WiFi_RxData[MSG_DATA];
    //_MSG_BUFFER.DATA[_MSG_BUFFER.LENGTH.INT_VAL] = 0;
    Rx_Data[DataLength.INT_VAL] = 0;
                
    if( DataLength.INT_VAL >= 4 )
    {// Status analysis
        Data.all = Rx_Data[0];
        
        if(wifi_settings.isInWorkout == false || (CheckRFIDLoginResult() != WIFI_SUCCESS && CheckRFIDLoginResult() != WIFI_SUCCESS))
            WiFi_State.bit.NotReconnect = 0;      
        
        if(WiFi_State.bit.NotReconnect == 0)
                {
            if(wifi_settings.status.wifi_connected == 1 && Data.Dat.bit0 == 0) // true -> false
                WifiConnectionStatusChange( false );
            else if(wifi_settings.status.wifi_connected == 0 && Data.Dat.bit0 == 1 ) // false -> true
        {
                WifiConnectionStatusChange( true );
                WiFi_State.bit.NotReconnect = 1;
        }
                }
                  ///////////////////////////////////////////////////////////////////////
        wifi_settings.status.wifi_connected =      (bool)Data.Dat.bit0;
        wifi_settings.status.error =               (bool)Data.Dat.bit1;
        wifi_settings.status.unsupported_command = (bool)Data.Dat.bit2;
        wifi_settings.status.resend_package =      (bool)Data.Dat.bit3;
        wifi_settings.status.buffer_full =         (bool)Data.Dat.bit4;
        wifi_settings.status.data_ready =          (bool)Data.Dat.bit5;
        wifi_settings.status.tcp_connection =      (bool)Data.Dat.bit6;
        wifi_settings.status.ucb_lcb_updating =    (bool)Data.Dat.bit7;
                
        //
        Data.all = Rx_Data[1];
        wifi_settings.status.mcu_updating =     (bool)Data.Dat.bit0;
        wifi_settings.status.ucb_updating =     (bool)Data.Dat.bit1;
        wifi_settings.status.lcb_updating =     (bool)Data.Dat.bit2;
        wifi_settings.status.lcb2_updating =    (bool)Data.Dat.bit3;
        wifi_settings.status.tcp_connected[0] = (bool)Data.Dat.bit4;
        wifi_settings.status.tcp_connected[1] = (bool)Data.Dat.bit5;
        wifi_settings.status.tcp_connected[2] = (bool)Data.Dat.bit6;
        wifi_settings.status.tcp_connected[3] = (bool)Data.Dat.bit7;

        wifi_settings.status.last_error.Dat.L_CHAR_VAL = Rx_Data[2];
        wifi_settings.status.last_error.Dat.H_CHAR_VAL = Rx_Data[3];
	}

    switch( Command.INT_VAL )
	{
	case WIFI_GET_WIFI_VERSION:
            wifi_settings.wifi_version.Dat.L_CHAR_VAL = Rx_Data[4];
            wifi_settings.wifi_version.Dat.H_CHAR_VAL = Rx_Data[5];
		break;
	case WIFI_GET_UART_VERSION:
            wifi_settings.uart_version.Dat.L_CHAR_VAL = Rx_Data[4];
            wifi_settings.uart_version.Dat.H_CHAR_VAL = Rx_Data[5];
		break;
	case WIFI_GET_MAC_ADDRESS:
            WiFi_memcpy( wifi_settings.mac_address, &Rx_Data[4], 6 );
		break;
	case WIFI_GET_IP://0x0005
            WiFi_memcpy( wifi_settings.ip_address, &Rx_Data[4], 6 );
		break;
	case WIFI_GET_SSID://0x0011
            strcpy_s( wifi_settings.security.ssid, (char*)( &Rx_Data[4]), 33, DataLength.INT_VAL - 4 );
		break;
	case WIFI_GET_SECURITY_TYPE:
            wifi_settings.security.security_type = (WIFI_SECURITY_TYPE)Rx_Data[4];
		break;
	case WIFI_GET_SECURITY_KEY:
            if( DataLength.INT_VAL == 4 )
			WiFi_memset( (char*)wifi_settings.security.security_key, 0, 255 );
		else
                strcpy_s( (char*)wifi_settings.security.security_key, (char*)( &Rx_Data[4] ), 255, DataLength.INT_VAL - 4 );
		break;
	case WIFI_GET_MACHINE_UUID://0x0015
            if( DataLength.INT_VAL == 0x28 )
            if( WiFi_memcmp( &Rx_Data[4], machineConfig->machineIdentification, _WIFI_Length_MachineID ) != 0 )
			{
                WiFi_memcpy( machineConfig->machineIdentification, &Rx_Data[4], _WIFI_Length_MachineID );
			}
		break;
	case WIFI_GET_SIGNAL_STRENGTH:
            if( DataLength.INT_VAL == 5 )
                wifi_settings.signalStrength = Rx_Data[4];
		break;
	case WIFI_GET_TCP_ADDRESS:
            if( Rx_Data[4] < 4 && Rx_Data[4] > 0 )
		{
                strcpy_s( (char*)wifi_settings.tcp_settings[Rx_Data[4] - 1].ip_address, (char*)( &Rx_Data[5] ), 20,
                DataLength.INT_VAL - 5 );
		}
		break;
	case WIFI_GET_TCP_PORT://0x0024
            if( Rx_Data[4] < 4 && Rx_Data[4] > 0 )
		{
                wifi_settings.tcp_settings[Rx_Data[4] - 1].port.INT_VAL = *(UINT16*)( &Rx_Data[5] );
                wifi_settings.tcp_settings[Rx_Data[4] - 1].mode = (TCP_CONNECTION_MODE)( Rx_Data[7] );
		}
		break;
	case WIFI_GET_TCP_DATA://0x0028
            ProcessTCPData( Rx_Data[4], (char*)( &Rx_Data[5] ), DataLength.INT_VAL - 5 );
		break;
	case WIFI_GET_TCP_BUFFER_SIZE:
            if( Rx_Data[4] < 4 && Rx_Data[4] > 0 )
		{
                wifi_settings.tcp_settings[Rx_Data[4] - 1].tx_buffer_size.INT_VAL = *(UINT16*)( &Rx_Data[5] );
                wifi_settings.tcp_settings[Rx_Data[4] - 1].rx_buffer_size.INT_VAL = *(UINT16*)( &Rx_Data[7] );
		}
		break;
	case WIFI_GET_UDP_PORT:
            wifi_settings.udp_port = *(UINT16*)( &Rx_Data[4] );
		break;
	case WIFI_GET_SSID_AP:
            strcpy_s( wifi_settings.ap_security.ssid, (char*)( &Rx_Data[4] ), 33, DataLength.INT_VAL - 4 );
		break;
	case WIFI_GET_AP_SECURE_TYPE:
            wifi_settings.ap_security.security_type = (WIFI_SECURITY_TYPE)Rx_Data[4];
		break;
	case WIFI_GET_AP_SECURE_KEY:
            strcpy_s( (char*)wifi_settings.ap_security.security_key, (char*)( &Rx_Data[4] ), 255, DataLength.INT_VAL - 4 );
		break;
        //// 20150901
        case WIFI_SEND_HTTP_DATA:
            flag_502_t = (Rx_Data[2] == 1)?true:false;
                break;
        ////////////////////////////////////////////////////
	case WIFI_GET_HTTP_RESPONSE://0x0503
            {
              if(DataLength.INT_VAL == 7)//4 + HTTP status code(2)+data type(1)
              {
                cmd503_response_state = 0;
              }
              else if(DataLength.INT_VAL > 7)
              {
                if(Rx_Data[4] == 0xC8)//http status 200
                  cmd503_response_state = 1;
              }       
              RTSFlag = 0;//lot of response data need Get,if not clear here,will cause error
              ProcessHTTPPost( (char*)( &Rx_Data[4] ), DataLength.INT_VAL - 4, by_10ms_timer );
            }            
            break;
        ////////////////////////////////////////////////////
//        case WIFI_SET_HTTP_DATA: // 0x0500 workout complete data re-send
//            flag_500 = Rx_Data[2];
//            break;
        //// 2016 RSCU
        case WIFI_SET_REMOTE_UPDATE: // 0x0600
            switch(Rx_Data[4])
            {
                case 0x11:
                    SoftwareReadyToDownload = 1;
                    break;
                default:
                    SoftwareReadyToDownload = 2;
                    break;
            }
            break;
        case WiFi_UCBLCB_GetProgrammingStatus:
            switch(Rx_Data[4])
            {
                case 0x11: // Ready for programming form wifi module.
                    SoftwareDownloadFinish = 1;
                    break;
                case 0x44:  // firmware download fail.
                    SoftwareDownloadFinish = 2;
                    break;
                case 0x22: // firmware is downloading.    
                default: // Waiting for downloading
                    SoftwareDownloadFinish = 0;
                    break;
            }
            break;
	case WiFi_UCB_ProgrammingData:
            WiFi_BinGetFromModulle = true;
            //WiFi_memcpy(ProgramData, &Rx_Data[4], (DataLength.INT_VAL - 4));  // 2.45
            // 2.48
            Addr_temp.Dat.CHAR_VAL_0 = Rx_Data[4];
            Addr_temp.Dat.CHAR_VAL_1 = Rx_Data[5];
            Addr_temp.Dat.CHAR_VAL_2 = Rx_Data[6];
            Addr_temp.Dat.CHAR_VAL_3 = Rx_Data[7];
            ProgramDataIndex = Addr_temp.INT_VAL;
            WiFi_memcpy(ProgramData, &Rx_Data[8], (DataLength.INT_VAL - 8));  
            break;    
        case WIFI_GET_CONNECTED_STRENGTH:   // 0x0019
            wifi_settings.ConnectedStrength = Rx_Data[4];
            break;
        case WIFI_SEARCH_AP:    // 0x001A
            //AP_Num = Rx_Data[4];
            //WiFi_memcpy(&AP.All[0], &Rx_Data[5], (33*AP_Num)); 
            break;
	default: // kmy3
		break;
	}
}
// wifi RX int
void Digital_Recv_Int_wifi(UCHAR by_RXBUF0)
{
    static unsigned char F0_Check = 0;
  
    __HAL_UART_CLEAR_FLAG(&huart5, UART_FLAG_RXNE);
    

    if(WiFi_RxCounter == 0)
  {
        F0_Check = 0;
        if(by_RXBUF0 == DRVE_START)
  {
            WiFi_RxData[WiFi_RxCounter++] = by_RXBUF0;
  }
              }
              else
              {
        if(WiFi_RxCounter < MSG_PACKAGE_SIZE)
              {
            if(F0_Check == 1)
            {
                WiFi_RxData[WiFi_RxCounter++] = (0xF0 + by_RXBUF0);
                F0_Check = 0;
            }
            else
            {
                if(by_RXBUF0 == 0xF0)
                    F0_Check = 1;
            else
                WiFi_RxData[WiFi_RxCounter++] = by_RXBUF0;
                
            } 
              }
        if(by_RXBUF0 == DRVE_END)
        {// RX Completed
            WiFi_State.bit.RX_Finish = 1;
              // Disable the UART5 Receive interrupt 
              __HAL_UART_CLEAR_FLAG(&huart5, UART_FLAG_ORE) ;
              __HAL_UART_DISABLE_IT(&huart5, UART_IT_RXNE);
              
            // WiFi Test
            WiFi_Test.RecievingTimes++;
            if(WiFi_Test.Flag == 0)
                WiFi_Test.RecievingTimes = 0;
            WiFi_Test.OnePackageTime = by_xMs_Timer_wifi;
  }
}
}

void WIFI_UART_TXDATA(unsigned char by_D) 
{
  huart2.Instance->DR = (by_D & 0xFF);
}

/* WiFi communication test*/
void WiFiTest_Start(void)
{
    WiFi_Test.SendingTimes = 0;
    WiFi_Test.RecievingTimes = 0;
    WiFi_Test.Flag = 1;
}
void WiFiTest_End(void)
{
    WiFi_Test.SendingTimes = 0;
    WiFi_Test.RecievingTimes = 0;
    WiFi_Test.Flag = 0;
}
unsigned long WiFiTest_GetInfo(unsigned char Index)
{
    unsigned long by_Dat = 0;
    switch(Index)
    {
        case _SendingTimes:
            by_Dat = WiFi_Test.SendingTimes;
            break;
        case _RecievingTimes:
            by_Dat = WiFi_Test.RecievingTimes;
            break;
        case _StartFlag:
            by_Dat = WiFi_Test.Flag;
            break;
        case _OnePackageTime:
            by_Dat = WiFi_Test.OnePackageTime; // unit: 10ms
            break;
        default:
            break;
    }
    return by_Dat;
}
unsigned char WiFiTest_Communication(void)
{
    if(WiFiTest_GetInfo(_StartFlag) == 1)
        return SendGetMacAddress();//SendGetSSID();
    return 0;
}
/*----------------------------------------------------------------------------*/
void WiFi_Set_WiFiTx_Data(unsigned char DataIndex, unsigned short DataLength, unsigned char* Data)
{// place the data info of package format. 
    unsigned short i = 0;
    
    if(DataLength != 0)
    {
        for(i=0; i<DataLength; i++)
            WiFi_TxData[MSG_DATA+i+DataIndex] = *(Data+i);
    }
}
unsigned char WiFi_CanSendCommmand(void)
{//0: not sent, 1:Yes
    if(send_ready == 0)// && IO_WiFi_Ready() == 0)
        return 1;
    return 0;
}
// WIFI_SET_SSID_TYPE_KEY
unsigned char  SendSetSSIDTypeKey( char *ssid, char type, char *key )
{
    if(WiFi_CanSendCommmand() == 0)
        return 0;
    
    WiFi_Set_WiFiTx_Data(0, 32, (unsigned char*)ssid);
    WiFi_Set_WiFiTx_Data(32, 1, (unsigned char*)&type);
    WiFi_Set_WiFiTx_Data(33, 64, (unsigned char*)key);
            
    return StartSendingMessage(WIFI_SET_SSID_TYPE_KEY, 32 + 1 + 64);
}

// WIFI_SET_AP_SSID_TYPE_KEY
unsigned char  SendSetAP_SSIDTypeKey( void )
{
    if(WiFi_CanSendCommmand() == 0)
        return 0;
    
    WiFi_Set_WiFiTx_Data(0, strlen(WIFI_SSID), (unsigned char*)WIFI_SSID);
    WiFi_Set_WiFiTx_Data(strlen(WIFI_SSID), 1, 0);
            
    return StartSendingMessage(WIFI_SET_AP_SSID_TYPE_KEY, strlen(WIFI_SSID) + 1);
}

// 2016 RSCU
char getRemoteUpdate_ReadyToDownload(void)
{// 1: Success, 2:Failure,
    return SoftwareReadyToDownload;
}

UCHAR getRemoteUpdate_DownloadFinish(void)
{// 1: Success, 2:Failure,
    return SoftwareDownloadFinish;    
}
void getRemoteUpdate_Clear(void)
{
    SoftwareReadyToDownload = 0;
    SoftwareDownloadFinish = 0;
}

unsigned char WiFiBoot_CheckUCBInfo( unsigned char *Source,unsigned char *Target, unsigned char Len )
{
  unsigned char RetValue = 1 ;
  unsigned char idx = 0 ;
  while( *(Source+idx) != 0 && idx < Len )
  {
      if( *(Source+idx) != *(Target+idx) )
      {
          return 0 ;
      }
      idx += 1 ;
  }
  // 20110727
  if( idx < Len && *(Target+idx) != 0 )
  {
      RetValue = 0 ;
  }
  //
  return RetValue ;
}

