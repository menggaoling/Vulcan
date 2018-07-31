//#include  "stm32f10x_lib.h"

typedef struct
{
  unsigned char Seconds;
  unsigned char Minutes;
  unsigned char Hours;
  unsigned char Date;
  unsigned char Month;
  unsigned char Day;
  unsigned char Year;
  unsigned char WriteProtect;
}SHT1381;

typedef struct
{
  unsigned char Second;
  unsigned char Minute;
  unsigned char Hour;
  unsigned char Month;
  unsigned char Day;
  unsigned char Year;
}SHT1381_RealTime;

__root void HT1381_SetDIN(void);
__root void HT1381_SetDOUT(void);
__root void ReadHT1381(SHT1381 *ReadBuffer);
__root void WriteHT1381(SHT1381 *ReadBuffer);
__root void HT1381_HWInitial(void);
__root void HT1381_RealTime(SHT1381_RealTime *ReadBuffer);
