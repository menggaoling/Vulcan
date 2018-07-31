//
#include  "General.h"
#include  "SST25VF010.h"

//==============================================================================

#define     _RD_                0x03    // Read
#define     _SE_                0x20    // Sector Erase 4Kbyte  x (A12~A16)
#define     _BE_                0x52    // Block Erase 32Kbyte X (A15~A16)
#define     _CE_                0x60    // Chip Erase
#define     _WRB_               0x02    // Byte Programmer
#define     _AAI_               0xAF    // Auto Address Increment(AAI)Program
#define     _AAI1_              0xB0    //
#define     _RDSR_              0x05    // Read Status Register
#define     _EWSR_              0x50    // Enable Write Status Register
#define     _WRSR_              0x01    // Write Status Register
#define     _WREN_              0x06    // Write Enable
#define     _WRDI_              0x04    // Write Disable
#define     _RDID_              0x90    // Read ID


//
/*
0     BUSY      1 = Internal Write operation is in progress                     0 R
                0 = No internal Write operation is in progress
1     WEL       1 = Device is memory Write enabled                              0 R
                0 = Device is not memory Write enabled
2     BP0       Indicate current level of block write protection (See Table 4)  1 R/W
3     BP1       Indicate current level of block write protection (See Table 4)  1 R/W
4:5   RES       Reserved for future use 0 N/A
6     AAI       Auto Address Increment Programming status                       0 R
                1 = AAI programming mode
                0 = Byte-Program mode
7     BPL       1 = BP1, BP0 are read-only bits                                 0 R/W
                0 = BP1, BP0 are read/writable
*/
union {
  struct {
    unsigned char BUSY:1 ;
    unsigned char WEL:1 ;
    unsigned char BP:2 ;
    unsigned char RES:2 ;
    unsigned char AAI:1 ;
    unsigned char BPL:1 ;
  } Bit ;
  unsigned char All ;
} SST_Status ;


unsigned char SST25VF010Error = 0 ;

//------------------------------------------------------------------------------
  
void SST25VF010_CommandOut( unsigned char Cmd, unsigned long Addr, unsigned char Data ) ;
void SST25VF010_ByteOut(unsigned char Data) ;
unsigned char SST25VF010_ByteInput(void) ;
unsigned char SST25VF010_ReadStatus(void) ;
void SST25VF010_BlockProtectionEnable( void ) ;
unsigned char SST25VF010_BlockProtectionDisable( void );
void SST25VF010_WriteEnable(void);
void SST25VF010_WriteDisable(void);
unsigned char SST25VF010_WaitBusyFree(void) ;





//---------------------------------------------------------------------------------
void  SST25VF010_CommandOut( unsigned char Cmd, unsigned long Addr, unsigned char Data )
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
      case  _AAI_   :// Auto Address Increment(AAI)Program                    
                    SST25VF010_ByteOut(Cmd) ;
                    SST25VF010_ByteOut(AddrData.Byte.Byte2) ;
                    SST25VF010_ByteOut(AddrData.Byte.Byte1) ;
                    SST25VF010_ByteOut(AddrData.Byte.Byte0) ;
                    SST25VF010_ByteOut(Data) ;
                    break ;
      case  _AAI1_  :
                    SST25VF010_ByteOut(_AAI_) ;
                    SST25VF010_ByteOut(Data) ;
                    break ;
      case  _WRSR_  :// Write Status Register
                    SST25VF010_ByteOut(Cmd) ;
                    SST25VF010_ByteOut(Data) ;
                    break ;
      case  _CE_    :// Chip Erase
      case  _RDSR_  :// Read Status Register
      case  _EWSR_  :// Enable Write Status Register
      case  _WREN_  :// Write Enable
      case  _WRDI_  :// Write Disable
                    SST25VF010_ByteOut(Cmd) ;
                    break ;
      case  _WRB_   :// Byte Programmer
                    SST25VF010_ByteOut(Cmd) ;
                    SST25VF010_ByteOut(AddrData.Byte.Byte2) ;
                    SST25VF010_ByteOut(AddrData.Byte.Byte1) ;
                    SST25VF010_ByteOut(AddrData.Byte.Byte0) ;
                    SST25VF010_ByteOut(Data) ;
                    break ;  
                    
      case  _RD_    :// Read
      case  _SE_    :// Sector Erase 4Kbyte  x (A12~A16)
      case  _BE_    :// Block Erase 32Kbyte X (A15~A16)                    
      case  _RDID_  :// Read ID
                    SST25VF010_ByteOut(Cmd) ;
                    SST25VF010_ByteOut(AddrData.Byte.Byte2) ;
                    SST25VF010_ByteOut(AddrData.Byte.Byte1) ;
                    SST25VF010_ByteOut(AddrData.Byte.Byte0) ;
                    break ;
      default       :
                    break ;
      }   
  return ;
}


//------------------------------------------------------------------------------
void SST25VF010_ByteOut(unsigned char Data)
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
    //
    IO_Clear_EEPROM_SK();
    asm("nop") ;
    if( OutData.Bit.B7 == 1 )
        {
        IO_Set_EEPROM_DI();
        }
    else
        {
        IO_Clear_EEPROM_DI();
        }
    //
    OutData.All = OutData.All << 1 ;
    IO_Set_EEPROM_SK();
    asm("nop") ;
    }
  //
  return ;
}

//------------------------------------------------------------------------------
unsigned char SST25VF010_ByteInput(void)
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
unsigned char SST25VF010_ReadByte( unsigned long MAddr )
{
  unsigned char ByteData ;
  //
  IO_Clear_EEPROM_CS() ;
  //
  SST25VF010_CommandOut(_RD_,MAddr,0) ;
  ByteData = SST25VF010_ByteInput()  ;
  //
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS();
  //
  return ByteData ;
}



//------------------------------------------------------------------------------
unsigned short SST25VF010_ReadWord( unsigned long MAddr )
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
  SST25VF010_CommandOut(_RD_,MAddr,0) ;
  RData.Byte.Low = SST25VF010_ByteInput()  ;
  RData.Byte.High  = SST25VF010_ByteInput()  ;
  //
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS();
  //
  return RData.All ;
}

//------------------------------------------------------------------------------
void SST25VF010_ReadBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr )
{
  unsigned long ByteCount ;
  //
  IO_Clear_EEPROM_CS() ;
  //
  SST25VF010_CommandOut(_RD_,MAddr,0) ;
  for( ByteCount = 0 ; ByteCount < Size ; ByteCount++ )
      {
      *(Rptr+ByteCount) = SST25VF010_ByteInput()  ;
      }
  //
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS();
  //
  return ;
}

//------------------------------------------------------------------------------
void SST25VF010_WriteByte( unsigned long MAddr, unsigned char Data )
{
  unsigned char ErrorCount ;
  ErrorCount = 0 ;
  //
  SST25VF010_ReadStatus() ;
  if( SST_Status.Bit.BP != 0 )
      {
      while( ErrorCount < 3 && SST25VF010_BlockProtectionDisable() != 0 ) 
          {
          ErrorCount += 1 ;
          }
      }
  //
  if( ErrorCount < 3 )
      {
      //
      SST25VF010_WriteEnable() ;
      //
      IO_Clear_EEPROM_CS() ;
      SST25VF010_CommandOut(_WRB_ ,MAddr,Data) ;
      //SST25VF010_ByteOut(Data) ;
      IO_Clear_EEPROM_SK() ;
      asm("nop") ;
      IO_Set_EEPROM_CS();
      asm("nop") ;
      //
      SST25VF010_WriteDisable() ;
      //
      SST25VF010_BlockProtectionEnable() ;
      //
      }
  else
      {
      SST25VF010Error = 1 ;
      }
  //
  return ;
}


//------------------------------------------------------------------------------
void SST25VF010_WriteBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr )
{
  unsigned long ByteCount ;
  unsigned char ErrorCount ;
  ErrorCount = 0 ;
  //
  SST25VF010_ReadStatus() ;
  if( SST_Status.Bit.BP != 0 )
      {
      while( ErrorCount < 3 && SST25VF010_BlockProtectionDisable() != 0 ) 
          {
          ErrorCount += 1 ;
          }
      }
  //
  if( ErrorCount < 3 )
      {
      //
      SST25VF010_WriteEnable() ;
      //
      IO_Clear_EEPROM_CS() ;
      SST25VF010_CommandOut( _AAI_ , MAddr, *Rptr ) ;//_AAI_
      //SST25VF010_ByteOut(*Rptr) ;
      IO_Clear_EEPROM_SK() ;
      asm("nop") ;
      IO_Set_EEPROM_CS();
      asm("nop") ;
      //
      for( ByteCount = 1 ; ByteCount < Size ; ByteCount++ )
          {
          SST25VF010_WaitBusyFree() ;
          IO_Clear_EEPROM_CS();
          SST25VF010_CommandOut(_AAI1_,MAddr,*(Rptr+ByteCount) ) ;
          IO_Clear_EEPROM_SK();
          asm("nop") ;
          IO_Set_EEPROM_CS();
          asm("nop") ;
          //
          }
      //
      SST25VF010_WriteDisable() ;
      //
      SST25VF010_BlockProtectionEnable() ;
      
      //
      }
  else
      {
      SST25VF010Error = 1 ;
      }
  return ;
}


//------------------------------------------------------------------------------
void SST25VF010_SectorErase(unsigned char Sector) 
{
  unsigned long SectorAddr = 0 ;
  unsigned char ErrorCount = 0 ;
  //
  SST25VF010_ReadStatus() ;
  if( SST_Status.Bit.BP != 0 )
      {
      while( ErrorCount < 3 && SST25VF010_BlockProtectionDisable() != 0 )
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
          SST25VF010_WriteEnable() ;
          //
          IO_Clear_EEPROM_CS() ;
          asm("nop") ;
          SST25VF010_CommandOut(_SE_,SectorAddr,0) ;
          IO_Clear_EEPROM_SK() ;
          asm("nop") ;
          IO_Set_EEPROM_CS() ;
          //----------------------------------------------------------------------------
          SST25VF010_WriteDisable() ;
          //
          SST25VF010_BlockProtectionEnable() ;
          //
          }
      }
  else
      {
      SST25VF010Error = 1 ;
      }
  //----------------------------------------------------------------------------
  return ;
}

//------------------------------------------------------------------------------
unsigned int SST25VF010_ReadID(void)
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
  SST25VF010_CommandOut(_RDID_,0,0) ;
  RData.Byte.High = SST25VF010_ByteInput()  ;
  //
  RData.Byte.Low = SST25VF010_ByteInput()  ;
  //
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS();
  //
  //----------------------------------------------------------------------------
  return RData.All ;
}



//------------------------------------------------------------------------------
unsigned char SST25VF010_ReadStatus(void)
{
  //----------------------------------------------------------------------------
  IO_Clear_EEPROM_CS() ;
  //
  SST25VF010_CommandOut(_RDSR_,0,0) ;
  SST_Status.All = SST25VF010_ByteInput()  ;
  //
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS();
  //
  //----------------------------------------------------------------------------
  return SST_Status.All;
}


//------------------------------------------------------------------------------
void SST25VF010_BlockProtectionEnable( void )
{
  //----------------------------------------------------------------------------
  //
  SST25VF010_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  IO_Clear_EEPROM_CS() ;
  //
  SST25VF010_CommandOut(_EWSR_,0,0) ;
  //
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS() ;
  //
  asm("nop") ;
  IO_Clear_EEPROM_CS() ;
  //
  SST25VF010_CommandOut(_WRSR_,0,0x0c) ;
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS();
  //
  //----------------------------------------------------------------------------
  //
  SST25VF010_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  return ;
}


//------------------------------------------------------------------------------
unsigned char SST25VF010_BlockProtectionDisable( void )
{
  //----------------------------------------------------------------------------
  //
  SST25VF010_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  IO_Clear_EEPROM_CS() ;
  //
  SST25VF010_CommandOut(_EWSR_,0,0) ;
  //
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS() ;
  //
  asm("nop") ;
  IO_Clear_EEPROM_CS() ;
  //
  SST25VF010_CommandOut(_WRSR_,0,0) ;
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS();
  //
  //----------------------------------------------------------------------------
  //
  SST25VF010_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  if( SST_Status.Bit.BP != 0 )
      return 1 ;
  //
  return 0 ;
}


void SST25VF010_WriteEnable(void)
{
  //
  SST25VF010_WaitBusyFree() ;
  //
  IO_Clear_EEPROM_CS() ;
  SST25VF010_CommandOut(_WREN_,0,0) ;
  //
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS();
  asm("nop") ;
  //
  return ;
}


void SST25VF010_WriteDisable(void)
{
  //
  SST25VF010_WaitBusyFree() ;
  //
  IO_Clear_EEPROM_CS() ;
  SST25VF010_CommandOut(_WRDI_,0,0) ;
  //
  IO_Clear_EEPROM_SK() ;
  asm("nop") ;
  IO_Set_EEPROM_CS();
  asm("nop") ;
  //
  return ;
}


//------------------------------------------------------------------------------
unsigned char SST25VF010_WaitBusyFree(void)
{
  unsigned long DelayCount ;
  unsigned char DelayCheck ;
  DelayCount = 0 ;
  //
  do
  {
  for( DelayCheck = 0 ; DelayCheck < 0xFF ; DelayCheck++ ) ;
  DelayCount += 1 ;
  SST25VF010_ReadStatus() ;  
  } while(SST_Status.Bit.BUSY == 1 && DelayCount < 10000 ) ;
  //
  if( DelayCount > 10000 )
      return 0xFF ;
  //
  return 0 ;  
}







