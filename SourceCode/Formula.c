#include "Formula.h"
#include "General.h"

/*******************************************************************************
* Function Name  : Formula_Calories()
* Input          : by_Type / by_Cal_Speed / by_Cal_Incline
* Output         : None
* Return         : by_Out
*******************************************************************************/
// TM 分钟卡
// speed > 4.0 
// Calories/h = ((speed x 5.36)+(incline% x speed x 24.12)+3.5)
// speed <= 4.0
// Calories/h = ((speed x 2.68)+(incline% x speed x 48.24)+3.5)
long unsigned Formula_Calories(char by_Type,long unsigned by_Cal_Speed,long unsigned by_Cal_Incline)
{
  long unsigned by_Out;
  
  if(by_Type == _TM_)
  {
      if(by_Cal_Speed > 40)
      {
          by_Out = ((by_Cal_Speed) * 53600)/10;
          by_Out += (by_Cal_Incline * by_Cal_Speed * 2412)/100;
      }
      else
      {
          by_Out = ((by_Cal_Speed) * 26800)/10;
          by_Out += (by_Cal_Incline * by_Cal_Speed * 4824)/100;
      }
      by_Out += 35000;
      by_Out /= 100;
  }
  
  return by_Out;
}
/*******************************************************************************
* Function Name  : Formula_HourCalories()
* Input          : by_Cal_Weight / by_Bu
* Output         : None
* Return         : by_Out
*******************************************************************************/
// TM 小时卡公式 :
// speed > 4.0 
// Calories/h = ((speed x 5.36)+(incline% x speed x 24.12)+3.5)x weight(kg) / 3.3333
// speed <= 4.0
// Calories/h = ((speed x 2.68)+(incline% x speed x 48.24)+3.5)x weight(kg) / 3.3333
unsigned short Formula_HourCalories(long unsigned by_Cal_Weight,long unsigned by_Bu)
{
  long unsigned by_Out;
  
  by_Out = by_Bu;
  by_Out *= by_Cal_Weight / 100;
  by_Out = (long unsigned)((float)by_Out / 3.3333) / 100;  
  return by_Out;
}
/*******************************************************************************
* Function Name  : Formula_SecCalories()
* Input          : by_Cal_Weight / by_Bu
* Output         : None
* Return         : by_Out
*******************************************************************************/
// TM cal/second 秒卡公式 :
// speed > 4.0 
// Calories/h = ((speed x 5.36)+(incline% x speed x 24.12)+3.5)x weight(kg) / 12000
// speed <= 4.0
// Calories/h = ((speed x 2.68)+(incline% x speed x 48.24)+3.5)x weight(kg) / 12000 
// note : 
// 分钟卡改为秒卡 => 12000 = 60 / 0.005
// 分钟卡公式就 / 12000 就可以了.
unsigned short Formula_SecCalories(long unsigned by_Cal_Weight,long unsigned by_Bu)
{
  long unsigned by_Out;

  by_Out = by_Bu;
  by_Out *= by_Cal_Weight;
  by_Out /= 12000;// 12000 = 60 / 0.005 (分钟卡改为秒卡)
  return by_Out+1;
}
/*******************************************************************************
* Function Name  : Formula_TM_BMI()
* Description    : BMI结果换算
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short Formula_BMI(char by_Type,char _Unit,unsigned short _Weight,unsigned short _Height)
{
  long unsigned by_DSum = 0;
  long unsigned by_HSum = 0;
  
  if(by_Type == _TM_)
  {//Modify by Garry Meng
      by_HSum = _Height * _Height;
      by_DSum = _Weight * 10000;
      by_DSum = by_DSum/by_HSum*1000 + (by_DSum%by_HSum*1000)/by_HSum;
      if(_Unit == C_MILE)
      {//==> Height unit is (in)英吋
          by_DSum *= 703;
          by_DSum /= 10;
      }
      by_DSum += 500;//四舍五入
      by_DSum /= 1000;
  }
  return by_DSum;
}
/*******************************************************************************
* Function Name  : Formula_VO2()
* Description    : VO2结果换算
* Input          : None
* Output         : None
* Return         : None
* TM : V02=56.981+(1.242*TT)-(0.805*BMI)
*******************************************************************************/
unsigned short Formula_VO2(char by_Type,unsigned short _ACCtime,unsigned short _BMI)
{
  long unsigned by_DSum = 0;
  long unsigned by_TT = 0;
  
  if(by_Type == _TM_)
  {
      by_TT = _ACCtime % 60;
      if(by_TT != 0) by_TT %= 6;
      if(by_TT == 1) by_TT=2;
      else if(by_TT == 2) by_TT=3;
      else if(by_TT == 3) by_TT=5;
      else if(by_TT == 4) by_TT=7;
      else if(by_TT == 5) by_TT=8;
      if((_ACCtime % 60) != 0)
      {
          by_TT = (((_ACCtime % 60)/6)*10)+by_TT;
      }
      if((_ACCtime / 60) != 0)
      {
          by_DSum = _ACCtime / 60 * 100;
          by_TT = by_DSum + by_TT;
      } 
      by_TT = (long unsigned)((float)by_TT * 124.2) / 10;
      by_DSum = (long unsigned)(56981+by_TT-(_BMI*0.805))/100;
  }
  return by_DSum;
}
/*******************************************************************************
* Function Name  : Formula_METs()
* Description    : METs结果换算
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short Formula_METs(char by_Type,char _Unit,unsigned short _Speed,unsigned short _Incline)
{
  unsigned short by_Out;  

  if(by_Type == _TM_)
  { 
      if(_Unit == C_KM)
      {
          if(_Speed <= 64)//==>6.4 km
          {//==>METs=(3.5+(2.68*speed mph)+(48.28*speed mph)(% incline))/3.5 
              by_Out=35 + (unsigned short)((float)(2.68*(_Speed*0.62137)))+(((u16)((float)(48.28*(_Speed*0.62137)))*_Incline)/1000);
          }
          else
          {//==>METs=(3.5+(5.36*speed mph)+(24.14*speed mph)(% incline))/3.5 
              by_Out=35 + (unsigned short)((float)(5.36*(_Speed*0.62137)))+(((u16)((float)(24.14*(_Speed*0.62137)))*_Incline)/1000);
          }
      }
      else
      {
          if(_Speed <= 40)//==>4 mile
          {//==>METs=(3.5+(2.68*speed mph)+(48.28*speed mph)(% incline))/3.5 
              by_Out=35 + (unsigned short)((float)(2.68*_Speed))+(((u16)((float)(48.28*_Speed))*_Incline)/1000);
          }
          else
          {//==>METs=(3.5+(5.36*speed mph)+(24.14*speed mph)(% incline))/3.5 
              by_Out = 35 + (unsigned short)((float)(5.36*_Speed))+(((u16)((float)(24.14*_Speed))*_Incline)/1000);
          }
      } 
      by_Out = (by_Out*10)/35;
  }
  return by_Out;
}



