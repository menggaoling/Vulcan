#include  "stm32f10x_conf.h"
#include  "Flash.h"

u32 w_DataBuffer[256];
u8 by_Block;

/*******************************************************************************
* Function Name  : Flash_SetBlack
* Description    : ѡ��Ҫ�����ݷ��������,����A,B��������
* Input          : �����ڴ�����ѡ�� (Flash_A,Flash_B)
* Output         : None
* Return         : None
*******************************************************************************/
void Flash_SetBlock(u8 by_Dat)
{
    by_Block=by_Dat;
}
/*******************************************************************************
* Function Name  : Flash_ReadDataToBuffer
* Description    : ������ѡ�������е����ݷ����ݴ���
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
* Description    : ���������ݷ�����ѡ��ĵ�ַ�ݴ�
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
* Description    : ����������д����ѡ�������
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
        FlashAddress=0x0801F800;//==>��A������ʼλ�� (1k fash�ռ�)
    }
    else
    {
        FlashAddress=0x0801FC00;//==>��B������ʼλ�� (1k fash�ռ�)
    }
    FLASH_Unlock();
    FLASH_ErasePage(FlashAddress);//==>����˵�ַ��������� 255 ��
    for(x=0;x <= 255;x++)
    {/*���ݴ�����4��byte,��256�� (ÿ�δ��涼��һ���������)*/
        FLASH_ProgramWord(FlashAddress,w_DataBuffer[x]);
        FlashAddress+=4;
    }
    FLASH_Lock();
}
/*******************************************************************************
* Function Name  : Flash_ReadDataCMD
* Description    : ������ѡ������������Ӧ��ַ����Ϣ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 Flash_ReadDataCMD(u8 FlashAddress)
{
    u32 *BufferSTRS;
    if(by_Block == Flash_A)
    {
        BufferSTRS=(u32 *)0x0801F800;//==>��A������ʼλ�� (1k fash�ռ�)
    }
    else
    {
        BufferSTRS=(u32 *)0x0801FC00;//==>��B������ʼλ�� (1k fash�ռ�)	
    }
    return BufferSTRS[FlashAddress];
}











