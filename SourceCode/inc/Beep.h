#ifndef _Beep_H_
#define _Beep_H_

#include "General.h"

void BEEP_Initial(void);
void BEEP_Counter_Int(void);
void BEEP_Proce(u8 by_time,u8 by_on,u8 by_off);
void BEEP_Int(void);
u8 BEEP_Get_OK(void);
void BEEP_Set_Blink(void);
u8 BEEP_Get_Key_Show(void);
u8 BEEP_Get_Heart_Stop(void);
void BEEP_SetSwitch(char by_D);

#endif
