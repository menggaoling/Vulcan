/*==============================
====      HT1381Ê±¼äIC      ====
================================*/


#include "HT1381.h"
#include "stm32_reg.h"



//=================================================================
//====> RTC ¿ØÖÆ½Å
//=================================================================
#define IO_Set_HT1381_SCK()       {HCC_GPIO_BSRR(C)=(1u<<2);}
#define IO_Clear_HT1381_SCK()     {HCC_GPIO_BRR(C)=(1u<<2);}
#define IO_Set_HT1381_Data()      {HCC_GPIO_BSRR(C)=(1u<<3);}
#define IO_Clear_HT1381_Data()    {HCC_GPIO_BRR(C)=(1u<<3);}
#define IO_Set_HT1381_RST()       {HCC_GPIO_BSRR(C)=(1u);}
#define IO_Clear_HT1381_RST()     {HCC_GPIO_BRR(C)=(1u);}
#define IO_Read_HT1381_Data()     HCC_GPIO_IDR(C)>>3&0x01

/*******************************************************************************
* Function Name  : Delay_Time
* Description    : Delay
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_Time(void)
{
    unsigned short i;
    for(i = 0;i < 1000;i++) ;      
}


__root void HT1381_HWInitial(void)
{
  // PC0 Output
  // PC2 Output
  // PC3 Input
  HCC_RCC_APB2ENR|=HCC_V(IOPC,1);	/* enable PORTC */
  HCC_GPIO_CRL(C)&=(0xffff00f0);
  HCC_GPIO_CRL(C)|=(0x00008303);		
  IO_Set_HT1381_Data() ;
}

__root void SubWriteHT1381(unsigned char PU8_Data)
{
    unsigned char  LU8_Bit;
    for( LU8_Bit = 0; LU8_Bit < 8; LU8_Bit++ )
    {
        IO_Clear_HT1381_Data();
        if(PU8_Data & (unsigned char)0x01)
        {
            IO_Set_HT1381_Data();
        }    
        IO_Set_HT1381_SCK();
        PU8_Data >>= 1;
        IO_Clear_HT1381_SCK();
    }
}


__root void ReadHT1381(SHT1381 *ReadBuffer)
{
  unsigned char LU8_Data;
  unsigned char LU8_Bit, LU8_Byte;
  unsigned char* LBuffer;
  
  LBuffer = (unsigned char*)ReadBuffer;
  IO_Set_HT1381_RST();
  IO_Clear_HT1381_RST();
  IO_Clear_HT1381_SCK();
  IO_Clear_HT1381_Data();
  HT1381_SetDOUT();
  IO_Set_HT1381_RST();
  SubWriteHT1381( 0xBF );
  HT1381_SetDIN();
  //
  for(LU8_Byte = 0; LU8_Byte < sizeof(SHT1381); LU8_Byte++)
      {
      LU8_Data = 0;
      for(LU8_Bit = 0; LU8_Bit < 8; LU8_Bit++)
          {
          IO_Set_HT1381_SCK();
          LU8_Data >>= 1;
          if(IO_Read_HT1381_Data()) 
              LU8_Data |= 0x80;
          IO_Clear_HT1381_SCK();
          }
      *(LBuffer+LU8_Byte) = LU8_Data;
      }
  IO_Clear_HT1381_RST();
}


__root void WriteHT1381(SHT1381 *ReadBuffer)
{
    unsigned char LU8_Data;
    unsigned char LU8_Byte;
    unsigned char* LBuffer;

    IO_Set_HT1381_RST();
    IO_Clear_HT1381_RST();
    IO_Clear_HT1381_SCK();
    IO_Clear_HT1381_Data();
    HT1381_SetDOUT();
    IO_Set_HT1381_RST();
    SubWriteHT1381(0x8E); // write protect command
    SubWriteHT1381(0x00); // enable write
    IO_Clear_HT1381_RST();
    LBuffer = (unsigned char*)ReadBuffer;
    LU8_Data = 0xBE;
    IO_Set_HT1381_RST();
    for( LU8_Byte = 0; LU8_Byte < sizeof(SHT1381); LU8_Byte++ )
    {
        SubWriteHT1381( LU8_Data );
        LU8_Data = *LBuffer++;
    }
    IO_Clear_HT1381_RST();
    Delay_Time();
    Delay_Time();
    IO_Set_HT1381_RST();
    SubWriteHT1381( 0x8E ); // write protect command
    SubWriteHT1381( 0x80 ); // disable Write
    IO_Clear_HT1381_RST();
}


/*******************************************************************************
* Function Name  : HT1381_SetDIN
* Description    : GPIO ports set to input
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__root void HT1381_SetDIN(void)
{
  // PC0 Output
  // PC2 Output
  // PC3 Input
  HCC_GPIO_CRL(C)&=(0xffff0fff);
  HCC_GPIO_CRL(C)|=(0x00008000);	
  IO_Set_HT1381_Data() ;	
}


/*******************************************************************************
* Function Name  : HT1381_SetDOUT
* Description    : GPIO ports set to output
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__root void HT1381_SetDOUT(void)
{
  // PC0 Output
  // PC2 Output
  // PC3 Output
  HCC_GPIO_CRL(C)&=(0xffff0fff);
  HCC_GPIO_CRL(C)|=(0x00003000);		
}








