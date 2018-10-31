/**
  ******************************************************************************
  * @file    stm32f2xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"
#include "stm32f2xx.h"
#include "stm32f2xx_it.h"

/* USER CODE BEGIN 0 */
#include "Beep.h"
#include "heart.h"
#include "led.h"
#include "keyboard.h"
#include "Digital.h"
#include "Console.h"
#include "timer.h"
#include "UART.h"
#include "C_safe.h"
#include "TV.h"
#include "HRM8700.h"
//#include "Voice.h"
#include "EEPRom.h"
#include "UsbFunction.h"
#include "LCB_Support.h"
#include "General.h"
//#include "WiFi.h"
#include "RF900.h"
#include "KeyFunction.h"
// 2014.11.24
#include "wifi_Digital.h"
#include "EEPRom.h"
//
#include "wifi_interface.h"
#include "Show_Functions.h"
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
#define GARRY_MENG
#ifdef GARRY_MENG
void hard_fault_handler_c(unsigned int * hardfault_args)
{
  unsigned int stacked_r0;
  unsigned int stacked_r1;
  unsigned int stacked_r2;
  unsigned int stacked_r3;
  unsigned int stacked_r12;
  unsigned int stacked_lr;
  unsigned int stacked_pc;
  unsigned int stacked_psr;
  stacked_r0 = ((unsigned long) hardfault_args[0]);
  stacked_r1 = ((unsigned long) hardfault_args[1]);
  stacked_r2 = ((unsigned long) hardfault_args[2]);
  stacked_r3 = ((unsigned long) hardfault_args[3]);
  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr = ((unsigned long) hardfault_args[5]);
  stacked_pc = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);
  printf ("[Hard fault handler]\n");
  printf ("R0 = 0x%x\n", stacked_r0);
  printf ("R1 = 0x%x\n", stacked_r1);
  printf ("R2 = 0x%x\n", stacked_r2);
  printf ("R3 = 0x%x\n", stacked_r3);
  printf ("R12 = 0x%x\n", stacked_r12);
  printf ("LR = 0x%x\n", stacked_lr);
  printf ("PC = 0x%x\n", stacked_pc);
  printf ("PSR = 0x%x\n", stacked_psr);
  printf ("BFAR = 0x%x\n", (*((volatile unsigned long *)(0xE000ED38))));
  printf ("CFSR = 0x%x\n", (*((volatile unsigned long *)(0xE000ED28))));
  printf ("HFSR = 0x%x\n", (*((volatile unsigned long *)(0xE000ED2C))));
  printf ("DFSR = 0x%x\n", (*((volatile unsigned long *)(0xE000ED30))));
  printf ("AFSR = 0x%x\n", (*((volatile unsigned long *)(0xE000ED3C))));
//  exit(0); // terminate
  return;
}
void HardFault_Handler(void)
{
    
    asm("TST LR, #4\n"
        "ITE EQ\n"
        "MRSEQ R0, MSP\n"
        "MRSNE R0, PSP\n"
        "B hard_fault_handler_c");
    
    while(1)
      ;
    SCB->AIRCR = 0x05fa0004 ; 
}
#else
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
  // 2013.11.26
  SCB->AIRCR = 0x05fa0004 ; // Software Reset
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}
#endif
/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
extern u8 by_In_Workout ;
/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static u8 Key_Delay=0;// LEDTest_Delay=0;;
static u8 LED_Loop1=0;
//static u8 LEDTest_Loop1=0, LEDTest_Loop2=0;
void SysTick_Handler(void)
{//==>每1ms中断一次
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */
if(LCB_Get_LEDSwitch() == 0)
  {
      Led_ComScan();
  }
  Console_SEC_Counter_Int();
  Heart_1ms_Int();//Heart_Counter_Int();
  BEEP_Int();
    /*
    #ifdef  RF_AM
    if(Console_GetAM_Status() == 0)
    {
        BLX_RXSignalCheck_Int();
    }
    #endif
    
    #ifdef  WiFi_Module
    if(Console_GetAM_Status() == 1)
    {
        WiFi_Process() ;
    }
    #endif
    */
    Show_Timer();
    //UsbFunction_CheckConnected(20000,5000) ;
//    HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_0);
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F2xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f2xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */
/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
  extern TIM_HandleTypeDef htim3;
  /* TIM2_CH1 toggling with frequency = 1ms */
  if(__HAL_TIM_GET_IT_SOURCE(&htim3,TIM_IT_CC1) != RESET)
  {     
      UsbFunction_CheckConnected(20000,5000) ;
      if(LCB_Get_LowPower_State() != 1)// Sinkyo
      {
          CSAFE_Counter_Int();
          if(Console_Audio(Data_Get,0)== Audio_TV )
          {
              TV_AutoReply();      
          }
          Digital_AutoReply();
          //Heart_Timer_Counter_Int();
          //Voice_CheckVoice_Int();  
          KeyScan_Int();
          if(Key_Delay >= 7)//20
          {
              Key_Delay=0;              
              Key_Counter_Int();
          }
          else Key_Delay++;
          
          if(LED_Loop1 >= 100)
          {
              LED_Loop1=0;
              Led_Blink_Flag_Int();
              Timer_Flag_Int(); 
              if(Console_Audio(Data_Get,0) == Audio_RF900)
              {
                  RF900_MessageHoldTime();
              }
              WiFi_Timer_Int();
          }
          else LED_Loop1++;
          // 2014.11.24 WiFi Function Enable
          if(EEPROM_ReadInformation(EE_Wireless) == D_Enable)
          {
              if((LED_Loop1 % 10) == 0)
              {
                  // 2015.01.22
                  //Digital_Command_Proce_wifi();
                  //
                  Digital_10ms_Timeout_Int_wifi();
              }
          }
          // 2014.01.23
          //RFID_GATRM310_Process();
          //
      }
      LCB_Timer_Int(); // LCB_Support.c
//      HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_0);
      __HAL_TIM_CLEAR_FLAG(&htim3,TIM_FLAG_CC1);
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1)+100);     
  }	
  /*
  if(TIM_GetITStatus(TIM2,TIM_IT_CC1) != RESET)
  {
      Led_ComScan();
      TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);
      TIM_SetCompare1(TIM2,TIM_GetCapture1(TIM2)+75);  
  }
  */
  /* TIM2_CH2 toggling with frequency = 1ms */
  /*
  if(TIM_GetITStatus(TIM2,TIM_IT_CC2) != RESET)
  {
      CSAFE_Counter_Int();
      TV_AutoReply();      
      Digital_AutoReply();
      Heart_Timer_Counter_Int();
      Voice_CheckVoice_Int();
      
      
      if(Key_Delay >= 7)//20
      {
          Key_Delay=0;
          Key_Scan_Int();
          Key_Counter_Int();
          
      }
      else Key_Delay++;
      
      if(LED_Loop1 >= 100)
      {
          LED_Loop1=0;
          Led_Blink_Flag_Int();
          Timer_Flag_Int(); 
      }
      else LED_Loop1++;
      
      
      TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);
      TIM_SetCompare2(TIM2,TIM_GetCapture2(TIM2)+100);        
  }	*/
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{//==>数字通讯传输
  if(LCB_Get_LowPower_State() != 1)// Sinkyo
  {
      Digital_UartTxRx_Information();
      return;
  }
  __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_RXNE);
  __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TXE);
  __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TC);
  __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
  __HAL_UART_DISABLE_IT(&huart2, UART_IT_TXE);
  __HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{//==>C-SFAE通讯传输 
  //UART_TxRx_Information();
  //CSAFE_TxRx_Information();
  
    if(__HAL_UART_GET_IT_SOURCE(&huart3, UART_IT_TXE) != RESET && __HAL_UART_GET_FLAG(&huart3,UART_FLAG_TXE) != RESET)//判断是否发送中断
    {
      /* Write one byte to the transmit data register */
      huart3.Instance->DR = (CSAFE_TxBuffer() & 0xFF);
      /* Clear the USART3 transmit interrupt */
      __HAL_UART_CLEAR_FLAG(&huart3, UART_FLAG_TXE);
      if(CSAFE_TxRx_Information(1) == 1)// TX
      {
        // Disable the USART3 Transmit interrupt 
        __HAL_UART_DISABLE_IT(&huart3, UART_IT_TXE);
      }
    }
    if(__HAL_UART_GET_IT_SOURCE(&huart3, UART_IT_RXNE) != RESET && __HAL_UART_GET_FLAG(&huart3,UART_FLAG_RXNE) != RESET) //判断是否接收中断
    {
      
      /* Read one byte from the receive data register */
      CSAFE_RxBuffer((int8_t)(huart3.Instance->DR & 0x00FF));//==>取得讯框信息
      /* Clear the USART3 Receive interrupt */
      __HAL_UART_CLEAR_FLAG(&huart3, UART_FLAG_RXNE);
      CSAFE_TxRx_Information(0);// RX
    }
}
/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{//==>TV控制
  //if(Console_Audio(Data_Get,0) == Audio_CAB)
  //{
  //    CAB_TxRx_Information();
  //}
  //else 
  if(Console_Audio(Data_Get,0) == Audio_TV)
  {
      TV_UartTxRx_Information();  
  }
  else
  {
      __HAL_UART_CLEAR_FLAG(&huart4, UART_FLAG_TXE);
      __HAL_UART_CLEAR_FLAG(&huart4, UART_FLAG_RXNE);
  }
}          
/**
  * @brief  This function handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{//==>AM system
  // 2014.11.24
  wifi_Digital_UartTxRx_Information();
  //
  /*
  #ifdef  RF_AM
    if(Console_GetAM_Status() == 0)
    {
        RF_UartTxRx_Information();  
        return;
    }
  #endif
  #ifdef  WiFi_Module
    if(Console_GetAM_Status() == 1)
    {
        WiFi_UartTxRx_Information();
        return;
    }
  #endif
    
  USART_ITConfig(UART5,USART_IT_TC,DISABLE);
  USART_ITConfig(UART5,USART_IT_RXNE,DISABLE); 
  USART_ITConfig(UART5,USART_IT_TXE, DISABLE);   
  */
}          
/*******************************************************************************
* Function Name  : USART6_IRQHandler
* Description    : This function handles USART6 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART6_IRQHandler(void)
{//==>Heart Rate communication function
  HR_UartTxRx_Information();
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
