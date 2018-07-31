#ifndef _TV_H_
#define _TV_H_

//#include  "stm32f10x_conf.h"

#define TVCom_Number     0x0b
#define TVCom_ENTER      0x0f
#define TVCom_VOL        0x10
#define TVCom_CH         0x12
#define TVCom_POWER      0x40
#define TVCom_RESET      0x41
#define TVCom_LAST       0x42
#define TVCom_ReadStatus 0x81
#define TVCom_Mute       0x21
#define TVCom_CC         0x22
#define TVCom_Aspect     0x26
#define TVCom_Home       0x28

#define TVData_CHUp      0x00
#define TVData_CHDown    0x01
#define TVData_VOLUp     0x80
#define TVData_VOLDown   0xc0
#define TVData_PowerON   0x80
#define TVData_PowerOFF  0x00

void TV_Initial(void);
void TV_UartTxRx_Information(void);
//void TV_SetCommand(void);
void TV_SetCommand(u8 by_COM,u8 by_Dat);
void TV_AutoReply(void);
void TV_Key(u8 by_Dat);
void TV_RxAssay(void);
u8 TV_GetPowe_OnOff(void);

#endif // _TV_H_
