#ifndef _Digital_H_
#define _Digital_H_

#include "General.h"

#define LCB_DCI      0xC0
#define LCB_TopTek   0xC1
#define LCB_DELTA    0xC2
#define LCB_DELTA_C3 0xC3

#define CmdInitial              0x70 //�趨�°��ʼ��
#define CmdGetStatus            0x71 //״̬ȡ��
#define CmdGetErrorCode         0x72 //�������ȡ��
#define CmdGetVersion           0x73 //ȡ��MCB�汾
#define CmdSkipCurrentError     0x76 //�Ƴ�Ŀǰ������
#define CmdGetDCI_Version       0x78 //DCIר��command

//==> Digital for TM
#define CmdSetMotorCounter      0xf1 //�趨���ת��
#define CmdSetPwmAddStep        0xf2 //�趨��������
#define CmdSetPwmDecStep        0xf3 //�趨��������
#define CmdSetPemStopStep       0xf4 //�趨ֹͣ����
#define CmdSetInclineAction     0xf5 //�趨��������
#define CmdSetInclineLocation   0xf6 //�趨����λ��
#define CmdSetWorkStatus        0xf7 //�趨�˶�״̬
#define CmdGetRollerRpm         0xf8 //ȡ�ù����ٶ�
#define CmdGetMotorRpm          0xf9 //ȡ������ٶ�
#define CmdGetInclineLocation   0xfa //ȡ������׼λ
#define CmdGetDriverType        0x91 //ȡ���������������
#define CmdDCI_GetMaxDataLength 0x92 //DCIר��command

#define CmdGetTreadmillInUsed   0x95 //��Ա���
#define CmdGetMainMotorInformation 0x96 //ȡ�¿���Ϣ

#define CmdDCI_SetInclineRange  0xa1 //DCIר��command,�����趨�Զ�����ʱ��������Χ
#define CmdDCI_GetInclineRange  0xa0 //DCIר��command,
#define CmdDCI_MANUAL_CALIBRATION 0xab //DCIר��command,
#define CmdDCI_FORCE_INCLINE_OPERATION 0x94 // DCIר��command,ǿ�������½�ֹͣ��
#define CmdDCI_GET_RPM_SPEED_RANGE 0xa6 //DCIר��command,����ȡ��RPM/SPEED��Χ
#define CmdDCI_SET_RPM_SPEED_RANGE 0xa7 //DCIר��command,�����趨RPM/SPEED��Χ

//==> Sinkyo
#define CmdEUPsMode                   0x7A // ʡ��ģʽ
#define CmdLCBDeviceData              0x50 // ȡ��LCB Device ������� 
#define SecCmdGetEEPromMemorySizes    0x01 // CmdLCBDeviceData���µĵڶ���Command // ȡ��LCB EEProm�Ѿ���������� unit:byte
#define SecCmdGetEEPromMemoryContent  0x02 // CmdLCBDeviceData���µĵڶ���Command // ȡ��LCB EEProm�Ѿ����������
#define SecCmdGetECBCurrent           0x03 // CmdLCBDeviceData���µĵڶ���Command // ȡ��LCB 2��ECB�ĵ���ֵ unit:mA
#define SecCmdGetEStopVale            0x04
// <==
#define InUserMotoCheckTime 6 // ����ȶ�ʱ�����ֵ
//

u8 Digital_GetMachineType(void);
void Digital_SetMachineType(u8 by_D);
u8 Digital_PowerON_MachineTypeCheck(void);

u8 Digital_CheckSafeKeyStatus(void);
void Digital_ClearSafeKeyStatus(void);
u8 Digital_eSTOPKeyDelay(void);
void Digital_eSTOPKeyDelayClear(void);
void Digital_CommandBufferReset(void);


void Digital_Initial(void);//====> ����ͨѶ����Ĭ��
void Digital_RX(void);//====> ����ͨѶ RX ���ݴ���
u8 Digital_Command(u8 by_Cmd,u32 by_Dat);//====> ����ͨѶ command �趨
u8 Digital_CRC8(u8 *ptr,u8 DATALENGTH);//====> ����ͨѶ CRC code
void Digital_AutoReply(void);//====> ����ͨѶ�������
u16 Digtial_Get_MCBVersion(void);//====> MCB�汾ȡ��
u16 Digtial_Get_InclineLocation(void);//====> ����׼λȡ��
u16 Digtial_Get_RollerRpm(void);//====> ����RPMȡ��
u16 Digtial_Get_MotorRpm(void);//====> ���RPMȡ��
void Digital_Set_Work(u8 by_Dat);//====> ����ͨѶ�����л�ʹ��ָ�� (��Ϊʹ�����Ƿ�ʼ�����˶��ж�)
u8 Digital_Get_RPMStatus(void);//====> �ش�����״̬
u16 Digtial_Get_ErrorCode(void);//====> ������
void Digtial_Set_ErrorCode(u16 by_Dat);
//void Digital_Clear_ErrorCode(void);//====> ����ȡ�õĴ��������λ�ƵĶ���,������λ��
u8 Digital_Check_Buffer(void);//====> ����ͨѶ����������Ƿ�����
void Digital_Set_InclineCheck(u8 by_Dat);//====> �趨��������
void Digital_Clear_ElevationOld(void);//====> ����Ŀ��Inclineֵ���
u16 Digital_Get_ElevationOld(void);//====> ����Ŀ��Inclineֵȡ��
void Digital_Set_DigitalSpeedTarget(u16 by_Dat);//====> ����ٶ�׷��Ŀ��ֵ�趨
void Digital_TargetSpeed_To_DigitalCommand(void);//====> ����ٶ�׷��������
void Digital_Set_TargetSpeedStart(u8 by_Dat);//====> ����ٶ�׷������趨
u8 Digtial_ErrorCode_Level(u16 by_Dat);//====> ������ĵȼ�
void Digital_Set_SpeedTargetDelay(u16 by_Dat);//====> �����ٶ������ӳ�ʱ��
void Digital_Set_AutoCheck(u8 by_Dat);//====> ��̨�Ƿ�����Զ�����ģʽ
u8 Digital_Get_TargetSpeed(void);//====> ȡ���ٶ�׷�����
void Digital_UartTxRx_Information(void);//====> TX / RX ���ݴ���
u8 Digtial_ElevationVariationCheck(u8 by_D);//====>�������ʵ��ADֵ��Ŀ��ADֵ�Ƿ�������
//u32 Digtial_Get_DCIVersion(void);
void Digital_ErrorCode_ChangToAddress(void);
//u8 Digital_CheckInclineRange(void);
u8 Digital_CheckError(void);

u8 Digital_CheckClassCError(void);
void Digital_ErroeCodeUpdate(void);
u8 Digital_InclineProtection(u8 by_Dat);// S003-01
u8 Digital_GetStatus(void);// S003-01
void Digital_Clear_ErrorCount(void);
u16 Digital_GetMCB_Information(u8 by_D);
//void Digital_CommunicationStart(u8 by_D);
//u8 Digital_CommunicationSave(void);
//u32 Digital_CommunicationTotal(u8 by_D);
void Digital_CommandStart(char by_D);
void Digital_MotorFunctionCheck(void);

#endif // _Digital_H_
