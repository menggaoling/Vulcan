#include "KeyFunction.h"
#include "keyboard.h"
#include "EEPRom.h"

#define KEY_BUFFER_NUM  16

KeyFunction _Key_Data;// ����
JHTKeyFunctionLib *KeyInfor;// ����


struct {
         unsigned Flag   : 1;
         unsigned Repeat : 1;
         unsigned GO     : 1;
       } KeyScan;

unsigned int by_ReadKeyData;
unsigned char KeyValueBuffer[KEY_BUFFER_NUM];
unsigned int _KeypadTestTime = 0;
unsigned char _KeyInt = 0;
unsigned char SpecailKeyBuffer[16] = {0};
unsigned char _KeyFunctionIntSwitch = 0;  
unsigned char by_Repeat = 0;
unsigned char _BypassKeyNumber=0;
unsigned char by_Count = 0;


struct {
       unsigned char BadKey[16];
       unsigned char BypassKey[16];
       unsigned short BadKeyHoldTime[16];

       } KeyInfo ;



/*******************************************************************************
* Function Name  : KeyScan_BypassKey()
* Description    :  
* Input          : None
* Output         : None
*******************************************************************************/
void KeyScan_BypassKey(void)
{
  unsigned char i,j,k;

  if(_BypassKeyNumber != 0)
  {
      // check bypass buffer is true
      for(i=0;i<_BypassKeyNumber;i++)
      {
          for(j=0;j<16;j++)
          {
              if(SpecailKeyBuffer[j] == 0)
                  j=50;
              else if(KeyInfo.BypassKey[i] == SpecailKeyBuffer[j])
                  j=100;
          }
          if(j == 51) 
          {
              _BypassKeyNumber-=1;
              for(k=i;k<15;k++)
              {
                  KeyInfo.BypassKey[k]=KeyInfo.BypassKey[k+1];
                  if(KeyInfo.BypassKey[k] == 0) k=99;
              }
          }
      }
      // Sieve bypass key
      
      for(i=0;i<16;i++)
      {
          if(SpecailKeyBuffer[i] == 0) 
              i = 99;
          else
          {
              
              for(j=1;j<=_BypassKeyNumber;j++)
              {
                  if(SpecailKeyBuffer[i] == KeyInfo.BypassKey[j-1])
                  {
                      //��Joe�ظ���������,�����ȷ�����!������������Ҳ�����ʧЧ
                      //by_Count-=1;
                      //
                      for(k=i;k<15;k++)
                      {
                          SpecailKeyBuffer[k] = SpecailKeyBuffer[k+1];
                          if(SpecailKeyBuffer[k] == 0) k=99;
                      }
                      j=0;
                  }
              }
              
          }
      }
      
      //
  }
  // Key wrong check
  // Set bad key
  for(i=0;i<16;i++)
  {
      if(SpecailKeyBuffer[i] == 0)
          i=100;
      else
      {
          for(j=0;j<16;j++)
          {
              if(KeyInfo.BadKey[j] == SpecailKeyBuffer[i])
              {
                  j=100;
              }
              else if(KeyInfo.BadKey[j] == 0)
              {
                  KeyInfo.BadKey[j] = SpecailKeyBuffer[i];
                  j=100;
              }
          }
      }
  }
  // Sequence bad key
  for(i=0;i<15;i++)
  {
      if(KeyInfo.BadKey[i] == 0)
      {
          KeyInfo.BadKey[i]=KeyInfo.BadKey[i+1];
          KeyInfo.BadKeyHoldTime[i]=KeyInfo.BadKeyHoldTime[i+1];
      }
      else
      {
          for(j=0;j<16;j++)
          {
              if(KeyInfo.BadKey[i] == SpecailKeyBuffer[j]) j=100;
          }
          if(j == 16)
          {
              KeyInfo.BadKey[i]=0;
              KeyInfo.BadKeyHoldTime[i]=0;
              i-=1;
          }
      }
  }
  // Bad key hold time count
  for(i=0;i<16;i++)
  {
      if(KeyInfo.BadKey[i] != 0)
      {
          KeyInfo.BadKeyHoldTime[i]++;
          if(KeyInfo.BadKeyHoldTime[i] > 6000) //60's = 6000
          {
              // Get new bypass key number
              KeyInfo.BypassKey[_BypassKeyNumber]=KeyInfo.BadKey[i];
              for(j=i;j<15;j++)
              {
                  KeyInfo.BadKey[j]=KeyInfo.BadKey[j+1];
                  KeyInfo.BadKeyHoldTime[j]=KeyInfo.BadKeyHoldTime[j+1];
              }
              _BypassKeyNumber++;
              by_Repeat = 0;
          }
      }
      else
          i=99;
  }
  //
}
/*******************************************************************************
* Function Name  : KeyScan_BadKeyRegion()
* Description    : Region check 
* Input          : 1=Upper 2=Lower 3=Quick Overlay
* Output         : Yes or No
*******************************************************************************/
unsigned char KeyScan_BadKeyRegion(unsigned char by_Region)
{
  unsigned char i;
  unsigned char _Min=1;
  unsigned char _Max=UpperOverlayEnd;
  
  if(_BypassKeyNumber !=0)
  {
      if(by_Region == 2)
      {
          _Min=UpperOverlayEnd;
          _Max=LowerOverlayEnd;
      }
      else if(by_Region == 3)
      {
          _Min=LowerOverlayEnd;
          _Max=QuickKeyEnd;
      }
      for(i=0;i<16;i++)
      {
          if(KeyInfo.BypassKey[i] >= _Min && \
             KeyInfo.BypassKey[i] < _Max)
          {
              return 1;
          }
          if(KeyInfo.BypassKey[i] == 0)
          {
              return 0;
          }
      }
  }
  return 0;
}
/*******************************************************************************
* Function Name  : KeyScan_GetBypaddKeyNumber()
* Description    : Get Number
* Input          : None
* Output         : _BypassKeyNumber
*******************************************************************************/
unsigned char KeyScan_GetBypaddKeyNumber(void)
{
  return _BypassKeyNumber;
}

void KeyScan_Initial( JHTKeyFunctionLib *KeyInformation, const unsigned char *_KeyTable,
                      const unsigned char *_SpecialKey, const unsigned short *_HoldKeyTime,
                      const unsigned char *_HoldKey)
{
  KeyScan.Flag = 0;
  KeyScan.Repeat = 0;
  KeyScan.GO = 0;
  _KeypadTestTime = 0;
  
  KeyInfor = KeyInformation;
  KeyInfor->KeyScanNumber = 8 ;
  KeyInfor->KeyDataNumber = 8 ;
  KeyInfor->_KeyCheckNumber = 10 ;
  KeyInfor->_KeyPress = 5 ; 
  KeyInfor->_KeyTable = _KeyTable ;
  KeyInfor->SpecialKeyGroup = 3 ;
  KeyInfor->SpecialKeyInforNumber = 4 ; 
  KeyInfor->_SpecialKeyTable = _SpecialKey ;
  KeyInfor->HoldKeyColseTime = 10000 ;
  KeyInfor->HoldKeyFistTime = 300 ;
  KeyInfor->HoldKeyLoopChangeTime = 3000 ;
  KeyInfor->HoldKeyHighSpeedLoop = 60 ;
  KeyInfor->HoldKeyLowSpeedLoop = 200 ;
  KeyInfor->HoldKeyNumber = 5 ;
  KeyInfor->HoldKeyTimeGroup = 3 ;
  KeyInfor->_HoldKeyTimeTable = _HoldKeyTime ;
  KeyInfor->_HoldKeyTable = _HoldKey ;
  KeyInfor->HoldKeyMASK = 0x0200;
  KeyInfor->KeypadeTest = 3 ;
}
/*******************************************************************************
* Function Name  : KeyScan_SetKeyIntSwitch()
* Description    : �жϹ��ܿ��� 
*                  _KeyIntClose / _KeyIntOpen
* Input          : None
* Output         : None
*******************************************************************************/
void KeyScan_SetKeyIntSwitch(char by_D)
{
  _KeyFunctionIntSwitch = by_D ;
}
char KeyScan_GetKeyIntSwitch(void)
{
  return _KeyFunctionIntSwitch ;
}
/*******************************************************************************
* Function Name  : KeyScan_Int()
* Description    : ɨ���ж� 1`ms
* Input          : None
* Output         : None
*******************************************************************************/
void KeyScan_Int(void)
{
  
  unsigned char KeyBuffer[4] = {0};
//   static char by_Loop = 0;
   unsigned char by_KeyNum = 0,i,j;
//   static char _KeySum = 0;
   
   if(_KeyFunctionIntSwitch == _KeyIntOpen)
   {
       KeyScan.Flag = 1;
       if(_KeyInt < 10)
       {
           _KeyInt++;
       }
       by_KeyNum = Key_Value_Read(KeyBuffer);
       if(by_KeyNum > 0)
       {         
         for(i = 0; i< by_KeyNum;i++)
         {
           if((KeyBuffer[i] & 0x7F) > 35) //invalid key value
             continue;
           if(KeyBuffer[i] & 0x80)
           {
             for(j= 0 ;j < KEY_BUFFER_NUM ;j++)
             {
               if(KeyValueBuffer[j] == 0)
               {
                 KeyValueBuffer[j] = KeyBuffer[i];
                 break;
               }
             }
             
           }
           else
           {
             for(j= 0 ;j < KEY_BUFFER_NUM ;j++)
             {
               if((KeyBuffer[i] | 0x80) == KeyValueBuffer[j])
               {
                 KeyValueBuffer[j] = 0;
               }
             }
             
           }
           
         }
         
       }
   }
}

/*******************************************************************************
* Function Name  : KeyScan_GetKeyNumber
* Description    : ������Ӧ�����ж� (����������ϼ�)
* Input          : None
* Output         : None
* Return         : by_Out ; ��Ӧ��������
*******************************************************************************/
unsigned int KeyScan_GetKeyNumber(void)
{
//  unsigned int by_Check;
  unsigned int _Seg = 0;
  unsigned int by_Out = 0;
  unsigned char by_x,by_x1,i;
  unsigned char _SpecialKeyCheckSum = 0;
  
  by_Count = 0;
  by_x = KeyInfor->KeyScanNumber;
  by_x1 = KeyInfor->KeyDataNumber;
  
  for(by_x = 0;by_x < KeyInfor->KeyScanNumber;by_x++)
    {//==> x ��ɨ�� pin
      if(KeyValueBuffer[by_x] != 0)
      {//==>�а���ʱ��������ȡ�� 
//        by_Out = (KeyValueBuffer[by_x] & 0x7F);
        by_Out = *(KeyInfor->_KeyTable + (KeyValueBuffer[by_x] & 0x7F));
        SpecailKeyBuffer[by_Count] = by_Out;
        by_Count++;                
      }
  }
  
  // Sieve bad key
  if(EEPROM_ReadInformation(EE_KeyStuck) == D_Enable)
  {
      if(by_Out != 0)
      {
          KeyScan_BypassKey();
          by_Out = SpecailKeyBuffer[by_Count-1];
      }
      else
      {
          // Clear all Bay key count
          if(KeyInfo.BadKey[0] != 0)
          {
              for(i=0;i<16;i++)
              {
                  KeyInfo.BadKey[i]=0;
                  KeyInfo.BadKeyHoldTime[i]=0;
              }
          }
          if(_BypassKeyNumber != 0)
          {
              _BypassKeyNumber=0;
              for(i=0;i<16;i++)
              {
                  KeyInfo.BypassKey[i]=0;
              }
          }
          //
      }
  }
  // Quick key change to control key
  if(by_Out >= K_QuickSpeedU && \
     by_Out <= K_QuickSTOP)
  {
      by_Out=(by_Out-K_QuickSpeedU)+K_SpeedU;
  }
  //

  //
  if(by_Count >= 2)
  {//==>������ϼ�ʱ����ȶԳ���Ӧ�Ĵ���
      
      by_Repeat = 1;
      for(by_x = 0;by_x < KeyInfor->SpecialKeyGroup;by_x++)
      {//==>���ⰴ������
          for(i = 0;i < by_Count;i++)
          {//==>����������
              for(by_x1 = 0;by_x1 < KeyInfor->SpecialKeyInforNumber - 1;by_x1++)
              {//==> �������Ӧ��
                  //if(_SpecialKey[by_x][by_x1] != 0)
                  _Seg = (by_x * KeyInfor->SpecialKeyInforNumber) + by_x1;
                  if(*(KeyInfor->_SpecialKeyTable + _Seg) != 0) 
                  {//==> ���������Ӧ��
                      _Seg = (by_x * KeyInfor->SpecialKeyInforNumber) + by_x1;
                      if(SpecailKeyBuffer[i] == *(KeyInfor->_SpecialKeyTable + _Seg))//_SpecialKey[by_x][by_x1])
                      {
                          _SpecialKeyCheckSum++;
                      }
                  }
                  if(_SpecialKeyCheckSum == by_Count)
                  {//==>������ⰴ�������ȶԲ����
                      //by_Out = _SpecialKey[by_x][DF_SpecialKeyInforNumber - 1];
                      _Seg = (by_x * KeyInfor->SpecialKeyInforNumber) + (KeyInfor->SpecialKeyInforNumber - 1);
                      by_Out = *(KeyInfor->_SpecialKeyTable + _Seg);
                      by_x = KeyInfor->SpecialKeyGroup;
                      by_x1 = KeyInfor->SpecialKeyInforNumber;
                      i = by_Count;
                  }
              }
          }
          if(_SpecialKeyCheckSum != by_Count)
          {//==>�ޱȶԵ����ⰴ��
              _SpecialKeyCheckSum = 0;
          }
      }
      if(_SpecialKeyCheckSum != by_Count)
      {//==>�ޱȶԵ����ⰴ���Ͳ����
          by_Out = 0x00;
          
      }
      for(by_x = 0;by_x < 16;by_x++) SpecailKeyBuffer[by_x] = 0;//KeyInfor->SpecialKeyGroup
  }
  else 
  {
      if(by_Repeat == 1)
      {//==>���а�����ϼ�ʱ��Ҫ�ڷſ����а�����Ż��ٳ����°���
          if(by_Out == 0)//==>�ſ����а���
              by_Repeat = 0;
          else //==>��û�зſ������������¾�ȡ��������������
              by_Out = 0;
      }
      SpecailKeyBuffer[0] = 0;
  }
  return by_Out;
}
/*******************************************************************************
* Function Name  : KeyScan_Proce
* Description    : ������������ٻ�HOLD������������ж�
* Input          : None
* Output         : None
* Return         : by_Out ; ��Ӧ��������
*******************************************************************************/
unsigned int KeyScan_Proce(void)
{
  static unsigned int by_Hold_Time = 0;
  static unsigned int by_Replay_Time = 0;
  unsigned int _Seg = 0;
  unsigned int by_Out = 0;
  unsigned int by_Dat;
  char by_replay;
  
  
  char i,j;
  
  if(_KeyInt >= 10 && _KeyFunctionIntSwitch == _KeyIntOpen)
  {//==>ÿ10ms���жϴ�ʱ����һ��
      _KeyInt = 0;
      by_Dat = KeyScan_GetKeyNumber();//==>ȡ�ð�������
      by_Out = 0x00;
      if(by_Dat != 0x00)
      {//==>���а���ʱ
          /*
          if(by_Dat == K_GO)
          {// Press GO key,But not return key number
              KeyScan.GO = 1;           
              return 0x00;
          }
          */
          if(by_Hold_Time > (KeyInfor->HoldKeyColseTime / 10))
          {//==>������������Լx��ʱ��ȡ���˰���
              return 0x00;
          }
          else
          {
              by_Hold_Time++;// ������ʱ�㿪ʼ���� 
              if(by_Hold_Time == 1)
              {//==>������һ�γ���ʱ
                  by_Out = by_Dat;
                  if(by_Dat != 0)
                  {//==> ������ȷ��ַʱ
                      // ���ΰ�����������              
                      BEEP_Proce(2,2,1);
                        
                  }
              }
              if(by_Dat == K_GO)
              {// Press GO key,But not return key number
                  KeyScan.GO = 1;  
                  return 0x00;
              }
              if(by_Hold_Time >= (KeyInfor->HoldKeyFistTime / 10))
              {//==>hold x ms ��
               //==>���׶μ���
                  if(by_Hold_Time >= (KeyInfor->HoldKeyLoopChangeTime / 10))//==> hold Լ x ms������л�
                      by_replay = KeyInfor->HoldKeyHighSpeedLoop / 10;//==> ÿ x ms�ۼӨ��
                  else 
                      by_replay = KeyInfor->HoldKeyLowSpeedLoop / 10;//==> ÿ x ms�ۼӨ��
                  
                  for(i = 0;i < KeyInfor->HoldKeyTimeGroup;i++)
                  {
                      for(j = 0;j < KeyInfor->HoldKeyNumber;j++)
                      {
                          if(*(KeyInfor->_HoldKeyTimeTable +i) == 1)
                          {//==> һ����ټ�
                              if(by_Replay_Time >= by_replay)
                              {
                                  _Seg = (i * KeyInfor->HoldKeyNumber) + j;
                                  if(by_Dat == *(KeyInfor->_HoldKeyTable + _Seg))//_HoldKey[i][j])
                                  {
                                      by_Out = by_Dat | KeyInfor->HoldKeyMASK;
                                      j = KeyInfor->HoldKeyNumber;
                                  }
                              }
                              else j = KeyInfor->HoldKeyNumber;
                          }
                          else
                          {//==> �����
                              if((*(KeyInfor->_HoldKeyTimeTable +i) / 10) == by_Hold_Time)
                              {
                                  _Seg = (i * KeyInfor->HoldKeyNumber) + j;
                                  if(by_Dat == *(KeyInfor->_HoldKeyTable + _Seg))//_HoldKey[i][j])
                                  {
                                      by_Out = by_Dat | KeyInfor->HoldKeyMASK;
                                      j = KeyInfor->HoldKeyNumber;
                                  }
                              }
                              else j = KeyInfor->HoldKeyNumber;
                          }
                      }
                  }
                  if(by_Replay_Time < by_replay)
                      by_Replay_Time++;
                  else
                      by_Replay_Time = 0;
              }
              
          }
      }
      else
      {
          if(KeyScan.GO == 1)
          {// If had press GO key,unloose GO key will return GO number key
              KeyScan.GO = 0;
              by_Out = K_GO;
          }
          by_Hold_Time = 0;
      }

  }
  return by_Out;
}
/*******************************************************************************
* Function Name  : KeyScan_KeypadTest()
* Description    : ����ʱkeypad������
* Input          : None
* Output         : Not press key / Press key / Key Fail
********************************************************************************/
char KeyScan_KeypadTest(void)
{
  char by_S = 100;
  
  if(KeyScan.Flag)
  {
      KeyScan.Flag = 0;
      _KeypadTestTime++;
  }
  if(_KeypadTestTime > 200)//==>��ȷ�����а�������ɨ��һ��,���ǰ200ms�Ȳ��ж�
  {
      if(KeyScan_GetKeyNumber() != 0)
      {
          if(_KeypadTestTime - 200 >= (KeyInfor->KeypadeTest * 1000))// ����x�밴��
              by_S = _KeyTest_KeyFail;
          else 
              by_S = _KeyTest_Presskey;
      }
      else by_S = _KeyTest_NotPress; 
  }
  return by_S;
}


