#include "Beep.h"
#include "General.h"
#include "Bike.h"

#define HR_STOP_TIME      15

struct {
         unsigned Beep_One         : 1;
         unsigned Beep_Heart_Stop  : 1;
         unsigned Beep_Blink       : 1;
         unsigned Key_Show         : 1;
         unsigned Switch           : 1;
       } Beep;
u16 w_Beep_ON,w_Beep_OFF;
u8 by_Beep_Time;
u8 by_Beep_Heart;

void BEEP_Initial(void)
{
  by_Beep_Heart=0;
  Beep.Beep_Heart_Stop=0;
  Beep.Key_Show=1;
  Beep.Beep_One=0;
}
void BEEP_SetSwitch(char by_D)
{
  if(by_D == 0)
      Beep.Switch=0;
  else
      Beep.Switch=1;
}
u8 BEEP_Get_OK(void)
{
  return(Beep.Beep_One);
}
u8 BEEP_Get_Heart_Stop(void)
{
  return(Beep.Beep_Heart_Stop);
}
u8 BEEP_Get_Key_Show(void)
{
  return(Beep.Key_Show);
}
void BEEP_Proce(u8 by_time,u8 by_on,u8 by_off)
{  
  w_Beep_ON = by_on * 50;
  w_Beep_OFF = by_off * 50;
  by_Beep_Time = by_time;
  Beep.Beep_One = 1;
}
void BEEP_Set_Blink(void)
{
  Beep.Beep_Blink=1;
}
void BEEP_Int(void)
{
  static u8 by_Loop = 0;
  static u16 w_Time = 0;
  if(Beep.Beep_One)
  {
      if(by_Loop < by_Beep_Time)
      {
          if(by_Loop & 0x01)
          {
              if(w_Time == 0)
              {
                  IO_Clear_BEEP();
                  if(Beep.Beep_Blink) Beep.Key_Show = 0;
              }
              if(w_Time >= w_Beep_OFF)
              {
                  w_Time=0;
                  ++by_Loop;
              }
              else ++w_Time;
          }
          else
          {
              if(w_Time == 0)
              {
                  if(Beep.Switch == _ON) 
                  {
                    IO_Set_BEEP();
                  }
                  if(Beep.Beep_Blink) Beep.Key_Show=1;
                  Beep.Beep_Heart_Stop=1;
                  by_Beep_Heart=0;
              }
              if(w_Time >= w_Beep_ON)
              {
                  w_Time=0;
                  ++by_Loop;
              }
              else ++w_Time;
          }
      }
      else
      {
          if(Beep.Beep_Blink) Beep.Beep_Blink=0;
          Beep.Key_Show=1;
          by_Loop=0;
          w_Time=0;
          Beep.Beep_One=0;
          IO_Clear_BEEP();
      }
  }
  else IO_Clear_BEEP();
}

void BEEP_Counter_Int(void)
{
   if(by_Beep_Heart < HR_STOP_TIME) 
       ++by_Beep_Heart;
   else 
       Beep.Beep_Heart_Stop=0;
}
