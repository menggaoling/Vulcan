#ifndef _Time_H_
#define _Time_H_

#include "General.h"

#define T_LOOP 0
#define T_STEP 1
#define Time_AllClose 0xff
#define Time_TotalBuffer 16

#define _Time_WiFiConnectCheck   10
#define _Time_HJSCheck           11 //Phone Jack Sensor
#define _Time_USBProtectionCheck 12 
#define _Time_RSCUCheck          13 
#define _Time_KeepDelay          14
#define _Time_Back               15  

void Timer_Initial(void);
void Timer_Flag_Int(void);
void Timer_Counter_Clear(u8 by_Index);
u8 Timer_Counter(u8 by_Mode,u8 by_Who,u16 w_Dat);
void Timer_Close(u8 by_Dat);
void Timer_Delay(unsigned char delayTime);

#endif // _Time_H_
