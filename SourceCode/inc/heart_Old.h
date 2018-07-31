#include "General.h"

#define HRC_UP   2
#define HRC_DOWN 1
#define HRC_NONE 0
void Heart_Initial(void);
void Heart_Counter_Int(void);
u8 Heart_Get_Heart(void);
u8 Heart_Decoder(void);
void Heart_Timer_Counter_Int(void);
void Heart_Stop(void);
void Heart_Start(void);
u8 Heart_Get_Target(void);
void Heart_Set_Target(u8 by_Dat);
void Heart_Reload_HR1_Int(void);
void Heart_Reload_HR2_Int(void);
u8 Heart_GetBlink(void);
u8 Heart_GetShowZero(void);
u8 Heart_Get_HRSignal(void);


