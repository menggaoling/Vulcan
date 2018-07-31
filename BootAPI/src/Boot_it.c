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
#include "Boot_it.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void BootNMI_Handler(void) 
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void BootHardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
  SCB->AIRCR = 0x05fa0004 ; // Software Reset   
  /* Go to infinite loop when Hard Fault exception occurs */
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void BootMemManage_Handler(void)
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
void BootBusFault_Handler(void)
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
void BootUsageFault_Handler(void)
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
void BootSVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void BootDebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void BootPendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void BootSysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F2xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f2xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */
/**
  * @brief  This function handles Window WatchDog interrupt request.
  * @param  None
  * @retval None
  */
void BootWWDG_IRQHandler(void)
{
}

/**
  * @brief  This function handles PVD through EXTI Line detection interrupt request.
  * @param  None
  * @retval None
  */
void BootPVD_IRQHandler(void)
{
}

/**
  * @brief  This function handles Tamper and TimeStamps through the EXTI line interrupt request.
  * @param  None
  * @retval None
  */
void BootTAMP_STAMP_IRQHandler(void)
{
}

/**
  * @brief  This function handles RTC Wakeup through the EXTI line interrupt request.
  * @param  None
  * @retval None
  */
void BootRTC_WKUP_IRQHandler(void)
{
}

/**
  * @brief  This function handles FLASH interrupt request.
  * @param  None
  * @retval None
  */
void BootFLASH_IRQHandler(void) 
{
}

/**
  * @brief  This function handles RCC interrupt request.
  * @param  None
  * @retval None
  */
void BootRCC_IRQHandler(void)
{
}

/**
  * @brief  This function handles EXTI Line 0 interrupt request.
  * @param  None
  * @retval None
  */
void BootEXTI0_IRQHandler(void)
{
}

/**
  * @brief  This function handles EXTI Line 1 interrupt request.
  * @param  None
  * @retval None
  */
void BootEXTI1_IRQHandler(void)
{
}

/**
  * @brief  This function handles EXTI Line 2 interrupt request.
  * @param  None
  * @retval None
  */
void BootEXTI2_IRQHandler(void)
{
}

/**
  * @brief  This function handles EXTI Line 3 interrupt request.
  * @param  None
  * @retval None
  */
void BootEXTI3_IRQHandler(void)
{
}

/**
  * @brief  This function handles EXTI Line 4 interrupt request.
  * @param  None
  * @retval None
  */
void BootEXTI4_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Stream 0 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA1_Stream0_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Stream 1 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA1_Stream1_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Stream 2 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA1_Stream2_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Stream 3 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA1_Stream3_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Stream 4 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA1_Stream4_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Stream 5 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA1_Stream5_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Stream 6 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA1_Stream6_IRQHandler(void)
{
}

/**
  * @brief  This function handles ADC1, ADC2 and ADC3s interrupt request.
  * @param  None
  * @retval None
  */
void BootADC_IRQHandler(void)
{
}

/**
  * @brief  This function handles CAN1 TX Handler.
  * @param  None
  * @retval None
  */
void BootCAN1_TX_IRQHandler(void)
{
}

/**
  * @brief  This function handles CAN1 RX0 Handler.
  * @param  None
  * @retval None
  */
void BootCAN1_RX0_IRQHandler(void)
{
}

/**
  * @brief  This function handles CAN1 RX1 Handler.
  * @param  None
  * @retval None
  */
void BootCAN1_RX1_IRQHandler(void)
{
}

/**
  * @brief  This function handles CAN1 SCE Handler.
  * @param  None
  * @retval None
  */
void BootCAN1_SCE_IRQHandler(void)
{
}

/**
  * @brief  This function handles External Line[9:5]s interrupt request.
  * @param  None
  * @retval None
  */
void BootEXTI9_5_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM1 Break interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM1_BRK_TIM9_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM1 Update interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM1_UP_TIM10_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM1 Trigger and Commutation interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM1_TRG_COM_TIM11_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM1 Capture Compare interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM1_CC_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM2_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM3_IRQHandler(void)
{
}
          
/**
  * @brief  This function handles TIM4 interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM4_IRQHandler(void)
{
}
          
/**
  * @brief  This function handles I2C1 Event interrupt request.
  * @param  None
  * @retval None
  */
void BootI2C1_EV_IRQHandler(void)
{
}

/**
  * @brief  This function handles I2C1 Error interrupt request.
  * @param  None
  * @retval None
  */
void BootI2C1_ER_IRQHandler(void)
{
}

/**
  * @brief  This function handles I2C2 Event interrupt request.
  * @param  None
  * @retval None
  */
void BootI2C2_EV_IRQHandler(void)
{
}

/**
  * @brief  This function handles I2C2 Error interrupt request.
  * @param  None
  * @retval None
  */
void BootI2C2_ER_IRQHandler(void)
{
}
          
/**
  * @brief  This function handles SPI1 interrupt request.
  * @param  None
  * @retval None
  */
void BootSPI1_IRQHandler(void)
{
}

/**
  * @brief  This function handles SPI2 interrupt request.
  * @param  None
  * @retval None
  */
void BootSPI2_IRQHandler(void)
{
}
          
/**
  * @brief  This function handles USART1 interrupt request.
  * @param  None
  * @retval None
  */
void BootUSART1_IRQHandler(void)
{
} 

/**
  * @brief  This function handles USART2 interrupt request.
  * @param  None
  * @retval None
  */
void BootUSART2_IRQHandler(void)
{
} 

/**
  * @brief  This function handles USART3 interrupt request.
  * @param  None
  * @retval None
  */
void BootUSART3_IRQHandler(void)
{
} 

/**
  * @brief  This function handles External Line[15:10]s interrupt request.
  * @param  None
  * @retval None
  */
void BootEXTI15_10_IRQHandler(void)
{
}            
    
/**
  * @brief  This function handles RTC Alarm (A and B) through EXTI Line interrupt request.
  * @param  None
  * @retval None
  */
void BootRTC_Alarm_IRQHandler(void)
{
}            

/**
  * @brief  This function handles USB OTG FS Wakeup through EXTI line interrupt request.
  * @param  None
  * @retval None
  */
void BootOTG_FS_WKUP_IRQHandler(void)
{
}  

/**
  * @brief  This function handles TIM8 Break and TIM12 interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM8_BRK_TIM12_IRQHandler(void)
{
}  

/**
  * @brief  This function handles TIM8 Update and TIM13 interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM8_UP_TIM13_IRQHandler(void)
{
}  

/**
  * @brief  This function handles TIM8 Trigger and Commutation and TIM14 interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM8_TRG_COM_TIM14_IRQHandler(void)
{
}  
         
/**
  * @brief  This function handles TIM8 Capture Compare interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM8_CC_IRQHandler(void)
{
}          
         
/**
  * @brief  This function handles DMA1 Stream7 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA1_Stream7_IRQHandler(void)
{
}          
         
/**
  * @brief  This function handles FSMC interrupt request.
  * @param  None
  * @retval None
  */
void BootFSMC_IRQHandler(void)
{
}          
         
/**
  * @brief  This function handles SDIO interrupt request.
  * @param  None
  * @retval None
  */
void BootSDIO_IRQHandler(void)
{
}          
         
/**
  * @brief  This function handles TIM5 interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM5_IRQHandler(void)
{
}          

/**
  * @brief  This function handles SPI3 interrupt request.
  * @param  None
  * @retval None
  */
void BootSPI3_IRQHandler(void)
{
}          
      
/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
void BootUART4_IRQHandler(void)
{
}          
         
/**
  * @brief  This function handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
//extern void WiFiBoot_UartTxRx_Information(void) ;
void BootUART5_IRQHandler(void)
{
  //WiFiBoot_UartTxRx_Information() ;
  //return ;    
}          

/**
  * @brief  This function handles TIM6 and DAC1&2 underrun errors interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM6_DAC_IRQHandler(void)
{
}

          
/**
  * @brief  This function handles TIM7 interrupt request.
  * @param  None
  * @retval None
  */
void BootTIM7_IRQHandler(void)
{
  
}
          
/**
  * @brief  This function handles DMA2 Stream 0 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA2_Stream0_IRQHandler(void)
{
}


/**
  * @brief  This function handles DMA2 Stream 1 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA2_Stream1_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA2 Stream 2 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA2_Stream2_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA2 Stream 3 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA2_Stream3_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA2 Stream 4 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA2_Stream4_IRQHandler(void)
{
}          


void BootCAN2_TX_IRQHandler(void)
{
}

/**
  * @brief  This function handles CAN2 RX0 Handler.
  * @param  None
  * @retval None
  */
void BootCAN2_RX0_IRQHandler(void)
{
}

/**
  * @brief  This function handles CAN2 RX1 Handler.
  * @param  None
  * @retval None
  */
void BootCAN2_RX1_IRQHandler(void)
{
}

/**
  * @brief  This function handles CAN2 SCE Handler.
  * @param  None
  * @retval None
  */
void BootCAN2_SCE_IRQHandler(void)
{
}
          
/**
  * @brief  This function handles DMA2 Stream 5 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA2_Stream5_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles DMA2 Stream 6 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA2_Stream6_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles DMA2 Stream 7 interrupt request.
  * @param  None
  * @retval None
  */
void BootDMA2_Stream7_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles USART6 interrupt request.
  * @param  None
  * @retval None
  */
void BootUSART6_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles I2C3 event interrupt request.
  * @param  None
  * @retval None
  */
void BootI2C3_EV_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles I2C3 error interrupt request.
  * @param  None
  * @retval None
  */
void BootI2C3_ER_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles USB OTG HS End Point 1 Out interrupt request.
  * @param  None
  * @retval None
  */
void BootOTG_HS_EP1_OUT_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles USB OTG HS End Point 1 In interrupt request.
  * @param  None
  * @retval None
  */
void BootOTG_HS_EP1_IN_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles USB OTG HS Wakeup through EXTI interrupt request.
  * @param  None
  * @retval None
  */
void BootOTG_HS_WKUP_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles USB OTG HS interrupt request.
  * @param  None
  * @retval None
  */
void BootOTG_HS_IRQHandler(void)
{
  
}
           
/**
  * @brief  This function handles Hash and RNG interrupt request.
  * @param  None
  * @retval None
  */
void BootHASH_RNG_IRQHandler(void)
{
  
}
 
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
