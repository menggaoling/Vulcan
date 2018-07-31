//
#include  "Mx25L1606E.h"

//==============================================================================


//
union {
  struct {
    unsigned char WIP:1 ;
    unsigned char WEL:1 ;
    unsigned char BP:4 ;
    unsigned char RES:1 ;
    unsigned char SRWD:1 ;
  } Bit ;
  unsigned char All ;
} MX_Status ;


unsigned char Mx25L1606EError = 0 ;
unsigned char wLed_Buffer[10]={0}, wData_Buffer[2]={0};
//------------------------------------------------------------------------------
  
void Mx25L1606E_CommandOut( unsigned char Cmd, unsigned long Addr, unsigned char Data ) ;
void Mx25L1606E_ByteOut(unsigned char Data) ;
unsigned char Mx25L1606E_ByteInput(void) ;
unsigned char Mx25L1606E_ReadStatus(void) ;
void Mx25L1606E_BlockProtectionEnable( void ) ;
unsigned char Mx25L1606E_BlockProtectionDisable( void );
void Mx25L1606E_WriteEnable(void);
void Mx25L1606E_WriteDisable(void);
unsigned char Mx25L1606E_WaitBusyFree(void) ;
void Mx25L1606E_InsertDummyCycle( unsigned char dummy_cycle ) ;
void Mx25L1606E_PageProgram( unsigned long MAddr, unsigned short Size, unsigned char *Rptr ) ;




//---------------------------------------------------------------------------------
void  Mx25L1606E_CommandOut( unsigned char Cmd, unsigned long Addr, unsigned char Data )
{
  //
  union {
    struct {
      unsigned char Byte0:8 ;
      unsigned char Byte1:8 ;
      unsigned char Byte2:8 ;
      unsigned char Byte3:8 ;
    } Byte ;
    unsigned long All ;
  } AddrData ;
  //
  AddrData.All = Addr ;
  //
  switch( Cmd ) 
      {
      case  _WRSR_  :// Write Status Register
                    Mx25L1606E_ByteOut(Cmd) ;
                    Mx25L1606E_ByteOut(Data) ;
                    break ;
      case  _CE_    :// Chip Erase
      case  _RDSR_  :// Read Status Register
      case  _WREN_  :// Write Enable
      case  _WRDI_  :// Write Disable
                    Mx25L1606E_ByteOut(Cmd) ;
                    break ;
      case  _RD_    :// Read
      case  _SE_    :// Sector Erase 4Kbyte  x (A12~A16)
      case  _BE_    :// Block Erase 32Kbyte X (A15~A16)                    
      case  _RDID_  :// Read ID
      case  _PP_    :// Page Programmer        
                    Mx25L1606E_ByteOut(Cmd) ;
                    Mx25L1606E_ByteOut(AddrData.Byte.Byte2) ;
                    Mx25L1606E_ByteOut(AddrData.Byte.Byte1) ;
                    Mx25L1606E_ByteOut(AddrData.Byte.Byte0) ;
                    break ;
      case  _DREAD_ :
      case  _FRD_   :          
                    Mx25L1606E_ByteOut(Cmd) ;
                    Mx25L1606E_ByteOut(AddrData.Byte.Byte2) ;
                    Mx25L1606E_ByteOut(AddrData.Byte.Byte1) ;
                    Mx25L1606E_ByteOut(AddrData.Byte.Byte0) ;
                    Mx25L1606E_InsertDummyCycle(8) ;
                    break ;
      default       :
                    break ;
      }   
  return ;
}


//------------------------------------------------------------------------------
void Mx25L1606E_ByteOut(unsigned char Data)
{
  //
  union {
    struct {
      unsigned char B0:1 ;
      unsigned char B1:1 ;
      unsigned char B2:1 ;
      unsigned char B3:1 ;
      unsigned char B4:1 ;
      unsigned char B5:1 ;
      unsigned char B6:1 ;
      unsigned char B7:1 ;
    } Bit ;
    unsigned char All ;
  } OutData ;
  unsigned char i ;
  OutData.All = Data ;
  //
  for( i = 0 ; i < 8 ; i++ )  
    {
    if( OutData.Bit.B7 == 1 )
        {
        IO_Set_EEPROM_DI();
        }
    else
        {
        IO_Clear_EEPROM_DI();
        }
    //
    IO_Clear_EEPROM_SK();
    asm("nop") ;
    OutData.All = OutData.All << 1 ;
    IO_Set_EEPROM_SK();
    asm("nop") ;
    }
  //
  return ;
}

//------------------------------------------------------------------------------
unsigned char Mx25L1606E_ByteInput(void)
{
  //
  union {
    struct {
      unsigned char B0:1 ;
      unsigned char B1:1 ;
      unsigned char B2:1 ;
      unsigned char B3:1 ;
      unsigned char B4:1 ;
      unsigned char B5:1 ;
      unsigned char B6:1 ;
      unsigned char B7:1 ;
    } Bit ;
    unsigned char All ;
  } RData ;
  //
  RData.All = 0 ;
  unsigned char i ;
  //
  for( i = 0 ; i < 8 ; i++ )
      {
      RData.All = RData.All << 1 ;
      IO_Clear_EEPROM_SK(); 
      asm("nop") ;
      RData.Bit.B0 = IO_Read_EEPROM_DO() ;      
      IO_Set_EEPROM_SK();
      asm("nop") ;
      }
  //
  return RData.All ;
}



//------------------------------------------------------------------------------
unsigned char Mx25L1606E_ReadByte( unsigned long MAddr )
{
  unsigned char ByteData ;
  //
  IO_Clear_EEPROM_CS() ;
  //
  Mx25L1606E_CommandOut(_RD_,MAddr,0) ;
  ByteData = Mx25L1606E_ByteInput()  ;
  //
  IO_Set_EEPROM_CS();
  //
  return ByteData ;
}



//------------------------------------------------------------------------------
unsigned short Mx25L1606E_ReadWord( unsigned long MAddr )
{
  //
  union {
    struct {
      unsigned char Low:8 ;
      unsigned char High:8 ;
    } Byte ;
    unsigned int All ;
  } RData ;
  //
  IO_Clear_EEPROM_CS() ;
  //
  Mx25L1606E_CommandOut(_RD_,MAddr,0) ;
  RData.Byte.Low = Mx25L1606E_ByteInput()  ;
  RData.Byte.High = Mx25L1606E_ByteInput()  ;
  //
  IO_Set_EEPROM_CS();
  //
  return RData.All ;
}

//------------------------------------------------------------------------------
void Mx25L1606E_ReadBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr )
{
  unsigned long ByteCount ;
  //
  IO_Clear_EEPROM_CS() ;
  //
  Mx25L1606E_CommandOut(_FRD_,MAddr,0) ;
  for( ByteCount = 0 ; ByteCount < Size ; ByteCount++ )
      {
      *(Rptr+ByteCount) = Mx25L1606E_ByteInput()  ;
      }
  //
  IO_Set_EEPROM_CS();
  //
  //
  return ;
}

//------------------------------------------------------------------------------
void Mx25L1606E_WriteByte( unsigned long MAddr, unsigned char Data )
{
  unsigned char ErrorCount ;
  ErrorCount = 0 ;
  //
  Mx25L1606E_ReadStatus() ;
  if( MX_Status.Bit.BP != 0 )
      {
      while( ErrorCount < 3 && Mx25L1606E_BlockProtectionDisable() != 0 ) 
          {
          ErrorCount += 1 ;
          }
      }
  //
  if( ErrorCount < 3 )
      {
      //
      Mx25L1606E_WaitBusyFree() ;
      //
      Mx25L1606E_WriteEnable() ;
      //
      IO_Clear_EEPROM_CS() ;
      Mx25L1606E_CommandOut(_PP_ ,MAddr,0) ;
      Mx25L1606E_ByteOut(Data) ;
      IO_Set_EEPROM_CS();
      asm("nop") ;
      //
      Mx25L1606E_WaitBusyFree() ;
      //
      Mx25L1606E_WriteDisable() ;
      //
      Mx25L1606E_BlockProtectionEnable() ;
      //
      }
  else
      {
      Mx25L1606EError = 1 ;
      }
  //
  return ;
}


//------------------------------------------------------------------------------
void Mx25L1606E_WriteBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr )
{
  unsigned char ErrorCount ;
  unsigned long DataIndex ;
  unsigned long DataSize ;
  unsigned long TotalSize ;
  unsigned long MemAddress ;
  
  ErrorCount = 0 ;
  TotalSize = Size ;
  DataIndex = 0 ;
  MemAddress = MAddr ;
  Mx25L1606E_ReadStatus() ;//20130805
  if( MX_Status.Bit.BP != 0 )
      {
      while( ErrorCount < 3 && Mx25L1606E_BlockProtectionDisable() != 0 ) 
          {
          ErrorCount += 1 ;
          }
      }
  
  if( ErrorCount < 3 )
      {
      //
      do
          {
          //  
          DataSize = _PP_PageSize_ - (MemAddress%_PP_PageSize_) ;
          if( TotalSize <= DataSize )
              DataSize = (unsigned short)TotalSize ;
        
          //
          Mx25L1606E_PageProgram(MemAddress,DataSize,(Rptr+DataIndex)) ;
          //
          //
          MemAddress += DataSize ;
          DataIndex += DataSize ;
          TotalSize -=  DataSize ;
          //
          } while( TotalSize != 0 ) ;
      //
      Mx25L1606E_BlockProtectionEnable() ;
      //
      }
  else
      {
      Mx25L1606EError = 1 ;
      }    
  //
  return ;
}

void Mx25L1606E_PageProgram( unsigned long MAddr, unsigned short Size, unsigned char *Rptr )
{
  unsigned long ByteCount ;
 
  //
  Mx25L1606E_WaitBusyFree() ;
  //
  Mx25L1606E_WriteEnable() ;
  //
  IO_Clear_EEPROM_CS() ;
  Mx25L1606E_CommandOut( _PP_ , MAddr , 0 ) ;
  //
  for( ByteCount = 0 ; ByteCount < Size ; ByteCount++ )
      {
      Mx25L1606E_ByteOut(*(Rptr+ByteCount)) ;
      }
  IO_Set_EEPROM_CS();
  asm("nop") ;
  //
  Mx25L1606E_WaitBusyFree() ;
  //
  Mx25L1606E_WriteDisable() ;
  //
  return ;
}




//------------------------------------------------------------------------------
void Mx25L1606E_SectorErase(unsigned short Sector) 
{
  unsigned long SectorAddr = 0 ;
  unsigned char ErrorCount = 0 ;
  //
  Mx25L1606E_ReadStatus() ;
  if( MX_Status.Bit.BP != 0 )
      {
      while( ErrorCount < 3 && Mx25L1606E_BlockProtectionDisable() != 0 )
          {
          ErrorCount += 1 ;
          }
      }
  //
  if( ErrorCount < 3 )
      {
      //----------------------------------------------------------------------------
      if( Sector < _SectorNumber_ )
          {
          if( Sector == 0 )
              SectorAddr = 0 ;
          else
              SectorAddr = _SectorBase_ * Sector ;
          //
          Mx25L1606E_WriteEnable() ;
          //
          IO_Clear_EEPROM_CS() ;
          asm("nop") ;
          Mx25L1606E_CommandOut(_SE_,SectorAddr,0) ;
          //
          IO_Set_EEPROM_CS() ;
          //----------------------------------------------------------------------------
          Mx25L1606E_WriteDisable() ;
          //
          Mx25L1606E_BlockProtectionEnable() ;
          //
          }
      }
  else
      {
      Mx25L1606EError = 1 ;
      }
  //----------------------------------------------------------------------------
  return ;
}

void Mx25L1606E_MajoritySectorErase(unsigned long Address, unsigned long Size)
{
    unsigned long i = 0;
    
    // Erase
    for(i=0;i<(Size/_SectorBase_);i++)
    {
        Mx25L1606E_SectorErase(i+(Address/_SectorBase_));
    }   
}


//------------------------------------------------------------------------------
unsigned int Mx25L1606E_ReadID(void)
{
  //
  union {
    struct {
      unsigned char Low:8 ;
      unsigned char High:8 ;
    } Byte ;
    unsigned int All ;
  } RData ;
  //----------------------------------------------------------------------------
  IO_Clear_EEPROM_CS() ;
  //
  Mx25L1606E_CommandOut(_RDID_,0,0) ;
  RData.Byte.High = Mx25L1606E_ByteInput()  ;
  RData.Byte.Low = Mx25L1606E_ByteInput()  ;
  //
  IO_Set_EEPROM_CS();
  //
  //----------------------------------------------------------------------------
  return RData.All ;
}



//------------------------------------------------------------------------------
unsigned char Mx25L1606E_ReadStatus(void)
{
  //----------------------------------------------------------------------------
  IO_Clear_EEPROM_CS() ;
  //
  Mx25L1606E_CommandOut(_RDSR_,0,0) ;
  MX_Status.All = Mx25L1606E_ByteInput()  ;
  //
  IO_Set_EEPROM_CS();
  //
  //----------------------------------------------------------------------------
  return MX_Status.All;
}


//------------------------------------------------------------------------------
void Mx25L1606E_BlockProtectionEnable( void )
{
  //----------------------------------------------------------------------------
  //
  Mx25L1606E_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  IO_Clear_EEPROM_CS() ;
  //
  Mx25L1606E_CommandOut(_WREN_,0,0) ;
  //
  IO_Set_EEPROM_CS() ;
  //
  asm("nop") ;
  IO_Clear_EEPROM_CS() ;
  //
  Mx25L1606E_CommandOut(_WRSR_,0,0x3c) ;
  //
  IO_Set_EEPROM_CS();
  //
  //----------------------------------------------------------------------------
  //
  Mx25L1606E_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  return ;
}


//------------------------------------------------------------------------------
unsigned char Mx25L1606E_BlockProtectionDisable( void )
{
  //----------------------------------------------------------------------------
  //
  Mx25L1606E_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  IO_Clear_EEPROM_CS() ;
  //
  Mx25L1606E_CommandOut(_WREN_,0,0) ;
  //
  IO_Set_EEPROM_CS() ;
  //
  asm("nop") ;
  IO_Clear_EEPROM_CS() ;
  //
  Mx25L1606E_CommandOut(_WRSR_,0,0) ;
  IO_Set_EEPROM_CS();
  //
  //----------------------------------------------------------------------------
  //
  Mx25L1606E_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  if( MX_Status.Bit.BP != 0 )
      return 1 ;
  //
  return 0 ;
}


void Mx25L1606E_WriteEnable(void)
{
  //
  Mx25L1606E_WaitBusyFree() ;
  //
  IO_Clear_EEPROM_CS() ;
  Mx25L1606E_CommandOut(_WREN_,0,0) ;
  //
  IO_Set_EEPROM_CS();
  asm("nop") ;
  //
  return ;
}


void Mx25L1606E_WriteDisable(void)
{
  //
  Mx25L1606E_WaitBusyFree() ;
  //
  IO_Clear_EEPROM_CS() ;
  Mx25L1606E_CommandOut(_WRDI_,0,0) ;
  //
  IO_Set_EEPROM_CS();
  asm("nop") ;
  //
  return ;
}


//------------------------------------------------------------------------------
unsigned char Mx25L1606E_WaitBusyFree(void)
{
  unsigned long DelayCount ;
  unsigned char DelayCheck ;
  DelayCount = 0 ;
  //
  do
  {
  for( DelayCheck = 0 ; DelayCheck < 0xFF ; DelayCheck++ ) ;
  DelayCount += 1 ;
  Mx25L1606E_ReadStatus() ;  
  } while(MX_Status.Bit.WIP == 1 && DelayCount < 10000 ) ;
  //
  if( DelayCount > 10000 )
      return 0xFF ;
  //
  return 0 ;  
}



void Mx25L1606E_InsertDummyCycle( unsigned char dummy_cycle )
{
  unsigned char i;
  for( i=0; i < dummy_cycle; i=i+1 )
      {
      IO_Clear_EEPROM_SK() ;
      IO_Set_EEPROM_SK() ;
      }
  return ;
}

//
void WiFi_TargetLed(unsigned char TargetLed)
{  
  unsigned char i;
  
  for(i=0;i<10;i++)
    wLed_Buffer[i] = 0x00;
  
  WiFi_Led_SetData();
  wLed_Buffer[8] = 0x08;    //COM 11
  switch(TargetLed)
  {
      case 1 : // LED Time Elapsed
                    wLed_Buffer[0] = 0x80;
                    WiFi_Led_SetData();
                    break;
      case 2 : // From USB Update иак╦ LED Program Time
                    if(wData_Buffer[0] != 0x40)      
                        wData_Buffer[0] = 0x40;      
                    else
                        wData_Buffer[0] = 0x00;
                    wLed_Buffer[0] = wData_Buffer[0];
                    WiFi_Led_SetData();
                    break;
      case 3 : // иак╦ LED Time Remaining
                    if(wData_Buffer[1] != 0x20)      
                        wData_Buffer[1] = 0x20;      
                    else
                        wData_Buffer[1] = 0x00;
                    wLed_Buffer[0] = wData_Buffer[1]; //Target Heart
                    WiFi_Led_SetData();
                    break;
      default:
                    return; 
  }
  return;
}

void WiFi_Led_SetData(void)
{
  unsigned char by_Loop,by_Check,by_CheckA;
  
  HCC_GPIO_BSRR(E)=(1u<<24); // PE8 off LED_CLK
  for(by_Loop=0;by_Loop<10;by_Loop++)
  {
     by_CheckA= wLed_Buffer[by_Loop];
     for(by_Check=0;by_Check<8;by_Check++)
     {
        if(by_CheckA & 0x80)
            HCC_GPIO_BSRR(E)=(1u<<10); // PE10 on LED_SDI
        else
            HCC_GPIO_BSRR(E)=(1u<<26); // PE10 off LED_SDI
        
        by_CheckA=by_CheckA << 1;
        HCC_GPIO_BSRR(E)=(1u<<8); // PE8 on LED_CLK
        HCC_GPIO_BSRR(E)=(1u<<24); // PE8 off LED_CLK
     }
  }
  HCC_GPIO_BSRR(E)=(1u<<25); // PE9 off LED_LE
  HCC_GPIO_BSRR(E)=(1u<<7); // PE7 on LED_OE
  HCC_GPIO_BSRR(E)=(1u<<9); // PE9 on LED_LE
  HCC_GPIO_BSRR(E)=(1u<<23); // PE7 off LED_OE
  HCC_GPIO_BSRR(E)=(1u<<25); // PE9 off LED_LE
  return;
}




