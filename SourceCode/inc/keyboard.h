#ifndef _keyboard_H_
#define _keyboard_H_

#include "Beep.h"
#include "stm32f10x.h"

#define K_NONE      0x00
#define K_NULL      0xff
#define K_MASK      0x0200//0x80
#define K_ERROR     0xfe

enum
{
// UP keypad (Main key)  
  K_BACK = 1,
  K_ENTER,
  K_MANUAL,
  K_Training,
  K_TargetHR,
  K_FatBurn,
  K_Rolling,
  K_FitnessTest,
  K_0,
  K_1,
  K_2,
  K_3,
  K_4,
  K_5,
  K_6,
  K_7,
  K_8,
  K_9,
  K_T1,
  K_T2,
  K_T3,
  K_T4,
  K_PAUSE,
  K_TimeU,
  K_TimeD,
  K_SpeedU,
  K_SpeedD,
  K_InclineU,
  K_InclineD,
    // END
  UpperOverlayEnd,
// Down keypad (TV key)
  K_GO = UpperOverlayEnd,
  K_STOP,  
  K_TV_POWER,
  K_TV_VU,
  K_TV_VD,
  K_TV_CU,
  K_TV_CD,
  K_TV_LAST,
  K_Mute,
  K_CC,
  K_CoolDown,
    // End
  LowerOverlayEnd,
// Quick keypad
  K_QuickSpeedU = LowerOverlayEnd,
  K_QuickSpeedD,
  K_QuickInclineU,
  K_QuickInclineD,
  K_QuickGO,
  K_QuickSTOP,
  // End
  QuickKeyEnd,  
// Other 
  K_IDSD = QuickKeyEnd, //==>Incline Down & Speed Down
  K_ClearNumber     
};



#define K_HoldIDSD    (K_IDSD | K_MASK)

void Keyboard_Initial(void);
void Key_Counter_Int(void);
u16 Key_Proce(void);
u8 Key_Value_Read( u8 * Buffer);
u16 Keyboard_NumberCount(u8 by_Range,u16 by_KeyInput);
u32 Keyboard_DFCount(u8 by_D,u16 by_D1,u32 by_D2,u8 by_D3,u32 by_D4);

#endif //_keyboard_H_
