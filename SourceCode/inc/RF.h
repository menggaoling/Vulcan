#include "General.h"

/*=============================================
  BLX system 相关参数设定
===============================================*/
//==> Message Types
#define MSG_COMMAND                 0x01
#define MSG_RESPONSE                0x02
#define MSG_NOTIFICATION            0x03

//==> BLX COMMANDS
#define CMD_S_GET_BLX_STATUS        0x57
#define CMD_S_SET_BLX_DATA_RECORD   0x58
#define CMD_RESET_RADIO            0x5A //==>特殊指令,专门重置 RF RADIO
//==> PESPONSE CODES
#define RSP_ACK                     0x01
#define RSP_INVALID_TAG             0x17
#define RSP_INVALID_LEN             0x18
#define RSP_WRITE_FAILED            0x19

//==> Sensor Specific Notifications
#define N_HUB_CONNECT               0x71
#define N_HUB_DISCONNECT            0x72
#define N_OFFLOAD_COMPLETE          0x73
#define N_ERROR                     0x91

//==> Activity Measure Tags (ACM) command
#define ACM_Program                 0x12

//==> Machine Status Tags (MAS) command
#define MAS_Total_Utilization_Time  0x01
#define MAS_Total_H_Distance        0x02
#define MAS_Total_V_Distance        0x03
#define MAS_Current                 0x04
#define MAS_Voltage                 0x05
#define MAS_Error_Code              0x06
#define MAS_Service_Code            0x07
#define MAS_Serial_Number           0x20
#define MAS_Version                 0x21
#define MAS_Error_String            0x22
#define MAS_InfoExt                 0x25
#define MAS_StatusExt               0x26

//==> MFG byte
#define MFG_MATRIX                  0x10 // 16
#define MFG_JOHNSON                 0x15 // 21
#define MFG_VISION                  0x1F // 31

//==> CID byte
#define CID_Treadmill               0x01
#define CID_Bike                    0x02
#define CID_Elliptical              0x03

#define RF_Service_None 0
#define RF_Service_ON   1
#define RF_Service_OFF  2

void RF_UartTxRx_Information(void);

void BLX_SetPower(void);
void BLX_Set_RF_Service(u8 by_Dat);
u8 BLX_Get_RF_Service(void);
void BLX_Get_RF_RadioStatus(void);
void BLX_SetWowrkoutInformation(void);
void BLX_SetMachineInformation(void);
void BLX_DataCheck(void);
void BLX_GetInformation(void);
void BLX_RXSignalCheck_Int(void);
void BLX_CopeWith_RX_Data(void);
void BLX_CheckSumDecode(u8 by_D1,u8 by_D2);
void BLX_Command(u8 by_D,u8 by_D1);
void BLX_Get_SerialNumber(void);
void BLX_Get_ClubIDNumber(void);

/*
#define RF_CmdGetVersion      0x91
#define RF_CmdGetUtilization  0x99
#define RF_CmdGetHorizontal   0xa1
#define RF_CmdGetProgram      0xa4
#define RF_CmdGetMotorCurrent 0x9a
*/



void RF_Initial(void);
void RF_TX(void);
void RF_TX_InformationBuffer(u8 by_Dat);
void RF_EngineerModeTest(void);
u8 RF_EngineerModeTest_GetStatus(void);
u16 BLX_RadioVersion(void);

