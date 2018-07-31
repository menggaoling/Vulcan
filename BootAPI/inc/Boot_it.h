/**
  ******************************************************************************
  * @file    stm32f2xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOOT_IT_H
#define __BOOT_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void BootNMI_Handler(void);
void BootHardFault_Handler(void);
void BootMemManage_Handler(void);
void BootBusFault_Handler(void);
void BootUsageFault_Handler(void);
void BootSVC_Handler(void);
void BootDebugMon_Handler(void);
void BootPendSV_Handler(void);
void BootSysTick_Handler(void);

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f205xx.s).                                            */
/******************************************************************************/                                                                            
void BootWWDG_IRQHandler(void);                                                                          
void BootPVD_IRQHandler(void);                                                                           
void BootTAMP_STAMP_IRQHandler(void);                                                                    
void BootRTC_WKUP_IRQHandler(void);                                                                      
void BootFLASH_IRQHandler(void);                                                                         
void BootRCC_IRQHandler(void);                                                                           
void BootEXTI0_IRQHandler(void);                                                                         
void BootEXTI1_IRQHandler(void);                                                                         
void BootEXTI2_IRQHandler(void);                                                                         
void BootEXTI3_IRQHandler(void);                                                                         
void BootEXTI4_IRQHandler(void);                                                                         
void BootDMA1_Stream0_IRQHandler(void);                                                                  
void BootDMA1_Stream1_IRQHandler(void);                                                                  
void BootDMA1_Stream2_IRQHandler(void);                                                                  
void BootDMA1_Stream3_IRQHandler(void);                                                                  
void BootDMA1_Stream4_IRQHandler(void);                                                                  
void BootDMA1_Stream5_IRQHandler(void);                                                                  
void BootDMA1_Stream6_IRQHandler(void);                                                                  
void BootADC_IRQHandler(void);                                                                           
void BootCAN1_TX_IRQHandler(void);                                                                       
void BootCAN1_RX0_IRQHandler(void);                                                                      
void BootCAN1_RX1_IRQHandler(void);                                                                      
void BootCAN1_SCE_IRQHandler(void);                                                                      
void BootEXTI9_5_IRQHandler(void);                                                                       
void BootTIM1_BRK_TIM9_IRQHandler(void);                                                                 
void BootTIM1_UP_TIM10_IRQHandler(void);                                                                 
void BootTIM1_TRG_COM_TIM11_IRQHandler(void);                                                            
void BootTIM1_CC_IRQHandler(void);                                                                       
void BootTIM2_IRQHandler(void);                                                                          
void BootTIM3_IRQHandler(void);                                                                          
void BootTIM4_IRQHandler(void);                                                                          
void BootI2C1_EV_IRQHandler(void);                                                                       
void BootI2C1_ER_IRQHandler(void);                                                                       
void BootI2C2_EV_IRQHandler(void);                                                                       
void BootI2C2_ER_IRQHandler(void);                                                                       
void BootSPI1_IRQHandler(void);                                                                          
void BootSPI2_IRQHandler(void);                                                                          
void BootUSART1_IRQHandler(void);                                                                        
void BootUSART2_IRQHandler(void);                                                                        
void BootUSART3_IRQHandler(void);                                                                        
void BootEXTI15_10_IRQHandler(void);                                                                     
void BootRTC_Alarm_IRQHandler(void);                                                                     
void BootOTG_FS_WKUP_IRQHandler(void);                                                                   
void BootTIM8_BRK_TIM12_IRQHandler(void);                                                                
void BootTIM8_UP_TIM13_IRQHandler(void);                                                                 
void BootTIM8_TRG_COM_TIM14_IRQHandler(void);                                                            
void BootTIM8_CC_IRQHandler(void);                                                                       
void BootDMA1_Stream7_IRQHandler(void);                                                                  
void BootFSMC_IRQHandler(void);                                                                          
void BootSDIO_IRQHandler(void);                                                                          
void BootTIM5_IRQHandler(void);                                                                          
void BootSPI3_IRQHandler(void);                                                                          
void BootUART4_IRQHandler(void);                                                                         
void BootUART5_IRQHandler(void);                                                                         
void BootTIM6_DAC_IRQHandler(void);                                                                      
void BootTIM7_IRQHandler(void);                                                                          
void BootDMA2_Stream0_IRQHandler(void);                                                                  
void BootDMA2_Stream1_IRQHandler(void);                                                                  
void BootDMA2_Stream2_IRQHandler(void);                                                                  
void BootDMA2_Stream3_IRQHandler(void);                                                                  
void BootDMA2_Stream4_IRQHandler(void);                                                                  
void BootCAN2_TX_IRQHandler(void);                                                                       
void BootCAN2_RX0_IRQHandler(void);                                                                      
void BootCAN2_RX1_IRQHandler(void);                                                                      
void BootCAN2_SCE_IRQHandler(void);                                                                      
void OTG_FS_IRQHandler(void);                                                                            
void BootDMA2_Stream5_IRQHandler(void);                                                                  
void BootDMA2_Stream6_IRQHandler(void);                                                                  
void BootDMA2_Stream7_IRQHandler(void);                                                                  
void BootUSART6_IRQHandler(void);                                                                        
void BootI2C3_EV_IRQHandler(void);                                                                       
void BootI2C3_ER_IRQHandler(void);                                                                       
void BootOTG_HS_EP1_OUT_IRQHandler(void);                                                                
void BootOTG_HS_EP1_IN_IRQHandler(void);                                                                 
void BootOTG_HS_WKUP_IRQHandler(void);                                                                   
void BootOTG_HS_IRQHandler(void);                                                                        
void BootHASH_RNG_IRQHandler(void);                                                                      


#ifdef __cplusplus
}
#endif

#endif /* __BOOT_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
