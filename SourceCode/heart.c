#include "General.h"
#include "heart.h"
#include "Beep.h"

#define  C_HEART_ONE_MINUTE    60000 //  1分钟
#define  C_HEART_LOW_LIMIT     C_HEART_ONE_MINUTE/240  // 240BPM, ==> 1BPM = 250ms 
#define  C_HEART_HIGH_LIMIT    C_HEART_ONE_MINUTE/40   //  40BPM, ==> 1BPM = 1500ms
#define  HR_MEM_LONG           20
#define  HR_OFFSET             10
#define  HR_OUT_NUM            5
#define  HR_CLEAR_NUM          3
#define  HR_DELAY_DECTECT      2


void MEM_Clear(unsigned char *pby_Dat);

unsigned short w_Polar_Counter=C_HEART_HIGH_LIMIT;
unsigned short w_Heart_Data=C_HEART_HIGH_LIMIT;
unsigned short w_Heart_Data1=C_HEART_HIGH_LIMIT;//心跳脉宽暂存
unsigned char by_Heart;
unsigned char by_Target;
unsigned char by_Target_HR;//==>追击心跳用目标值
unsigned char aby_Heart[HR_MEM_LONG];
unsigned char by_Jump;
unsigned short _HR_BlinkDelayTime;
struct
{
  unsigned Heart_OK        : 1 ;
  unsigned Start           : 1 ;
  unsigned Beep            : 1 ;//==>蜂鸣器是否响起 CG ADD
  unsigned HR              : 1 ;//==>假心跳判断用
  unsigned HR_Check        : 1 ;
  unsigned Over_Speed      : 1 ;//==>作为讯号噪声是否进入判断
  unsigned Over            : 1 ;//==>第一次噪声进入
  unsigned HoldTime        : 1 ;
  unsigned Blink           : 1 ;
  unsigned BlinkStart      : 1 ;
} HR_State;
/*******************************************************************************
* Function Name  : Heart_Initial
* Description    : 参数预设
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Heart_Initial(void)
{
  by_Jump=0;
  HR_State.Start=0;
  HR_State.HR=0;
  by_Target=80;
  by_Target_HR=0;
  HR_State.Heart_OK=0;
  HR_State.Beep=0;
  by_Heart=0;
  HR_State.HR_Check=0;
  HR_State.Over_Speed=0;
  HR_State.Over=0;
  HR_State.HoldTime=0;
  HR_State.Blink=0;
  HR_State.BlinkStart = 0;
  MEM_Clear(aby_Heart);
}
/*******************************************************************************
* Function Name  : Heart_Stop
* Description    : Heart Rate 停止做动
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Heart_Stop(void)
{
  HR_State.Start=0;
}
/*******************************************************************************
* Function Name  : Heart_Start
* Description    : Heart Rate 开始做动
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Heart_Start(void)
{
  HR_State.Start=1;
}
/*******************************************************************************
* Function Name  : Heart_1ms_Int
* Description    : 1ms Interrupt
* Input          : None
* Output         : None
* Return         : None
___   __________   __________   __________
   | |          | |          | |
   | |          | |          | |
   ---          ---          ---
 15~50ms      15~50ms      15~50ms
*******************************************************************************/
void Heart_1ms_Int(void)
{
  static unsigned short w_Time_ms=0;
  static unsigned short w_Delay=0;
  static unsigned short HR_Signal_Hi = 0;
  static unsigned short HR_Signal_Lo = 0;
  static unsigned char HR_Detect = 0;
  
  //--------------------------------------------------------------------------
  // 功能时间累计 
  if(!HR_State.HR_Check)
  {
      if(++w_Delay >= 1000)
      {//==>每1秒变更一次心跳值
          w_Delay=0;
          HR_State.HR_Check=1;
      }
  }
  
  if(HR_State.Beep)
  {
      if(++w_Time_ms >= 3000)
      {//==>3秒后
          HR_State.Beep=0;
          w_Time_ms = 0;
      }
  }
  else 
      w_Time_ms=0;
  
  //--------------------------------------------------------------------------
  if(++w_Polar_Counter > C_HEART_HIGH_LIMIT)
  {
      w_Heart_Data=w_Polar_Counter;
      
      w_Polar_Counter=0;
      HR_State.Heart_OK=1;
      
      HR_Detect = 0;
      HR_Signal_Hi = 0;
      HR_Signal_Lo = 0;
      return;
  }
  //---------------------------------------------------------------------------
  // Catch the HR Wave
  if(IO_HeartRateSignal())
  {// High
      if(HR_Signal_Lo != 0)
          HR_Detect |= 0x02;
      HR_Signal_Hi++;
  }
  else
  {// Low
      if(HR_Signal_Hi != 0)
          HR_Detect |= 0x01; 
      HR_Signal_Lo++;
  }
  if((HR_Detect & 0x03) == 0x03)
  {// Complete the catch
      if(HR_Signal_Lo < 15 || HR_Signal_Lo > 50)
      {
          HR_Detect = 0;
          w_Polar_Counter = 0;
      }
      else
      {
          if(w_Polar_Counter < C_HEART_HIGH_LIMIT)
              HR_Detect |= 0x80;
          else
          {
              HR_Detect = 0;
              w_Polar_Counter = 0;
          }
      }
      HR_Signal_Hi = 0;
      HR_Signal_Lo = 0;   
      //HR_State.BlinkStart = 1;
  }
  if((HR_Detect & 0x80) == 0x80)
  {
      if(BEEP_Get_OK())
      {//==>当蜂鸣器再响时停止心跳撷取
          if(!HR_State.Beep) 
              HR_State.Beep=1;//==>蜂鸣器再叫时将设为 1
          w_Heart_Data=w_Heart_Data1;
      }
      else
      {
          if(w_Polar_Counter > C_HEART_LOW_LIMIT)  
          {
              if(HR_State.Beep)
              {//==>蜂鸣器在叫时就放入前一比暂存心跳数据
                  w_Heart_Data=w_Heart_Data1;
              }
              else
              {
                  w_Heart_Data1=w_Polar_Counter;
                  w_Heart_Data=w_Polar_Counter;
              }
          } 
      }
      HR_Detect = 0;
      w_Polar_Counter=0;
      HR_State.Heart_OK=1;
      HR_State.HoldTime=1;
  }
  //--------------------------------------------------------------------------
  if(HR_State.BlinkStart == 1)
  {// HR signal blink
      _HR_BlinkDelayTime++;
      if(_HR_BlinkDelayTime >= 300)
      {
          _HR_BlinkDelayTime = 0;
          HR_State.Blink = ~HR_State.Blink;
      }
  }
  //--------------------------------------------------------------------------
}

/*******************************************************************************
* Function Name  : MEM_FIFO
* Description    : 心跳数据位移
* Input          : *pby_Dat (资料 Buffer)
* Output         : None
* Return         : None
*******************************************************************************/
void MEM_FIFO(unsigned char *pby_Dat)
{
  unsigned char loop;
  for(loop=HR_MEM_LONG-1;loop > 0 ;loop--)
  {
      *(pby_Dat+loop)=*(pby_Dat+loop-1);
  }
}
/*******************************************************************************
* Function Name  : MEM_Clear
* Description    : 心跳数据暂存清除
* Input          : *pby_Dat (资料 Buffer)
* Output         : None
* Return         : None
*******************************************************************************/
void MEM_Clear(unsigned char *pby_Dat)
{
  unsigned char loop;
  for(loop=0;loop < HR_MEM_LONG;loop++)
  {
      *(pby_Dat+loop)=0;
  }
}
/*******************************************************************************
* Function Name  : MEM_Cul
* Description    : 心跳数据达特定范围值笔数比较
* Input          : *pby_Avg,*pby_Dat (资料 Buffer)
* Output         : None
* Return         : by_Max_Count (次数)
*******************************************************************************/
unsigned char MEM_Cul(unsigned char *pby_Avg,unsigned char *pby_Dat)
{
  unsigned char loop,count=0;
  unsigned char by_Max_Count=0;
  unsigned short w_Max_Total=0;
  unsigned short total=0;
  
  for(loop=0;loop < HR_MEM_LONG;loop++)
  {
      if(*(pby_Dat+0)<= 40)
      { 
          if(*(pby_Dat+loop)==0) 
          {
              if(++count > HR_CLEAR_NUM) 
                  count = HR_MEM_LONG;
          }
      }
      else
      {//判断是否为正负10以内 
          if(*(pby_Dat+loop)<=((*(pby_Dat+0))+HR_OFFSET) && *(pby_Dat+loop)>=((*(pby_Dat+0))-HR_OFFSET))
          {
              ++count;
              total+=*(pby_Dat+loop);  
          }
      } 
  }
  if(count > by_Max_Count)
  {
      by_Max_Count = count;
      w_Max_Total = total;
  }

  *pby_Avg = (by_Max_Count==0)?0:(w_Max_Total/by_Max_Count);
  
  return(by_Max_Count); 
}
/*******************************************************************************
* Function Name  : Heart_Decode
* Description    : 心跳数据计算
* Input          : None
* Output         : by_Heart
* Return         : b_Out (是否进入计数)
*******************************************************************************/
void Heart_Decoder(void)
{
  unsigned char by_Dat,by_Count;

  if(HR_State.Heart_OK)
  {
      HR_State.Heart_OK=0;
      if(by_Jump >= HR_DELAY_DECTECT)
      {//==>第一笔心跳要先在约2秒后开始取样
          HR_State.BlinkStart = 1;
          
          if(w_Heart_Data >= C_HEART_LOW_LIMIT)
          {
              by_Dat = (w_Heart_Data < C_HEART_HIGH_LIMIT)?(C_HEART_ONE_MINUTE/w_Heart_Data):0;
              MEM_FIFO(aby_Heart);
              aby_Heart[0]=by_Dat;
              by_Count=MEM_Cul(&by_Dat,aby_Heart);
              if(by_Count > HR_OUT_NUM)
              {//==>数据累计次数是否 > HR_OUT_NUM
                  by_Heart=by_Dat;
                  if(by_Heart==0)
                  {
                      by_Jump=0;
                      MEM_Clear(aby_Heart);
                      HR_State.HoldTime=0;
                      HR_State.HR=0;
                      HR_State.Over=0;
                      HR_State.Over_Speed=0;
                      by_Target_HR=0;
                      w_Heart_Data1=C_HEART_HIGH_LIMIT;
                      HR_State.BlinkStart = 0;
                      HR_State.Blink = 0;
                  }
              }
              else
              {
                  if(by_Heart > 40)
                  {
                      if(by_Heart > by_Dat) 
                          --by_Heart;
                      if(by_Heart < by_Dat) 
                          ++by_Heart;
                  }
              }
          }
      }
      else
      {
          if(HR_State.HoldTime && HR_State.HR_Check)
          {
              HR_State.HR_Check=0;
              by_Jump++;
          }
      }
  }
}
/*******************************************************************************
* Function Name  : Heart_Get_Heart
* Description    : 心跳数据取得
* Input          : None
* Output         : None
* Return         : by_Target_HR (显示用心跳值)
*******************************************************************************/
unsigned char Heart_Get_Heart(void)
{
  static unsigned char by_DelayTime;
  if(HR_State.Start)
  {
      if(by_Heart==0)
      {
          by_DelayTime=0;
          return(0);
      }
      else
      {
          if(!HR_State.HR)
          {//==>显示第一次秀值取得判断
              if((by_Heart >= 180) && (by_DelayTime < 8))
              {
                  if(HR_State.HR_Check) 
                      by_DelayTime++;
                  HR_State.HR_Check=0;
                  return(0);
              }
              else
              {
                  by_DelayTime=0;
                  HR_State.HR=1;
                  by_Target_HR=by_Heart;
              }
          }
          else
          {
              if(HR_State.HR_Check)
              {//==>每1秒进行一次
                  HR_State.HR_Check=0;
                  if(!HR_State.Over &&(by_Heart > (by_Target_HR+80)))
                  {//==>第一次进入噪声判断时
                      HR_State.Over=1;
                      HR_State.Over_Speed=1;
                  }
                  if(HR_State.Over_Speed)
                  {//==>类似噪声判断累计6秒
                      if((by_DelayTime > 6)||(by_Heart < (by_Target_HR+50))) HR_State.Over_Speed=0;
                      else by_DelayTime++;
                  }
                  if(!HR_State.Over_Speed)
                  {//==>心跳显示值
                      by_DelayTime=0;
                      if((by_Target_HR >= (by_Heart-20))&&(by_Target_HR <= (by_Heart+20)))
                      {//==>当在标准正负20下时就将下次噪声判断的旗标设为 0
                          HR_State.Over=0;
                      }
                      if((by_Target_HR >=(by_Heart-10))&&(by_Target_HR <=(by_Heart+10)))
                      {//==>当在标准正负10下时
                          if(by_Target_HR < by_Heart) by_Target_HR++;
                          else if(by_Target_HR > by_Heart) by_Target_HR--;
                      }
                      else if((by_Target_HR > (by_Heart+10))) by_Target_HR-=5;
                      else if((by_Target_HR < (by_Heart-10))) by_Target_HR+=5;
                  }
              }
          }
          return(((by_Target_HR > _HeartRate_MaxBPM)?_HeartRate_MaxBPM:by_Target_HR));
      }
  }
  else 
  {
      return(0);
  }
}
/*******************************************************************************
* Function Name  : Heart_Set_Target
* Description    : 目标心跳值设定
* Input          : by_Dat
* Output         : by_Target
* Return         : None
*******************************************************************************/
void Heart_Set_Target(unsigned char by_Dat)
{
  by_Target=by_Dat;
}
/*******************************************************************************
* Function Name  : Heart_Get_Target
* Description    : 取得目标心跳值
* Input          : None
* Output         : None
* Return         : by_Target
*******************************************************************************/
unsigned char Heart_Get_Target(void)
{
  return(by_Target);
}
/*******************************************************************************
* Function Name  : Heart_Signal(void)
* Description    : 取得是否有心跳讯号
* Input          : None
* Output         : None
* Return         : by_Jump (0:无讯号  1,2,3:有讯号)
*******************************************************************************/
unsigned char Heart_Signal(void)
{
  return ((by_Jump>=HR_DELAY_DECTECT)?1:0);
}
/*******************************************************************************
* Function Name  : Heart_GetBlink()
* Description    : 闪烁判断
* Input          : None
* Output         : None
* Return         : HR.Blink
*******************************************************************************/
unsigned char Heart_GetBlink(void)
{
  return HR_State.Blink;
}
