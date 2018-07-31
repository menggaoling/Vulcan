#ifndef _C_safe_H_
#define _C_safe_H_

#include "stm32f2xx_hal.h"
#include "stm32f10x.h"

//v4 2012.09.12
#define Audio_OFF   0
#define Audio_TV    1
#define Audio_PCTV  2
#define Audio_RF900 3
#define Audio_CAB   4
//================================================================
//==> mode
#define Csafe_Ready    0x01//0x81
#define Csafe_Offline  0x09//0x89
#define Csafe_IDle     0x02//0x82
#define Csafe_Manual   0x08
#define Csafe_HaveID   0x03//0x83
#define Csafe_In_Use   0x05//0x85
#define Csafe_Paused   0x06//0x86
#define Csafe_Finished 0x07//0x87
//================================================================
//==> ��Ӧ command
//================================================================
#define cmdUpList         0x02

#define cmdIDDigits       0x10
#define cmdSetTime        0x11
#define cmdSetDate        0x12
#define cmdSetTimeout     0x13

#define cmdSetTWork       0x20
#define cmdSetHorizontal  0x21
#define cmdSetVertical    0x22
#define cmdSetCalories    0x23
#define cmdSetProgram     0x24
#define cmdSetSpeed       0x25
#define cmdSetGrade       0x28 // Incline
#define cmdSetGear        0x29 // Resistance
#define cmdSetUserInfo    0x2b
#define cmdSetTorque      0x2c
#define cmdSetLevel       0x2d

#define cmdSetTargetHR    0x30
#define cmdSetMETs        0x33 // METS goal
#define cmdSetPower       0x34 
#define cmdSetHRZone      0x35
#define cmdSetHRMAX       0x36

#define cmdSetChannelRange 0x40
#define cmdSetVolumeRange  0x41
#define cmdSetAudioMute    0x42
#define cmdSetAudioChannel 0x43
#define cmdSetAudioVolume  0x44

#define cmdGetStatus      0x80              
#define cmdReset          0x81                
#define cmdGoIdle         0x82                
#define cmdGoHavelID      0x83                
#define cmdGoInUse        0x85                
#define cmdGoFinished     0x86                
#define cmdGoReady        0x87                
#define cmdBadID          0x88

#define cmdGetVersion     0x91
#define cmdGetID          0x92
#define cmdGetUnits       0x93
#define cmdGetSerial      0x94
#define cmdGetList        0x98
#define cmdGetUtilization 0x99
#define cmdGetMotorCurrent 0x9a
#define cmdGetOdometer    0x9b
#define cmdGetErrorCode   0x9c
#define cmdGetServiceCode 0x9d

#define cmdGetTWork       0xa0
#define cmdGetHorizontal  0xa1
#define cmdGetVertical    0xa2
#define cmdGetCalories    0xa3
#define cmdGetProgram     0xa4
#define cmdGetSpeed       0xa5
#define cmdGetPace        0xa6
#define cmdGetCadence     0xa7  // Current Cadence
#define cmdGetGrade       0xa8  //(Incline)
#define cmdGetGear        0xa9
#define cmdGetUpList      0xaa                
#define cmdGetUserInFo    0xab  
#define cmdGetTorque      0xac

#define cmdGetHRCur       0xb0                
#define cmdGetHRTZone     0xb2
#define cmdGetMETs        0xb3
#define cmdGetPower       0xb4
#define cmdGetHRAvg       0xb5
#define cmdGetHRMax       0xb6

#define cmdGetAudioChannel 0xc0
#define cmdGetAudioVolume  0xc1
#define cmdGetAudioMute    0xc2
//================================================================
//==> �ֱ����׼�����͸������Ķ�Ӧ��Ϣ����
//================================================================
#define _Set_WorkoutTime     1  // �˶��ۼ�ʱ��
#define _Set_WorkoutDistance 2  // �˶��ۼƾ���
#define _Set_WorkoutCalories 3  // �˶��ۼƿ�·��
#define _Set_Level           4  // Level
#define _Set_Speed           5  // Speed
#define _Set_MET             6  // MET
#define _Set_Watt            7  // Watts
#define _Set_Program         8  // Program
#define _Set_AccumulatedTime 9  // ��ʱ���ۼ�
#define _Set_Manufacturer    10 //
#define _Set_CID             11 //
#define _Set_Model           12 //
#define _Set_Version         13 //
#define _Set_Release         14 //  
#define _Set_Units           15 // 0x01(Ӣ��)0x00(����)
#define _Set_DistanceUnits   16 // Distance ��ʾ��λ
#define _Set_SpeedUnits      17 // Speed ��ʾ��λ
#define _Set_Online          18 // �������
#define _Set_StartWorkout    19 // ��ʼ�˶����
#define _Set_Incline         20 // Incline
#define _Set_HRMax           21 // ʵ��ȡ�õ��������ֵ
#define _Set_HRAvg           22 // ʵ��ƽ������ֵ
#define _Set_HRTZone         23 // ʵ�����������趨��Χ�ڵ�ʱ�����
#define _Set_Gender          24 // Genser
#define _Set_AGE             25 // AGE
#define _Set_WeightUnits     26 // Weight ��ʾ��λ
#define _Set_Weight          27 // Weight
#define _Set_Mode            28 // CSAFE����ģʽ
#define _Set_HRCur           29 // ʵʱ����
#define _Set_AccumulatedDistance 30 // �ܾ����ۼ�
#define _Set_AvgSpeed        31 
#define _Set_Serial          32
#define _Set_MotorCurrent    33
#define _Set_ErrorCode       34
#define _Set_ServiceCode     35
#define _Set_Gear            36
#define _Set_Torque          37
#define _Set_AudioMute       38
#define _Set_AudioChannel    39
#define _Set_AudioVolume     40 
#define _Set_WorkoutVertical 41 
#define _Set_VerticalUnits   42 
#define _Set_CommunicationMode 43 // ��Χ����ͨѶģʽ
//================================================================
//==> �ֱ�ȡ�������趨�Ķ�Ӧ��Ϣ����
//================================================================
#define _Get_DFdistnce        1 // �趨ֵ
#define _Get_TargetDistance   2 // �趨���
#define _Get_DFcalories       3 // �趨ֵ
#define _Get_TargetCalories   4 // �趨���
#define _Get_DFprogram        5 // �趨ֵ
#define _Get_Mode             6 // CSAFEģʽ״̬
#define _Get_Online           7 // ����״̬
#define _Get_HRZoneMax        8 // �����趨������Χ���ֵ(����Χʱ�������)
#define _Get_HRZoneMin        9 // �����趨������Χ��Сֵ(����Χʱ�������)
#define _Get_THR             10 // �����趨����Ŀ��ֵ
#define _Get_TargetHR        11 // �����趨����Ŀ��ֵ���
#define _Get_THR_Max         12 // �����趨����Ŀ�����ֵ
#define _Get_TargetTime      13 // �趨���
#define _Get_Time            14 // �趨ֵ
#define _Get_TargetLevel     15 // �趨���
#define _Get_Level           16 // �趨ֵ
#define _Get_Gender          17 // �趨ֵ
#define _Get_AGE             18 // �趨ֵ
#define _Get_Power           19 // �趨ֵ
#define _Get_Weight          20 // �趨ֵ
#define _Get_TargetPower     21 // �趨���
#define _Get_SetTime         22 // �����趨��ʱ��ʱ��
#define _Get_SetDate         23 // �����趨��ʱ������
#define _Get_SetTimeout      24 // �����趨��Timeoutֵ
#define _Get_TargetSpeed     25 // �趨���
#define _Get_TargetGrade     26 // �趨���
#define _Get_TargetGear      27 // �趨���
#define _Get_TargetTorque    28 // �趨���
#define _Get_SetSpeed        29 // �趨ֵ
#define _Get_SetGrade        30 // �趨ֵ
#define _Get_SetGear         31 // �趨ֵ
#define _Get_SetTorque       32 // �趨ֵ
#define _Get_TargetMETs      33
#define _Get_TargetChannelRange 34 // �趨���
#define _Get_TargetVolumeRange  35 // �趨���
#define _Get_TargetAudioMute    36 // �趨���
#define _Get_TargetAudioChannel 37 // �趨���
#define _Get_TargetAudioVolume  38 // �趨���
#define _Get_SetMETs         39 // �趨ֵ 
#define _Get_SetChannelRange 40 // �趨ֵ
#define _Get_SetVolumeRange  41 // �趨ֵ
#define _Get_SetAudioMute    42 // �趨ֵ
#define _Get_SetAudioChannel 43 // �趨ֵ
#define _Get_SetAudioVolume  44 // �趨ֵ
#define _Get_TargetUserInfo  45 // �趨���
#define _Get_HRAvg           46 // HRƽ��ֵ
#define _Get_CID             47
#define _Get_Manufacturer    48
#define _Get_Model           49

//================================================================


unsigned long int CSAFE_GetInformation(char by_D);
void CSAFE_SetInformation(char by_D,unsigned long int by_Data);

void CSAFE_Counter_Int(void);
void CSAFE_RX_Buffer_Decision(void);
void CSAFE_Number_Check(void);
char CSAFE_TX_XOR(char by_Dat);
void CSAFE_RX_Answer_Buffer(char by_Dat);
void CSAFE_Initial(void);
void CSAFE_Power_On(void);
void CSAFE_Set_ID_Number(unsigned long int by_Dat);
void CSAFE_ClearIDNumber(void);
void CSAFE_Set_AnyKey(void);
void CSAFE_Answer(char by_Dat);
void CSAFE_Error_Answer(char by_Dat);
char CSAFE_Mode_Check_Answer(char by_Dat);
void CSAFE_Work_Exit(void);
char CSAFE_Mode_Judge(void);
void CSAFE_Reset(void);//==>��C-SAFEϵͳ��������
void CSAFE_EngTest(void);
void CSAFE_SetInformation(char by_D,unsigned long int by_Data);
void CSAFE_Set_EngTest(char by_Dat);
char CSAFE_Get_EngTestStatus(void);
void CSAFE_Write(char by_Length);
char CSAFE_TxRx_Information(char by_D);
char CSAFE_TxBuffer(void);
void CSAFE_RxBuffer(char by_D);
void CSAFE_900M_Cardio(char by_Command,char by_Num); 
void CSAFE_SetCheckAnyKeyStatus(char by_Dat);
void CSAFE_900M_E7command(void);
char CSAFE_900M_CommandStatus(void);
void CSAFE_900M_CommandStatusClear(void);
void CSAFE_900M_E7DataAnalysis(void);
char CSAFE_GetNetPulse(void);
void CSAFE_HRInformation(void);
char CSAFE_900M_GetControlBuffer(char by_D);

#endif // _C_safe_H_
