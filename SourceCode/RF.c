#include "RF.h"
#include "Digital.h"
#include "EEPRom.h"
#include "treadmill.h"

#define FIFO_MAX 10
#define Buffet_SUM 80//64
//#define Auto_DelayTime 1800 //21600 //==>6 hour

u8 BitCnt=0x0a;
u8 ByteCnt;
u8 RX_BitCnt;
u8 RX_Data;
u8 BLX_XCS=0;
u8 BLX_ACS=0;
u8 TX_Seg;//==>TX�ݴ����λ��
u8 RX_Seg;//==>RX�ݴ����λ��
u8 by_Lan=0;
u8 TXData[Buffet_SUM];//={0xf1,0x80,0x80,0xf2};
u8 RXData[Buffet_SUM];
u8 Tagged_Data[Buffet_SUM];
u8 BLX_ID[8];
//===========================>>
u8 TX_FIFO_Start=0;
u8 TX_FIFO_END=0;
u8 TX_FIFO[FIFO_MAX][Buffet_SUM]={0};
u8 _AMserviceMode=0;
u8 by_RF_TEST=0;//==>����ģʽ�µĵ������������
u8 _TX_POP_Sum=0;//==>���´��ʹ����ۼ�
u16 TX_Delay=0;
u16 _PowerON_Delay=0;
//u16 _AutomaticPush=0;
u16 _ClubID=0;
//<<===========================
u16 RXTXData; 
u16 RX_GetCheckTime=0;
u16 BLX_Version;
//===========================>>
u32 _AccTime=0;
u32 _AccDistance=0;
u32 _SerialNumber=0;
//<<===========================
struct
{
    unsigned int TX_END  : 1;
    unsigned int TX_SetData : 1;
    unsigned int TX_Start : 1;
    unsigned int Power : 1;
}BLXbit;

//==============================================================================
//==>> �� Digital.c ȡ����Ϣ
extern u16 by_Freq;
extern u16 by_Current;
extern u16 by_OutputVolt;
extern u16 by_DC_BusVolt;
extern u16 by_IGBT_Temp;
//==============================================================================


void RF_Initial(void)
{
    BLX_Version = 0;
    _TX_POP_Sum=0;
    //_AutomaticPush=0;
    BLXbit.Power=0;
    _PowerON_Delay=0;
    _AccTime=0;
    _AccDistance=0;
    TX_Delay=0;
    BLXbit.TX_Start=0;
    BLXbit.TX_SetData=0;
    TX_FIFO_Start=0;
    TX_FIFO_END=0;
    BitCnt=0x0a;
    ByteCnt=0;
    RX_BitCnt=8;
    RXTXData=0;
    RX_Data=0;
    RX_Seg=0;
    TX_Seg=0;
    by_Lan=0;
    BLXbit.TX_END=0;
    RX_GetCheckTime=0;
    by_RF_TEST=0;
}
/*******************************************************************************
* Function Name  : RF_TX(void)
* Description    : TX���俪ʼ
* Input          : Data
* Output         : None
* Return         : None
*******************************************************************************/
void RF_TX(void)
{
    BLXbit.TX_Start=1;
    BLXbit.TX_END=0;
    ByteCnt=0;
    USART_ITConfig(UART5, USART_IT_TXE, ENABLE);//==>UART5�жϴ�
}


/*******************************************************************************
* Function Name  : RF_UartTxRx_Information()
* Description    : RF UART TX / RX ���ݴ�������� <������USART�ж���ʹ��>
* Input          : None
* Output         : 
* Return         : None
*******************************************************************************/
void RF_UartTxRx_Information(void)
{
    if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
    {//==>�ж��Ƿ����ж�
    /* Write one byte to the transmit data register */
        USART_SendData(UART5,TX_FIFO[TX_FIFO_END][ByteCnt]);
    /* Clear the USART5 transmit interrupt */
        USART_ClearITPendingBit(UART5, USART_IT_TXE);
        if(ByteCnt >= TX_FIFO[TX_FIFO_END][Buffet_SUM-1]) 
        {
            RX_Seg=0;//==>��RX�������ݴ����λ�������ͷ
            RX_GetCheckTime=0;
            /* Disable the USART5 Transmit interrupt */
            USART_ITConfig(UART5,USART_IT_TXE, DISABLE);
            USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);//==>USART5 RX�жϴ�
        }
        else 
        {
            ByteCnt++;
        }
    }
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) 
    {//==>�ж��Ƿ�����ж�
        /* Read one byte from the receive data register */
        RXData[RX_Seg] = USART_ReceiveData(UART5);
        if(RX_Seg != 0 && RXData[RX_Seg] == 0xC0) 
        {
            if((RXData[0] == 0x03)&&(RXData[1] == N_HUB_CONNECT || RXData[1] == N_HUB_DISCONNECT || \
                RXData[1] == N_OFFLOAD_COMPLETE))  
            {//==>��AP���ش˶�Ӧcommandʱ //==>981102
            }
            else
            {
                if(RX_Seg != 1)
                {
                    if(by_RF_TEST == 1)
                    {//==>��Ϊ����ģʽʱ
                        by_RF_TEST=2;                    
                    }  
                    BLX_CopeWith_RX_Data();
                }    
            }
            RX_Seg=0;//==>�����ݽ����ͽ�RX�������ݴ����λ�������ͷ 
            /* Disable the UART5 Transmit interrupt */
            USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
        }
        else if(RXData[RX_Seg] != 0xC0) 
        {//==>���жϷ�0xC0ʱ�ſɽ����ۼ�
            RX_Seg++;//==>�´η���λ��
        }
        /* Clear the UART5 Receive interrupt */
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);  
    }
}

/*******************************************************************************
* Function Name  : RF_TX_InformationBuffer()
* Description    : ׼�������Ϣս��
* Input          : command
* Output         : TXData
* Return         : None
*******************************************************************************/
void RF_TX_InformationBuffer(u8 by_Dat)
{
   u8 i;
   if(_AMserviceMode != RF_Service_None)
       {//==>���д�BLX����ʱ
       TX_Seg=1;
       TX_FIFO[TX_FIFO_Start][1]=by_Dat;
       switch(by_Dat)
          {
          case CMD_S_SET_BLX_DATA_RECORD://==>0x58
                for(i = 0 ; i <= by_Lan ; i++)
                    {
                    TX_FIFO[TX_FIFO_Start][i + 2]=Tagged_Data[i];
                    }
                TX_Seg += (by_Lan + 1);
                by_Lan=0;                
          case CMD_S_GET_BLX_STATUS://==>0x57  
                TX_FIFO[TX_FIFO_Start][0]=MSG_COMMAND;
                break;
          case CMD_RESET_RADIO://==>0x5A  
                TX_FIFO[TX_FIFO_Start][0]=MSG_COMMAND;
                TX_FIFO[TX_FIFO_Start][2]=0x5A;
                TX_FIFO[TX_FIFO_Start][3]=0xFE;
                TX_FIFO[TX_FIFO_Start][4]=0x5A;
                TX_FIFO[TX_FIFO_Start][5]=0xFE;
                TX_Seg=5;
                break;         
          case N_HUB_CONNECT://==>0x71
          case N_HUB_DISCONNECT://==>0x72  
                TX_FIFO[TX_FIFO_Start][0]=MSG_NOTIFICATION;
                for(i = 0 ; i < 8; i++)
                    {
                    TX_FIFO[TX_FIFO_Start][i + 2]=BLX_ID[i];
                    }
                TX_Seg=9;
                break;
          case N_ERROR://==>0x91  
         
          case N_OFFLOAD_COMPLETE://==>0x73
                TX_FIFO[TX_FIFO_Start][0]=MSG_NOTIFICATION;
                break;
          }
       BLX_CheckSumDecode(0,TX_Seg);//++
       TX_FIFO[TX_FIFO_Start][++TX_Seg]=BLX_XCS;
       TX_FIFO[TX_FIFO_Start][++TX_Seg]=BLX_ACS;
       BLX_DataCheck();
       TX_FIFO[TX_FIFO_Start][++TX_Seg]=0xC0;
       TX_FIFO[TX_FIFO_Start][Buffet_SUM-1]=TX_Seg;//==>��λ���������һ���ݴ�byte
       TX_FIFO_Start++;
       if(TX_FIFO_Start >= FIFO_MAX) TX_FIFO_Start=0;
       if(TX_FIFO_Start == TX_FIFO_END)//==>�����Ŀǰ�����ָ�����´���Ҫ�����λ��һ��ʱ�ͱ��벻������
           {
           if(TX_FIFO_END == 0) 
               TX_FIFO_Start=FIFO_MAX;
           else 
               TX_FIFO_Start--;
           }
       }
}
/*******************************************************************************
   BLX���������ȷ���Ƿ���0xC0 or 0xDB byte ? ���н�ת̬ !
   1. 0xC0 => 0xDB,0xDC
   2. 0xDB => 0xDB,0xDD
*******************************************************************************/
void BLX_DataCheck(void)
{
  unsigned char _i,_j,_k;
  _j=TX_Seg;
  for(_i = 0;_i <= _j;_i++)
      {
      if(TX_FIFO[TX_FIFO_Start][_i] == 0xC0)
          {
          TX_FIFO[TX_FIFO_Start][_i]=0xDB;
          _j++;
          _i++;
          for(_k = _j;_k > _i;_k--)   
              {
              TX_FIFO[TX_FIFO_Start][_k]=TX_FIFO[TX_FIFO_Start][_k-1]; 
              }
          TX_FIFO[TX_FIFO_Start][_i]=0xDC;
          }
      else if(TX_FIFO[TX_FIFO_Start][_i] == 0xDB)
          {
          _j++;
          _i++;
          for(_k = _j;_k > _i;_k--)   
              {
              TX_FIFO[TX_FIFO_Start][_k]=TX_FIFO[TX_FIFO_Start][_k-1]; 
              }
          TX_FIFO[TX_FIFO_Start][_i]=0xDD;  
          }
      }
  TX_Seg=_j;
}
/*******************************************************************************
* Function Name  : RF_EngineerModeTest()
* Description    : RF����ģʽר�ô������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RF_EngineerModeTest(void)
{
   RF_TX_InformationBuffer(CMD_RESET_RADIO);
   by_RF_TEST=1;     
}
/*******************************************************************************
* Function Name  : RF_EngineerModeTest()
* Description    : ȡ��RF����ģʽ������Խ��
* Input          : None
* Output         : None
* Return         : 0->����Fail ; 1->����Pass
*******************************************************************************/
u8 RF_EngineerModeTest_GetStatus(void)
{
  if(by_RF_TEST == 2)
  {
      by_RF_TEST=0;
      return 1;
  }
  else
  {
      by_RF_TEST=0;
      return 0;
  }
}
/*******************************************************************************
   XCS & ACS checksum ����
   by_D1 = 0 (TXData)  / by_D1 = 1 (RXData)
   by_D2 = total byte
*******************************************************************************/
void BLX_CheckSumDecode(u8 by_D1,u8 by_D2)
{
  u8 i,by_Data;
  BLX_XCS = 0;
  BLX_ACS = 0;
  for(i = 0 ; i <= by_D2 ; i++)
      {
      if(by_D1 == 0)
          by_Data = TX_FIFO[TX_FIFO_Start][i];
      else 
          by_Data = RXData[i];
      if(i == 0)
          BLX_XCS = by_Data;
      else
          BLX_XCS ^= by_Data;
      BLX_ACS += by_Data;
      }
  BLX_ACS += BLX_XCS;
}
/*******************************************************************************
   RX������ɺ�Ĵ���
*******************************************************************************/
void BLX_CopeWith_RX_Data(void)
{
  u8 i; 
  switch(RXData[1])
      {
      case CMD_S_GET_BLX_STATUS:
               for(i = 0 ; i <= 7; i++)
                   {
                   BLX_ID[i] = RXData[6 + i];
                   }
               BLX_Version = RXData[3]*100;
               BLX_Version += RXData[4]*10;
               BLX_Version += RXData[5];
               
               break;
               
      }
  
  if(BLXbit.TX_Start)
      {//==>TX���ͺ������յ�RX���ݺ��׼����һ��
      BLXbit.TX_Start=0;  
      _TX_POP_Sum=0; 
      TX_FIFO_END++;
      if(TX_FIFO_END > FIFO_MAX) TX_FIFO_END=0;
      }
  
}
/*******************************************************************************
   BLX ����汾�ش�
*******************************************************************************/
u16 BLX_RadioVersion(void)
{
  return BLX_Version;
}
/*******************************************************************************
   BLX �µ��� command
   by_D1 : �Ƿ񷢳�command 0:Yes / 1:No
*******************************************************************************/
void BLX_Command(u8 by_D,u8 by_D1)
{
  union
    {
       long unsigned int LONG_WORD;
       unsigned int WORD[2];
       unsigned char BYTE[4];
    }BLX_Data;
  
  if(_AMserviceMode != RF_Service_None)
      {//==>���д�BLX����ʱ
      Tagged_Data[by_Lan]=by_D;
      Tagged_Data[++by_Lan]=0x00;
      Tagged_Data[++by_Lan]=0x00;
      Tagged_Data[++by_Lan]=0x07;
      switch(by_D)
          {
          case ACM_Program://==>1,0,0,12
               Tagged_Data[by_Lan]=0x01;//==>���Ӧֵ��ͬ
               Tagged_Data[++by_Lan]=0x02;
               Tagged_Data[++by_Lan] = Treadmill_ProgramMode(Data_Get,0);//==> Program ID
               Tagged_Data[++by_Lan] = Treadmill_Level(Data_Get,0);//==> Level
               break;
          case MAS_Total_Utilization_Time://==>7,0,0,1
               Tagged_Data[++by_Lan]=0x04;
               BLX_Data.LONG_WORD=_AccTime;
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[0];//==> Seconds
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[1];
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[2];
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[3];
               break;    
          case MAS_Total_H_Distance://==>7,0,0,2 (Horizontal)
               Tagged_Data[++by_Lan]=0x04;
               BLX_Data.LONG_WORD=_AccDistance;
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[0];//==> 0.1mile
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[1];
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[2];
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[3];
               break;          
          case MAS_Error_Code://==>7,0,0,6
               BLX_Data.LONG_WORD=Digtial_Get_ErrorCode();
               Tagged_Data[++by_Lan]=0x03;
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[0];
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[1];
               Tagged_Data[++by_Lan]=0x00;
               break; 
          case MAS_Service_Code://==>7,0,0,7
               Tagged_Data[++by_Lan]=0x03;
               Tagged_Data[++by_Lan]=0x00;
               Tagged_Data[++by_Lan]=0x00;
               Tagged_Data[++by_Lan]=0x00;
               break;          
          case MAS_Serial_Number://==>7,0,0,20
               Tagged_Data[++by_Lan]=19;//==>λ��
               Tagged_Data[++by_Lan]='F';//==>FitLinxx header
               Tagged_Data[++by_Lan]='L';
               Tagged_Data[++by_Lan]='X';
               Tagged_Data[++by_Lan]='1';
               Tagged_Data[++by_Lan]='0';
               Tagged_Data[++by_Lan]='T';//==> Serial Number
               Tagged_Data[++by_Lan]='M';
               Tagged_Data[++by_Lan]='5';
               Tagged_Data[++by_Lan]='1';
               Tagged_Data[++by_Lan]='9';
               //Tagged_Data[++by_Lan]='D';
               BLX_Data.LONG_WORD=_SerialNumber;
               Tagged_Data[++by_Lan]=(BLX_Data.LONG_WORD / 100000000)+'0';
               BLX_Data.LONG_WORD %= 100000000;
               Tagged_Data[++by_Lan]=(BLX_Data.LONG_WORD / 10000000)+'0';
               BLX_Data.LONG_WORD %= 10000000;
               Tagged_Data[++by_Lan]=(BLX_Data.LONG_WORD / 1000000)+'0';
               BLX_Data.LONG_WORD %= 1000000;
               Tagged_Data[++by_Lan]=(BLX_Data.LONG_WORD / 100000)+'0';
               BLX_Data.LONG_WORD %= 100000;
               Tagged_Data[++by_Lan]=(BLX_Data.LONG_WORD / 10000)+'0';
               BLX_Data.LONG_WORD %= 10000;
               Tagged_Data[++by_Lan]=(BLX_Data.LONG_WORD / 1000)+'0';
               BLX_Data.LONG_WORD %= 1000;
               Tagged_Data[++by_Lan]=(BLX_Data.LONG_WORD / 100)+'0';
               BLX_Data.LONG_WORD %= 100;
               Tagged_Data[++by_Lan]=(BLX_Data.LONG_WORD / 10)+'0';
               Tagged_Data[++by_Lan]=(BLX_Data.LONG_WORD % 10)+'0';
               break;        
          case MAS_Version://==>7,0,0,21
               Tagged_Data[++by_Lan]=0x05;//==> n
               Tagged_Data[++by_Lan]=0x10;//==> Manufacturer (MFG)
               Tagged_Data[++by_Lan]=0x01;//==> ClassIdentifier (CID)
               Tagged_Data[++by_Lan]=0x05;//==> Model
               Tagged_Data[++by_Lan]=UCB_SoftwareVerRF;//==> Version
               Tagged_Data[++by_Lan]=0x00;//==> Release               
               break;        
          case MAS_InfoExt://==>7,0,0,25
               Tagged_Data[++by_Lan]=21;//==> n
               Tagged_Data[++by_Lan]=1;
               BLX_Data.LONG_WORD=_ClubID;//==>63:JHT / 58:NARD
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[0];//==> Club id (����ģʽ�趨)
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[1];
               Tagged_Data[++by_Lan]=0x05;//==> Machine type id (�Ա�)
               Tagged_Data[++by_Lan]=0;
               Tagged_Data[++by_Lan]=UCB_SoftwareVerRF;//==> Console rev
               Tagged_Data[++by_Lan]=0;
               Tagged_Data[++by_Lan]=0;
               Tagged_Data[++by_Lan]=0;
               Tagged_Data[++by_Lan]=0;//==> i/o board revision
               Tagged_Data[++by_Lan]=0;
               Tagged_Data[++by_Lan]=0;
               Tagged_Data[++by_Lan]=0;
               //if(Digital_GetMachineType() == LCB_DCI)
               //{
               //   BLX_Data.LONG_WORD=Digtial_Get_DCIVersion() / 1000;
               //   Tagged_Data[++by_Lan]=BLX_Data.BYTE[0];//==> Daughter board
               //   Tagged_Data[++by_Lan]=BLX_Data.BYTE[1];
               //}
               //else
               //{
                  BLX_Data.LONG_WORD=Digtial_Get_MCBVersion();
                  Tagged_Data[++by_Lan]=BLX_Data.BYTE[0];//==> Daughter board
                  Tagged_Data[++by_Lan]=BLX_Data.BYTE[1];
               //}
               Tagged_Data[++by_Lan]=0x00;
               Tagged_Data[++by_Lan]=0x00;
               //if(Digital_GetMachineType() == LCB_DCI)
               //{
               //    BLX_Data.LONG_WORD=Digtial_Get_DCIVersion() % 1000;
               //    Tagged_Data[++by_Lan]=BLX_Data.BYTE[0];//==> Motor board 
               //    Tagged_Data[++by_Lan]=BLX_Data.BYTE[1];
               //}
               //else
               //{
                   Tagged_Data[++by_Lan]=0x00;
                   Tagged_Data[++by_Lan]=0x00;
               //}
               Tagged_Data[++by_Lan]=0x00;
               Tagged_Data[++by_Lan]=0x00;
               break;   
          case MAS_StatusExt://==>7,0,0,26
               Tagged_Data[++by_Lan]=0x05;//==> n
               Tagged_Data[++by_Lan]=0x01;//==> Struct Format
               BLX_Data.LONG_WORD = by_Current;
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[0];//==> dc bus current
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[1]; 
               BLX_Data.LONG_WORD = by_DC_BusVolt;
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[0];//==> dc bus voltage
               Tagged_Data[++by_Lan]=BLX_Data.BYTE[1];
               break;            
          }
      if(by_D1 == 0)
          {
          RF_TX_InformationBuffer(CMD_S_SET_BLX_DATA_RECORD);
          }
      else 
          {
          ++by_Lan;  
          }
      }
}
/*******************************************************************************
  �ϵ�ÿ1ms����һ��
*******************************************************************************/
void BLX_RXSignalCheck_Int(void)
{
  if(BLXbit.Power)
      {//==>���������������BLX��ֱ�4.5����5��ʱ��Machine information
      _PowerON_Delay++;
      if(_PowerON_Delay == 4500)
          {
          BLX_SetMachineInformation();  
          }
      else if(_PowerON_Delay > 5000)
          {
          BLXbit.Power=0;
          _PowerON_Delay=0;
          BLX_SetMachineInformation();
          } 
      }
  else
      //{//==>������ÿ6Сʱ�ش��˶���Ϣһ��
      //_PowerON_Delay++;
      //if(_PowerON_Delay >= 1000)  
      //    {//==> 1`s
          _PowerON_Delay=0;  
      //    _AutomaticPush++;    
      //    if(_AutomaticPush >= Auto_DelayTime)
      //        {
      //        _AutomaticPush=0;
      //        BLX_SetWowrkoutInformation();
      //        }
      //    }
      //}
  
  if(BLXbit.TX_SetData)
      {//==>���������������50ms
      TX_Delay++;  
      if(TX_Delay > 50)
          {
          BLXbit.TX_SetData=0; 
          TX_Delay=0;
          RF_TX();
          }
      }
  else
      {
      if(!BLXbit.TX_Start && (TX_FIFO_Start != TX_FIFO_END))
          {//==>������command���ݴ�������command����ʱ
          BLXbit.TX_SetData=1; 
          }  
      TX_Delay=0;    
      } 
  
  if(RX_Seg != 0)
      {//==>RX�ڽ��յ�����ʱ�������100ms��û��һ���������ʱ������ȡ? 
      RX_GetCheckTime++;
      if(RX_GetCheckTime > 100)
          {
          RX_Seg=0;
          RX_GetCheckTime=0;
          }
      }
  else
      {
      if(BLXbit.TX_Start)
          {//==>����command����200ms���յ��ش�ʱ������command
          RX_GetCheckTime++;
          if(RX_GetCheckTime > 200)  
              {
              BLXbit.TX_Start=0;
              RX_GetCheckTime=0;  
              _TX_POP_Sum++;
              if(_TX_POP_Sum > 5)
                  {//==>����������ζ�û�лش��ͷ�����command
                  _TX_POP_Sum=0;    
                  TX_FIFO_END++;
                  if(TX_FIFO_END > FIFO_MAX) TX_FIFO_END=0;
                  }
              }
          }
      else 
          {
          RX_GetCheckTime=0;
          }
      }
  
}
/*******************************************************************************
   ����ʱ����
*******************************************************************************/
void BLX_SetPower(void)
{
   BLXbit.Power=1;
}
/*******************************************************************************
   �ش�ֵ��ȡ��
*******************************************************************************/
void BLX_GetInformation(void)
{
  _AccTime = EEPROM_ReadInformation(EE_ACCUMULATED_TIME);
  //_AccTime += EEPROM_Read(EE_ACCUMULATED_TIMEA); 
  _AccDistance = EEPROM_ReadInformation(EE_ACCUMULATED_DISTANCE) / 100;
}
/*******************************************************************************
   Power on or Enabled set command
*******************************************************************************/
void BLX_SetMachineInformation(void)
{
  BLX_Command(MAS_Serial_Number,1);
  BLX_Command(MAS_Version,1);
  BLX_Command(MAS_InfoExt,0);
}
/*******************************************************************************
   �����˶���Ϣ
*******************************************************************************/
void BLX_SetWowrkoutInformation(void)
{
  BLX_Command(MAS_Total_Utilization_Time,1);
  BLX_Command(MAS_Total_H_Distance,1);
  BLX_Command(MAS_StatusExt,0);
  BLX_Command(ACM_Program,0);
}
/*******************************************************************************
   ����ʱȡRF Radio״̬������
*******************************************************************************/
void BLX_Get_RF_RadioStatus(void)
{
  RF_TX_InformationBuffer(CMD_S_GET_BLX_STATUS);
  RF_TX_InformationBuffer(N_HUB_CONNECT);
  RF_TX_InformationBuffer(N_HUB_DISCONNECT);
  //RF_TX_InformationBuffer(N_OFFLOAD_COMPLETE);
}
/*******************************************************************************
   �趨RF SERVICE�ش�״̬
   RF_Service_None : �ر�BLX���� 
   RF_Service_OFF  : һ�㴫�� 
   RF_Service_ON   : һ�㴫��+Workout END��Ϣ
*******************************************************************************/
void BLX_Set_RF_Service(u8 by_Dat)
{
  if(by_Dat > RF_Service_OFF) by_Dat=RF_Service_OFF;
  _AMserviceMode=by_Dat;
}
u8 BLX_Get_RF_Service(void)
{
  return _AMserviceMode;
}
/*******************************************************************************
   ȡ�ò���ֵ
*******************************************************************************/
void BLX_Get_SerialNumber(void)
{
  _SerialNumber=EEPROM_ReadInformation(EE_SerialNumber);
}
void BLX_Get_ClubIDNumber(void)
{
  _ClubID=EEPROM_ReadInformation(EE_CLUB_ID);
}



