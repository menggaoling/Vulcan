;/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
;* File Name          : startup_stm32f205xx.s
;* Author             : MCD Application Team
;* Description        : STM32F20x/21x devices vector table for EWARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == _iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR 
;*                        address.
;*                      - Branches to main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M3 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;********************************************************************************
;* 
;* Redistribution and use in source and binary forms, with or without modification,
;* are permitted provided that the following conditions are met:
;*   1. Redistributions of source code must retain the above copyright notice,
;*      this list of conditions and the following disclaimer.
;*   2. Redistributions in binary form must reproduce the above copyright notice,
;*      this list of conditions and the following disclaimer in the documentation
;*      and/or other materials provided with the distribution.
;*   3. Neither the name of STMicroelectronics nor the names of its contributors
;*      may be used to endorse or promote products derived from this software
;*      without specific prior written permission.
;*
;* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
;* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;* 
;*******************************************************************************
;
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler

        DCD     BootNMI_Handler           ; NMI Handler
        DCD     BootHardFault_Handler     ; Hard Fault Handler
        DCD     BootMemManage_Handler     ; MPU Fault Handler
        DCD     BootBusFault_Handler      ; Bus Fault Handler
        DCD     BootUsageFault_Handler    ; Usage Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     BootSVC_Handler           ; SVCall Handler
        DCD     BootDebugMon_Handler      ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     BootPendSV_Handler        ; PendSV Handler
        DCD     BootSysTick_Handler       ; SysTick Handler

         ; External Interrupts
        DCD     BootWWDG_IRQHandler               ; Window WatchDog
        DCD     BootPVD_IRQHandler                ; PVD through EXTI Line detection
        DCD     BootTAMP_STAMP_IRQHandler         ; Tamper and TimeStamps through the EXTI line
        DCD     BootRTC_WKUP_IRQHandler           ; RTC Wakeup through the EXTI line
        DCD     BootFLASH_IRQHandler              ; FLASH
        DCD     BootRCC_IRQHandler                ; RCC
        DCD     BootEXTI0_IRQHandler              ; EXTI Line0
        DCD     BootEXTI1_IRQHandler              ; EXTI Line1
        DCD     BootEXTI2_IRQHandler              ; EXTI Line2
        DCD     BootEXTI3_IRQHandler              ; EXTI Line3
        DCD     BootEXTI4_IRQHandler              ; EXTI Line4
        DCD     BootDMA1_Stream0_IRQHandler       ; DMA1 Stream 0
        DCD     BootDMA1_Stream1_IRQHandler       ; DMA1 Stream 1
        DCD     BootDMA1_Stream2_IRQHandler       ; DMA1 Stream 2
        DCD     BootDMA1_Stream3_IRQHandler       ; DMA1 Stream 3
        DCD     BootDMA1_Stream4_IRQHandler       ; DMA1 Stream 4
        DCD     BootDMA1_Stream5_IRQHandler       ; DMA1 Stream 5
        DCD     BootDMA1_Stream6_IRQHandler       ; DMA1 Stream 6
        DCD     BootADC_IRQHandler                ; ADC1, ADC2 and ADC3s
        DCD     BootCAN1_TX_IRQHandler            ; CAN1 TX
        DCD     BootCAN1_RX0_IRQHandler           ; CAN1 RX0
        DCD     BootCAN1_RX1_IRQHandler           ; CAN1 RX1
        DCD     BootCAN1_SCE_IRQHandler           ; CAN1 SCE
        DCD     BootEXTI9_5_IRQHandler            ; External Line[9:5]s
        DCD     BootTIM1_BRK_TIM9_IRQHandler      ; TIM1 Break and TIM9
        DCD     BootTIM1_UP_TIM10_IRQHandler      ; TIM1 Update and TIM10
        DCD     BootTIM1_TRG_COM_TIM11_IRQHandler ; TIM1 Trigger and Commutation and TIM11
        DCD     BootTIM1_CC_IRQHandler            ; TIM1 Capture Compare
        DCD     BootTIM2_IRQHandler               ; TIM2
        DCD     BootTIM3_IRQHandler               ; TIM3
        DCD     BootTIM4_IRQHandler               ; TIM4
        DCD     BootI2C1_EV_IRQHandler            ; I2C1 Event
        DCD     BootI2C1_ER_IRQHandler            ; I2C1 Error
        DCD     BootI2C2_EV_IRQHandler            ; I2C2 Event
        DCD     BootI2C2_ER_IRQHandler            ; I2C2 Error
        DCD     BootSPI1_IRQHandler               ; SPI1
        DCD     BootSPI2_IRQHandler               ; SPI2
        DCD     BootUSART1_IRQHandler             ; USART1
        DCD     BootUSART2_IRQHandler             ; USART2
        DCD     BootUSART3_IRQHandler             ; USART3
        DCD     BootEXTI15_10_IRQHandler          ; External Line[15:10]s
        DCD     BootRTC_Alarm_IRQHandler          ; RTC Alarm (A and B) through EXTI Line
        DCD     BootOTG_FS_WKUP_IRQHandler        ; USB OTG FS Wakeup through EXTI line
        DCD     BootTIM8_BRK_TIM12_IRQHandler     ; TIM8 Break and TIM12
        DCD     BootTIM8_UP_TIM13_IRQHandler      ; TIM8 Update and TIM13
        DCD     BootTIM8_TRG_COM_TIM14_IRQHandler ; TIM8 Trigger and Commutation and TIM14
        DCD     BootTIM8_CC_IRQHandler            ; TIM8 Capture Compare
        DCD     BootDMA1_Stream7_IRQHandler       ; DMA1 Stream7
        DCD     BootFSMC_IRQHandler               ; FSMC
        DCD     BootSDIO_IRQHandler               ; SDIO
        DCD     BootTIM5_IRQHandler               ; TIM5
        DCD     BootSPI3_IRQHandler               ; SPI3
        DCD     BootUART4_IRQHandler              ; UART4
        DCD     BootUART5_IRQHandler              ; UART5
        DCD     BootTIM6_DAC_IRQHandler           ; TIM6 and DAC1&2 underrun errors
        DCD     BootTIM7_IRQHandler               ; TIM7
        DCD     BootDMA2_Stream0_IRQHandler       ; DMA2 Stream 0
        DCD     BootDMA2_Stream1_IRQHandler       ; DMA2 Stream 1
        DCD     BootDMA2_Stream2_IRQHandler       ; DMA2 Stream 2
        DCD     BootDMA2_Stream3_IRQHandler       ; DMA2 Stream 3
        DCD     BootDMA2_Stream4_IRQHandler       ; DMA2 Stream 4
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     BootCAN2_TX_IRQHandler            ; CAN2 TX
        DCD     BootCAN2_RX0_IRQHandler           ; CAN2 RX0
        DCD     BootCAN2_RX1_IRQHandler           ; CAN2 RX1
        DCD     BootCAN2_SCE_IRQHandler           ; CAN2 SCE
        DCD     OTG_FS_IRQHandler                 ; USB OTG FS
        DCD     BootDMA2_Stream5_IRQHandler       ; DMA2 Stream 5
        DCD     BootDMA2_Stream6_IRQHandler       ; DMA2 Stream 6
        DCD     BootDMA2_Stream7_IRQHandler       ; DMA2 Stream 7
        DCD     BootUSART6_IRQHandler             ; USART6
        DCD     BootI2C3_EV_IRQHandler            ; I2C3 event
        DCD     BootI2C3_ER_IRQHandler            ; I2C3 error
        DCD     BootOTG_HS_EP1_OUT_IRQHandler     ; USB OTG HS End Point 1 Out
        DCD     BootOTG_HS_EP1_IN_IRQHandler      ; USB OTG HS End Point 1 In
        DCD     BootOTG_HS_WKUP_IRQHandler        ; USB OTG HS Wakeup through EXTI
        DCD     BootOTG_HS_IRQHandler             ; USB OTG HS
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     BootHASH_RNG_IRQHandler           ; Hash and RNG

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler

        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK BootNMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootNMI_Handler
        B BootNMI_Handler

        PUBWEAK BootHardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootHardFault_Handler
        B BootHardFault_Handler

        PUBWEAK BootMemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootMemManage_Handler
        B BootMemManage_Handler

        PUBWEAK BootBusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootBusFault_Handler
        B BootBusFault_Handler

        PUBWEAK BootUsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootUsageFault_Handler
        B BootUsageFault_Handler

        PUBWEAK BootSVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootSVC_Handler
        B BootSVC_Handler

        PUBWEAK BootDebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDebugMon_Handler
        B BootDebugMon_Handler

        PUBWEAK BootPendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootPendSV_Handler
        B BootPendSV_Handler

        PUBWEAK BootSysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootSysTick_Handler
        B BootSysTick_Handler

        PUBWEAK BootWWDG_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootWWDG_IRQHandler  
        B BootWWDG_IRQHandler

        PUBWEAK BootPVD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootPVD_IRQHandler  
        B BootPVD_IRQHandler

        PUBWEAK BootTAMP_STAMP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTAMP_STAMP_IRQHandler  
        B BootTAMP_STAMP_IRQHandler

        PUBWEAK BootRTC_WKUP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootRTC_WKUP_IRQHandler  
        B BootRTC_WKUP_IRQHandler

        PUBWEAK BootFLASH_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootFLASH_IRQHandler  
        B BootFLASH_IRQHandler

        PUBWEAK BootRCC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootRCC_IRQHandler  
        B BootRCC_IRQHandler

        PUBWEAK BootEXTI0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootEXTI0_IRQHandler  
        B BootEXTI0_IRQHandler

        PUBWEAK BootEXTI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootEXTI1_IRQHandler  
        B BootEXTI1_IRQHandler

        PUBWEAK BootEXTI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootEXTI2_IRQHandler  
        B BootEXTI2_IRQHandler

        PUBWEAK BootEXTI3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootEXTI3_IRQHandler
        B BootEXTI3_IRQHandler

        PUBWEAK BootEXTI4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootEXTI4_IRQHandler  
        B BootEXTI4_IRQHandler

        PUBWEAK BootDMA1_Stream0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA1_Stream0_IRQHandler  
        B BootDMA1_Stream0_IRQHandler

        PUBWEAK BootDMA1_Stream1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA1_Stream1_IRQHandler  
        B BootDMA1_Stream1_IRQHandler

        PUBWEAK BootDMA1_Stream2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA1_Stream2_IRQHandler  
        B BootDMA1_Stream2_IRQHandler

        PUBWEAK BootDMA1_Stream3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA1_Stream3_IRQHandler  
        B BootDMA1_Stream3_IRQHandler

        PUBWEAK BootDMA1_Stream4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA1_Stream4_IRQHandler  
        B BootDMA1_Stream4_IRQHandler

        PUBWEAK BootDMA1_Stream5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA1_Stream5_IRQHandler  
        B BootDMA1_Stream5_IRQHandler

        PUBWEAK BootDMA1_Stream6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA1_Stream6_IRQHandler  
        B BootDMA1_Stream6_IRQHandler

        PUBWEAK BootADC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootADC_IRQHandler  
        B BootADC_IRQHandler

        PUBWEAK BootCAN1_TX_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootCAN1_TX_IRQHandler  
        B BootCAN1_TX_IRQHandler

        PUBWEAK BootCAN1_RX0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootCAN1_RX0_IRQHandler  
        B BootCAN1_RX0_IRQHandler

        PUBWEAK BootCAN1_RX1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootCAN1_RX1_IRQHandler  
        B BootCAN1_RX1_IRQHandler

        PUBWEAK BootCAN1_SCE_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootCAN1_SCE_IRQHandler  
        B BootCAN1_SCE_IRQHandler

        PUBWEAK BootEXTI9_5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootEXTI9_5_IRQHandler  
        B BootEXTI9_5_IRQHandler

        PUBWEAK BootTIM1_BRK_TIM9_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM1_BRK_TIM9_IRQHandler  
        B BootTIM1_BRK_TIM9_IRQHandler

        PUBWEAK BootTIM1_UP_TIM10_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM1_UP_TIM10_IRQHandler  
        B BootTIM1_UP_TIM10_IRQHandler

        PUBWEAK BootTIM1_TRG_COM_TIM11_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM1_TRG_COM_TIM11_IRQHandler  
        B BootTIM1_TRG_COM_TIM11_IRQHandler
        
        PUBWEAK BootTIM1_CC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM1_CC_IRQHandler  
        B BootTIM1_CC_IRQHandler

        PUBWEAK BootTIM2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM2_IRQHandler  
        B BootTIM2_IRQHandler

        PUBWEAK BootTIM3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM3_IRQHandler  
        B BootTIM3_IRQHandler

        PUBWEAK BootTIM4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM4_IRQHandler  
        B BootTIM4_IRQHandler

        PUBWEAK BootI2C1_EV_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootI2C1_EV_IRQHandler  
        B BootI2C1_EV_IRQHandler

        PUBWEAK BootI2C1_ER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootI2C1_ER_IRQHandler  
        B BootI2C1_ER_IRQHandler

        PUBWEAK BootI2C2_EV_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootI2C2_EV_IRQHandler  
        B BootI2C2_EV_IRQHandler

        PUBWEAK BootI2C2_ER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootI2C2_ER_IRQHandler  
        B BootI2C2_ER_IRQHandler

        PUBWEAK BootSPI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootSPI1_IRQHandler  
        B BootSPI1_IRQHandler

        PUBWEAK BootSPI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootSPI2_IRQHandler  
        B BootSPI2_IRQHandler

        PUBWEAK BootUSART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootUSART1_IRQHandler  
        B BootUSART1_IRQHandler

        PUBWEAK BootUSART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootUSART2_IRQHandler  
        B BootUSART2_IRQHandler

        PUBWEAK BootUSART3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootUSART3_IRQHandler  
        B BootUSART3_IRQHandler

        PUBWEAK BootEXTI15_10_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootEXTI15_10_IRQHandler  
        B BootEXTI15_10_IRQHandler

        PUBWEAK BootRTC_Alarm_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootRTC_Alarm_IRQHandler  
        B BootRTC_Alarm_IRQHandler

        PUBWEAK BootOTG_FS_WKUP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootOTG_FS_WKUP_IRQHandler  
        B BootOTG_FS_WKUP_IRQHandler
      
        PUBWEAK BootTIM8_BRK_TIM12_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM8_BRK_TIM12_IRQHandler  
        B BootTIM8_BRK_TIM12_IRQHandler

        PUBWEAK BootTIM8_UP_TIM13_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM8_UP_TIM13_IRQHandler  
        B BootTIM8_UP_TIM13_IRQHandler

        PUBWEAK BootTIM8_TRG_COM_TIM14_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM8_TRG_COM_TIM14_IRQHandler  
        B BootTIM8_TRG_COM_TIM14_IRQHandler

        PUBWEAK BootTIM8_CC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM8_CC_IRQHandler  
        B BootTIM8_CC_IRQHandler

        PUBWEAK BootDMA1_Stream7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA1_Stream7_IRQHandler  
        B BootDMA1_Stream7_IRQHandler

        PUBWEAK BootFSMC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootFSMC_IRQHandler  
        B BootFSMC_IRQHandler

        PUBWEAK BootSDIO_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootSDIO_IRQHandler  
        B BootSDIO_IRQHandler

        PUBWEAK BootTIM5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM5_IRQHandler  
        B BootTIM5_IRQHandler

        PUBWEAK BootSPI3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootSPI3_IRQHandler  
        B BootSPI3_IRQHandler

        PUBWEAK BootUART4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootUART4_IRQHandler  
        B BootUART4_IRQHandler

        PUBWEAK BootUART5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootUART5_IRQHandler  
        B BootUART5_IRQHandler

        PUBWEAK BootTIM6_DAC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM6_DAC_IRQHandler  
        B BootTIM6_DAC_IRQHandler

        PUBWEAK BootTIM7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootTIM7_IRQHandler  
        B BootTIM7_IRQHandler

        PUBWEAK BootDMA2_Stream0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA2_Stream0_IRQHandler  
        B BootDMA2_Stream0_IRQHandler

        PUBWEAK BootDMA2_Stream1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA2_Stream1_IRQHandler  
        B BootDMA2_Stream1_IRQHandler

        PUBWEAK BootDMA2_Stream2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA2_Stream2_IRQHandler  
        B BootDMA2_Stream2_IRQHandler

        PUBWEAK BootDMA2_Stream3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA2_Stream3_IRQHandler  
        B BootDMA2_Stream3_IRQHandler

        PUBWEAK BootDMA2_Stream4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA2_Stream4_IRQHandler  
        B BootDMA2_Stream4_IRQHandler

        PUBWEAK BootCAN2_TX_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootCAN2_TX_IRQHandler  
        B BootCAN2_TX_IRQHandler

        PUBWEAK BootCAN2_RX0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootCAN2_RX0_IRQHandler  
        B BootCAN2_RX0_IRQHandler

        PUBWEAK BootCAN2_RX1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootCAN2_RX1_IRQHandler  
        B BootCAN2_RX1_IRQHandler

        PUBWEAK BootCAN2_SCE_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootCAN2_SCE_IRQHandler  
        B BootCAN2_SCE_IRQHandler

        PUBWEAK OTG_FS_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
OTG_FS_IRQHandler  
        B OTG_FS_IRQHandler

        PUBWEAK BootDMA2_Stream5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA2_Stream5_IRQHandler  
        B BootDMA2_Stream5_IRQHandler

        PUBWEAK BootDMA2_Stream6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA2_Stream6_IRQHandler  
        B BootDMA2_Stream6_IRQHandler

        PUBWEAK BootDMA2_Stream7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootDMA2_Stream7_IRQHandler  
        B BootDMA2_Stream7_IRQHandler

        PUBWEAK BootUSART6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootUSART6_IRQHandler  
        B BootUSART6_IRQHandler

        PUBWEAK BootI2C3_EV_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootI2C3_EV_IRQHandler  
        B BootI2C3_EV_IRQHandler

        PUBWEAK BootI2C3_ER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootI2C3_ER_IRQHandler  
        B BootI2C3_ER_IRQHandler

        PUBWEAK BootOTG_HS_EP1_OUT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootOTG_HS_EP1_OUT_IRQHandler  
        B BootOTG_HS_EP1_OUT_IRQHandler

        PUBWEAK BootOTG_HS_EP1_IN_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootOTG_HS_EP1_IN_IRQHandler  
        B BootOTG_HS_EP1_IN_IRQHandler

        PUBWEAK BootOTG_HS_WKUP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootOTG_HS_WKUP_IRQHandler  
        B BootOTG_HS_WKUP_IRQHandler

        PUBWEAK BootOTG_HS_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootOTG_HS_IRQHandler  
        B BootOTG_HS_IRQHandler

        PUBWEAK BootHASH_RNG_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BootHASH_RNG_IRQHandler  
        B BootHASH_RNG_IRQHandler

        END
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
