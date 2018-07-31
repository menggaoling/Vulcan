#include "Digital.h"
#include "Console.h"
#include  "UART.h"
#include "LCB_Support.h"
#include "UsbFunction.h"

#define eSTOP_DelayTime 400
#define InclineProtectionDelayTime 1000 

u16 MCBVersion;       //==>MCB�汾
u16 InclineLocation=0;  //==>����׼λ
u16 MotorRpm;         //==>����ٶ�
u16 ErrorCode_Hold;
u16 eSTOP_Time=0;
u16 by_Delay=0;//==>�Զ�����ش�ʱ�����
u16 by_DigitalSpeedTarget=0;//==>Ŀ���ٶ�׷����
u16 by_TargetSpeedDelay=100;//==>�ٶȼ����ʵ��ӳ���
u16 by_ElevationOld=0;//==>������Ŀ��ֵ
u16 _CheckElevationCommandAD = 0;
u16 by_Freq;
u16 by_Current;
u16 by_OutputVolt;
u16 by_DC_BusVolt;
u16 by_IGBT_Temp;
u16 ErrorCode;          //==>������
u16 _ErrorCodeBuffer[5];//==>�������ݴ�
u16 Console_SpeedRPM;
u16 _InclineProtectionDelay; 
u16 _InclineStopCheckTime = 0;
u16 by_TargetDelay=0;
u8 RX_Buffer[35];//==>��Ϣȡ���ݴ�
u8 TX_Buffer[25][20];//==>��Ϣ�����ݴ�,20��command,5������
u8 by_TX=0;//==>��Ϣ����ʱ��λ��ָ��
u8 by_TX_Target=0;//==>Command���������ݴ�λ��
u8 by_TX_Hold;//���ǰ���ݴ�
u8 by_TX_Insert = 20;// ���command����λ��
u8 by_TX_Byte=0;//==>����λ��
u8 by_RX=0;//==>������Ϣ�����ݴ���λ��ָ��
u8 by_ErrorTime=0;//==>����ش�ʱ�����
u8 by_Status=0;//==>����ش�״̬
u8 by_Command;//==>ָ��
u8 by_RX_ReturnTime=0;//==>���ݽ����Ƿ����ʱ�����
u8 by_TX_ConnectDelay=0;
u8 by_Error=0;//==>����û���߲Ķ��������ۼ��� (����ʧЧ����ⲿ��)
u8 by_ErrorCommandSum;//==>��֧�ֻ����� command �ش���������
u8 MachineType = 0;       //==>����(�°��ͺ�)
u8 by_GetDkip = 0;
u8 by_CheckInUser = 0;
u8 by_CheckInUserTimeNumber = 0;
u8 _ErrorCodeTarget = 0;//==>�����봢��λ��
u8 _MotorStopTime = 0;
u8 by_GetADC_Delay=0;
u8 _CheckRpmStopTime;
u8 _RunningCommand;
u8 _InclineCheckAgainDelayTime;

u8 by_InUserModeMotoSafeTime = InUserMotoCheckTime;// ����ȶ�ʱ��
u8 by_NonInUserTime = 0;
u8 Digital_CeckNonInUser = 0;

void Digital_InclineSafeFunctionCheck(void);
//
struct
   {
   unsigned RX            : 1;// �����ж��ж����
   unsigned RX_Return     : 1;// ���ݽ���
   unsigned Chang         : 1;// ����ȡ���л�
   unsigned Error         : 1;// ��������ж����
   unsigned Work          : 1;// ʹ�����Ƿ�ʼ�����˶��ж�
   unsigned Command       : 1;// Ϊ���ж�command�Ƿ�ɴ�����
   unsigned RPMStop       : 1;// ����Ƿ�ֹͣ�ж�
   unsigned Skip          : 1;// �ж��Ƿ������������
   unsigned TargetSpeed   : 1;// �Ƿ�׷�ٶ�             
   unsigned Buffer_Over   : 1;// ����ݴ����������
   unsigned TX            : 1;// ����
   unsigned AutoCheck     : 1;// ��̨�Զ������Ƿ�����
   unsigned Incline_Check : 1;// ����COMMAND�Ƿ�����
   unsigned Incline_Shift : 1;// �����Ƿ��ƶ�
   unsigned RPM_Check     : 1;// RPMת��ȷ�����
   unsigned SpeedStart    : 1;// ���趨�ٶ�ʱ
   unsigned DaughterBoardReset : 1;
   unsigned SAFE_Key      : 1;
   unsigned VersionStatus : 1;
   unsigned InclineStop   : 1; 
   unsigned Insert        : 1;// ָ����
   unsigned InsertSend    : 1;// ���ָ�ʼ����
   unsigned PowerOn04A0Pass : 1;
   unsigned CommandStart : 1;
   unsigned Stop : 1;         // �ܴ�ֹͣ
   unsigned InclineStopCheck : 1;
   }Digital;


/*
struct
{
  unsigned char by_TX;// ��Ϣ����ʱ��λ��ָ��
  unsigned char by_TX_Target;// Command���������ݴ�λ��
  unsigned char by_TX_Hold;// ���ǰ���ݴ�
  unsigned char by_TX_Insert;// ���command����λ��
  unsigned char by_TX_Byte;// ����λ��
  unsigned char by_RX;// ������Ϣ�����ݴ���λ��ָ��
  unsigned char by_ErrorTime;// ����ش�ʱ�����
  unsigned char by_Status;// ����ش�״̬
  unsigned char by_Command;// ָ��
  unsigned char by_RX_ReturnTime;// ���ݽ����Ƿ����ʱ�����
  unsigned char by_TX_ConnectDelay;
  unsigned char by_Error;// ����û���߲Ķ��������ۼ��� (����ʧЧ����ⲿ��)
  unsigned char by_ErrorCommandSum;// ��֧�ֻ����� command �ش���������
  unsigned char MachineType;// ����(�°��ͺ�)
  unsigned char by_GetDkip;
  unsigned char by_CheckInUser;
  unsigned char by_CheckInUserTimeNumber;
  unsigned char _ErrorCodeTarget;// �����봢��λ��
  unsigned char _MotorStopTime;
  unsigned char by_GetADC_Delay=0;
  unsigned char _CheckRpmStopTime;
  unsigned char _RunningCommand;
  unsigned char _InclineCheckAgainDelayTime;
  unsigned char RX_Buffer[35];// ��Ϣȡ���ݴ�
  unsigned char TX_Buffer[25][20];// ��Ϣ�����ݴ�,20��command,5������
 
  unsigned short MCBVersion;// MCB�汾
  unsigned short InclineLocation;// ����׼λ
  unsigned short MotorRpm;// ����ٶ�
  unsigned short ErrorCode_Hold;
  unsigned short eSTOP_Time;
  unsigned short by_Delay;// �Զ�����ش�ʱ�����
  unsigned short by_DigitalSpeedTarget;// Ŀ���ٶ�׷����
  unsigned short by_TargetSpeedDelay;// �ٶȼ����ʵ��ӳ���
  unsigned short by_ElevationOld;// ������Ŀ��ֵ
  unsigned short _CheckElevationCommandAD;
  unsigned short by_Freq;
  unsigned short by_Current;
  unsigned short by_OutputVolt;
  unsigned short by_DC_BusVolt;
  unsigned short by_IGBT_Temp;
  unsigned short ErrorCode;// ������
  unsigned short Console_SpeedRPM;
  unsigned short _InclineProtectionDelay; 
  unsigned short _InclineStopCheckTime;
  unsigned short by_TargetDelay;
  unsigned short _ErrorCodeBuffer[5];// �������ݴ�

}DigitalData;
*/
/*******************************************************************************
* Function Name  : Digital_ErrorCodeUpdate
* Description    : ����ǰ�Ĵ������������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_ErroeCodeUpdate(void)
{
  u8 i;
  if(_ErrorCodeTarget != 0)
  {
      
      for(i = 10;i > 0;i--)
      {
          if(i > _ErrorCodeTarget)
          {
              Console_DigitalError(Data_Set,i-1,Console_DigitalError(Data_Get,i-_ErrorCodeTarget-1,0));
              Console_DigitalError(Data_Set,i-_ErrorCodeTarget-1,0);
          }
          else
          {
              Console_DigitalError(Data_Set,i - 1,_ErrorCodeBuffer[_ErrorCodeTarget - i]);
              _ErrorCodeBuffer[_ErrorCodeTarget - i] = 0;
          }
      }
      _ErrorCodeTarget = 0;
  }
}
/*******************************************************************************
* Function Name  : Digital_ErrorCode_ChangToAddress()
* Description    : ����ȡ�õĴ��������λ�ƵĶ���,���ݴ�λ��
* Input          : None
* Output         : None
*******************************************************************************/
void Digital_ErrorCode_ChangToAddress(void)
{
  u8 i;
  
  for(i = 0;i < 5;i++)
  {
      if(_ErrorCodeBuffer[i] == ErrorCode)
      {//==>������ͬ�ľͲ�����
          i = 100;
      }
      else if(_ErrorCodeBuffer[i] == 0)
      {//==>����û�еĴ�����ͷ����ݴ���
          _ErrorCodeBuffer[i] = ErrorCode;
          if(Digtial_ErrorCode_Level(ErrorCode) == 1)
          {//==>Class C
              //BLX_Command(MAS_Error_Code,0);
              /*
              if( Console_GetAM_Status() == 1 ) // WIFI
              #ifdef WiFi_Module
                  UsbFunction_SaveWorkoutDataToWiFiAM(1) ; //����ʱ��ش���������ݷ��
              #endif
              else if( Console_GetAM_Status() == 0 ) // RF
              #ifdef  RF_AM
                  BLX_Command(MAS_Error_Code,0);    
              #endif 
              */
          }
          i = 200;
          _ErrorCodeTarget+=1;
      }
  }
  if(i < 10)
  {//==>���ݴ���û�п�λʱ����Ҫ�Զ����Ƴ�һ��λ��
      for(i = 0;i < 4;i++)
      {
          _ErrorCodeBuffer[i] = _ErrorCodeBuffer[i + 1];
      }
      _ErrorCodeBuffer[4] = ErrorCode;
      if(Digtial_ErrorCode_Level(ErrorCode) == 1)
      {//==>Class C
          //BLX_Command(MAS_Error_Code,0);
          /*
          if( Console_GetAM_Status() == 1 ) // WIFI
          #ifdef WiFi_Module
              UsbFunction_SaveWorkoutDataToWiFiAM(1) ; //����ʱ��ش���������ݷ��
          #endif
          else if( Console_GetAM_Status() == 0 ) // RF
          #ifdef  RF_AM
              BLX_Command(MAS_Error_Code,0);    
          #endif
          */
      }
      _ErrorCodeTarget+=1;
  }
  if(_ErrorCodeTarget > 5) _ErrorCodeTarget = 5;
}
/*******************************************************************************
* Function Name  : Digital_CheckClassCError()
* Description    : ȷ���ݴ������Ƿ��д�����C�ķ���
* Input          : None
* Output         : 0=>NO 1=>YES
*******************************************************************************/
u8 Digital_CheckClassCError(void)
{
  u8 by_Out = 0;
  u8 i;
  
  for(i = 0;i < 5;i++)
  {
      if(Digtial_ErrorCode_Level(_ErrorCodeBuffer[i]) == 1)
      {
          i = 10;
          by_Out = 1;
      }
  }
  return by_Out;
}
/*******************************************************************************
* Function Name  : u16 Digtial_Set_ErrorCode()
* Description    : �趨������
* Input          : ������
* Output         : None
* Return         : None
*******************************************************************************/
void Digtial_Set_ErrorCode(u16 by_Dat)
{
    ErrorCode = by_Dat;
}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_ErrorCode()
* Description    : ������ȡ��
* Input          : None
* Output         : None
* Return         : ErrorCode
*******************************************************************************/
u16 Digtial_Get_ErrorCode(void)
{
    return(ErrorCode);
}
/*******************************************************************************
* Function Name  : Digital_Initial
* Description    : ����ͨѶ����Ĭ��
* Input          : None
* Output         : UART1
* Return         : None
*******************************************************************************/
void Digital_Initial(void)
{   
    Digital.InclineStopCheck = 0;
    Digital.Stop = 0;
    _CheckRpmStopTime = 0;
    _RunningCommand = 0;
    _InclineCheckAgainDelayTime = 0;
    Digital.PowerOn04A0Pass = 0;// ��һ�ο���04A0�ų�
    //Digital.CommunicationStart = 0;
    //Digital.CommunicationSave = 0;
    //_Communication_TxTotal = 0;
    //_Communication_RxTotal = 0;
    _InclineProtectionDelay = 0; 
    by_Freq = 0;
    by_Current = 0;
    by_OutputVolt = 0;
    by_DC_BusVolt = 0;
    by_IGBT_Temp = 0;
    by_CheckInUser = 0x55;//==>Non Used
    Digital_CeckNonInUser = 0;
    Digital.VersionStatus=0;
    InclineLocation=0;
    Digital.SAFE_Key=0;
    eSTOP_Time=0;
    ErrorCode_Hold=0;//==>981104
    Digital.InsertSend = 0;
    Digital.Insert = 0;
    Digital.Incline_Shift=0;
    Digital.AutoCheck=0;
    Digital.RX=0;
    Digital.RX_Return=0;
    Digital.Chang=0;
    Digital.Error=0;
    Digital.Work=0;
    Digital.Buffer_Over=0;
    Digital.Incline_Check=0;
    Digital.TargetSpeed=0;
    Digital.RPMStop=0;//==>ֹͣ
    Digital.RPM_Check=0;
    by_RX=0;
    by_TX_Target=0;
    by_TX=0;
    by_TX_Byte=0;
    by_Error = 0;
    ErrorCode = 0;
    by_TargetSpeedDelay=100;
    by_TX_ConnectDelay=0;
    by_ElevationOld=0;
    by_ErrorCommandSum=0;
    Digital.DaughterBoardReset=0;
    
    _ErrorCodeBuffer[0] = 0;
    _ErrorCodeBuffer[1] = 0;
    _ErrorCodeBuffer[2] = 0;
    _ErrorCodeBuffer[3] = 0;
    _ErrorCodeBuffer[4] = 0;
    _ErrorCodeTarget = 0;
    
    Console_SpeedRPM = 0; 
    _MotorStopTime = 0;
}
/*******************************************************************************
* Function Name  : Digital_PowerON_MachineTypeCheck()
* Description    : �¿���֧���ж�
* Input          : None
* Output         : None
* Return         : 0 -> No 1 -> Yes
*******************************************************************************/
u8 Digital_PowerON_MachineTypeCheck(void)
{
   by_ElevationOld = Console_HighLowADC(Data_Get,0,0); 
   if(MachineType >= 0xC0)
       return 1;
   else
       return 0;
}
/*******************************************************************************
* Function Name  : Digital_GetMachineType()
* Description    : �¿��������ͺ�ȡ��
* Input          : None
* Output         : None
* Return         : MachineType
*******************************************************************************/
u8 Digital_GetMachineType(void)
{
   return MachineType;
}
void Digital_SetMachineType(u8 by_D)
{
   MachineType=by_D;
}

/*******************************************************************************
* Function Name  : Digital_CheckSafeKeyStatus()
* Description    : ��ⰴ�������غ��Ƿ�ȡ��0x02B2������
* Input          : None
* Output         : None
* Return         : Digital.SAFE_Key
*******************************************************************************/
u8 Digital_CheckSafeKeyStatus(void)//==>981015
{
  return Digital.SAFE_Key;
}
void Digital_ClearSafeKeyStatus(void)
{
  Digital.SAFE_Key=0;
}

/*******************************************************************************
* Function Name  : Digital_eSTOPKeyDelay()
* Description    : ���������ӳ�ʱ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 Digital_eSTOPKeyDelay(void)
{
  if(eSTOP_Time >= eSTOP_DelayTime)
      return 1;
  else 
      return 0;
}
void Digital_eSTOPKeyDelayClear(void)
{
   eSTOP_Time=0;
}

/*******************************************************************************
* Function Name  : Digital__DCI_DaughterBoard_Reset(
* Description    : DCI daughter board����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_CommandBufferReset(void)
{
    Digital.DaughterBoardReset=1; 
}

/*******************************************************************************
* Function Name  : Digital_Set_AutoCheck(u8 by_Dat)
* Description    : ��̨�Ƿ�����Զ�����ģʽ
* Input          : by_Dat
* Output         : 0->No 1->Yes
* Return         : None
*******************************************************************************/
void Digital_Set_AutoCheck(u8 by_Dat)
{
   Digital.AutoCheck=by_Dat;
}
/*******************************************************************************
* Function Name  : Digital_Set_SpeedTargetDelay(u16 by_Dat)
* Description    : �����ٶ������ӳ�ʱ��
* Input          : by_Dat
* Output         : by_TargetSpeedDelay
* Return         : None
*******************************************************************************/
void Digital_Set_SpeedTargetDelay(u16 by_Dat)
{
   by_TargetSpeedDelay=by_Dat;
}
/*******************************************************************************
* Function Name  : Digital_Set_DigitalSpeedTarget(u16 by_Dat)
* Description    : ����ٶ�׷��Ŀ��ֵ�趨
* Input          : by_Dat
* Output         : by_DigitalSpeedTarget ; Digital.TargetSpeed
* Return         : None
*******************************************************************************/
void Digital_Set_DigitalSpeedTarget(u16 by_Dat)//==>�趨׷����ʼֵ
{
  by_DigitalSpeedTarget = by_Dat * 10;
  if(by_Dat == 0)
  {// ׼��ֹͣ�ܴ�,�������������֮ǰ�µ�ָ��
      Digital.Buffer_Over = 1;// ��ֹͣ��ָ��
      by_TX=0;
      by_TX_Target=0;
      by_TX_Hold = 0;
      by_TX_Insert = 20;// �����ͷ
      Digital.Insert = 0;
      Digital.InsertSend = 0;// ���������
      if(by_Status & BIT2) 
      {
          Digital_Command(CmdSetMotorCounter,0);// Speed Stop
      }
      if((by_Status & BIT0)||(by_Status & BIT1)) 
      {
          Digital_Command(CmdSetInclineAction,0);// Inlcine Stop
          Digital.InclineStopCheck = 1;
      }
      if(by_Status & BIT2)
      {
          Digital_Command(CmdSetMotorCounter,0);// Speed Stop
      }
      by_GetADC_Delay = 0;
      Digital.Buffer_Over = 0;// ����������ָ��
      Digital.Stop = 1;
      _CheckRpmStopTime = 0;
      Digital.Incline_Check = 0;
      _InclineCheckAgainDelayTime = 0;
  }
  Digital.TargetSpeed = 1;//==>��ʼ׷��
}
/*******************************************************************************
* Function Name  : Digital_Set_TargetSpeedStart(u8 by_Dat)
* Description    : ����ٶ�׷������趨
* Input          : by_Dat
* Output         : Digital.TargetSpeed ; 1->��ʼ׷�� 0->ֹͣ׷��
* Return         : None
*******************************************************************************/
void Digital_Set_TargetSpeedStart(u8 by_Dat)
{
   Digital.TargetSpeed = by_Dat;
   if(by_Dat == 1)
   {
       Digital.RPM_Check=1;//==>����׷��ʱ�Ϳ�ʼ��RPM��ȷ�϶���
   }
}
/*******************************************************************************
* Function Name  : Digital_Get_TargetSpeed()
* Description    : ȡ���ٶ�׷�����
* Input          : None
* Output         : None
* Return         : Digital.TargetSpeed
*******************************************************************************/
u8 Digital_Get_TargetSpeed(void)
{
   return(Digital.TargetSpeed);
}
/*******************************************************************************
* Function Name  : Digital_TargetSpeed_To_DigitalCommand()
* Description    : ����ٶ�׷��������
* Input          : None
* Output         : CmdGetInclineLocation ; CmdSetMotorCounter ; Digital.TargetSpeed
* Return         : None
*******************************************************************************/
void Digital_TargetSpeed_To_DigitalCommand(void)
{
  if(by_TargetDelay > by_TargetSpeedDelay && !Digital.InsertSend)
  {//==>�ٶ�׷�����ӳ�
      by_TargetDelay=0;
      if(Digital.TargetSpeed)//==>��ʼ׷��ʱ
      {
         if(!Digital.Buffer_Over) //==>���ݴ�������λ��ʱ
         {
             if(by_GetADC_Delay == 2)//==>Ϊ��������������������ٶ�׷���б������ADֵ��ȡ��
             {
                 by_GetADC_Delay++;
                 Digital_Command(CmdGetInclineLocation,0);//==>ȡ��ADCֵ
             }
             else if(by_GetADC_Delay == 4)//==>Ϊ���RPMȡֵ�����������ٶ�׷���б������RPMֵ��ȡ��
             {
                 by_GetADC_Delay=0;
                 Digital_Command(CmdGetMotorRpm,0);//==>ȡ��RPMֵ
             }
             else
             {
                 by_GetADC_Delay++;
                 if(by_DigitalSpeedTarget == 0 || Digital.Stop == 1)// 2014.07.04
                 {// ����Ҫ��ֹ̨ͣʱ�ͼ���
                     Digital_Command(CmdSetMotorCounter,0);
                     Digital.TargetSpeed=0;//==>ֹͣ׷��
                 }
                 else//==>��Ҫ��̨׷����
                 {
                     if(by_DigitalSpeedTarget > Console_Information(Info_WorkoutSpeed,Data_Get,0))//==>����  Console_Speed(Data_Get,0)
                     {
                         by_DigitalSpeedTarget-=10;
                         Digital_Command(CmdSetMotorCounter,by_DigitalSpeedTarget);
                         if(by_DigitalSpeedTarget <= Console_Information(Info_WorkoutSpeed,Data_Get,0))//Console_Speed(Data_Get,0)
                         {//==>���ﵽĿ��ֵʱ��ֹͣ׷��
                             Digital.TargetSpeed=0;
                         }    
                     }
                     else if(by_DigitalSpeedTarget < Console_Information(Info_WorkoutSpeed,Data_Get,0))//==>����  Console_Speed(Data_Get,0)
                     {
                         by_DigitalSpeedTarget+=10;
                         if(!Digital.Buffer_Over) Digital_Command(CmdSetMotorCounter,by_DigitalSpeedTarget);
                         if(by_DigitalSpeedTarget >= Console_Information(Info_WorkoutSpeed,Data_Get,0))//Console_Speed(Data_Get,0)
                         {//==>���ﵽĿ��ֵʱ��ֹͣ׷��
                             Digital.TargetSpeed=0;
                         } 
                     }
                 }    
             }
         }
      }
  }
  else by_TargetDelay++;

}
/*******************************************************************************
* Function Name  : Digital_Set_InclineCheck(u8 by_Dat)
* Description    : �Ƿ��趨��������
* Input          : by_Dat ; 0->ֹͣ  1->����
* Output         : Digital.Incline_Check
* Return         : None
*******************************************************************************/
void Digital_Set_InclineCheck(u8 by_Dat)
{
   Digital.Incline_Check = by_Dat;
}
/*******************************************************************************
* Function Name  : Digital_Clear_ElevationOld()
* Description    : ����Ŀ���Inclineֵ���
* Input          : None
* Output         : by_ElevationOld
* Return         : None
*******************************************************************************/
void Digital_Clear_ElevationOld(void)
{
  by_ElevationOld=0;
}
/*******************************************************************************
* Function Name  : u16 Digital_Get_ElevationOld()
* Description    : ����Ŀ��Incline����ֵȡ��
* Input          : None
* Output         : None
* Return         : by_ElevationOld
*******************************************************************************/
u16 Digital_Get_ElevationOld(void)
{
  return by_ElevationOld; 
}

/*******************************************************************************
* Function Name  : Digtial_ElevationVariationCheck()
* Description    : �������ʵ��ADֵ��Ŀ��ADֵ�Ƿ�������
* Input          : 0=>Reset ����
*                  1=>Workout ����
* Output         : None
* Return         : Yes / No
*******************************************************************************/
u8 Digtial_ElevationVariationCheck(u8 by_D)
{
    if(by_D == 0) // Reset ����
    {
        if(InclineLocation >= ((Console_HighLowADC(Data_Get,0,0))+256))
        {// > ��͵�ֵ
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else // Workout
    {
        if((InclineLocation > (Console_HighLowADC(Data_Get,0,0)) &&
            InclineLocation < (Console_HighLowADC(Data_Get,1,0))) &&
           (InclineLocation >= by_ElevationOld+(2*128) || 
            InclineLocation <= by_ElevationOld-(2*128)) )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }    
}

/*******************************************************************************
* Function Name  : u8 Digital_Check_Buffer()
* Description    : ����ͨѶ����������Ƿ�����
* Input          : None
* Output         : None
* Return         : Digital.Buffer_Over ; 1->���� 0->���п�λ
*******************************************************************************/
u8 Digital_Check_Buffer(void)
{
    return(Digital.Buffer_Over);
}
/*******************************************************************************
* Function Name  : Digital_Set_Work(u8 by_Dat)
* Description    : ����ͨѶ�����л�ʹ��ָ�� (��Ϊʹ�����Ƿ�ʼ�����˶��ж�)
* Input          : by_Dat
* Output         : Digital.Work ; 1->Yes 0->No
* Return         : None
*******************************************************************************/
void Digital_Set_Work(u8 by_Dat)
{
    Digital.Work=by_Dat;
}
/*******************************************************************************
* Function Name  : Digital_Command(u8 by_Cmd,u16 by_Dat)
* Description    : ����ͨѶ command �趨
* Input          : by_Cmd - command
                   by_Dat - �趨����ֵ
* Output         : by_TX_Target ; Digital.Buffer_Over ; Digital.TX ; IO_Set_Digital_CTL(); TX_Buffer[][]
* Return         : None
*******************************************************************************/
u8 Digital_Command(u8 by_Cmd,u32 by_Dat)// Sinkyo
{
    u8 _Buffer = 0;
    u16 by_A = 0;
    u16 by_B = 0;
    
    Digital.Command=0;
    if(!Digital.Insert)
    {// ����
        _Buffer = by_TX_Target;
    }
    else
    {// ���
        _Buffer = by_TX_Insert;
    }  
    TX_Buffer[_Buffer][0]=0x00;//==> Start
    TX_Buffer[_Buffer][1]=0xff;//==> Address
    switch(by_Cmd)
    {
        case CmdInitial:           //  0x70 //�趨�°��ʼ�� 
        case CmdGetStatus:         //  0x71 //״̬ȡ��
        case CmdGetErrorCode:      //  0x72 //�������ȡ��
        case CmdSkipCurrentError:  //  0x76 //�Ƴ�������
        case CmdGetRollerRpm:      //  0xf8 //ȡ�ù����ٶ�
        case CmdGetMotorRpm:       //  0xf9 //ȡ������ٶ�
        case CmdGetInclineLocation://  0xfa //ȡ������׼λ
        case CmdGetVersion:        //  0x73 //ȡ��MCB�汾
        //case CmdGetDCI_Version:    //  0x78 //ȡ��DCI���¿�����汾  
        case CmdDCI_MANUAL_CALIBRATION:  // 0xab 
        case CmdDCI_GetInclineRange:  
        case CmdGetTreadmillInUsed:      // 0x95  
        case CmdGetMainMotorInformation: // 0x96  
                 TX_Buffer[_Buffer][2]=by_Cmd;
                 TX_Buffer[_Buffer][3]=0;   //==>data����
                 TX_Buffer[_Buffer][4]=Digital_CRC8((u8*)TX_Buffer[_Buffer],4);
                 break;
        case CmdSetInclineAction:  //  0xf5 //�趨��������
                 Digital.Incline_Shift=0;         
        case CmdSetWorkStatus:     //  0xf7 //�趨�˶�״̬    
        case CmdDCI_FORCE_INCLINE_OPERATION:   // 0x94  
          
                 TX_Buffer[_Buffer][2]=by_Cmd;
                 TX_Buffer[_Buffer][3]=1;   //==>data����
                 TX_Buffer[_Buffer][4]=by_Dat;
                 TX_Buffer[_Buffer][5]=Digital_CRC8((u8*)TX_Buffer[_Buffer],5);
                 break;
        case CmdSetInclineLocation://  0xf6 //�趨����λ��
                 if(Digital.Work)//==>������ڹ���ģʽ��״����
                 {
                     if(by_Dat==0)
                     {
                         by_Dat = Console_HighLowADC(Data_Get,0,0);
                     }
                     else
                     {
                         
                         by_A = Console_HighLowADC(Data_Get,1,0)-Console_HighLowADC(Data_Get,0,0);
                         // InclineΪ���趨�������߶�, Range = 0 ~ 1500 (����0~15%
                         by_Dat = (by_Dat * 100) / 2;                         
                         by_Dat = (u32)( by_Dat * by_A ) / 1500;
                         by_Dat += Console_HighLowADC(Data_Get,0,0);
                     }
                     if(by_ElevationOld != by_Dat)
                     {//==>���¾�AD������ͬʱ�ͽ���ָ���´ﶯ��
                         Digital.Incline_Shift = 1;
                         by_ElevationOld = by_Dat;
                         //==>���������ﲻͬ�������
                         if(MachineType == LCB_TopTek || MachineType == LCB_DELTA_C3)
                         {//==>����
                             by_Dat |= 0x8000;//==>Incline����
                         }
                     }
                     else
                     {
                         Digital.Command=1;//==>��ͬ����ʱ�Ͳ���command�Ĵ���
                     }
                 }
                 else if(MachineType == LCB_TopTek|| MachineType == LCB_DELTA_C3)//==>���������ﲻͬ�������
                 {//==>����
                   by_Dat |= 0x8000;//==>Incline����
                 }  
        case CmdSetMotorCounter:   //  0xf1 //�趨���ת��
                 if(by_Cmd==CmdSetMotorCounter)
                 {
                     if(by_Dat != 0)
                     {
                         if(Digital.PowerOn04A0Pass == 0)
                         {// ��� by Pass ����
                             Digital.PowerOn04A0Pass = 1;
                         }
                         Console_SpeedRPM = by_Dat;// �Ǳ��µ��ٶ�ֵ
                         if(Console_SpeedUnit(Data_Get,0) == C_KM)
                         {
                             by_Dat = (u32)((float)by_Dat * 0.62137);
                         }
                         if(by_Dat < 50) by_Dat = 50;
                         /* =============================================>>
                         170 RPM : LowSpeed 0.5 mph
                         4070RPM : HighSpeed 12.0 mph
                         5112RPM : HighSpeed 15.0 mph
                         LowSpeed  : 50
                         HighSpeed : 1500 //1200
                         RPM = 170RPM + (((Speed - LowSpeed) * (5112RPM - 170RPM)) / (HighSpeed - LowSpeed))
                         <<============================================= */
                         //if(MachineType == LCB_DCI)
                         //    by_Dat = 170 + ((u32)((by_Dat - 50) * 4942) / 1450);
                         //else
                         //{
                             by_A = Console_RPM_Parameter(0,0,0);// Get MinRPM
                             by_B = Console_RPM_Parameter(0,1,0);// Get MaxRPM
                             by_Dat = by_A + ((u32)((by_Dat - 50) * by_B) / 1500);
                             //by_Dat = 178 + ((u32)((by_Dat - 50) * Console_Get_Parameter()) / 1450);
                         //}
                         if(_MotorStopTime == 0)
                         {// START
                             _MotorStopTime = 1;
                         }
                         
                     } 
                     else 
                     {// ������
                         _MotorStopTime = 0;
                         Console_SpeedRPM = Console_StartSpeed(Data_Get,0) * 10;
                     }
                     if(by_Dat > 5300)
                     {
                         Digital.Command=1;//==>����command�Ĵ���
                     }
                 }
                 if(!Digital.Command)
                 {
                     TX_Buffer[_Buffer][2]=by_Cmd;
                     TX_Buffer[_Buffer][3]=2;   //==>data����
                     TX_Buffer[_Buffer][4]=by_Dat/256;
                     TX_Buffer[_Buffer][5]=by_Dat%256;
                     TX_Buffer[_Buffer][6]=Digital_CRC8((u8*)TX_Buffer[_Buffer],6);
                 }
                 break;
        case CmdDCI_SetInclineRange://==>����ADֵ��Χ�趨 (10~240)   
                 /*
                 TX_Buffer[_Buffer][2]=by_Cmd;
                 TX_Buffer[_Buffer][3]=4;   //==>data����
                 TX_Buffer[_Buffer][4]=0x05;// 10  
                 TX_Buffer[_Buffer][5]=0x00;
                 TX_Buffer[_Buffer][6]=0x78;// 240  
                 TX_Buffer[_Buffer][7]=0x00;
                 TX_Buffer[_Buffer][8]=Digital_CRC8((u8*)TX_Buffer[_Buffer],8);
                 */
                 break;   
        case CmdDCI_SET_RPM_SPEED_RANGE://==>RPMֵ��Χ�趨 (15mile=165~5300)(12mile=165~4300)   
                 /*
                 TX_Buffer[_Buffer][2]=by_Cmd;
                 TX_Buffer[_Buffer][3]=8;   //==>data����
                 TX_Buffer[_Buffer][4]=0x00; 
                 TX_Buffer[_Buffer][5]=0xa5;
                 TX_Buffer[_Buffer][6]=0x14; 
                 TX_Buffer[_Buffer][7]=0xb4;
                 TX_Buffer[_Buffer][8]=0;//0x01; 
                 TX_Buffer[_Buffer][9]=0;//0xd1;
                 TX_Buffer[_Buffer][10]=0;//0x31; 
                 TX_Buffer[_Buffer][11]=0;//0x6a;
                 TX_Buffer[_Buffer][12]=Digital_CRC8((u8*)TX_Buffer[_Buffer],12);
                 */
                 break; 
        case CmdEUPsMode: // Sinkyo
             Digital_Set_LCBEUPsState(0);     
             TX_Buffer[_Buffer][2]=by_Cmd;
             TX_Buffer[_Buffer][3]=1;   //==>data����
             TX_Buffer[_Buffer][4]=by_Dat;
             TX_Buffer[_Buffer][5]=Digital_CRC8((u8*)TX_Buffer[_Buffer],5);
             break;
        case CmdLCBDeviceData :  // Sinkyo 
            // LCB_Set_EEPromGetState(0);
             TX_Buffer[_Buffer][2]=by_Cmd;
             TX_Buffer[_Buffer][3]=3;   //==>data����
             TX_Buffer[_Buffer][4]=by_Dat >> 16; // second command
             TX_Buffer[_Buffer][5]=(u8)((u16)by_Dat>>8);
             TX_Buffer[_Buffer][6]=(u8)by_Dat;
             TX_Buffer[_Buffer][7]=Digital_CRC8((u8*)TX_Buffer[_Buffer],7);
             break;                        
    }
    if(!Digital.Command)//==>��������ͬʱ����command,�������������command
    {
        if(!Digital.Insert)
        {// ����
            by_TX_Target++;
            if(by_TX_Target > 19) by_TX_Target=0;
            if(Digital.InsertSend)
            {
                if(by_TX_Target == by_TX_Hold)//==>�����Ŀǰ�����ָ�����´���Ҫ�����λ��һ��ʱ�ͱ��벻������
                {
                    if(by_TX == 0) by_TX_Target=19;
                    else by_TX_Target--;
                    Digital.Buffer_Over=1;//==>�ݴ�������
                }
            }
            else
            {
                if(by_TX_Target == by_TX)//==>�����Ŀǰ�����ָ�����´���Ҫ�����λ��һ��ʱ�ͱ��벻������
                {
                    if(by_TX == 0) by_TX_Target=19;
                    else by_TX_Target--;
                    Digital.Buffer_Over=1;//==>�ݴ�������
                }
            }
            //
        }
        else
        {// Command������
            by_TX_Insert++;
            if(by_TX_Insert > 24) by_TX_Insert = 20;
            if(by_TX_Insert == by_TX)
            {//��������˾Ͳ�������
                if(by_TX == 20) 
                    by_TX_Insert = 24;
                else
                    by_TX_Insert--;
            }
            Digital.Insert = 0;
            Digital.InsertSend = 1;// �������
        }
        if(!Digital.RX)//==>����ɽ���ʱ�ż�������һ���
        {
            Digital.TX=1;
        }
    }
    return Digital.Buffer_Over;
}
/*******************************************************************************
* Function Name  : u8 Digital_CRC8(u8 *ptr,u8 DATALENGTH)
* Description    : ����ͨѶ CRC code
* Input          : *ptr ; DATALENGTH
* Output         : None
* Return         : CHECKDATA
*******************************************************************************/
const u8 CrcTab[16]={0x00,0x31,0x62,0x53,0xc4,0xf5,0xa6,0x97,0xb9,0x88,0xdb,0xea,0x7d,0x4c,0x1f,0x2e};
u8 Digital_CRC8(u8 *ptr,u8 DATALENGTH)
{
    u8 crcheckcnt,crchecktemp,crchalf,CHECKDATA=0;
    for(crcheckcnt =0; crcheckcnt < DATALENGTH; crcheckcnt ++)
    {
        crchecktemp = *(ptr+crcheckcnt);
        crchalf =(CHECKDATA/16);
        CHECKDATA<<=4;
        CHECKDATA^=CrcTab[crchalf ^( crchecktemp / 16)];
        crchalf =(CHECKDATA/16);
        CHECKDATA<<=4;
        CHECKDATA^=CrcTab[crchalf ^( crchecktemp & 0x0f)];
    }
    return CHECKDATA;
}
/*******************************************************************************
* Function Name  : u8 Digital_Get_RPMStatus()
* Description    : �ش�����״̬ (���������)
* Input          : None
* Output         : None
* Return         : Digital.RPMStop ; 0->ֹͣ   1->�˶�
*******************************************************************************/
u8 Digital_Get_RPMStatus(void)
{
       return Digital.RPMStop;
    
}
/*******************************************************************************
* Function Name  : Digital_AutoReply()
* Description    : ����ͨѶ������� ; ÿ 1ms ������
* Input          : None
* Output         : CmdSetInclineLocation ; CmdGetInclineLocation ; CmdGetErrorCode
* Return         : None
*******************************************************************************/
void Digital_AutoReply(void)
{
  static u8 by_CheckInUserDelayTime = 0;
  static u16 by_SpeedInclineCheckDelay = 0; 
 
  //
  if(Digital.TX)
  {//==>׼������TX����Ĵ���
      by_TX_ConnectDelay++;
      if(by_TX_ConnectDelay >= 50)
      {//==>command and command�����Ҫ50ms
          IO_Set_Digital_CTL();//==>�����Ϊ���
          by_TX_ConnectDelay=0;
          Digital.TX=0;
          // �Ͳ��ָ��
          if(Digital.InsertSend)
          {
              if(by_TX < 20)
              {// ��һ�β���ͷ�����ʼλ��
                  by_TX_Hold = by_TX;
                  by_TX = 20;//by_TX_Insert;
              }
          }
          if(TX_Buffer[by_TX][4+TX_Buffer[by_TX][3]] == Digital_CRC8((u8*)TX_Buffer[by_TX],4+TX_Buffer[by_TX][3]))
          {//==>������е� CRC ��ȶ���ȷʱ
              __HAL_UART_ENABLE_IT(&huart2, UART_IT_TXE);//==>USART2�жϴ�
          }
          else
          {//==>�Ƴ� command
              Digital.RX=0;
              Digital.RX_Return=0;
              by_TX++;
              if(Digital.InsertSend)
              {// ���
                  if(by_TX > 24) by_TX=20;
              }
              else
              {// ����
                  if(by_TX > 19) by_TX=0;
              }
          }
          //
      }    
  }
  //
  if(Digital.RX)//==>׼��������շ��
  { 
      if(Digital.CommandStart == 0) by_ErrorTime++;
      if(by_ErrorTime > 200)
      {//==>������200msû���ʱ���ش�               
          by_ErrorTime=0;
          if(by_Error > 20) //5000ms=250msx20 ;250=200ms+50ms�ķ����� 
          {//==>Լ5��󴢴������
              by_Error=0;
              ErrorCode=0x04B0;//0x04a0
              Digital_ErrorCode_ChangToAddress();
              by_GetDkip = 0;
          }
          else by_Error++;//==>����û���߲Ķ��������ۼ��� (����ʧЧ����ⲿ��)
          Digital.RX=0;
          by_RX=0;
          Digital.TX=1;
          IO_Set_Digital_CTL();//==>�����Ϊ���
      }
      else
      {
          if(!Digital.RX_Return)//==>TX���ݴ�����
          {
              if((USART2->SR & 0xc0)==0xc0)
              //if((USART2->SR & 0x80)==0x80 && (USART2->SR & 0x40)==0x40)
              {/* 0x80Ϊ Bit7��TXE���ݴ�������ж�
                  0x40Ϊ Bit6��TC���ݴ�������ж�
                  �ж���ȷ������д������,����Ϊ���������
               */
                  IO_Clear_Digital_CTL();
              }
          }
          else//==>���ݽ�����
          {
              by_RX_ReturnTime++;
              if(by_RX_ReturnTime > 10)
              {//==>������ 10msû��byte����ʱ���ж�Ϊ����������,ͬʱҲ���Ա����¿����˫���ķ����л���������·���Ĵ������
                  Digital_RX();//==>��ʼ���������Ϣ
                  if(Digital.Skip)//==>���д����뷢��ʱ
                  {
                      Digital.Skip=0;
                      if(Digital_CheckError())
                      {//==>�������Ƿ����
                          if(Digital.AutoCheck && ErrorCode != 0)// ==0x0140)//==>���Զ��������ų��˴�����
                          {
                              ErrorCode=0;
                              Digital_Command(CmdInitial,1);//==>S001-01 for DCI
                              Digital_Command(CmdSetWorkStatus,1);//==>S001-01 for DCI
                          }
                          else
                          {
                              Digital_ErrorCode_ChangToAddress();
                          }
                      }
                      else ErrorCode=0;
                  }
                  if(!Digital.Error && ((Digital.InsertSend == 1 && (by_TX == by_TX_Insert)) ||
                                        (Digital.InsertSend == 0 && (by_TX == by_TX_Target))))
                  {//==>�����Buffer��û������command��������Ļ��Ͳ��������Ķ���
                     
                  }
                  else
                  {//==>���� command               
                      Digital.Error=0;
                      Digital.TX=1;
                      IO_Set_Digital_CTL();//==>�����Ϊ���
                  }
              }
          }
      }
  }
  // Other
  if(!Digital.CommandStart)
  {
      // Incline Target 014.07.04 
      Digital_InclineSafeFunctionCheck();
      // Speed Target
      Digital_TargetSpeed_To_DigitalCommand();
      //
      if(eSTOP_Time < eSTOP_DelayTime)
      {
          eSTOP_Time++;  
      }
      if(by_Delay > 400)//==>ÿ400ms�Զ�����1��
      {
          by_Delay=0;
          Digital.Chang=1;
      }
      else by_Delay++;
      if(Digital.Chang && !Digital.Buffer_Over) 
      {//==>���ݴ�������λ��������ͨѶÿ400ms�Զ�ȡ����Ϣ1��
          if(_RunningCommand == 0)
          {
              Digital_Command(CmdGetInclineLocation,0);
              _RunningCommand = 1;
          }
          else if(_RunningCommand == 1)
          {
              Digital_Command(CmdGetMotorRpm,0);
              _RunningCommand = 0;
              Digital.Chang=0;
          }
      }
      if(by_SpeedInclineCheckDelay >= 1000)
      {//==>ÿ1����һ��
          by_SpeedInclineCheckDelay=0;
          if(!Digital.InsertSend)
          {//==>���϶�Ӧ�¿�����
              if(Console_WorkoutStatus(Data_Get,0) == 1 && by_CheckInUserTimeNumber != 0xff &&
                 Digital_GetMachineType() > LCB_TopTek )
              {//==>����̨�����˶����д����������½�����Ա��⶯��
                  by_CheckInUserDelayTime++;
                  if(by_CheckInUserDelayTime >= 1)// 2
                  {//==> ÿx����һ��command
                      by_CheckInUserDelayTime = 0;
                      if(!Digital.Buffer_Over)
                      {
                          Digital_Command(CmdGetTreadmillInUsed,0);//==>��Ա���
                      }
                  }
                  if(by_CheckInUser == 0xff)//==>> ��Ա��⹦��
                  {//==> In Used (0xff)
                      by_NonInUserTime = 0;
                      Digital_CeckNonInUser = 0;
                  }
                  else if(by_CheckInUser == 0x55)
                  {//==> Non Used (0x55)
                      by_NonInUserTime++;
                      if(by_NonInUserTime >= by_CheckInUserTimeNumber - by_InUserModeMotoSafeTime)
                      {//==>���� x ����ȷ������
                          Digital_CeckNonInUser = 1;
                      }
                  }
              }
              else
              {
                  by_InUserModeMotoSafeTime = InUserMotoCheckTime;// ����ȶ�ʱ��
                  by_NonInUserTime = 0;
                  by_CheckInUserDelayTime = 0;
                  Digital_CeckNonInUser = 0;
              }
              if(!Digital.Buffer_Over)
              {
                  Digital_Command(CmdGetMainMotorInformation,0);//==>�¿���Ϣȡ��
              }
          }
          // �����ض���ȷ��ȷ�� ==>>
          Digital_MotorFunctionCheck();
          // <<======
      }   
      else by_SpeedInclineCheckDelay++;
      //
  }
  //
}






/*******************************************************************************
* Function Name  : Digital_UartTxRx_Information()
* Description    : ����ͨѶ UART TX / RX ���ݴ�������� <������USART�ж���ʹ��>
* Input          : None
* Output         : TX_Buffer[][] ; by_Command ; RX_Buffer[by_RX]
* Return         : None
*******************************************************************************/
void Digital_UartTxRx_Information(void)
{
  
    if(__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_TXE) != RESET && __HAL_UART_GET_FLAG(&huart2,UART_FLAG_TXE) != RESET)
    {//==>�ж��Ƿ����ж�
    /* Write one byte to the transmit data register */
        huart2.Instance->DR = (TX_Buffer[by_TX][by_TX_Byte]) & 0xFF;
    /* Clear the USART2 transmit interrupt */
        __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TXE);
        if(by_TX_Byte >= (4+TX_Buffer[by_TX][3]))
        {
            Digital.RX=1;
            Digital.RX_Return=0;
            by_TX_Byte=0;
            by_RX_ReturnTime=0;
            by_TX_ConnectDelay=0;
            by_Command=TX_Buffer[by_TX][2];
            /* Disable the USART2 Transmit interrupt */
            __HAL_UART_DISABLE_IT(&huart2, UART_IT_TXE);//==>USART2 TX�жϹر�
            __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);//==>USART2 RX�жϴ�
        }
        else by_TX_Byte++;
    }
    if(__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE) != RESET && __HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE) != RESET)
    {//==>�ж��Ƿ�����ж�
        /* Read one byte from the receive data register */
        RX_Buffer[by_RX] = (uint8_t)(huart2.Instance->DR & (uint16_t)0x00FF);
        /* Clear the USART2 Receive interrupt */
        __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_RXNE);
        if(Digital.RX)
        {
            if(RX_Buffer[0] == 0x01)
            {//==>����ʼbyte = 0x01ʱ�Ż���ɺ����ķ������
                by_RX++;
            }
            by_RX_ReturnTime=0;
            by_TX_ConnectDelay=0;
            Digital.RX_Return=1;
        }
        else 
        {
            /* Disable the USART2 Transmit interrupt */
          __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
        }
    }
}


/*******************************************************************************
* Function Name  : Digital_RX()
* Description    : ����ͨѶ RX ���ݴ���
* Input          : None
* Output         : Digital.RPMStop ; MCBVersion ; RollerRpm ; MotorRpm ; InclineLocation ; ErrorCode ; Digital.Buffer_Over ; Digital.RX ; by_TX ; by_RX
                   Digital.Error ; by_Status
* Return         : None
*******************************************************************************/
void Digital_RX(void)
{
    union
    {
       unsigned int WORD;
       char BYTE[2];
    }byData;
    u8 by_ErrorLoop=0;
    
    by_Status = RX_Buffer[1];
#ifdef _DebugClose    
    if((by_Status & BIT5)||(RX_Buffer[0]!=0x01)||(RX_Buffer[2]!=by_Command)||(RX_Buffer[by_RX-1]!=Digital_CRC8((u8*)RX_Buffer,by_RX-1)))
#else // Debugʱ�ر�BIT5�Ļ���,����������!
    if((RX_Buffer[0]!=0x01)||(RX_Buffer[2]!=by_Command)||(RX_Buffer[by_RX-1]!=Digital_CRC8((u8*)RX_Buffer,by_RX-1)))  
#endif      
    {//==>����������ʱ���ش���һ�ʷ��
        Digital.Error=1;
        if(by_Status & BIT5) by_ErrorTime = 0;//==>���д����֧�ֵ�commandʱ
        if(by_Status & BIT5 || by_ErrorCommandSum >= 3)
        {//==>���ش�3�λ���ʧ�ܺ���Ƴ���command
            Digital.Error=0;
            if(Digital.InsertSend)
            {// ���
                if(by_TX != by_TX_Insert)
                {
                    by_TX++;
                    if(by_TX > 24) by_TX=20;
                }
            }
            else
            {// ����
                if(by_TX != by_TX_Target)  
                {
                    by_TX++;
                    if(by_TX > 19) by_TX=0;
                }
            }
            Digital.Buffer_Over=0;//==>�ݴ�������λ�ÿɴ��command
            by_ErrorCommandSum=0;
        }
        else by_ErrorCommandSum++;
        if(by_Command == CmdInitial || by_Command == CmdSetWorkStatus)
        {// ����������ʱ����ָ������´�
            by_GetDkip = 0;
            Digital.TargetSpeed=0;//==>ֹͣ׷��
            by_ErrorCommandSum=0;
            by_TX=0;
            by_TX_Target=0;
            Digital.Buffer_Over=0;
            by_TX_Hold = 0;
            by_TX_Insert = 20;// �����ͷ
            Digital.Insert = 0;
            Digital.InsertSend = 0;// ���������
            Digital_Command(CmdInitial,0);
            Digital_Command(CmdSetWorkStatus,0);
            Digital_Command(CmdSetWorkStatus,1);
        }
        if(by_Command == CmdGetTreadmillInUsed)
        {// ����Ա���Command����ʱ��������ۼ�
            by_NonInUserTime = 0;
        }
    }
    else
    {
        by_ErrorTime = 0;
        Digital.Error=0;
        by_Error=0;
        by_ErrorCommandSum=0;
        //********************************************//
        //**** ���Ѷ������Ƿ�ת��               ****//
        //********************************************//
        //B3:����(TM) RPM״̬          0: ֹͣ 1:����
        //B2:����ﶯ��״̬            0: ֹͣ 1:����
        //B1:�����������״̬          0: ֹͣ 1:����
        //B0:��������½�״̬          0: ֹͣ 1:�½�
        if((by_Status & 0x0f)==0)//==>0x0f(BIT0,1,2,3)
        {
            Digital.RPMStop=0;//==>ֹͣ
            Digital.Stop = 0;
        }
        else
        {
            Digital.RPMStop=1;//==>�˶�
        }
        if(RX_Buffer[3]==2)//==>ȡ�� Lenght byte��
        {
            byData.BYTE[0]=RX_Buffer[5];
            byData.BYTE[1]=RX_Buffer[4];
        }
        switch(by_Command)
        {
            case CmdSetInclineLocation://  0xf6
                     _CheckElevationCommandAD = by_ElevationOld;
                     break;
            case CmdEUPsMode:     
                     if(RX_Buffer[3] == 1)
                     {
                         if(RX_Buffer[4] != 0x01)
                            Digital_Set_LCBEUPsState(0xff);
                         else
                            Digital_Set_LCBEUPsState(RX_Buffer[4]);
                     }
                     else
                          Digital_Set_LCBEUPsState(0xff);
                     break;     
          
            case CmdGetTreadmillInUsed:      // 0x95 
                     by_CheckInUser = RX_Buffer[4];
                     break;
            case CmdGetMainMotorInformation: // 0x96
                     byData.BYTE[0]=RX_Buffer[5];
                     byData.BYTE[1]=RX_Buffer[4];
                     by_Freq = byData.WORD;
                     byData.BYTE[0]=RX_Buffer[7];
                     byData.BYTE[1]=RX_Buffer[6];
                     by_Current = byData.WORD;
                     byData.BYTE[0]=RX_Buffer[9];
                     byData.BYTE[1]=RX_Buffer[8];
                     by_OutputVolt = byData.WORD;
                     byData.BYTE[0]=RX_Buffer[11];
                     byData.BYTE[1]=RX_Buffer[10];
                     by_DC_BusVolt = byData.WORD;
                     byData.BYTE[0]=RX_Buffer[13];
                     byData.BYTE[1]=RX_Buffer[12];
                     by_IGBT_Temp = byData.WORD;
                     break;
            case CmdSkipCurrentError:  //==>0x76 �Ƴ�������
                     by_GetDkip = 0;
                     break;
            case CmdGetRollerRpm:      //==>0xf8 ȡ�ù����ٶ�
                     //RollerRpm=byData.WORD;
                     break;
            case CmdGetMotorRpm:       //==>0xf9 ȡ������ٶ�
                     MotorRpm=byData.WORD;
                     break;
            case CmdGetInclineLocation://==>0xfa ȡ������׼λ
                     InclineLocation=byData.WORD;
                     break;
            case CmdGetErrorCode:      //==>0x72 �������ȡ��
                     if(RX_Buffer[3] > 2)
                     {//==>���������кü���ʱ
                         for(by_ErrorLoop = 0;by_ErrorLoop < (RX_Buffer[3]/2);by_ErrorLoop++)
                         {
                             byData.BYTE[0]=RX_Buffer[5+(by_ErrorLoop*2)];
                             byData.BYTE[1]=RX_Buffer[4+(by_ErrorLoop*2)];
                             ErrorCode = byData.WORD;
                             if(Digital_CheckError())
                             {//==>���������
                                 Digital_ErrorCode_ChangToAddress();
                             }
                             else
                             {
                                 ErrorCode = 0;
                             }
                         } 
                     }              
                     else if(RX_Buffer[3] == 2) ErrorCode = byData.WORD;
                     Digital.Skip=1;
                     break;
            case CmdGetVersion:        //==>0x73 ȡ��MCB�汾
                     Digital.VersionStatus=1;
                     MCBVersion=byData.WORD;
                         MachineType=byData.BYTE[1];
                     break;         
                     /*
            case CmdGetDCI_Version:    //==>0x78 DCIר�ð汾������ȡ��
                     DCI_MCBVersion=(RX_Buffer[6]-0x30)*10;
                     DCI_MCBVersion=(DCI_MCBVersion+(RX_Buffer[8]-0x30))*10;
                     DCI_MCBVersion=(DCI_MCBVersion+(RX_Buffer[9]-0x30))*10;
                     DCI_MCBVersion=(DCI_MCBVersion+(RX_Buffer[19]-0x30))*10;
                     DCI_MCBVersion=(DCI_MCBVersion+(RX_Buffer[21]-0x30))*10;
                     DCI_MCBVersion+=(RX_Buffer[22]-0x30);
                     break;
                     
            case CmdDCI_GetInclineRange:
                     byData.BYTE[0]=RX_Buffer[5];
                     byData.BYTE[1]=RX_Buffer[4];
                     by_Incline_A=byData.WORD;
                     byData.BYTE[0]=RX_Buffer[7];
                     byData.BYTE[1]=RX_Buffer[6];
                     by_Incline_B=byData.WORD;
                     break;   
                     */
        }
        if(by_TX != by_TX_Target)// ȷ���Ƿ����õ��ݴ������� 
        {
            by_TX++;
            if(!Digital.InsertSend)
            {// ����
                if(by_TX > 19) by_TX = 0;
            }
            else
            {// ���            
                if(by_TX == by_TX_Insert)
                {// �����ָ��������
                    by_TX_Insert = 20;// �����ͷ
                    by_TX = by_TX_Hold;// ��ԭ�ϴ���������ֹͣ��λ�� 
                    Digital.InsertSend = 0;
                }
            }
            Digital.Buffer_Over=0;//==>�ݴ�������λ�ÿɴ��command
        }
    }
    if(by_Status & BIT6)
    {//==>���¿��лش�������ʱ,����ȡ�ô�����command
        Digital.Error = 0;
        if(by_GetDkip == 0)
        {
            by_GetDkip = 1;
            Digital.Insert = 1;// Command������
            Digital_Command(CmdGetErrorCode,0);
            Digital.Insert = 1;// Command������
            Digital_Command(CmdSkipCurrentError,0);
        }
    }
    Digital.RX=0;//==>�������
    by_RX=0;
    if(Digital.DaughterBoardReset && !Digital.InsertSend)
    {//==>RESET�¿�����
        _MotorStopTime = 0;
        Console_SpeedRPM = 0;
        by_GetDkip = 0;
        Digital.DaughterBoardReset = 0; 
        by_TX = 0;
        by_TX_Target = 0;
        Digital.Buffer_Over=0;
        by_TX_Insert = 20;// �����ͷ
        by_TX_Hold = 0;
        Digital.Insert = 0;
        Digital.InsertSend = 0;
        Digital.TargetSpeed = 0;//==>ֹͣ׷��
        by_DigitalSpeedTarget=0;
        Digital_Command(CmdInitial,0);
        Digital_Command(CmdSetWorkStatus,0);
        Digital_Command(CmdSetWorkStatus,1);
        if(Digital.AutoCheck == 1 || Digtial_ElevationVariationCheck(0) == 1)
        {//==>������ʵ��ֵ��Ŀ��ֵ�������,�����֮ǰ���趨��Ŀ��ֵ����Ȼ��������ȡ��
            by_ElevationOld=0;//==>������ɵ�AD�ݴ�ֵ
            //Console_Incline(Data_Set,0);
            Digital.Work=1;
            Console_Information(Info_WorkoutIncline,Data_Set,0);
        }
        Digital.AutoCheck = 0;  
    }
}
/*
u8 Digital_CheckInclineRange(void)
{
    if(by_Incline_A == 0x0500 && by_Incline_B == 0x7800)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}
*/
/*******************************************************************************
* Function Name  : u8 Digtial_ErrorCode_Level(u16 by_Dat)
* Description    : ������ĵȼ� Class A,Class B,Class C
* Input          : by_Dat ; �������
* Output         : None
* Return         : Class
*******************************************************************************/
u8 Digtial_ErrorCode_Level(u16 by_Dat)
{
   union
   {
       u16 WORD;
       u8 BYTE[2];
   }Dat;
   Dat.WORD=by_Dat;
   if(Dat.BYTE[0] >= 0xa0) return(1);//==> Class C
   else if(Dat.BYTE[0] >= 0x40) return(2);//==> Class B
   else return(3);//==> Class A

}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_MCBVersion()
* Description    : MCB�汾ȡ��
* Input          : None
* Output         : None
* Return         : MCBVersion
*******************************************************************************/
u16 Digtial_Get_MCBVersion(void)
{
    return(MCBVersion);
}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_InclineLocation()
* Description    : ����׼λȡ��
* Input          : None
* Output         : None
* Return         : InclineLocation
*******************************************************************************/
u16 Digtial_Get_InclineLocation(void)
{
    return(InclineLocation);
}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_RollerRpm()
* Description    : ����RPMȡ��
* Input          : None
* Output         : None
* Return         : RollerRpm
*******************************************************************************/
u16 Digtial_Get_RollerRpm(void)
{
    return(MotorRpm);//RollerRpm
}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_MotorRpm()
* Description    : ���RPMȡ��
* Input          : None
* Output         : None
* Return         : MotorRpm
*******************************************************************************/
u16 Digtial_Get_MotorRpm(void)
{
    return Console_SpeedRPM;
}

/*******************************************************************************
* Function Name  : Digital_CheckError()
* Description    : ȷ�ϴ������Ƿ���Ϲ淶
* Input          : None
* Output         : None
* Return         : Yes=1 / No=0
*******************************************************************************/
u8 Digital_CheckError(void)
{
   if(ErrorCode == 0x02B2)
   {
       Digital.SAFE_Key=1;
       Digital.TargetSpeed=0;//==>ֹͣ׷��
   }
   else if(ErrorCode == 0x0441)
   {
   }
   else //if((ErrorCode >= 0x0040 && ErrorCode <= 0x0045)||(ErrorCode >= 0x00A0 && ErrorCode <= 0x00A1) || 
        //   (ErrorCode >= 0x0140 && ErrorCode <= 0x0144)||(ErrorCode >= 0x01A0 && ErrorCode <= 0x01AD) || 
        //   (ErrorCode >= 0x0240 && ErrorCode <= 0x0248)|| ErrorCode == 0x0201 || ErrorCode == 0x029F  || 
        //   (ErrorCode >= 0x02A0 && ErrorCode <= 0x02BD)||(ErrorCode >= 0x0440 && ErrorCode <= 0x0442) ||  
        //   (ErrorCode >= 0x04A0 && ErrorCode <= 0x04A1)|| ErrorCode == 0x0340 || ErrorCode == 0x04B0)  
   {
       if(ErrorCode == 0x04A0)
       {
           if(Digital.PowerOn04A0Pass == 0)
           {// ��� by Pass ����
               Digital.PowerOn04A0Pass = 1;
               return 0;
           }
       }
       return 1;
   }
   
   return 0;

}
//------------------------------------------------------------------------------
// S003-01
/*******************************************************************************
* Function Name  : Digital_GetStatus()
* Description    : ͨѶ״̬����ȡ��
* Input          : None
* Return         : by_Status
*******************************************************************************/
u8 Digital_GetStatus(void) 
{
  return by_Status;
}
/*******************************************************************************
* Function Name  : Digital_InclineProtection()
* Description    : �������Ʊ�������
* Input          : None
* Return         : 0=>���� ; 1=>��������
*******************************************************************************/
u8 Digital_InclineProtection(u8 by_Dat)
{
  u16 _ADC_Buffer,_ADC_A,_ADC_B;
  
  
  if((by_Status & 0x03) != 0)
  {// ����������ʱ
      // �����׼���µĶ�ӦAD����
      if(by_Dat == 0)
      {
          _ADC_Buffer = Console_HighLowADC(Data_Get,0,0);
      }
      else
      {
          _ADC_A = (Console_HighLowADC(Data_Get,1,0) - Console_HighLowADC(Data_Get,0,0))/ 30;
          _ADC_B = (Console_HighLowADC(Data_Get,1,0) - Console_HighLowADC(Data_Get,0,0))% 30;
          if(by_Dat <= _ADC_B)
          {
              _ADC_Buffer = (Console_HighLowADC(Data_Get,0,0) + (by_Dat*_ADC_A)+ by_Dat);
          }
          else
          {
              _ADC_Buffer = (Console_HighLowADC(Data_Get,0,0) + (by_Dat*_ADC_A)+ _ADC_B);
          }
      }
      //
      if((((by_Status & 0x01) == 0x01)&&(InclineLocation < _ADC_Buffer))||  
         (((by_Status & 0x02) == 0x02)&&(InclineLocation > _ADC_Buffer)))
      {// UP���з��� or DOWN���з���
          Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ
          _InclineProtectionDelay = 1;
          return 1; 
      }
  }
  else
  {
      if(Digital.InclineStop == 0)
      {// ����ֹͣ , ����δȷ��ǰ����command����
          Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ
          _InclineProtectionDelay = 1;
          return 1;
      }
  }
  return 0;
}

/*******************************************************************************
* Function Name  : void Digital_Clear_ErrorTime()
* Description    : ���߼���(Error : 0x04B0)
* Input          : None
* Output         : None
* Return         : InclineLocation
*******************************************************************************/
void Digital_Clear_ErrorCount(void)
{
    by_Error = 0;
}

/*******************************************************************************
* Function Name  : Digital_GetMCB_Information()
* Description    : ȡ�¿���������Ϣ
* Input          : ��Ϣ����
* Return         : 
*******************************************************************************/
u16 Digital_GetMCB_Information(u8 by_D)
{
  switch(by_D)
  {
      case 1:// Ƶ��
             return by_Freq;
      case 2:// ����
             return by_Current;
      case 3:// �����ѹ
             return by_OutputVolt;
      case 4:// DC Bus��ѹ
             return by_DC_BusVolt;
      case 5:// IGBT �¶�
             return by_IGBT_Temp;
  }
  return 0;
}
/*******************************************************************************
* Function Name  : Digital_CommandStart()
* Description    : �������������
* Input          : by_D
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_CommandStart(char by_D)
{
  Digital.CommandStart = by_D;
}
/*******************************************************************************
* Function Name  : Digital_MotorFunctionCheck()
* Description    : �����ض���ȷ��ȷ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_MotorFunctionCheck(void)
{
  // Error Code judge
  if(_MotorStopTime != 0 && MotorRpm == 0)
  {
      _MotorStopTime++;
      if(_MotorStopTime > 40)
      {// ����40��û����              
          ErrorCode = 0x00A1;// �ٶ��޻ش�
          Digital_ErrorCode_ChangToAddress();
          by_GetDkip = 0;
      }
  }
  // ȷ�� Speed and Incline ֹͣ��������
  if(Digital.Stop == 1)
  {
      if(_CheckRpmStopTime >= 2)  
      {
          _CheckRpmStopTime = 0;
          if((by_Status & BIT2)&&(Digital.Buffer_Over == 0)) 
          {
              Digital_Command(CmdSetMotorCounter,0);// Speed Stop
          }
          if(((by_Status & BIT0)||(by_Status & BIT1))&&
             (Digital.InclineStopCheck == 1)&&
             (Digital.Buffer_Over == 0)) 
          {
              Digital_Command(CmdSetInclineAction,0);// Inlcine Stop
          }
      }
      else
          _CheckRpmStopTime++;
  }
  // ȷ��������ﶨλָ���
  if(_InclineCheckAgainDelayTime != 0)
  {
      if(_InclineCheckAgainDelayTime >= 3)
      {
          if(_CheckElevationCommandAD != by_ElevationOld)
          {// �շ���ͬ����Ҫ�ٲ���һ��
              if(Digital.Buffer_Over == 0)
              {
                  by_ElevationOld = 0;
                  Digital_Command(CmdSetInclineLocation,Console_Information(Info_WorkoutIncline,Data_Get,0));//Console_Incline(Data_Get,0)
                  _InclineCheckAgainDelayTime = 1;
              }
          }
          else
          {
              _InclineCheckAgainDelayTime = 0;
          }
      }
      else _InclineCheckAgainDelayTime++;
  }                             
}
/*******************************************************************************
* Function Name  : Digital_InclineSafeFunctionCheck()
* Description    : �����л��ӳ�,��������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_InclineSafeFunctionCheck(void)
{
  if((by_Status & 0x03) == 0)
  {
      if(_InclineStopCheckTime > 300) // ����300msû����
          Digital.InclineStop = 1; 
      else 
          _InclineStopCheckTime++;
  }
  else
  {
      _InclineStopCheckTime = 0;
      Digital.InclineStop = 0;
  }
  if(_InclineProtectionDelay > 0)
  {// �����ӳ�����
      _InclineProtectionDelay++;
      if(_InclineProtectionDelay > InclineProtectionDelayTime)
      {// ���������ӳ�Լx��
          _InclineProtectionDelay = 0;
      }
  }
  else
  { 
      if(Digital.Incline_Check && !Digital.Buffer_Over && !Digital.InsertSend)
      {//==>��������ָ�����ݴ�������λ��ʱ
          if(Digital_InclineProtection(Console_Information(Info_WorkoutIncline,Data_Get,0)) == 0)//Console_Incline(Data_Get,0)
          {// ����������ʱ
              if(!Digital.Buffer_Over)
              {
                  if(Digital_Command(CmdSetInclineLocation,Console_Information(Info_WorkoutIncline,Data_Get,0)) == 0)//Console_Incline(Data_Get,0)
                  {//==>�����趨
                      Digital.Incline_Check=0;
                      _InclineCheckAgainDelayTime = 1;
                      Digital.InclineStopCheck = 0;
                      _CheckElevationCommandAD = 0;
                  }
                  
              }
              if(!Digital.Buffer_Over)
              {
                  Digital_Command(CmdGetInclineLocation,0);
              }
          }
      }
  }
}




