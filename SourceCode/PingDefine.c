#include "stm32f2xx_hal.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
        //EXTI_InitTypeDef 		EXTI_InitStructure;	

        
        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        
	/********************* Configure  UART *******************/
	/* Configure USART1 / USART2 Rx (PA.10/PA3) as input floating */
        GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
	/* Configure USART1 / USART2 Tx (PA.09/PA2) as alternate function push-pull */
	GPIO_InitStruct.Pin = GPIO_PIN_2;//;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Configure USART3 Rx (PB11) as input floating */
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Configure USART3 Tx (PB10) as alternate function push-pull */
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
        
        /* PORT D -> pin2:RF_RX */
        GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
        /* 
           PORT C -> pin12:RF_TX 
                     pin10:TV_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        
        /* PORT C -> pin11:TV_RX */
        GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        
        /* ================================================================== */
        /*  PORT A  */
        /*
            pin0 :NC
            pin1 :DIR_485
            pin2 :UART2_TX 
            pin3 :UART2_RX 
            pin4 :EEPROM CS
            pin5 :EEPROM SK
            pin6 :EEPROM DO
            pin7 :EEPROM DI
            pin8 :USB_CTL1 x
            pin9 :USB_CTL2 x
            pin10:USB_CTL3 x
            pin11:USB_DM 
            pin12:USB_DP 
            pin13:TMS/SWDIO (JTAG)
            pin14:TCK/SWDIO (JTAG)
            pin15:TDI       (JTAG) 
        */
	GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7 |\
                                      GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 ;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
        
        GPIO_InitStruct.Pin =  GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);//==>没用到设LOW
        /* ================================================================== */
        /* PORT B */   
        /*  
            pin0 : USB_IAD
            pin1 : NC
            pin2 : DQ5 (Quick key)
            pin3 : TDO (N)
            pin4 : TRST (N)
            pin5 : Safe_Key 
            pin6 : VOL_CTL (PWM) TM4_CH1
            pin7 : NC
            pin8 : I2C_KEY SCL
            pin9 : I2C_KEY SDA
            pin10: UART3_TX (N)
            pin11: UART3_RX (N)
            pin12: LED_OE
            pin13: LED_CLK
            pin14: LED_LE
            pin15: LED_SDI
        */
        
        GPIO_InitStruct.Pin =  GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 |\
                               GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
        
        GPIO_InitStruct.Pin = GPIO_PIN_0 ;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_15;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        
        //==>PWM
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 
        
        /* Configure GPIOB.8 & GPIOB.9 as open-drain output */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        
        /* ==================================================================*/
        /* PORT C */ 
        /*    
            pin0 : RST_1381
            pin1 : CSAFE_CTS
            pin2 : SCK_1381
            pin3 : I/O 1381
            pin4 : DQ1 (Quick key)
            pin5 : DQ2 (Quick key)
            pin6 : HR TX
            pin7 : HR RX
            pin8 : BURN_IN
            pin9 : nSHUTDOWN
            pin10: TV_RX (N)
            pin11: TV_TX (N)
            pin12: RF_TX (N)
            pin13: NC
            pin14: NC
            pin15: NC
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_4 |\
                              GPIO_PIN_5 |GPIO_PIN_9 | GPIO_PIN_13| GPIO_PIN_14|\
                              GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
       
        GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_6 |\
                                      GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        
        // NA
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,GPIO_PIN_RESET);
        //
        
        /* ==================================================================*/
        /* PORT D */
        /* 
           pin0 : WiFi_nREADY
           pin1 : WiFi_nRESET
           pin2 : WiFi_RX (N)
           pin3 : WiFi_RTS
           pin4 : 
           pin5 : 
           pin6 : Safe_key
           pin7 : BEEP
           pin8 : NC
           pin9 : NC
           pin10: I2C_KEY RST
           pin11: I2C_KEY INT
           pin12: NC
           pin13: NC
           pin14: NC
           pin15: NC
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6  | GPIO_PIN_8  | GPIO_PIN_9 |\
                              GPIO_PIN_12 | GPIO_PIN_13| GPIO_PIN_14 |\
                              GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_4  | GPIO_PIN_5 | GPIO_PIN_7|\
                              GPIO_PIN_10 ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);	
        
        // NA
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4 | GPIO_PIN_5,GPIO_PIN_SET);
        //
        
        /* ================================================================== */
        /* PORT E */
        /* 
           pin0 : 3.3_Power
           pin1 : +12V Power
           pin2 : +5V Power
           pin3 : NA
           pin4 : USB_ERR
           pin5 : USB_EN
           pin6 : USB_LOAD
           pin7 : 
           pin8 ~ pin15 : Key Pad Sacn SK1 ~ SK8  
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2  | \
                                      GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8  | \
                                      GPIO_PIN_9 | GPIO_PIN_10| GPIO_PIN_11| GPIO_PIN_12 | \
                                      GPIO_PIN_13| GPIO_PIN_14| GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);	
        
        GPIO_InitStruct.Pin = GPIO_PIN_6 |GPIO_PIN_4 | GPIO_PIN_3 ;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
        
        //
        //
        
        /* EXTI interrupt init*/
        HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/* TIM init function */
void MX_TIM_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  
  /* ---------------------------------------------------------------
  TIM3 Configuration: Output Compare Inactive Mode:
  TIM3CLK = 60 MHz, Prescaler = (600 - 1), TIM3 counter clock = 100 KHz
  TIM3_CH1 delay = CCR1_Val/TIM3 counter clock  = 100 us
  
  TIM3CLK / Prescaler = TIM3 counter clock
  --------------------------------------------------------------- */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = (600 - 1);
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0xFFFF;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  htim3.State = HAL_TIM_STATE_RESET;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }


  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 100; // 1ms
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

   /* TIM IT enable */
  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1);
  /* TIM3 enable counter */
  HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);
//  HAL_TIM_MspPostInit(&htim3);
  
  /* -----------------------------------------------------------------------
  TIM4 Configuration: generate 1 PWM signals with 1 different duty cycles:
  The TIM4CLK frequency is set to SystemCoreClock (Hz), to get TIM4 counter
  clock at 24 MHz the Prescaler is computed as following:
  - Prescaler = (TIM4CLK / TIM4 counter clock) - 1
  SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
  and Connectivity line devices and to 24 MHz for Low-Density Value line and
  Medium-Density Value line devices
  
  The TIM4 is running at 6 KHz: TIM4 Frequency = TIM4 counter clock/(ARR + 1)
  = 36 MHz / 4000 = 6 KHz
  IM3 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
  ----------------------------------------------------------------------- */  
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 4000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  htim4.State = HAL_TIM_STATE_RESET;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  HAL_TIM_MspPostInit(&htim4);
  /* TIM4 enable counter */
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

}

