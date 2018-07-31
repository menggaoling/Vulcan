

#ifndef __WIFI_COMMAND_H__
#define __WIFI_COMMAND_H__


/* Define to prevent recursive inclusion -------------------------------------*/
// Protocol 
// Transmit Packet
// Start Byte (1)	Command (2)	Data Length (2)	Data (N)	CRC(2)	End Byte(1)
// 
// Receive Packet
// Start Byte (1)	Command (2)	DataR Length (2)	Data (2 + N)	CRC(2)	End Byte(1)
// Data(2) = Status information. It must be transmitted first.
// Status.0 (WIFI connection): 1 = Ready
// Status.1 (Error): 1 = Error (analyze the Data(2) to get the error code
// Status.2 (Unused): 1 = Not supported command
// Status.3 (Resend): 1 = Resend the package

#define WiFi_START                              0xf1
#define WiFi_END                                0xf2
#define WiFi_SCF                                0xf0

#define WiFi_StartPacket                        0x00
#define WiFi_CommandPacket                      0x01
#define WiFi_LengthPacket                       0x03
#define WiFi_DataPacket                         0x05

#define WiFi_HeaderSize                         0x05
#define WiFi_CrcSize                            0x02
#define WiFi_EndSize                            0x01

#define WiFi_PacketOffset                       (WiFi_HeaderSize+WiFi_CrcSize+WiFi_EndSize)
                                                        // S  2+R  
#define WiFi_GetModuleSoftwareVersion           0x0001	// 0	2	  Get WIFI Module WIFI Software Version
#define WiFi_GetUartSoftwareVersion             0x0002	// 0	2	  Get WIFI Module UART Software Version
#define WiFi_GetModuleStatus                    0x0003	// 0	2	  Get WIFI Module Status
#define WiFi_GetMACAddress                      0x0004	// 0	6	  Get MAC Address
#define WiFi_GetIPAddress                       0x0005	// 0	6	  Get WIFI IP
                                                        //        For IPV4: 0x00, 0x00, IP
#define WiFi_ConnectToAP                        0x0006	// 0	0	  Connect to WIFI AP
#define WiFi_DisconnectToAP                     0x0007	// 0	0	  Disconnect to WIFI AP
#define WiFi_SetSSID                            0x0008	// N	N	  Set WIFI SSID ( 32 Bytes )
#define WiFi_SetSecurityType                    0x0009	// 1	1	  Set WIFI Security Type (DataT = DataR)
                                                        //        0: None
                                                        //        1: WEP_40Bits
                                                        //        2: WEP_104Bits
                                                        //        3: WPA_KEY
                                                        //        4: WPA_PHRASE
                                                        //        5: WPA2_KEY
                                                        //        6: WPA2_PHRASE
                                                        //        7: WPA_AUTO_KEY
                                                        //        8: WPA_AUTO_PHRASE
#define WiFi_SetSecurityKey                     0x000A	// N	N	  Set WIFI Security Key (DataT = DataR)
                                                        //        (Max 32Bytes)
// 20120110 Add by Kunlung
#define WiFi_EraseAllFlash                      0x0010
#define WiFi_GetSSID                            0x0011
#define WiFi_GetSecurityType                    0x0012
#define WiFi_GetSecurityKey                     0x0013
//

#define WiFi_GetTcpIpBufferSize                 0x0020  // 0  4   Get TCP/IP Buffer Size ( TX(2)+RX(2) )
#define WiFi_SetTcpIpAddress                    0x0021  // N	0	  (Max 128Bytes with 0x00 terminator)
#define WiFi_GetTcpIpAddress                    0x0022	// 0	N	  Get TCP/IP address
#define WiFi_SetTcpIpPort                       0x0023	// 2	0	  Set TCP/IP port
#define WiFi_GetTcpIpPort                       0x0024	// 0	2	  Get TCP/IP port
#define WiFi_TcpIpConnect                       0x0025	// 0	0	  TCP/IP connect
#define WiFi_TcpIpDisconnect                    0x0026	// 0	0	  TCP/IP disconnect
#define WiFi_SetTcpIpData                       0x0027	// N	0	  Set TCP/IP data (Max 256Bytes)
#define WiFi_GetTcpIpData                       0x0028	// 0	N	  Get TCP/IP data (Max 256Bytes)


#define WiFi_EntryProgrammingMode               0x0100	// 6	1	  Entry Programming Mode
                                                        //        DataT: JHTPgr
                                                        //        DataR: 1 = Ready
#define WiFi_LeaveProgrammingMode               0x0101	// 0	0	  Leave Programming Mode
#define WiFi_SetProgrammingAddress              0x0102	// 4	0	  Set Programming Address
#define WiFi_ProgrammingData                    0x0103	// N	0	  Programming Data
                                                        //        N size must be less than 128.
#define WiFi_GetProgrammingStatus               0x0104	// 0	1	  Get Programming Status
                                                        //        Bit0: 1 = Programming Mode
                                                        //        Bit1: 7 = Busy
#define WiFi_ProgrammingErase                   0x0105  // 0  0   
                                                        //        
                                                        //        
                                                        //
#define	WiFi_UCBLCB_LeaveProgrammingMode				0x0201  // 1  0
#define	WiFi_UCBLCB_GetProgrammingStatus				0x0202  // 0  1
#define	WiFi_UCB_ProgrammingData					      0x0203  // 6  N   Length(2)+FlashAddr(4)
#define	WiFi_LCB_ProgrammingData					      0x0303  // 6  N   Length(2)+FlashAddr(4)
#define WiFi_AM_DataToModule                    0x0500  // N  1   Length(2)+FlashAddr(4)+Data(100)
#define WiFi_AM_DataToWeb                       0x0502  // 0  1

#define WiFi_WorkoutProgramLevel                0x1000	// 4	0	  Workout Program & Level
                                                        //        1~2: Program ID
                                                        //        3~4: Program Level
#define WiFi_WorkoutName                        0x1001	// N	0	  Workout Name
                                                        //        N size must be less than 128.
#define WiFi_UserInfo                           0x1002	// 4	0	  User Info
                                                        //        1: Gender (0:Female, 1:Male)
                                                        //        2: Age
                                                        //        3~4: Weight
                                                        //             Bit15(MSB)
                                                        //             0: Metric Unit (kg)
                                                        //             1: Imperial Unit (lbs)
#define WiFi_WorkoutSpeed                       0x1003	// 2	0	  Workout Speed
                                                        //        Resolution = 0.1
                                                        //        Bit15(MSB)
                                                        //        0: Metric Unit (km)
                                                        //        1: Imperial Unit (mile)
#define WiFi_WorkoutDistancePerFloor            0x1004	// 2	0	  Workout Distance/ Workout floor
                                                        //        Resolution = 0.1/floor
                                                        //        Bit15(MSB)
                                                        //        0: Metric Unit (km)
                                                        //        1: Imperial Unit (mile)
#define WiFi_WorkoutIncline                     0x1005	// 2	0	  Workout Incline
                                                        //        Resolution = 0.1%
                                                        //        Bit15(MSB)
                                                        //           0: Positive
                                                        //           1: Negative
#define WiFi_WorkoutMotorRPM                    0x1006	// 2	0 	Workout Motor RPM/RPM/SPM
#define WiFi_WorkoutTime                        0x1007	// 4	0	  Workout Time
                                                        //        Unit => Second
#define WiFi_WorkoutCalories                    0x1008	// 2	0	  Workout Calories
#define WiFi_WorkoutMETs                        0x1009	// 2	0	  Workout METs
                                                        //        Unit => 0.1 METs
#define WiFi_WorkoutPower                       0x100A	// 2	0	  Workout Power
                                                        //        Unit => Watts
#define WiFi_WorkoutHR                          0x100B	// 1	0	  Workout HR
                                                        //        Unit => BPM
#define WiFi_Info                               0x2000	// 16 + (Version Count * 4)	0	Info
                                                        //        1-2: Structure Version
                                                        //        3-4: Club ID
                                                        //        5~6: Machine Type ID
                                                        //        7~8: Version Count
                                                        //        9~12: Software Version Console
                                                        //        13~16: Software Version LCB
#define WiFi_Info1                              0x2001	// 25	0	  Serial Number
                                                        //        1~5: Factory ID (JHT01/JIS01)
                                                        //        6~12: Model ID
                                                        //        13~25: Serial Number (20100730XXXXX)
#define WiFi_Version                            0x3000	// 8		  Version
                                                        //        1: Manufacturer ID
                                                        //        2: Class Identifier
                                                        //        3~4: Model
                                                        //        5~6: Version
                                                        //        7~8: Protocol Version
#define WiFi_RecordTotalHorizontalDistance      0x3001	// 4	0	  Record Total Horizontal Distance
                                                        //        1: 0x00 => Metric Unit (km)
                                                        //           0xFF => Imperial Unit (mile)
                                                        //        2~5: Accumulated H Distance
#define WiFi_RecordTotalVerticalDistance        0x3002	// 4	0	  Record Total Vertical Distance
                                                        //        1: 0x00 => Metric Unit (km)
                                                        //           0xFF => Imperial Unit (mile)
                                                        //        2~5: Accumulated V Distance
#define WiFi_RecordTotalUtilizationTime         0x3003	// 4	0	  Record Total Utilization Time
                                                        //        Unit => Second
#define WiFi_RecordErrorCode                    0x3004	// 2	0	  Record Error Code
#define WiFi_RecordServiceCode                  0x3005	// 2	0	  Record Service Code


// for DEMO
#define DDemo_GetAllData						            0xF000



#define _STATUS_ERROR_DATA_SIZE_                4


#endif /* __WIFI_COMMAND_H__ */


