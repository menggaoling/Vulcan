#include "General.h"
#include "TV.h"
#include "led.h"
#include "keyboard.h"
#include "Console.h"
#include "C_safe.h"
#include "EngineerMode.h"
#include "EEPRom.h"
#include "UART.h"

static struct
{
    unsigned int RX       : 1;
    unsigned int RX_Check : 1;
    unsigned int TX       : 1;
    unsigned int POWER    : 1;
    unsigned int POWER_ST : 1;
    unsigned int PO       : 1;
    unsigned int OffLine  : 1;// 100.08.03
}TV;

u8 TV_ComBuffer[10][5];
u8 TV_RXBuffer[5];
u8 TV_Com;
u8 by_TV_TXByte=0;
u8 by_TV_TXStart=0;
u8 by_TV_TXEnd=0;
u8 by_TV_RX=0;
u8 _TryCommand = 0;
u16 by_TVCommandClear=0;


//u8 _CommandTV = 0; 

void TV_Initial(void)
{
    u8 i,j;
    
    for(i = 0;i < 10;i++) 
    {
        for(j = 0;j < 5;j++)
        {
            TV_ComBuffer[i][j]=0;
        }
    }
    TV.OffLine = 0;
    TV.RX=0;
    TV.RX_Check=0;
    TV.TX=0;
    TV.POWER=0;
    TV.POWER_ST=0;
    TV.PO=0;
    by_TV_TXByte=0;
    by_TV_TXStart=0;
    by_TV_TXEnd=0;
    by_TVCommandClear=0;
    TV_SetCommand(TVCom_ReadStatus,0);    
}
/*******************************************************************************
* Function Name  : TV_SetCommand()
* Description    : Command 下达放入堆栈
* Input          : by_COM,by_Dat
* Output         : None
* Return         : None
*******************************************************************************/
void TV_SetCommand(u8 by_COM,u8 by_Dat)
{
    TV_Com=by_COM;
    TV_ComBuffer[by_TV_TXStart][0]=0x02;
    switch(by_COM)
    {
        case TVCom_Number:
        case TVCom_VOL:
        case TVCom_CH:
        case TVCom_POWER:
                 TV_ComBuffer[by_TV_TXStart][1]=by_COM;
                 TV_ComBuffer[by_TV_TXStart][2]=0x01;
                 TV_ComBuffer[by_TV_TXStart][3]=by_Dat;
                 TV_ComBuffer[by_TV_TXStart][4]=TV_ComBuffer[by_TV_TXStart][0]+TV_ComBuffer[by_TV_TXStart][1]+TV_ComBuffer[by_TV_TXStart][2]+TV_ComBuffer[by_TV_TXStart][3];
                 break;   
        case TVCom_ENTER:        
        case TVCom_RESET:
        case TVCom_LAST:
        case TVCom_ReadStatus:
        case TVCom_Mute:
        case TVCom_CC:  
        case TVCom_Aspect:
        case TVCom_Home:
                 TV_ComBuffer[by_TV_TXStart][1]=by_COM;
                 TV_ComBuffer[by_TV_TXStart][2]=0x00;
                 TV_ComBuffer[by_TV_TXStart][3]=TV_ComBuffer[by_TV_TXStart][0]+TV_ComBuffer[by_TV_TXStart][1]+TV_ComBuffer[by_TV_TXStart][2];
                 break;
    }
    by_TV_TXStart++;
    if(by_TV_TXStart > 9) by_TV_TXStart=0;
    if(by_TV_TXStart == by_TV_TXEnd)
    {
        if(by_TV_TXStart == 0)
            by_TV_TXStart=9;
        else 
            by_TV_TXStart--;
    }
    if(!TV.RX && !TV.RX_Check)
    {//==> 可以下Command时
        TV.TX=1;        
    }
}
/*******************************************************************************
* Function Name  : TV_AutoReply()
* Description    : 数据传输与接收处理 (1ms做ㄧ次)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
     u16 by_TVDelay=0;
     u16 by_TVDelay1=0;
     u16 by_TVDelay2=0;
     u16 _PowerCheckDelay = 0;

void TV_AutoReply(void)
{  
    
    if(TV.TX)
    {
        if(by_TVDelay > 350)
        {//==> 350ms后下command
            TV.TX=0;            
            by_TV_RX = 0;
            by_TVDelay=0;
            __HAL_UART_ENABLE_IT(&huart4,UART_IT_TXE);//==>USART4中断打开
        }
        else
            by_TVDelay++;
        by_TVDelay1=0;
    }
    else
    {
        if(by_TVDelay1 > 1100)
        {//==> 1100ms后下command
            by_TVDelay1=0;
            TV_SetCommand(TVCom_ReadStatus,0);
        }
        else
            by_TVDelay1++;
        if(!TV.PO && TV.POWER_ST)
        {
            if(by_TVDelay2 > 10000)
            {//==> 10`s后下command
                TV.PO=1;
                by_TVDelay2=0;
                // 预设台号
//                TV_SetCommand(TVCom_Number,EEPROM_ReadInformation(EE_CHANNEL)/10);
//                TV_SetCommand(TVCom_Number,EEPROM_ReadInformation(EE_CHANNEL)%10);
                //
            }
            else
                by_TVDelay2++;
        }
        else 
            by_TVDelay2=0;
        by_TVDelay=0;
    }
    
    if(_PowerCheckDelay > 10000)  
    {
        TV_Initial();
        TV.OffLine = 1;
        _PowerCheckDelay = 0;
        Led_Set_Target(Target_TV_ON,0);
        Led_Set_Target(Target_TV_OFF,1);
    }
    else _PowerCheckDelay++;
    if(!TV.RX && TV.RX_Check)
    {//==>接收数据完成准备进行接收数据的处理  
        TV.OffLine = 0;
        _PowerCheckDelay = 0;
        TV_RxAssay();
        by_TV_TXEnd++;
        if(by_TV_TXEnd > 9) by_TV_TXEnd=0;
        if(by_TV_TXEnd != by_TV_TXStart) 
        {//==>当还有Command未传出时
            TV.TX=1;
        }
        TV.RX_Check=0;   
    }
    else if(TV.RX)
    {//==>如果TX数据传送后500ms无回复3次后就移除command
        by_TVCommandClear++;
        if(by_TVCommandClear > 500)
        {
            by_TVCommandClear=0;
            _TryCommand++;
            if(_TryCommand >= 3)
            {// 尝试3次后就排除command
                _TryCommand = 0;
                
                if(by_TV_TXEnd == by_TV_TXStart) 
                {
                    TV.RX = 0;
                    TV.RX_Check = 0;
                }
                else
                {
                    by_TV_TXEnd++;
                    if(by_TV_TXEnd > 9) by_TV_TXEnd=0;
                    if(by_TV_TXEnd != by_TV_TXStart) 
                    {//==>当还有Command未传出时就继续传送
                        TV.TX = 1;
                        TV.RX = 0;
                    }
                }
            }
            else
            {
                TV.TX = 1;
                TV.RX = 0;
            }
        }
    }
}
/*******************************************************************************
* Function Name  : TV_RxAssay()
* Description    : RX 资料分析
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TV_RxAssay(void)
{
    if(TV_RXBuffer[1] == 0xc6)
    {
        if(TV_Com == TVCom_ReadStatus)
        {
            if(TV_RXBuffer[3] & BIT7)
            {//==>取得状态是否为开机
                TV.POWER=1;
                Led_Set_Target(Target_TV_OFF,0);
                Led_Set_Target(Target_TV_ON,1);
                if(!TV.POWER_ST && TV_RXBuffer[3] != 0xe0)
                {
                    TV.POWER_ST=1;
                    TV.PO=0;
                }
                if(Console_Audio(Data_Get,0) == Audio_TV && \
                   EngineerMode_GetMode() == 0 )
                {
                    IO_TV_Audio();// 音源
                    IO_OpenAudio();//==>SHUTDOWN
                    TIM4->CCR1 = Console_GetVolumeNumber(Console_GetUserVolume());
                }
                
            }
            else
            {//==>关机
                TV.POWER=0;
                TV.POWER_ST=0;
                TV.PO=0;
                Led_Set_Target(Target_TV_OFF,1);
                Led_Set_Target(Target_TV_ON,0);
                if(Console_Audio(Data_Get,0) == Audio_TV && \
                   EngineerMode_GetMode() == 0 )
                {
                    TIM4->CCR1 = CCR1_Val_Close;//==> PWM duty调整使用(PB.6)
                    IO_CloseAudio();//==>SHUTDOWN
                }
            }            
        }
    }
}
/*******************************************************************************
* Function Name  : TV_UartTxRx_Information()
* Description    : TV UART TX / RX 数据传输与接收 <放置在USART中断中使用>
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TV_UartTxRx_Information(void)
{
    
    if(__HAL_UART_GET_IT_SOURCE(&huart4, UART_IT_TXE) != RESET && __HAL_UART_GET_FLAG(&huart4,UART_FLAG_TXE) != RESET)
    {//==>判断是否发送中断
        /* Clear the USART4 transmit interrupt */
        __HAL_UART_CLEAR_FLAG(&huart4, UART_FLAG_TXE);
        if(by_TV_TXByte >= TV_ComBuffer[by_TV_TXEnd][2]+4)
        {
            //_CommandTV = TV_ComBuffer[by_TV_TXEnd][1];
            TV_RXBuffer[0] = 0;
            TV_RXBuffer[1] = 0;
            TV_RXBuffer[2] = 0;
            TV_RXBuffer[3] = 0;
            TV_RXBuffer[4] = 0;
            
            TV.RX=1;
            by_TV_TXByte=0;  
            by_TV_RX=0;
            by_TVCommandClear=0;
            /* Disable the USART4 Transmit interrupt */
            __HAL_UART_DISABLE_IT(&huart4,UART_IT_TXE);
            __HAL_UART_ENABLE_IT(&huart4,UART_IT_RXNE);
        }
        else
        {
            /* Write one byte to the transmit data register */
            huart4.Instance->DR = (TV_ComBuffer[by_TV_TXEnd][by_TV_TXByte]) & 0xFF;
            by_TV_TXByte++;
        }
    }
    if(__HAL_UART_GET_IT_SOURCE(&huart4, UART_IT_RXNE) != RESET && __HAL_UART_GET_FLAG(&huart4,UART_FLAG_RXNE) != RESET)
    {//==>判断是否接收中断
        /* Clear the USART1 Receive interrupt */
        __HAL_UART_CLEAR_FLAG(&huart4, UART_FLAG_RXNE);
        /* Read one byte from the receive data register */
        TV_RXBuffer[by_TV_RX] = (uint8_t)(huart4.Instance->DR & (uint16_t)0x00FF);
        
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
                    __HAL_UART_DISABLE_IT(&huart4,UART_IT_RXNE);
                }
            }
        }    
    }
}
/*******************************************************************************
* Function Name  : TV_Key()
* Description    : TV专用按键
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TV_Key(u8 by_Dat)
{
    switch(by_Dat)
    {
        case K_TV_POWER:
                 if(TV.OffLine == 0)//100.08.03
                 {// 非脱机的状态下
                     if(!TV.PO && !TV.POWER)
                     {
                         TV.POWER=1;
                         TV.POWER_ST=0;
                         TV_SetCommand(TVCom_POWER,TVData_PowerOFF);
                     }
                     else
                     {
                         if(TV.PO)
                         {
                             TV.POWER=0;
                             TV.POWER_ST=0;
                             TV.PO=0;
                             TV_SetCommand(TVCom_POWER,TVData_PowerOFF);
                         }
                     } 
                 }
                 break;
                 /*
        case K_TV_0:
        case K_TV_1:
        case K_TV_2:
        case K_TV_3:
        case K_TV_4:
        case K_TV_5:
        case K_TV_6:
        case K_TV_7:
        case K_TV_8:
        case K_TV_9:
                 if(TV.PO) TV_SetCommand(TVCom_Number,by_Dat-K_TV_0);
                 break; 
                 */
        case K_TV_LAST:
                 if(TV.PO) TV_SetCommand(TVCom_LAST,0);
                 break;  
        
        case K_CC:
                 if(TV.PO) TV_SetCommand(TVCom_CC,0);
                 break;  
        case K_Mute:  
                 if(TV.PO) TV_SetCommand(TVCom_Mute,0);
                 break;  
        //case K_Home:
        //         if(TV.PO) TV_SetCommand(TVCom_Home,0);
        //         break;  
        case K_TV_CU | K_MASK:          
        case K_TV_CU:         
                 if(TV.PO) TV_SetCommand(TVCom_CH,TVData_CHUp);
                 break;
        case K_TV_CD | K_MASK:         
        case K_TV_CD:         
                 if(TV.PO) TV_SetCommand(TVCom_CH,TVData_CHDown);
                 break;
        case K_TV_VU | K_MASK:         
        case K_TV_VU:         
                 if(TV.PO) TV_SetCommand(TVCom_VOL,TVData_VOLUp);
                 break;
        case K_TV_VD | K_MASK:         
        case K_TV_VD:         
                 if(TV.PO) TV_SetCommand(TVCom_VOL,TVData_VOLDown);
                 break;
                 
    }
}
/*******************************************************************************
* Function Name  : TV_GetPowe_OnOff()
* Description    : 取得TV是否开机判断
* Input          : None
* Output         : None
* Return         : TV.POWER ; 0=OFF,1=ON
*******************************************************************************/
u8 TV_GetPowe_OnOff(void)
{
    return TV.POWER;
}


