#include  "stm32f10x_conf.h"
#include  "Flash.h"

u32 w_DataBuffer[256];
u8 by_Block;

/*******************************************************************************
* Function Name  : Flash_SetBlack
* Description    : 选择要将数据放入的区块,共分A,B两个区块
* Input          : 储存内存区块选择 (Flash_A,Flash_B)
* Output         : None
* Return         : None
*******************************************************************************/
void Flash_SetBlock(u8 by_Dat)
{
    by_Block=by_Dat;
}
/*******************************************************************************
* Function Name  : Flash_ReadDataToBuffer
* Description    : 读出所选择区块中的数据放入暂存中
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Flash_ReadDataToBuffer(void)
{
    u16 x;
    for(x=0;x <= 255;x++)
    {
        w_DataBuffer[x]=Flash_ReadDataCMD(x);
    }
}
/*******************************************************************************
* Function Name  : Flash_Write_Word
* Description    : 将储存数据放入所选择的地址暂存
* Input          : by_Address , by_Dat
* Output         : None
* Return         : None
*******************************************************************************/
void Flash_Write_Word(u8 by_Address,u32 by_Dat)
{
    w_DataBuffer[by_Address]=by_Dat;
}
/*******************************************************************************
* Function Name  : Flash_WriteDataCMD
* Description    : 将储存数据写入所选择的区块
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Flash_WriteDataCMD(void)
{
    u32 FlashAddress;
    u16 x;
    if(by_Block == Flash_A)
    {
        FlashAddress=0x0801F800;//==>第A区块启始位置 (1k fash空间)
    }
    else
    {
        FlashAddress=0x0801FC00;//==>第B区块启始位置 (1k fash空间)
    }
    FLASH_Unlock();
    FLASH_ErasePage(FlashAddress);//==>清除此地址起算的区块 255 个
    for(x=0;x <= 255;x++)
    {/*数据储存ㄧ次4个byte,做256次 (每次储存都是一个区块进行)*/
        FLASH_ProgramWord(FlashAddress,w_DataBuffer[x]);
        FlashAddress+=4;
    }
    FLASH_Lock();
}
/*******************************************************************************
* Function Name  : Flash_ReadDataCMD
* Description    : 读出所选择的区块中其对应地址的信息
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 Flash_ReadDataCMD(u8 FlashAddress)
{
    u32 *BufferSTRS;
    if(by_Block == Flash_A)
    {
        BufferSTRS=(u32 *)0x0801F800;//==>第A区块启始位置 (1k fash空间)
    }
    else
    {
        BufferSTRS=(u32 *)0x0801FC00;//==>第B区块启始位置 (1k fash空间)	
    }
    return BufferSTRS[FlashAddress];
}











