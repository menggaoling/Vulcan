/* Includes ------------------------------------------------------------------*/
#include  <stdio.h>
#include  <stdarg.h>
#include  <string.h>
#include  "Console.h"
#include  "General.h"
#include  "Beep.h"
#include "heart.h"
#include  "led.h"
#include  "keyboard.h"
#include  "Digital.h"
#include  "Show.h"
#include  "UART.h"
#include  "EEPRom.h"
#include  "timer.h"
#include  "C_safe.h"
#include  "TV.h"
//#include  "cab.h"
#include  "Flash.h"
#include  "Boot.h"
#include  "UsbFunction.h"
#include  "HT1381.h"
//#include  "Voice.h"
#include  "LCB_Support.h"
//#include  "WiFi.h"
#include  "RF900.h"
#include  "WebFile.h"
#include  "KeyFunction.h"
#include  "EEPRom_Language.h"
#include "EngineerMode.h"
// 2014.11.24
#include  "wifi_Digital.h"
#include  "WiFi_Information.h"
#include  "wifi_interface.h"
//
#include "Show_Functions.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

u8 Default_Program(void);
u8 Default_ProgramDF(u8 by_DF);
u8 Workout(void);
u8 CSAFE_ID(void);
u8 Main_MotorStatusCheckMessage(u8 by_D);
char ErrorCodeDisplay_Mode(char by_D);
//void RCC_Configuration(void);
//void NVIC_Configuration(void);
void Open_UCB_PSW(void);
void Message_TestMode(void);
void Workout_Information(u8 by_D);
void Workout_CsafeWorkoutInformation(char by_D);
u8 Workout_LevelChange(void);
u8 BeltStop_StatusCheck(void);
u8 Main_EnterLogin(void);
void BackupOriginalSoftware(void);
unsigned char HeartRate_HomeMode(void);

// start v2.1-13
void main_RSCUcheck(void);
// end 
/* Private functions ---------------------------------------------------------*/
void MX_GPIO_Init(void);
void MX_TIM_Init(void);



void System_Initial(void)
{
//       TIM_OCInitTypeDef  TIM_OCInitStructure;
       //TIM_ICInitTypeDef  TIM_ICInitStructure;
       //ADC_InitTypeDef  ADC_InitStructure;
       UART_HandleTypeDef huart;
       
       
       huart.Instance = USART1;
       HAL_UART_MspDeInit(&huart);
       huart.Instance = USART2;
       HAL_UART_MspDeInit(&huart);
       huart.Instance = USART3;
       HAL_UART_MspDeInit(&huart);
       huart.Instance = UART4;
       HAL_UART_MspDeInit(&huart);
       huart.Instance = UART5;
       HAL_UART_MspDeInit(&huart);
	/* System Clocks Configuration */
	//RCC_Configuration(); // Ƶ���趨,�������ⲿ BUS  
 
       HAL_Init();  
        
	/* Enable GPIOC clock */
        /*Ҫ���ⲿ�ж���Ҫ���� RCC_APB2Periph_AFIO �趨*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB \
//                             | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1  | RCC_APB2Periph_AFIO \
//                             , ENABLE);//RCC_APB2Periph_ADC1 | 
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | \
//                               RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5 | RCC_APB1Periph_PWR,ENABLE);
	
        
       MX_GPIO_Init();
       MX_TIM_Init();
       IO_SleepMode_LED(GPIO_PIN_SET);
       //UsartInitial();
       
       /* NVIC Configuration */
//       NVIC_Configuration(); 
       
       /* Setup SysTick Timer for 1 msec interrupts  */
//       SysTick_Config(SystemCoreClock / 1000);
       //===============================================================================================================  
      
        /* Time base configuration */
        //TIM_TimeBaseStructure.TIM_Period = 0xFFFF;          
        //TIM_TimeBaseStructure.TIM_Prescaler = 0x02;       
        //TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
        //TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
        //TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
        /* Prescaler configuration */
        //TIM_PrescalerConfig(TIM2,0x0168,TIM_PSCReloadMode_Immediate);
        /* Output Compare Active Mode configuration: Channel1 */
        //TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;          
        //TIM_OCInitStructure.TIM_Pulse = 75;//==> 75/(100KHz) = 750us  
        //TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
        //TIM_OC1Init(TIM2, &TIM_OCInitStructure);
        //TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
        /* Output Compare Active Mode configuration: Channel2 */
        //TIM_OCInitStructure.TIM_Pulse = 100;  
        //TIM_OC2Init(TIM2, &TIM_OCInitStructure);//==>1ms 
        //TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
        /* TIM2 enable counter */
        //TIM_Cmd(TIM2, ENABLE);
        /* TIM IT enable */
        //TIM_ITConfig(TIM2,TIM_IT_CC1 | TIM_IT_CC2,ENABLE);// | TIM_IT_CC3 
//==============================================================================================================        
//==============================================================================================================        
//====> ADC�ж��趨
//==============================================================================================================        
  /* ADC1 configuration ------------------------------------------------------*/
  /*
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  ADC_Init(ADC1, &ADC_InitStructure);
  */
  /* Set injected sequencer length */
  //ADC_InjectedSequencerLengthConfig(ADC1, 2);
  /* ADC1 injected channel Configuration */ 
  //ADC_InjectedChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_239Cycles5);//PC5
  //ADC_InjectedChannelConfig(ADC1, ADC_Channel_14, 2, ADC_SampleTime_239Cycles5);//PC4
  /* ADC1 injected external trigger configuration */
  //ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);
  /* Enable automatic injected conversion start after regular one */
  //ADC_AutoInjectedConvCmd(ADC1, ENABLE);
  /* Enable JEOC interupt */
  //ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
  /* Enable ADC1 */
  //ADC_Cmd(ADC1, ENABLE);
  /* Enable ADC1 reset calibaration register */   
  //ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  //while(ADC_GetResetCalibrationStatus(ADC1));
  /* Start ADC1 calibaration */
  //ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  //while(ADC_GetCalibrationStatus(ADC1));
  /* Start ADC1 Software Conversion */ 
  //ADC_SoftwareStartConvCmd(ADC1, ENABLE);    
  
}



static struct
{
    unsigned long key       : 1;
    unsigned long key1      : 1;
    unsigned long key2      : 1;
    unsigned long NumberKey : 1;
    unsigned long Stop      : 1;
    unsigned long ShowChang : 1;
    unsigned long WarmUp    : 1;
    unsigned long CoolDown  : 1;
    unsigned long THR_Over  : 1;
    unsigned long THR_Safe  : 1;
    unsigned long WorkEnd   : 1;
    unsigned long Enter     : 1;
    unsigned long KeyColse  : 1;
    unsigned long ErrorCode : 1;
    unsigned long GO        : 1;
    unsigned long SafeKey   : 1;
    unsigned long FitTest   : 1;
    unsigned long Pause     : 1;
    unsigned long FirstTime : 1;
    unsigned long FirstHR : 1;
    unsigned long Choose    : 1;
    unsigned long ChangProgram : 1;
    unsigned long PowerON : 1;
    unsigned long ShowCoolDownStart : 1;
    unsigned long QuickStart : 1;
    unsigned long FitTest_END : 1;
    unsigned long SafeKeyStatus : 1;
    unsigned long USB_SaveSummary : 1;
    unsigned long RF900Show : 1;// 100.09.28
    unsigned long RF900ShowStart : 1;// 100.09.28
    unsigned long ShowInformation : 1;
    unsigned long HR_Avg : 1;
    unsigned long WiFi_SendSummary : 1;
    unsigned long HRT_Based : 1;
    unsigned long HR_Blink : 1;
    unsigned long UseNumKey : 1;
    // 2014.11.24
    unsigned long ErpMode : 1;
    unsigned long machineIdentification : 1;// WiFi��̨��֤��ȡ��
    //
    unsigned long Check : 1;
    // 
    unsigned long AutoCheckUpdate : 1;// v2.1-13
    unsigned long CanUpdateSoftware : 1;// v2.2-5
    unsigned long CanUpdateSoftwareStare : 1;// v2.2-5
    unsigned long HJS_ErrorCheckDisplay : 1;
    unsigned long KEY_ErrorCheckDisplay : 1;

}bit;

#define C_POWER_ON 0x5665

u8 by_Address=M_RESET;
u8 by_WorkoutDFNewProgram;//==>��Ϊ�˶��������趨��Program���жϻ�׼,�ݴ����趨��
u8 by_WFI_Show=0;
u8 Show_Loop=1;
u8 _Message_Status = 0;
u8 _Table_Custom[4];

//==>�� Digital.c �ش�����
extern u8 Digital_CeckNonInUser;
extern u8 by_NonInUserTime;// 2014.02.12-1
extern u8 by_InUserModeMotoSafeTime; // 2014.02.12-1
// 2014.11.24 �ظ���WiFiʹ��
extern char _WorkoutFinish;
extern char _NowWorkoutStatus;



u32 by_HoldBuffer;
unsigned char _Display_Logo;

/*******************************************************************************
* Function Name  : USB_SaveWorkoutSummary()
*                  �˶�������ר�Ŵ����˶���Ϣʹ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*
void USB_SaveWorkoutSummary(void)
{
  if(UsbFunction_USB_Status() == 1)
  {//==>���������
      if(!bit.USB_SaveSummary)
      {//==>��û�н��й����涯��ʱ
          //----------------------------------------------------------------------------
          // Set USB workout Summary Data to buffer
          if(Console_SpeedUnit(Data_Get,0) == C_KM)
              UsbFunction_SetWorkoutSummaryNumber(IN_Distance,(u16)((float)Console_Get_Distance() * 1.609) / 10);
          else
              UsbFunction_SetWorkoutSummaryNumber(IN_Distance,Console_Get_Distance()/10);
          UsbFunction_SetWorkoutSummaryNumber(IN_Averpace,Console_Get_AvgPace());
          UsbFunction_SetWorkoutSummaryNumber(IN_Calories,Console_Get_Calories());
          UsbFunction_SetWorkoutSummaryNumber(IN_TotalTime,Console_Parameter(Info_AccumulatedTime));
          //----------------------------------------------------------------------------
          Led_Set_Reset();
          bit.USB_SaveSummary = 1; 
          if(UsbFunction_WorkoutSummary() == C_WriteOK)
          {//==>��ɴ���
              while(!Led_Get_String_End())
              {
                  UsbFunction_USB(0x500);
                  Show_Message(Message_WorkoutSaved,0,0,Show_Auto);
              }
          }
          else
          {
              while(!Led_Get_String_End())
              {
                  UsbFunction_USB(0x500);
                  Show_Message(Message_WorkoutSaveFailed,0,0,Show_Auto);
              }
          }
      }
  }
 
}
*/
/*******************************************************************************
* Function Name  : Safe key check
* Input          : None
* Output         : GPIOB Pin 12 signal
* Return         : None
*******************************************************************************/
u8 Check_SafeKey(void)
{
    if(Digital_CheckSafeKeyStatus())
    {//==>��ȡ��0x02B2������ʱ
        Digital_ClearSafeKeyStatus();
        bit.SafeKeyStatus=1;
        by_WorkoutDFNewProgram=0;
        bit.SafeKey=1;
        return 0;
    }
    if(IO_SafeKey()) 
    {//==>�ſ� eStop key
        if(bit.SafeKeyStatus)
        {//==>����һ�ηſ�ʱ��������command����LCB��ʼ��,��Ž������ض���
            bit.SafeKeyStatus=0;
            Digital_CommandBufferReset();//==>�����¿�  
        }
        Digital_eSTOPKeyDelayClear();
    }
    else
    {//==>���� eStop key
        if(!bit.SafeKeyStatus)
        {//==>����һ�ΰ�����δȡ��0x02B2������ʱ
            if(Digital_eSTOPKeyDelay())
            {//==>���� ms�������س���
                bit.SafeKeyStatus=1;
                by_WorkoutDFNewProgram=0;
                bit.SafeKey=1;
                return 0;
            }
        }
        else return 0; 
    }
    return 1; 
}
/*******************************************************************************
* Function Name  : PowerOn_ERPsWakeUp
* Description    : ����ǿ����һ�λ��ѻ���,ȷ���Ǳ������¿��ǿ��Զ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PowerOn_ERPsWakeUp(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  char by_Enter = 0;
  
  Digital_CommandStart(1);// ֹͣ�Զ�������
  while(by_Enter == 0)
  {
      GPIO_InitStructure.Pin = GPIO_PIN_2;
      GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStructure.Pull = GPIO_NOPULL;
      GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
      IO_Set_Digital_CTL();
      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
      Timer_Counter_Clear(0);
      while(!Timer_Counter(T_STEP,0,5));     
      Digital_Initial();
      Digital_CommandStart(1);// ֹͣ�Զ�������
      UsartInitial();
      Digital_Command(CmdEUPsMode,0x00);
      Timer_Counter_Clear(0);
      while(!Timer_Counter(T_STEP,0,5))
      {
          if(LCB_Get_EUPsState() != 0 || by_HoldBuffer >= 2)
          {// LCB��Ӧ���ѻ���3��ʧ��
              by_Enter = 1;
              break;
          }
          /*
          if(by_HoldBuffer >= 3)
          {// ����3��ʧ��
              LED_5V_Set();
              while(1) Show_Message(Message_LCBNoWakeUp,0,0);
          }
          */
      }
      by_HoldBuffer++;
  }
  Digital_CommandBufferReset();//solve after power on error status has been clear but by_GetDkip not clear 
  Digital_CommandStart(0);// �����Զ�������
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ucb_main(void)
{ 
  System_Initial();	
  Open_UCB_PSW();//==>����ʱ����򿪵Ŀ���
  //Voice_Reset();
  Heart_Initial();
  Led_Initial();  
  BackupOriginalSoftware();
  //Digital_Initial();
  //UsartInitial();
  if(EEPROM_Initial() == C_FAIL)
  {//==>���ж��ڴ��Ƿ����ܵ�
      BEEP_Proce(6,3,3);
      while(BEEP_Get_OK());
      
      if(EEPROM_GetMemoryErrorStatus() != 0 ) 
      {
          Digtial_Set_ErrorCode(0x0301);//==>Flash / EEPROM Error
          Digital_ErrorCode_ChangToAddress();
          Digtial_Set_ErrorCode(0);
          Led_String("Error:    ",0,0);
          Led_Special(2,40,0x0301);
          
          while(1) ;
      }
  }
  if(Console_PowerOn(Data_Get,0)!= C_POWER_ON)//==>�жϷ���ROM��������Ƿ���ʧ;��������������
  {
      Console_PowerOn(Data_Set,C_POWER_ON);
      by_Address=M_RESET;
  }
  // ����ǿ����һ�λ��ѻ���,ȷ���Ǳ������¿��ǿ��Զ�����.
  PowerOn_ERPsWakeUp();
  //
  CSAFE_Initial();
  TV_Initial();
  // ======> CSAFE ����趨
  CSAFE_Initial();
  CSAFE_SetInformation(_Set_Manufacturer,0x10);   
  CSAFE_SetInformation(_Set_CID,0x02);  
  CSAFE_SetInformation(_Set_Model,CSAFE_Model);
  CSAFE_SetInformation(_Set_Version,UCB_Software_Ver);        
  CSAFE_SetInformation(_Set_Release,UCB_Software_Rel);  
  CSAFE_Power_On();
  //
  Keyboard_Initial();
  /*==========================================================================
    =============> �����ο���������500ms����command���¿����� <===============
    =============> ͬʱ��HT1381ʱ��IC�������Ƿ������ж�       <===============
    ==========================================================================*/
  Timer_Counter_Clear(0);
  SHT1381 time;//==>HT1381ʱ��IC Dataָ��
  time.Seconds=0;
  time.Minutes=0;
  time.Hours=0;
  time.Year=0;
  time.Month=0;
  time.Date=0;
  time.Day=0;
  while(!Timer_Counter(T_STEP,0,5))
  {
      
  }
  ReadHT1381(&time);//==>��ȡHT1381��ʱ��ֵ
  if(time.Seconds > 0x59)
  {//==>�ж�HT1381 Oscillator�Ƿ�����,��û�о��ȸ����趨ֵʹ������
      time.Seconds=0x00;
      time.Minutes=0x00;
      time.Hours=0x10;
      time.Year=0x18;
      time.Month=0x09;
      time.Date=0x19;
      time.Day=0x07;
      WriteHT1381(&time);
  }  
  /*������ʼ�� Daughter Board*/
  Digital_Command(CmdInitial,0);
  Digital_Command(CmdSetWorkStatus,0);
  Digital_Command(CmdSetWorkStatus,1);
  Digital_Command(CmdGetVersion,0);
  Digital_Command(CmdGetInclineLocation,0);
  
#ifdef _DebugClose 
  Timer_Counter_Clear(0); 
  u8 by_QQQ=0;
  while(by_QQQ < 10)
  {//==>ȷ���¿��ͺ�
      if(Timer_Counter(T_STEP,0,20))
      {
          Digital_Command(CmdGetVersion,0);  
          Timer_Counter_Clear(0); 
          by_QQQ++;
      }
      if(by_QQQ > 2)
      {//==>ȷ�ϼ��κ����޷�ȷ�Ͼ���ΪĬ��ֵ
          Digital_SetMachineType(LCB_DELTA_C3);
          by_QQQ=100;  
      }
      else if(Digital_PowerON_MachineTypeCheck()==1)
      {
          by_QQQ=100;  
      }
  }
  by_WorkoutDFNewProgram=0;
  bit.PowerON=0;
  bit.SafeKey=1;
  if(EEPROM_ReadInformation(EE_MachineType_LCB) != Digital_GetMachineType())
  {// ��ͬ����������Ҫ�л� RPM Ԥ�����
    EEPROM_SaveWorkInformation(EE_MachineType_LCB,Digital_GetMachineType());
    if(Digital_GetMachineType() == LCB_DELTA_C3)
    {// ̨��ҵ���ҵȺ��
      EEPROM_SaveWorkInformation(EE_MinRPM_Parameter,DELTA_MinRPM);
      EEPROM_SaveWorkInformation(EE_MaxRPM_Parameter,DELTA_MaxRPM);
    }
    else
    {// ���հ�
      EEPROM_SaveWorkInformation(EE_MinRPM_Parameter,TOPTEK_MinRPM);
      EEPROM_SaveWorkInformation(EE_MaxRPM_Parameter,TOPTEK_MaxRPM);
    }
  }
  // ����TM�Ļ���ʱ����Ҫ�������û�Ԥ��
  if(EEPROM_ReadInformation(EE_MachineType) != MachineType_TM)
  {
      EEPROM_Save_Initial();
  }
  //
  EEPROM_Read_Initial();
  Console_Initial();
  if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8))
  {//==>BURN IN����ൽLOWѶ��ʱ��ȫ��
      u8 by_A=0;
      bit.GO=0;
      
      Digital_Set_AutoCheck(0);//==>�뿪�Զ�����ģʽ
      Digital_Set_Work(1);//==>���˶�ģʽ��
      Timer_Counter_Clear(0);
      while(!Timer_Counter(T_STEP,0,50)) ;
      Console_Information(Info_WorkoutSpeed,Data_Set,100);
      Digital_Set_DigitalSpeedTarget(1);//==>��ʼ׷��Ŀ���ٶ�
      Console_Information(Info_WorkoutIncline,Data_Set,0);
      Timer_Counter_Clear(0);
      while(1)
      {
          Led_Set_Target(Display_ON,1);
          if(Timer_Counter(T_STEP,0,100))
          {
              Timer_Counter_Clear(0);
              if(bit.GO)
              {
                  if(by_A > 1) 
                  {
                      by_A--;
                  } 
                  else
                  {
                      by_A=1;
                      bit.GO=0;
                  }
              }
              else
              {
                  by_A++;
                  if(by_A > 30) 
                  {
                      by_A--;
                      bit.GO=1;
                  }
              }
              Console_Information(Info_WorkoutIncline,Data_Set,by_A); 
          }    
      }    
  }
#endif
  
  bit.SafeKeyStatus = 0;
  //=================================================================
  TIM4->CCR1 = CCR1_Val_Close ;//==> PWM duty����ʹ��(PB.6)
  //
  IO_USB_SetILIM();//==>USB power switch on
  bit.ErrorCode = 0;
  
  // 2014.11.24 
  //WiFi module initial
  UsartInitial_WiFi();
  WiFi_Information_Initial();
  Digital_Initial_Data_wifi();
  // 2015.01.23
  //RFID_GATRM310_HW_Initial();
  //
  bit.GO = 0;
  if(Language_EEPROMCheck(_Lang_UcbMachineType_) != _Lang_Success_)
  {// �ȶ�����Ļ����ַ�����EEPROM�Ļ����ַ����Ƿ�һ��,����һ��ʱ
      if(EEPROM_ReadInformation(EE_LANGUAGE) != L_English) 
      {
          EEPROM_SaveWorkInformation(EE_LANGUAGE,L_English);
      }
  }
  //

  
  bit.HJS_ErrorCheckDisplay=0;  
  bit.KEY_ErrorCheckDisplay=0;
  // start v2.2-5
  bit.AutoCheckUpdate=0;
  bit.CanUpdateSoftware=0;
  bit.CanUpdateSoftwareStare=0;
  // end v2.1-13
  //HRM_Init();
  while(1)//{HRM_State_Machine();}
  {
      switch(by_Address)
      {
          default:
          case M_RESET:
                   // USB change mode
                   IO_USB_OnlyCharge();
                   //
                   EngineerMode_SetMode(0);
                   Led_Set_Target(Display_OFF,0);                    
//                   LED_MATRIX(_BigMatrix_Clear);// ���������ʼ������
                   if(!Check_SafeKey())
                   {
                       by_WorkoutDFNewProgram=0;
                       bit.SafeKey=1;
                       if((Digital_GetStatus() & BIT0)||(Digital_GetStatus() & BIT1))// 2014.07.16
                       {
                           Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ
                       }
                   }
                   Digital_Set_Work(1);//==>���˶�ģʽ��
                   Digital_Set_DigitalSpeedTarget(0);//==>�ٶȹ���
                   if(!bit.PowerON && Console_Audio(Data_Get,0)== Audio_TV)
                   {
                       bit.PowerON=1;
                       Led_Set_Target(Target_TV_ON,0);
                       Led_Set_Target(Target_TV_OFF,1);
                   }
                   if(bit.ErrorCode)
                   {//==>���д��������ʱ�ʹ���
                       bit.ErrorCode = 0;
                       Digital_ErrorCodeUpdate();
                       EEPROM_SaveError();//==>�ٽ����������
                   }
                   if(by_WorkoutDFNewProgram == 0)
                   {//==>���������˶��������趨��programʱ
                       Timer_Initial();
                       BEEP_Proce(4,3,3);
                       while(BEEP_Get_OK()); 
                       if(Language_LanguageChecking(EEPROM_ReadInformation(EE_LANGUAGE)) != _Lang_Success_)
                       {
                           if(EEPROM_ReadInformation(EE_LANGUAGE) != L_English)
                           {
                               EEPROM_SaveWorkInformation(EE_LANGUAGE,L_English);
                           }
                       }
                       if(Pattern_CheckingPattern(EEPROM_ReadInformation(EE_Logo)) != _Lang_Success_)
                       {
                           if(EEPROM_ReadInformation(EE_Logo) != Logo_MATRIX)
                           {
                               EEPROM_SaveWorkInformation(EE_Logo,Logo_MATRIX);
                           }
                       }
                       if(bit.SafeKey)
                       {// Press eSTOP
                           bit.SafeKey=0;
                           //Led_FanControlReset();//==>�رշ���
                           while(!Check_SafeKey())
                           {
                               Show_Message(Message_PressToRelease,0,0,Show_Auto);
                               Key_Proce();
                               //UsbFunction_USB(0x500);
                               //UsbFunction_SoftwareUpdate(0);//==>����.bin��Ѱģʽ
                           }
                           Led_Initial(); 
                       }
                       if(Digtial_ElevationVariationCheck(0) == 1)
                       {// ������ʵ��ֵ��Ŀ��ֵ�������,�����֮ǰ���趨��Ŀ��ֵ����Ȼ��������ȡ��
                           Digital_Clear_ElevationOld();//==>������ɵ�AD�ݴ�ֵ
                           Console_Information(Info_WorkoutIncline,Data_Set,0);
                       }
                       if(EEPROM_ReadInformation(EE_TVPower) == _OFF && TV_GetPowe_OnOff() == 1)
                       {//==>��TV ON ʱ�� OFF
                           TV_SetCommand(TVCom_POWER,TVData_PowerOFF);
                       }
                   }
                   else
                   {
                       Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ
                   }
                   Digital_Set_AutoCheck(0);//==>�뿪�Զ�����ģʽ
                   EEPROM_Read_Initial();
                   Console_Initial();
                   //Heart_Initial();
                   Heart_Set_Target((220-Console_Information(Info_AGE,Data_Get,0))*Console_Information(Info_PERCENT,Data_Get,0)/100);//Console_AGE(Data_Get,0) Console_THRPercent(Data_Get,0)
                   by_Address=M_PROGRAM;    
                   Show_SetSummaryNumber(0);
                   //Led_FanStart(0);
                   Workout_CsafeWorkoutInformation(1);
                   Console_WorkoutStatus(Data_Set,0);
                   if(Console_Audio(Data_Get,0) == Audio_RF900)
                   {
                       CSAFE_SetInformation(_Set_CommunicationMode,Audio_RF900);
                       RF900_Initial();
                   }
                   else
                   {
                       CSAFE_SetCheckAnyKeyStatus(0);// ����������жϹ���
                   }
                   // ��PCTV / RF900��Դ
                   if(Console_Audio(Data_Get,0) == Audio_PCTV ||\
                      Console_Audio(Data_Get,0) == Audio_RF900 )
                   {
                       IO_Ext_Audio();// ��Դ
                       IO_OpenAudio();// SHUTDOWN
                       // PWM duty����ʹ��(PB.6)
                       TIM4->CCR1 = Console_GetVolumeNumber(Console_GetUserVolume());
                       if(Console_Audio(Data_Get,0) == Audio_RF900)
                       {
                           RF900_MuteClear();// ǿ��ȡ������
                       }
                   }
                   else if(Console_Audio(Data_Get,0) == Audio_OFF)
                   {
                     TIM4->CCR1 = CCR1_Val_Close;//==> PWM duty����ʹ��(PB.6)
                     IO_CloseAudio();//==>SHUTDOWN
                   }
                   by_InUserModeMotoSafeTime = InUserMotoCheckTime;
                   // 2014.11.24
                   _NowWorkoutStatus = _WiFi_WorkoutState_Pause;
                   _WorkoutFinish = 0;
                   Timer_Counter_Clear(_Time_RSCUCheck);
                   break;
          case M_PROGRAM:
                   switch(by_Address = Default_Program())
                   {
                       case M_RESET:
                              CSAFE_Work_Exit();
                              EEPROM_SaveWorkInformation(EE_HJS_total,Console_HJS(Data_Get,0));
                              break;
                       case M_START:
                              CSAFE_SetInformation(_Set_StartWorkout,1);//=>����
                              break;
                   }
                   break;
          case M_START: 
                   Timer_Close(Time_AllClose);//==>�ر�����Time������
          case M_ReWork:  
                   //Led_FanStart(1);
                   if(Workout() == M_RESET) 
                   {
                       Show_Message(Message_Blank,0,0,Show_Blank);
                       EEPROM_SaveWorkInformation(EE_ACCUMULATED_TIME,Console_Parameter(Info_AccumulatedTime));
                       EEPROM_SaveWorkInformation(EE_ACCUMULATED_DISTANCE,Console_Get_Distance());//+Console_ChangeDistanceBuffer(1)
                       EEPROM_SaveWorkInformation(EE_HJS_total,Console_HJS(Data_Get,0));
                       /*service mode ��������*/
                       EEPROM_WriteInformation(EE_RunningBeltCurrentAccHours,EEPROM_ReadInformation(EE_RunningBeltCurrentAccHours)+Console_Parameter(Info_AccumulatedTime));
                       EEPROM_WriteInformation(EE_RunningBeltCurrentAccMiles,EEPROM_ReadInformation(EE_RunningBeltCurrentAccMiles)+Console_Get_Distance());
                       
                       EEPROM_WriteInformation(EE_RunningDeckCurrentAccHours,EEPROM_ReadInformation(EE_RunningDeckCurrentAccHours)+Console_Parameter(Info_AccumulatedTime));
                       EEPROM_WriteInformation(EE_RunningDeckCurrentAccMiles,EEPROM_ReadInformation(EE_RunningDeckCurrentAccMiles)+Console_Get_Distance());
                       
                       EEPROM_WriteInformation(EE_MCBCurrentAccHours,EEPROM_ReadInformation(EE_MCBCurrentAccHours)+Console_Parameter(Info_AccumulatedTime));
                       EEPROM_WriteInformation(EE_MCBCurrentAccMiles,EEPROM_ReadInformation(EE_MCBCurrentAccMiles)+Console_Get_Distance());
                       
                       EEPROM_WriteInformation(EE_MoterCurrentAccHours,EEPROM_ReadInformation(EE_MoterCurrentAccHours)+Console_Parameter(Info_AccumulatedTime));
                       EEPROM_WriteInformation(EE_MoterCurrentAccMiles,EEPROM_ReadInformation(EE_MoterCurrentAccMiles)+Console_Get_Distance());
                       
                       EEPROM_WriteInformation(EE_HeadphoneCurrentAccHours,EEPROM_ReadInformation(EE_HeadphoneCurrentAccHours)+Console_Get_Distance());
                       EEPROM_WriteInformation(EE_HeadphoneCurrentInsertsTimes,Console_HJS(Data_Get,0));
                       
                       EEPROM_WriteInformation(EE_Prev_MaintenanceCurrentAccHours,EEPROM_ReadInformation(EE_Prev_MaintenanceCurrentAccHours)+Console_Parameter(Info_AccumulatedTime));
                       EEPROM_WriteInformation(EE_Prev_MaintenanceCurrentAccMiles,EEPROM_ReadInformation(EE_Prev_MaintenanceCurrentAccMiles)+Console_Get_Distance());
                       /***********************/
                       by_Address=M_RESET;
                   }   
                   
                   break;
          case M_CSAFE:  
                   Timer_Close(Time_AllClose);//==>�ر�����Time������
                   switch(by_Address = CSAFE_ID()) 
                   {
                       case M_RESET:
                              CSAFE_Work_Exit();
                              break;
                   }
                   break;
      }
  }
}

//==============================================================================
//==> Workout_CsafeWorkoutInformation()
//==> CSAFE���˶���Ϣ�����ṩ�ظ�ʹ��
//==> by_D : 1=Reset / 0=Information
//==============================================================================
void Workout_CsafeWorkoutInformation(char by_D)
{
  u16 _Buffer = Console_Get_Distance();
    
  if(Console_SpeedUnit(Data_Get,0) == C_KM)
  {
      CSAFE_SetInformation(_Set_DistanceUnits,0x23);//0.01����
      CSAFE_SetInformation(_Set_Units,0x00);
      CSAFE_SetInformation(_Set_SpeedUnits,0x31);//0.1����/hr
      CSAFE_SetInformation(_Set_WeightUnits,0x27);//kg
  }
  else
  {
      CSAFE_SetInformation(_Set_DistanceUnits,0x03);//0.01Ӣ��
      CSAFE_SetInformation(_Set_Units,0x01);
      CSAFE_SetInformation(_Set_SpeedUnits,0x11);//0.1Ӣ��/h
      CSAFE_SetInformation(_Set_WeightUnits,0x07);//bls
  }
  //
  if(by_D == 0)
  {// Information
      if(Console_SpeedUnit(Data_Get,0) == C_KM)
      {
          _Buffer = (u16)((float)_Buffer * 1.609);
      }
      CSAFE_SetInformation(_Set_WorkoutDistance,_Buffer / 10);
      CSAFE_SetInformation(_Set_Gender,Console_Information(Info_GENDER,Data_Get,0));//Console_Gender(Data_Get,0)
      CSAFE_SetInformation(_Set_WorkoutTime,Console_Get_CsafeAccTime());
      CSAFE_SetInformation(_Set_WorkoutCalories,Console_Get_Calories());
      CSAFE_SetInformation(_Set_Level,Console_Information(Info_LEVEL,Data_Get,0));//Console_Level(Data_Get,0)
      CSAFE_SetInformation(_Set_Speed,Console_Information(Info_WorkoutSpeed,Data_Get,0)/10);//Console_Speed(Data_Get,0)
      CSAFE_SetInformation(_Set_Incline,Console_Information(Info_WorkoutIncline,Data_Get,0) * 10 / 2);//Console_Incline(Data_Get,0)
      CSAFE_SetInformation(_Set_MET,Console_Get_METs());
      CSAFE_SetInformation(_Set_Watt,(u16)((float)Console_Get_Calories_Hour() / 4.2));
      CSAFE_SetInformation(_Set_Program,Console_ProgramMode(Data_Get,0)); 
      CSAFE_SetInformation(_Set_Weight,Console_Information(Info_WEIGHT,Data_Get,0));//Console_Weight(Data_Get,0)
      CSAFE_SetInformation(_Set_AGE,Console_Information(Info_AGE,Data_Get,0));//Console_AGE(Data_Get,0)
      CSAFE_SetInformation(_Set_HRCur,Heart_Get_Heart()); 
      CSAFE_HRInformation();
  }
  else
  {   // ��DAPI��ȡ��ƽ��ֵ,������Ǳ����ú�����Ϊ0,���������岻ȻDAPI��ֻץ��0
      CSAFE_SetInformation(_Set_HRAvg,0);
  }
}
/*******************************************************************************
* Function Name  : Main_MotorStatusCheckMessage()
* Description    : �����״̬��û��ȫֹͣʱ������ʾ�ַ���һ��
* Input          : 0=Not Workout / 1=Workout
* Output         : None
* Return         : None
*******************************************************************************/
u8 Main_MotorStatusCheckMessage(u8 by_D)
{
    u8 by_Status = M_NEXT;
    u8 by_KeyBuffer;
    
    Led_Set_Reset();
    while(by_Status == M_NEXT)
    {
        Show_Message(Message_Restarting,0,0,Show_Auto);
        if(Led_Get_String_End() == 1)
        {
            by_Status = M_NONE;
        }
        if(Timer_Counter(T_STEP,7,50))
        {
            bit.QuickStart=1;
        }
        by_KeyBuffer = Key_Proce();
        if(bit.QuickStart && !Digital_Get_RPMStatus())
        {//==>���ֹͣת��
            if(by_KeyBuffer == K_GO) 
            {
                by_Status = M_START;
            }    
            if(by_Address == M_ReWork)
            {
                if(by_KeyBuffer == K_PAUSE) by_Status = M_START;
            }    
        }
        if(!Check_SafeKey()) 
        {
            by_Status=M_RESET;
        }
    }
    Led_String("                  ",0,0);
    return by_Status;
}
/*******************************************************************************
* Function Name  : Default_Program()
* Input          : None
* Output         : None
* Return         : by_Status
*******************************************************************************/
const unsigned char _ProgramInfor[15][4]=
{ // ָʾ�ƴ��� / Program��ʾ���� / Program�л���ʾ���� / Ԥ��Levelͼ��
  //TM_Manual 
  {Target_Manual      ,Message_Manual      ,Message_ResetForManual,1}, 
  {Target_Training    ,Message_5k          ,Message_ResetForTrainingWorkouts,6},
  {Target_Training    ,Message_10k         ,Message_ResetForTrainingWorkouts,6},
  {Target_TargetHR    ,Message_TargetHR    ,Message_ResetForHR,1},
  {Target_FatBurn     ,Message_FatBurn     ,Message_ResetForFatBurn,11},   
  {Target_RollingHills,Message_RollingHills,Message_ResetForRolling,11}, 
  //TM_Gerkin
  {Target_FitnessTest ,Message_Gerkin      ,Message_ResetForFitTest,1},  
  {Target_FitnessTest ,Message_PEB         ,0,1}, //TM_PEB
  {Target_FitnessTest ,Message_Amry        ,0,1}, //TM_Army
  {Target_FitnessTest ,Message_Navy        ,0,1}, //TM_Navy
  {Target_FitnessTest ,Message_Airforce    ,0,1}, //TM_Airforce
  {Target_FitnessTest ,Message_Marine      ,0,1}, //TM_Marine
  {Target_FitnessTest ,Message_WFI         ,0,1},  //TM_WFI
  {Target_Training    ,Message_TraningWorkouts ,Message_ResetForTrainingWorkouts,6},
  {Target_FitnessTest ,Message_FitnessTest ,Message_ResetForFitTest,1}  
};
const struct
{
    struct
    {
        unsigned char Used : 1;     // 0:��ʹ�����ּ�, 1:ʹ��
        unsigned char Digits : 7;   // ����λ��
    }NumericKey;    
    unsigned char DisplayLine;      // ������ʾ��ʼ����
    unsigned char DisplayMode;      // ������ʾģʽ
    unsigned char Message;          // ��ʾ�ַ�������
    unsigned char InputMessage;     // ������ʾ�ַ�������
}DefaultNumber[9] = 
{
  {1,2,Window_2,2,Message_Time,Message_EnterTime},               //DF_TIME   
  {1,3,Window_2,1,Message_Weight,Message_EnterWeight},           //DF_WEIGHT 
  {1,3,Window_2,1,Message_Height,Message_EnterHeight},           //DF_HEIGHT 
  {1,3,Window_2,4,Message_Incline,Message_EnterTheIncline},      //DF_INCLINE
  {1,3,Window_2,1,Message_Age,Message_EnterAge},                 //DF_AGE    
  {0,0,0 ,0,Message_Male,Message_EnterGender},             //DF_GENDER 
  {1,2,Window_2,1,Message_Level,Message_EnterLevel},             //DF_LEVEL
  {1,2,Window_2,1,Message_THRPercent,Message_EnterTHRPercentage},//DF_PERCENT
  {1,3,Window_2,4,Message_Speed,Message_EnterTheSpeed}           //DF_SPEED  
};


u8 Default_Program(void)
{
    u8 by_Status=M_NONE;
    u8 by_DFStatus=DF_PROGRAM;
    u8 by_FitnessMode=1;
    u8 BuDef[10];// �趨�ײ㴢����
    u8 by_DefTarget=1;// �趨�ײ㴢����
    u8 by_SleepModeState = 0;// Sinkyo..EUPs
    u8 ChangeMessage = Console_Language(Data_Get,0);  
    u8 _ChangProgram = 0;
    u8 _xIDComeback=0;
    u8 InclineResetTime = 1;
    u16 by_KeyInput = 0;
    u16 by_ShowNumber;
    u16 _TotalNumber;

    
    LCB_SleepMode_Initial();// Sinkyo..EUPs
    BuDef[0]=DF_PROGRAM;
    bit.key=0;
    bit.key1=0;
    bit.key2=0;
    bit.Stop=0;
    bit.Pause=0;
    bit.NumberKey=0;
    bit.Choose=0;
    bit.GO = 0;
    Led_Set_Reset();
    Heart_Start();
    if(EEPROM_ReadInformation(EE_OutOfOrder) == _ON)
    {//==>��������̨˩��ʱ
        while(by_Status==M_NONE)
        {
            //UsbFunction_USB(0x500);
            //UsbFunction_SoftwareUpdate(1);//==>֧�����°��������ģʽ
            Show_Message(Message_OutOfOrder,0,0,Show_Auto);
            by_KeyInput = Key_Proce();
            by_Status = EngineerMode_SpecialMode(by_KeyInput);
            if(!Check_SafeKey()) by_Status=M_RESET;
        }    
    }
    //================================================================================================================
    //====> C-SAFE�����趨
    //================================================================================================================
    if(CSAFE_GetInformation(_Get_Mode) == Csafe_In_Use)
    {
        if(CSAFE_GetInformation(_Get_TargetUserInfo) == 1)
        {
            Console_Information(Info_AGE,Data_Set,CSAFE_GetInformation(_Get_AGE));//Console_AGE(Data_Set,CSAFE_GetInformation(_Get_AGE));
            Console_Information(Info_WEIGHT,Data_Set,CSAFE_GetInformation(_Get_Weight));//Console_Weight(Data_Set,CSAFE_GetInformation(_Get_Weight));
            if(CSAFE_GetInformation(_Get_Gender) != 0)
            {// CSAFE Gender���� 0:None / 1:Male / 2:Gemale
                Console_Information(Info_GENDER,Data_Set,CSAFE_GetInformation(_Get_Gender));
            }
        }
        if(CSAFE_GetInformation(_Get_TargetTime) == 1)
        {
            Console_Information(Info_TIME,Data_Set,CSAFE_GetInformation(_Get_Time)*60);
        }
        if(CSAFE_GetInformation(_Get_TargetDistance) || 
           CSAFE_GetInformation(_Get_TargetCalories))
        {//==>��������趨Ŀ��distance and caloriesʱ,��Time��Ҫ��Ϊ 0  
            Console_Information(Info_TIME,Data_Set,0);
        }
        if(CSAFE_GetInformation(_Get_TargetLevel) == 1)
        {
            Console_Information(Info_LEVEL,Data_Set,CSAFE_GetInformation(_Get_Level));
        }
        if(CSAFE_GetInformation(_Get_DFprogram) != 0)
        {
            bit.key=1;
            Console_ProgramMode(Data_Set,CSAFE_GetInformation(_Get_DFprogram));
            Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
            Show_Message(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][1],0,0,Show_Auto);
            Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][3],0);//==>��Table����ռ����
            switch(Console_ProgramMode(Data_Get,0))
            {
                case TM_Gerkin:  
                case TM_PEB: 
                case TM_Army: 
                case TM_Navy: 
                case TM_Airforce: 
                case TM_Marine: 
                case TM_WFI:  
                        by_FitnessMode = CSAFE_GetInformation(_Get_DFprogram) - 5; 
                        break;        
                case TM_TargetHR:    
                        if(CSAFE_GetInformation(_Get_DFprogram) == TM_TargetHR)
                        {
                            Heart_Set_Target((220-Console_Information(Info_AGE,Data_Get,0))*Console_Information(Info_PERCENT,Data_Get,0)/100);//Console_AGE(Data_Get,0) Console_THRPercent(Data_Get,0)
                        }
                        break;
            }
            Led_ProfileMap(2);//==>ȡͼ�����
        }
        else
        {// û���趨Program�����Ǳ��Ϊ��
            CSAFE_SetInformation(_Set_Program,Console_ProgramMode(Data_Get,0)); 
        }
        if(CSAFE_GetInformation(_Get_TargetHR) == 1) 
        {// �趨Heart Rate
            Heart_Set_Target(CSAFE_GetInformation(_Get_THR));
        }  
        bit.QuickStart=1;
    }
    else
    {
        Timer_Counter_Clear(7);
        bit.QuickStart=0;
    }
    if(CSAFE_GetInformation(_Get_Online) == 1)
    {//==>�˶����������ж��Ƿ��н�Cable����
        Timer_Counter_Clear(3);
    }
    //  <<========================================================================== 
    if(by_WorkoutDFNewProgram != 0)
    {//==>�����˶�PROGRAM���л������������趨ʱ (Fitness Test / Heart Rate)
        Console_ProgramMode(Data_Set,by_WorkoutDFNewProgram);
        switch(by_WorkoutDFNewProgram)
        {
            case TM_Manual:
            case TM_Rolling: 
            case TM_FatBurn: 
            case TM_5K:  
            case TM_10K:  
            case TM_TargetHR:
                     BuDef[by_DefTarget]=by_DFStatus;
                     by_DefTarget++;
                     by_DFStatus=Default_ProgramDF(by_DFStatus);
                     break;
            case TM_Gerkin:
                     bit.key=1;
                     by_FitnessMode++;
                     Show_Message(Message_Gerkin,0,0,Show_Auto);   
                     break; 
        }
        by_WorkoutDFNewProgram=0;
        Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
        Led_Set_Target(Target_Program,0);
        Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
        Led_ProfileMap(2);//==>ȡͼ�����
    }
    bit.THR_Safe=0;
    Timer_Counter_Clear(4);
    
    Timer_Counter_Clear(5);
    
    // 2014.11.24
    bit.machineIdentification = 0;
    userFlashConfig[0]->LoggedOn = 0;
    Timer_Counter_Clear(_Time_WiFiConnectCheck);
    while(by_Status==M_NONE)
    {
        //UsbFunction_USB(0x500);
        //UsbFunction_SoftwareUpdate(1);//==>֧�����°��������ģʽ
#ifdef _DebugClose
       if(ErrorCodeDisplay_Mode(0) == M_RESET)
       {//==>һ��ģʽ 
           by_Status = M_RESET;  
       }
#endif

      if(Timer_Counter(T_STEP,5,50))
      {
           if(InclineResetTime < 25)
           {
             if(Digtial_ElevationVariationCheck(0) == 1)
             {// ������ʵ��ֵ��Ŀ��ֵ�������,�����֮ǰ���趨��Ŀ��ֵ����Ȼ��������ȡ��
               Digital_Clear_ElevationOld();//==>������ɵ�AD�ݴ�ֵ
               //Console_Incline(Data_Set,0);
               Console_Information(Info_WorkoutIncline,Data_Set,0);            
               Timer_Counter_Clear(5);
               InclineResetTime++;
             }
           }
           else
           {//if incline motor stuck more than 2 minutes,stop it
             Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ
           }
      }  
      // 
      if(Timer_Counter(T_STEP,7,30))
      {
          bit.QuickStart=1;
      }
      // NetPluse  
      if(CSAFE_GetNetPulse() == 1 && 
         CSAFE_GetInformation(_Get_Mode)!= Csafe_In_Use && 
         CSAFE_GetInformation(_Get_Mode)!= Csafe_Paused )
      {//==>��Ϊ NetPluse ģʽʱ3������лش� IDle
          if(Timer_Counter(T_STEP,9,30))
          {
              CSAFE_Answer(Csafe_IDle);
          }
      }
      else
          Timer_Counter_Clear(9) ;
      //
      Heart_Decoder();
      if(bit.Stop == 0)
      {
        //==========================================================
        //==> C-SAFE״̬�ж�code
        //==========================================================
        if(CSAFE_GetInformation(_Get_Mode) == Csafe_IDle) 
        {
            by_Status = M_CSAFE;  
        }
        else if(CSAFE_GetInformation(_Get_Mode) == Csafe_Finished)
        {
            CSAFE_Work_Exit(); //��Ҫ����,��Ȼ����ʱ�������
            by_Status = M_RESET;
        }
        if(CSAFE_GetInformation(_Get_Online) == 1)
        {//==>�˶����������ж��Ƿ��н�Cable��
            if(Timer_Counter(T_STEP,3,20))
            {//==>������2��û�лش� Command ����Ϊ Offline
                CSAFE_SetInformation(_Set_Online,0);
                CSAFE_SetInformation(_Set_Mode,Csafe_Offline);
            }
        }
        //==========================================================
        
        switch(by_DFStatus)
        {//==>����Ӧ�����趨ģʽ
            case DF_TIME:
            case DF_WEIGHT:  
            case DF_HEIGHT:  
            case DF_INCLINE:  
            case DF_AGE:
            case DF_GENDER:
            case DF_LEVEL:
            case DF_PERCENT:
                         if(!bit.key1)
                         {//==>��һ�ν���ʱ���趨��Ϣ
                             bit.key1=1;
                             if(by_DFStatus == DF_TIME)
                                 by_ShowNumber = Console_Information(Info_TIME,Data_Get,0);
                             else if(by_DFStatus == DF_WEIGHT)
                                 by_ShowNumber = Console_Information(Info_WEIGHT,Data_Get,0);
                             else if(by_DFStatus == DF_HEIGHT)
                                 by_ShowNumber = Console_Information(Info_HEIGHT,Data_Get,0);
                             else if(by_DFStatus == DF_INCLINE)
                                 by_ShowNumber = Console_Information(Info_StartIncline,Data_Get,0)*10/2;
                             else if(by_DFStatus == DF_AGE)
                                 by_ShowNumber = Console_Information(Info_AGE,Data_Get,0);
                             else if(by_DFStatus == DF_LEVEL)
                                 by_ShowNumber = Console_Information(Info_LEVEL,Data_Get,0);
                             if(by_DFStatus == DF_GENDER)
                             {
                                 if(Console_Information(Info_GENDER,Data_Get,0) == D_Male) 
                                     Show_Message(Message_Male,0,0,Show_Auto);//==>Male
                                 else 
                                     Show_Message(Message_Female,0,0,Show_Auto);//==>Female
                             }
                             else if(by_DFStatus == DF_PERCENT)
                             {
                                 bit.Choose=0;
                                 Timer_Counter_Clear(0);//no matter change %THR value or not display Your thr is
                                 _Message_Status = Show_Message(Message_THRPercent,0,0,Show_Auto);//==>Percent
                                 if(_Message_Status == _Message_Loop)
                                 {// ���ַ���
                                     if(Led_Get_String_End())
                                     {
                                         Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,Console_Information(by_DFStatus-1,Data_Get,0));//Console_THRPercent(Data_Get,0)
                                     }
                                     else
                                     {
                                         bit.key1=0;
                                         break;
                                     }
                                 }
                                 else if(_Message_Status == _Message_Keep)
                                 {// ���ַ���
                                     Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,Console_Information(by_DFStatus-1,Data_Get,0));//Console_THRPercent(Data_Get,0)
                                 }
                             }
                             else
                             {// ��ʾ���� multi language string too long solve 
                                 _Message_Status = Show_Message(DefaultNumber[by_DFStatus-1].Message,0,0,Show_Auto);
                                 if(_Message_Status == _Message_Loop)
                                 {// ���ַ���
                                   if(Led_Get_String_End())
                                   {
                                     if(DefaultNumber[by_DFStatus-1].DisplayMode == 2)// Display Time mode
                                       Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,by_ShowNumber / 60 * 100);
                                     else
                                       Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,by_ShowNumber);
                                   }
                                  else
                                  {
                                    bit.key1=0;
                                    break;
                                  } 
                                 }
                                 else if(_Message_Status == _Message_Keep)
                                 {// ���ַ���
                                   if(DefaultNumber[by_DFStatus-1].DisplayMode == 2)// Display Time mode
                                     Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,by_ShowNumber / 60 * 100);
                                   else
                                     Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,by_ShowNumber);
                                 }
                             }
                     }
                     else if(by_DFStatus == DF_PERCENT)
                     {
                         if(bit.Choose == 1)
                         {
                             Show_Message(Message_THRIs,1,(u16)((220-Console_Information(Info_AGE,Data_Get,0))*Console_Information(Info_PERCENT,Data_Get,0))/100,Show_Auto);
                         }
                         else if(bit.NumberKey == 0 && Timer_Counter(T_STEP,0,20))
                         {
                             bit.Choose=1;
                         }
                     }
                     break;
//solve fitness test & training workouts program flow 
            case DF_TRAINING:
                     Show_Message(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][1],0,0,Show_Auto);
                     Led_ProfileMap(0);//==>��ȫͼ
                     break;
            case DF_FITNESS:
                     Show_Message(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][1],0,0,Show_Auto);
                     Led_ProfileMap(0);//==>��ȫͼ
                     break;         
            case DF_HRControl:
                     if(!bit.key1)
                     {
                         if(bit.HRT_Based == 1)
                         {// Speed control Enable
                             Show_Message(Messgae_ChooseHRcontrol,0,0,Show_Auto);
                             Led_Set_Target(Target_Speed,2);
                             Led_Set_Target(Target_Incline,2);
                         }
                         else
                         {// Speed control Disable
                             Show_Message(Message_SetInclineMaximumValue,0,0,Show_Auto);
                             if(Led_Get_String_End())
                             {
                                 bit.key1=1;
                                 Led_Set_Target(Target_Incline,1);
                                 Led_Window(Window_1,4,Console_GetHRControlMax());
                             }
                         }
                     }
                     break;
            //------------------------------------------------------- 
            case DF_PROGRAM:
                     if(!bit.key)
                     {
                         if(!LCB_GetSleepModeState())// Sinkyo..EUPs
                         {
//                             LED_MATRIX(_BigMatrix_Action,_Display_Logo);// ��Logo�ַ��������
                             // start v2.2-6
                             Led_TargetLED_Loop();
                             if(CSAFE_GetInformation(_Get_Mode) != Csafe_In_Use)
                             {
                             // 2014.11.24 WiFi Login Check
                             if((EEPROM_ReadInformation(EE_Wireless) == D_Enable)&&
                                (EEPROM_ReadInformation(EE_xID) == D_Enable) )
                             {// Open xID login function ��Home������
                                 // WiFi check offline and reset
                                 WiFi_Information_ConnectCheck();
                                 //
                                     if(bit.machineIdentification == 0)
                                     {
                                         if(WIFICanLoginUser() != 0) 
                                         {
                                             if(strncmp(DEFAULT_UUID,machineConfig->machineIdentification,36 ) != 0) 
                                             {// ������֤��ȡ��������ǿ�Ⱥ����Login
                                                 _xIDComeback=Main_EnterLogin();
                                                 switch(_xIDComeback)
                                                 {
                                                     case M_RESET:
                                                              return M_RESET;
                                                     case M_START:   
                                                              Console_ProgramMode(Data_Set,TM_Manual);
                                                              Led_Set_Target(Target_Program,0);
                                                              Led_Set_Target(Target_Manual,1);
                                                              by_Status = M_START;
                                                              break;
                                                     default:
                                                              if(WIFISignalStrength() != 0)
                                                              {
                                                                  bit.machineIdentification = 1;
                                                              }
                                                              break;
                                                 }
                                             }
                                             Timer_Counter_Clear(_Time_WiFiConnectCheck);
                                         }
                                         Show_Message(Message_xIDworkoutTrackingAvailable,0,0,Show_Auto);
                                         break;
                                     }
                                 }
                                 // WifiѶ��ǿ��
//                                 Led_Set_Target(Target_WiFiSignal,WIFISignalStrength());
                             }
                             Show_Message(Message_BeginYourWorkoutByButton,0,0,Show_Auto);// Press go or select a workout
                             // end v2.2-6
                         }
                     }
                     else
                     {
                       if(Console_ProgramMode(Data_Get,0) == TM_5K)
                         Show_Message(_ProgramInfor[Console_ProgramMode(Data_Get,0)+11][1],0,0,Show_Auto);
                       else if(Console_ProgramMode(Data_Get,0) == TM_Gerkin)
                         Show_Message(_ProgramInfor[Console_ProgramMode(Data_Get,0)+7][1],0,0,Show_Auto);
                       else
                         Show_Message(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][1],0,0,Show_Auto);
                       Led_ProfileMap(0);//==>��ȫͼ
                     }
                     break;
            case DF_START:
                     Show_Message(Message_EnterStartToBegin,0,0,Show_Auto);
                     break;
            
            case DF_SPEED:
                         if(Console_ProgramMode(Data_Get,0) == TM_Gerkin)
                         {
                             Show_Message(Message_StartingSpeedGrade,0,0,Show_Auto);
                             if(Led_Get_String_End())
                             {//==>������������κ�ͽ�����һ��
                                 BuDef[by_DefTarget] = by_DFStatus;
                                 by_DefTarget++;
                                 by_DFStatus = Default_ProgramDF(by_DFStatus);
                             }
                         }
                         else if(Console_ProgramMode(Data_Get,0) == TM_WFI)
                         {
                             if(by_WFI_Show == 0)
                             {
                                 //Show_SetSummaryNumber(4);//==>��ʾ��Ϣֵģʽ xx.x
                                 Show_Message(Message_BMI,_MsgAddNum_Fixed_6Int1Dec,Console_Get_BMI()/100,Show_RightToLeft);
                             }    
                             else
                                 Show_Message(Message_StartingSpeedGrade,0,0,Show_Auto);
                             if(Led_Get_String_End())
                             {//==>������������κ�ͽ�����һ��
                                 by_WFI_Show++;
                                 if(by_WFI_Show > 1)
                                 {
                                     BuDef[by_DefTarget] = by_DFStatus;
                                     by_DefTarget++;
                                     by_DFStatus = Default_ProgramDF(by_DFStatus);
                                 }    
                             }
                         }
                         else
                         {
                                 if(!bit.key1)
                                 {//==>��һ�ν���ʱ���趨��Ϣ
                                     _Message_Status = Show_Message(Message_Speed,0,0,Show_Auto);
                                     if(_Message_Status == _Message_Loop)
                                     {// ���ַ���
                                         if(Led_Get_String_End())
                                         {
                                             bit.key1=1;
                                             Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,Console_Information(Info_StartSpeed,Data_Get,0));//Console_FitTestSpeed(Data_Get)
                                             Timer_Counter_Clear(0);
                                         }
                                     }
                                     else if(_Message_Status == _Message_Keep)
                                     {// ���ַ���
                                         bit.key1=1;
                                         Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,Console_Information(Info_StartSpeed,Data_Get,0));
                                         //Timer_Counter_Clear(0);
                                     }
                                 }    
                             }   
                     break;
            case DF_THR:
                     if(Console_ProgramMode(Data_Get,0) == TM_WFI)
                         Heart_Set_Target((u8)((float)(208-(0.7*Console_Information(Info_AGE,Data_Get,0)))*0.85));
                     else if(Console_ProgramMode(Data_Get,0) == TM_Gerkin)
                         Heart_Set_Target((u8)((float)(220-Console_Information(Info_AGE,Data_Get,0))*0.85));
                     Show_Message(Message_THRIs,1,Heart_Get_Target(),Show_Auto);
                     if(Led_Get_String_End())
                     {//==>������������κ�ͽ�����һ��
                         BuDef[by_DefTarget]=by_DFStatus;
                         by_DefTarget++;
                         by_DFStatus=Default_ProgramDF(by_DFStatus);
                     }
                     break;
        }
        
        by_KeyInput = Key_Proce();
        // If comeback from xID mode, Key need change to it.
        if(_xIDComeback >= K_MANUAL &&\
           _xIDComeback <= K_FitnessTest )
        {
            by_KeyInput=_xIDComeback;
        }
        _xIDComeback=0;
        //
        if(by_KeyInput == K_NONE)
        {//==>�����趨����ģʽ��ʱû������30���RESET
            if(Timer_Counter(T_STEP,8,300) == 1)  
            {
                by_Status=M_RESET;
            }
        }
        else 
        {//==>���ư���
            Timer_Counter_Clear(8);
        }
        
#ifdef _EUPs        
            by_SleepModeState = LCB_SleepMode(1,by_KeyInput,by_Status,Console_SleepTime(Data_Get,0)*60 );//60
            //by_SleepModeState = LCB_SleepMode(1,by_KeyInput,by_Status,50); 
            // Sleep Time : �뵽Engineering Mode�����趨. 
            // Ĭ��ֵ5���ӣ���Χ 1����~4Сʱ,Sleep Time ��λΪ���ӵ���.
            // 2014.11.24
            bit.ErpMode = 0;
            //
            if(by_SleepModeState == 2)                                                                                              
            {
                by_Status = M_RESET;
                by_KeyInput = K_NONE;
            }
            else if(by_SleepModeState == 1)
            {
                // 2014.11.24
                bit.ErpMode = 1;
                //
                Timer_Close(8);// �رհ�����30��ļ���
//                LED_MATRIX(_BigMatrix_Clear);// ����
                by_KeyInput = K_NONE;
            }
            //
#endif    
        if(by_Status == M_NONE)
        {
            by_Status = EngineerMode_SpecialMode(by_KeyInput);
            if(by_Status == M_RESET)//When Quit Engineer mode,Reset
              return by_Status;
        }
        bit.ShowChang = 0;
        switch(by_KeyInput)
        {

            case K_STOP | K_MASK:
                     by_Status=M_RESET;
                     break;
            case K_GO:
                     CSAFE_Set_AnyKey();
                     if(bit.QuickStart && !Digital_Get_RPMStatus())
                     {//==>���ֹͣת��    
                         Led_Set_Target(Target_Program,0);
                         if(Console_ProgramMode(Data_Get,0) == TM_QuickStart)
                         {//==>ֱ�Ӱ�Quick start
                             Console_ProgramMode(Data_Set,TM_Manual);
                             Led_Set_Target(Target_Program,0);
                             Led_Set_Target(Target_Manual,1);
                         }
                         by_Status = M_START;
                         // start v2.2-5
                         Console_AutoSoftwareUpdateStatus(1);
                         // end v2.2-5
                     }
                     else 
                     {
                         Led_Set_Target(Display_OFF,0);
                         by_Status = Main_MotorStatusCheckMessage(0);
                         if(by_Status == M_START)
                         {
                             if(Console_ProgramMode(Data_Get,0) == TM_QuickStart)
                             {
                                 Console_ProgramMode(Data_Set,TM_Manual);
                                 Led_Set_Target(Target_Manual,1);
                             }
                         }
                         if(by_DFStatus == DF_PROGRAM)
                         {
//                             LED_MATRIX(_BigMatrix_Clear);// ����
                             Led_Set_Target(Target_Program,0);
                         }
                         else
                         {
                             Led_ProfileMap(0);//==>��ȫͼ
                         }
                         if(Console_ProgramMode(Data_Get,0) != TM_QuickStart)
                         {
                           Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
                           Show_Message(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][1],0,0,Show_Auto);
                         }
                     }
                     if(by_Status == M_START && CSAFE_GetInformation(_Get_Mode)== Csafe_Paused)
                     {
                         CSAFE_Answer(Csafe_In_Use);
                     }
                     
                     break;
            case K_TimeU:
            case K_InclineU:  
            case K_SpeedU:
            case K_TimeU|K_MASK:
            case K_InclineU|K_MASK:   
            case K_SpeedU|K_MASK:
                     bit.ShowChang = 1;
            case K_TimeD:
            case K_InclineD:  
            case K_SpeedD:  
            case K_TimeD|K_MASK:
            case K_InclineD|K_MASK:  
            case K_SpeedD|K_MASK: 
                     CSAFE_Set_AnyKey();
                     //bit.Choose = 0; 
                     switch(by_DFStatus)
                     {//==>�������趨
                         case DF_PERCENT:  
                                 Timer_Counter_Clear(0);
                                 if(bit.Choose == 1) bit.key1=0;
                                 bit.Choose = 0;
                         case DF_TIME:
                         case DF_HEIGHT:
                         case DF_WEIGHT:  
                         case DF_AGE:  
                         case DF_INCLINE:
                         case DF_LEVEL:  
                         case DF_SPEED:
                                 if(by_DFStatus == DF_SPEED && \
                                   (Console_ProgramMode(Data_Get,0) == TM_Gerkin || \
                                    Console_ProgramMode(Data_Get,0) == TM_WFI) )
                                 {
                                     break;
                                 }
                                 if(by_DFStatus == DF_PERCENT && bit.key2 == 1)
                                 {// ����ʾ�е���������������ʾ�����ַ���
                                     bit.key2=0;
                                     bit.key1=0;
                                 }
                                 if(Led_Get_String_End() == 0)
                                 {//Garry Meng Multi Language if scroll string clear it
                                   Show_Message(Message_Blank,0,0,Show_Blank);//==>�������
                                 }
                                 bit.key=1;
                                 bit.NumberKey=0;//solve number key & up/down key mix input issue
                                 if(!bit.key1)
                                 {// ��ʾ�����ַ�������
                                     bit.key1=1;
                                     if(by_DFStatus == DF_PERCENT || by_DFStatus == DF_SPEED)
                                     {
                                         _Message_Status = Show_Message(DefaultNumber[by_DFStatus-1].Message,0,0,Show_Auto);
                                         if(_Message_Status == _Message_Loop)
                                         {// ���ַ���
                                             bit.key1=0;
                                             Show_Message(Message_Blank,0,0,Show_Blank);
                                         }
                                         by_ShowNumber = Console_Information(by_DFStatus-1,Data_Get,0);
                                     }
                                     else if(by_DFStatus == DF_LEVEL && Console_Language(Data_Get,0) == L_German)
                                     {
                                         bit.key1=0;
                                         Show_Message(Message_Blank,0,0,Show_Blank);
                                         by_ShowNumber = Console_Information(Info_LEVEL,Data_Get,0);
                                     }
                                     else
                                         Show_Message(DefaultNumber[by_DFStatus-1].Message,0,0,Show_Auto);
                                 }
                                 else
                                 {// ������������ʱ
                                     Console_Information(by_DFStatus-1,bit.ShowChang,0);
                                     if(by_DFStatus == DF_INCLINE)
                                         by_ShowNumber = Console_Information(by_DFStatus-1,Data_Get,0) * 10 / 2;
                                     else  
                                         by_ShowNumber = Console_Information(by_DFStatus-1,Data_Get,0);
                                     if(by_DFStatus == DF_PERCENT)
                                     {
                                         Timer_Counter_Clear(0);
                                     }
                                 }
                                 if(DefaultNumber[by_DFStatus-1].DisplayMode == 2)// Display Time mode
                                     Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,by_ShowNumber / 60 * 100);
                                 else
                                     Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,by_ShowNumber);
                                 if(by_DFStatus == DF_LEVEL)
                                 {
                                     Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
                                     Led_ProfileMap(2);//==>ȡͼ�����
                                 }
                                 break;
                         case DF_PROGRAM:
                                 if(Console_ProgramMode(Data_Get,0) != TM_QuickStart)
                                 {
                                     bit.key=1;
                                     if(bit.ShowChang == 1)
                                         Console_ProgramMode(Data_Add,0);
                                     else
                                         Console_ProgramMode(Data_Dec,0);
                                     if(Console_ProgramMode(Data_Get,0) == TM_10K)
                                     {
                                       if(bit.ShowChang == 1)
                                         Console_ProgramMode(Data_Set,TM_TargetHR);
                                       else
                                         Console_ProgramMode(Data_Set,TM_5K);
                                     }
                                     Led_Set_Target(Display_OFF,0);
                                     Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
                                     Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][3],0);//==>��Table����ռ����
                                 }   
                                 break;
                         case DF_TRAINING:
                                 if(Console_ProgramMode(Data_Get,0) != TM_5K)
                                   Console_ProgramMode(Data_Set,TM_5K);
                                 else
                                   Console_ProgramMode(Data_Set,TM_10K);
                                 Led_Set_Target(Display_OFF,0);
                                 Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
                                 Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][3],0);//==>��Table����ռ����
                                 break;
                         case DF_FITNESS:
                                 if(bit.ShowChang == 1)
                                 {
                                   by_FitnessMode++;
                                   if(by_FitnessMode > 7) by_FitnessMode = 1;
                                 }
                                 else
                                 {
                                   if(by_FitnessMode > 1)
                                     by_FitnessMode--;
                                   else
                                     by_FitnessMode = 7;
                                 }
                                 Console_ProgramMode(Data_Set,TM_Gerkin+by_FitnessMode-1);
                                 Led_Set_Target(Display_OFF,0);
                                 Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
                                 Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][3],0);//==>��Table����ռ����
                                 break;        
                         case DF_HRControl:
                                 if(by_KeyInput == K_TimeU || by_KeyInput == K_TimeD) break;
                                 bit.key=1;
                                 
                                 if(!bit.key1)
                                 {
                                     bit.key1=1;
                                     Show_Message(Message_Blank,0,0,Show_Blank);
                                     if(bit.HRT_Based == 1)
                                     {// Speed control Enable
                                         Led_Set_Target(Target_Speed,0);
                                         Led_Set_Target(Target_Incline,0);
                                         if(by_KeyInput == K_InclineU || by_KeyInput == K_InclineD)
                                         {
                                             Console_SetHRControlModel(0);
                                             Led_Set_Target(Target_Incline,1);
                                             Led_Window(Window_1,4,Console_GetHRControlMax()); 
                                         }
                                         else if(by_KeyInput == K_SpeedU || by_KeyInput == K_SpeedD)
                                         {
                                             Console_SetHRControlModel(1);
                                             Led_Set_Target(Target_Speed,1);
                                             Led_Window(Window_2,4,Console_GetHRControlMax());
                                         }
                                     }
                                     else
                                     {// Speed control Disable
                                         Led_Set_Target(Target_Incline,1);
                                         Led_Window(Window_1,4,Console_GetHRControlMax());
                                     }
                                 }
                                 else
                                 {
                                     if(Console_GetHRControlMode() == 0)
                                     {// Incline ģʽ�°��� Incline key
                                         if(by_KeyInput == K_SpeedU || by_KeyInput == (K_SpeedU|K_MASK) ||
                                            by_KeyInput == K_SpeedD || by_KeyInput == (K_SpeedD|K_MASK) )
                                         {
                                             break;  
                                         }
                                     }
                                     else
                                     {// Speed ģʽ�°��� Speed key
                                         if(by_KeyInput == K_InclineU || by_KeyInput == (K_InclineU|K_MASK) ||
                                            by_KeyInput == K_InclineD || by_KeyInput == (K_InclineD|K_MASK) )
                                         {
                                             break;  
                                         }
                                     }
                                     if(bit.ShowChang == 1)
                                         Console_HRControlMax(D_ADD);
                                     else
                                         Console_HRControlMax(D_DEC);
                                     if(Console_GetHRControlMode() == 0)
                                     {// Incline
                                         Led_Window(Window_1,4,Console_GetHRControlMax());
                                     }
                                     else
                                     {// Speed
                                         Led_Window(Window_2,4,Console_GetHRControlMax());
                                     }
                                 }
                                 break;
                         //-------------------------------------------------------
                         case DF_GENDER:
                                 bit.key=1;
                                 if(!bit.key1)
                                     bit.key1=1;
                                 else
                                 {
                                     if(Console_Information(Info_GENDER,Data_Get,0) == D_Male) //Console_Gender(Data_Get,0)
                                         Console_Information(Info_GENDER,Data_Set,D_Female);//Console_Gender(Data_Set,D_Female);
                                     else 
                                         Console_Information(Info_GENDER,Data_Set,D_Male);//Console_Gender(Data_Set,D_Male);
                                 }
                                 if(Console_Information(Info_GENDER,Data_Get,0) == D_Male) 
                                     Show_Message(Message_Male,0,0,Show_Auto);//==>Male
                                 else 
                                     Show_Message(Message_Female,0,0,Show_Auto);//==>Female
                                 break;
                     }
                     // ������ڴ�
                     Keyboard_NumberCount(0,by_KeyInput);
                     bit.NumberKey = 0;
                     //
                     break;

            case K_MANUAL:
                     if(Console_ProgramMode(Data_Get,0) != TM_Manual)
                         _ChangProgram = TM_Manual;
                     else
                         _ChangProgram = 99;
                     break;
                     
            case K_FatBurn:
                     if(Console_ProgramMode(Data_Get,0) != TM_FatBurn)
                     {
                       if(Console_Information(Info_TIME,Data_Get,0) == 0)
                         Console_Information(Info_TIME,Data_Set,4*60);
                         _ChangProgram = TM_FatBurn;
                     }
                     else
                         _ChangProgram = 99;
                     break;
            case K_Rolling:  
                     if(Console_ProgramMode(Data_Get,0) != TM_Rolling)
                     {
                       if(Console_Information(Info_TIME,Data_Get,0) == 0)
                         Console_Information(Info_TIME,Data_Set,4*60);
                         _ChangProgram = TM_Rolling;
                     }
                     else
                         _ChangProgram = 99;
                     break;
                     
            case K_TargetHR:
                     if(Console_ProgramMode(Data_Get,0) != TM_TargetHR)
                     {
                       if(Console_Information(Info_TIME,Data_Get,0) == 0)
                         Console_Information(Info_TIME,Data_Set,4*60);
                         bit.HRT_Based = EEPROM_ReadInformation(EE_HRTSpeedBased);
                         _ChangProgram = TM_TargetHR;
                     }    
                     else
                         _ChangProgram = 99;
                     break;
            case K_Training:
                     CSAFE_Set_AnyKey();
                     bit.key = 1;
                     Led_Set_Target(Display_OFF,0);
                     Led_Set_Target(Target_Training,1);
                     if(Console_ProgramMode(Data_Get,0) != TM_5K)
                         Console_ProgramMode(Data_Set,TM_5K);
                     by_DFStatus = DF_PROGRAM;
                     by_DefTarget = 0;
                     BuDef[0] = by_DFStatus;
                     Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][3],0);//==>��Table����ռ����  
                     break;
            case K_FitnessTest:
                     CSAFE_Set_AnyKey();
                     bit.key = 1;
                     by_WFI_Show=0;
                     Led_Set_Target(Display_OFF,0);
                     Led_Set_Target(Target_FitnessTest,1);
                     Console_ProgramMode(Data_Set,TM_Gerkin);
                     by_DFStatus = DF_PROGRAM;
                     by_DefTarget = 0;
                     BuDef[0] = by_DFStatus;
                     Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][3],0);//==>��Table����ռ����
                     break;
            case K_0:
            case K_1:
            case K_2:
            case K_3:
            case K_4:
            case K_5:
            case K_6:
            case K_7:
            case K_8:
            case K_9:
                     CSAFE_Set_AnyKey();
                     if(by_DFStatus != DF_PROGRAM && by_DFStatus != DF_START \
                       && by_DFStatus != DF_TRAINING && by_DFStatus != DF_FITNESS)
                     {
                         if(by_DFStatus == DF_THR || \
                            by_DFStatus == DF_GENDER || \
                            by_DFStatus == DF_HRControl || \
                           (by_DFStatus == DF_SPEED && Console_ProgramMode(Data_Get,0) == TM_Gerkin)|| \
                           (by_DFStatus == DF_SPEED && Console_ProgramMode(Data_Get,0) == TM_WFI)) 
                         {
                         }
                         else
                         {
                             if(by_DFStatus == DF_SPEED && Console_ProgramMode(Data_Get,0) == TM_Gerkin) 
                             {
                                 break;
                             }
                             if(by_DFStatus == DF_PERCENT && bit.Choose == 1)
                             {// ����ʾ�е���������������ʾ�����ַ���
                                 bit.Choose = 0;
                                 //Timer_Counter_Clear(0);
                                 _Message_Status = Show_Message(DefaultNumber[by_DFStatus-1].Message,0,0,Show_Auto);
                                 if(_Message_Status == _Message_Loop)
                                 {// ���ַ���
                                     Show_Message(Message_Blank,0,0,Show_Blank);
                                 }
                             }
                             // ���ּ������ۼ�ֵ
                             _TotalNumber = Keyboard_NumberCount(DefaultNumber[by_DFStatus-1].NumericKey.Digits,by_KeyInput);
                             // 
                             if(bit.NumberKey == 0)
                             {
                               if(Led_Get_String_End() == 0)
                               {//Garry Meng Multi Language if scroll string clear it
                                 Show_Message(Message_Blank,0,0,Show_Blank);//==>�������
                               }
                               else
                                 Show_Message(DefaultNumber[by_DFStatus-1].Message,0,0,Show_Auto);
                             }
                             if(DefaultNumber[by_DFStatus-1].DisplayMode == 2)// Display Time mode
                                 Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,_TotalNumber * 100);
                             else
                                 Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,_TotalNumber);    
                     
                             bit.key = 1;
                             bit.key1 = 1;
                             bit.NumberKey = 1;
                             Timer_Counter_Clear(_Time_Back);
                         }   
                     }
                     else
                         // ������ڴ�
                         Keyboard_NumberCount(0,by_KeyInput);
                         //
                     break;
           case K_ENTER:
                     bit.Check = 0;
                     if(bit.NumberKey == 1)
                     { 
                         bit.NumberKey=0;
                       
                         if(by_DFStatus == DF_TIME)
                         {
                             if( _TotalNumber >= 4 && _TotalNumber <= (Console_MaxTime(Data_Get,0)/60))
                                 Console_Information(Info_TIME,Data_Set,_TotalNumber*60);
                             else
                                 bit.Check = 1;
                         }
                         else
                         {
                             if(Console_Information(by_DFStatus-1,Data_Set,_TotalNumber) == 0)
                                 bit.Check = 1;
                             
                             if(by_DFStatus == DF_PERCENT)
                             {// ������������ʾĿ���ַ���
                                 bit.key2=0;
                                 Timer_Counter_Clear(0);
                                 Keyboard_NumberCount(0,by_KeyInput);
                                 break;
                             }
                         }
                         if(by_DFStatus == DF_LEVEL)
                         {
                             Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
                             Led_ProfileMap(0);//==>��ȫͼ
                         }
                         else if(by_DFStatus == DF_PERCENT)
                         {
                             bit.Check = 1;
                             bit.Choose = 0;
                             Timer_Counter_Clear(0);
                         }
                     }
                     if(bit.Check == 0 && by_DFStatus != DF_START)
                     {
                         _TotalNumber = 0;
                         
                         CSAFE_Set_AnyKey();
                         if(Console_ProgramMode(Data_Get,0) == TM_QuickStart)
                         {
                             Console_ProgramMode(Data_Set,TM_Manual);
                             Led_Set_Target(Target_Program,0);
                             Led_Set_Target(Target_Manual,1);
                         }
                         Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
                         Led_ProfileMap(0);//==>��ȫͼ
                         
                         BuDef[by_DefTarget] = by_DFStatus;
                         by_DefTarget++;
                         by_DFStatus = Default_ProgramDF(by_DFStatus);
                         bit.key2=0;
                         bit.NumberKey=0;
                         Timer_Close(0);//==>�رյ�0��ʱ���ۼ�
                     }
                     if(by_DFStatus == DF_HEIGHT && \
                       Console_ProgramMode(Data_Get,0) == TM_WFI)
                     {
                       Console_BMI();
                     }
                     // ������ڴ�
                     Keyboard_NumberCount(0,by_KeyInput);
                     //
                     break;

           case K_BACK:
                     CSAFE_Set_AnyKey();
                     if(!bit.NumberKey || by_DFStatus == DF_HRControl)
                     {
                         if(by_DefTarget > 0) 
                         {//==>�˻���һ�趨��Ŀ
                             by_DefTarget--;
                             by_DFStatus = BuDef[by_DefTarget];
                             bit.key = 0;
                             bit.key1 = 0;
                             bit.key2 = 0;
                             bit.Stop = 0;
                             bit.Pause = 0;
                             bit.Choose = 0;
                             Timer_Close(0);//==>�رյ�0��ʱ���ۼ�
                             Show_Message(Message_Blank,0,0,Show_Blank);
                             Led_Set_Target(Target_Speed,0);
                             Led_Set_Target(Target_Incline,0);
                             Led_Set_Reset();
                             if(by_DFStatus == DF_PROGRAM)
                             {
                                 Led_Initial();
                                 Console_ProgramMode(Data_Set,TM_QuickStart);
//                                 LED_MATRIX(_BigMatrix_Clear);// ����
                                 Led_Set_Target(Target_Program,0);
                             }
                         }    
                     }
                     else 
                     {
                         bit.NumberKey = 0;
                         bit.Check = 1;
                     }
                     if(by_DFStatus == DF_PERCENT)
                     {// ������������ʾĿ���ַ���
                       bit.key2=0;
                       Timer_Counter_Clear(0);
                     }
                     // ������ڴ�
                     Keyboard_NumberCount(0,by_KeyInput);
                     //
                     break;
                     
            case K_PAUSE:
                     CSAFE_Set_AnyKey();
                     break;  
            case K_STOP:
                     Led_Set_Target(Target_Program,0);
                     CSAFE_Set_AnyKey();
                     by_DFStatus = DF_PROGRAM;
                     by_FitnessMode = 1;
                     by_DefTarget = 1;
                     BuDef[0] = DF_PROGRAM;
//                     LED_MATRIX(_BigMatrix_Clear);// ����
                     bit.key = 0;
                     bit.key1 = 0;
                     bit.key2 = 0;
                     bit.Pause = 0;
                     bit.NumberKey = 0;
                     bit.Choose = 0;
                     Led_Set_Reset();
                     Timer_Counter_Clear(4);
                     bit.THR_Safe = 0;
                     Console_ProgramMode(Data_Set,TM_QuickStart);
                     Timer_Close(8);
                     break;
                     
            case K_NONE:
                     
                     break;
        }

        if(bit.NumberKey == 0)
        { 
            Timer_Close(_Time_Back); 
        }
        else if(Timer_Counter(T_STEP,_Time_Back,50))
        {
            bit.Check = 1;
            bit.NumberKey = 0;
            Keyboard_NumberCount(0,K_BACK);// ��Ӌ��ȴ�
            if(by_DFStatus == DF_PERCENT)
            {
                bit.Choose = 0;
                Timer_Counter_Clear(0);
            }
        }
        // Display back default number
        if(bit.Check == 1)
        {
            bit.Check = 0;
            
            _TotalNumber = Console_Information(by_DFStatus-1,Data_Get,0); 
            if(by_DFStatus == DF_TIME)
                _TotalNumber /= 60;
            else if(by_DFStatus == DF_INCLINE)
                _TotalNumber = _TotalNumber * 10 / 2;
            if(DefaultNumber[by_DFStatus-1].DisplayMode == 2)// Display Time mode
                Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,_TotalNumber * 100);
            else
                Led_Window(DefaultNumber[by_DFStatus-1].DisplayLine,DefaultNumber[by_DFStatus-1].DisplayMode,_TotalNumber);    
            _TotalNumber = 0;
        }
        // Press Program key to change program use
        if(_ChangProgram != 0)
        {
           CSAFE_Set_AnyKey();
           if(_ChangProgram == 99)
           {// Enter next item
               BuDef[by_DefTarget] = by_DFStatus;
               if(by_DFStatus != DF_START)
               {
                   by_DefTarget++;
                   by_DFStatus = Default_ProgramDF(by_DFStatus);
               }
               Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
           }
           else
           {// Enter Choose Program
               bit.key = 1;
               by_DFStatus = DF_PROGRAM;
               by_DefTarget = 0;
               BuDef[0] = by_DFStatus;
               Console_ProgramMode(Data_Set,_ChangProgram);
               Led_Set_Target(Display_OFF,0);
               Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
               Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][3],0);//==>��Table����ռ����
           }
           Led_ProfileMap(0);//==>��ȫͼ
            _ChangProgram = 0;
        }
        //        
                
        if(Timer_Counter(T_STEP,4,50))
        {//==>5�����������������⹦��
            bit.THR_Safe=1;
        }
        if(bit.THR_Safe && by_DFStatus == DF_PROGRAM && Heart_GetBlink()\
           && Console_ProgramMode(Data_Get,0) == TM_QuickStart)//set program not display Detecting HR
        {//==>��������ֵʱ��ת������ʾHRģʽ
            bit.Stop = 1;
            bit.THR_Safe = 0;
            bit.HR_Blink = 0;
            bit.Enter = 1;
            Timer_Counter_Clear(2);  
            Show_Message(Message_Blank,0,0,Show_Blank);//==>�������
            _Message_Status = Show_Message(Message_DetectingHR,0,0,Show_Auto);
            Led_Set_Target(Target_Program,0);
        }
      }  
      else 
      {//==>HR������ʾģʽ��ʱ
          LCB_SleepMode_Initial();// Sinkyo..EUPs
          HeartRate_HomeMode();

          if(Heart_GetBlink()) Timer_Counter_Clear(2);
//          if(by_DFStatus == DF_PROGRAM && bit.key == 0)
//          {
//              LED_MATRIX(_BigMatrix_Action);// ��Logo�ַ��������
//          }
          // ������ʾHR�����°����������HRѶ��5����뻹ԭHome����
          if(Key_Proce()!= K_NONE || Timer_Counter(T_STEP,2,20))
          {
              if(bit.key == 1)
              {
//                  LED_MATRIX(_BigMatrix_Clear);// ���������ʼ������
                  bit.key = 0;
              }
              bit.key1 = 0;
              bit.key2 = 0;
              bit.Stop = 0;
              bit.Pause = 0;
              bit.NumberKey = 0;
              bit.THR_Safe = 0;
              by_DFStatus = DF_PROGRAM;
              by_FitnessMode = 1;
              by_DefTarget = 1;
              BuDef[0] = DF_PROGRAM;
              Led_Set_Reset();
              Timer_Counter_Clear(4);
              Led_Set_Target(Target_HRInfor,0);
          }
          //  
      }
      // ��������ۼ�
      Console_HJS(Data_Total,0);
      //
      WIFISignalStrength();  
      // start v2.1-13
      main_RSCUcheck();
      // end v2.1-13
      if(!Check_SafeKey()) by_Status=M_RESET;
      UsbFunction_Protection();
    }
    bit.Stop=0;
    if(Console_ProgramMode(Data_Get,0) == TM_5K ||\
       Console_ProgramMode(Data_Get,0) == TM_10K ||\
       Console_ProgramMode(Data_Get,0) >= TM_Gerkin )
    {//==>��Ϊ�����趨Ŀ��Distance or Caloriesʱ,��ȡ��CSAFEԭԤ��Goal����ж�
        Console_CsafGoalDisable();
    }
    return by_Status;
}
/*******************************************************************************
* Function Name  : Default_ProgramDF
* Description    : ������ȷ�����¨��趨������Ӧ�ж�
* Input          : by_DF ,Ŀǰ�趨����ģʽ
* Output         : by_Return ,�¨��趨����ģʽ
* Return         : by_Return
*******************************************************************************/
const unsigned char _NextDefault[9][9]=
{{DF_PROGRAM,DF_TIME    ,DF_WEIGHT ,DF_INCLINE  ,DF_SPEED    ,DF_START ,        0,       0,0},// Manual
 {DF_PROGRAM,DF_TRAINING,DF_LEVEL  ,DF_WEIGHT   ,DF_START    ,        0,        0,       0,0},// 5K Run 
 {DF_PROGRAM,DF_TRAINING,DF_LEVEL  ,DF_WEIGHT   ,DF_START    ,        0,        0,       0,0},// 10K Run 
 {DF_PROGRAM,DF_AGE     ,DF_PERCENT,DF_HRControl,DF_TIME     ,DF_WEIGHT,DF_START ,       0,0},// Heart Rate  
 {DF_PROGRAM,DF_LEVEL   ,DF_TIME   ,DF_WEIGHT   ,DF_START    ,        0,        0,       0,0},// Fat Burn
 {DF_PROGRAM,DF_LEVEL   ,DF_TIME   ,DF_WEIGHT   ,DF_START    ,        0,        0,       0,0},// Rolling Hills 
 {DF_PROGRAM,DF_FITNESS ,DF_AGE    ,DF_THR      ,DF_GENDER   ,DF_WEIGHT,DF_SPEED ,DF_START,0},// Gerkin 
 {DF_PROGRAM,DF_FITNESS ,DF_AGE    ,DF_GENDER   ,DF_WEIGHT   ,DF_SPEED ,DF_START ,       0,0},// PEB/NAVY/AIRFORCE/ARMY/MARINE
 {DF_PROGRAM,DF_FITNESS ,DF_AGE    ,DF_THR      ,DF_GENDER   ,DF_WEIGHT,DF_HEIGHT,DF_SPEED,DF_START} // WFI 

};
u8 Default_ProgramDF(u8 by_DF)
{
  char i,NextMode;
  u8 by_Return=by_DF;
  
  bit.key2=0;
  if(by_DF == DF_PERCENT)
  {// �趨Ŀ������ֵ
     Heart_Set_Target((220-Console_Information(Info_AGE,Data_Get,0))*Console_Information(Info_PERCENT,Data_Get,0)/100);
  }
  else if(by_DF == DF_HRControl)
  {
     Led_Set_Target(Target_Speed,0);
     Led_Set_Target(Target_Incline,0);
  }
  if(Console_ProgramMode(Data_Get,0) >= TM_PEB && \
     Console_ProgramMode(Data_Get,0) <= TM_Marine )
      NextMode = 7;
  else if(Console_ProgramMode(Data_Get,0) >= TM_WFI)
      NextMode = 8;
  else
  {
          NextMode = Console_ProgramMode(Data_Get,0)-1;
  }
  
  for(i=0;i<8;i++)
  {
      if(by_DF == _NextDefault[NextMode][i])
      {
          by_Return = _NextDefault[NextMode][i+1];
          // If Logged, will bypass weight/height/age/gender item.
          if(userFlashConfig[0]->LoggedOn == 1 &&\
            (by_Return == DF_AGE ||\
             by_Return == DF_GENDER ||\
             by_Return == DF_WEIGHT ||\
             by_Return == DF_HEIGHT ) )
          {// Logged nad bypass
              by_DF = by_Return;
          }
          //
          else
              i = 99;
      }
  }
  if(by_Return != DF_START)
  {
      Led_Set_Reset();
      bit.key=0;
      bit.key1=0;
      bit.NumberKey=0;
  }
  return by_Return;
}
/*******************************************************************************
* Function Name  : Start_CoolDown
* Description    : Cool Downģʽ�����ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Start_CoolDown(void)
{   
  bit.WarmUp = 0;
  if(Console_ProgramMode(Data_Get,0) == TM_WFI)
  {
      by_HoldBuffer=180;
  }
  else
  {//==>Cool down Time 2 ~ 5 min
      by_HoldBuffer = (Console_Parameter(Info_AccumulatedTime) * 2) / 10 ;
      if(by_HoldBuffer > 300) by_HoldBuffer = 300;
      else if(by_HoldBuffer < 120) by_HoldBuffer = 120;  
  }
  //Console_Time(Data_Set,by_HoldBuffer);
  Console_Information(Info_TIME,Data_Set,by_HoldBuffer);
  Console_UserTime(Data_Set,by_HoldBuffer);
  Console_Time_Cul();
  Console_TimeOver(Data_Set,0);
  if(Console_ProgramMode(Data_Get,0) >= TM_Gerkin)
  {
      if(Console_ProgramMode(Data_Get,0) == TM_Navy)//==>980810
      {
          Console_Set_StartAccumulated(0);//==>��ʱ��ֹͣ�ۼ�
      }
      Console_Set_FitTestTimeAcc(0);
  } 
  
  //Console_Incline(Data_Set,0); 
  Console_Information(Info_WorkoutIncline,Data_Set,0);
  //==> Now speed x 60%
  by_HoldBuffer=(u32)(Console_Information(Info_WorkoutSpeed,Data_Get,0) * 60) / 100;//Console_Speed(Data_Get,0)
  if(Console_SpeedUnit(Data_Get,0) == C_KM)
  {
      if(by_HoldBuffer < 80) by_HoldBuffer = 80;
      else if(by_HoldBuffer > 640) by_HoldBuffer = 640;
      if(Console_ProgramMode(Data_Get,0) == TM_WFI)
      {
          by_HoldBuffer = 480;
      }
  }
  else
  {
      if(by_HoldBuffer < 50) by_HoldBuffer = 50;
      else if(by_HoldBuffer > 400) by_HoldBuffer = 400;
      if(Console_ProgramMode(Data_Get,0) == TM_WFI)
      {
          by_HoldBuffer = 300;
      }
  }
  bit.KeyColse=0;//==>�����ּ�����
  //Console_Speed(Data_Set,by_HoldBuffer);
  Console_Information(Info_WorkoutSpeed,Data_Set,by_HoldBuffer);
  by_HoldBuffer=0;
  bit.ShowCoolDownStart=1;
  bit.CoolDown=1;
  // 2014.11.24
  _NowWorkoutStatus = _WiFi_WorkoutState_CoolDown;
  //_CoolDownTime = Console_UserTime(Data_Get,0)/60;
  //
}  
/*******************************************************************************
* Function Name  : NavyFitnessTestStart
* Description    : Navy test��ʼ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NavyFitnessTestStart(void)
{
   bit.WarmUp=0;
   //Console_Time(Data_Set,0);
   Console_Information(Info_TIME,Data_Set,0);
   Console_UserTime(Data_Set,0);
   Console_MaxTime(Data_Set,99*60);
   //Console_Incline(Data_Set,2); 
   Console_Information(Info_WorkoutIncline,Data_Set,2);
   Console_Time_Cul();
   Console_TimeOver(Data_Set,0);
   Console_ClearDistance();
   Console_Set_StartAccumulated(1);//==>��ʱ�俪ʼ�ۼ�
   Console_Set_FitTestTimeAcc(1);
   
}

/*******************************************************************************
* Function Name  : Fitness_Status
* Description    : Fitnessģʽ������ʾ״̬
* Input          : None
* Output         : None
* Return         : 1=Fitness / 0=NO
*******************************************************************************/
u8 Fitness_Status(u8 by_Dat)
{        
    LED_END();
    switch(Console_ProgramMode(Data_Get,0))
    {
        case TM_WFI:
                 if(by_Dat == 1)
                     Show_WFIinInformation(0);
                 else
                     Show_WFIinInformation(1);//==>����������
                 return 1; 
        case TM_Gerkin:
                 if(by_Dat == 1)
                     Show_GerkinInformation(0);
                 else
                     Show_GerkinInformation(1);//==>����������
                 return 1; 
        case TM_PEB:
        case TM_Army:  
        case TM_Airforce: 
        case TM_Navy: 
        case TM_Marine:  
                 if(Console_ProgramMode(Data_Get,0) == TM_Navy)
                 {
                     if(by_Dat == 1)
                         Show_NavyInformation(0);
                     else 
                         Show_NavyInformation(1);
                 }    
                 else
                 {
                     if(by_Dat == 1)
                         Show_FITInformation(0);
                     else
                         Show_FITInformation(1);
                 }    
                 return 1; 
    }
    return 0;
}
/*******************************************************************************
* Function Name  : WFI_End_CoolDownShow()
* Description    : WFI cool downģʽ����ʾ
* Input          : ��ʾ��Ŀ
* Output         : None
* Return         : None
*******************************************************************************/
void WFI_End_CoolDownShow(void)//==>980724
{
    switch(Show_Loop)
    {
        case 1:Show_Message(Message_GoalAchieved,0,0,Show_Auto);
               break; 
        case 2:Show_Message(Message_CoolDown,0,0,Show_RightToLeft);
               break;
        case 3:Show_Loop++;
               Timer_Counter_Clear(8); 
               Led_String(" VO2 :    ",0,0);
               Led_Window(Window_2,4,Console_Get_VO2());
               break;
    }    
    if(Show_Loop < 3 && Led_Get_String_End())
    {//==>�����ַ���
        Show_Loop++;
    }
    else if(Show_Loop >= 3 && Timer_Counter(T_STEP,8,50))
    {        
        Show_Loop=1;
        Show_Message(Message_Blank,0,0,Show_Blank);
        bit.ShowCoolDownStart=0;
    }    
    
}
/*******************************************************************************
* Function Name  : Workout
* Description    : work out
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
const unsigned char ToggleDisplayInfo[4][3]=
{{BIT1,Target_Calories     ,Target_Incline},
 {BIT2,Target_DistanceMiles,Target_Speed},
 {BIT3,Target_TimeElapsed  ,Target_Clock},
 {BIT4,Target_Calories     ,Target_Watts}
};

u8 Workout(void)
{
    u8 by_Status = M_NONE;
    u8 by_Beep;    
    u8 by_NoHRTime = 0;//==>��ΪGerkin program�������ʹ���ۼ�
    u8 by_PauseTimeShowChang = 0;
    u8 by_HRSum = 0;
    u8 by_Conversion = 0;
    u8 by_FitnessENDStatus = 0;
    u8 _EN957_HRZero = 0;
    u8 _EN957_Delay = 0;
    u8 _TargetWindow=0;
    u8 _TargetWindowStatus = 0;
    u8 TargetWindow[4]={ Target_Calories,
                         Target_DistanceMiles,
                         Target_TimeElapsed,
                         Target_Speed
                        };
    u8 WindowDisplay[4]={Target_Calories,
                         Target_DistanceMiles,
                         Target_TimeElapsed,
                         Target_Speed
                        };
    u8 _ChangDFTime = 0;
    u8 _DisplayWindowScan[4] = {0};
    u8 _WindowScanBit = 0;
    // ������ʾ��ԭ�л���(2��һ��)
    // [0] Incline ��ʾ����
    // [1] �л�ǰ����ʾλ��
    // [2] Speed ��ʾ����
    // [3] �л�ǰ����ʾλ��
    // [4] Time ��ʾ����
    // [5] �л�ǰ����ʾλ��
    u8 _ItemBuffer[6] = {0};
    //
    u8 by_HRControlTime = 0;
    u8 by_HRControlNumber = 0;
    u16 by_HRBuffer=0;
    u16 by_PauseTime=0;
    u16 _TotalNumber;
    u16 by_KeyNumber;//==>��������
    
    u16 inclineAdcValue = 0; //peter add
//    u8   needCalibration = 1; //peter add
    u8   needCalibration = EEPROM_ReadInformation(EE_InclineCalibrationFlag);
    
    _ItemBuffer[0] = 1;
    _ItemBuffer[1] = Target_Calories;
    _ItemBuffer[2] = 1;
    _ItemBuffer[3] = Target_Speed;
    _ItemBuffer[4] = 0;
    _ItemBuffer[5] = Target_TimeElapsed;
    Show_Loop=1;
    
    bit.WiFi_SendSummary = 0;
    bit.ShowChang=0;
    bit.NumberKey=0;
    bit.THR_Over=0;
    bit.FitTest=0;
    bit.WorkEnd=0;
    bit.KeyColse=0;
    bit.FirstTime = 0;
    bit.FirstHR = 0;
    bit.ChangProgram=0;
    bit.ShowCoolDownStart=0;
    bit.FitTest_END = 0;
    bit.USB_SaveSummary = 0;
    bit.RF900Show = 0;
    bit.RF900ShowStart = 0;
    bit.Choose = 0;
    
    RF900_ClearMessageNumber();
//    Led_Set_Target(Target_WiFiSignal,0);
    if(by_Address == M_START)
    {   
        bit.WarmUp=0;
        bit.CoolDown=0;  
        Workout_CsafeWorkoutInformation(0);//==> CSAFE������Ϣ����
        Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
        Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),1);
        Led_ProfileMap(2);//==>ȡͼ�����
        Console_UserTime(Data_Set,Console_Information(Info_TIME,Data_Get,0));//Console_Time(Data_Get,0)
        switch(Console_ProgramMode(Data_Get,0))
        {
            case TM_TargetHR:
                    bit.WarmUp = 1;
                    by_HoldBuffer = Console_Information(Info_TIME,Data_Get,0);//Console_Time(Data_Get,0);
                    Console_Information(Info_TIME,Data_Set,120);
                    Console_UserTime(Data_Set,120);
                    break;
            case TM_Navy:
                    bit.WarmUp=1;
                    Console_Information(Info_TIME,Data_Set,180);
                    Console_UserTime(Data_Set,180);
                    break;
            case TM_Gerkin:
            case TM_WFI:
                    bit.KeyColse=1;//==>�ر����ּ�����
                    bit.WarmUp=1;
                    by_HoldBuffer=15*60;
                    Console_Information(Info_TIME,Data_Set,180);
                    Console_UserTime(Data_Set,180);
                    if(Console_SpeedUnit(Data_Get,0) == C_KM)
                        Console_StartSpeed(Data_Set,20);//==>2KPH 
                    else  
                        Console_StartSpeed(Data_Set,13);//==>1.3MPH 
                    break;
            case TM_5K:
            case TM_10K:  
            case TM_Marine:
            case TM_PEB:
            case TM_Army:
            case TM_Airforce:
                    Console_Information(Info_TIME,Data_Set,0);
                    Console_UserTime(Data_Set,0);
                    Console_MaxTime(Data_Set,99*60);
                    break;
                    
        }

        Console_NewTime(Data_Set);
        while(BEEP_Get_OK()); 
        for(by_Beep=0;by_Beep < 3;by_Beep++)
        {
            BEEP_Proce(2,12,12);
            Show_Message(Message_3+by_Beep,0,0,Show_Auto);
            while(BEEP_Get_OK())
            {
                Key_Proce();//==>TV control
                if(!Check_SafeKey())
                {
                    by_Beep=6;
                    bit.SafeKey=1;
                    by_Status=M_RESET;
                }
            }
            if(by_Beep == 2)
            {
                Console_Information(Info_WorkoutSpeed,Data_Set,Console_StartSpeed(Data_Get,0)*10);
                Digital_Set_DigitalSpeedTarget(1);//==>��ʼ׷��Ŀ���ٶ�
            }
        }
        
        Digital_Command(CmdGetTreadmillInUsed,0);//==>��Ա���
        
    //peter add incline auto calibration
    if(needCalibration)
    {          
      Timer_Counter_Clear(2);
      Timer_Counter_Clear(3);
      inclineAdcValue = Digtial_Get_InclineLocation();

      Digital_Set_Work(0);//==>��ENGģʽ��
    }
    //peter end
    else
    {    
      Console_GetInclineInformation();
      if(Digtial_ElevationVariationCheck(1) == 1)
      {//==>������ʵ��ֵ��Ŀ��ֵ���ʱ,�����֮ǰ���趨��Ŀ��ֵ����Ȼ��������ȡ��
        Digital_Clear_ElevationOld();//==>������ɵ�AD�ݴ�ֵ  
        Digital_Set_InclineCheck(1);
      } 
    }
    by_Address=M_ReWork;
    }
    else
    {
        if(!bit.Stop)
        {//==>����̨Ϊ�˶���ʱ
            Digital_Set_DigitalSpeedTarget(1);//==>��ʼ׷��Ŀ���ٶ�
            Digital_Set_InclineCheck(1);//==>�����趨
            if(bit.WarmUp)
            {
                by_HoldBuffer = Console_Information(Info_TIME,Data_Get,0);//Console_Time(Data_Get,0);
                Console_Information(Info_TIME,Data_Set,120);
                Console_UserTime(Data_Set,120);
            }
        }
    }
    // 2014.11.24
    if(bit.WarmUp == 1)
    {
        _NowWorkoutStatus = _WiFi_WorkoutState_WarmUp;
    }
    else
    {
        _NowWorkoutStatus = _WiFi_WorkoutState_Workout;
    }
    //
    Show_Message(Message_Blank,0,0,Show_Blank);//==>�������
    Console_Time_Cul();
    Console_TimeStart(1);//==>����ʼ�˶�ʱ������ʱ�����
    Console_WorkoutStatus(Data_Set,1);
    if(Console_ProgramMode(Data_Get,0) == TM_Navy)
    {
        Console_Set_StartAccumulated(0);//==>��ʱ��ֹͣ�ۼ�
        Console_Set_FitTestTimeAcc(0);
    }
    else
    {
        Console_Set_StartAccumulated(1);//==>��ʱ�俪ʼ�ۼ�
        Console_Set_FitTestTimeAcc(1);
    }    
    Heart_Start();
    by_WorkoutDFNewProgram=0;
//    if(Console_SpeedUnit(Data_Get,0) == C_KM)
//    {
//        TargetWindow[1] = Target_DistanceKM;
//        WindowDisplay[1] = Target_DistanceKM;
//    }
    Show_Distance(Console_SpeedUnit(Data_Get,0));
    Show_Calories();
//    if(Console_UserTime(Data_Get,0) == 0)
    if(EEPROM_ReadInformation(EE_TimerMode) == Time_UP)
    {
      Show_Time(Time_UP);
      TargetWindow[2] = Target_TimeElapsed;
    }
    else
    {
      Show_Time(Time_DOWN);
      TargetWindow[2] = Target_TimeRemaining;
    }
    // 2014.11.24
    ChangeWorkoutState(WIFI_WORKOUT_RUNNING);
    //

    
    while(by_Status == M_NONE)
    {
        //UsbFunction_USB(0x500);
      
#ifdef _DebugClose 
        if(ErrorCodeDisplay_Mode(1) == M_RESET)
        {//==>�˶�ģʽ 
            by_Status = M_RESET; 
        }
#endif
        if(!bit.ChangProgram && !bit.Stop && !bit.Pause && !bit.WorkEnd)
        {//==>�˶���     
            Heart_Decoder();
            if(Console_Calories_Counter_Proce()||\
               Console_Distance_Counter_Proce() )
            {// Calories�����ۼƽ�λʱ
             // Distance�����ۼƽ�λʱ 
                bit.ShowInformation = 1;
            } 
            //peter add incline auto calibration
            if((bit.QuickStart == 1)&&needCalibration )
            {
              if((Digital_GetStatus() & 0x01) == 1)
              {
                if(Timer_Counter(T_STEP,2,1))
                {
                  Digital_Command(CmdGetInclineLocation,0);
                  Timer_Counter_Clear(2);
                }
                if(Timer_Counter(T_STEP,3,20)) 
                {
                  if(Digtial_Get_InclineLocation() < inclineAdcValue-(2*128))
                  {
                    inclineAdcValue = Digtial_Get_InclineLocation();
                  }
                  else
                  {
                    Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ
                    Console_HighLowADC(Data_Set,1,Digtial_Get_InclineLocation()+(163 + 13)*128);//(12*128)
                    Console_HighLowADC(Data_Set,0,Digtial_Get_InclineLocation()+13*128);//(9*128)
                    EEPROM_SaveWorkInformation(EE_LOW_ELEVATION,Console_HighLowADC(Data_Get,0,0));
                    EEPROM_SaveWorkInformation(EE_MAX_ELEVATION,Console_HighLowADC(Data_Get,1,0));
                    needCalibration = 0;
                    EEPROM_WriteInformation(EE_InclineCalibrationFlag,needCalibration);
                    Digital_Set_Work(1);//==>���˶�ģʽ��
                    if((Digital_GetStatus() & 0x03) != 0)// S003-01
                    {
                      //==>> ��������ֹͣ����,ȷ��RELAY�Ķ�����ȷ 
                      Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ
                    }     
                    Console_GetInclineInformation();
                    if(Digtial_ElevationVariationCheck(1) == 1)
                    {//==>������ʵ��ֵ��Ŀ��ֵ���ʱ,�����֮ǰ���趨��Ŀ��ֵ����Ȼ��������ȡ��
                      Digital_Clear_ElevationOld();//==>������ɵ�AD�ݴ�ֵ  
                      Digital_Set_InclineCheck(1);
                    }                     
                  }
                  Timer_Counter_Clear(3);
                }
              }
              else
              {
                if(Timer_Counter(T_STEP,2,3))
                {
                  Digital_Command(CmdSetInclineAction,0xff);//==>�����½�
                  Timer_Counter_Clear(2);
                }
                Timer_Counter_Clear(3);
              }
            }
            
            //if(Console_Distance_Counter_Proce())
            //{//==>Distance�����ۼƽ�λʱ
            //    bit.ShowInformation = 1;
            //} 
            if(_ChangDFTime == 0 && Console_Time_Counter_Proce())
            {//==>ÿ���ӳ������
                bit.ShowInformation = 1;// ��Ϣ��ʾ
                Workout_Information(TargetWindow[2]);//show time
                //===========> ESD�Բ���
                Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
                // <=====================
                // CSAFE ���ݴ���
                Workout_CsafeWorkoutInformation(0);//==> CSAFE������Ϣ����
                //
                if(!bit.CoolDown)
                {//==>��Cool downģʽ��
                    switch(Console_ProgramMode(Data_Get,0))
                    {
                        case TM_Manual:
                                 if(Console_Time_Seg_Proce() == 1)
                                 {
                                     Led_ProfileMap(2);//==>ȡͼ�����
                                 }
                                 break;
                        case TM_5K:    
                        case TM_10K:  
                                 if(Console_5K10K_Seg_Proce() == 0)
                                 {
                                     break;
                                 }
                        case TM_Rolling:
                        case TM_FatBurn:
                                 if((Console_ProgramMode(Data_Get,0) == TM_Rolling ||  
                                     Console_ProgramMode(Data_Get,0) == TM_FatBurn) &&
                                     Console_Time_Seg_Proce() == 0)
                                 { 
                                     break;
                                 }
                                 Led_ProfileMap(2);//==>ȡͼ�����
                                 Console_GetInclineInformation();
                                 // ��Incline�����л�
                                 if(TargetWindow[0] != Target_Incline)
                                 {
                                     _ItemBuffer[0] = 1;
                                     _ItemBuffer[1] = TargetWindow[0];
                                     Led_Set_Target(TargetWindow[0],0);
                                     TargetWindow[0] = Target_Incline;
                                     bit.ShowInformation = 1;
                                 }
                                 //
                                 break;
                        case TM_TargetHR:
                                 if(Console_Time_Seg_Proce() == 1)
                                 {
                                     Led_ProfileMap(2);//==>ȡͼ�����
                                 }
                                 if(!bit.WarmUp)
                                 {
                                     if(Console_GetHRControlMode() == 0)
                                     {// Incline
                                         by_HRControlTime = 5;// 5��
                                         by_HRControlNumber = 1;
                                     }
                                     else
                                     {// Speed
                                         by_HRControlTime = 20;// 20��
                                         if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                             by_HRControlNumber = 5;// 0.5 km/h
                                         else   
                                             by_HRControlNumber = 3;// 0.3 mph
                                     }
                                     if(by_HoldBuffer >= by_HRControlTime - 1)
                                     {//==>ÿx�������
                                         bit.THR_Safe=0;
                                         by_HoldBuffer=0;
                                         by_HRBuffer /= by_HRSum;
                                         if(by_HRBuffer > 40 || Heart_Get_Heart() >= 240)
                                         {
                                             if(by_HRBuffer > 220-Console_Information(Info_AGE,Data_Get,0))//Console_AGE(Data_Get,0)
                                             {//==>����ֵ > Ŀ��ֵ�����Χֵ
                                                 bit.THR_Over=1;
                                             }
                                             else if(by_HRBuffer > Heart_Get_Target()+4 ||
                                                     by_HRBuffer < Heart_Get_Target()-4 )
                                             {//==>����ֵ > Ŀ��ֵ+4
                                                 if(by_HRBuffer > Heart_Get_Target()+4)
                                                 {
                                                     Console_HRControlChange(D_DEC,by_HRControlNumber);
                                                 }
                                                 else
                                                 {
                                                     Console_HRControlChange(D_ADD,by_HRControlNumber);
                                                 }
                                                 if(Console_GetHRControlMode() == 0)
                                                 {// Incline
                                                     // ��Incline�����л�
                                                     if(TargetWindow[0] != Target_Incline)
                                                     {
                                                         _ItemBuffer[0] = 1;
                                                         _ItemBuffer[1] = TargetWindow[0];
                                                         Led_Set_Target(TargetWindow[0],0);
                                                         TargetWindow[0] = Target_Incline;
                                                     }
                                                 }
                                                 else
                                                 {// Speed
                                                     // ���ٶȴ����л�
                                                     if(TargetWindow[1] != Target_Speed)
                                                     {
                                                         _ItemBuffer[2] = 1;
                                                         _ItemBuffer[3] = TargetWindow[1];
                                                         Led_Set_Target(TargetWindow[1],0);
                                                         TargetWindow[1] = Target_Speed;
                                                     }
                                                 }
                                                 bit.ShowInformation = 1;
                                             }
                                             else bit.THR_Safe=1;
                                         }
                                         by_HRSum=0;
                                         by_HRBuffer=0;                        
                                     }
                                     else 
                                     {//==>ÿ���ۼӨ��
                                         by_HoldBuffer++;
                                         if(Heart_Get_Heart() >= 40)
                                         {//==>��Ϊ�˷�Χʱ�ͽ����ۼӵĶ���
                                             by_HRBuffer += Heart_Get_Heart();
                                             by_HRSum++;
                                         }
                                     }
                                 }
                                 //==> EN957 
                                 if(Heart_Get_Heart() < 40)
                                 {// ��û������ʱ
                                     if(_EN957_Delay >= 10)
                                     {//==>ÿ 10 ������ٶ�һ��
                                         if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                             _EN957_Delay = 80;
                                         else
                                             _EN957_Delay = 50;
                                         if(Console_Information(Info_WorkoutSpeed,Data_Get,0) > _EN957_Delay)//Console_Speed(Data_Get,0)
                                         {// ������ʱ   
                                             bit.RF900Show = 0;
                                             if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                                 Console_Information(Info_WorkoutSpeed,Data_Dec,10);
                                             else
                                                 Console_Information(Info_WorkoutSpeed,Data_Dec,6);
                                             Led_Set_Target(TargetWindow[1],0);
                                             // ���ٶȴ����л�
                                             if(TargetWindow[1] != Target_Speed)
                                             {
                                                 _ItemBuffer[2] = 1;
                                                 _ItemBuffer[3] = TargetWindow[1];
                                             }
                                             //
                                             TargetWindow[1] = Target_Speed;
                                             if(bit.ShowCoolDownStart == 1)
                                             {
                                                 bit.ShowCoolDownStart=0;
                                                 Show_Message(Message_Blank,0,0,Show_Blank);
                                             }
                                             bit.ShowInformation = 1;
                                         }
                                         _EN957_Delay = 1;
                                     }
                                     else if(_EN957_Delay != 0)
                                     {
                                         _EN957_Delay++;  
                                     }
                                     else
                                     {
                                         if(_EN957_HRZero >= 30)
                                             _EN957_Delay = 1; 
                                         else 
                                             _EN957_HRZero++;
                                     }
                                 }
                                 else
                                 {
                                     _EN957_HRZero = 0;
                                     _EN957_Delay = 0;
                                 }
                                 break;
                        case TM_WFI:
                        case TM_Gerkin:
                                 
                                 if(Console_Time_Seg_Proce() == 1)
                                 {
                                     Led_ProfileMap(2);//==>ȡͼ�����
                                 } 
                                 if(!bit.WarmUp)
                                 {
                                     
                                     if(Heart_Get_Heart() > Heart_Get_Target())
                                     {
                                         by_HRBuffer++;                                     
                                     }
                                     else
                                     {
                                         by_HRBuffer=0;
                                     }
                                     if(by_HRBuffer >= 15 || Console_Parameter(Info_AccumulatedTime) >= 1080)
                                     {//==>��������Ŀ��ֵ����15����˶�18����
                                         by_NoHRTime = 0;
                                         bit.FitTest_END = 1;
                                         by_FitnessENDStatus = 1;//==>Fitness test ��������
                                     } 
                                     else 
                                     {
                                         by_Conversion = Console_GerkinConversion();
                                         if(by_Conversion != 0)
                                         {
                                             BEEP_Proce(2,2,2);
                                             if(bit.RF900Show == 0)
                                             {
                                                 if(by_Conversion == 1 && by_NoHRTime == 0 && !bit.ShowCoolDownStart) 
                                                 {// Speed
                                                     // ���ٶȴ����л�
                                                     if(TargetWindow[1] != Target_Speed)
                                                     {
                                                         _ItemBuffer[2] = 1;
                                                         _ItemBuffer[3] = TargetWindow[1];
                                                         Led_Set_Target(TargetWindow[1],0);
                                                         TargetWindow[1] = Target_Speed;
                                                     }
                                                     //
                                                     bit.ShowInformation = 1;
                                                 }    
                                                 else if(by_Conversion == 2) 
                                                 {// Inlcine
                                                     // ��Incline�����л�
                                                     if(TargetWindow[0] != Target_Incline)
                                                     {
                                                         _ItemBuffer[0] = 1;
                                                         _ItemBuffer[1] = TargetWindow[0];
                                                         Led_Set_Target(TargetWindow[0],0);
                                                         TargetWindow[0] = Target_Incline;
                                                     }
                                                     //
                                                     bit.ShowInformation = 1;
                                                 }    
                                             }
                                         }
                                     }
                                     if(!Heart_Signal())
                                     {//==>��û��HRѶ��ʱ
                                         by_NoHRTime++;
                                         if(by_NoHRTime >= 15)
                                         {//==>������������15��ͽ���ģʽ
                                             by_NoHRTime = 0;
                                             bit.FitTest_END = 1;
                                             by_FitnessENDStatus = 2;//==>Fitness test ����������
                                         }    
                                     } 
                                     if(Console_ProgramMode(Data_Get,0) == TM_WFI && bit.FitTest_END)
                                     {
                                         bit.RF900Show = 0;
                                         Led_Set_Reset();
                                         while(!Led_Get_String_End())
                                         {
                                             Show_Message(Message_GoalAchieved,0,0,Show_Auto);
                                         }
                                         Console_VO2();
                                     }
                                 }
                                 else
                                 {//����EN957������
                                     if(_EN957_Delay < 3)
                                     {
                                         _EN957_Delay++;
                                     }
                                     else
                                     {
                                         if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                             _EN957_HRZero = 48;
                                         else
                                             _EN957_HRZero = 30;
                                         if(Console_Information(Info_WorkoutSpeed,Data_Get,0)/ 10 < _EN957_HRZero)//Console_Speed(Data_Get,0)
                                         {// ��Ŀ���ٶ�ʱ
                                              bit.RF900Show = 0;
                                              Console_Information(Info_WorkoutSpeed,Data_Add,1);
                                              // ��Cool down�ַ�����ʾ
                                              if(bit.ShowCoolDownStart == 1)
                                              {
                                                  bit.ShowCoolDownStart = 0;
                                                  Show_Message(Message_Blank,0,0,Show_Blank);
                                              }
                                              //
                                              Led_Set_Target(TargetWindow[1],0);
                                              // ���ٶȴ����л�
                                              if(_ItemBuffer[2] != 0 || TargetWindow[1] != Target_Speed)
                                              {
                                                  _ItemBuffer[2] = 1;
                                                  if(TargetWindow[1] != Target_Speed)
                                                  {
                                                      _ItemBuffer[3] = TargetWindow[1];
                                                  }
                                              }
                                              else _ItemBuffer[3] = TargetWindow[1];
                                              //
                                              TargetWindow[1] = Target_Speed;
                                         }
                                     }
                                 }
                                 break;
                        case TM_PEB:
                        case TM_Army:  
                        case TM_Airforce: 
                        case TM_Navy: 
                        case TM_Marine:  
                                 if(Console_Time_Seg_Proce() == 1)
                                 {
                                     Led_ProfileMap(2);//==>ȡͼ�����
                                 }
                                 if(Console_DistanceEND() == 1)
                                 {//==>Fitness Test�ľ����ж�
                                     bit.RF900Show = 0;
                                     bit.FitTest_END = 1;
                                     by_FitnessENDStatus = 1;//==>Fitness test ��������
                                     Console_ShowDistanceTarget(Data_Set,1);//==>ʹ��һ��Distance��ʾģʽ
                                 } 
                                 break;
                    }
                }
                else
                {//==>ΪCool downģʽ��ʱ
                    by_HoldBuffer++;
                    if(by_HoldBuffer % 20 == 0)
                    {//==>ÿ20's��һ����ʾ�ַ���
                        by_HoldBuffer = 0;
                        bit.RF900Show = 0;
                        bit.ShowCoolDownStart = 1;
                        bit.ShowInformation = 0;// ����Ϣ��ʾ
                        Show_Message(Message_Blank,0,0,Show_Blank);
                    }
                }
                if(by_Status != M_RESET && (bit.FitTest_END || bit.THR_Over || 
                   Console_TimeOver(Data_Get,0) || 
                  ((Console_DistanceGoal_Over()) && !bit.CoolDown))) 
                {//==>HR�������Ŀ��ֵ��ʱ�����������5K��C-SAFEĿ����ʱ        
                    BEEP_Proce(4,2,2);
                    bit.RF900Show = 0;
                    bit.FitTest_END = 0;
                    _DisplayWindowScan[0] = 0;// ȡ�������Զ��л�
                    _DisplayWindowScan[1] = 0;// ȡ�������Զ��л�
                    _DisplayWindowScan[2] = 0;// ȡ�������Զ��л�
                    _DisplayWindowScan[3] = 0;// ȡ�������Զ��л�
                    _WindowScanBit = 0;
                    if(bit.WarmUp)
                    {//==>Warm up ����
                        bit.WarmUp=0;
                        // 2014.11.24
                        _NowWorkoutStatus = _WiFi_WorkoutState_Workout;
                        //
                            if(Console_ProgramMode(Data_Get,0) == TM_Navy)
                            {
                                NavyFitnessTestStart(); //Timer Up 
                                TargetWindow[2] = Target_TimeElapsed;
                            }
                            else
                            {
                                Console_Information(Info_TIME,Data_Set,by_HoldBuffer);
                                Console_UserTime(Data_Set,by_HoldBuffer);
                                Console_Time_Cul();
                                Console_TimeOver(Data_Set,0);
                                by_HoldBuffer=0;
                                Show_Message(Message_Blank,0,0,Show_Blank);
                                bit.ShowInformation = 1;
                            }
                            Console_NewTime(Data_Set);// ȡĿǰ��Timeֵ
                        if(Console_ProgramMode(Data_Get,0) == TM_Gerkin || 
                           Console_ProgramMode(Data_Get,0) == TM_WFI) 
                        {
                            if(Console_ProgramMode(Data_Get,0) == TM_WFI)
                            {
                                Heart_Set_Target((u8)((float)(208-(0.7*Console_Information(Info_AGE,Data_Get,0)))*0.85));//Console_AGE(Data_Get,0)
                            }
                            else
                            {                          
                                Heart_Set_Target((u8)((float)(220-Console_Information(Info_AGE,Data_Get,0))*0.85));//Console_AGE(Data_Get,0)
                            }    
                            Console_MaxTime(Data_Set,by_HoldBuffer);
                            if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                Console_StartSpeed(Data_Set,72);//==>7.2KPH
                            else  
                                Console_StartSpeed(Data_Set,45);//==>4.5MPH
                        }
                        
                    }
                    else if(!bit.THR_Over && !bit.CoolDown && !bit.FitTest &&
                            (Console_ProgramMode(Data_Get,0) < TM_Gerkin ||
                             Console_ProgramMode(Data_Get,0) > TM_Marine))
                    {//==>�������� Cool down
                            Start_CoolDown();
                            Led_Set_Target(TargetWindow[2],0);
                            if(EEPROM_ReadInformation(EE_TimerMode) == Time_UP)
                              TargetWindow[2] = Target_TimeElapsed;
                            else
                              TargetWindow[2] = Target_TimeRemaining;
                            Led_Set_Target(TargetWindow[2],1);
                        // ���ٶȴ����л�
                        if(TargetWindow[1] != Target_Speed)
                        {
                            _ItemBuffer[2] = 1;
                            _ItemBuffer[3] = TargetWindow[1];
                            Led_Set_Target(TargetWindow[1],0);
                            TargetWindow[1] = Target_Speed;
                            Led_Set_Target(TargetWindow[1],1);
                        }
                        // ��Incline�����л�
                        if(TargetWindow[0] != Target_Incline)
                        {
                            _ItemBuffer[0] = 1;
                            _ItemBuffer[1] = TargetWindow[0];
                            Led_Set_Target(TargetWindow[0],0);
                            TargetWindow[0] = Target_Incline;
                            Led_Set_Target(TargetWindow[0],1);
                        }
                        //
                        bit.ShowInformation = 1;
                    }
                    else
                    {//==>�˶�ʱ�����ʱ
                        //Led_FanControlReset();//==>�رշ���
                        Console_WorkoutStatus(Data_Set,0);
                        Digital_Set_DigitalSpeedTarget(0);//==>�ٶȹ���
                        Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ  
                        bit.WorkEnd=1;
                        Timer_Counter_Clear(2);
                        CSAFE_Work_Exit();
                        if(Fitness_Status(by_FitnessENDStatus) == 1)
                        {
                            by_Status=M_RESET; 
                        }
                    }    
                }
                // �����Զ��л�
                if(_WindowScanBit & BIT1)
                {
                    _DisplayWindowScan[0]++;
                    if(_DisplayWindowScan[0] > 10)
                    {// ÿ10��һ��
                        _DisplayWindowScan[0] = 1;
                        Led_Set_Target(TargetWindow[0],0);
                        TargetWindow[0]++;
                        if(TargetWindow[0] > Target_Incline)
                        {  
                            TargetWindow[0] = Target_Calories;
                        }
                        bit.ShowInformation = 1;
                        bit.RF900Show = 0;
                    }
                }
                if(_WindowScanBit & BIT2)
                {
                    _DisplayWindowScan[1]++;
                    if(_DisplayWindowScan[1] > 10)
                    {// ÿ10��һ��
                        _DisplayWindowScan[1] = 1;
                        Led_Set_Target(TargetWindow[1],0);
                        TargetWindow[1]++;
                        if(TargetWindow[1] > Target_Speed)
                        {
                            TargetWindow[1] = Target_DistanceMiles; 
                        }
                        bit.ShowInformation = 1;
                        bit.RF900Show = 0;
                    }
                }
                if(_WindowScanBit & BIT3)
                {
                    _DisplayWindowScan[2]++;
                    if(_DisplayWindowScan[2] > 10)
                    {// ÿ10��һ��
                        _DisplayWindowScan[2] = 1;
                        Led_Set_Target(TargetWindow[2],0);
                        TargetWindow[2]++;
                        if(TargetWindow[2] > Target_Clock) 
                        {
                            TargetWindow[2] = Target_TimeElapsed;  
                        }   
                        bit.ShowInformation = 1;
                        bit.RF900Show = 0;
                    }
                }
                if(_WindowScanBit & BIT4)
                {
                    _DisplayWindowScan[3]++;
                    if(_DisplayWindowScan[3] > 10)
                    {// ÿ10��һ��
                        _DisplayWindowScan[3] = 1;
                        Led_Set_Target(TargetWindow[3],0);
                        TargetWindow[3]++;
                        if(TargetWindow[3] > Target_Watts) 
                        {
                            TargetWindow[3] = Target_Calories;  
                        }
                        bit.ShowInformation = 1;
                        bit.RF900Show = 0;
                    }
                }
                // �Ǵ����ֶλ�ԭ��ʾ����
                if(_ItemBuffer[0] != 0) _ItemBuffer[0]++;
                if(_ItemBuffer[2] != 0) _ItemBuffer[2]++;
                //
            }// <<== ��λ���ӳ����ֹ
            
            if(bit.ShowCoolDownStart && bit.RF900Show == 0)
            {//==>��COOL DOWN��ʾ�ַ�����
                if(Console_ProgramMode(Data_Get,0) == TM_WFI)
                {//==>
                    WFI_End_CoolDownShow();
                }    
                else
                {
                    Show_Message(Message_CoolDown,0,0,Show_DownToUp);
                    if(Led_Get_String_End())
                    {//==>�������ַ���
                        bit.ShowCoolDownStart=0;
                        bit.ShowInformation = 1;
                    }
                    _ChangDFTime = 0;
                    bit.FirstTime = 0;
                }
            }
            
            if(!bit.CoolDown && \
              (Console_ProgramMode(Data_Get,0) == TM_Gerkin || \
               Console_ProgramMode(Data_Get,0) == TM_WFI)) 
            {//==>��Gerkin or WFI program��
                if(by_NoHRTime > 0)
                {// ��û������ʱ
                    if(Heart_Get_Heart() > 40)
                    {//==>��������ֵʱ�ͻ�ԭ��ʾ
                        by_NoHRTime=0;
                        bit.ShowChang=1;
                        Show_Message(Message_Blank,0,0,Show_Blank);
                        bit.ShowInformation = 1;
                    }
                    else if(Heart_Signal())//Heart_Get_HRSignal()
                    {//==>����HRѶ��ʱ
                        if(!bit.ShowChang)
                        {
                            Show_Message(Message_HeartRateDetected,0,0,Show_Auto);
                        }
                        if(Led_Get_String_End())
                        {
                            bit.ShowChang=1;
                            by_NoHRTime=0;
                            Show_Message(Message_Blank,0,0,Show_Blank);
                            bit.ShowInformation = 1;
                        }
                    }    
                    else  
                    {//==>�������������ʱ������ʾ�ַ���
                        bit.RF900Show = 0;
                        bit.ShowChang=0;
                        Show_Message(Message_GraspHRSensors,0,0,Show_Auto);
                    }   
                    
                }
            }
            //�������ʾͼ
            Led_InclineChangeSetProfileMap();
        }
        else
        {//==>�˶���ͣ���л�Programʱ
            if(bit.NumberKey)
            {
                bit.NumberKey=0;
            }  
            _ChangDFTime = 0;
            bit.FirstTime = 0;
            if(bit.ChangProgram)
            {//==>���е���Programʱ
                if(by_WorkoutDFNewProgram != 0)
                { 
                    Show_Message(_ProgramInfor[by_WorkoutDFNewProgram-1][2],0,0,Show_Auto);
                    if(Led_Get_String_End())
                    {//==>�ַ����������LED��
                        bit.ChangProgram=0;
                    }
                }
                else
                {
                    if(_Message_Status == _Message_Keep)
                    {
                        if(Timer_Counter(T_STEP,1,20))
                        {//==>��ʾ2���ָ�������ʾ����
                            bit.ChangProgram=0;                    
                        } 
                    }
                    else
                    {
                        if(Console_ProgramMode(Data_Get,0) == TM_FatBurn)
                        {//==>��Ϊ�ַ���̫��,���Ա���ʹ�þ��ķ�ʽ
                            Show_Message(Message_FatBurn,0,0,Show_Auto);
                        }
                        else if(Console_ProgramMode(Data_Get,0) == TM_Rolling)
                        {//==>��Ϊ�ַ���̫��,���Ա���ʹ�þ��ķ�ʽ
                            Show_Message(Message_RollingHills,0,0,Show_Auto);
                        }
                        if(Led_Get_String_End()) bit.ChangProgram=0;   
                    }
                    if(by_KeyNumber == K_ENTER)
                    {
                        bit.ChangProgram=0;
                    }
                }
                if(!bit.ChangProgram)
                {
                    Led_Set_Target(Target_Program,0);
                    Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
                    Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
                    switch(Console_ProgramMode(Data_Get,0))
                    {
                        case TM_5K:         
                        case TM_10K:   
                        case TM_Rolling:
                        case TM_FatBurn:
                                 if(by_WorkoutDFNewProgram == 0)
                                 {// �˶��������Porgram
                                     bit.KeyColse=0;
                                     by_Status = Workout_LevelChange();
                                     switch(by_Status)
                                     {// ����LEVELѡ��
                                         case M_RESET:
                                         case M_SP1:// Press PAUSE key
                                         case M_SP2:// Press STOP key  
                                                  break;
                                         default:
                                                  by_Status = M_NONE;
                                                  break;
                                     }
                                 }
                                 break;    
                    }
                    by_WorkoutDFNewProgram = 0;
                    Console_GetInclineInformation();// ȡ��Ӧ��Incline
                    Show_Message(Message_Blank,0,0,Show_Blank);
                    Led_ProfileMap(2);//==>ȡͼ�����
                    bit.ShowInformation = 1;
                }
            }
            else
            {
                if(bit.Pause)
                {//==>Pause time for Engineer mode default
//                    if(by_PauseTimeShowChang <= 5)
                    {
                        Led_7SEG(2,(by_PauseTime/60*100)+(by_PauseTime%60));
                    }    
                    if(by_PauseTimeShowChang > 5)
                    {
                        if(Led_Get_String_End())
                        {
                            by_PauseTimeShowChang=0;
                            Show_Message(Message_Paused,0,0,Show_Auto);
                        }
                        else
                        {
                            Show_Message(Message_PressStartToResume,0,0,Show_Auto);
                        }    
                    }    
                    if(Timer_Counter(T_STEP,2,10))
                    {
                        Timer_Counter_Clear(2);
                        if(by_PauseTime == 0)
                        {
                            CSAFE_Work_Exit(); 
                            Show_Message(Message_Blank,0,0,Show_Blank);
                            by_Status=M_RESET;
                        } 
                        else by_PauseTime--;
                        if(by_PauseTimeShowChang == 5)
                        {   
                            Show_Message(Message_PressStartToResume,0,0,Show_Auto);
                        }
                        by_PauseTimeShowChang++;
                    }
                }
                else if(bit.WorkEnd && bit.THR_Over)
                {//==>���˶�������THRĿ��ֵ����ʱ
                    Show_Message(Message_ExceedingMaxHR,0,0,Show_Auto);
                    if(Timer_Counter(T_STEP,2,300))
                    {
                        Show_Message(Message_Blank,0,0,Show_Blank);
                        by_Status=M_RESET;
                    }
                }
                else if(bit.WorkEnd || bit.Stop)
                {//==>Stop time and Workout END time = 30`s for Summary screen
                    // 2014.11.24
                    _WorkoutFinish = 1;
                    // 2014.11.24
                    ChangeWorkoutState(WIFI_WORKOUT_STOP);
                    // 
                    Show_WorkoutSummary();
                    Led_Set_Target(Display_OFF,0);                    
                    by_Status = M_RESET;
                }
            }
        }
        if(bit.WorkEnd ==0 && bit.Stop == 0 )
        {
            Led_ProfileMap(1);//������
        }   
        by_KeyNumber = Key_Proce();
        //==>��NetPulse���յ�C-SAFE��Finish��commandʱ�ͽ����˶�
        if(CSAFE_GetNetPulse() == 1 && CSAFE_GetInformation(_Get_Mode) == Csafe_Finished)
        {
            by_KeyNumber = K_STOP;
        }
        //
        //======================================================================  
        //==>> ��Ա��⹦��
#ifdef _TreadSense 
        if((by_KeyNumber != K_NONE)||(Heart_Get_Heart() > 40))
        {
            by_NonInUserTime = 0;
            Digital_CeckNonInUser = 0;
        }
        if(!bit.Stop && !bit.Pause && !bit.WorkEnd && Digital_CeckNonInUser == 1 )//
        {
            switch(BeltStop_StatusCheck())
            {
                case 1:by_Status = M_RESET;
                       break;
                case 2:by_KeyNumber = K_STOP;
                       break;
                case 3:by_KeyNumber = K_PAUSE;
                       break;
            }
        }
#endif        
        //======================================================================
        if(by_Status == M_SP1)
        {
            by_Status = M_NONE;
            by_KeyNumber = K_PAUSE;           
        }
        else if(by_Status == M_SP2)
        {
            by_Status = M_NONE;
            by_KeyNumber = K_STOP;  
        }
        switch(by_KeyNumber)
        {
            case K_STOP | K_MASK:
                     CSAFE_Work_Exit();
                     by_Status = M_RESET;
                     break;
            //case K_PAUSE:         
            case K_CoolDown:
                     if(!bit.ChangProgram && !bit.CoolDown && !bit.Stop && !bit.Pause && !bit.WorkEnd)
                     {
                         if(Console_ProgramMode(Data_Get,0) >= TM_Gerkin) 
                         {// Fitness Test Program
                             by_NoHRTime=0;
                             bit.FitTest=1;
                             Console_ShowDistanceTarget(Data_Set,1);//==>ʹ��һ��Distance��ʾģʽ
                             if(Console_ProgramMode(Data_Get,0) == TM_WFI)
                             {
                                 Console_VO2();
                             }
                         }
                         if(Console_ProgramMode(Data_Get,0) == TM_5K ||\
                            Console_ProgramMode(Data_Get,0) == TM_10K)
                         {
                             Console_ShowDistanceTarget(Data_Set,1);//==>ʹ��һ��Distance��ʾģʽ
                         }
                         _ChangDFTime = 0;
                         bit.FirstTime = 0;
                         by_FitnessENDStatus = 2;//==>Fitness test ����������
                         Start_CoolDown(); 
                         Led_Set_Target(TargetWindow[2],0);
                         if(EEPROM_ReadInformation(EE_TimerMode) == Time_UP)
                           TargetWindow[2] = Target_TimeElapsed;
                         else
                           TargetWindow[2] = Target_TimeRemaining;
                         Led_Set_Target(TargetWindow[2],1);
                         // ���ٶȴ����л�
                         if(TargetWindow[1] != Target_Speed)
                         {
                             _ItemBuffer[2] = 1;
                             _ItemBuffer[3] = TargetWindow[1];
                             Led_Set_Target(TargetWindow[1],0);
                             TargetWindow[1] = Target_Speed;
                             Led_Set_Target(TargetWindow[1],1);
                         }
                         //
                         // ��Incline�����л�
                         if(TargetWindow[0] != Target_Incline)
                         {
                             _ItemBuffer[0] = 1;
                             _ItemBuffer[1] = TargetWindow[0];
                             Led_Set_Target(TargetWindow[0],0);
                             TargetWindow[0] = Target_Incline;
                             Led_Set_Target(TargetWindow[0],1);
                         }
                          //
                         bit.ShowInformation = 1;
                         _TargetWindowStatus = 1;
                     }
                     bit.RF900Show = 0;// 100.09.28
                     break;  
                     
            case K_STOP:
                     _TargetWindowStatus = 1;
                     if(!bit.Stop && !bit.WorkEnd && 
                        CSAFE_GetInformation(_Get_Mode) != Csafe_Finished)
                     {
                         Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
                         bit.Pause=0;
                         if(bit.ChangProgram)
                         {
                             bit.ChangProgram=0;
                             by_WorkoutDFNewProgram=0;
                         }
                         bit.ShowCoolDownStart=0;
                         _ChangDFTime = 0;
                         bit.Stop=1;
                         BEEP_Proce(4,2,2);
                         Console_TimeStart(0);
                         Console_WorkoutStatus(Data_Set,0);
                         Digital_Set_DigitalSpeedTarget(0);//==>�ٶȹ���
                         if(Digtial_ElevationVariationCheck(0) == 1)
                         {//==>������ʵ��ֵ��Ŀ��ֵ�������,�����֮ǰ���趨��Ŀ��ֵ����Ȼ��������ȡ��
                             Digital_Clear_ElevationOld(); //==>������ɵ�AD�ݴ�ֵ
                         }
                         Console_Information(Info_WorkoutIncline,Data_Set,0);
                         Timer_Counter_Clear(2);
                         CSAFE_Work_Exit();
                     }
                     else
                     {
                         Led_Set_Target(Display_OFF,0);
                         by_Status = M_RESET;
                     }
                     bit.RF900Show = 0;
                     break;
            //case K_CoolDown:         
            case K_PAUSE:
                     bit.RF900Show = 0;
                     _TargetWindowStatus = 1;
                     if(!bit.Stop && !bit.Pause && !bit.WorkEnd)
                     {
                         bit.ShowCoolDownStart=0;
                         bit.ChangProgram=0;
                         by_WorkoutDFNewProgram=0;
                         _ChangDFTime = 0;
                         BEEP_Proce(4,2,2);
                         Console_TimeStart(0);
                         Console_WorkoutStatus(Data_Set,0);
                         Digital_Set_DigitalSpeedTarget(0);//==>�ٶȹ���
                         Timer_Counter_Clear(2);
                         if(CSAFE_GetInformation(_Get_Mode) == Csafe_In_Use)
                         {
                             CSAFE_Answer(Csafe_Paused);//==>��ʱֹͣ�˶� 
                         }
                         by_PauseTime=0;
                         by_PauseTimeShowChang=0;
                         if(Console_ProgramMode(Data_Get,0) < TM_Gerkin)
                         {
                             bit.Pause=1;
                             Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ
                             Console_StartSpeed(Data_Set,EEPROM_ReadInformation(EE_StartSpeed));
                             Show_Message(Message_Paused,0,0,Show_Auto);
                             by_PauseTime=EEPROM_ReadInformation(EE_PauseTime);
                             bit.RF900ShowStart = 0;
                             
                         }
                         else
                         {
                             bit.Stop = 1;
                             CSAFE_Work_Exit();
                             Console_Information(Info_WorkoutIncline,Data_Set,0);
                         }
                         //RF900_SetCommand(0x44,1);
                         //TIM4->CCR1 = CCR1_Val_Close;
                         bit.ShowInformation = 0;
                         break;
                     } 
                     else
                     {
                         if(Console_ProgramMode(Data_Get,0) == TM_Gerkin)
                         {
                             CSAFE_Work_Exit();
                             by_Status = M_RESET;
                         }
                         else
                         {
            case K_GO:    bit.RF900Show = 0;
                             if(!bit.Pause && Console_ProgramMode(Data_Get,0) == TM_Navy && bit.WarmUp)
                             {//==>��ΪNavyģʽ����Warm up״̬��ʱ��������Warm upֱ�ӽ��в���
                                 NavyFitnessTestStart();
                                 Console_NewTime(Data_Set);// ȡĿǰ��Timeֵ
                             }           
                             if(bit.Pause && !bit.WorkEnd)
                             {
                                 if(Digital_Get_RPMStatus())
                                 {
                                     by_Status = Main_MotorStatusCheckMessage(1);
                                     if(by_Status == M_NONE)
                                     {
                                         if(by_PauseTimeShowChang <= 5)
                                             Show_Message(Message_Paused,0,0,Show_Auto);
                                         else
                                             Show_Message(Message_PressStartToResume,0,0,Show_Auto);
                                     }
                                 }
                                 else
                                 {
                                     by_Status = M_START;
                                 }
                                 if(by_Status == M_START)
                                 {
                                    Led_Set_Target(Target_Program,0); 
                                    by_Status=M_NONE;
                                    Led_Set_Target(_ProgramInfor[Console_ProgramMode(Data_Get,0)-1][0],1);
                                    for(by_Beep=0;by_Beep < 3;by_Beep++)
                                    {
                                        BEEP_Proce(2,12,12);
                                        Show_Message(Message_3+by_Beep,0,0,Show_Auto);
                                        while(BEEP_Get_OK())
                                        {
                                            if(!Check_SafeKey())
                                            {
                                                by_Beep=6;
                                                CSAFE_Work_Exit();
                                                by_Status = M_RESET;
                                            }
                                        }
                                    }
                                    bit.Stop=0;
                                    bit.Pause=0;
                                    Console_TimeStart(1);
                                    Console_WorkoutStatus(Data_Set,1);
                                    if((Console_ProgramMode(Data_Get,0) != TM_Gerkin)&&
                                       (Console_ProgramMode(Data_Get,0) != TM_WFI))
                                    {
                                        Console_Information(Info_WorkoutSpeed,Data_Set,Console_StartSpeed(Data_Get,0)*10);
                                    }    
                                    Digital_Set_DigitalSpeedTarget(1);//==>��ʼ׷��Ŀ���ٶ�
                                    if(Digtial_ElevationVariationCheck(1) == 1)
                                    {//==>������ʵ��ֵ��Ŀ��ֵ�������ʱ,�����֮ǰ���趨��Ŀ��ֵ����Ȼ��������ȡ��
                                        Digital_Clear_ElevationOld();//==>������ɵ�AD�ݴ�ֵ
                                        Digital_Set_InclineCheck(1);//==>�����趨
                                    }
                                    Show_Message(Message_Blank,0,0,Show_Blank);
                                    if(CSAFE_GetInformation(_Get_Mode) == Csafe_Paused) 
                                    {
                                        CSAFE_Answer(Csafe_In_Use);//==>�����˶�ʱ  
                                    }
                                 }
                             }
                         }
                     }
                     break;
            case K_TimeU|K_MASK: 
            case K_TimeU:
            case K_TimeD|K_MASK: 
            case K_TimeD:  
                     bit.Choose = 1; 
                     if(!bit.WarmUp && !bit.ChangProgram && !bit.CoolDown && !bit.Stop && !bit.Pause && !bit.WorkEnd && \
                       (Console_ProgramMode(Data_Get,0) == TM_Manual || 
                        Console_ProgramMode(Data_Get,0) == TM_TargetHR || 
                        Console_ProgramMode(Data_Get,0) == TM_Rolling || 
                        Console_ProgramMode(Data_Get,0) == TM_FatBurn))
                     {
                             // ��Cool down�ַ�����ʾ
                             if(bit.ShowCoolDownStart == 1)
                             {
                                 bit.ShowCoolDownStart = 0;
                                 Show_Message(Message_Blank,0,0,Show_Blank);
                             }
                             //
                             _ChangDFTime = 1;
                             Console_Set_StartAccumulated(0);//==>��ʱ��ֹͣ�ۼ�
                             if(bit.FirstTime == 0) 
                             {
                                 bit.FirstTime = 1;
                                 Console_NewTime(Data_Set);// ȡĿǰ��Timeֵ
                             }    
                             else
                             {
                                 Timer_Counter_Clear(4);
                             }   
                             if(by_KeyNumber == K_TimeU || by_KeyNumber == (K_TimeU|K_MASK))
                                 Console_NewTime(Data_Add);
                             else
                                 Console_NewTime(Data_Dec);
                             // ��Time�����л�
                             if(TargetWindow[2] != Target_TotalTime)
                             {
                                 _ItemBuffer[4] = 1;
                                 _ItemBuffer[5] = TargetWindow[2];
                                 Led_Set_Target(TargetWindow[2],0);// ��ǰ��
                                 TargetWindow[2] = Target_TotalTime;
                                 Led_Set_Target(TargetWindow[2],1);
                             }
                             //
                             bit.ShowInformation = 1;
                             _WindowScanBit &= ~BIT3;
                     }
                     bit.RF900Show = 0;// 100.09.28
                     break;
         
            case K_SpeedU|K_MASK: 
            case K_SpeedU:    
            case K_SpeedD|K_MASK:
            case K_SpeedD:  
                     bit.Choose = 1; 
                     if(!bit.ChangProgram && !bit.Stop && !bit.KeyColse && !bit.Pause && !bit.WorkEnd)// && !bit.CoolDown
                     {
                         // ���ٶȴ����л�
                         if(_ItemBuffer[2] != 0 || TargetWindow[1] != Target_Speed)
                         {
                             _ItemBuffer[2] = 1;
                             if(TargetWindow[1] != Target_Speed)
                             {
                                 _ItemBuffer[3] = TargetWindow[1];
                             }
                         }
                         else _ItemBuffer[3] = TargetWindow[1];
                         // ��Cool down�ַ�����ʾ
                         if(bit.ShowCoolDownStart == 1)
                         {
                             bit.ShowCoolDownStart = 0;
                             Show_Message(Message_Blank,0,0,Show_Blank);
                         }
                         // �а����ּ�����Ҫȡ��
                         if(bit.NumberKey == 1)
                         {
                             bit.NumberKey = 0;
                         }
                         //
                         if(by_KeyNumber == K_SpeedU || by_KeyNumber == (K_SpeedU|K_MASK))
                           Console_Information(Info_WorkoutSpeed,Data_Add,(by_KeyNumber & K_MASK)? 99:1);//by_SpeedBuffer);
                         else
                             Console_Information(Info_WorkoutSpeed,Data_Dec,1);//by_SpeedBuffer);
                         Led_Set_Target(TargetWindow[1],0);
                         TargetWindow[1] = Target_Speed;
                         bit.ShowInformation = 1;
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                         _WindowScanBit &= ~BIT2;
                     }
                     bit.RF900Show = 0;// 100.09.28
                     break;
            case K_InclineU:
            case K_InclineU|K_MASK:
            case K_InclineD:
            case K_InclineD|K_MASK:  
                     bit.Choose = 1; 
                     if(Console_ProgramMode(Data_Get,0) == TM_Navy && !bit.WarmUp) break;//==>980810 
                     if(!bit.ChangProgram && !bit.Stop && !bit.KeyColse && !bit.Pause && !bit.WorkEnd)
                     {
                         // ��Cool down�ַ�����ʾ
                         if(bit.ShowCoolDownStart == 1)
                         {
                             bit.ShowCoolDownStart = 0;
                             Show_Message(Message_Blank,0,0,Show_Blank);
                         }
                         //
                         if(by_KeyNumber == K_InclineU || by_KeyNumber == (K_InclineU|K_MASK))
                             Console_Information(Info_WorkoutIncline,Data_Add,0);
                         else
                             Console_Information(Info_WorkoutIncline,Data_Dec,0);
                         // ��Incline�����л�
                         if(_ItemBuffer[0] != 0 || TargetWindow[0] != Target_Incline)
                         {
                             _ItemBuffer[0] = 1;
                             if(TargetWindow[0] != Target_Incline)
                             {
                                 _ItemBuffer[1] = TargetWindow[0];
                             }
                         }
                         else _ItemBuffer[1] = TargetWindow[0];
                         //
                         Led_Set_Target(TargetWindow[0],0);
                         TargetWindow[0] = Target_Incline;
                         bit.ShowInformation = 1; 
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                         _WindowScanBit &= ~BIT1;
                     }
                     bit.RF900Show = 0;// 100.09.28
                     
                     break;
            case K_0:
            case K_1:
            case K_2:
            case K_3:
            case K_4:
            case K_5:
            case K_6:
            case K_7:
            case K_8:
            case K_9:
                     
                     if(!bit.ChangProgram && !bit.Stop && !bit.KeyColse && !bit.Pause && !bit.WorkEnd)
                     {//==>�����ڻ�̨��ͣ���˶������¾Ϳ���ʹ�����ּ�
                         Timer_Counter_Clear(0);
                         // ��Cool down�ַ�����ʾ
                         if(bit.ShowCoolDownStart == 1)
                         {
                             bit.ShowCoolDownStart = 0;
                             bit.ShowInformation = 1;
                             Show_Message(Message_Blank,0,0,Show_Blank);
                         }
                             if(bit.NumberKey == 0)
                             {
                                 Led_Window(Window_2,5,0); 
                                 Keyboard_NumberCount(3,K_BACK);
                             }
                             // ���ּ������ۼ�ֵ
                             _TotalNumber = Keyboard_NumberCount(3,by_KeyNumber);
                             // 
                             Led_Window(Window_2,4,_TotalNumber);
                             bit.NumberKey=1;
                             //
                         if(TargetWindow[1] != Target_Speed)
                         {
                             _ItemBuffer[3] = TargetWindow[1];
                             Led_Set_Target(TargetWindow[1],0);
                             TargetWindow[1] = Target_Speed;
                             Led_Set_Target(TargetWindow[1],1);
                             _ItemBuffer[2] = 1;
                         }
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                         _WindowScanBit &= ~BIT2;
                     }  
                     bit.RF900Show = 0;
                     break;
            case K_BACK:
                     if(bit.NumberKey)
                     {
                         bit.NumberKey=0;
                         Led_Window(Window_4,4,Console_Information(Info_WorkoutSpeed,Data_Get,0) / 10);
                     }
                     break;
            case K_ENTER:
                     if(!bit.ChangProgram && !bit.Stop && !bit.KeyColse && !bit.Pause && !bit.WorkEnd)
                     {
                         if(bit.NumberKey)
                         {
                             bit.NumberKey=0;
                             Timer_Close(0);//==>�رյ�0��ʱ���ۼ�
                             if(_ItemBuffer[2] != 0)
                             {
                                 _ItemBuffer[2] = 1;// ��SPEED�����л�
                             }
                             if((Console_SpeedUnit(Data_Get,0) == C_KM && \
                                (_TotalNumber >= 8 && _TotalNumber <= Console_MaxSpeed(Data_Get,0))) || \
                                (Console_SpeedUnit(Data_Get,0) == C_MILE && \
                                (_TotalNumber >= 5 && _TotalNumber <= Console_MaxSpeed(Data_Get,0))))
                             {
                                 Console_Information(Info_WorkoutSpeed,Data_Set,_TotalNumber * 10);
                             }
                         }
                     } 
                     else if(bit.ChangProgram)
                     {
                         if(by_WorkoutDFNewProgram > 4)
                         {               
                             CSAFE_Work_Exit();
                             by_Status = M_RESET;
                         }
                     }
                     if(_ChangDFTime == 1)
                     {
                         _ChangDFTime = 2;
                     }
                     bit.RF900Show = 0;
                     break;
            case K_MANUAL:
            case K_FatBurn:
            case K_Rolling:
                     if(!bit.CoolDown && !bit.Stop && !bit.Pause && !bit.WorkEnd)
                     {
                         bit.ChangProgram=1;
                         if(Console_ProgramMode(Data_Get,0) != TM_Manual && \
                            Console_ProgramMode(Data_Get,0) != TM_Rolling && \
                            Console_ProgramMode(Data_Get,0) != TM_FatBurn  )      
                         {
                             if(by_KeyNumber == K_MANUAL && by_WorkoutDFNewProgram != TM_Manual)
                             {
                                 by_WorkoutDFNewProgram = TM_Manual;
                             }
                             else if(by_KeyNumber == K_FatBurn && by_WorkoutDFNewProgram != TM_FatBurn)
                             {
                                 by_WorkoutDFNewProgram = TM_FatBurn;
                             }
                             else if(by_KeyNumber == K_Rolling && by_WorkoutDFNewProgram != TM_Rolling)
                             {
                                 by_WorkoutDFNewProgram = TM_Rolling;
                             }
                             else
                             {
                                 CSAFE_Work_Exit();
                                 Timer_Counter_Clear(8);//solve when press program key twice 
                                 by_Status=M_RESET;  //reset current program can't reset issue
                             }
                         }    
                         else 
                         {
                             Timer_Counter_Clear(1);
                             by_WorkoutDFNewProgram=0;
                             if(by_KeyNumber == K_MANUAL)
                             {
                                 _Message_Status = Show_Message(Message_Manual,0,0,Show_Auto);
                                 Console_ProgramMode(Data_Set,TM_Manual);
                             }
                             else if(by_KeyNumber == K_FatBurn)
                             {
                                 _Message_Status = Show_Message(Message_FatBurn,0,0,Show_Auto);
                                 Console_ProgramMode(Data_Set,TM_FatBurn);
                             }
                             else if(by_KeyNumber == K_Rolling)
                             {
                                 _Message_Status = Show_Message(Message_RollingHills,0,0,Show_Auto);
                                 Console_ProgramMode(Data_Set,TM_Rolling);
                             }
                         }    
                         Led_Set_Target(Target_Program,0);
                         if(by_KeyNumber == K_MANUAL)
                             Led_Set_Target(Target_Manual,1);
                         else if(by_KeyNumber == K_FatBurn)
                             Led_Set_Target(Target_FatBurn,1);
                         else if(by_KeyNumber == K_Rolling)
                             Led_Set_Target(Target_RollingHills,1);
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                         _TargetWindowStatus = 1;
                     } 
                     bit.RF900Show = 0;
                     break;

            case K_Training:
                     if(!bit.CoolDown  && !bit.Stop && !bit.Pause && !bit.WorkEnd) 
                     {
                         bit.ChangProgram=1;
                         if(Console_ProgramMode(Data_Get,0) != TM_5K && \
                            Console_ProgramMode(Data_Get,0) != TM_10K )
                         {
                             if(by_WorkoutDFNewProgram != TM_5K &&\
                                by_WorkoutDFNewProgram != TM_10K)
                             {
                                 if(Console_ProgramMode(Data_Get,0) == TM_5K)
                                     by_WorkoutDFNewProgram=TM_10K;
                                 else 
                                     by_WorkoutDFNewProgram=TM_5K;
                             }
                             else
                             {
                                 CSAFE_Work_Exit();
                                 Timer_Counter_Clear(8);
                                 by_Status=M_RESET;
                             }
                         }    
                         else 
                         {
                             //bit.Choose = 1; 
                             Timer_Counter_Clear(1);
                             by_WorkoutDFNewProgram=0;
                             if(Console_ProgramMode(Data_Get,0) == TM_5K)
                             {
                                 _Message_Status = Show_Message(Message_10k,0,0,Show_Auto);
                                 Console_ProgramMode(Data_Set,TM_10K);
                             }
                             else
                             {
                                 _Message_Status = Show_Message(Message_5k,0,0,Show_Auto);
                                 Console_ProgramMode(Data_Set,TM_5K);
                             }
                         }
                         Led_Set_Target(Target_Program,0);
                         Led_Set_Target(Target_Training,1);
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                         _TargetWindowStatus = 1;
                     } 
                     bit.RF900Show = 0;
                     break;

            case K_TargetHR:
                     if(!bit.Stop && !bit.CoolDown  && !bit.Pause && !bit.WorkEnd)
                     {
//                         bit.ChangProgram=1;
                         if(Console_ProgramMode(Data_Get,0) != TM_TargetHR)
                         {
                             if(by_WorkoutDFNewProgram != TM_TargetHR)
                             {
                                 by_WorkoutDFNewProgram=TM_TargetHR;
                             }
                             else
                             {
                                 CSAFE_Work_Exit();
                                 Timer_Counter_Clear(8);
                                 by_Status=M_RESET;
                             }
                             bit.ChangProgram=1;
                         }   
                         Led_Set_Target(Target_Program,0);
                         Led_Set_Target(Target_TargetHR,1);
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                         _TargetWindowStatus = 1;
                     }  
                     bit.RF900Show = 0;
                     break;
            case K_FitnessTest:
                     if(!bit.Stop && !bit.CoolDown  && !bit.Pause && !bit.WorkEnd)
                     {
//                         bit.ChangProgram=1;
                         if(Console_ProgramMode(Data_Get,0) < TM_Gerkin)
                         {                             
                             if(by_WorkoutDFNewProgram != TM_Gerkin)
                             {
                                 by_WorkoutDFNewProgram=TM_Gerkin;
                             }
                             else
                             {
                                 CSAFE_Work_Exit();
                                 Timer_Counter_Clear(8);
                                 by_Status=M_RESET;
                             }
                             bit.ChangProgram=1;
                         }
                         Led_Set_Target(Target_Program,0);
                         Led_Set_Target(Target_FitnessTest,1);
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                         _TargetWindowStatus = 1;
                     }  
                     bit.RF900Show = 0;
                     break;  
                     
            case K_T1:
            case K_T2:
            case K_T3:
            case K_T4:
                     if(!bit.Stop && !bit.Pause && !bit.WorkEnd && by_NoHRTime == 0 )
                     {
                         _TargetWindow = by_KeyNumber-K_T1;
                         Led_Set_Target(TargetWindow[_TargetWindow],0);
                         TargetWindow[_TargetWindow]++;
                         if(TargetWindow[_TargetWindow] > ToggleDisplayInfo[_TargetWindow][2]) 
                         {
                             TargetWindow[_TargetWindow] = ToggleDisplayInfo[_TargetWindow][1]; 
                         }
                         _WindowScanBit &= ~ToggleDisplayInfo[_TargetWindow][0];
                         _TargetWindowStatus = 2;
                         if(by_KeyNumber == K_T1)
                         {
                             _ItemBuffer[0] = 0;
                         }
                         else if(by_KeyNumber == K_T2)
                         {
                             _ItemBuffer[2] = 0;
                             if(bit.NumberKey == 1)
                             {
                                 bit.NumberKey = 0;
                                 Timer_Close(0);//==>�رյ�0��ʱ���ۼ�
                                 Keyboard_NumberCount(3,K_BACK);
                             }
                         }
                     }
                     break;
            case K_T1|K_MASK:
            case K_T2|K_MASK:
            case K_T3|K_MASK:
            case K_T4|K_MASK:  
                     _TargetWindow = (by_KeyNumber-(K_T1|K_MASK));
                     if((_WindowScanBit &(ToggleDisplayInfo[_TargetWindow][0]))!=(ToggleDisplayInfo[_TargetWindow][0]))
                     {
                         _WindowScanBit |= ToggleDisplayInfo[_TargetWindow][0];
                         BEEP_Proce(4,2,2);
                         Led_Set_Target(TargetWindow[_TargetWindow],0);
                         TargetWindow[_TargetWindow] = ToggleDisplayInfo[_TargetWindow][1]; 
                         bit.ShowInformation = 1;
                         _DisplayWindowScan[_TargetWindow] = 1;
                     }
                     break;

        }
        // 5K RUN and 10K RUN program change use
        if(bit.Choose == 1)
        {
            bit.Choose = 0; 
            if(bit.ChangProgram == 1)
            {
                if(Console_ProgramMode(Data_Get,0) == TM_5K ||\
                   Console_ProgramMode(Data_Get,0) == TM_10K )
                {
                    Timer_Counter_Clear(1);
                    by_WorkoutDFNewProgram=0;
                    if(Console_ProgramMode(Data_Get,0) == TM_5K)
                     {
                        _Message_Status = Show_Message(Message_10k,0,0,Show_Auto);
                        Console_ProgramMode(Data_Set,TM_10K);
                     }
                    else
                     {
                        _Message_Status = Show_Message(Message_5k,0,0,Show_Auto);
                        Console_ProgramMode(Data_Set,TM_5K);
                    }
                }
            }
        }
        // �ۺ϶���ͳһִ��
        if(_TargetWindowStatus != 0)
        {
            if(_TargetWindowStatus == 1)
            {
                _DisplayWindowScan[0] = 0;// ȡ�������Զ��л�
                _DisplayWindowScan[1] = 0;// ȡ�������Զ��л�
                _DisplayWindowScan[2] = 0;// ȡ�������Զ��л�
                _DisplayWindowScan[3] = 0;// ȡ�������Զ��л�
                _WindowScanBit = 0;
            }
            else if(_TargetWindowStatus == 2)
            {
                bit.ShowInformation = 1;
                bit.RF900Show = 0;
                if(_ChangDFTime == 1)
                {
                    _ChangDFTime = 2;
                }
                // ��Cool down�ַ�����ʾ
                if(bit.ShowCoolDownStart == 1)
                {
                    bit.ShowCoolDownStart = 0;
                    Show_Message(Message_Blank,0,0,Show_Blank);
                }
            }
            _TargetWindowStatus = 0;
        }
        //
        if(_ChangDFTime == 2 ||(_ChangDFTime == 1 && Timer_Counter(T_STEP,4,50)))
        {// �������˶�ʱ���5�����;��Ϊȷ��
            _ChangDFTime = 0;
            if(Console_ProgramMode(Data_Get,0) == TM_TargetHR)
            {// ���������ӵ�Warm up
                Console_Information(Info_TIME,Data_Set,Console_NewTime(Data_Get) - (Console_Parameter(Info_AccumulatedTime)-120));
            }
            else
                Console_Information(Info_TIME,Data_Set,Console_NewTime(Data_Get) - Console_Parameter(Info_AccumulatedTime));
            if(Console_Information(Info_TIME,Data_Get,0) != 0)
            {
                Console_UserTime(Data_Set,Console_NewTime(Data_Get));
                Console_ChangeTimeCul();// ���¼��㵥��ʱ��
                Console_Set_StartAccumulated(1);//==>��ʱ���ۼ�start
                Led_ProfileMap(2);//==>ȡͼ�����
            } 
            else
            {
                Console_TimeOver(Data_Set,1);//==>�趨ʱ�����
            }
            _ItemBuffer[4]=10;
            bit.ShowInformation = 1;
        }
        else if(_ChangDFTime == 1)
        {
          Workout_Information(TargetWindow[2]);//show time
        }
        //
        if(bit.NumberKey && Timer_Counter(T_STEP,0,50))
        {//==>���趨������Լ5�����ûȷ���趨�Ļ����Զ��ָ�����
            bit.NumberKey = 0;
            Keyboard_NumberCount(3,K_BACK);
        }    
        if(Heart_Get_Heart() > 40)
        {// ��������ʱΪ����LED��˸��
            if(bit.ChangProgram == 0 && bit.ShowCoolDownStart == 0 &&
               bit.Stop == 0 && bit.Pause == 0 && bit.WorkEnd == 0 &&
               bit.RF900Show == 0  )
            {
                if(WindowDisplay[0] == Target_HRInfor)
                {
                    Workout_Information(TargetWindow[0]);
                }
                else if(bit.FirstHR == 0)
                {// First Heart Rate Number
                    bit.FirstHR = 1;
                    bit.ShowInformation = 1;
                    Led_Set_Target(TargetWindow[0],0);
                    TargetWindow[0] = Target_HRInfor;
                    _WindowScanBit &= ~BIT1;
                }
            }
            else if(WindowDisplay[0] == Target_HRInfor)
            {
                Led_Set_Target(TargetWindow[0],1);
            }
            
        }
        else bit.FirstHR = 0;
        //
        // RF900MHz control����ʾ��Ϣ
        if(bit.RF900Show == 0 && bit.RF900ShowStart == 1)
        {// ����������Ҫȡ����ʾʱ
            RF900_ClearMessageNumber();
            bit.RF900ShowStart = 0;
            Show_Message(Message_Blank,0,0,Show_Blank); 
            if(bit.NumberKey == 1)
            {// ���а���ʱ�ĵ�һ����ʾ����
                bit.NumberKey = 0;
                Keyboard_NumberCount(3,K_BACK);
            }
        } 
        if(RF900_ShowMessage() == 1)
        {// �ظ���Ҫ��ʾ��ʾ
            bit.RF900Show = 1;
            bit.RF900ShowStart = 1;
            // ��Cool down�ַ�����ʾ
            if(bit.ShowCoolDownStart == 1)
            {
                bit.ShowCoolDownStart = 0;
//                Show_Message(Message_Blank,0,0,Show_Blank);//solve when cool down display can't display volume
            }
            //
        }
        else if(bit.RF900Show && bit.RF900ShowStart)
        {// ������ʾ��ʾ
            bit.RF900Show = 0;
            bit.RF900ShowStart = 0;
            Show_Message(Message_Blank,0,0,Show_Blank); 
        }
        // 
        if(bit.ShowInformation == 1 && bit.RF900Show == 0 && \
          bit.ShowCoolDownStart == 0 && bit.ChangProgram == 0 && by_NoHRTime == 0 )// 
        {// ���˶���Ϣ
            bit.ShowInformation = 0;
            // ��Ϊ��һ�����˶���Ϣʱ��Ҫ����һ�δ��ڷ�Χ
            if(WindowDisplay[0] != TargetWindow[0])
            {
                WindowDisplay[0] = TargetWindow[0];
                Led_Window(Window_1,5,0);
            }
            if(WindowDisplay[1] != TargetWindow[1])
            {
                WindowDisplay[1] = TargetWindow[1];
                if(bit.NumberKey == 0)
                {// ��û��ʹ�����ּ������ٶ�ʱ
                  Led_Window(Window_2,5,0);
                }
            }
            if(WindowDisplay[2] != TargetWindow[2])
            {
                WindowDisplay[2] = TargetWindow[2];
//                Led_Window(Window_3,5,0);
            }
            if(WindowDisplay[3] != TargetWindow[3])
            {
                WindowDisplay[3] = TargetWindow[3];                
//                Led_Window(Window_4,5,0);              
            }
            // Window 1
            Workout_Information(TargetWindow[0]);
            if(_ItemBuffer[0] > 4)
            {// ����ʱ�䵽��ԭ
                _ItemBuffer[0] = 0;
                Led_Set_Target(TargetWindow[0],0);
                TargetWindow[0] = _ItemBuffer[1];
                bit.ShowInformation = 1;
            }
            // Window 4
//            Workout_Information(TargetWindow[3]);
            // Window 3
//            Workout_Information(TargetWindow[2]);
            if(_ItemBuffer[4] > 1)
            {// ����ʱ�䵽��ԭ
                _ItemBuffer[4] = 0;
                Led_Set_Target(TargetWindow[2],0);
                TargetWindow[2] = _ItemBuffer[5];
                bit.ShowInformation = 1;
            }
            // Window 2
            if(bit.NumberKey == 0)
            {// ��û��ʹ�����ּ������ٶ�ʱ
                Workout_Information(TargetWindow[1]);
                // ��������������������ʱ
                if(Console_DisplaySpeedChange(Data_Get,0) == 0)
                {
                    if(_ItemBuffer[2] > 5)
                    {// ����ٶ���ʾ��
                        _ItemBuffer[2] = 0;
                        Led_Set_Target(TargetWindow[1],0);
                        TargetWindow[1] = _ItemBuffer[3];
                        bit.ShowInformation = 1;
                    }
                }
                else
                {
                    _ItemBuffer[2] = 1;
                }
                //
            } 
            //            
        }
        //
        
        if(Check_SafeKey() == 0) 
        {
            CSAFE_Work_Exit();
            by_Status = M_RESET;
        }
        // 2014.11.24
        WIFISignalStrength();
        // ��������ۼ�
        Console_HJS(Data_Total,0);
        //
        UsbFunction_Protection();
        // start v2.2-5
        main_RSCUcheck();
        // end v2.2-5
    }
    // 2014.11.24
    ChangeWorkoutState(WIFI_WORKOUT_STOP);
    // 
    //Led_FanControlReset();//==>�رշ���
    return by_Status;
}
/*******************************************************************************
* Function Name  : Workout_LevelChange()
* Description    : �˶��ڼ������LEVEL����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 Workout_LevelChange(void)
{
  u8 by_Status = M_NONE;
  u8 by_Bu = Console_Information(Info_WorkoutSpeed,Data_Get,0)/10;
  u8 _Total;
  u16 by_K ;
  
  bit.key = 0;
  Keyboard_NumberCount(3,K_BACK);
  if(Console_SpeedUnit(Data_Get,0) == C_KM)
      Console_Information(Info_WorkoutSpeed,Data_Set,80);
  else
      Console_Information(Info_WorkoutSpeed,Data_Set,50); 
  Show_Message(Message_Level,0,0,Show_Auto);//==>Level
  Led_Window(Window_2,1,Console_Information(Info_LEVEL,Data_Get,0));
  Timer_Counter_Clear(0);
  
  while(by_Status == M_NONE)
  {
      Led_ProfileMap(1);//������
      by_K = Key_Proce();
      switch(by_K)
      {
          case K_PAUSE:
                   return M_SP1;
          case K_STOP:
                   return M_SP2;
          case K_SpeedU|K_MASK: 
          case K_SpeedU:    
          case K_SpeedD|K_MASK:
          case K_SpeedD: 
                   if(by_K == K_SpeedU||by_K == (K_SpeedU|K_MASK))
                       Console_Information(Info_LEVEL,Data_Add,0);
                   else
                       Console_Information(Info_LEVEL,Data_Dec,0);
                   Led_Window(Window_2,1,Console_Information(Info_LEVEL,Data_Get,0));
                   Timer_Counter_Clear(0);
                   Keyboard_NumberCount(0,by_K);
                   bit.key = 0;
                   Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
                   Led_ProfileMap(0);//==>ȡͼ�����
                   break;
          case K_0:
          case K_1:
          case K_2:
          case K_3:
          case K_4:
          case K_5:
          case K_6:
          case K_7:
          case K_8:
          case K_9:
                   // ���ּ������ۼ�ֵ
                   _Total = Keyboard_NumberCount(2,by_K);
                   // 
                   Led_Window(Window_2,1,_Total);    
                   Timer_Counter_Clear(0);
                   bit.key = 1;
                   break; 
          case K_ENTER:
                   if(bit.key == 0 || Console_Information(Info_LEVEL,Data_Set,_Total) == 1)
                   {
                       Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
                       Led_ProfileMap(0);//==>ȡͼ�����
                       by_Status = M_NEXT;
                       break;
                   }
          case K_BACK:  
                   bit.key = 0;
                   Led_Window(Window_2,1,Console_Information(Info_LEVEL,Data_Get,0));
                   Timer_Counter_Clear(0);
                   Keyboard_NumberCount(0,by_K);
                   break;
      }
      if(!Check_SafeKey()) by_Status = M_RESET;
      if(Timer_Counter(T_STEP,0,50)) by_Status = M_NEXT;
  }
  if(by_Status != M_RESET && by_Status != M_NEXT)
  {
      Console_Information(Info_WorkoutSpeed,Data_Set,by_Bu * 10);//20150825 change program speed retard to 0.5/0.8
  }
  Keyboard_NumberCount(0,by_K);
  return by_Status;
}
/*******************************************************************************
* Function Name  : Workout_Information()
* Description    : ��ʾ�˶���Ϣ
* Input          : ��ʾ��ʽ
* Output         : None
* Return         : None
*******************************************************************************/
void Workout_Information(u8 by_D)
{
  u16 _ClockTime = 0;
    
  SHT1381 time;//==>HT1381ʱ��IC Dataָ��
  
  switch(by_D)
  {
      case Target_HRInfor:
               Show_HeartRate();
               break;
      case Target_Incline:
               Show_Incline();
               break;
      case Target_METs:
               Show_METs();
               break;
      case Target_DistanceMiles:
               Show_Distance(C_MILE);
               break;
      case Target_DistanceKM:
               Show_Distance(C_KM);
               break;
      case Target_Pace:
               Show_Pace(Target_Pace);
               break;
      case Target_AveragePace:
               Show_Pace(Target_AveragePace);
               break;
      case Target_TimeElapsed:
               Show_Time(Time_UP);
               break;
      case Target_TimeRemaining:
               Show_Time(Time_DOWN);
               break;
      case Target_TotalTime:
               Show_Time(Time_Program);// need change
               break;
      case Target_Clock:
               ReadHT1381(&time);// ��ȡTIME 
               _ClockTime = (time.Hours / 16 * 10)+(time.Hours % 16);
               _ClockTime *= 100;
               _ClockTime += (time.Minutes / 16 * 10)+(time.Minutes % 16);
               Show_Clock(_ClockTime);
               break;
      case Target_Calories:
               Show_Calories();
               break;
      case Target_Speed:
               Show_Speed();
               break;
      case Target_CaloriesHour:
               Show_CaloriesHour();
               break;
      case Target_Watts:
               Show_Watts();
               break;
               
  }

}
/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 CSAFE_ID(void)
{
 u8 by_Status = M_NONE;
 u8 by_Message = 1;
 u8 ID_Seg = 0;
 u8 ID_Number = 0;
 unsigned short _Multiplier = 10000;
 u32 by_IDNumber = 0;
 
 bit.key = 0;
 bit.key1 = 0;
 bit.ShowChang = 0;
 Led_Set_Reset(); 
 Timer_Close(9);
 //CSAFE_NetPulse(0);
 
 Led_Set_Target(Display_OFF,0);
// LED_MATRIX(_BigMatrix_Clear);// ����
 Timer_Counter_Clear(0);
 while(by_Status==M_NONE)
 {
     if(!Check_SafeKey()) by_Status=M_RESET;
     // NetPulse
     if((CSAFE_GetInformation(_Get_Mode) == Csafe_Ready)||
        (CSAFE_GetNetPulse() == 1) ||
        (CSAFE_GetInformation(_Get_Mode) == Csafe_In_Use)) 
     {//==>Matthew�����Ҫ����������յ�HaveID״̬ʱ��������ģʽ 
         by_Status = M_PROGRAM;
     }
     else
     {
         //
         switch(by_Message)
         {
             case 1:Show_Message(Message_EnterUserID,0,0,Show_Auto);
                    if(Led_Get_String_End()) 
                    {//==>�ƶ�һ���ַ�����
                        //Led_String("      ID  :       ",0,0);
                        //Led_NumberSite(71,0);
                        //Led_NumberSite(77,0);
                        //Led_NumberSite(83,0);
                        //Led_NumberSite(89,0);
                        //Led_NumberSite(95,0);
                        //bit.key = 0;
                        bit.key1 = 1;
                        by_Message=2;
                    }
                    break;
             case 2:switch(ID_Number = Key_Proce())
                    {
                        case K_ENTER:
                               Led_Set_Reset(); 
                               CSAFE_Set_ID_Number(by_IDNumber); 
                               while(Led_Get_String_End() == 0)
                               {//==>�ƶ�һ���ַ�����
                                   Show_Message(Message_SubmittingID,0,0,Show_Auto);
                                   if(!Check_SafeKey()) break;
                               }
                               Timer_Counter_Clear(1);
                               while(by_Status==M_NONE)
                               {
                                   if(!Check_SafeKey()) break;
                                   
                                   if(CSAFE_GetInformation(_Get_Mode) == Csafe_In_Use)
                                   {
                                       Led_Set_Reset(); 
                                       while(Led_Get_String_End() == 0)
                                       {//==>�ƶ������ַ�����
                                           Show_Message(Message_IDAccepted,0,0,Show_Auto);
                                           if(!Check_SafeKey()) break;
                                       }
                                       by_Status=M_PROGRAM;
                                   }
                                   else if(CSAFE_GetInformation(_Get_Mode) == Csafe_IDle)
                                   {
                                       Led_Set_Reset(); 
                                       while(Led_Get_String_End() == 0)
                                       {//==>�ƶ������ַ�����
                                           Show_Message(Message_BadID,0,0,Show_Auto);
                                           if(!Check_SafeKey()) break;
                                       }
                                       //Led_String("      ID  :       ",0,0);
                                       //Led_NumberSite(71,0);
                                       //Led_NumberSite(77,0);
                                       //Led_NumberSite(83,0);
                                       //Led_NumberSite(89,0);
                                       //Led_NumberSite(95,0);
                                       //bit.key = 0;
                                       bit.key1 = 1;
                                       ID_Seg = 0;
                                       _Multiplier = 10000;
                                       by_IDNumber = 0;
                                       ID_Number = 0;
                                       break;
                                   }
                                   else if(CSAFE_GetInformation(_Get_Online) == 1 && Timer_Counter(T_STEP,1,100))
                                   {//==>������10���޻ش�Commandʱ,�ж�����δ����
                                       Led_Set_Reset(); 
                                       while(Led_Get_String_End() == 0)
                                       {//==>�ƶ������ַ�����
                                           Show_Message(Message_NetworkMiss,0,0,Show_Auto);
                                           if(!Check_SafeKey()) break;
                                       }
                                       CSAFE_SetInformation(_Set_Mode,Csafe_Offline);
                                       by_Status=M_PROGRAM;
                                   }
                                   else
                                   {
                                       Show_Message(Message_PleaseHold,0,0,Show_Auto);
                                   }
                                   
                               }
                               break;
                        case K_0:
                        case K_1:
                        case K_2:
                        case K_3:
                        case K_4:
                        case K_5:
                        case K_6:
                        case K_7:
                        case K_8:
                        case K_9:
                               bit.key = 1;
                               ID_Number -= K_0;
                               break; 
                        case K_BACK:
                               ID_Seg = 0;
                               _Multiplier = 10000;
                               by_IDNumber = 0;
                               ID_Number = 0;
                               //Led_String("      ID  :       ",0,0);
                               //Led_NumberSite(71,0);
                               //Led_NumberSite(77,0);
                               //Led_NumberSite(83,0);
                               //Led_NumberSite(89,0);
                               //Led_NumberSite(95,0);
                               //bit.key = 0;
                               bit.key1 = 1;
                               break;
                        case K_NONE://==>û�а���ʱ
                               break;
                        default://==>���˷�������ʱ
                               CSAFE_Set_AnyKey();
                               by_Status=M_PROGRAM;
                               break; 
                    }              
                    break;
         }
         if(bit.key1 == 1)
         {
             bit.key = 0;
             bit.key1 = 0;
             Led_String(" ID:      ",0,0);
             Led_NumberSite(32,0);
             Led_NumberSite(38,0);
             Led_NumberSite(44,0);
             Led_NumberSite(50,0);
             Led_NumberSite(56,0);
         }
         if(bit.key == 1)
         {
             bit.key = 0;
             if(ID_Seg < 5)
             {// ���ݻ�������ʾ
                 
                 Led_NumberSite(32+(ID_Seg * 6),ID_Number);
                 
                 by_IDNumber += (ID_Number * _Multiplier);
                 _Multiplier /= 10;
                 /*
                 switch(ID_Seg)
                 {
                     case 0:by_IDNumber = ID_Number * 10000;
                            break;
                     case 1:by_IDNumber += ID_Number * 1000;
                            break;
                     case 2:by_IDNumber += ID_Number * 100;
                            break;
                     case 3:by_IDNumber += ID_Number * 10;
                            break;
                     case 4:by_IDNumber += ID_Number;
                            break;
                 }
                 */
                 ID_Number = 0;
                 ID_Seg += 1;
             }
         }
         if(by_Message == 2 && Timer_Counter(T_LOOP,0,5) == 1 && ID_Seg < 5)
         {// ��һλ����˸����
             if(bit.ShowChang == 1)
             {
                 bit.ShowChang = 0;
                 Led_NumberSite(32+(ID_Seg * 6),127);
             }
             else
             {
                 bit.ShowChang = 1;
                 Led_NumberSite(32+(ID_Seg * 6),ID_Number);
             }
         }
     }
 }
 return(by_Status);
}

//=========================================================
//  5V, 12V, USB, ShutDown, ... ��������
//=========================================================
void Open_UCB_PSW(void)
{
  u16 i=0;   
    
  for(i=0;i < 65000;i++);  // Delay
  LED_5V_Set();  
  for(i = 0;i < 65000;i++);  // Delay   
   //ERP power should enable before WiFi module reset pin set 2015/4/14 Garry
  ERP_Power(GPIO_PIN_SET);
  IO_Set_OE();//solve when power on TLC59282 Display all on issue
  LED_12V_Set();
  for(i = 0;i < 65000;i++);  // Delay     
  IO_USB_SetILIM();
  for(i=0;i < 65000;i++);  // Delay  
  IO_WiFi_ResetHi();   
  IO_OpenAudio();//==>ShutDown  
  Led_Initial(); 
  // USB Control send HI
  IO_USB_SetCTL1();
  IO_USB_SetCTL2();
  IO_USB_SetCTL3();
  IO_USB_SetILIM();
  IO_USB_SetEN();
  //
}

//==============================================================================
//==>ָ��������³���
// 0 : ���������  1 : Language������  2:Logo
//==============================================================================
void Target_SoftwareUpdate(u8 by_M)// 2013.07.26
{
  u8 by_Status = M_NONE;
  u8 by_Target = 0;
  u8 by_LoopCheck = 0;
  u8 by_UpdateState = 0;
  
  bit.key = 0;
  Led_Set_Target(Display_OFF,0);
  Timer_Counter_Clear(0);
  bit.key1 = 0;
  UsbFunction_ClearUSBFirstCheck();
  if(UsbFunction_GetTotalBinFile() != 0)
  {
      UsbFunction_ClearBinInfo();
  }
  while(by_Status == M_NONE)
  {
      if(UsbFunction_USB_Status() == 1)
      {//==>���������
          if(UsbFunction_GetTotalBinFile() != 0)
          {
              if(!bit.key)
              { 
                  switch(by_M)
                  {
                     default:
                     case 1://==>�������
                     case 2://==>�������
                            Show_Message(Message_PleaseChooseUpdateName,0,0,Show_Auto);
                            break;
                     case 3://==>Logoͼ
                            Show_Message(Message_ChoosePatternName,0,0,Show_Auto);
                            break;
                  }
              }
              else
              {
                  Led_String((unsigned char*)UsbFunction_BinFileNameMessage(by_Target),0,Show_Auto);
              }
          }
          else
          {
              switch(by_M)
              {
                 default:
                 case 1://==>�������
                        Show_Message(Message_NotHaveHandleFile,0,0,Show_Auto);
                        break;
                 case 2://==>�������
                 case 3://==>Logoͼ  
                        Show_Message(Message_NoFileForUpdate,0,0,Show_Auto);
                        break;
              }
          }
      }
      else
      {
          bit.key = 0;
          Show_Message(Message_NotUSB,0,0,Show_Auto);
          if(UsbFunction_GetTotalBinFile() != 0)
          {
              UsbFunction_ClearBinInfo();
          }
      }
      UsbFunction_USB(0x500);
      switch(by_M)
      {
         default:
         case 1://==>�����������.bin��Ѱģʽ
                UsbFunction_SoftwareUpdate(0);
                break;
         case 2://==>�����������.bin��Ѱģʽ
                UsbFunction_SoftwareUpdate(2); 
                break;
         case 3://==>����Logoͼ����.bin��Ѱģʽ
                UsbFunction_SoftwareUpdate(3);
                break;
      }
      bit.key2 = 0;
      switch(Key_Proce())
      {        
               /*
          case K_GO:
               if(by_M == 3)
               {// ��������Logoͼ��
                   bit.key = 1;
                   bit.key1 = 1;
                   by_Target = 1;
               }
               else
                   break;
               */
          case K_ENTER:  
               if(!bit.key)
               {
                   bit.key = 1;
                   by_Target = 1;
               }
               else
               {
                   if(UsbFunction_GetTotalBinFile() != 0)
                   {
                       switch(by_M)
                       {
                          default:
                          case 1://==>�������
                                 UsbFunction_AssignSoftwareUpdate(by_Target);
                                 break;
                          case 2://==>���¶������
                          case 3://==>����Logoͼ  
                                 while(by_LoopCheck < 3)
                                 {// 3���������ѭ��
                                     Led_Set_Reset();
                                     while(Led_Get_String_End() == 0)
                                     {
                                         if(by_M == 2)
                                              Show_Message(Message_LoadingLanguages,0,0,Show_Auto);
                                          else
                                              Show_Message(Message_LoadingPattern,0,0,Show_Auto);
                                     }
                                     if(by_M == 2)
                                         by_UpdateState = Lang_WriteLanguageToEEPRom(_Lang_UcbMachineType_,UsbFunction_BinFileAddress(by_M,by_Target)) ;
                                     else
                                         by_UpdateState = Pattern_WritePatternToEEPRom(UsbFunction_BinFileAddress(by_M,by_Target));
                                     if(by_UpdateState == _Lang_Success_)
                                     {
                                         by_LoopCheck = 99;// �뿪����ģʽ
                                         if(by_M == 3)
                                         {
                                             if(bit.key1 == 1)
                                             {// ��Ҫ��������ͼ��ʱ
                                                 by_Target += 1;
                                                 if(by_Target > UsbFunction_GetTotalBinFile())
                                                     by_Target -= 1;
                                                 else 
                                                     by_LoopCheck = 0;
                                             }
                                             if(by_LoopCheck == 99)
                                             {
                                                 Pattern_CheckingPattern(by_Target);
                                                 EEPROM_SaveWorkInformation(EE_Logo,Pattern_GetPatternAmount());
                                             }
                                         }
                                     }
                                     else if(by_UpdateState == _Lang_Machine_Error_)
                                     {
                                         Show_Message(Message_Blank,0,0,Show_Blank);
                                         while(1)
                                         {
                                             Show_Message(Message_ThisFileCanNotBeUsedInThisConsole,0,0,Show_Auto);
                                             if(Led_Get_String_End()) break;
                                             if(!Check_SafeKey()) break;
                                         }
                                         by_LoopCheck = 3;
                                     }
                                     else
                                     {
                                         by_LoopCheck++;// ����������һ��
                                     }
                                 }
                                 // ���Ը���3�ζ�ʧ��
                                 by_Status = M_RESET;
                                 break;
                       }
                       if(by_LoopCheck >= 99)
                       {
                           if(by_M == 2)
                           {
                               Language_EEPROMCheck(_Lang_UcbMachineType_);
                           }
                       }
                       Led_Set_Reset();
                       while(Led_Get_String_End() == 0)
                       {
                           if(by_LoopCheck >= 99)
                               Show_Message(Message_UpdateSuccessful,0,0,Show_RightToLeft);
                           else
                               Show_Message(Message_UpdatedFailed,0,0,Show_RightToLeft);
                       }
                   }
               }
               break;
          case K_SpeedU:
          case K_InclineU:  
          case K_TimeU:
               bit.key2 = 1;
          case K_SpeedD:
          case K_InclineD:  
          case K_TimeD:     
               if(UsbFunction_GetTotalBinFile() != 0)
               {
                   bit.key = 1;
                   Led_Set_Reset();
                   if(bit.key2 == 1)
                   {
                       by_Target += 1;
                       if(by_Target > UsbFunction_GetTotalBinFile())
                       {
                           by_Target = 1;
                       }
                   }
                   else
                   {
                       if(by_Target <= 1)
                       {
                           by_Target = UsbFunction_GetTotalBinFile();
                       }
                       else by_Target -= 1;
                   }
               }
               break;
          case K_BACK:
               by_Status=M_RESET;
               break; 
          case K_STOP|K_MASK:
               by_Status=M_RESET;
               /*
               if(by_M == 2)
               {
                   Led_String("  Language Erase  ",0,0);
                   Timer_Counter_Clear(0);
                   while(!Timer_Counter(T_STEP,0,20)) ;
                   Language_Erase_AllData();
                   Language_LanguageChecking(L_English);
                   EEPROM_SaveWorkInformation(EE_LANGUAGE,L_English);
               }
               else if(by_M == 3)
               {
                   Led_String("  Pattern Erase   ",0,0);
                   Timer_Counter_Clear(0);
                   while(!Timer_Counter(T_STEP,0,20)) ;
                   Pattern_Erase_AllData();
                   Pattern_CheckingPattern(Logo_MATRIX);
                   EEPROM_SaveWorkInformation(EE_Logo,Logo_MATRIX);
                   LED_MATRIX(_BigMatrix_Clear);// ���Logo��
               }
               */
               break; 
               
      }
      if(!Check_SafeKey()) by_Status=M_RESET;
  }
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void RCC_Configuration(void)
//{
//  ErrorStatus HSEStartUpStatus;
///* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
//  /* RCC system reset(for debug purpose) */
//  RCC_DeInit();
//
//  /* Enable HSE */
//  RCC_HSEConfig(RCC_HSE_ON);
//
//  /* Wait till HSE is ready */
//  HSEStartUpStatus = RCC_WaitForHSEStartUp();
//
//  if (HSEStartUpStatus == SUCCESS)
//  {
//    /* Enable Prefetch Buffer */
//    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//
//    /* Flash 2 wait state */
//    FLASH_SetLatency(FLASH_Latency_2);
// 
//    /* HCLK = SYSCLK */
//    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
//  
//    /* PCLK2 = HCLK */
//    RCC_PCLK2Config(RCC_HCLK_Div1); 
//
//    /* PCLK1 = HCLK */
//    RCC_PCLK1Config(RCC_HCLK_Div4);
//
//#ifdef STM32F10X_CL
//    /* Configure PLLs *********************************************************/
//    /* PLL2 configuration: PLL2CLK = (HSE / 2) * 10 = 40 MHz */
//    RCC_PREDIV2Config(RCC_PREDIV2_Div2);
//    RCC_PLL2Config(RCC_PLL2Mul_10);
//
//    /* Enable PLL2 */
//    RCC_PLL2Cmd(ENABLE);
//
//    /* Wait till PLL2 is ready */
//    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
//    {}
//
//    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
//    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
//    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
//#else
//    /* PLLCLK = 8MHz * 9 = 72 MHz */
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
//#endif
//
//    /* Enable PLL */ 
//    RCC_PLLCmd(ENABLE);
//
//    /* Wait till PLL is ready */
//    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
//    {
//    }
//
//    /* Select PLL as system clock source */
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//
//    /* Wait till PLL is used as system clock source */
//    while(RCC_GetSYSCLKSource() != 0x08)
//    {
//    }
//  }
//  else
//  { /* If HSE fails to start-up, the application will have wrong clock configuration.
//       User can add here some code to deal with this error */    
//
//    /* Go to infinite loop */
//    while (1)
//    {
//    }
//  }
        /* TIM2 clock enable */
        //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        /* TIM3 clock enable */
        //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void NVIC_Configuration(void)
//{
//        NVIC_InitTypeDef 		NVIC_InitStructure;
//	//#ifdef  VECT_TAB_RAM
//	/* Set the Vector Table base location at 0x20000000 */
//	//NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
//	//#else  /* VECT_TAB_FLASH  */
//	/* Set the Vector Table base location at 0x08000000 */
//	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xD000);
//	//#endif
//        
//	/* Configure one bit for preemption priority */
////	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//            
//        /* Enable the TIM2 global Interrupt */
//        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init(&NVIC_InitStructure);
//        
//        /* Enable the EXTI9_5 Interrupt */
//        
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);		    
        
        /* Enable ADC IRQChannel */
        /*
        NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        */
//}

//******************************************************************************
//==> ErrorCodeDisplay_Mode()
//==> ��������ʾģʽ
//==> 0:һ��ģʽ  1:�˶�ģʽ
//******************************************************************************
char ErrorCodeDisplay_Mode(char by_D)
{
  char by_S = M_NONE;
  u16 by_KeyInput = 0;
  
    // ������������ʾ
  if(EEPROM_ReadInformation(EE_HJS_total) >= \
     EEPROM_ReadInformation(EE_HJS_CheckNumber))
  {
      if(bit.HJS_ErrorCheckDisplay == 0)
      {
          bit.HJS_ErrorCheckDisplay=1;
      Digtial_Set_ErrorCode(0x0304);
      Digital_ErrorCode_ChangToAddress();
      }
      if(EEPROM_ReadInformation(EE_HJS_Switch) == D_Enable)
      {
          Show_Message(Message_Blank,0,0,Show_Blank);
          Led_Set_Reset();
          while(Led_Get_String_End() == 0)
          {
              Show_Message(Message_ReplaceEarphoneBoard,0,0,Show_Auto);
          }
      }       
  }
  if(by_D == 0)
  {// Home Screen һ��ģʽ
  // Keypad Bad Notification 
  if(EEPROM_ReadInformation(EE_KeyNotifiction) == D_Enable)
  {
      unsigned char i;
     
      // Have bad key
      if(KeyScan_GetBypaddKeyNumber() != 0)
      {
              if(bit.KEY_ErrorCheckDisplay == 0)
              {
                  bit.KEY_ErrorCheckDisplay=1;
          Digtial_Set_ErrorCode(0x0306);
          Digital_ErrorCode_ChangToAddress();
      }
          }
          else
              bit.KEY_ErrorCheckDisplay=0;
          // have up/lower/quick block need to check
      for(i=Keypad_Upper;i<=Keypad_Quick;i++)
      {
          if(KeyScan_BadKeyRegion(i) == 1)
          {
              Led_Set_Reset();
              while(Led_Get_String_End() == 0)
              {
                  Show_Message(Message_ReplaceUpToggleOverlay+(i-1),0,0,Show_Auto);
              }
          }
      }
  }
  }
  if(Digtial_Get_ErrorCode() != 0)
  {
      bit.ErrorCode = 1;
      // 2014.11.24
      if(bit.ErpMode == 1) return by_S;
      //
      if(Digital_CheckClassCError() == 1) 
      {//==>Class C�ȼ��Ĵ��������ʱ
          WiFi_OccurredErrorCode(Digtial_Get_ErrorCode());// AM system 
          Show_Message(Message_Blank,0,0,Show_Blank);
          Led_ClearProfileMap();
          Led_String("Error:    ",0,0);
          Led_Special(2,40,Digital_Get_DisplayError());
          Digtial_Set_ErrorCode(0);//==>����������ݴ�
          Digital_ErrorCodeUpdate();
          EEPROM_SaveError();//==>�ٽ����������
          bit.ErrorCode = 0;
          Digital_Set_DigitalSpeedTarget(0);//==>�ٶȹ���
          Digital_Command(CmdSetInclineAction,0);//==>����ֹͣ
          // 2014.11.24
          ChangeWorkoutState(WIFI_WORKOUT_STOP);
          //
          while(by_S == M_NONE)
          {
             by_KeyInput = Key_Proce();
             by_S =  EngineerMode_SpecialMode(by_KeyInput);
          }
      }
      else
      {
          if(by_D == 0)
          {
              if(EEPROM_ReadInformation(EE_ErrorCode) == D_Enable)
              {//==>������Class C���´�������ʾ
                  Show_Message(Message_Blank,0,0,Show_Blank);
                  Led_String("Error:    ",0,0);
                  Led_Special(2,40,Digital_Get_DisplayError());
                  Timer_Counter_Clear(0);
                  while(!Timer_Counter(T_STEP,0,30)) 
                  {
                      by_KeyInput = Key_Proce();
                      by_S =  EngineerMode_SpecialMode(by_KeyInput);
                  }
                  Show_Message(Message_Blank,0,0,Show_Blank);
              }
          }
          Digtial_Set_ErrorCode(0);//==>����������ݴ�
      }
  }
  return by_S; 
}
                                         
/*******************************************************************************
* Function Name  : Message_TestMode
* Input          : None
* Output         : None
*******************************************************************************/                                         

void Message_TestMode(void)
{
  u8 by_Status = M_NONE;
  u8 by_La = 0;
  u8 by_Me = 1;
  
  Led_MessageSpeed(120);// ������ԭ�ٶ�
  while(by_Status == M_NONE)
  {
      Console_Language(Data_Set,by_La);
      Show_Message(by_Me,0,0,Show_Auto);
      switch(Key_Proce())
      {
          case K_STOP:  
                   by_Status=M_RESET;
                   break;
          case K_SpeedU:
                   by_Me++; 
                   if(by_Me > Message_TotalMessage)
                   {
                       by_Me = 1;
                   }
                   Led_Set_Reset();
                   Show_Message(Message_Blank,0,0,Show_Blank);
                   break;
          case K_SpeedD:
                   if(by_Me > 1)
                       by_Me--;
                   else
                       by_Me = Message_TotalMessage;
                   Led_Set_Reset();
                   Show_Message(Message_Blank,0,0,Show_Blank);
                   break;         
          case K_InclineU:
                   by_La++;
                   if(by_La > Language_GetLanguageAmount()) by_La = 0; 
                   if(Language_LanguageChecking(by_La) != _Lang_Success_) 
                   {
                       by_La = 1;
                   }
                   by_Me = 1;
                   Led_Set_Reset();
                   Show_Message(Message_Blank,0,0,Show_Blank);
                   break;
          case K_InclineD:
                   if(by_La > 0) 
                       by_La--;
                   else
                       by_La = Language_GetLanguageAmount();
                   if(Language_LanguageChecking(by_La) != _Lang_Success_) 
                   {
                       by_La = 1;
                   }
                   by_Me = 1;
                   Led_Set_Reset();
                   Show_Message(Message_Blank,0,0,Show_Blank);
                   break;
      }
      if(!Check_SafeKey()) by_Status=M_RESET;
  }
  Led_MessageSpeed(39);// ��ԭ�ٶ�
}    
/*******************************************************************************
* Function Name  : BeltStop_StatusCheck
*                  ����ʾ�������ж��Ƿ�����˶�ģʽ
* Input          : None
* Output         : �Ƿ���� Workout
*******************************************************************************/                                         
u8 BeltStop_StatusCheck(void)
{
  u8 by_Status = M_NONE;
  u8 _Out = 0;
  u8 _Count = 10;
  u8 _Mode = 0;
  
  
  Led_Set_Reset();
  while(by_Status == M_NONE)
  {
      Heart_Decoder();
      if(_Mode == 0)
      {
          Show_Message(Message_BeltWillBeStoppedIn10Second,0,0,Show_Auto);
          if(Led_Get_String_End() == 1) 
          {
              Show_Message(Message_Blank,0,0,Show_Auto);//Message_NoUserDetected
              Led_NumberSite(49,1);
              Led_NumberSite(55,0);
              Timer_Counter_Clear(0);
              _Mode = 2;
          }
      }
      else
      {
          if(Timer_Counter(T_STEP,0,10))
          {
              if(_Count == 10)
              {
                  Show_Message(Message_Blank,0,0,Show_Auto);
              }
              _Count -= 1;
              Led_NumberSite(52,_Count);
              Timer_Counter_Clear(0);
              if(_Count == 0)
              {
                  _Out = 1;
                  by_Status = M_NEXT;
              }
          }
      }
      switch(Key_Proce())
      {
          case K_STOP: 
                      _Out = 2;
                      by_Status = M_NEXT;
                      break;
          case K_PAUSE: 
                      _Out = 3;
                      by_Status = M_NEXT;
                      break;            
          case K_NONE:      
                      break;
          default:
                      by_Status = M_NEXT;
                      break;            
      } 
      if(Check_SafeKey() == 0)
      {
          _Out = 1;
          by_Status = M_NEXT;
      }   
      if((Digital_CeckNonInUser == 0)||(Heart_Get_Heart() > 40)) by_Status = M_NEXT;//
  }
  by_InUserModeMotoSafeTime = 0;
  by_NonInUserTime = 0;
  Digital_CeckNonInUser = 0;
  Timer_Counter_Clear(0);
  Show_Message(Message_Blank,0,0,Show_Blank);
  return _Out;
}
/*******************************************************************************
* Function Name  : Main_EnterLogin
* Description    : ʹ���ߵ�¼
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
u8 Main_EnterLogin(void)
{
  u8 by_Status = M_NONE;
  unsigned short by_Key = 0;
  char Xid[14];
  char PassCode[4];
  u8 _DisplayString = 0;
  u8 _xIDWord = 1;
  u8 i;
  u8 by_ErPState = 0;
  
  Led_Set_Target(Display_OFF,0);
  Led_Set_Reset();
  Led_7SEG(4,0);
  Show_Message(Message_Blank,0,0,Show_Blank);
  for(i=0;i<14;i++)
  {
      Xid[i] = 0;
      if(i < 4) PassCode[i] = 0;
  }
  bit.THR_Safe = 0;
  bit.Enter=0;
  while(by_Status == M_NONE)
  {
      if(bit.ErpMode == 0)
      {
          switch(_DisplayString)
          {
              case 0:
                     Heart_Decoder();//==>Heart Rate����
                     /*  
                     if(HR_Signal >= 1 && HR_Signal < 3)
                     {// have Heart Rate signal
                         Show_Message(Message_DetectingHR,0,0,Show_Auto);
                         Led_7SEG(1,Heart_Get_Heart());
                         if(Heart_Get_Heart() > 40)
                         {
                             HR_Signal = 2;
                             Timer_Counter_Clear(4);
                         }
                         // HR < 40 or 15`s not heart rate will return PROGRAM display
                         if((HR_Signal == 2 && Heart_Get_Heart() < 40) ||
                            (Timer_Counter(T_STEP,4,150)) )
                         {
                             Timer_Counter_Clear(4);
                             HR_Signal = 0;
                             Led_7SEG(4,0);
                         }
                     }
                     else
                     {
                         if(HR_Signal == 0 && Heart_Get_HRSignal())
                         {//==>����HRѶ�Ų���ʱ
                             HR_Signal = 1;
                             Led_Set_Reset();
                             Show_Message(Message_DetectingHR,0,0,Show_Auto);
                             Timer_Counter_Clear(4);
                         }
                     }
                     */
                     if(HeartRate_HomeMode()==1)
                     {
                         Show_Message(Message_LonginXID,0,0,Show_Auto);
                     }
                     break;
              case 1:
                     Led_xIDNumber(0,_xIDWord-1,Xid);
                     break;
              case 2:
                     Show_Message(Message_Enter4DigitsPin,0,0,Show_Auto);
                     break;
              case 3: 
                     Led_xIDNumber(1,_xIDWord-1,PassCode);
                     break;
              case 4:
                     Show_Message(Message_Login,0,0,Show_Auto);
                     if( CheckLoginResult() == 1)
                     {
                         userFlashConfig[0]->LoggedOn = 1;
                         // start v2.1-7
                         Timer_Counter_Clear(0);
                         while(Timer_Counter(T_STEP,0,30)==0)
                         {
                             Show_Message(Message_Welcome,0,0,Show_Auto);
                         }
                         // end
                         by_Status = M_NEXT;
                     }
                     else
                     {
                         if(CheckLoginResult() == 2 || Timer_Counter(T_STEP,0,300))
                         {
                             _DisplayString = 5;
                             Led_Set_Reset();
                             Led_7SEG(4,0);
                         }
                     }
                     break;
              case 5:// start v2.2-6
                     if(WIFISignalStrength() == 0)
                     {
                         break;
                     }
                     // end v2.2-6
              case 6:  
                         Show_Message(Message_PinIsWrong,0,0,Show_Auto);
                     if(Led_Get_String_End() == 1)
                     {
                         _DisplayString = 0;
                         _xIDWord = 1;
                         for(i=0;i<14;i++)
                         {
                             Xid[i] = 0;
                             if(i<4) PassCode[i] = 0;
                         }
                     }
                     break;
          }
      }
      by_Key  = Key_Proce();
      if(by_Key != K_NONE ||(_DisplayString == 0 || _DisplayString >= 4))
      {
          Timer_Counter_Clear(1);
      }
      // δ�������뿪Login mode
      if(WIFISignalStrength() == 0 ||\
         CSAFE_GetInformation(_Get_Mode) == Csafe_IDle)
      {
          // start v2.2-6
          if(WIFISignalStrength() == 0 && _DisplayString == 5)
          {
              Led_Set_Reset();
              while(Led_Get_String_End() == 0)
              {// show twice
                  Show_Message(Message_NoInternetconnection,0,0,Show_Auto);
              }
          }
          // end v2.2-6
          by_Status = M_NEXT;
      }
          if(Console_SleepTime(Data_Get,0) != 0)
          {// Sleep Time �趨ʱ��Ϊ������ʱ
              by_ErPState = LCB_SleepMode(1,by_Key,M_NONE,Console_SleepTime(Data_Get,0)*60); 
              // 1. Sleep Time : �뵽Engineering Mode�����趨 
              //    Ĭ��ֵOFF����Χ 0~30����,Sleep Time ��λΪ 0.1��, ����Ϊ0ʱ
              //    ��ʾ�ر�EUPs.
              // 2. ��Ϊ���ϵͳʱ�Ż�����EUPs����
              bit.ErpMode = 0;
              if(by_ErPState == 2)                                                                                              
              {
                  by_Status = M_RESET;
              }
              else if(by_ErPState == 1)
              {
                  bit.ErpMode = 1;
              }  
          }
      if(EngineerMode_SpecialMode(by_Key) == M_RESET) return M_RESET; 
      
      switch(by_Key)
      {
          case K_STOP | K_MASK:
                     by_Status=M_RESET;
                     break;        
          case K_ENTER:
                  switch(_DisplayString)
                  {
                      case 1:
                             if(_xIDWord >= 5 && _xIDWord <= 15)
                                 _DisplayString = 2;
                             else
                                 _DisplayString = 6;
                             _xIDWord = 1;
                             break;
                      case 3:
                             if(_xIDWord >= 5)
                             {
                                 _DisplayString = 4;
                                 Led_7SEG(4,0);
                                 // Input xID
                                 WiFi_memcpy(userFlashConfig[0]->xid,Xid,14);
                                 // Input Pass Code
                                 WiFi_memcpy(userFlashConfig[0]->passcode,PassCode,4);
                                 Login( 0 , Xid, PassCode);// 1
                                 //Login(1,"998877112233","1234");
                                 Timer_Counter_Clear(0);
                             }
                             else
                             {
                                 _DisplayString = 2; 
                                 _xIDWord=1;
                                 for(i=0;i<4;i++)
                                 {
                                     PassCode[i] = 0;
                                 }
                             }
                             break;
                  }
                  Led_Set_Reset();
                  break;
          case K_BACK:    
                  if(_xIDWord != 1) _xIDWord--;
                  if(_DisplayString == 1)
                      Xid[_xIDWord-1] = 0;
                  else if(_DisplayString == 3)
                      PassCode[_xIDWord-1] = 0;
                  break;
          default:       
                  if(by_Key >= K_0 && by_Key <= K_9)
                  {
                      switch(_DisplayString)
                      {
                          case 0:
                                 Led_String("          ",0,0);
                                 _DisplayString = 1;
                                 WiFi_Interface_ClearUserExternalID();
                          case 1:       
                                 if(_xIDWord < 15)
                                 {
                                     Xid[_xIDWord - 1] = (by_Key - K_0) + '0';  
                                     Led_xIDNumber(0,_xIDWord,Xid);
                                     _xIDWord++;
                                 }
                                 break;
                          case 2:
                                 Led_String("          ",0,0);
                                 _DisplayString = 3;       
                          case 3:
                                 if(_xIDWord < 5)
                                 {
                                     PassCode[_xIDWord - 1] = (by_Key - K_0) + '0';  
                                     Led_xIDNumber(1,_xIDWord,PassCode);
                                     _xIDWord++;
                                 }
                                 break;
                      } 
                  }
                  break;
          case K_MANUAL:
          case K_Rolling:
          case K_Training:
          case K_FatBurn:
          case K_TargetHR:
          case K_FitnessTest:
                  WiFi_Interface_ClearUserExternalID();
                  by_Status = by_Key;//M_NEXT;
                  break;
          case K_GO:
                  if(Digital_Get_RPMStatus() == 0)
                  {
                  WiFi_Interface_ClearUserExternalID();
                  by_Status = M_START;
                  }
                  break;
                  /*
          case K_CoolDown:
                  
                  // ������,����wifi would��ΪAPģʽ
                  RestoreWifiToFactoryDefaults();
                  Timer_Counter_Clear(1);
                  while(Timer_Counter(T_STEP,1,30) == 0)
                  {
                      //Led_String("Activated");
                      Show_Message(Message_Activated,0,0,Show_Auto);
                  }
                  
                  //2014.10.21 ������
                  // wifi module ��������Low����300ms������Hi
                  IO_WiFi_ResetLow();
                  Timer_Counter_Clear(1);
                  while(Timer_Counter(T_STEP,1,5) == 0);
                  IO_WiFi_ResetHi();// WiFi Reset Pin  
                  //
                  //by_Status = M_RESET;
                  
                  break;
                  */
        
      }
      if(Timer_Counter(T_STEP,1,300))
      {
          _DisplayString = 0;
          _xIDWord = 1;
          for(i = 0;i < 14;i++) Xid[i] = 0;// DAPIλ��û���� 0
      }
      // ��������ۼ�
      Console_HJS(Data_Total,0);
      // start v2.1-13
      main_RSCUcheck();
      // end v2.1-13
      // WifiѶ��ǿ��
//      Led_Set_Target(Target_WiFiSignal,WIFISignalStrength());
      //
      if(ErrorCodeDisplay_Mode(0) == M_RESET)
      {//==>һ��ģʽ 
          by_Status = M_RESET; 
      }
      if(Check_SafeKey() == 0) by_Status = M_RESET;
  }
  Led_7SEG(4,0);
  Led_Set_Reset();
//  LED_MATRIX(_BigMatrix_Clear);// ���������ʼ������
  Show_Message(Message_Blank,0,0,Show_Blank);
  return by_Status;      
}
/*******************************************************************************
* Function Name  : HeartRate_HomeMode
* Description    : ����������ʾ�ӳ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char HeartRate_HomeMode(void)
{
  if(bit.THR_Safe) 
  {
     LCB_SleepMode_Initial();// Sinkyo..EUPs
     Show_HeartRate();
     if(Heart_Get_Heart() < 40) 
     {
         bit.THR_Safe = 0;
         bit.Enter = 0;
         Led_Set_Target(Target_HRInfor,0);
         Show_Message(Message_Blank,0,0,Show_Blank);//==>�������
     }
  }    
  else 
  {
     if(Heart_GetBlink())
     {
         bit.Enter = 1;
         bit.HR_Blink = 0;
     }
     else
     {
         if(_Message_Status == _Message_Keep)
         {
             if(bit.Enter)
             {
                 bit.HR_Blink = 1;
                 Show_Message(Message_Blank,0,0,Show_Blank);//==>�������
             }
             if(Timer_Counter(T_STEP,0,20)) 
              {
                 bit.Enter=0; 
                 //Show_HeartRate();
             }
         }
     }
     if(bit.Enter && !bit.HR_Blink)
     {
         _Message_Status = Show_Message(Message_DetectingHR,0,0,Show_Auto);
         if(_Message_Status == _Message_Keep)
         {
             Timer_Counter_Clear(0);
         }
         else if(_Message_Status == _Message_Loop)
         {// ���ַ���
             if(Led_Get_String_End())
             {
                 Show_Message(Message_Blank,0,0,Show_Blank);//==>�������
                 bit.Enter=0; 
                 Show_HeartRate();
             }
         }
     }
     if(Heart_Get_Heart() > 40)
     {
         bit.THR_Safe=1;
         Show_Message(Message_Blank,0,0,Show_Blank);//==>�������
     }
  } 
  if(!bit.Enter && !bit.THR_Safe)
      return 1;
  else 
      return 0;
}

// start v2.2-5
void main_RSCUcheck(void)
{
  unsigned char by_Bu=0;
  unsigned char by_UpdateCountDn = 0;

  if(WIFISignalStrength() == 0) 
  {// WiFi not connect  
      return;
  }
  SHT1381_RealTime time;//==>HT1381ʱ��IC Dataָ��
  if(EEPROM_ReadInformation(EE_AutoUpdateSwitch) == D_Enable)
  {
      if(bit.CanUpdateSoftware == 1)
      {// �����������,��ʱ����׼�����µ�״̬
          HT1381_RealTime(&time);
          by_Bu=time.Hour;  
          if(by_Bu == EEPROM_ReadInformation(EE_AutoUpdateTime) && \
             bit.CanUpdateSoftwareStare == 0 ) 
          {// �Զ�����ʱ�䵽��
              bit.CanUpdateSoftwareStare=1;
          }
          if(bit.CanUpdateSoftwareStare == 1 && \
             Console_AutoSoftwareUpdateStatus(0) == 0)
          {// �ڷ��˶�ģʽ��׼����ʼ���и���
              by_UpdateCountDn = 30;
              Timer_Counter_Clear(_Time_RSCUCheck);
//              LED_MATRIX(_BigMatrix_Clear,0);// ����
              Led_Set_Target(Display_OFF,0);// Led All Off
              //
              while(1)
              {// ׼������ǰ30����ʾ
                  if(Timer_Counter(T_LOOP,_Time_RSCUCheck,10))
                  {
                      by_UpdateCountDn -= 1;
                  }
                  Show_Message( Message_SoftwareUpdateIn, 0, 0,Show_Auto); 
                  Led_7SEG(1, by_UpdateCountDn);
                  if(by_UpdateCountDn == 0)
                  {// ��ʼ����
                      EngineerMode_RSCUpdate(0);
                      break;
                  }
                  if(Key_Proce() != K_NONE)
                  {// �������ȡ�����ڸ���
                    Led_7SEG(4,0);
                    WiFi_RSCU_Process(_RSCU_Reset);
                    break;
                  }
              }
              bit.AutoCheckUpdate=0; 
              bit.CanUpdateSoftware=0;
              bit.CanUpdateSoftwareStare=0;
              Timer_Counter_Clear(_Time_RSCUCheck);
//              LED_MATRIX(_BigMatrix_Clear,0);// ����
          }
      }
      else if(bit.AutoCheckUpdate == 1)
      {// ������ر���
          by_Bu=WiFi_RSCU_Process(_RSCU_Update);
          switch(by_Bu)
          {
              case _RSCU_CanUpdate:
                  bit.CanUpdateSoftware = 1;
              case _RSCU_NoLatestSoftware:    
                  bit.AutoCheckUpdate = 0;
                  break;
              case _RSCU_CanNotUpdate:
              case _RSCU_BinCheckError:
              case _RSCU_EEPRomError:
                  bit.AutoCheckUpdate = 0;
                  bit.CanUpdateSoftware=0;
                  bit.CanUpdateSoftwareStare=0;
                  break;
          }
      }
      else if(WIFICanLoginUser() != 0)
      {
          if(Timer_Counter(T_LOOP,_Time_RSCUCheck,600)==1)
          {// every 60's
              HT1381_RealTime(&time);
              by_Bu=time.Day;
              if(EEPROM_ReadInformation(EE_AutoUpdateDate) == 99)
              {// ��ʼ��һ����Ҫȡ������
                  EEPROM_SaveWorkInformation(EE_AutoUpdateDate,by_Bu);
              }
              else if(EEPROM_ReadInformation(EE_AutoUpdateDate)!= by_Bu)
              {// �����л�(ÿ��һ��)
                  EEPROM_SaveWorkInformation(EE_AutoUpdateDate,by_Bu);
                  //����������WiFiģ��
                  bit.AutoCheckUpdate=1;
                  WiFi_RSCU_Process(_RSCU_Reset);
              }
          }
      }
  }
}
// end v2.1-13
//20160325
/*******************************************************************************
* Function Name  : BackupOriginalSoftware
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BackupOriginalSoftware(void)
{
  if(Mx25L1606E_ReadByte(EEPRomPartition_Information_StartAddress_Backup) != _AlreadyBackUp_)
    EngineerMode_BackUpProcess(0);
}