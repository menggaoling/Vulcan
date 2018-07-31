#include "General.h"

#define Flash_A   1
#define Flash_B   2

void Flash_Write_Word(u8 by_Address,u32 by_Dat);
void Flash_WriteDataCMD(void);
u32 Flash_ReadDataCMD(u8 FlashAddress);
void Flash_SetBlock(u8 by_Dat);
void Flash_ReadDataToBuffer(void);
