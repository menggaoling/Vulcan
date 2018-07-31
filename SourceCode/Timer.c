#include "General.h"
#include "timer.h"

u16 B_Delay[Time_TotalBuffer];
u8 B_Time[Time_TotalBuffer];

// Delay ����
// Time[0]  :
// Time[1]  :
// Time[2]  :
// Time[3]  :
// Time[4]  :
// Time[5]  : 
// Time[6]  :
// Time[7]  :
// Time[8]  :
// Time[9]  :
// Time[10] : WiFi Connect Check Time
// Time[11] : Phone Jack Sensor Check Time

// time �������������
void Timer_Initial(void)
{
  u8 by_Loop;
  
  for(by_Loop=0;by_Loop < 10;by_Loop++)
  {
      B_Delay[by_Loop]=0;
      B_Time[by_Loop]=0;
  }
}
//�ر��ض�������
void Timer_Close(u8 by_Dat)
{
  u8 i;
  if(by_Dat == Time_AllClose)
  {
      for(i=0;i < 10;i++) B_Time[i]=0;
  }
  else B_Time[by_Dat]=0;
}
//ÿ100ms�����
void Timer_Flag_Int(void)
{
  u8 i;
  
  for(i = 0 ; i < Time_TotalBuffer ; i++)
  {
      if((B_Time[i] == 1)&&(B_Delay[i] < 9000)) B_Delay[i]++;
  }
}
//�������ĳһ������
void Timer_Counter_Clear(u8 by_Index)
{
  B_Delay[by_Index]=0;
  B_Time[by_Index]=1;
}
//�趨������ 1.LOOP or STEP 2.��?������ 3.����TIME
u8 Timer_Counter(u8 by_Mode,u8 by_Who,u16 w_Dat)
{
  u8 b_Out;
  b_Out=0;
  if(B_Time[by_Who]==1 && B_Delay[by_Who]>=w_Dat)
    {
    b_Out=1;
    if(by_Mode==T_LOOP) B_Delay[by_Who]=0;
      else B_Time[by_Who]=0;
    }
  return(b_Out);
}
void Timer_Delay(unsigned char delayTime)
{// Real Delay Time = delayTime * 0.1s 
    Timer_Counter_Clear(_Time_KeepDelay);
    while(!Timer_Counter(T_STEP,_Time_KeepDelay,delayTime));
};