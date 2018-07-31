//
#ifndef __Mx25L1606E_H__
#define __Mx25L1606E_H__

//#include  "stm32f10x.h"
#include  "stm32_reg.h"

unsigned char Mx25L1606E_ReadByte( unsigned long MAddr ) ;
unsigned short Mx25L1606E_ReadWord( unsigned long MAddr ) ;
void  Mx25L1606E_ReadBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr ) ;
unsigned int Mx25L1606E_ReadID(void) ;
void Mx25L1606E_WriteByte( unsigned long MAddr, unsigned char Data ) ;
void Mx25L1606E_WriteBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr ) ;
void Mx25L1606E_SectorErase(unsigned short Sector) ;
void Mx25L1606E_MajoritySectorErase(unsigned long Address, unsigned long Size);

#define   _Block_Size_        65536
#define   _Block_Number_      32
#define   _SectorBase_        4096
#define   _SectorNumber_      512
#define   _PP_PageSize_       256

// Mx25L1606E
#define   _Block0_Sector0_    0
#define   _Block0_Sector1_    (1*_SectorBase_)
#define   _Block0_Sector2_    (2*_SectorBase_)
#define   _Block0_Sector3_    (3*_SectorBase_)
#define   _Block0_Sector4_    (4*_SectorBase_)
#define   _Block0_Sector5_    (5*_SectorBase_)
#define   _Block0_Sector6_    (6*_SectorBase_)
#define   _Block0_Sector7_    (7*_SectorBase_)
#define   _Block0_Sector8_    (8*_SectorBase_)
#define   _Block0_Sector9_    (9*_SectorBase_)
#define   _Block0_SectorA_    (10*_SectorBase_)
#define   _Block0_SectorB_    (11*_SectorBase_)
#define   _Block0_SectorC_    (12*_SectorBase_)
#define   _Block0_SectorD_    (13*_SectorBase_)
#define   _Block0_SectorE_    (14*_SectorBase_)
#define   _Block0_SectorF_    (15*_SectorBase_)


//EEPROM Partition (For saving software use) 
#define EEPRomPartition_StartAddress_Backup  0*_SectorBase_
#define EEPRomPartition_Size_Backup 129*_SectorBase_  // size: 512K + 4K
#define EEPRomPartition_Information_StartAddress_Backup (EEPRomPartition_Size_Backup - _SectorBase_)
#define EEPRomPartition_StartAddress_RSCU  EEPRomPartition_StartAddress_Backup + EEPRomPartition_Size_Backup
#define EEPRomPartition_Size_RSCU 129*_SectorBase_    // size: 512K + 4K
//
#define EEPRomPartition_StartAddress_OtherUse EEPRomPartition_StartAddress_RSCU+EEPRomPartition_Size_RSCU



//------------------------------------------------------------------------------
#define     _RD_                0x03    // Read
#define     _SE_                0x20    // Sector Erase 4Kbyte  x (A12~A16)
#define     _BE_                0x52    // Block Erase 64Kbyte  X (A15~A16)
#define     _CE_                0x60    // Chip Erase
#define     _PP_                0x02    // Page Programmer
#define     _FRD_               0x0B    // Fast Read Data
#define     _RDSR_              0x05    // Read Status Register
#define     _EWSR_              0x50    // Enable Write Status Register
#define     _WRSR_              0x01    // Write Status Register
#define     _WREN_              0x06    // Write Enable
#define     _WRDI_              0x04    // Write Disable
#define     _RDID_              0x9F    // Read ID
#define     _RDSR_              0x05    // Read Status Register
#define     _DREAD_             0x3B    // Double Output Mode Command
#define     _REMS_              0x90    // Read Electronic manufacturer & device ID
#define     _RES_               0xAB    // Read Electronic ID

//=================================================================
//====> ¥~±¾EEPROM±±¨î¸}
//=================================================================
#define IO_Clear_EEPROM_CS()    {HCC_GPIO_BSRR(B)=(1u<<28);}
#define IO_Set_EEPROM_CS()      {HCC_GPIO_BSRR(B)=(1u<<12);}
#define IO_Clear_EEPROM_SK()    {HCC_GPIO_BSRR(B)=(1u<<29);}
#define IO_Set_EEPROM_SK()      {HCC_GPIO_BSRR(B)=(1u<<13);}
#define IO_Clear_EEPROM_DI()    {HCC_GPIO_BSRR(B)=(1u<<30);}
#define IO_Set_EEPROM_DI()      {HCC_GPIO_BSRR(B)=(1u<<14);}
#define IO_Read_EEPROM_DO()     HCC_GPIO_IDR(B)>>15&0x01

void WiFi_TargetLed(unsigned char TargetLed);
void WiFi_Led_SetData(void);

#endif /* __Mx25L1606E_H__ */




