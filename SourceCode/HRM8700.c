#include "HRM8700.h"

#define TX_BUFFER_SIZE 25
#define RX_BUFFER_SIZE 16


typedef struct {
  unsigned char Length ;
  unsigned char Command ;
  unsigned char State ;
  unsigned char *Data ;
  SM4200_CrC16  CRC16 ; 
} HR_Device ;

u8 TV_TXBuffer[TX_BUFFER_SIZE];
u8 TV_RXBuffer[RX_BUFFER_SIZE];

void HR_UartTxRx_Information(void)
{
    
    if(__HAL_UART_GET_IT_SOURCE(&huart6, UART_IT_TXE) != RESET && __HAL_UART_GET_FLAG(&huart6,UART_FLAG_TXE) != RESET)
    {//==>判断是否发送中断
        /* Clear the USART6 transmit interrupt */
        __HAL_UART_CLEAR_FLAG(&huart6, UART_FLAG_TXE);
        if(by_TV_TXByte >= TV_ComBuffer[by_TV_TXEnd][2]+4)
        {
        
            /* Disable the USART4 Transmit interrupt */
            __HAL_UART_DISABLE_IT(&huart6,UART_IT_TXE);
            __HAL_UART_ENABLE_IT(&huart6,UART_IT_RXNE);
        }
        else
        {
            /* Write one byte to the transmit data register */
            huart6.Instance->DR = (TV_ComBuffer[by_TV_TXEnd][by_TV_TXByte]) & 0xFF;
            by_TV_TXByte++;
        }
    }
    if(__HAL_UART_GET_IT_SOURCE(&huart6, UART_IT_RXNE) != RESET && __HAL_UART_GET_FLAG(&huart6,UART_FLAG_RXNE) != RESET)
    {//==>判断是否接收中断
        /* Clear the USART1 Receive interrupt */
        __HAL_UART_CLEAR_FLAG(&huart6, UART_FLAG_RXNE);
        /* Read one byte from the receive data register */
        TV_RXBuffer[by_TV_RX] = (uint8_t)(huart6.Instance->DR & (uint16_t)0x00FF);
        
        if(TV.RX)
        {           
            if(TV_RXBuffer[0] == 0x82)
            {
                by_TV_RX++;
                if(by_TV_RX >= 5)
                {
                    TV.RX_Check=1;
                    TV.RX=0;
                    by_TV_RX=0;
                    _TryCommand = 0;
                    __HAL_UART_DISABLE_IT(&huart6,UART_IT_RXNE);
                }
            }
        }    
    }
}