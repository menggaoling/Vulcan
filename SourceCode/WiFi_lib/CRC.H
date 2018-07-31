#ifndef __CRC_H
#define __CRC_H


#include "wifi_define.h"
#include "MyStruct.H"
#define DCrc_ResetValue							0xFFFF


U16 GenerateCRC_CCITT_F1( UPtr8 PUPtr8, U16 PU16_Count );//kill F1
U16 GenerateCRC_CCITT( UPtr8 PUPtr8, U16 PU16_Count );


#endif  /* __CRC_H */
