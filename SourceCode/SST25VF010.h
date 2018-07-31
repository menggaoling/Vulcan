//
#ifndef __SST25VF010_H__
#define __SST25VF010_H__


unsigned char SST25VF010_ReadByte( unsigned long MAddr ) ;
unsigned short SST25VF010_ReadWord( unsigned long MAddr ) ;
void  SST25VF010_ReadBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr ) ;
unsigned int SST25VF010_ReadID(void) ;
void SST25VF010_WriteByte( unsigned long MAddr, unsigned char Data ) ;
void SST25VF010_WriteBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr ) ;
void SST25VF010_SectorErase(unsigned char Sector) ;


#define     _SectorBase_        4096
#define     _SectorNumber_      32

#endif /* __SST25VF010_H__ */




