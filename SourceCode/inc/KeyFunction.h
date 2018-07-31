#ifndef _keyFunction_H_
#define _keyFunction_H_

#define _KeyScanSignal    1 // 设定单一扫描脚 LOW
#define _KeyScanReset     2 // 所有Pin脚初始化 HI
#define _KeyDataRead      3 // 取得对应数据PORT数据
#define _KeyIntClose      4 // 关闭中断执行
#define _KeyIntOpen       5 // 打开中断执行
#define _KeyBeep          6 // 蜂鸣器
#define _KeyTest_NotPress 7
#define _KeyTest_Presskey 8
#define _KeyTest_KeyFail  9


typedef struct {
  unsigned char  KeyScanNumber ;            // 扫瞄PIN数 (最大16PIN)DF:8 
  unsigned char  KeyDataNumber ;            // 资料PIN数 (最大16PIN)DF:8 
  unsigned char  _KeyCheckNumber ;          // 按键判断次数 ; DF:10 
  unsigned char  _KeyPress ;                // 按键成立判断次数 ; DF:5 
  unsigned char  SpecialKeyGroup ;          // 特殊复合按键组数 ; DF:3 
  unsigned char  SpecialKeyInforNumber ;    // 特殊复合按键内的数据数 ; DF:4 
  unsigned short HoldKeyColseTime ;         // 持续按住 x`s 后将不在承认按键 ; DF:10000 
  unsigned short HoldKeyFistTime ;          // 按住 xx ms 后开始成立加速条件 ; DF:300 
  unsigned short HoldKeyLoopChangeTime ;    // 两段加速成立条件切换时间 ; DF:3000 
  unsigned short HoldKeyHighSpeedLoop ;     // High speed 每 x ms 成立按键一次 ; DF:60 
  unsigned short HoldKeyLowSpeedLoop ;      // Low speed 每 x ms 成立按键一次 ; DF:200 
  unsigned short HoldKeyMASK ;              // 持续按键成立时加入代码 ; DF:0x0200
  unsigned char  KeypadeTest ;              // 开机时做按键测试是否成立时间参数 ; DF:3(秒)
  unsigned char  HoldKeyNumber ;            // Table表每组 Hold key 数 ; DF:5 
  unsigned char  HoldKeyTimeGroup ;         // Hold key delay time 组数 ; DF:3 
  const unsigned short *_HoldKeyTimeTable ; // Hold key delay time 对应表
  const unsigned char  *_HoldKeyTable ;     // Hold key 对应表
  const unsigned char  *_KeyTable ;         // 按键代码对应表
  const unsigned char  *_SpecialKeyTable ;  // 特殊复合按键代码对应表
  } JHTKeyFunctionLib ;

typedef unsigned int (*KeyFunction)(char ,char );
extern KeyFunction _Key_Data;


void KeyScan_Int(void);
unsigned int KeyScan_Proce(void);
unsigned int KeyScan_GetKeyNumber(void);
void KeyScan_Initial( JHTKeyFunctionLib *KeyInformation, const unsigned char *_KeyTable,
                      const unsigned char *_SpecialKey, const unsigned short *_HoldKeyTime,
                      const unsigned char *_HoldKey);
void KeyScan_SetKeyIntSwitch(char by_D);
char KeyScan_GetKeyIntSwitch(void);
char KeyScan_KeypadTest(void);
unsigned char KeyScan_BadKeyRegion(unsigned char by_Region);
unsigned char KeyScan_GetBypaddKeyNumber(void);

#endif // _keyFunction_H_
