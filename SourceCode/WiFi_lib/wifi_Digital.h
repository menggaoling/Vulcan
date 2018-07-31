#ifndef _DIGITAL_wifi
#define _DIGITAL_wifi
#include "General.h"
#include "wifi_api.h"

#include "CRC.H"

#define YDRIVER   0x00
#define NDRIVER   0xff

#define M_TM    0
#define M_EPBI  1

#define NO_PAR   0


//#define WIFI_DELAY 1000
//#define FALSE   0x00
//#define TRUE    0xFF

// kmy
#define WIFI_GET_WIFI_VERSION    0x0001 
#define WIFI_GET_UART_VERSION    0x0002
#define WIFI_GET_STATUS          0x0003
#define WIFI_GET_MAC_ADDRESS     0x0004
#define WIFI_GET_IP              0x0005
#define WIFI_CONNECT_AP          0x0006
#define WIFI_DISCONNECT_AP       0x0007
#define WIFI_SET_SSID            0x0008
#define WIFI_SET_SECURITY_TYPE   0x0009
#define WIFI_SET_SECURITY_KEY    0x000A
//#define WIFI_GET_ERASE_FLASH     0x0010
#define WIFI_GET_SSID            0x0011
#define WIFI_GET_SECURITY_TYPE   0x0012
#define WIFI_GET_SECURITY_KEY    0x0013
#define WIFI_GET_SIGNAL_STRENGTH 0x0014
#define WIFI_GET_MACHINE_UUID    0x0015
#define WIFI_CLEAR_ERROR_CODE    0x0016
#define WIFI_SET_MACHINE_INFO    0x0017
#define WIFI_SET_SSID_TYPE_KEY   0x0018   // 0x0008 + 0x0009 + 0x000A
#define WIFI_GET_CONNECTED_STRENGTH  0x0019
#define WIFI_SEARCH_AP           0x001A
#define WIFI_GET_TCP_BUFFER_SIZE 0x0020
#define WIFI_SET_TCP_ADDRESS     0x0021
#define WIFI_GET_TCP_ADDRESS     0x0022
#define WIFI_SET_TCP_PORT        0x0023
#define WIFI_GET_TCP_PORT        0x0024
#define WIFI_TCP_CONNECT         0x0025
#define WIFI_TCP_DISCONNECT      0x0026
#define WIFI_SET_TCP_DATA        0x0027
#define WIFI_GET_TCP_DATA        0x0028
#define WIFI_SET_SSID_AP         0x0030
#define WIFI_SET_AP_SECURE_TYPE  0x0031
#define WIFI_SET_AP_SECURE_KEY   0x0032
#define WIFI_GET_SSID_AP         0x0033
#define WIFI_GET_AP_SECURE_TYPE  0x0034
#define WIFI_GET_AP_SECURE_KEY   0x0035
#define WIFI_SET_AP_SSID_TYPE_KEY      0x0036    // 0x0030 + 0x0031 + 0x0032
#define WIFI_SET_UDP_PORT        0x0091
#define WIFI_GET_UDP_PORT        0x0092

#define WiFi_UCBLCB_DownFirmware         0x0200  // 12 0   Length(4)+StartAddr(4)+EndAddr(4)
#define	WiFi_UCBLCB_LeaveProgrammingMode 0x0201  // 1  0
#define	WiFi_UCBLCB_GetProgrammingStatus 0x0202  // 0  1
#define	WiFi_UCB_ProgrammingData	 0x0203  // 6  4+N   Length(2)+FlashAddr(4) FlashAddr(4)+Data(N) N<=256
#define	WiFi_LCB_ProgrammingData	 0x0303  // 6  N   Length(2)+FlashAddr(4)

#define WIFI_SET_HTTP_DATA       0x0500
#define WIFI_SEND_HTTP_DATA   	 0x0502
#define WIFI_GET_HTTP_RESPONSE   0x0503
#define WIFI_SET_WORKOUT_DATA_IP 0x0504


//#define WIFI_SET_WORKOUT_DATA_IP 0x0504
#define WIFI_SET_REMOTE_UPDATE   0x0600
#define WIFI_SET_BROADCAST_DATA  0x0700
#define WIFI_SET_AP_STA_MODE     0x0800
#define WIFI_RESET_WIFI          0x0900

// 2.48
typedef union long_temp
{ struct
 {
   unsigned char CHAR_VAL_0;
   unsigned char CHAR_VAL_1;
   unsigned char CHAR_VAL_2;
   unsigned char CHAR_VAL_3;
 }Dat;
 unsigned long INT_VAL;
} long_temp;

// kmy
typedef union _UINT16
{ struct
 {
   UCHAR L_CHAR_VAL;
   UCHAR H_CHAR_VAL;
 }Dat;
 UINT16 INT_VAL;
} _UINT16;

typedef union _CRC16
{ struct
 {
   UCHAR CRC_H;
   UCHAR CRC_L;
 }Dat;
UINT16 CRC_HL;
}CRC16;

typedef union
 {
    struct
 {
        unsigned char bit0:1;
        unsigned char bit1:1;
        unsigned char bit2:1;
        unsigned char bit3:1;
        unsigned char bit4:1;
        unsigned char bit5:1;
        unsigned char bit6:1;
        unsigned char bit7:1;
 }Dat;
    unsigned char all;
}_UINT8BIT;

typedef enum _TCP_CONNECTION_MODE
{
  TCP_CLIENT = 0x11,
  TCP_SERVER = 0x22,
}TCP_CONNECTION_MODE;

typedef enum _WIFI_MODE
{
  AP_MODE_ON = 0x11,
  STA_MODE_ON = 0x33,
  DUAL_MODE_ON = 0x55,
  AP_MODE_SSID_HIDDEN = 0x66,
  AP_MODE_SSID_BROADCAST = 0x77,
}WIFI_MODE;

typedef enum _WIFI_SECURITY_TYPE
{
  WIFI_SECURITY_UNSECURED = 0,  // None
  WIFI_SECURITY_WEP40,          // WEP
  WIFI_SECURITY_WEP104,         // WEP-104Bits
  WIFI_SECURITY_WPA_PHRASE,     // WPA-PSK
  WIFI_SECURITY_WPA2_PHRASE,    // WPA2-PSK
  WIFI_SECURITY_WPA_AUTO_PHRASE,// WPA_AUTO-PSK
  WIFI_SECURITY_AUTO_DETECT,
  NUM_WIFI_SECURITY_TYPES
}WIFI_SECURITY_TYPE;

typedef struct _WIFI_STATUS {
  bool wifi_connected;
  bool error;
  bool unsupported_command;
  bool resend_package;
  bool buffer_full;
  bool data_ready;
  bool tcp_connection;
  bool ucb_lcb_updating;
  
  bool mcu_updating;
  bool ucb_updating;
  bool lcb_updating;
  bool lcb2_updating;
  bool tcp_connected[4];
  bool UCBfileCheckOK;
  _UINT16 last_error;
} WIFI_STATUS;

typedef struct _WIFI_SECURITY_SETTINGS {
  char ssid[33];
  WIFI_SECURITY_TYPE security_type;
   char security_key[255];//unsigned  2014.06.17
  
} WIFI_SECURITY_SETTINGS;

typedef struct _WIFI_TCP_SETTINGS {
  _UINT16 tx_buffer_size;
  _UINT16 rx_buffer_size;
  unsigned char ip_address[20];
  _UINT16 port;
  TCP_CONNECTION_MODE mode;
} WIFI_TCP_SETTINGS;

typedef struct _WIFI_SETTINGS {
  WIFI_STATUS status;
  _UINT16 wifi_version;
  _UINT16 uart_version;
  unsigned char mac_address[_WIFI_Length_MacAddress];
  unsigned char ip_address[_WIFI_Length_IPAddress];
  WIFI_SECURITY_SETTINGS security;
  WIFI_SECURITY_SETTINGS ap_security;
  WIFI_TCP_SETTINGS tcp_settings[3];
  char data_server[256];
  UINT16 udp_port;
  UINT8 signalStrength;
  bool isInRSCU;
  bool isInWorkout;
  char ConnectedStrength;
} WIFI_SETTINGS;

typedef enum {
	HTTP_GET,
	HTTP_POST
} HTTP_METHOD;

typedef enum {
	HTTP_APPLICATION_URLENCODED,
	HTTP_APPLICATION_JSON,
	HTTP_APPLICATION_XML,
	HTTP_APPLICATION_OCTET_STREAM,
	HTTP_APPLICATION_TEXT,
} HTTP_DATA_TYPE;

#pragma pack( push, 1 )
typedef struct _HTTP_Post_Command {
	UINT8 post;
	UINT8 dataType;
	char url[256];
} HTTP_Post_Command;
typedef struct{
	UINT8 Secure;
	char Server[256];
	char Url[256];
	UINT16 Port;
}DAPI_ADDRESS_INFO;
#pragma pack( pop )



extern WIFI_SETTINGS wifi_settings;

void Digital_10ms_Timeout_Int_wifi(void);
void Digital_Recv_Int_wifi(UCHAR by_RXBUF0);
void Digital_Send_FIFO_wifi(void);

UCHAR Digital_GetError_wifi(void);//debug by ckm


unsigned char SendGetWifiVersion();
unsigned char SendGetUartVersion();
unsigned char SendGetStatus();
unsigned char SendGetMacAddress();
unsigned char SendGetIP();
unsigned char SendConnectAP();
unsigned char SendDisconnectAP();
unsigned char SendSetSSID( char *string, UINT16 string_length );
unsigned char SendSetSecurityType( UINT8 type );
unsigned char SendSetSecurityKey( char *string, UINT16 string_length );
unsigned char SendGetSSID();
unsigned char SendGetSecurityType();
unsigned char SendGetSecurityKey();
unsigned char SendGetTCPBufferSize( UINT8 index );
unsigned char SendSetTCPAddress( UINT8 index, char *string, UINT16 string_length );
unsigned char SendGetTCPAddress( UINT8 index );
unsigned char SendSetTCPPort( UINT8 index, UINT16 port, UINT8 mode );
unsigned char SendGetTCPPort( UINT8 index );
unsigned char SendTCPConnect( UINT8 index );
unsigned char SendTCPDisconnect( UINT8 index );
unsigned char SendSetTCPData( UINT8 index, char *data, UINT16 data_size );
unsigned char SendGetTCPData( UINT8 index );
unsigned char SendSetUDPPort( UINT16 port );
unsigned char SendGetUDPPort();
unsigned char SendSetSSID_AP( char *string, UINT16 string_length );
unsigned char SendGetSSID_AP();
unsigned char SendSetSecurityType_AP( UINT8 type );
unsigned char SendSetSecurityKey_AP( char *string, UINT16 string_length );
unsigned char SendGetSecurityType_AP();
unsigned char SendGetSecurityKey_AP();
unsigned char SendSetBroacastData( char *string, UINT16 string_length );
unsigned char SendSetApStaMode( UINT8 type );
unsigned char SetHTTPData(char *string, UINT16 string_length);
unsigned char SendHTTPData( HTTP_METHOD command, HTTP_DATA_TYPE dataType, char *url, UINT16 url_length, UINT8 Secure);

unsigned char GetHTTPResponseData();
unsigned char SendSetWorkoutIP( char *string, UINT16 string_length );
unsigned char SendSetMachineInfo( char *string, UINT16 string_length );
unsigned char ResetWiFi();

char SendRemoteUpdate( char* console, char* equipment, char* version, UINT16 lcbVersion, UINT16 wifiVersion, int clubId, char* serialNumber );


unsigned char SendGetMachineUUID();
unsigned char SendGetSignalStrength();
UCHAR GetHttpCmdCoounter ( void );

unsigned char SendWifiCommand( UINT16 message_id, char* data, int data_size );

unsigned char wifi_Digital_UartTxRx_Information(void);
char getRemoteUpdate_ReadyToDownload(void);
UCHAR getRemoteUpdate_DownloadFinish(void);
void getRemoteUpdate_Clear(void);
UCHAR GetProgramReadStatus(void);
unsigned char WiFiBoot_CheckUCBInfo( unsigned char *Source,unsigned char *Target, unsigned char Len );
//
void WiFiTest_Start(void);
void WiFiTest_End(void);
unsigned char WiFiTest_Communication(void);
//
enum
{
    _SendingTimes = 0,
    _RecievingTimes,
    _StartFlag,
    _OnePackageTime,
};
unsigned long WiFiTest_GetInfo(unsigned char Index);
//
void WiFi_Set_WiFiTx_Data(unsigned char DataIndex, unsigned short DataLength, unsigned char* Data);
// WIFI_SET_SSID_TYPE_KEY
unsigned char  SendSetSSIDTypeKey( char *ssid, char type, char *key );
// WIFI_SET_AP_SSID_TYPE_KEY
unsigned char  SendSetAP_SSIDTypeKey(void);

//
#endif
