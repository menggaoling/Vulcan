#ifndef _keyFunction_H_
#define _keyFunction_H_

#define _KeyScanSignal    1 // �趨��һɨ��� LOW
#define _KeyScanReset     2 // ����Pin�ų�ʼ�� HI
#define _KeyDataRead      3 // ȡ�ö�Ӧ����PORT����
#define _KeyIntClose      4 // �ر��ж�ִ��
#define _KeyIntOpen       5 // ���ж�ִ��
#define _KeyBeep          6 // ������
#define _KeyTest_NotPress 7
#define _KeyTest_Presskey 8
#define _KeyTest_KeyFail  9


typedef struct {
  unsigned char  KeyScanNumber ;            // ɨ��PIN�� (���16PIN)DF:8 
  unsigned char  KeyDataNumber ;            // ����PIN�� (���16PIN)DF:8 
  unsigned char  _KeyCheckNumber ;          // �����жϴ��� ; DF:10 
  unsigned char  _KeyPress ;                // ���������жϴ��� ; DF:5 
  unsigned char  SpecialKeyGroup ;          // ���⸴�ϰ������� ; DF:3 
  unsigned char  SpecialKeyInforNumber ;    // ���⸴�ϰ����ڵ������� ; DF:4 
  unsigned short HoldKeyColseTime ;         // ������ס x`s �󽫲��ڳ��ϰ��� ; DF:10000 
  unsigned short HoldKeyFistTime ;          // ��ס xx ms ��ʼ������������ ; DF:300 
  unsigned short HoldKeyLoopChangeTime ;    // ���μ��ٳ��������л�ʱ�� ; DF:3000 
  unsigned short HoldKeyHighSpeedLoop ;     // High speed ÿ x ms ��������һ�� ; DF:60 
  unsigned short HoldKeyLowSpeedLoop ;      // Low speed ÿ x ms ��������һ�� ; DF:200 
  unsigned short HoldKeyMASK ;              // ������������ʱ������� ; DF:0x0200
  unsigned char  KeypadeTest ;              // ����ʱ�����������Ƿ����ʱ����� ; DF:3(��)
  unsigned char  HoldKeyNumber ;            // Table��ÿ�� Hold key �� ; DF:5 
  unsigned char  HoldKeyTimeGroup ;         // Hold key delay time ���� ; DF:3 
  const unsigned short *_HoldKeyTimeTable ; // Hold key delay time ��Ӧ��
  const unsigned char  *_HoldKeyTable ;     // Hold key ��Ӧ��
  const unsigned char  *_KeyTable ;         // ���������Ӧ��
  const unsigned char  *_SpecialKeyTable ;  // ���⸴�ϰ��������Ӧ��
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
