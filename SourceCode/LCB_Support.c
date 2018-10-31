#include  <stdio.h>
#include "stm32f2xx_ll_exti.h"
#include "stm32f2xx_ll_gpio.h"
#include  "fat.h"
#include  "mst.h"
#include  "os.h"
#include  "Digital.h"
#include  "led.h"
#include  "UsbFunction.h"
#include  "LCB_Support.h"
#include  "General.h"
#include  "keyboard.h"
#include  "Show.h"
#include  "Console.h"
#include  "UART.h"
//#include  "WiFi.h"
#include "TV.h"
#include "EEPRom.h"
// 2015.01.23
//#include  "wifi_Digital.h"
#include  "WiFi_Information.h"
#include  "wifi_interface.h"
#include "Show_Functions.h"
#include  "Mx25L1606E.h"
#include "C_safe.h"

static struct
{
    unsigned SleepMode : 1;
    unsigned Sleeping : 1;
    unsigned LCBNoSleepMode : 1;
    unsigned WakeUpRetry : 1;
    unsigned LEDSwitch : 1;
}lcb;

unsigned short LCB_Delay[3];
unsigned char LCB_Timer[3];

// LCB EEPROM ERROR Content
unsigned short LCB_EEPROM_Memory_Size = 0;
//unsigned char GetLCBDeviceState = 0; //==> �õ�LCB Device ��Ϣ״̬
                         //==> 0 : Clear  1: Get OK  2: No Supply
unsigned char LCB_DeviceDataBuffer[20] = {0};
unsigned char LCB_GetingEEPRomFlag = 0;// ==>LCB EEPROM ����ȡ�����

// LCB EuPs
unsigned char LCB_EUPs_State = 0;
unsigned char LowPower_State = 0;
unsigned char SleepCount = 0,SleepCount_2 = 6, EnterSleepStep = 0, WakeUpRetryCount = 0;


//==============================================================================
//==> ʱ�䴦��
//==============================================================================
//�ر��ض�������
void LCB_TimerClose(unsigned char by_Dat)
{
  LCB_Timer[by_Dat]=0;
}
//ÿ1ms�� 1 ��
void LCB_Timer_Int(void)
{
  static unsigned short LCB_Loop = 0;
  char i;
  
  if(LCB_Loop >= 1000)
  {// 1000 ms
      LCB_Loop = 0;
      for(i=0;i<3;i++)
      {
          if((LCB_Timer[i] == 1)&&(LCB_Delay[i] < 65000)) LCB_Delay[i]++;//==>�����ۼ���С��6500��ʱ
      }
  }
  else
      LCB_Loop ++;
}
//�������ĳһ������
void LCB_TimerCounterClear(unsigned char by_Index)
{
  LCB_Delay[by_Index]=0;
  LCB_Timer[by_Index]=1;
}
//�趨������ 1.LOOP or STEP 2.��N������ 3.����TIME
unsigned char LCB_TimerCounter(unsigned char by_Mode,unsigned char by_Who,unsigned short w_Dat)
{
  unsigned char b_Out;
  b_Out=0;
  if(LCB_Timer[by_Who]==1 && LCB_Delay[by_Who]>=w_Dat)
  {
        b_Out=1;
        if(by_Mode==T_LOOP) LCB_Delay[by_Who]=0;
        else LCB_Timer[by_Who]=0;
  }
  return(b_Out);
}

void Main_TimeDelay(unsigned char DelayTime)
{//==> DelayTime  unit: 1 second
    //LCB_Set_GetLCBEEPRomFlag(0);
    LCB_TimerCounterClear(0);
    while(!LCB_TimerCounter(T_STEP,0,DelayTime));
    LCB_TimerClose(0);
    return;
}

//==============================================================================
//==>EuPs(Sleep Mode)����
// Return : 0 ==> δ����
//          1 ==> ׼������
//          2 ==> �� Sleep Mode ����
//==============================================================================
unsigned char LCB_SleepMode(unsigned short by_SystemStatus, unsigned char by_KeyState, unsigned char by_NowStatus, unsigned short LCB_SleepTime)
{//==> by_SystemStatus  1:AC  0:Self-Powered
    unsigned char by_Retn = 0;
    
    //==>20111219 EUPS test
    //if(by_KeyState == K_NONE && by_NowStatus == M_NOVE && by_SystemStatus == 1 && LCB_SleepTime != 0)
    if(EnterSleepStep == 2 ||(by_KeyState == K_NONE && by_NowStatus == M_NONE && by_SystemStatus == 1 && LCB_SleepTime != 0))
    {
         if(lcb.SleepMode)
         {// Sleep Mode
              by_Retn = 1;
              switch(EnterSleepStep)
              {
                  case 0: Show_Message(Message_EnterSleepMode,0,0,Show_Auto);
                          if(Led_Get_String_End())
                          {
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              EnterSleepStep = 1;
                              LCB_TimerCounterClear(2);
                              Led_Set_Target(Display_OFF,0);
                              // 2014.01.21-4
                              if(EEPROM_ReadInformation(EE_TVPower) == _OFF && TV_GetPowe_OnOff() == 1)
                              {//==>��TV ON ʱ�� OFF
                                  TV_SetCommand(TVCom_POWER,TVData_PowerOFF);
                              }
                              //
                              
                          }
                          break;
                  case 1: 
                          Led_NumberSite(29,SleepCount_2-1);
                          if(LCB_TimerCounter(T_LOOP,2,1))
                              SleepCount_2 -= 1;
                          if(SleepCount_2 == 0)
                          {
                              if(!Check_SafeKey())
                              {// �������ذ���
                                  return 2;
                              }
                              EnterSleepStep = 2;
                              Digital_Command(CmdEUPsMode,LCB_EUPs_Implement);
                              SleepCount += 1;
                              LCB_TimerCounterClear(2);
                              //Led_StringParagraph(255,0);//==>������� 
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              Led_Set_Target(Display_OFF,0);
                              //Led_FanControlReset();
                          }
                          break;  
                  case 2: 
                          if(LCB_EUPs_State == 1 || lcb.LCBNoSleepMode == 1)
                          {
                              lcb.Sleeping = 0;
                              while(!Low_PowerMode());
                              by_Retn = 2;
                              LCB_TimerClose(2);
                          }
                          else if(LCB_TimerCounter(T_STEP,2,2))
                          {
                              if(SleepCount >= 3 || LCB_EUPs_State == 0xff)
                              {
                                  SleepCount = 0;
                                  lcb.LCBNoSleepMode = 1;
                              }
                              else
                              {
                                  LCB_TimerCounterClear(2);
                                  Digital_Command(CmdEUPsMode,LCB_EUPs_Implement);
                                  SleepCount += 1;
                              }
                          } 
                          break;
              }      
         }
         else if(LCB_TimerCounter(T_STEP,2,LCB_SleepTime))
         {
              lcb.SleepMode = 1;
              Show_Message(Message_Blank,0,0,Show_Blank);
              Led_Set_Target(Display_OFF,0);
         }    
    }
    else
    { 
        lcb.LCBNoSleepMode = 0;
        LCB_TimerCounterClear(2);
        if(lcb.SleepMode)
            by_Retn = 1;
        lcb.SleepMode = 0;
        SleepCount = 0;
        EnterSleepStep = 0;
        SleepCount_2 = 6;    
        WakeUpRetryCount = 0;
        lcb.WakeUpRetry = 0;
    }
    return by_Retn;
}

void LCB_SleepMode_Initial(void)
{
    LowPower_State = 0;
    SleepCount = 0;
    SleepCount_2 = 6;
    EnterSleepStep = 0;
    lcb.SleepMode = 0;
    lcb.LCBNoSleepMode = 0;
    lcb.Sleeping = 0;
    WakeUpRetryCount = 0;
    lcb.WakeUpRetry = 0;    
    lcb.LEDSwitch = 0;
    LCB_TimerCounterClear(2);
}

unsigned char Low_PowerMode(void)
{//==>����ʡ��ģʽ
  LL_EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  unsigned long i=0,j=0,k=0;
  unsigned char SendCount = 0;
  unsigned short Key_State = 0;
  
  
  if(!lcb.LCBNoSleepMode)
  {
      Digital_Clear_ErrorCount();
      LowPower_State = 1;
      //IO_Set_Digital_CTL();//==>20111219 EUPS test
      //
      if(!lcb.Sleeping || lcb.WakeUpRetry)
      {
          GPIO_Reset(); 
          IO_SleepMode_LED(GPIO_PIN_SET);
          // PA2 Set High
          GPIO_InitStructure.Pin = GPIO_PIN_2;
          GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
          GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
          HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
          HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
          if(!lcb.WakeUpRetry)
              Main_TimeDelay(2);
      }
      lcb.Sleeping = 1;
      lcb.LEDSwitch = 1;
      //
      if(!lcb.WakeUpRetry)
      {// MCU Enter STOP Mode
          LL_EXTI_DeInit() ;
          __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
          __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
          //
          //====>> Ϊ���MCUʡ��ģʽ�趨ʹ��,��Ϊ�ⲿѶ�Ŵ���������
          // 0: PA
          // 1: PB
          // 2: PC
          // 3: PD
          // 4: PE
          // 5: PF
          // 6: PG
          //                                   
          // AFIO->EXTICR[0] 0~3      >> 0x0000xxxx
          // AFIO->EXTICR[1] 4~7      >> 0x0000xxxx
          // AFIO->EXTICR[2] 8~11     >> 0x0000xxxx
          // AFIO->EXTICR[3] 12~15    >> 0x0000xxxx
          SYSCFG->EXTICR[2] = 0x00003333 ;
          SYSCFG->EXTICR[3] = 0x00003333 ;
          //
          // 
          /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
          EXTI_InitStructure.Line_0_31 = LL_EXTI_LINE_8|LL_EXTI_LINE_9|LL_EXTI_LINE_10|LL_EXTI_LINE_11|LL_EXTI_LINE_12|LL_EXTI_LINE_13|LL_EXTI_LINE_14|LL_EXTI_LINE_15;
          EXTI_InitStructure.Mode = LL_EXTI_MODE_EVENT;
          EXTI_InitStructure.Trigger = LL_EXTI_TRIGGER_RISING;
          EXTI_InitStructure.LineCommand = ENABLE ;
          LL_EXTI_Init(&EXTI_InitStructure);
        
          
          /* Mode: SLEEP + Entry with WFE*/
          //__WFE();
          /* Mode: STOP + Regulator in ON + Entry with WFE*/
          HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFE);
          /* Request to enter STOP mode with regulator in low power mode*/
          //PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE);
          LL_EXTI_DeInit();
      }
      SystemInit(); // Reset Clock
      HAL_Init(); // Reset Clock
  }
  //EXTI_DeInit();
  //SystemInit(); // Reset Clock
  while(1)
  {
       if(lcb.WakeUpRetry)
          break;
      if(!lcb.LCBNoSleepMode)
      {
          k += 1;
          Key_State = LL_GPIO_ReadInputPort(GPIOD)&0xFF00;
          if( Key_State != K_NONE)
              j += 1;
          else
              j = 0;
          
          if(j > 200000) // 200 ms
              break;
          else if(k > 1000000) // 1 s
              return 0;
      }
      else
      {// ��֧Ԯ Sleep Mode
          if(Key_Proce() != K_NONE)
              break;
          else
              return 0;
      }
  }
  //
  
  // ������ !! ǿ�ƽ�������
  //lcb.LCBNoSleepMode = 0;
  //
  
  
  if(!lcb.LCBNoSleepMode)
  {
      //EXTI_DeInit() ;
      // Send the low signal to wake up the LCB
      IO_Set_Digital_CTL();
      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);

      for(i=0;i < 2000000;i++); // 450 ms
      // System Initial
      System_Initial(); 
      // Power
      //GPIO_SetBits(GPIOC,GPIO_Pin_14);//==>5v
      //GPIO_SetBits(GPIOC,GPIO_Pin_15);//==>12v
      //
      CSAFE_Initial();
      TV_Initial();
      Digital_Initial(); 
      Digital_CommandStart(1);// ֹͣ�Զ�������
      UsartInitial();
      for(i=0;i < 1000000;i++); // Delay 225ms
      
      SendCount = 0;
      //
      LCB_EUPs_State = 0xff;
      //
      Digital_Command(CmdEUPsMode,LCB_EUPs_Leave);
      //==>20111219 EUPS test
      LowPower_State = 0;
      //
      LCB_TimerCounterClear(2);  
      while(1) 
      {
          Digital_Clear_ErrorCount();
          //==>20111219 EUPS test
          if(LCB_EUPs_State == 1)  
          {
              Main_TimeDelay(1);
              break;
          }
          else if(LCB_TimerCounter(T_STEP,2,2) == 1)
          {
              SendCount += 1;
              if(SendCount >= 3)
              {
                  WakeUpRetryCount += 1;
                  if(WakeUpRetryCount >= 3)
                  {
                      
                      LED_5V_Set();
                      Led_Set_Target(Display_OFF,1);
                      lcb.LEDSwitch = 0;
                      while(1)
                          Show_Message(Message_LCBNoWakeUp,0,0,Show_Auto);
                      
                  }
                  else
                  {
                      
                      lcb.WakeUpRetry = 1;
                      return 0;
                  }
              }
              Digital_Command(CmdEUPsMode,LCB_EUPs_Leave); 
              LCB_TimerCounterClear(2);
          }
      }
      Digital_CommandStart(0);// �����Զ�������
      // RFID
      //RFID_GATRM310_HW_Initial();
      //
  } 
      
  Main_TimeDelay(1); // Delay
  
  //lcb.Sleeping = 0;
  
  Open_UCB_PSW();  
  
  // 2015.09.11 
  //WiFi module initial
  UsartInitial_WiFi();
  WiFi_Information_Initial();
  Digital_Initial_Data_wifi();
  
  IO_Set_SK8();  //==> ��������Ϣ�ͳ��󣬳�ʼ����λ�����ⰴ����Ϣ����
  IO_Set_SK1();
  IO_Set_SK2();
  IO_Set_SK3();
  IO_Set_SK4();
  IO_Set_SK5();
  IO_Set_SK6();
  IO_Set_SK7();
  
    
  IO_SleepMode_LED(GPIO_PIN_RESET);  
  CSAFE_Answer(Csafe_Finished);
  for(i=0;i < 500000;i++); // Delay 225ms
  lcb.Sleeping = 0;
  lcb.LEDSwitch = 0;
  return 1;   
}
unsigned char LCB_Get_LEDSwitch(void)
{
  return lcb.LEDSwitch;
}
unsigned char LCB_Get_LowPower_State(void)
{
    return LowPower_State;
}
//===================================
// 2012.11.22
//void LCB_Set_LowPower_State(u8 by_D)
//{
//  LowPower_State = by_D;
//}
//===================================
void Digital_Set_LCBEUPsState(unsigned char by_Dat)
{
    LCB_EUPs_State = by_Dat;
}
unsigned char LCB_Get_EUPsState(void)
{
  return LCB_EUPs_State;
}

unsigned char LCB_GetSleepModeState(void)
{
    return lcb.SleepMode;
}

void GPIO_Reset(void)// Sinkyo..EUPs
{
    //====> BEEP ���ƽ�
    //=================================================================
    IO_Clear_BEEP(); 
    //=================================================================
    //====> MBI5026 ���ƽ�
    //=================================================================
    IO_Clear_OE(); //IO_Set_OE()   
    IO_Clear_CLK(); 
    IO_Clear_LE();  
    IO_Clear_SDI();  
    //=================================================================
    //====> Keypad�������ƽ�
    //=================================================================
    IO_Set_SK1();
    IO_Set_SK2();
    IO_Set_SK3();
    IO_Set_SK4();
    IO_Set_SK5();
    IO_Set_SK6();
    IO_Set_SK7();
    IO_Set_SK8();
    //=================================================================
    //====> ���ȿ��ƽ�
    //=================================================================
    //IO_Clear_PAN_LSP()    
    //IO_Clear_PAN_MSP()  
    //IO_Clear_PAN_HSP()   
    //=================================================================
    //====> Voice ���ƽ�
    //=================================================================
    //IO_Clear_Voice1()
    //IO_Clear_Voice2()
    //IO_Clear_Voice3();  
    //=================================================================
    //====> RTC ���ƽ�
    //=================================================================
    IO_Clear_HT1381_SCK();   
    IO_Clear_HT1381_Data();  
    IO_Clear_HT1381_RST();      
    //=================================================================
    //====> ����ͨѶ TX/RX ������ƽ�
    //=================================================================
    IO_Clear_Digital_CTL();
    //=================================================================
    //====> ���EEPROM���ƽ�
    //=================================================================
    IO_Clear_EEPROM_CS();   
    IO_Clear_EEPROM_SK();   
    IO_Clear_EEPROM_DI();  
    //=================================================================
    //====> POWER ���ƽ�
    //=================================================================
    LED_5V_Clear();  
    LED_12V_Clear();  
    ERP_Power(GPIO_PIN_RESET);
    IO_USB_ResetILIM(); 
    //================================================================   
    //GPIO_ResetBits(GPIOD,GPIO_Pin_0);
    IO_WiFi_ResetLow();  
    IO_TV_Audio();// ��Դ
    IO_CloseAudio();//==>ShutDown
    // USB Control send Low
    IO_USB_ResetCTL1();
    IO_USB_ResetCTL2();
    IO_USB_ResetCTL3();
    IO_USB_ResetILIM();
    IO_USB_ResetEN();
    //
}
// start v2.001-1
unsigned char LCB_QuickERP(unsigned char _SleepTime)
{
  unsigned char EnterSleepStep = 1;
  unsigned char by_String[18] = {0};
  
  Show_Message(Message_Blank,0,0,Show_Blank);
  Led_Set_Target(Display_OFF,0);
  if(EEPROM_ReadInformation(EE_TVPower) == _OFF && TV_GetPowe_OnOff() == 1)
  {//==>��TV ON ʱ�� OFF
      TV_SetCommand(TVCom_POWER,TVData_PowerOFF);
  }
  lcb.LCBNoSleepMode = 0;
  LCB_EUPs_State=0;
  LCB_TimerCounterClear(2);
  while(1)
  {
      switch(EnterSleepStep)
      {
          case 1: 
                  if(_SleepTime != 0)
                  {
                      sprintf((char*)by_String, "%d",_SleepTime);
                      Led_String(by_String, 0, Show_Auto);
                      if(LCB_TimerCounter(T_LOOP,2,1))
                      {
                          _SleepTime -= 1;
                      }
                  }
                  else 
                  {
                      if(Check_SafeKey()==0)
                      {// �������ذ���
                          LCB_TimerCounterClear(2);
                          while(1)
                          {
                              Led_String("Safe key wrong",0,Show_Auto);
                              if(LCB_TimerCounter(T_LOOP,2,3)) return M_RESET;
                          }
                      }
                      EnterSleepStep = 2;
                      Digital_Command(CmdEUPsMode,LCB_EUPs_Implement);
                      SleepCount += 1;
                      LCB_TimerCounterClear(2);
//                      Led_StringParagraph(255,0);//==>�������
                      Led_Set_Target(Display_OFF,0);
//                      Led_FanControlReset();
                  }
                  break;  
          case 2: 
                  if(LCB_EUPs_State == 1 || lcb.LCBNoSleepMode == 1)
                  {
                      lcb.Sleeping = 0;
                      while(!Low_PowerMode());
                      return M_RESET;
                  }
                  else if(LCB_TimerCounter(T_STEP,2,2))
                  {
                      if(SleepCount >= 3 || LCB_EUPs_State == 0xff)
                      {
                          //SleepCount = 0;
                          //lcb.LCBNoSleepMode = 1;
                          LCB_TimerCounterClear(2);
                          while(1)
                          {
                              Led_String("MCB not support",0,Show_Auto);
                              if(LCB_TimerCounter(T_LOOP,2,5)) return M_NONE;
                          }
                      }
                      else
                      {
                          LCB_TimerCounterClear(2);
                          Digital_Command(CmdEUPsMode,LCB_EUPs_Implement);
                          SleepCount += 1;
                      }
                  } 
                  break;
      }  
  }
}
// end
