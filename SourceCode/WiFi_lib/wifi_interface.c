/*
 * wifi_interface.c
 *
 *  Created on: May 2, 2013
 *      Author: Brenden.Capps
 */

#include "wifi_define.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wifi_interface.h"
#include "wifi_Digital.h"
#include "HT1381.h"
#include "wifi_event_handlers.h"
#include "wifi_api.h"
#include "WiFi_Information.h"
#include "EEPRom_Language.h"
#include "Boot.h"

// 20150901
unsigned char RTSFlag = 0;
extern bool flag_502_t;
extern bool delay_get_status;
extern char cmd503_response_state;
// extern function and variable
extern const unsigned char Boot_Manufacture[52] ;
extern const unsigned char Boot_McuType[20];
extern const unsigned char Boot_ModuleName[20] ;
extern const unsigned char Boot_Version[20];

#define _wifi_pending_responses_total 10
static bool CanLoginUserFlag;
static char pre_http_command;
// 2014.10.13
// ???à?B?Y JHT HQ?rD?§?§?§??a????§|
//#define FACTORY_TEST_SSID "JHT HQ"//FACTORY
//#define FACTORY_TEST_PW   "ful6g0pewlew"
#define FACTORY_TEST_SSID "Wifi-Test"//FACTORY
#define FACTORY_TEST_PW   ""

//
#define HTTP_STATUS_OK 200
#define _SYNCWORD 0xAA55

typedef struct
{
  UINT8 state;
  UINT16 command;
  UINT16 error;
  UINT8 port;
} WifiResponseState;


void GetWorkoutDataComplete( WIFIWorkoutDataCompleteEx* data, SHT1381_RealTime WorkoutStartTime);
void GetWorkoutData( WIFIWorkoutDataEx* data, SHT1381_RealTime WorkoutStartTime );
void SHT1381ToNormal(SHT1381_RealTime Source, DateAndTime* Dest);

WIFI_STATUS_TYPE _wifiStatus;
WIFI_EVENT_HANDLE _wifiEventHandle;

static UINT16 wifi_response_index = 0;
static UINT16 wifi_next_response_index = 0;
static UINT8 wifi_num_pending_responses = 0;
WIFIWorkoutDataCompleteEx wkDataComplete;
WIFIWorkoutDataEx WorkdataTrack;
DEPLOYMENT_TYPE remoteUpdateDeploymentType = DEPLOYMENT_PRODUCTION;//DEPLOYMENT_DEVELOPMENT DEPLOYMENT_QA DEPLOYMENT_DEVELOPMENT DEPLOYMENT_PRODUCTION
SOFTWARE_TYPE   remoteUpdateSoftwareType = SOFTWARE_TYPE_NON_MCU;
RSCUData RSCU={0};
unsigned short SoftwareToEEPROMPercent = 0;  // the percentage which software flashes into EEPROM
union
{
 char byte[4];
 unsigned long all;
}RSCU_SoftwareVer = {0};

const char* softwaretype[SOFTWARE_TYPE_END]=
{
 "gui",
 "boot_loader",
 "non_gui_mcu",
 "non_gui_sub_mcu",
 "lcb_1",
 "lcb_2",
 "lcm",
 "wifi",
 "bluetooth",
 "rfid",
 "nfc"
};


static WifiResponseState wifi_response_state[_wifi_pending_responses_total] = {0};
static WifiCommand wifiResponse;
static int wifi_response_substate = 0;
static UINT16 transactionId;
static UINT16 dataTransactionId = 0;
//
static UINT16 workoutTrackingDataRate = 1;
static UINT8 workoutTrackingDataCount = 0;
static WORKOUT_STATE _workoutState = WIFI_WORKOUT_IDLE;
static UINT8 _userInfoUpdated = 0;

static WIFIUserLoginRequest _loginRequest;
static HTTP_Post_Command postCommand;
__no_init static DAPI_ADDRESS_INFO DapiInfo;
static SHT1381_RealTime _workoutStartTime;
static bool _sentWorkoutData = false;

static UINT16 programT;

static UserResponse _userDataResponse;
static RFIDData rfiddata;

unsigned char WiFi_Timer_Count = 0;
bool WiFi_BinGetFromModulle = false;
bool WiFi_CommandGetFromModulleOK = false;
unsigned long ProgramDataIndex = 0;
unsigned char ProgramData[_Bin_1_time_size]={0};
static UCHAR _UserExternalID[_WIFI_Length_UserExternalID];


// wifi connect
extern unsigned char wifi_connect;

struct
{
    unsigned GetLatestVersionStatus  : 1;
    unsigned GetLatestVersionError   : 1;
    unsigned HaveTheLatestSoftware : 1;
    unsigned CloseRSCU : 1;
    unsigned RSCU_Error : 1;
} WiFi_Interface = {0};

void InitMessage( UINT8 lingoId, UINT8 messageId, UINT16 tranId, UINT16 dataSize )
{
  wifiResponse.header.syncWord = _SYNCWORD;
  wifiResponse.header.transactionId = tranId;//transactionId++;
  wifiResponse.header.dataSize = dataSize;
  wifiResponse.header.lingoId = lingoId;
  wifiResponse.header.messageId = messageId;
  wifiResponse.header.checksum =  dataSize > 0 ? GenerateCRC_CCITT( (UPtr8)&wifiResponse.data, dataSize ) : 0;
}
void NextWiFiState()
{
  wifi_response_substate = 0;
  wifi_response_state[wifi_response_index].state = 0;
  wifi_response_index++;
  wifi_num_pending_responses--;
  if( wifi_response_index == _wifi_pending_responses_total )
              wifi_response_index = 0;
}

void AddWifiResponseState( UINT8 state, UINT16 command, UINT16 error, UINT8 port )
{
  //    unsigned char by_Dat = 0;
  
  if(state == WCS_POST_WORKOUT_DATA && wifi_settings.status.wifi_connected == 0)
    return;
  
  if( wifi_num_pending_responses == _wifi_pending_responses_total || WiFiTest_GetInfo(_StartFlag) == 1)
  {//20160420 Garry Meng
    if(state == WCS_GET_REMOTE_DATA || state == WCS_COMPLETE_WORKOUT)
    {
      if(wifi_next_response_index)
        wifi_next_response_index--;
      wifi_num_pending_responses--;
    }
    else
      return;
  }
  //    for(int wnri = 0; wnri < wifi_num_pending_responses; wnri++)
  //    {
  //        by_Dat = wnri + wifi_next_response_index;
  //        if(by_Dat >= _wifi_pending_responses_total)
  //            by_Dat -= _wifi_pending_responses_total;
  //        
  //        if(wifi_response_state[by_Dat].state != WCS_POST_WORKOUT_DATA)
  //        {
  //            if(wifi_response_state[by_Dat].state == state && \
    //               wifi_response_state[by_Dat].command == command && \
      //               wifi_response_state[by_Dat].error == error && \
        //               wifi_response_state[by_Dat].port == port )
        //          return;
        //        }
        //    }
          
          wifi_response_state[wifi_next_response_index].port = port;//ProcessExternalMessage(interface)
          wifi_response_state[wifi_next_response_index].state = state;//WIFI_COMMAND_STATES
          wifi_response_state[wifi_next_response_index].command = command;//data_TransactionId
          wifi_response_state[wifi_next_response_index++].error = error;
          wifi_num_pending_responses++;
          if( wifi_next_response_index == _wifi_pending_responses_total )
            wifi_next_response_index = 0;
}

void ProcessHTTPPost( char* message, int length, UINT16 timer )
{
	if( length < sizeof(WifiHeader) + 3 )
		return;

	UINT16 statusCode = *(UINT16*)message;
	UINT8 dataType = message[2];
	UINT16 messageLocation = 3;
	WifiCommand* response;
        
	if( statusCode != HTTP_STATUS_OK )
	{
		if( _sentWorkoutData )
		{
			//SaveWorkoutToFlash();
			_sentWorkoutData = false;
		}
		return;
	}
        
	if( dataType != HTTP_APPLICATION_OCTET_STREAM )
		return;
        if(delay_get_status)
          delay_get_status = false;
	while( messageLocation < length - sizeof(WifiHeader) )
	{
		// Search for the sync word.
		response = (WifiCommand*)( message + messageLocation );
		if( response->header.syncWord != _SYNCWORD )
		{
			messageLocation++;
			continue;
		}

		messageLocation += response->header.dataSize + sizeof(WifiHeader);
		if( messageLocation > length )
			break;

		UINT16 checksum = GenerateCRC_CCITT( (UPtr8)&response->data, response->header.dataSize );

		// Verify the checksum.
		if( response->header.checksum != checksum )
			continue;

		switch( response->header.lingoId )
		{
		case WIFI_LINGO_WORKOUT_TRACKING://0x80
			switch( response->header.messageId )
			{
			case WIFI_WORKOUT_TRACKING_ACKNOWLEDGMENT:
				if( response->data.Ack.messageType == WIFI_WORKOUT_TRACKING_COMPLETE_EX )
				{// ?\??§??§??Y?§???§|????DAPI  
					if( response->data.Ack.error != WIFI_ACK_OK )
					{
						//SaveWorkoutToFlash();
					}                                                                                
                                        if(EEPROM_ReadInformation(EE_AM)==1) 
                                          AddWifiResponseState( WCS_UPDATE_MACHINE_STATS, 0, 0, 0 );
					_sentWorkoutData = false;
				}
			}
			break;
		case WIFI_LINGO_ASSET_MANAGEMENT://0x81
			switch( response->header.messageId )
			{
			case WIFI_ASSET_MANAGEMENT_ACKNOWLEDGMENT:
                                if( response->data.Ack.messageType == WIFI_ASSET_MANAGEMENT_GET_LATEST_VERSION )
				{ 
                                    if( response->data.Ack.error != WIFI_ACK_OK )
                                    {
                                  WiFi_Interface.GetLatestVersionError=true;
                                    }
				}  
                                else if( response->data.Ack.messageType == WIFI_ASSET_MANAGEMENT_ASSET_MANAGEMENT_ENABLED )
                                {
                                  AddWifiResponseState( WCS_UPDATE_MACHINE_INFO, 0, 0, 0 ); 
                                }
				break;
                        case WIFI_ASSET_MANAGEMENT_GET_LATEST_VERSION:
                                // ??§?§??èa?e??§a,??yDAPI?§a?§o3?§|?§?é?§?é?§?é?§?é??§2D?§?§?§??a?D??à?§?è????¨¤??r??§?????§???§|_
                            //WiFi_memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                            
                            //WiFi_memset( &wifiResponse.data.responseLatestVersion, 0, sizeof(WIFIDATAResponseLatestVersion) );
                            //strcpy(wifiResponse.data.responseLatestVersion.CurrentVersion,response->data.responseLatestVersion.CurrentVersion);
                            //strcpy(wifiResponse.data.responseLatestVersion.SoftwareType,response->data.responseLatestVersion.SoftwareType);
                            //strcpy(wifiResponse.data.responseLatestVersion.SoftwareUrl,response->data.responseLatestVersion.SoftwareUrl);                        
                            //strcpy(wifiResponse.data.responseLatestVersion.MD5,response->data.responseLatestVersion.MD5);
                            
                            
                                for(int i = 0; i<SOFTWARE_TYPE_END ;i++)
                                {
                                for(int j=0; j<strlen(softwaretype[i]); j++)
                                  {
                                    if(softwaretype[i][j] != response->data.responseLatestVersion.SoftwareType[j])
                                        break;
                                    
                                    if((j+1) == strlen(softwaretype[i]))
                                    RSCU.DatatoModule.softwaretype = i;
                                  }
                                }
                            
                            //
                            WiFi_Interface.HaveTheLatestSoftware = 0;
                            if(RSCU.DatatoModule.softwaretype == SOFTWARE_TYPE_NON_MCU)
                            {// check for version
                             if(WiFi_CheckRSCUVersion(response->data.responseLatestVersion.CurrentVersion) == 1)
                                     WiFi_Interface.HaveTheLatestSoftware = 1;
                                 strcpy(RSCU.DatatoModule.URL, response->data.responseLatestVersion.SoftwareUrl);
                             strcpy(RSCU.DatatoModule.MD5, response->data.responseLatestVersion.MD5);     
                            }
                        WiFi_Interface.GetLatestVersionStatus=true;
                                break;
			case WIFI_ASSET_MANAGEMENT_MACHINE_INFO_EX://0x07
                       /* if( strncmp( machineConfig.timeZone, response->data.MachineInfoExResponse.timeZone, _WIFI_Length_TimeZone ) != 0 )
                                    AddWifiResponseState( WCS_UPDATE_TIME, 0, 0, 0 );
			        WiFi_memcpy( machineConfig->machineIdentification, response->data.MachineInfoExResponse.machineIdentification, 36 );
				machineConfig->brandid = response->data.MachineInfoExResponse.brandID;
				machineConfig->machineType = response->data.MachineInfoExResponse.machineType;
				machineConfig->modelType = response->data.MachineInfoExResponse.modelType;
                                machineConfig->driveType = response->data.MachineInfoExResponse.driveType;
                                machineConfig->consoleType = response->data.MachineInfoExResponse.consoleType;
				machineConfig->csafeManufactureId = response->data.MachineInfoExResponse.csafeManufacture;
				machineConfig->csafeId = response->data.MachineInfoExResponse.csafeID;
				machineConfig->csafeModelId = response->data.MachineInfoExResponse.csafeModelID;
				WiFi_memcpy( machineConfig->serialNumber, response->data.MachineInfoExResponse.serialNumber, WIFI_SerialNumberSize );//15 // 2015.05.28
				WiFi_memcpy( machineConfig->frameSerialNumber, response->data.MachineInfoExResponse.frameSerialNumber, WIFI_SerialNumberSize );//15 // 2015.05.28
				machineConfig->clubId = response->data.MachineInfoExResponse.clubID;
				machineConfig->minMPHx10 = response->data.MachineInfoExResponse.minMPHX10;
				machineConfig->maxMPHx10 = response->data.MachineInfoExResponse.maxMPHX10;
				machineConfig->minKPHx10 = response->data.MachineInfoExResponse.minKPHX10;
				machineConfig->maxKPHx10 = response->data.MachineInfoExResponse.maxKPHX10;
				machineConfig->maxInclinex10 = response->data.MachineInfoExResponse.maxInclineX10;
				machineConfig->minInclinex10 = response->data.MachineInfoExResponse.minInclineX10;
				machineConfig->maxLevel = response->data.MachineInfoExResponse.maxResistanceLevel;
				machineConfig->minLevel = response->data.MachineInfoExResponse.minResistanceLevel;
				machineConfig->maxUser = response->data.MachineInfoExResponse.maxUsers;
				WiFi_memcpy( machineConfig->timeZone, response->data.MachineInfoExResponse.timeZone, 65 );
				machineConfig->Autologin = response->data.MachineInfoExResponse.AutoLogin;
*/
                                WiFi_memcpy( machineConfig->machineIdentification, response->data.MachineInfoExResponse.machineIdentification, _WIFI_Length_MachineID );
				machineConfig->accumulatedDistanceMilesX100 = response->data.MachineInfoExResponse.accumulatedDistanceMilesX100;
				machineConfig->accumulatedSeconds = response->data.MachineInfoExResponse.accumulatedTime;
				machineConfig->headphoneInsertion = response->data.MachineInfoExResponse.headphoneInsertions;
				machineConfig->headphoneRemoval = response->data.MachineInfoExResponse.headphoneRemovals;
                            
                                //WiFi_Information_SaveMachineConfig();// 2014.06.17   SAVE_MACHINE_CONFIG_TO_FLASH
                                CanLoginUserFlag = true;
				break;
			case WIFI_ASSET_MANAGEMENT_MACHINE_REGISTRATION://0x02
				WiFi_memcpy( machineConfig->machineIdentification, response->data.RegisterResponse.machineIdentification, 36 );
				AddWifiResponseState( WCS_UPDATE_TIME, 0, 0, 0 );
				//WiFi_Information_SaveMachineConfig();// 2014.06.17   SAVE_MACHINE_CONFIG_TO_FLASH
				break;
			case WIFI_ASSET_MANAGEMENT_GET_DAPI_LOCATION:
				WiFi_memset(&DapiInfo, 0, sizeof(DAPI_ADDRESS_INFO));
                                if(EEPROM_ReadInformation(EE_SSL))
                                {
                                  DapiInfo.Port = response->data.AMWIFIDAPIResponse.Port;
                                  DapiInfo.Secure = response->data.AMWIFIDAPIResponse.Secure;
                                }
                                else//Garry Meng 2016/2/4 SSL 
                                {
                                  DapiInfo.Port = 80;
                                  DapiInfo.Secure = 0;
                                }
				WiFi_memcpy( DapiInfo.Server, response->data.AMWIFIDAPIResponse.Server, 256 );
				WiFi_memcpy( DapiInfo.Url, response->data.AMWIFIDAPIResponse.Url, 256 );
				AddWifiResponseState( WCS_SET_MARIO_BOX, 0, 0, 0 );
				machineConfig->wifiisAPMode = 0;
			break;
			}
			break; 
		case WIFI_LINGO_NETWORK_TIME://0x82
			switch( response->header.messageId )
			{
			case WIFI_NETWORK_TIME_ACKNOWLEDGMENT:
				break;
			case WIFI_NETWORK_TIME_GET_TIME://0x01
			{
				WIFIDataRegion region;
				region.year = response->data.TimeResponse.year;
				region.month = response->data.TimeResponse.month;
				region.day = response->data.TimeResponse.day;
				region.hour = response->data.TimeResponse.hour;
				region.minute = response->data.TimeResponse.minute;
				region.second = response->data.TimeResponse.second;
				region.units = machineConfig->units;
				region.timeFormat = machineConfig->timeFormat;
				WiFi_memcpy( region.timeZone, machineConfig->timeZone, _WIFI_Length_TimeZone );
                                
				if(UpdateRTCTime( &region, 0))
                                {
                                  AddWifiResponseState( WCS_AM_ENABLE, 0, 0, 0 );                           
                                }
			}
                        break;
			}
			break;
		case WIFI_LINGO_USER_MANAGEMENT://0x83
			switch( response->header.messageId )
			{
                            case WIFI_USER_MANAGEMENT_ACKNOWLEDGMENT:// 0x00
                                switch( response->data.Ack.messageType )
                                {
                                    case WIFI_USER_CHECK_XID:
                                       _userDataResponse.xidCheckResponse = response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                                       break;
                                     case WIFI_USER_LOGIN:                                    
                                        _userDataResponse.loginCheckResponse = response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                                       break;
                                     case WIFI_USER_REGISTRATION:
                                        _userDataResponse.registrationCheckResponse = response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                                        break;
                                     case WIFI_USER_UNLINK_USER:
                                        _userDataResponse.DeleteUserCheckResponse = response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                                        break;
                                     case WIFI_USER_RFID_LOGIN:
                                        _userDataResponse.RfidLoginflag= response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                                        break;
                                     case WIFI_USER_RFID_ASSOCIATE:
                                        _userDataResponse.RfidAssociateflag= response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                                        break;
                                     case WIFI_USER_RFID_DISASSOCIATE:
                                        _userDataResponse.RfidDisassociateflag= response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                                        break;
                                }
                                if(response->data.Ack.error == WIFI_ACK_OK)
                                {
                                    _wifiStatus= WIFI_STATUS_SUCCESS;
                                    _wifiEventHandle = WIFI_EVENT_SUCCESS;
                                }
                                else
                                {
                                    _wifiStatus= WIFI_STATUS_FAIL;
                                    _wifiEventHandle = WIFI_EVENT_FAIL;
                                }
                            break;
                        case WIFI_USER_RFID_LOGIN:// 0x12
                        {
                            if( response->data.rfidLoginResp.userID <= MAX_USERS)
                            {
                              UINT8 userId = response->data.UserDataResponse.userConsoleId;
                                WiFi_memcpy( _UserExternalID,  response->data.rfidLoginResp.externalUserID,  36 );
                                userFlashConfig[userId]->gender = response->data.rfidLoginResp.gender;
                                userFlashConfig[userId]->weightX10 = response->data.rfidLoginResp.userWeightX10;
                                userFlashConfig[userId]->age = response->data.rfidLoginResp.userAge;
                            userFlashConfig[userId]->birth_year = response->data.rfidLoginResp.userBirthYear;
                            userFlashConfig[userId]->birth_month = response->data.rfidLoginResp.userBirthMonth;
                            userFlashConfig[userId]->birth_day= response->data.rfidLoginResp.userBirthDay;
                                userFlashConfig[userId]->language= response->data.rfidLoginResp.userLanguageId;
                                userFlashConfig[userId]->DisplayUnits= response->data.rfidLoginResp.displayUnits;
                                userFlashConfig[userId]->AcceptedTerms= response->data.rfidLoginResp.acceptedTerms;
                                userFlashConfig[userId]->WorkoutDataShare= response->data.rfidLoginResp.shareWorkoutData;
                                WiFi_memset( userFlashConfig[userId]->name, 0, 65 );
                                strncpy( userFlashConfig[userId]->name, response->data.rfidLoginResp.userName, 64 );
                                userFlashConfig[userId]->LoggedOn = 1;
                                machineConfig->units = userFlashConfig[userId]->DisplayUnits;
                                userFlashConfig[userId]->isLastUser = 1;
                                _userDataResponse.loginCheckResponse = WIFI_SUCCESS;
                                _userDataResponse.RfidLoginflag = WIFI_SUCCESS;
                                _wifiStatus= WIFI_STATUS_SUCCESS;
                                _wifiEventHandle = WIFI_EVENT_SUCCESS;
                                WiFi_Information_SaveUserConfig();
                            }
                            else
                                _userDataResponse.RfidLoginflag= WIFI_FAIL;
                        }
                        break;     
			case WIFI_USER_LOGIN://0x02
				if(response->data.UserLoginResponse.userId <= MAX_USERS )
				{
                                  UINT8 userId = response->data.UserDataResponse.userConsoleId;
					WiFi_memcpy( _UserExternalID, response->data.UserLoginResponse.externalUserId, _WIFI_Length_UserExternalID );
					WiFi_memcpy( userFlashConfig[userId]->xid, _loginRequest.xid, XIDMAXLENGTH );
					WiFi_memcpy( userFlashConfig[userId]->passcode, _loginRequest.passcode, _XidPassWordLength );
						userFlashConfig[userId]->weightX10 = response->data.UserLoginResponse.weightX10+5;
						userFlashConfig[userId]->weightX10 =userFlashConfig[userId]->weightX10/10;
						userFlashConfig[userId]->weightX10 = userFlashConfig[userId]->weightX10*10;
					userFlashConfig[userId]->age = response->data.UserLoginResponse.age;
                                    userFlashConfig[userId]->birth_year = response->data.UserLoginResponse.birth_year;
                                    userFlashConfig[userId]->birth_month = response->data.UserLoginResponse.birth_month;
                                    userFlashConfig[userId]->birth_day = response->data.UserLoginResponse.birth_day;
					userFlashConfig[userId]->gender = response->data.UserLoginResponse.gender;
					userFlashConfig[userId]->language = response->data.UserLoginResponse.language;
					userFlashConfig[userId]->DisplayUnits = response->data.UserLoginResponse.DisplayUnits;
					userFlashConfig[userId]->AcceptedTerms = 0;
					userFlashConfig[userId]->UserSetup = 1;
                                        userFlashConfig[userId]->LoggedOn = 1;
                                        machineConfig->units = userFlashConfig[userId]->DisplayUnits;
					//Information_Set_Last_User( userId );
					WiFi_memset( userFlashConfig[userId]->name, 0, 65 );
					strncpy( userFlashConfig[userId]->name, response->data.UserLoginResponse.userName, 64 );
					//Main_Initial_Informatiom();
                                        //Main_Initial_Data();
					//WiFi_Information_SetWeight( userFlashConfig[userId]->weightX10/10);
					// ?§o1?§?????§??Y?§??Yd?§?§??x?¨¤?§a
                                        //WiFi_Information_Set(_WiFi_Unit,userFlashConfig[userId]->DisplayUnits);
                                        //WiFi_Information_Set(_WiFi_Weight,userFlashConfig[userId]->weightX10/10);
                                        //WiFi_Information_Set(_WiFi_Age,userFlashConfig[userId]->age);
                                        //WiFi_Information_Set(_WiFi_Gender,userFlashConfig[userId]->gender);
                                        
                                        //
                                        _userDataResponse.loginCheckResponse = 1;
					WiFi_Information_SaveUserConfig();// 2014.06.17  SAVE_USER_CONFIG
				}

				break;
			}
			break;
		}
	}

}

void WifiConnectionStatusChange( bool wifiConnected )
{
	if( wifiConnected )
    {// 2015.01.21
        WIFIClearStateBuffer();
        //
		AddWifiResponseState( WCS_GET_WIFI_INFO, 0, 0, 0 );
	}
	else
	{
		WiFi_memset( &wifi_settings.status, 0, sizeof(WIFI_STATUS) );
		WiFi_memset( &wifi_settings.mac_address, 0, _WIFI_Length_MacAddress );
		WiFi_memset( &wifi_settings.ip_address, 0, _WIFI_Length_IPAddress );
                WiFi_memset( &wifi_settings.security, 0, sizeof(WIFI_SECURITY_SETTINGS) );
		WiFi_memset( &wifi_settings.ap_security, 0, sizeof(WIFI_SECURITY_SETTINGS) );
		WiFi_memset( &wifi_settings.tcp_settings, 0, 3 * sizeof(WIFI_TCP_SETTINGS) );
                WiFi_memset( &wifi_settings.data_server, 0, 256 );
                //WiFi_memset( &DapiInfo , 0, sizeof(DAPI_ADDRESS_INFO) );
		wifi_settings.udp_port = 0;
                CanLoginUserFlag = false;
	}
}

UCHAR Wifi_Get_EnggineTestResult(void)
{
	if( wifi_settings.mac_address[0] == 0 )
	{
		return 1;
	}
	if( wifi_settings.ip_address[0] == 0 )
	{
		return 2;
	}
	return 0;
}

void ProcessExternalMessage( int interface, WifiCommand* message )
{
    if( wifi_num_pending_responses == _wifi_pending_responses_total )
		return;

	switch( message->header.lingoId )
	{
	case WIFI_LINGO_MODULE_SETTINGS:// 0x01
		switch( message->header.messageId )
		{
		case WIFI_GET_WIFI_SETTINGS:
			AddWifiResponseState( WCS_GET_WIFI_SETTINGS, 0, 0, interface );
			break;
		case WIFI_CONNECT_TO_AP:
			WiFi_memset( wifi_settings.security.ssid, 0, 33 );
			strncpy( wifi_settings.security.ssid, message->data.APConnection.ssid, 32 );
			wifi_settings.security.security_type = WIFI_SECURITY_AUTO_DETECT;
			WiFi_memset( wifi_settings.security.security_key, 0, 256 );
			WiFi_memcpy( wifi_settings.security.security_key, message->data.APConnection.securityKey, 255 );
			AddWifiResponseState( WCS_CONNECT_TO_AP, 0, 0, interface );
			break;
		}
		break;
	case WIFI_LINGO_GENERAL:// 0x02
		switch( message->header.messageId )
		{
		case WIFI_GET_LINGO_VERSION:
			AddWifiResponseState( WCS_GET_LINGO_INFO, message->data.UByte, 0, interface );
			break;      
		case WIFI_SET_DATA_SERVER:
			if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
				AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_DATA_SERVER, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
			else
			{
				WiFi_memset( machineConfig->MarioBox, 0, 256 );
				strncpy( machineConfig->MarioBox, message->data.DataServer.server, 255 );
				//WiFi_Information_SaveMachineConfig();// 2014.06.17   SAVE_MACHINE_CONFIG_TO_FLASH
				AddWifiResponseState( WCS_SET_DATA_SERVER, 0, 0, interface );
			}
			break;
		case WIFI_GET_DATA_SERVER:
			AddWifiResponseState( WCS_GET_DATA_SERVER, 0, 0, interface );
			break;
		case WIFI_CHECK_FOR_UPDATE:
			AddWifiResponseState( WCS_CHECK_FOR_UPDATE, 0, 0, interface );
			break;
		}
		break;
	case WIFI_LINGO_REMOTE:// 0x03
	case WIFI_LINGO_SETTINGS:// 0x04
	case WIFI_LINGO_TEST:// 0x05
        default:
		break;
	}
}

char tcpDataBuffer[3][sizeof(WifiCommand)];
UINT16 tcpDataBufferIndex[3] = { 0, 0, 0 };

int AddTCPDataToBuffer( int buffer, char* data, int data_size )
{
	if( data_size + tcpDataBufferIndex[buffer] > sizeof(WifiCommand) )
	{
		WiFi_memcpy( tcpDataBuffer[buffer] + tcpDataBufferIndex[buffer], data, sizeof(WifiCommand) - tcpDataBufferIndex[buffer] );
		return sizeof(WifiCommand) - tcpDataBufferIndex[buffer];
	}
	else
		WiFi_memcpy( tcpDataBuffer[buffer] + tcpDataBufferIndex[buffer], data, data_size );
	return data_size;
}

static bool init = false;
//static int external_data_timer = 0;
void wifi_ResetInit(void)
{
  init = false;
  pre_http_command = WCS_IDLE;
  CanLoginUserFlag = false;
  wifi_response_substate = 0;
  WiFi_memset( &DapiInfo , 0, sizeof(DAPI_ADDRESS_INFO) );
}

//
void RunWiFiInterface( UINT16 timer )
{
    char IpAndPort[300] = {0};  
    static int status_timer = 0;
    static int data_timer = 0;     
    static int http_timer = 0;
//    static int complete_timer = 0;
    static unsigned short  WaitForWIFIWriteFlash = 0; 
    bool updateDataTransaction = false;
    char _SendAdressBuffer[12]; // 200 // RSCU firmware 2.44
    if( !init )
    {
        WiFi_memset( _UserExternalID, 0, _WIFI_Length_UserExternalID );
        init = true;
        wifi_settings.isInRSCU = false;
        wifi_settings.isInWorkout = false;
        AddWifiResponseState( WCS_INIT_WIFI, 0, 0, 0 );
    }

    // 2016.02.19 Garry Meng
    if(wifi_settings.ip_address[0] !=0 )
    {
        if(IO_WiFi_RTS() == 0)
        {
            if( /*wifi_num_pending_responses < (_wifi_pending_responses_total - 1) && */RTSFlag == 0)
            {
                AddWifiResponseState( WCS_GET_REMOTE_DATA, 0, 0, 0 ); 
                RTSFlag = 1;
            }
        }
        else
            RTSFlag = 0;
    }    
    if( timer < status_timer ) status_timer = 0 ;
    if( timer - status_timer >= 2000 )
    {// 20`s ?§?§??§???????§???§??§C????§?§
//        AddWifiResponseState( WCS_GET_SIGNAL_STRENGTH , 0 , 0 , 0 ); 
      if(delay_get_status == true)//if send 0500 ,delay send get status cmd
        delay_get_status = false;
      else
        AddWifiResponseState(WCS_GET_STATUS, 0, 0, 0);
        
      status_timer = timer;      
 /*       if( WIFIIsOnline( ) == 0)
        {// ?]?B?W??§a?L???B?§|?
          if(wifi_settings.status.wifi_connected)
            WifiConnectionStatusChange((bool)1);
        }*/
    }
    // 2015.01.21
    //if(wifi_settings.status.wifi_connected)
    if( WIFIIsOnline( ) == 1)
    {
        if( timer < data_timer ) data_timer = 0 ;
        if( timer < http_timer ) http_timer = 0 ;
//        if( timer < complete_timer ) complete_timer = 0 ;
        if( timer - data_timer >= 100 * workoutTrackingDataRate )
        {// every 1`s
            if( (_workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED) && (userFlashConfig[0]->LoggedOn == 1) )
            {
                AddWifiResponseState( WCS_POST_WORKOUT_DATA, dataTransactionId, 0, 0 );
                updateDataTransaction = true;
            }
            data_timer = timer;
        }
        if( timer - http_timer >= 500 * workoutTrackingDataRate )  // 20150901
        {// every 200 ms
            if( flag_502_t == true)
            {
                flag_502_t = false;
                AddWifiResponseState( WCS_SEND_HTTP, 0, 0, 0 );
            }
            http_timer = timer;
        }
        if( updateDataTransaction )
        {
            dataTransactionId++;
        }
		        // resend http comand 
        if(cmd503_response_state == 0)//0503 command response err
        {
          if(pre_http_command != WCS_IDLE)
          {
            AddWifiResponseState( pre_http_command, 0, 0, 0 );
            cmd503_response_state = 2;
          }
        }
    }
    // Handle response states.
    switch( wifi_response_state[wifi_response_index].state )
    {
        case WCS_IDLE:
                  break;
	case WCS_GET_WIFI_INFO://used by WifiConnection_Status_Change
		switch( wifi_response_substate )
		{
		case 0:
			if( SendGetWifiVersion() )
				wifi_response_substate++;
			break;
		case 1:
			if( SendGetUartVersion() )
				wifi_response_substate++;
			break;
		case 2:
			if( SendGetMacAddress() )
				wifi_response_substate++;
			break;
		case 3:
			if( SendGetIP() )//0x0005
                            wifi_response_substate++;
			break;
		case 4:
			if( SendGetSSID() )//0x0011
			{
                             if( wifi_settings.ip_address[0] == 0 \
                                 && wifi_settings.ip_address[1] == 0 \
                                 && wifi_settings.ip_address[2] == 0 \
					&& wifi_settings.ip_address[3] == 0 )
                                     wifi_response_substate = 0;
				else
					wifi_response_substate++;
			}
			break;
		case 5:
			if( SendGetSecurityType() )//0x0012
				wifi_response_substate++;
			break;
		case 6:
			if( SendGetSecurityKey() )//0x0013
				wifi_response_substate++;
			break;
		case 7:
			if( SendGetSSID_AP() )//0x0033
				wifi_response_substate++;
			break;
		case 8:
			if( SendGetSecurityType_AP() )//0x0034
				wifi_response_substate++;
			break;
		case 9:
			if( SendGetSecurityKey_AP() )//0x0035
				wifi_response_substate++;
			break;
		case 10:
				wifi_response_substate++;
			break;
		case 11:
				wifi_response_substate++;
			break;
		case 12:
			if( SendGetTCPBufferSize( 1 ) )//0x0020
				wifi_response_substate++;
			break;
		case 13:
			if( SendSetTCPAddress( 1, "0.0.0.0", strlen( "0.0.0.0" ) + 1 ) )//0x0021
				wifi_response_substate++;
			break;
		case 14:
			if( SendGetTCPAddress( 1 ) )//0x0022
				wifi_response_substate++;
			break;
		case 15:
			if( SendSetTCPPort( 1, 30304, 0x11 ) )//0x0023
				wifi_response_substate++;
			break;
		case 16:
			if( SendGetTCPPort( 1 ) )//0x0024
				wifi_response_substate++;
			break;
		case 17:
			if( SendGetTCPBufferSize( 2 ) )//0x0020
				wifi_response_substate++;
			break;
		case 18:
			if( SendGetTCPAddress( 2 ) )//0x0022
				wifi_response_substate++;
			break;
		case 19:
			if( SendSetTCPPort( 2, 30305, 0x11 ) )//0x0023
				wifi_response_substate++;
			break;
		case 20:
			if( SendGetTCPPort( 2 ) )//0x0024
				wifi_response_substate++;
			break;
		case 21:
			if( SendGetTCPBufferSize( 3 ) )//0x0020
				wifi_response_substate++;
			break;
		case 22:
			if( SendGetTCPAddress( 3 ) )//0x0022
				wifi_response_substate++;
			break;
		case 23:
			if( SendSetTCPPort( 3, 30306, 0x11 ) )//0x0023
				wifi_response_substate++;
			break;
		case 24:
			if( SendGetTCPPort( 3 ) )//0x0024
				wifi_response_substate++;
			break;
		case 25:
			if( SendSetWorkoutIP( (CHAR*)machineConfig->MarioBox, strlen( machineConfig->MarioBox) + 1 ) )//0x0504
				wifi_response_substate++;
			break;
		case 26:
                        wifi_response_substate++;
			break;
		case 27:
			if( SendTCPConnect( 1 ) )//0x0025
				wifi_response_substate++;
			break;
		case 28:
			if( SendTCPConnect( 2 ) )//0x0025
				wifi_response_substate++;
			break;
		case 29:
			if( SendTCPConnect( 3 ) )//0x0025
				wifi_response_substate++;
			break;
		case 30:
			if( SendGetMachineUUID() )//0x0015//for temp
			{
				wifi_response_substate ++;
			}
			break;
		case 31:
		  	WiFi_memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.DAPILocation.DapiVersion = WIFI_DAPI_VERSION;					
			WiFi_memcpy( wifiResponse.data.DAPILocation.macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
			WiFi_memcpy( wifiResponse.data.DAPILocation.machineIdentification,DEFAULT_UUID, _WIFI_Length_MachineID );//machineConfig->machineIdentification
			InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_GET_DAPI_LOCATION, transactionId, sizeof(WIFIDAPILocation) );
			wifi_response_substate ++;
		        break;
		case 32:
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
						wifi_response_substate++;
		        break;
		case 33:
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ) , 0 ) )
                        {                           
                            pre_http_command = wifi_response_state[wifi_response_index].state;
                            NextWiFiState();
                        }
  		        break;
		}
		break;

	case WCS_INIT_WIFI://used by init wifi
		switch( wifi_response_substate )
		{
		case 0:
		  {
			MachineInfor machineinfo;
			WiFi_memset( &machineinfo, 0x20, sizeof( MachineInfor ) );
			machineinfo.Brand = machineConfig->brandid;
			machineinfo.CSafeID = machineConfig->csafeId;
			machineinfo.CSafeManu = machineConfig->csafeManufactureId;
			machineinfo.DriveType = machineConfig->driveType;
			machineinfo.ModeType = machineConfig->modelType;
			machineinfo.MachineType = machineConfig->machineType;
			machineinfo.CSafeModel = machineConfig->csafeModelId;
			WiFi_memcpy( machineinfo.FrameNumber, machineConfig->frameSerialNumber, _WIFI_Length_SerialNumber );//15 // 2015.05.28
			WiFi_memcpy( machineinfo.SerialNumber, machineConfig->serialNumber, _WIFI_Length_SerialNumber );//15 // 2015.05.28
			if( SendSetMachineInfo( (char*)&machineinfo, sizeof( MachineInfor )))
				wifi_response_substate++;
		  }
			break;
		case 1:
			if( SendGetWifiVersion() )
				wifi_response_substate++;
			break;
		case 2:
			if( SendGetUartVersion() )//0x0004
				wifi_response_substate++;
			break;
		case 3://0x0031
			if( SendGetMacAddress() )//0x0034
				wifi_response_substate++;
			break;
		case 4:
			if( SendGetSecurityType_AP() )//0x0035
				wifi_response_substate++;
			break;
		case 5:
			if( SendGetSecurityKey_AP() )//0x0033
				wifi_response_substate++;
			break;
		case 6:
			if( SendGetSSID_AP() )//0x0033
				wifi_response_substate++;
			break;
		case 7://0x0900
			NextWiFiState();
			break;
                default:
			NextWiFiState();
		        break;        
		}
		break;
	case WCS_CONNECT_TO_AP:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			WiFi_memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.Ack.messageType = WIFI_CONNECT_TO_AP;
			wifiResponse.data.Ack.error = WIFI_ACK_OK;
			InitMessage( WIFI_LINGO_MODULE_SETTINGS, WIFI_MODULE_SETTINGS_ACKNOWLEDGMENT, transactionId, sizeof(WIFIDataAck) );
			break;
		case 1:
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (CHAR*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
		case 2:
			 if( SendDisconnectAP() )//0x0007
				wifi_response_substate++;
			 break;
		case 3://0x0008
			 if( SendSetSSID( wifi_settings.security.ssid, strlen( wifi_settings.security.ssid ) + 1 ) )
				wifi_response_substate++;
			 break;
		case 4:
			 if( SendSetSecurityType( wifi_settings.security.security_type  ) )
				wifi_response_substate++;
			 break;
		case 5:
			 if( SendSetSecurityKey( wifi_settings.security.security_key, strlen( wifi_settings.security.security_key ) + 1 ) )
				wifi_response_substate++;
			 break;
		case 6:
			 if( SendSetApStaMode( 0x33 ) )//0x0800
				wifi_response_substate++;
			 break;
		case 7:
			 if( SendConnectAP() )//0x0006
				wifi_response_substate++;
			 break;
		case 8:
			 if( ResetWiFi() )//0x0900
				NextWiFiState();
			 break;
                default:
			 NextWiFiState();
		         break;         
		}
		break;
	case WCS_GET_DAPI_LOCATION:
          {
            switch( wifi_response_substate )
            {
            case 0:
              wifiResponse.data.DAPILocation.DapiVersion = WIFI_DAPI_VERSION;				
              WiFi_memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
              WiFi_memcpy( wifiResponse.data.DAPILocation.macAddress, wifi_settings.mac_address, 6 );
              WiFi_memcpy( wifiResponse.data.DAPILocation.machineIdentification,DEFAULT_UUID, 36 );//machineConfig->machineIdentification
              InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_GET_DAPI_LOCATION, transactionId, sizeof(WIFIDAPILocation) );
              wifi_response_substate ++;
              break;
            case 1:
              if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                wifi_response_substate++;
              break;
            case 2:
              if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ) , 0 ) )
              {                           
                pre_http_command = wifi_response_state[wifi_response_index].state;
                NextWiFiState();
              }
              break;
            default:
              NextWiFiState();
              break;        
            }
          }
        break;
                case WCS_RFID_ASSOCIATE:
                    switch( wifi_response_substate )
                    {
                       case 0:
                       {
                         //_userDataResponse.RfidAssociateflag = 2;
                         wifi_response_substate++;
                         WiFi_memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                         WiFi_memcpy(wifiResponse.data.rfidAssociateReq.externalUserID, _UserExternalID, _WIFI_Length_UserExternalID);
                         WiFi_memcpy(wifiResponse.data.rfidAssociateReq.MachineID , machineConfig->machineIdentification , _WIFI_Length_MachineID);
                         WiFi_memcpy(wifiResponse.data.rfidAssociateReq.macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
                         wifiResponse.data.rfidAssociateReq.userID = rfiddata.userIndex;
                         WiFi_memcpy(wifiResponse.data.rfidAssociateReq.RFIDTag, rfiddata.rfidTag , 16);
                         wifiResponse.data.rfidAssociateReq.RFIDCarrier = rfiddata.rfidCarrier;
                         InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_RFID_ASSOCIATE, wifi_response_state[wifi_response_index].command,sizeof(WIFIRFidAssociateReq) );
                       }
                         break;
                     case 1://0x0500
                         if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                             wifi_response_substate++;
                         break;
                     case 2:
                         if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
                         {                           
                           pre_http_command = wifi_response_state[wifi_response_index].state;
                           NextWiFiState();
                         }
                         break;
                    }
                    break;

                case WCS_RFID_LOGIN:
                     switch(wifi_response_substate)
                     {
                         case 0:
                           {
                             //_userDataResponse.RfidLoginflag = 2;
                             wifi_response_substate++;
                             WiFi_memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                             WiFi_memcpy(wifiResponse.data.rfidLoginReq.MachineID , machineConfig->machineIdentification , 36);
                             WiFi_memcpy(wifiResponse.data.rfidLoginReq.macAddress, wifi_settings.mac_address, 6 );
                             wifiResponse.data.rfidLoginReq.userID = rfiddata.userIndex;
                             WiFi_memcpy(wifiResponse.data.rfidLoginReq.RFIDTag, rfiddata.rfidTag , RFTagLength);
                             wifiResponse.data.rfidLoginReq.RFIDCarrier = rfiddata.rfidCarrier;
                             InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_RFID_LOGIN, wifi_response_state[wifi_response_index].command,sizeof(WIFIRFidLoginReq) );
                           }
                             break;
                         case 1://0x0500
                             if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                                 wifi_response_substate++;
                             break;
                        case 2:
                             if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
                             {                           
                               pre_http_command = wifi_response_state[wifi_response_index].state;
                               NextWiFiState();
                             }
                             break;
                    }
                    break;
	case WCS_POST_WORKOUT_DATA:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			WiFi_memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			GetWorkoutData( &wifiResponse.data.WorkoutData_EX, _workoutStartTime);//Workout_Data

			InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_DATA_EX, wifi_response_state[wifi_response_index].command,
				sizeof(WIFIWorkoutDataEx) );
			break;
		case 1://0x0500
			if( SetHTTPData( (CHAR*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
			{
				workoutTrackingDataCount++;
				wifi_response_substate++;
			}
			break;
		case 2://every  count times
			if( workoutTrackingDataCount >= 20) // 4095 / sizeof(WIFIWorkoutDataEx) - 1 )//---101byte--  20150901
			{//0x0502
				if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ) ,DapiInfo.Secure ) )
				{
					wifi_response_substate++;
					workoutTrackingDataCount = 0;
                                        pre_http_command = WCS_IDLE;
				}
			}
			else
				wifi_response_substate++;
			break;
		case 3:
                default:  
			NextWiFiState();
			break;
		}
		break;
	case WCS_ACK:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			WiFi_memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.Ack.messageType = wifi_response_state[wifi_response_index].command & 0xff;
			wifiResponse.data.Ack.error = wifi_response_state[wifi_response_index].error;
			InitMessage( ( wifi_response_state[wifi_response_index].command >> 8 ) & 0xff, WIFI_GENERAL_ACKNOWLEDGMENT, transactionId, sizeof(WIFIDataAck) );
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (CHAR*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
		case 2:
                default:  
			NextWiFiState();
			break;
		}
		break;
	case WCS_GET_REMOTE_DATA:
		switch( wifi_response_substate )
		{
		case 0:
		case 1:
		case 2:
			if( wifi_settings.status.tcp_connected[wifi_response_substate + 1] == 0 )
				wifi_response_substate++;
			else if( SendGetTCPData( wifi_response_substate + 1 ) )
				wifi_response_substate++;
			break;
		case 3:
			if( GetHTTPResponseData() )//0x0503
				wifi_response_substate++;
			break;
		case 4:
		default:
			NextWiFiState();
			break;
		}
		break;
        
	case WCS_GET_STATUS:
		switch( wifi_response_substate )
		{
		case 0:
			if( SendGetStatus() )//0x0003
				wifi_response_substate++;
			break;
		case 1:
			NextWiFiState();
			break;
		}

		break;
        case WCS_AM_ENABLE:
                switch( wifi_response_substate )
		{
                case 0:
                        WiFi_memcpy( wifiResponse.data.ServiceAM.machineIdentification, machineConfig->machineIdentification, 36 );
                        WiFi_memcpy( wifiResponse.data.ServiceAM.MacAddress, wifi_settings.mac_address, 6 );
                        wifiResponse.data.ServiceAM.isAMsystem = machineConfig->isAMsystem;
			InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_ASSET_MANAGEMENT_ENABLED, transactionId, sizeof(WIFIServiceAM));
                        wifi_response_substate++;
                        break;
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ), DapiInfo.Secure ) )
                        {                           
                          pre_http_command = wifi_response_state[wifi_response_index].state;
                          NextWiFiState();
                        }
			break;
                default:
			NextWiFiState();
		        break;        
		}
		break;
        case WCS_SET_UPDATE_URL:
                switch(wifi_response_substate)
                {
                     case 0:  
                       _wifiStatus = WIFI_STATUS_DOWNLOADING_SOFTWARE;
                       _wifiEventHandle = WIFI_EVENT_DOWNLOADING_SOFTWARE;
                       //
                       getRemoteUpdate_Clear();
                       //
                       if(SendWifiCommand( WIFI_SET_REMOTE_UPDATE, RSCU.data,sizeof(RSCU.data) ))
                       {
                         WaitForWIFIWriteFlash = timer;
                         wifi_response_substate++;
                       }
                       break;
                      case 1:
                     switch(getRemoteUpdate_ReadyToDownload())
                            {
                         case 1:
                            _SendAdressBuffer[0] = 0x00;
                            _SendAdressBuffer[1] = 0x00;
                            _SendAdressBuffer[2] = 0x04;
                            _SendAdressBuffer[3] = 0x00;
                            _SendAdressBuffer[4] = 0x00;
                            _SendAdressBuffer[5] = 0x00;
                            _SendAdressBuffer[6] = 0x00;
                            _SendAdressBuffer[7] = 0x00;
                            _SendAdressBuffer[8] = 0xff;
                            _SendAdressBuffer[9] = 0xff;
                            _SendAdressBuffer[10] = 0x03;
                            _SendAdressBuffer[11] = 0x00;
                                  
                            if(SendWifiCommand(WiFi_UCBLCB_DownFirmware,&_SendAdressBuffer[0],12))
                                wifi_response_substate++;
                            break;
                         case 2:
                            WiFi_Interface.RSCU_Error = 1;
                            NextWiFiState();
                            break;
                         default:
                            if(timer - WaitForWIFIWriteFlash > 3000) // 30's
                            {// Time out
                                _wifiStatus = WIFI_STATUS_TIMEOUT;
                                _wifiEventHandle = WIFI_EVENT_TIMEOUT;
                                NextWiFiState();
                            }
                            break;
                     }
                     break;
                       case 2:            
                            _wifiStatus = WIFI_STATUS_DOWNLOAD_COMPLETED;
                            _wifiEventHandle = WIFI_EVENT_DOWNLOAD_COMPLETED;
                            NextWiFiState( );
                            //reboot
                            break;
                }
             break;
        case WCS_GET_LATEST_VERSION:
                switch( wifi_response_substate )
                {
                    case 0:
                        wifi_response_substate++;
                        WiFi_memset( &wifiResponse.data, 0, sizeof(WIFIDATAGetLatestVersion) );
                        wifiResponse.data.getlatestVersion.BrandID = machineConfig->brandid;
                        wifiResponse.data.getlatestVersion.ConsoleType = machineConfig->machineType;
                        WiFi_memcpy(wifiResponse.data.getlatestVersion.CurrentVersion, machineConfig->softwareVersion,20);
                        wifiResponse.data.getlatestVersion.Deployment = EEPROM_ReadInformation(EE_DAPI_Server);//remoteUpdateDeploymentType;
                        WiFi_memcpy(wifiResponse.data.getlatestVersion.MacAddress , wifi_settings.mac_address,6);
                        wifiResponse.data.getlatestVersion.MachineType = machineConfig->machineType;
                        WiFi_memcpy(wifiResponse.data.getlatestVersion.MachineUUid, machineConfig->machineIdentification,36);
                        wifiResponse.data.getlatestVersion.ModelType = machineConfig->modelType;
                        wifiResponse.data.getlatestVersion.ConsoleType = machineConfig->consoleType;
                        strcpy(wifiResponse.data.getlatestVersion.SoftwareType,softwaretype[remoteUpdateSoftwareType]);
                        InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_GET_LATEST_VERSION, transactionId, sizeof(WIFIDATAGetLatestVersion) );
                        break;
                    case 1:
                        if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                                    wifi_response_substate++;
                        break;
                    case 2:
                        if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) , DapiInfo.Secure) )
                        {                           
                          pre_http_command = wifi_response_state[wifi_response_index].state;
                          NextWiFiState();
                        }
                        break;
                   
                }
                break;
        
	case WCS_COMPLETE_WORKOUT:
		switch( wifi_response_substate )
		{
                case 0:                                             
                        wifi_response_substate++;
                        break;  
		case 1: //WIFI_WORKOUT_TRACKING_DATA_EX
			wifi_response_substate++;
			WiFi_memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );

                        WiFi_memcpy(&wifiResponse.data.WorkoutData_EX, &WorkdataTrack, sizeof(WIFIWorkoutDataEx));

                        WiFi_memcpy( wifiResponse.data.WorkoutData_EX.programUUID, machineConfig->programUUID, _WIFI_Length_ProgramUUID );	
                        wifi_response_index = (wifi_response_index == 0)? (_wifi_pending_responses_total - 1):(wifi_response_index-1);
			InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_DATA_EX, (wifi_response_state[wifi_response_index].command + 1), sizeof(WIFIWorkoutDataEx) );
                        wifi_response_index = (wifi_response_index == (_wifi_pending_responses_total - 1))? 0:(wifi_response_index+1);
			programT = wifiResponse.data.WorkoutData_EX.programType;
                        dataTransactionId = 0;
                        break;
		case 2://0x0500
		case 5:
                case 7:                  
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                          wifi_response_substate++;
			break;
		case 4: // WIFI_WORKOUT_TRACKING_SPRINT8_COMPLETE_EX, WIFI_WORKOUT_TRACKING_FITNESS_COMPLETE_EX
                        if(programT == SPRINT_8)
                        {
                            WiFi_memset( &wifiResponse.data, 0, sizeof(WIFIWorkoutDataSprint8CompleteEx) );
                            WiFi_memcpy( wifiResponse.data.WorkoutSprint8CompleteEx.macAddress, wifi_settings.mac_address, 6 );
                            WiFi_memcpy( wifiResponse.data.WorkoutSprint8CompleteEx.machineIdentification, machineConfig->machineIdentification, 36 );
                            WiFi_memcpy( wifiResponse.data.WorkoutSprint8CompleteEx.userExternalId, _UserExternalID, 36 );
                            wifiResponse.data.WorkoutSprint8CompleteEx.programType = programT;
                            WiFi_memcpy( wifiResponse.data.WorkoutSprint8CompleteEx.programUUID, machineConfig->programUUID, _WIFI_Length_ProgramUUID );
                            strcpy( wifiResponse.data.WorkoutSprint8CompleteEx.description, DEFAULT_DESCRIPTION);
                            
                            wifiResponse.data.WorkoutSprint8CompleteEx.sprint1SweatScoreX10 = 10;
                            wifiResponse.data.WorkoutSprint8CompleteEx.sprint2SweatScoreX10 = 10;
                            wifiResponse.data.WorkoutSprint8CompleteEx.sprint3SweatScoreX10 = 10;
                            wifiResponse.data.WorkoutSprint8CompleteEx.sprint4SweatScoreX10 = 10;
                            wifiResponse.data.WorkoutSprint8CompleteEx.sprint5SweatScoreX10 = 10;
                            wifiResponse.data.WorkoutSprint8CompleteEx.sprint6SweatScoreX10 = 10;
                            wifiResponse.data.WorkoutSprint8CompleteEx.sprint7SweatScoreX10 = 10;
                            wifiResponse.data.WorkoutSprint8CompleteEx.sprint8SweatScoreX10 = 10;
                            wifiResponse.data.WorkoutSprint8CompleteEx.totalSweatScoreX10 = 80;
                            wifiResponse.data.WorkoutSprint8CompleteEx.StartTime.year = wkDataComplete.StartTime.year;//year
                            wifiResponse.data.WorkoutSprint8CompleteEx.StartTime.month = wkDataComplete.StartTime.month;//month
                            wifiResponse.data.WorkoutSprint8CompleteEx.StartTime.day = wkDataComplete.StartTime.day;//day
                            wifiResponse.data.WorkoutSprint8CompleteEx.StartTime.hour = wkDataComplete.StartTime.hour;//hour
                            wifiResponse.data.WorkoutSprint8CompleteEx.StartTime.minute = wkDataComplete.StartTime.minute;//minute
                            wifiResponse.data.WorkoutSprint8CompleteEx.StartTime.second = wkDataComplete.StartTime.second;//second
                            InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_SPRINT8_COMPLETE_EX, transactionId, sizeof(WIFIWorkoutDataSprint8CompleteEx) );
                        }
                        else if(programT == FITNESS_TEST)
                        {
                            WiFi_memset( &wifiResponse.data, 0, sizeof(WIFIWorkoutDataFitnessTestCompleteEx) );
                            WiFi_memcpy( wifiResponse.data.WorkoutFitnessTestCompleteEx.macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
                            WiFi_memcpy( wifiResponse.data.WorkoutFitnessTestCompleteEx.machineIdentification, machineConfig->machineIdentification, _WIFI_Length_MachineID );
                            WiFi_memcpy( wifiResponse.data.WorkoutFitnessTestCompleteEx.userExternalId, _UserExternalID, _WIFI_Length_UserExternalID );
                            wifiResponse.data.WorkoutFitnessTestCompleteEx.programType = programT;
                            WiFi_memcpy( wifiResponse.data.WorkoutFitnessTestCompleteEx.programUUID, machineConfig->programUUID, _WIFI_Length_ProgramUUID );
                            strcpy( wifiResponse.data.WorkoutFitnessTestCompleteEx.description, DEFAULT_DESCRIPTION);
                            
                            wifiResponse.data.WorkoutFitnessTestCompleteEx.fitnessTestScore = WiFi_Information_Get(_WiFi_StepperFitTestScore);
                            wifiResponse.data.WorkoutFitnessTestCompleteEx.StartTime.year = wkDataComplete.StartTime.year;//year
                            wifiResponse.data.WorkoutFitnessTestCompleteEx.StartTime.month = wkDataComplete.StartTime.month;//month
                            wifiResponse.data.WorkoutFitnessTestCompleteEx.StartTime.day = wkDataComplete.StartTime.day;//day
                            wifiResponse.data.WorkoutFitnessTestCompleteEx.StartTime.hour = wkDataComplete.StartTime.hour;//hour
                            wifiResponse.data.WorkoutFitnessTestCompleteEx.StartTime.minute = wkDataComplete.StartTime.minute;//minute
                            wifiResponse.data.WorkoutFitnessTestCompleteEx.StartTime.second = wkDataComplete.StartTime.second;//second
                            InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_FITNESS_COMPLETE_EX, transactionId, sizeof(WIFIWorkoutDataFitnessTestCompleteEx) );
                        }
                        else
                          wifi_response_substate++;

                        _sentWorkoutData = true;
			wifi_response_substate++;
			break;
                case 6: // WIFI_WORKOUT_TRACKING_COMPLETE_EX
			WiFi_memset( &wifiResponse.data, 0, sizeof(WIFIWorkoutDataCompleteEx) );
                        WiFi_memcpy(&wifiResponse.data.WorkoutTrackingDataComplete_EX,&wkDataComplete,sizeof(WIFIWorkoutDataCompleteEx));
                        WiFi_memcpy( wifiResponse.data.WorkoutTrackingDataComplete_EX.machineIdentification, machineConfig->machineIdentification, 36 );
                        WiFi_memcpy( wifiResponse.data.WorkoutTrackingDataComplete_EX.programUUID, machineConfig->programUUID, 36 );
                        strcpy( wifiResponse.data.WorkoutTrackingDataComplete_EX.description, DEFAULT_DESCRIPTION);
                        wifiResponse.data.WorkoutTrackingDataComplete_EX.programType = programT;
                        InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_COMPLETE_EX, transactionId, sizeof(WIFIWorkoutDataCompleteEx) );
                        
                        _sentWorkoutData = true;
			wifi_response_substate++;
			break;
                case 3:
		case 8: // 0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ), DapiInfo.Secure ) )
                        {
                          wifi_response_substate++;
                          pre_http_command = WCS_IDLE;                         
                        }
			break;
		case 9:                        
                       wifi_response_substate++;
                       break;
                case 10:        
                default:  
			NextWiFiState();
			break;
		}
		break;
        case WCS_SEND_HTTP:  // 20150901
                if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ), DapiInfo.Secure ) )
                {                           
                  pre_http_command = WCS_IDLE;
                  NextWiFiState();
                }
                break;
	case WCS_SET_MARIO_BOX:              
                switch(wifi_response_substate)
		{
                    case 0:
                          WiFi_memset( &postCommand, 0, sizeof(HTTP_Post_Command) );
                          postCommand.post = HTTP_POST;
                          postCommand.dataType = HTTP_APPLICATION_OCTET_STREAM;
                          WiFi_memcpy( postCommand.url, DapiInfo.Url, 256 );
                          sprintf(IpAndPort,"%s:%d",DapiInfo.Server, DapiInfo.Port );
                          if( SendSetWorkoutIP( (CHAR*)IpAndPort, strlen( IpAndPort ) + 1 ) )//0x0504	
                          {
                              wifi_response_substate ++;
                          }
                          break;
                    case 1:
                            RegisterMachine( );
                            NextWiFiState();
                        break;
                }
	        break;
	case WCS_SYNC_CONSOLE_INFO://used by WifiConnection_Status_Change
		switch( wifi_response_substate )
		{
		case 0:
				WiFi_memset( &wifiResponse.data.MachineInfo_EX, 0, sizeof( WIFIDataRequestMachineInfo_EX ) );
                                WiFi_memcpy( wifiResponse.data.MachineInfo_EX.machineIdentification, machineConfig->machineIdentification, _WIFI_Length_MachineID );
				WiFi_memcpy( wifiResponse.data.MachineInfo_EX.macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
				wifiResponse.data.MachineInfo_EX.brand = machineConfig->brandid;
				wifiResponse.data.MachineInfo_EX.machineType = machineConfig->machineType;
				wifiResponse.data.MachineInfo_EX.modelType = machineConfig->modelType;
				wifiResponse.data.MachineInfo_EX.driveType = machineConfig->driveType;
                                wifiResponse.data.MachineInfo_EX.consoleType = machineConfig->consoleType;
				wifiResponse.data.MachineInfo_EX.csafeManufacture = machineConfig->csafeManufactureId;
				wifiResponse.data.MachineInfo_EX.csafeId = machineConfig->csafeId;
				wifiResponse.data.MachineInfo_EX.csafeModelId = machineConfig->csafeModelId;
				WiFi_memcpy( wifiResponse.data.MachineInfo_EX.serialNumber, machineConfig->serialNumber, _WIFI_Length_SerialNumber );// 2015.05.28
				WiFi_memcpy( wifiResponse.data.MachineInfo_EX.frameSerialNumber, machineConfig->frameSerialNumber, _WIFI_Length_SerialNumber );// 2015.05.28
				wifiResponse.data.MachineInfo_EX.clubId = machineConfig->clubId;
				wifiResponse.data.MachineInfo_EX.minMPHX10 = machineConfig->minMPHx10;
				wifiResponse.data.MachineInfo_EX.maxMPHX10 = machineConfig->maxMPHx10;
				wifiResponse.data.MachineInfo_EX.minKPHX10 = machineConfig->minKPHx10;
				wifiResponse.data.MachineInfo_EX.maxKPHX10 = machineConfig->maxKPHx10;
				wifiResponse.data.MachineInfo_EX.minInclineX10 = machineConfig->minInclinex10;
				wifiResponse.data.MachineInfo_EX.maxInclineX10 = machineConfig->maxInclinex10;
				wifiResponse.data.MachineInfo_EX.minLevel = machineConfig->minLevel;
				wifiResponse.data.MachineInfo_EX.maxLevel = machineConfig->maxLevel;
				WiFi_memcpy( wifiResponse.data.MachineInfo_EX.softwareVersion, machineConfig->softwareVersion, _WIFI_Length_SoftwareVer );
				wifiResponse.data.MachineInfo_EX.DapiVersion = WIFI_DAPI_VERSION ; 
				wifiResponse.data.MachineInfo_EX.lcbVendor = 0;
				wifiResponse.data.MachineInfo_EX.lcbVersion = machineConfig->LcbVersion;
                                WiFi_memcpy( wifiResponse.data.MachineInfo_EX.osVersion, machineConfig->osVersion, _WIFI_Length_SoftwareVer );
				wifiResponse.data.MachineInfo_EX.numUsers = MAX_USERS;
				WiFi_memcpy( wifiResponse.data.MachineInfo_EX.ipAddress, wifi_settings.ip_address, _WIFI_Length_IPAddress );
                                
                        SHT1381_RealTime now;
                        HT1381_RealTime( &now );
                        SHT1381ToNormal(now, &(wifiResponse.data.MachineInfo_EX.lastChangeTime));

				WiFi_memset( wifiResponse.data.MachineInfo_EX.timeZone, 0, _WIFI_Length_TimeZone );
				WiFi_memcpy( wifiResponse.data.MachineInfo_EX.timeZone, machineConfig->timeZone, (_WIFI_Length_TimeZone-1) );
                                WiFi_memcpy( wifiResponse.data.MachineInfo_EX.lastChangetimeZone, machineConfig->timeZone, (_WIFI_Length_TimeZone-1) );
				wifiResponse.data.MachineInfo_EX.AutoLogin = machineConfig->Autologin;//temp
				InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_MACHINE_INFO_EX, transactionId, sizeof(WIFIDataRequestMachineInfo_EX) );
                        wifi_response_substate++;
			break;
		case 1: // Machine_Info_Ex 
                case 3: // AM
                        //0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		
                case 2: // AM
                        WiFi_memcpy( wifiResponse.data.ServiceAM.machineIdentification, machineConfig->machineIdentification, 36 );
                        WiFi_memcpy( wifiResponse.data.ServiceAM.MacAddress, wifi_settings.mac_address, 6 );
                        wifiResponse.data.ServiceAM.isAMsystem = machineConfig->isAMsystem;
			InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_ASSET_MANAGEMENT_ENABLED, transactionId, sizeof(WIFIServiceAM));
                        wifi_response_substate++;
                        break;
                case 4://0x0502
                        if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ) ,DapiInfo.Secure) )
                        {                           
                          pre_http_command = wifi_response_state[wifi_response_index].state;
                          NextWiFiState(); //--Next--
                        }
                        break;
		}
		
		break;
	case WCS_UPDATE_TIME:
		switch( wifi_response_substate )
		{
		case 0:
			WiFi_memset( &wifiResponse.data, 0, sizeof( WifiCommandData ) );
			WiFi_memcpy( wifiResponse.data.TimeRequest.timeZone, machineConfig->timeZone, 65 );
			InitMessage( WIFI_LINGO_NETWORK_TIME, WIFI_NETWORK_TIME_GET_TIME, transactionId, sizeof(WIFITimeRequestRegion) );
			wifi_response_substate++;
			break;
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ), DapiInfo.Secure) )
                        {
                          wifi_response_substate++;
                          pre_http_command = wifi_response_state[wifi_response_index].state;
                        }
			break;
		case 3:
                default:  
			NextWiFiState();
			break;
		}
		break;
	case WCS_UPDATE_USER_INFO:
                switch( wifi_response_substate )
                {
                    case 0:
                            wifi_response_substate++;
                            //ReadHT1381
                            wifiResponse.data.UserDataRequest.userConsoleId = 0;
                            WiFi_memcpy( wifiResponse.data.UserDataRequest.xid, userFlashConfig[0]->xid, XIDMAXLENGTH );
                            WiFi_memcpy( wifiResponse.data.UserDataRequest.passcode, userFlashConfig[0]->passcode, _XidPassWordLength );
                            WiFi_memcpy( wifiResponse.data.UserDataRequest.macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
                            WiFi_memcpy( wifiResponse.data.UserDataRequest.machineIdentification, machineConfig->machineIdentification, _WIFI_Length_MachineID );
                            wifiResponse.data.UserDataRequest.lastChangeTime.year = userFlashConfig[0]->lastUpdateYear;
                            wifiResponse.data.UserDataRequest.lastChangeTime.month = userFlashConfig[0]->lastUpdateMonth;
                            wifiResponse.data.UserDataRequest.lastChangeTime.day = userFlashConfig[0]->lastUpdateDay;
                            wifiResponse.data.UserDataRequest.lastChangeTime.hour = userFlashConfig[0]->lastUpdateHour;
                            wifiResponse.data.UserDataRequest.lastChangeTime.minute = userFlashConfig[0]->lastUpdateMinute;
                            wifiResponse.data.UserDataRequest.lastChangeTime.second = userFlashConfig[0]->lastUpdateSecond;
                            wifiResponse.data.UserDataRequest.gender = userFlashConfig[0]->gender;
                            wifiResponse.data.UserDataRequest.weightX10 = userFlashConfig[0]->weightX10;
                            wifiResponse.data.UserDataRequest.age = WiFi_Information_Get(_WiFi_UserAge);
                            wifiResponse.data.UserDataRequest.year = userFlashConfig[0]->birth_year;
                            wifiResponse.data.UserDataRequest.month = userFlashConfig[0]->birth_month;
                            wifiResponse.data.UserDataRequest.day = userFlashConfig[0]->birth_day;
                            wifiResponse.data.UserDataRequest.language = userFlashConfig[0]->language;
                            wifiResponse.data.UserDataRequest.WorkoutDataShare = userFlashConfig[0]->WorkoutDataShare;
                            wifiResponse.data.UserDataRequest.DisplayUnits = userFlashConfig[0]->DisplayUnits;
                            wifiResponse.data.UserDataRequest.AcceptedTerms = userFlashConfig[0]->AcceptedTerms;
                            WiFi_memcpy( wifiResponse.data.UserDataRequest.name, userFlashConfig[0]->name, 65 );
                            wifiResponse.data.UserDataRequest.UserSetup= userFlashConfig[0]->UserSetup;
                            InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_SYNC_USER_INFO, transactionId, sizeof(WIFIExternalUserRequestData) );
                            break;
                    case 1:
                            //0x0500
                            if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                                wifi_response_substate++;
                            break;
                    case 2:
                            //0x0502
			    if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ) ,DapiInfo.Secure) )
                            {                           
                              pre_http_command = wifi_response_state[wifi_response_index].state;
                              NextWiFiState();
                            }
                            break;
	        }
		break;
        
	case WCS_UPDATE_MACHINE_INFO:// AM system
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			WiFi_memset( &wifiResponse.data.MachineInfo_EX, 0, sizeof( WIFIDataRequestMachineInfo_EX ) );
			WiFi_memcpy( wifiResponse.data.MachineInfo_EX.machineIdentification, machineConfig->machineIdentification, _WIFI_Length_MachineID );
			WiFi_memcpy( wifiResponse.data.MachineInfo_EX.macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
			wifiResponse.data.MachineInfo_EX.brand = machineConfig->brandid;
			wifiResponse.data.MachineInfo_EX.machineType = machineConfig->machineType;
			wifiResponse.data.MachineInfo_EX.modelType = machineConfig->modelType;
			wifiResponse.data.MachineInfo_EX.driveType = machineConfig->driveType;
			wifiResponse.data.MachineInfo_EX.consoleType = machineConfig->consoleType;
                        wifiResponse.data.MachineInfo_EX.csafeManufacture = machineConfig->csafeManufactureId;
			wifiResponse.data.MachineInfo_EX.csafeId = machineConfig->csafeId;
			wifiResponse.data.MachineInfo_EX.csafeModelId = machineConfig->csafeModelId;
                        WiFi_memcpy( wifiResponse.data.MachineInfo_EX.serialNumber, machineConfig->serialNumber, _WIFI_Length_SerialNumber );// 2015.05.28
			WiFi_memcpy( wifiResponse.data.MachineInfo_EX.frameSerialNumber, machineConfig->frameSerialNumber, _WIFI_Length_SerialNumber );// 2015.05.28
			wifiResponse.data.MachineInfo_EX.clubId = machineConfig->clubId;
			wifiResponse.data.MachineInfo_EX.minMPHX10 = machineConfig->minMPHx10;
			wifiResponse.data.MachineInfo_EX.maxMPHX10 = machineConfig->maxMPHx10;
			wifiResponse.data.MachineInfo_EX.minKPHX10 = machineConfig->minKPHx10;
			wifiResponse.data.MachineInfo_EX.maxKPHX10 = machineConfig->maxKPHx10;
			wifiResponse.data.MachineInfo_EX.minInclineX10 = machineConfig->minInclinex10;
			wifiResponse.data.MachineInfo_EX.maxInclineX10 = machineConfig->maxInclinex10;
			wifiResponse.data.MachineInfo_EX.minLevel = machineConfig->minLevel;
			wifiResponse.data.MachineInfo_EX.maxLevel = machineConfig->maxLevel;
                        WiFi_memcpy( wifiResponse.data.MachineInfo_EX.softwareVersion, machineConfig->softwareVersion, _WIFI_Length_SoftwareVer );
			wifiResponse.data.MachineInfo_EX.DapiVersion = WIFI_DAPI_VERSION ; 
			wifiResponse.data.MachineInfo_EX.lcbVendor = 0;
			wifiResponse.data.MachineInfo_EX.lcbVersion = machineConfig->LcbVersion;
			WiFi_memcpy( wifiResponse.data.MachineInfo_EX.osVersion, machineConfig->osVersion, _WIFI_Length_SoftwareVer );
			wifiResponse.data.MachineInfo_EX.numUsers = MAX_USERS;
			WiFi_memcpy( wifiResponse.data.MachineInfo_EX.ipAddress, wifi_settings.ip_address, _WIFI_Length_IPAddress );
                        WiFi_memset( wifiResponse.data.MachineInfo_EX.timeZone, 0, _WIFI_Length_TimeZone );
			WiFi_memcpy( wifiResponse.data.MachineInfo_EX.timeZone, machineConfig->timeZone, (_WIFI_Length_TimeZone-1) );
                        wifiResponse.data.MachineInfo_EX.AutoLogin = machineConfig->Autologin;//temp
			                        
                        SHT1381_RealTime now;
                        HT1381_RealTime( &now );
                        SHT1381ToNormal(now, &(wifiResponse.data.MachineInfo_EX.lastChangeTime));
                        
                        WiFi_memcpy( wifiResponse.data.MachineInfo_EX.lastChangetimeZone, machineConfig->timeZone, (_WIFI_Length_TimeZone-1) );
			InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_MACHINE_INFO_EX, transactionId, sizeof(WIFIDataRequestMachineInfo_EX) );
			break;
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ), DapiInfo.Secure ) )
                        {                           
                          pre_http_command = wifi_response_state[wifi_response_index].state;
                          NextWiFiState();
                        }
			break;
                default:
			NextWiFiState();
		        break;        
		}
		break;
        
	case WCS_RESTORE_FACTORY:
		switch( wifi_response_substate )
		{
		case 0:
                        //machineConfig->wifiisAPMode = 1;
			wifi_response_substate++;
			WiFi_memcpy( wifiResponse.data.ClearUsers.macAddress, wifi_settings.mac_address, 6 );
			WiFi_memcpy( wifiResponse.data.ClearUsers.machineIdentification, machineConfig->machineIdentification, 36 );
			InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_CLEAR_USERS, transactionId, sizeof( WIFIClearUsers ) );
			break;
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ),DapiInfo.Secure ) )
                        {
                          wifi_response_substate++;
                          pre_http_command = wifi_response_state[wifi_response_index].state;
                        }
			break;
                case 3:
                        if(SendSetAP_SSIDTypeKey())
                                wifi_response_substate++;
                        break;
                
                case 4:    
                        if(timer - WaitForWIFIWriteFlash >= 200)
				{
			if( SendSetApStaMode( 0x11 ) )
				{				
					wifi_response_substate++;
                            WaitForWIFIWriteFlash = 0;
				}
			}
			break;
                case 5:     
			if( ResetWiFi() )
                        {
                            machineConfig->wifiisAPMode = 1;
			    WiFi_memset(&DapiInfo , 0 , sizeof( DAPI_ADDRESS_INFO ));
			    //SAVE_MACHINE_CONFIG_TO_FLASH;
			    init = false;
                            wifi_response_substate++;
                        }
			break;
                case 6:        
                default:
			NextWiFiState();
		        break;        
		}
		break;
        
	case WCS_LOGIN_USER:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			WiFi_memset( &wifiResponse.data.UserLoginRequest, 0, sizeof(WIFIUserLoginRequest) );
			WiFi_memcpy( wifiResponse.data.UserLoginRequest.xid, _loginRequest.xid, XIDMAXLENGTH );
			WiFi_memcpy( wifiResponse.data.UserLoginRequest.passcode, _loginRequest.passcode, _XidPassWordLength );
			WiFi_memcpy( wifiResponse.data.UserLoginRequest.macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
			WiFi_memcpy( wifiResponse.data.UserLoginRequest.MachineID, machineConfig->machineIdentification, _WIFI_Length_MachineID );
			wifiResponse.data.UserLoginRequest.userId = _loginRequest.userId;

			InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_LOGIN, transactionId, sizeof(WIFIUserLoginRequest) );
			break;
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ), DapiInfo.Secure ) )
                        {
                          wifi_response_substate++;
                          pre_http_command = wifi_response_state[wifi_response_index].state;       
                        }
			break;
		case 3:
                default:  
			NextWiFiState();
			break;
		}
		break;

	case WCS_ENABLE_AP:
		switch( wifi_response_substate )
		{
		case 0://0x0030
			if( SendSetSSID_AP( wifi_settings.ap_security.ssid, strlen( wifi_settings.ap_security.ssid ) + 1 ) )
				wifi_response_substate++;
			break;
		case 1:
			if( SendSetApStaMode( 0x55 ) )
				wifi_response_substate++;
			break;
		case 2:
                default:  
			NextWiFiState();
			break;
		}
		break;

	case WCS_DISABLE_AP:
		switch( wifi_response_substate )
		{
		case 0:
			if( SendSetApStaMode( 0x33 ) )
				wifi_response_substate++;
			break;
		case 2:
                default:  
			NextWiFiState();
			break;
		}
		break;
          // 2015.01.21
	case WCS_CONNECT_TO_ROUTER://connect to RD AP
		switch( wifi_response_substate )
		{
               
                case 0:
                        if(SendSetSSIDTypeKey(wifi_settings.security.ssid, wifi_settings.security.security_type, wifi_settings.security.security_key))
                          wifi_response_substate++;
                        break;
		case 1:
			if( WaitForWIFIWriteFlash < 500 )
			{
				WaitForWIFIWriteFlash ++;
			}
			else
			{
			if( SendSetApStaMode( 0x33 ) )
				{
					WaitForWIFIWriteFlash = 0;
				wifi_response_substate++;
				}			
			}
			break;
		case 2:
			if( SendConnectAP() )
				wifi_response_substate++;
			break;
		case 3:
                default:  
			NextWiFiState();
			break;
		}
		break;
	case WCS_DISCONNECT_TO_ROUTER://disconnect to RD AP
		switch( wifi_response_substate )
		{
		case 0:
			if( SendDisconnectAP() )
				wifi_response_substate++;
			break;
		case 1:
                default:  
			NextWiFiState();
			break;
		}
		break;
	case WCS_REGISTER_MACHINE:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
            WiFi_memcpy( wifiResponse.data.RegisterRequest.macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
			InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_MACHINE_REGISTRATION, transactionId, sizeof(WIFIMachineRegisterRequest) );
			break;
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ), DapiInfo.Secure ) )
                        {
                          wifi_response_substate++;
                          pre_http_command = wifi_response_state[wifi_response_index].state;       
                        }
			break;
		case 3:
                default:  
			NextWiFiState();
			break;
		}
		break;
	case WCS_GET_SIGNAL_STRENGTH:
		switch( wifi_response_substate )
		{
		case 0:
			if( SendGetSignalStrength() )//0x0014
				wifi_response_substate++;
			break;
		case 1:
                default:  
			NextWiFiState();
			break;
		}
		break;
        case WCS_UPDATE_MACHINE_STATS:
            switch( wifi_response_substate )
            {
                case 0:
                    WiFi_Set_MachineInformation();
                    wifi_response_substate++;
                    WiFi_memset( &wifiResponse.data.MachineStats, 0, sizeof( WIFIUpdateMachineStats ) );
                    WiFi_memcpy( wifiResponse.data.MachineStats.machineIdentification, machineConfig->machineIdentification, _WIFI_Length_MachineID );
                    WiFi_memcpy( wifiResponse.data.MachineStats.MacAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
                    wifiResponse.data.MachineStats.accumulatedDistanceMilesX100 = machineConfig->accumulatedDistanceMilesX100;
                    wifiResponse.data.MachineStats.accumulatedTime = machineConfig->accumulatedSeconds;
                    wifiResponse.data.MachineStats.headphoneInsertions = machineConfig->headphoneInsertion;
                    wifiResponse.data.MachineStats.headphoneRemovals = machineConfig->headphoneRemoval;
                    InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_UPDATE_MACHINE_STATE, transactionId, sizeof(WIFIUpdateMachineStats) );
                    break;
                case 1://0x0500
                    if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                        wifi_response_substate++;
                    break;
                case 2://0x0502
                    if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ), DapiInfo.Secure ) )
                    {                           
                      pre_http_command = wifi_response_state[wifi_response_index].state;
                      NextWiFiState();
                    }
                    break;
                default:
                    NextWiFiState();
                    break;        
            }
            break;
        case WCS_OCCURRED_ERROR_CODE:
            switch( wifi_response_substate )
            {
                case 0:
                    wifi_response_substate++;
                    WiFi_memset( &wifiResponse.data.ErrorCodeOccurred, 0, sizeof( WIFIErrorCodeOccurred ) );
                    WiFi_memcpy( wifiResponse.data.ErrorCodeOccurred.machineIdentification, machineConfig->machineIdentification, _WIFI_Length_MachineID );
                    WiFi_memcpy( wifiResponse.data.ErrorCodeOccurred.MacAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
                    wifiResponse.data.ErrorCodeOccurred.errorCode = machineConfig->ErrorCode;

                    SHT1381_RealTime now;
                    HT1381_RealTime( &now );
                    SHT1381ToNormal(now, &(wifiResponse.data.ErrorCodeOccurred.OccurredTime));

                    InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_ERROR_CODE_OCCURRED, transactionId, sizeof(WIFIErrorCodeOccurred) );
                    break;
                case 1://0x0500
                    if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                            wifi_response_substate++;
                    break;
                case 2://0x0502
                    if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)postCommand.url, strlen( postCommand.url ), DapiInfo.Secure ) )
                    {                           
                      pre_http_command = wifi_response_state[wifi_response_index].state;
                      NextWiFiState();
                    }
                    break;
                default:
                    NextWiFiState();
                    break;        
            }
            break;        
        case WCS_DOWNLOADSOFTWARE:
        
            break;
        default:
            NextWiFiState();  
            break;
	}
}

void ProcessTCPData( int port, char* data, int data_size )
{
	UINT16 readSize = 0;

	if( port < 1 || port > 3 )
		return;
	while( data_size > 0 || tcpDataBufferIndex[port - 1] >= sizeof(WifiHeader) )
	{
		// Add data to buffer.
		if( data_size > 0 )
		{
			readSize = AddTCPDataToBuffer( port - 1, data, data_size );
			data_size -= readSize;
			data += readSize;
			tcpDataBufferIndex[port - 1] += readSize;
		}

		// Find sync word.
		for( int i = 0; i < tcpDataBufferIndex[port - 1] - 1; i++ )
		{
			WifiHeader* header = (WifiHeader*)( tcpDataBuffer[port - 1] + i );
			if( header->syncWord == 0xAA55 )
			{
				if( i != 0 )
				{
					for( int j = 0; j < i && i + j < tcpDataBufferIndex[port - 1]; i++ )
						tcpDataBuffer[port - 1][i] = tcpDataBuffer[port - 1][i + j];
					tcpDataBufferIndex[port - 1] -= i;
				}
				break;
			}
		}

		if( tcpDataBufferIndex[port - 1] >= sizeof(WifiHeader) )
		{
			WifiCommand* command = (WifiCommand*)tcpDataBuffer[port - 1];
			int messageSize = command->header.dataSize + sizeof(WifiHeader);

			if( command->header.dataSize + sizeof(WifiHeader) <= tcpDataBufferIndex[port - 1] )
			{
				transactionId = command->header.transactionId;
				ProcessExternalMessage( port, command );
				for( int i = 0; i < messageSize && i + messageSize < tcpDataBufferIndex[port - 1]; i++ )
					tcpDataBuffer[port - 1][i] = tcpDataBuffer[port - 1][i + messageSize];
				tcpDataBufferIndex[port - 1] -= messageSize;
			}
			else
				break;
		}
	}
}

void UpdateNetworkTime()
{
	AddWifiResponseState( WCS_UPDATE_TIME, 0, 0, 0 );
}

UINT8 CheckUserInfoUpdated()
{
	return _userInfoUpdated;
}
// AM system

void UpdateMachineInfo()
{
    WiFi_Set_MachineInformation();
    AddWifiResponseState( WCS_UPDATE_MACHINE_INFO, 0, 0, 0 );
}
void UpdateAMEnable(void)
{
        WiFi_Set_MachineInformation();
	AddWifiResponseState( WCS_AM_ENABLE, 0, 0, 0 );
}

void Login( unsigned char userId,  char* xid,  char* passcode )// 2014.06.17  unsigned  unsigned
{
	if( wifi_settings.status.wifi_connected && xid[0] != 0 )
	{
		WiFi_memcpy( _loginRequest.xid, xid, XIDMAXLENGTH );
        WiFi_memcpy( _loginRequest.passcode, passcode, _XidPassWordLength );
		_loginRequest.userId = userId;
                _userDataResponse.loginCheckResponse = 0;
                _userDataResponse.RfidLoginflag = 0;
		AddWifiResponseState( WCS_LOGIN_USER, 0, 0, 0 );
	}
}

UINT8 CheckLoginResult()
{
/* This function returns the result of the login.  
 * A 0 means it is still processing.
 * A 1 means success and anything else is an error.*/
	return _userDataResponse.loginCheckResponse;
}

void ChangeWorkoutState( WORKOUT_STATE state )
{
  char L = userFlashConfig[0]->LoggedOn;
  if(L == 1)
  {
      if( ( _workoutState==WIFI_WORKOUT_PAUSED||_workoutState == WIFI_WORKOUT_IDLE || _workoutState == WIFI_WORKOUT_STOP ) && state == WIFI_WORKOUT_RUNNING )
      {
            HT1381_RealTime( &_workoutStartTime );
          
          WiFi_memset( &wkDataComplete , 0 , sizeof(WIFIWorkoutDataCompleteEx) );            
          WiFi_memset( &WorkdataTrack, 0 , sizeof(WIFIWorkoutDataEx) );
            
            wifi_settings.isInWorkout = true;
      }
      else if( ( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED ) && state == WIFI_WORKOUT_STOP )
      {
            GetWorkoutData( &WorkdataTrack, _workoutStartTime);
            GetWorkoutDataComplete( &wkDataComplete, _workoutStartTime);
            //
          AddWifiResponseState( WCS_COMPLETE_WORKOUT, 0, 0, 0 );
          
          userFlashConfig[0]->LoggedOn = 0;
          _userDataResponse.loginCheckResponse = 0;
          _userDataResponse.RfidAssociateflag = 0;
          _userDataResponse.RfidLoginflag = 0;
            // dataTransactionId = 0; // 20151126
            wifi_settings.isInWorkout = false;
      }
  }
  _workoutState = state;
}

void EnableAP( char* ssid, UINT16 ssid_length )
{
	WiFi_memset( wifi_settings.ap_security.ssid, 0, 33 );
	if( ssid_length > 33 )
		strncpy( wifi_settings.ap_security.ssid, ssid, 33 );
	else
		strncpy( wifi_settings.ap_security.ssid, ssid, ssid_length );
	AddWifiResponseState( WCS_ENABLE_AP, 0, 0, 0 );
}

void DisableAP()
{
	AddWifiResponseState( WCS_DISABLE_AP, 0, 0, 0 );
}
// òà“t?O?¨?MDD?B?�
//Set WI-FI Security Type in STA mode  (DataT = DataR)
//  0: None
//  1: WEP_40Bits
//  2: WEP_104Bits
//  3: WPA_PHRASE
//  4: WPA2_PHRASE
//  5: WPA_AUTO_PHRASE
//  6: Auto Detect Security
void ConnectWIFI(char* ssid, UINT16 ssid_length, char* security_key, UINT16 security_key_length, UINT8 security_type )
{
  WiFi_memset( wifi_settings.security.security_key, 0, 255 );
  if( security_key_length > 255 )
      strncpy( wifi_settings.security.security_key, security_key, 255 );
  else
      strncpy( wifi_settings.security.security_key, security_key, security_key_length );
  //
  WiFi_memset( wifi_settings.security.ssid, 0, 33 );
  if( ssid_length > 33 )
      strncpy( wifi_settings.security.ssid, ssid, 33 );
  else
      strncpy( wifi_settings.security.ssid, ssid, ssid_length );
  //
  wifi_settings.security.security_type = Default_Type; 
  // 2014.10.13
  // ?”?B?Y JHT HQ?rDèòa???é WIFI_SECURITY_AUTO_DETECT
  //wifi_settings.security.security_type = WIFI_SECURITY_AUTO_DETECT;
  ConnectWiFi_AP();
}

// 將WiFi模組還原預設連線AP rd02
void ConnectAPForFactoryDefault( void )
{// SSID=rd02
    ConnectWIFI(Default_SSID,strlen(Default_SSID),Default_Key,strlen(Default_Key), Default_Type);
    EEPROM_WriteInformation(EE_WiFi_SSID,0);
    EEPROM_WriteInformation(EE_WiFi_Password,0);
    EEPROM_WriteInformation(EE_WiFi_Security,wifi_settings.security.security_type);
}
// ?B?�WiFi APó?
void ConnectWiFi_AP(void)
{
  AddWifiResponseState( WCS_CONNECT_TO_ROUTER, 0, 0, 0 ); 
}
void DisconnectWIFI()
{
  AddWifiResponseState( WCS_DISCONNECT_TO_ROUTER, 0, 0, 0 );
}
//

void RegisterMachine()
{
  WiFi_Set_MachineInformation();
  AddWifiResponseState( WCS_REGISTER_MACHINE, 0, 0, 0 );
}

void RestoreWifiToFactoryDefaults()
{
  // 2015.01.21
  WIFIClearStateBuffer();
  //
  AddWifiResponseState( WCS_RESTORE_FACTORY, 0, 0, 0 );
}

UINT8 WIFIIsOnline(void)
{
    if(wifi_settings.status.wifi_connected == 1 && DapiInfo.Url[0] != 0 && DapiInfo.Url[0] != 0xFF)
        return 1; 
    else
        return 0;
}
UINT8 WIFIIsAPMode(void)
{
  if(WIFIIsOnline( ))
  {
      machineConfig->wifiisAPMode = 0;
      return machineConfig->wifiisAPMode;
  }
  else
      return (machineConfig->wifiisAPMode==1);
}

UINT8 WIFISignalStrength(void)
{
  if(WIFIIsOnline( ))
  {
      if(wifi_settings.status.wifi_connected == 1)
      {
//          external_data_timer = 1;
          if(wifi_settings.ConnectedStrength == 0)
              return 4;
          else
              return (wifi_settings.ConnectedStrength);
      }
      else
      {
          //WiFi_memset( &DapiInfo , 0, sizeof(DAPI_ADDRESS_INFO) );
	  init = false;
          return 0;
      }
  }
  else
  {
       return 0;
  }
}

// WiFi 專用記憶體副程式
void WiFi_memset(void *s, char c, long n)
{       /* store c throughout unsigned char s[n] */
  const unsigned char uc = c;
  unsigned char *su = (unsigned char *)s;

  for (; 0 < n; ++su, --n)
  *su = uc;
  return ;
}
void WiFi_memcpy(void *s1, const void *s2, long n)
{       /* copy char s2[n] to s1[n] in any order */
  char *su1 = (char *)s1;
  const char *su2 = (const char *)s2;
  
  for(; 0 < n; ++su1, ++su2, --n)
      *su1 = *su2;
  return ;
}
char WiFi_memcmp(const void *s1, const void *s2, long n)
{       /* compare unsigned char s1[n], s2[n] */
  const unsigned char *su1 = (const unsigned char *)s1;
  const unsigned char *su2 = (const unsigned char *)s2;
  
  for (; 0 < n; ++su1, ++su2, --n)
    if (*su1 != *su2)
      return (*su1 < *su2 ? -1 : +1);
  return (0);
}
long WiFi_CountChar(const void *s1, long MaxLength)
{       /* Count the length of s1[n] */
  const unsigned char *su1 = (const unsigned char *)s1;
  
  for (long i=0; ;i++)
  {
      if(su1[i] == 0)
      {
          return (i >= MaxLength)?MaxLength:i;
      }
  }
}
// 2015.01.21
void WIFIClearStateBuffer(void)
{
  WiFi_memset(wifi_response_state,0x00,sizeof(WifiResponseState)*10);
  wifi_response_index=0;
  wifi_next_response_index = 0;
  wifi_response_substate=0;
  wifi_num_pending_responses = 0;
  CanLoginUserFlag = false;//disable main login
}
void WiFi_Interface_ClearUserExternalID(void)
{
    WiFi_memset( _UserExternalID, 0, _WIFI_Length_UserExternalID );
}


void wifiSetRFidLogin(UINT8 userIndex, char* rfidTag, RF_Standard rfidCarrier)  // RFIDCARRIER rfidCarrier
{
    if( rfidTag != 0 )
    {
        rfiddata.userIndex = userIndex;
        WiFi_memcpy(rfiddata.rfidTag,rfidTag,RFTagLength);
        rfiddata.rfidCarrier = rfidCarrier;
        
        if((_userDataResponse.loginCheckResponse == 1) && (_userDataResponse.RfidAssociateflag == 0))
          AddWifiResponseState(WCS_RFID_ASSOCIATE, 0, 0, 0 );
        else
        {
          _userDataResponse.RfidLoginflag = 0;
          AddWifiResponseState( WCS_RFID_LOGIN, 0, 0, 0); 
        }
    }
}
UINT8 CheckRFIDLoginResult()
{
/* This function returns the result of the login. 
 * A 0 means it is still processing.
 * A 1 means success and anything else is an error.*/
	return _userDataResponse.RfidLoginflag;
}

UINT8 CheckRFIDAssociate()
{
	return _userDataResponse.RfidAssociateflag;
}

void SetRFIDAssociate(char AF)
{
	_userDataResponse.RfidAssociateflag = AF;
}
void wifi_interface_ClearLoginFlag(void)
{
  _userDataResponse.RfidAssociateflag=0;
  _userDataResponse.RfidLoginflag=0;  
  _userDataResponse.loginCheckResponse=0;
    //
    userFlashConfig[0]->LoggedOn = 0;
    wifi_settings.isInWorkout = false;
    wifi_settings.isInRSCU = false;
}

/*******************************************************************************
* Function Name  : WiFi_RSCU_Process()
* Description    : RSCU handles.
* Input          : _RSCU_Reset, _RSCU_Update
* Output         : _RSCU_CanUpdate, _RSCU_NoLatestSoftware, ...
*******************************************************************************/                                         
unsigned char WiFi_RSCU_Process(unsigned char by_Mode)
{
    static union
    {
        struct
        {
            unsigned long LL:8 ;
            unsigned long LH:8 ;
            unsigned long HL:8 ;
            unsigned long HH:8 ;
        }DT ;
        unsigned long All ;
    } Temp;
    static unsigned char _CheckCounter=0;
    static unsigned long PragramIndex_temp;
    static unsigned char RSCU_RunStep = 0;
    static unsigned char RSCU_CurrentState = _RSCU_Begin;
    char _SendBuffer[6];
    static union 
    {
        struct
        {
            unsigned char GoToNext : 1;
            unsigned char Retry : 1;
            unsigned char ProcessEnd : 1;
            unsigned char CheckBoot_OK : 1;
            unsigned char CheckSoftware_OK : 1;
        }bit;
        unsigned char all;
    }RSCU_Status = {0};
  
    // Function off
    if(WiFi_Interface.CloseRSCU == 1)
        return _RSCU_Pause;
    
    switch(by_Mode)
    {
        case _RSCU_Reset:
            RSCU_Status.all = 0;
            RSCU_RunStep = 0;
            RSCU_CurrentState = _RSCU_Begin;
            WiFi_Timer_Count = 0;
            _CheckCounter = 0;
        case _RSCU_State:    
            return RSCU_CurrentState;
        case _RSCU_Update:    
        default:
            break;
    }

    if(RSCU_Status.bit.ProcessEnd == 0)
    {
        wifi_settings.isInRSCU = true;
        // Time out
        if(!WIFICanLoginUser())//if Machined Id is default,cause error
        {
            RSCU_CurrentState = _RSCU_Disconnection;
            RSCU_RunStep = 0xFE;
        }
        else if(WiFi_Timer_Count >= 50) // 5's
        {
            if(_CheckCounter >= 12) // 5*12 = 60 seconds
            {// Error
                RSCU_CurrentState = _RSCU_CanNotUpdate;
                RSCU_RunStep = 0xFE;
            }
            else
            {
                _CheckCounter++;
                WiFi_Timer_Count = 0;
                RSCU_Status.bit.Retry = 1;
            }
        }
        switch(RSCU_RunStep)
        {
            case 0: // Start
            WiFi_memset(RSCU.data,0,sizeof(RSCU));
                SoftwareToEEPROMPercent = 0;
            //
                WiFi_Interface.GetLatestVersionStatus=false;
                WiFi_Interface.GetLatestVersionError=false;
                WiFi_Interface.HaveTheLatestSoftware = 0;
                WiFi_Interface.RSCU_Error = 0;
            //
            wifi_settings.status.ucb_updating = false;
                WiFi_BinGetFromModulle = false;
            //
                AddWifiResponseState( WCS_GET_LATEST_VERSION, 0, 0, 0 );
                RSCU_CurrentState = _RSCU_CheckForSoftwareOnDAPI;
                
                // Next
                RSCU_Status.bit.GoToNext = 1;
                break;
            case 1: // Check whether have the latest software on DAPI or not
                if(WiFi_Interface.GetLatestVersionError == true)
                {// Error
                    RSCU_CurrentState = _RSCU_CanNotUpdate;
                    RSCU_RunStep = 0xFE;
                }
                else if(WiFi_Interface.GetLatestVersionStatus == true)
                {
                  
                    if(WiFi_Interface.HaveTheLatestSoftware == 0)
                    {// Error
                        RSCU_CurrentState = _RSCU_NoLatestSoftware;
                        RSCU_RunStep = 0xFE;
                    }
                    else
                    {// Have the latest software. Start to Download software from server to wifi module. 
                        if(EEPROM_ReadInformation(EE_RSCU_SoftwareVersion) != RSCU_SoftwareVer.all)
//                        if(WiFi_CheckRSCUVersion(RSCU_SoftwareVer.byte))
                        {// The software is not the same as EEPROM
                          
                          wifi_settings.status.ucb_updating = false;
                          AddWifiResponseState( WCS_SET_UPDATE_URL, 0, 0, 0 );
                          getRemoteUpdate_Clear();
                          SoftwareToEEPROMPercent = 0;
                          RSCU_CurrentState = _RSCU_DownloadSoftwareToModule;
                          
                          // Next
                          RSCU_Status.bit.GoToNext = 1;
                          
                        }
                        else
                        {// can update directly.
                            SoftwareToEEPROMPercent = 100;
                            RSCU_RunStep = 0xFF; // end
                        }
                    }
                  
                }
                break;
            case 2: // check whether complete software download from server to WiFi module or not.
                if(WiFi_Interface.RSCU_Error == 1)
                {
                    RSCU_CurrentState = _RSCU_CanNotUpdate;
                    RSCU_RunStep = 0xFE;
                }
                else if(RSCU_Status.bit.Retry == 1)
                {
                    if(SendWifiCommand(WiFi_UCBLCB_GetProgrammingStatus,0,0))
                    {
                        RSCU_Status.bit.Retry = 0;
                        WiFi_Timer_Count = 0;
                    }
                }
                else
                {
                    switch(getRemoteUpdate_DownloadFinish())
                    {
                        case 1:
                            RSCU_CurrentState = _RSCU_CheckForSoftwareOnModule;
                            // Next
                            RSCU_Status.bit.GoToNext = 1;
                            WiFi_BinGetFromModulle = false;
                            break;
                        case 2:
                            RSCU_CurrentState = _RSCU_CanNotUpdate;
                            RSCU_RunStep = 0xFE;
                            break;
                        default:
                            break;
                    }
                }
                break;
            case 3: // Check whether software in the wifi module is correct or not.
                if(RSCU_Status.bit.Retry == 1)
                {
                    if(_CheckCounter >= 5)
                    {
                        RSCU_CurrentState = _RSCU_BinCheckError;
                        RSCU_RunStep = 0xFE; //
                    }
                    else
                    {
                        if(RSCU_Status.bit.CheckSoftware_OK == 0)
                            Temp.All = _Info_ManufactureIndex_ ; // Bin file check data address
                        else if(RSCU_Status.bit.CheckBoot_OK == 0)
                            Temp.All = (unsigned long)Boot_Version & 0x000FFFFF ; // Boot version address
                        
                        _SendBuffer[0] = 0;
                        _SendBuffer[1] = 1;
                        _SendBuffer[2] = Temp.DT.LL ;
                        _SendBuffer[3] = Temp.DT.LH ;
                        _SendBuffer[4] = Temp.DT.HL ;
                        _SendBuffer[5] = Temp.DT.HH ;
                        if(SendWifiCommand(WiFi_UCB_ProgrammingData,&_SendBuffer[0],6))
                        {
                            
                            WiFi_Timer_Count = 0;
                            RSCU_Status.bit.Retry = 0;
                        }
                    }
                }
                else if(WiFi_BinGetFromModulle == true)
                {
                    WiFi_BinGetFromModulle = false;
                    if(RSCU_Status.bit.CheckSoftware_OK == 0)
                    {
                        if( WiFiBoot_CheckUCBInfo((unsigned char*)&Boot_Manufacture[0],&ProgramData[0],52) == 1 )
                        if( WiFiBoot_CheckUCBInfo((unsigned char*)&Boot_McuType[0],&ProgramData[52],20) == 1 )            
                        if( WiFiBoot_CheckUCBInfo((unsigned char*)&Boot_ModuleName[0],&ProgramData[72],20) == 1 )   
                        {
                            RSCU_Status.bit.CheckSoftware_OK = 1;
                            WiFi_Timer_Count = 0;
                            RSCU_Status.bit.Retry = 1;
                            break;
                        }
                    }
                    else if(RSCU_Status.bit.CheckBoot_OK == 0)
                    {
                        if( WiFiBoot_CheckUCBInfo((unsigned char*)&Boot_Version[0],&ProgramData[0],20) == 1)
                            RSCU_Status.bit.CheckBoot_OK = 1;
                    }
                    
                    if(RSCU_Status.bit.CheckSoftware_OK == 1 && RSCU_Status.bit.CheckBoot_OK == 1)
                    {
                        Mx25L1606E_MajoritySectorErase(_Bin_Block_, _Bin_Size_);
                        EEPROM_WriteInformation(EE_RSCU_SoftwareVersion, 0); // clear
                        Temp.All = 0;
                        PragramIndex_temp = 0;
                        ProgramDataIndex = 0;
                        WiFi_BinGetFromModulle = false;
                        RSCU_CurrentState = _RSCU_DownloadSoftwareToEEPROM;                                
                        // Next
                        RSCU_Status.bit.GoToNext = 1;
                        //
                        break;
                    }
                    else
                    {
                        RSCU_Status.bit.CheckSoftware_OK = 0;
                        RSCU_Status.bit.CheckBoot_OK = 0;
                        RSCU_CurrentState = _RSCU_BinCheckError;
                        RSCU_RunStep = 0xFE;
                    }
                }
                break;
            case 4: // move software from wifi module to the EEPROM of console
              /*  2.45
                if(PragramIndex_temp == 0 || PragramIndex_temp == ProgramDataIndex || RSCU_Status.bit.Retry == 1)
    {
                    Temp.All = PragramIndex_temp;  // 2.48     Temp.All = ProgramDataIndex;  // 2.45
                    _SendBuffer[0] = _Bin_1_time_size % 0x100 ;
                    _SendBuffer[1] = _Bin_1_time_size / 0x100 ;
                    _SendBuffer[2] = Temp.DT.LL ;
                    _SendBuffer[3] = Temp.DT.LH ;
                    _SendBuffer[4] = Temp.DT.HL ;
                    _SendBuffer[5] = Temp.DT.HH ;
        //
                    if(SendWifiCommand(WiFi_UCB_ProgrammingData,&_SendBuffer[0],6))
                    {
                        WiFi_Timer_Count = 0;
                        RSCU_Status.bit.Retry = 0;
                        WiFi_BinGetFromModulle = false;
                        if(PragramIndex_temp == ProgramDataIndex)
                            PragramIndex_temp += _Bin_1_time_size;
                    }
                }
                */
                //2.48
                if(WiFi_BinGetFromModulle == true)
                {// flash into the eeprom
                    WiFi_BinGetFromModulle = false;
                    WiFi_CommandGetFromModulleOK = false;
                    Mx25L1606E_WriteBlock((_Bin_Block_+ProgramDataIndex),_Bin_1_time_size ,ProgramData) ;
                    PragramIndex_temp = ProgramDataIndex + _Bin_1_time_size;
        //
                    // percentage of downloading
                    SoftwareToEEPROMPercent = (unsigned short)(PragramIndex_temp*100/_Bin_Size_);
        //
                    if(PragramIndex_temp >= _Bin_Size_)  
                    {
                        EEPROM_WriteInformation(EE_RSCU_SoftwareVersion, RSCU_SoftwareVer.all);
                        ProgramDataIndex = 0; 
                        RSCU_CurrentState = _RSCU_SoftwareDownloadOK;
                        
                        // Next
                        RSCU_Status.bit.GoToNext = 1;
                    }
                    WiFi_Timer_Count = 0;
                    _CheckCounter = 0;
                }
                else if(WiFi_CommandGetFromModulleOK == false || RSCU_Status.bit.Retry == 1)
                {
                    Temp.All = PragramIndex_temp;  // 2.48     Temp.All = ProgramDataIndex;  // 2.45
                    _SendBuffer[0] = _Bin_1_time_size % 0x100 ;
                    _SendBuffer[1] = _Bin_1_time_size / 0x100 ;
                    _SendBuffer[2] = Temp.DT.LL ;
                    _SendBuffer[3] = Temp.DT.LH ;
                    _SendBuffer[4] = Temp.DT.HL ;
                    _SendBuffer[5] = Temp.DT.HH ;
                    //
                    if(SendWifiCommand(WiFi_UCB_ProgrammingData,&_SendBuffer[0],6))
                    {
                        WiFi_Timer_Count = 0;
                        RSCU_Status.bit.Retry = 0;
                        WiFi_BinGetFromModulle = false;
                        WiFi_CommandGetFromModulleOK = true;
                    }
                }
                break;
            case 5:
            case 0xFF:
                RSCU_CurrentState = _RSCU_CanUpdate;
            default:    
                wifi_settings.isInRSCU = false;
                RSCU_Status.bit.ProcessEnd = 1;
                break;
        }
        
        if(RSCU_Status.bit.GoToNext == 1)
        {
            RSCU_Status.bit.GoToNext = 0;
            RSCU_RunStep++;
            WiFi_Timer_Count = 0;
            _CheckCounter = 0;
        }
    }
  
    return RSCU_CurrentState;
}

void WiFi_RSCU_Open(void)
{
    WiFi_Interface.CloseRSCU = 0;
}
void WiFi_RSCU_Close(void)
{
    WiFi_Interface.CloseRSCU = 1;
    wifi_settings.isInRSCU = false;
}

/*******************************************************************************
* Description : Check for RSCU version and console verison whether are the same.
* Input       : RSCU version string
* Output      : 0: form error, 1: have new software, 2: have no new software
*******************************************************************************/
#define _VerType 3  // the max = 4
unsigned char WiFi_CheckRSCUVersion(char* RSCUVersionString)
{// RSCUVersionString: XXX.XXX.XXX ....
    unsigned char i = 0, j = 0;
    unsigned char by_Dat = 0;
    unsigned char ConsoleVer[_VerType] = {UCB_Software_Ver, UCB_Software_Rel, UCB_Software_Build};
    
    RSCU_SoftwareVer.all = 0;
    // Version
    for(j=0; j<_VerType; j++)
    {
        by_Dat = 0;
        for(i=0;i<3;i++)
        {
            if(RSCUVersionString[4*j + i] >= '0' && RSCUVersionString[4*j + i] <= '9')
          {
                by_Dat *= 10;
                by_Dat += RSCUVersionString[4*j + i] - '0';
          }
            else
                return 0; // version string error
        }
        RSCU_SoftwareVer.byte[j] = by_Dat;
    }
    for(j=0; j<_VerType; j++)
    {
        if(RSCU_SoftwareVer.byte[j] < ConsoleVer[j])
            return 2;  // old version
        else if(RSCU_SoftwareVer.byte[j] > ConsoleVer[j])
            return 1;  // the latest software
    }

    return 2; // software is the same.
      }

/* This function is called at the end of the workout to get the workout total values. */
void GetWorkoutDataComplete( WIFIWorkoutDataCompleteEx* data, SHT1381_RealTime WorkoutStartTime)
{
    // TODO: Fill this out with actual data.    	
    //
    WiFi_memcpy( (*data).macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
    WiFi_memcpy( (*data).machineIdentification, machineConfig->machineIdentification, _WIFI_Length_MachineID );
    WiFi_memcpy( (*data).userExternalId, _UserExternalID, _WIFI_Length_UserExternalID );
    WiFi_memcpy( (*data).programUUID, machineConfig->programUUID, _WIFI_Length_ProgramUUID );
    //(*data).description   
    data->programType = WiFi_Information_Get(_WiFi_ProgramID);    
    data->warmupTimeSeconds = WiFi_Information_Get(_WiFi_WarmUpTime);
    data->workoutTimeSeconds = WiFi_Information_Get(_WiFi_WorkoutTime);
    data->cooldownTimeSeconds = WiFi_Information_Get(_WiFi_CoolDownTime);
    data->milesx100 = WiFi_Information_Get(_WiFi_Distance);
    data->averageHeartRate = WiFi_Information_Get(_WiFi_AvgHeartRate);
    data->totalCalories = WiFi_Information_Get(_WiFi_Calories);
    //data->totalFloorsX10 = WiFi_Information_Get(_WiFi_Floors);
    //data->totalSteps = WiFi_Information_Get(_WiFi_TotalSteps);
    data->totalElevationX100 = WiFi_Information_Get(_WiFi_Incline)*10;
    //
    // Date and time
    SHT1381ToNormal(WorkoutStartTime, &(data->StartTime));
  }
/*******************************************************************************
* This function fills out the structure passed with the current workout data. 
* This function is called when it is time to collect workout data for workout tracking.
 * The console software just needs to fill out the structure passed to this function
 * with the current values.
*******************************************************************************/
void GetWorkoutData( WIFIWorkoutDataEx* data, SHT1381_RealTime WorkoutStartTime )
{
    /* Notes on the various times.
    *
    * Workout Time = Total Workout Time - warm up - cool down.
    *
    * Stage Time Seconds = Program Stage Time.  This only applies to programs with multiple stages like
    * the fitness test and table based programs.
    *
    * State Time Seconds = Current state time.  This depends on the program state which is either in
    * warmup, workout, cool down, or pause.
    *
    * timeIn... variables is the elapsed time.
    */
    WiFi_memcpy( (*data).userExternalId, _UserExternalID, _WIFI_Length_UserExternalID );//userFlashConfig[_currUser]->
    WiFi_memcpy( (*data).macAddress, wifi_settings.mac_address, _WIFI_Length_MacAddress );
    WiFi_memcpy( (*data).programUUID, machineConfig->programUUID, _WIFI_Length_ProgramUUID );
    WiFi_memcpy( (*data).machineIdentification,machineConfig->machineIdentification,_WIFI_Length_MachineID);
      
    // TODO: Fill this out with actual data.
    data->programType = WiFi_Information_Get(_WiFi_ProgramID);
    
    data->workoutState = WiFi_Information_Get(_WiFi_WorkoutState);
    data->timeInStateSeconds = WiFi_Information_Get(_WiFi_TimeInState);
    
    // Stage
    // Stage time is used for table based programs, intervals, and fit test.  If this
    // is a manual workout, THR, VA, or constant watts the stage time = state time.    
    data->stageNumber = WiFi_Information_Get(_WiFi_WorkoutStage);
    data->timeInStage = WiFi_Information_Get(_WiFi_TimeInStage);
    
    // TODO: Get goal for calorie and distance goal programs.
    data->goalValueX100 = 100*WiFi_Information_Get(_WiFi_Goal);
    
    // Workout Data
    data->mphx10 = WiFi_Information_Get(_WiFi_Speed);
    data->inclinex10 = WiFi_Information_Get(_WiFi_Incline);
    data->totalCalories = WiFi_Information_Get(_WiFi_Calories);
    data->caloriesPerHour = WiFi_Information_Get(_WiFi_CaloriesPerHour);
    data->watts = WiFi_Information_Get(_WiFi_Watts);
    data->metsX10 = WiFi_Information_Get(_WiFi_METs);
    data->milesx100 = WiFi_Information_Get(_WiFi_Distance);
    data->heartRate = WiFi_Information_Get(_WiFi_HeartRate);
    data->averageHeartRate = WiFi_Information_Get(_WiFi_AvgHeartRate);
    data->rpm = WiFi_Information_Get(_WiFi_RPM);
    data->programLevel = WiFi_Information_Get(_WiFi_ProgramLevel);
    data->resistanceLevel = WiFi_Information_Get(_WiFi_ResistanceLevel);
    data->thr = WiFi_Information_Get(_WiFi_THR);
    data->targetWattsX10 = WiFi_Information_Get(_WiFi_TargetWatts);
    //data->floorsX10 = WiFi_Information_Get(_WiFi_Floors);
    //data->totalSteps = WiFi_Information_Get(_WiFi_TotalSteps);
    //data->lapIndex = 
    //data->splitIndex =
    data->averagePace = WiFi_Information_Get(_WiFi_AveragePace);
    // data->elevationX100 =
    // data->VO2X10 =
    
    
    switch(data->programType)
    {
        // Distance
        case GOAL_5K:
        case GOAL_10K:
        case GOAL_20K:
        case GOAL_30K:
        case DISTANCE_GOAL:
            data->goalType = 2;
            data->stageGoalValueX100 = data->goalValueX100;
            data->goalRemainingX100 = data->goalValueX100 - 100*data->milesx100;
            data->stageGoalRemainingX100 = data->goalRemainingX100;
            break;
        // Calorie
        case CALORIES_GOAL:      
            data->goalType = 3;
            data->stageGoalValueX100 = data->goalValueX100;
            data->goalRemainingX100 = data->goalValueX100 - 100*data->totalCalories;
            data->stageGoalRemainingX100 = data->goalRemainingX100;
            break;
        // Floors
        case MOUNTAIN_CLIMB:
        case STEP_GOAL:
        case LANDMARK:      
            data->goalType = 4;
            data->stageGoalValueX100 = data->goalValueX100;
            data->goalRemainingX100 = data->goalValueX100 - 100*data->floorsX10;
            data->stageGoalRemainingX100 = data->goalRemainingX100;
            break;
        // Time
        case MANUAL: 
            data->goalType = 1;
            data->stageGoalValueX100 = data->goalValueX100;
            data->goalRemainingX100 = data->goalValueX100 - 100*data->timeInStateSeconds;
            data->stageGoalRemainingX100 = data->stageGoalValueX100 - 100*(data->timeInStage);
            break;    
        default:    
            data->goalType = 1;
            data->stageGoalValueX100 = (data->workoutState == _WiFi_WorkoutState_Workout)?(10*WiFi_Information_Get(_WiFi_WorkoutStageTime)):data->goalValueX100;
            data->stageGoalRemainingX100 = data->stageGoalValueX100 - 100*(data->timeInStage);
            data->goalRemainingX100 = data->goalValueX100 - 100*data->timeInStateSeconds;
            break;
    }    
    
    
    // Date and time
    SHT1381ToNormal(WorkoutStartTime, &(data->StartTime));
}

// AM system
void UpdateMachineStats(void)
{
    WiFi_Set_MachineInformation();
    AddWifiResponseState( WCS_UPDATE_MACHINE_STATS, 0, 0, 0 );
}
void WiFi_OccurredErrorCode(u16 by_Dat)
{
    machineConfig->ErrorCode = by_Dat;
    AddWifiResponseState( WCS_OCCURRED_ERROR_CODE, 0, 0, 0 );
}


void SHT1381ToNormal(SHT1381_RealTime Source, DateAndTime* Dest)
{
    Dest->year = 2000 + Source.Year;
    Dest->month = Source.Month;
    Dest->day = Source.Day;
    Dest->hour = Source.Hour;
    Dest->minute = Source.Minute;
    Dest->second = Source.Second;
}
bool WIFICanLoginUser(void)
{
  return CanLoginUserFlag;
}
/*******************************************************************************
* Timer interruption for WiFi use
* Count one time per 100ms
*******************************************************************************/
void WiFi_Timer_Int(void)
{
    if(WiFi_Timer_Count < 250) // 25's
        ++WiFi_Timer_Count;
}
/*******************************************************************************
* Return the percenatge of flashing software into the eeprom.
* 0% ~ 100%
*******************************************************************************/
unsigned short WiFi_DownloadPercentage(void)
{
    return SoftwareToEEPROMPercent;
}