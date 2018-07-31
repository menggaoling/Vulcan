/******************** (C) COPYRIGHT 2007 Hakuto Taiwan LTD., ********************
* File Name          : uart_utl.c
* Author             :
* Date First Issued  : 02/05/2007
* Description        : Main program body
********************************************************************************
* History:
* 02/05/2007: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include  "UART.h"
#include  "General.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart6;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void  UsartInitial(void)
{
        //USART_ClockInitTypeDef  USART_ClockInitStructure ;
        /*######################################################################
          #  UART 1 = RFID 
          #  UART 2 = Digital 
          #  UART 3 = CSAFE 
          #  UART 5 = WiFi
          #  UART 6 = heart Rate
          ######################################################################*/     
        huart2.Instance = USART2;
        huart2.Init.BaudRate = 9600;
        huart2.Init.WordLength = UART_WORDLENGTH_8B;
        huart2.Init.StopBits = UART_STOPBITS_1;
        huart2.Init.Parity = UART_PARITY_NONE;
        huart2.Init.Mode = UART_MODE_TX_RX;
        huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart2.Init.OverSampling = UART_OVERSAMPLING_16;
        huart2.gState = HAL_UART_STATE_RESET;//wake up the MCB need several times,so we forced to init USART2 GPIO
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          _Error_Handler(__FILE__, __LINE__);
        }
        
        huart3.Instance = USART3;
        huart3.Init.BaudRate = 9600;
        huart3.Init.WordLength = UART_WORDLENGTH_8B;
        huart3.Init.StopBits = UART_STOPBITS_1;
        huart3.Init.Parity = UART_PARITY_NONE;
        huart3.Init.Mode = UART_MODE_TX_RX;
        huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart3.Init.OverSampling = UART_OVERSAMPLING_16;
        huart3.gState = HAL_UART_STATE_RESET;
        if (HAL_UART_Init(&huart3) != HAL_OK)
        {
          _Error_Handler(__FILE__, __LINE__);
        }
	
	/* Enable USART1,USART2,USART3 Receive interrupt */
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
	//USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

	//USART_Cmd(USART1, ENABLE);	
//	__HAL_UART_ENABLE(&huart2);	
//	__HAL_UART_ENABLE(&huart3);	
	//USART_Cmd(UART5, ENABLE);	
        
        /*######################################################################
          #  UART 4 default  (TV control) 
          ######################################################################*/        
        huart4.Instance = UART4;
        huart4.Init.BaudRate = 38400;
        huart4.Init.WordLength = UART_WORDLENGTH_8B;
        huart4.Init.StopBits = UART_STOPBITS_1;
        huart4.Init.Parity = UART_PARITY_NONE;
        huart4.Init.Mode = UART_MODE_TX_RX;
        huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart4.Init.OverSampling = UART_OVERSAMPLING_16;
        huart4.gState = HAL_UART_STATE_RESET;
        if (HAL_UART_Init(&huart4) != HAL_OK)
        {
          _Error_Handler(__FILE__, __LINE__);
        }
        
	/* Enable USART1 Receive interrupt */
        //USART_ITConfig(USART4, USART_IT_RXNE, ENABLE);

//        __HAL_UART_ENABLE(&huart4);	
        
        /*######################################################################
          #  UART 6 default  (Heart Rate) 
          ######################################################################*/        
        huart6.Instance = USART6;
        huart6.Init.BaudRate = 9600;
        huart6.Init.WordLength = UART_WORDLENGTH_8B;
        huart6.Init.StopBits = UART_STOPBITS_1;
        huart6.Init.Parity = UART_PARITY_NONE;
        huart6.Init.Mode = UART_MODE_TX_RX;
        huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart6.Init.OverSampling = UART_OVERSAMPLING_16;
        huart6.gState = HAL_UART_STATE_RESET;
        if (HAL_UART_Init(&huart6) != HAL_OK)
        {
          _Error_Handler(__FILE__, __LINE__);
        }

        
}


void UsartInitial_WiFi(void)
{
  /* Configure USART5 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* Enable USART5 Receive interrupt */
  __HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
  
//  __HAL_UART_ENABLE(&huart5);
  
//  /* Enable the USART5 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);		
}





/******************* (C) COPYRIGHT 2007 Hakuto Taiwan LTD., *****END OF FILE****/


//*******************************************************************************************************
//*****************                       C-SAFE 对应用 Function                     ********************
//*******************************************************************************************************
/* // 模块测试先封
static struct
{
    unsigned RX_OK     : 1;
    unsigned RX_Head   : 1;
    unsigned RX_TEST   : 1;
    unsigned FC        : 1;
    unsigned ERROR     : 1;
    unsigned Online    : 1;//==>判断是否有联机用
    unsigned EngTest   : 1;
    unsigned EngCheck  : 1;
}bit;
static u8 *pby_RX_Buffer;
static u8 *pby_TX_Buffer;
static u8 by_RX_Point=0;
static u8 by_TX_Point=0;
static u8 by_TX_Length=0;
void UART_Initial(void)
{
    bit.RX_OK=0;//===>input 判断
    bit.RX_Head=0;
    bit.RX_TEST=0;
    bit.FC=0;
    bit.ERROR=0;
    bit.Online=0;
    bit.EngTest=0;
}
void UART_Set_EngTest(u8 by_Dat)
{//==>搭配工程模式的C-SAFE功能测试用
    bit.EngTest=by_Dat;
    by_RX_Point=0;//==>讯框内部数据地址指针累计
    bit.EngCheck=0;
}
u8 UART_Get_EngTestStatus(void)
{//==>搭配工程模式的C-SAFE功能测试状态结果取得
    return bit.EngCheck;
}
void UART_Write(u8 *pby_Dat,u8 by_Length)
{
    pby_TX_Buffer=pby_Dat;
    by_TX_Length=by_Length;
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//==>USART3中断打开
}
void UART_Set_Read_Memery(u8 *pby_Dat)
{
    pby_RX_Buffer=pby_Dat;
}
u8 UART_Get_RX_OK(void)
{
    if(bit.RX_OK) return(1);
    else return(0);
}
void UART_Clear_RX(void)
{
    bit.RX_TEST=0;
}
void UART_Clear_RX_OK(void)//==>清除讯框结尾指标
{
    bit.RX_OK=0;
}
u8 UART_Get_RX_Point(void)
{
    return(by_RX_Point);
}
u8 UART_Get_ERROR(void)
{
    return(bit.ERROR);
}
u8 UART_Get_RX_Head(void)
{
    return(bit.RX_Head);
}
void UART_Clear_ERROR_Head(void)
{
    bit.ERROR=0;
    bit.RX_Head=0;
}
u8 UART_Get_FC(void)
{
    return(bit.FC);
}
void UART_Set_FC(u8 by_Dat)
{
    bit.FC=by_Dat;
}
u8 UART_XOR_Check(void)
{
    u8 i,j;
    u8 by_Dat=0;
    u8 by_Dat1,by_Dat2;
    u8 by_Buffer;
    u8 by_j;
    if(*(pby_RX_Buffer+(by_RX_Point-2))==0xf3)//==> 取得此讯框中的XOR值是否为F0~F3的特殊字符
    {
        switch(*(pby_RX_Buffer+(by_RX_Point-1)))
        {
            case 0x00:by_Dat1=0xf0;
                      break;
            case 0x01:by_Dat1=0xf1;
                      break;
            case 0x02:by_Dat1=0xf2;
                      break;
            case 0x03:by_Dat1=0xf3;
                      break;
        }
        by_Buffer=by_RX_Point-1;
        by_j=by_RX_Point-2;
    }
    else
    {
        by_Dat1=*(pby_RX_Buffer+(by_RX_Point-1));
        by_Buffer=by_RX_Point;
        by_j=by_RX_Point-1;
    }
    for(i=0;i<by_j;i++)
    {
        if(*(pby_RX_Buffer+i)==0xf3)//---> 作为F0~F3参数转换用
        {
            i++;
            switch(*(pby_RX_Buffer+i))
            {
                case 0x00:by_Dat2=0xf0;
                          break;
                case 0x01:by_Dat2=0xf1;
                          break;
                case 0x02:by_Dat2=0xf2;
                          break;
                case 0x03:by_Dat2=0xf3;
                          break;
            }
            *(pby_RX_Buffer+i-1)=by_Dat2;
            for(j=i;j<by_Buffer;j++)
            {//==>所有参数往右移一位,重F3开始
                *(pby_RX_Buffer+j)=*(pby_RX_Buffer+(j+1));
            }
            by_Buffer--;
            by_j--;
            by_Dat=by_Dat^by_Dat2;
            i--;
        }
        else by_Dat=by_Dat^*(pby_RX_Buffer+i);
    }
    by_RX_Point=by_Buffer;
    if(by_Dat==by_Dat1) return(1);
    else return(0);
}
u8 UART_Get_Online(void)//==>取得Cable线状态是否回传 2004.03.01 modify
{
    return(bit.Online);
}
void UART_Set_Online(u8 by_Dat)//==>设定Cable线联机回传状态 2004.03.01 modify
{
    bit.Online=by_Dat;
}
void UART_TxRx_Information(void)
{
    u8 by_Dat;
    if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)//判断是否发送中断
    {
        //Write one byte to the transmit data register 
        USART_SendData(USART3,*(pby_TX_Buffer+by_TX_Point));
        // Clear the USART3 transmit interrupt 
        USART_ClearITPendingBit(USART3, USART_IT_TXE);
        ++by_TX_Point;
        if(by_TX_Point == by_TX_Length)
        {
            by_TX_Point=0;
            // Disable the USART3 Transmit interrupt 
            USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
        }
    }
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //判断是否接收中断
    {
        // Read one byte from the receive data register 
        by_Dat=USART_ReceiveData(USART3);//==>取得讯框信息
        // Clear the USART3 Receive interrupt 
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        
        if(bit.EngTest)
        {//==>工程模式中的C-SAFE功能测试
            if(by_Dat==0xf1)//==>讯框开始开始
            {
                by_RX_Point=0;//==>讯框内部数据地址指针累计
                bit.EngCheck=0;
            }
            else if(by_Dat==0xf2)//==>讯框结束讯号
            {
                bit.EngTest=0;
                if(by_RX_Point == 2 && *(pby_RX_Buffer+0) == 0x08  && *(pby_RX_Buffer+1) == 0x00)
                {
                    bit.EngCheck=1;                    
                }
            }
            else
            {
                *(pby_RX_Buffer+by_RX_Point)=by_Dat;
                ++by_RX_Point;
            }
        }
        else
        {
            if(!bit.ERROR)
            {
                if(!bit.RX_Head)
                {
                    if(by_Dat==0xf1)//==>讯框开始开始
                    {
                        bit.RX_Head=1;//==>讯框启始指标
                        by_RX_Point=0;//==>讯框内部数据地址指针累计
                    }
                    else bit.ERROR=1;//==>回传错误讯息
                }
                else
                {
                    if(by_Dat==0xf2)//==>讯框结束讯号
                    {
                        if(UART_XOR_Check())
                        {
                            bit.RX_OK=1;//==>讯框结束指标
                            bit.RX_Head=0;
                            if(bit.FC) bit.FC=0;
                            else bit.FC=1;
                        }
                        else bit.ERROR=1;
                    }
                    else
                    {
                        *(pby_RX_Buffer+by_RX_Point)=by_Dat;
                        ++by_RX_Point;
                    }
                    bit.Online=0;//==>当主机有下command时
                }
            }
        }
        
    }
}

*/
