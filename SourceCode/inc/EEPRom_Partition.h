#ifndef __EEPRomPartition_H__
#define __EEPRomPartition_H__
//------------------------------------------------------------------------------
#include  "Mx25L1606E.h"

// Memory total size : 2048 KBytes
#define EEPROM_StartAddress EEPRomPartition_StartAddress_OtherUse  // 1032K

// for parameter use
#define _EEPRom_StartAddress_Parameter  EEPROM_StartAddress
#define _EEPRom_Size_Parameter _SectorBase_*2  // size: 2 * 4k = 8K

// for multi-language use
#define _EEPRom_StartAddress_Language  _EEPRom_StartAddress_Parameter+_EEPRom_Size_Parameter
#define _EEPRom_Size_Language _SectorBase_*25 // size: 25 * 4k = 100K

// for pattern(logo) use
#define _EEPRom_StartAddress_Pattern  _EEPRom_StartAddress_Language+_EEPRom_Size_Language
#define _EEPRom_Size_Pattern _SectorBase_*25 // size: 25 * 4k = 100K

//------------------------------------------------------------------------------
#endif /* __EEPRomPartition_H__ */
