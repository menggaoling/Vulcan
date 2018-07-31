#ifndef __HEART_H__
#define __HEART_H__

#define HRC_UP   2
#define HRC_DOWN 1
#define HRC_NONE 0

#define _HeartRate_MaxBPM  220

void Heart_Initial(void);
void Heart_1ms_Int(void);
unsigned char Heart_Get_Heart(void);
void Heart_Decoder(void);
void Heart_Stop(void);
void Heart_Start(void);
unsigned char Heart_Get_Target(void);
void Heart_Set_Target(unsigned char by_Dat);
void Heart_Reload_HR2_Int(void);
unsigned char Heart_Signal(void); 
unsigned char Heart_GetBlink(void);

#endif /* __HEART_H__ */
