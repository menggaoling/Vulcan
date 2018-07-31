/*
*
*
*/

#include  "stm32f10x_conf.h"
#include  "cab.h"
#include  "keyboard.h"


#define   CAB_UART                UART4
//#define   CAB_UART_IRQn           UART4_IRQn
//#define   CAB_UART_BUADRATE       38400

#define _CabMaxCmdBufferSize            20
#define _CabProtocolMaxSize             10
#define _NullCommand                    0xFF

const unsigned char CabCommandTable[26] = {'0','1','2','3','4','5','6','7','8','9','B','C','D','E','F','G','H','I','J','K','L','M','Q','R','S','Z'} ;
const unsigned char KeyChangeToCabCommand[20][2] = {
  {K_TV_0,      _CAB_DIGIT_0} ,
  {K_TV_1,      _CAB_DIGIT_1} ,
  {K_TV_2,      _CAB_DIGIT_2} ,
  {K_TV_3,      _CAB_DIGIT_3} ,
  {K_TV_4,      _CAB_DIGIT_4} ,
  {K_TV_5,      _CAB_DIGIT_5} ,
  {K_TV_6,      _CAB_DIGIT_6} ,
  {K_TV_7,      _CAB_DIGIT_7} ,
  {K_TV_8,      _CAB_DIGIT_8} ,
  {K_TV_9,      _CAB_DIGIT_9} ,
  {K_TV_VD,     _CAB_VOL_DOWN} ,
  {K_TV_VU,     _CAB_VOL_UP} ,
  {K_TV_CD,     _CAB_CH_DOWN} ,
  {K_TV_CU,     _CAB_CH_UP} ,
  {K_TV_POWER,  _CAB_POWER_TOGGLE} ,
  {K_TV_LAST,   _CAB_PREVIOUS_CH} ,
  {K_Mute,      _CAB_MUTE} ,
  {K_CC,        _CAB_CC} ,
  {K_TV_ENTER,  _CAB_ENTER},
  {K_TV_SUB,    _CAB_DASH}
} ;



__no_init unsigned char CabCommandBuffer[_CabMaxCmdBufferSize] ;
__no_init unsigned char CabSendBuffer[_CabProtocolMaxSize] ;
__no_init unsigned char CabInBufferIndex ;
__no_init unsigned char CabOutBufferIndex ;
__no_init unsigned char CabSendIndex ;
__no_init unsigned char CabSendLength ;
__no_init volatile unsigned char CabSendEnd ;

/**
**
**/
void CAB_UartInitial(void)
{
  //USART_InitTypeDef 	USART_InitStructure;
  //NVIC_InitTypeDef 	NVIC_InitStructure;
  
  //
  for( CabOutBufferIndex = 0 ; CabOutBufferIndex < _CabMaxCmdBufferSize ; CabOutBufferIndex++ )
      CabCommandBuffer[CabOutBufferIndex] = _NullCommand ;
  
  for( CabOutBufferIndex = 0 ; CabOutBufferIndex < _CabProtocolMaxSize ; CabOutBufferIndex++ )
      CabSendBuffer[CabOutBufferIndex] = _CAB_END ;
  //
  CabInBufferIndex = 0 ;
  CabOutBufferIndex = 0 ;     
  CabSendIndex = 0 ;
  CabSendLength = 0 ;
  CabSendEnd = 1 ;  
  //
  /*
  if( Mode == 1 )
      {
      // Disable Uart All 
      NVIC_InitStructure.NVIC_IRQChannel = CAB_UART_IRQn;//Channel;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
      NVIC_Init(&NVIC_InitStructure);	        
      
      USART_Cmd(CAB_UART, DISABLE);
      USART_ITConfig(CAB_UART, USART_IT_TXE, DISABLE);
      USART_ITConfig(CAB_UART, USART_IT_RXNE, DISABLE);
      USART_ITConfig(CAB_UART, USART_IT_TC, DISABLE);  
    
      // Configure CAB_UART   
      USART_InitStructure.USART_BaudRate = CAB_UART_BUADRATE;
      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
      USART_InitStructure.USART_StopBits = USART_StopBits_1;
      USART_InitStructure.USART_Parity = USART_Parity_No;
      USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
      USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
      USART_Init(CAB_UART, &USART_InitStructure);	
      USART_Cmd(CAB_UART, ENABLE);	
      //
      // Enable the CAB_UART Interrupt 
      NVIC_InitStructure.NVIC_IRQChannel = CAB_UART_IRQn;//Channel;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);	
      //
      }
  */
  return ;
}


/**
**
**/
void CAB_TxRx_Information(void)
{
  //    
  if(USART_GetITStatus(CAB_UART, USART_IT_TXE) != RESET)//ÅÐ¶ÏÊÇ·ñ·¢ËÍÖÐ¶Ï
      {
      /* Clear the CAB_UART transmit interrupt */
      USART_ClearITPendingBit(CAB_UART, USART_IT_TXE);      
      /* Write one byte to the transmit data register */
      if( CabSendIndex < CabSendLength )
          {
          USART_SendData(CAB_UART,CabSendBuffer[CabSendIndex]);   
          CabSendIndex += 1 ;
          }
      else
          {
          CabSendIndex = 0 ;
          CabSendEnd = 1 ;
          CabCommandBuffer[CabOutBufferIndex] = _NullCommand ;
          CabOutBufferIndex += 1 ;
          if( CabOutBufferIndex >= _CabMaxCmdBufferSize )
              CabOutBufferIndex =  0 ; 
          USART_ITConfig(CAB_UART, USART_IT_TXE, DISABLE);
          }
      }
  //
  USART_ClearITPendingBit(CAB_UART, USART_IT_RXNE); 
  //
  return ;
}


/**
**
**/
void CAB_SendCommand(unsigned char Key)
{
  unsigned char i ;
  unsigned char TempCmd = 0xFF ;
  for( i = 0 ; i < 20 ; i++ )
      {
      if( KeyChangeToCabCommand[i][0] == Key )
          {
          TempCmd = KeyChangeToCabCommand[i][1] ;
          break ;
          }
      }
  //
  if( TempCmd != 0xFF )
      {
      CAB_CommandToBuffer(TempCmd) ;
      }
  //  
  return ;
}


/**
**
**/
void CAB_CommandToBuffer(unsigned char cmd)
{
  //if( CAB_CommandEmpty() == 1 )
  //    {
  CabCommandBuffer[CabInBufferIndex] = cmd ;
  CabInBufferIndex += 1 ;
  if( CabInBufferIndex >= _CabMaxCmdBufferSize )
      CabInBufferIndex =  0 ;        
  //    }     
  return ;
}

      
/**
**
**/      
unsigned char CAB_CommandEmpty(void)
{
  if( CabInBufferIndex == CabOutBufferIndex )
      return 1 ;
  //
  if( CabInBufferIndex > CabOutBufferIndex )
      {
      if( CabOutBufferIndex == 0 && CabInBufferIndex < (_CabMaxCmdBufferSize-1))
          {
          return 1 ;
          }
      }
  //
  if( CabInBufferIndex < CabOutBufferIndex )
      {
      if( CabInBufferIndex <= (CabOutBufferIndex-1) )
          return 1 ;
      }
  //
  return 0 ;
}
      

/**
**
**/    
void CAB_CommandProcess(void)
{
  //
  if( CabCommandBuffer[CabOutBufferIndex] != _NullCommand )
      {
      if( CabSendEnd == 1 )
          {
          CabSendIndex = 0 ;
          CabSendEnd = 0 ;

          CabSendLength = 3 ;
          CabSendBuffer[0] = _CAB_START ;
          CabSendBuffer[1] = CabCommandTable[CabCommandBuffer[CabOutBufferIndex]] ;
          CabSendBuffer[2] = _CAB_END ;
          CabSendBuffer[3] = 0 ;

          USART_ITConfig(CAB_UART, USART_IT_TXE, ENABLE);
          }
      }    
    //
  return ;
}