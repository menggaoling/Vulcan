


// File System and usb host system
//#include "hcc_types.h"
//#include "usb_host.h"
#include "pmgr.h"
#include "usb_mst.h"
#include "fat.h"
#include "mst.h"
//#include "os.h"
#include "BOOTflash.h"
#include "Boot.h"
#include "UsbFunction.h"
#include "General.h"
#include "HT1381.h"
#include "keyboard.h"
#include "Show.h"
#include "led.h"
//#include "voice.h"
#include "Beep.h"
#include "Console.h"
#include "stm32uh_hub.h"
#include "EEPRom.h"
#include "timer.h"
//#include "WiFi.h"
#include "Digital.h"
#include "webfile.h"
#include "C_safe.h"
#include <stdio.h>
#include "wifi_Digital.h"
//#include "wifi_api.h"
#include "wifi_interface.h"
#include "WiFi_Information.h"
#include "Show_Functions.h"

//#include <string.h>
// 2013.07.26
const char *const LANG_PATH  = {"/MATRIX/FW/LANGUAGE"};
const char *const LANG_BIN = {"/MATRIX/FW/LANGUAGE/*.bin"} ;
const char *const PATTERN_PATH  = {"/MATRIX/FW/PATTERN"};
const char *const PATTERN_BIN = {"/MATRIX/FW/PATTERN/*.bin"} ;
//char UsbFunction_SearchFunctionBINfile(char*filename,char*retfilename); 
char UsbFunction_SearchBINfile(char*filename,char*retfilename,char by_Mode);
void UsbFunction_ClearFileName(void);
//char UsbFunction_SearchBINfile(char*filename,char*retfilename);
//
char UsbFunction_CheckBINFileInformation(char *filename,char *sAddr,long tAddr ,char by_D1);

extern const char *const ROOT_Path ;
extern const char *const JHT_Path;
extern const char *const FW_PATH;
extern const char *const FWF_PATH;
extern const char *const FW_BIN ;
extern const char *const FWF_BIN  ;
extern const char *const PROGRAM_PATH;
extern const char *const USERS_Path ;
extern const char *const USER_Path[4] ;
extern const char *const USER_Wrk_File[5];//USER_Wrk_File[4][5] ;

extern const unsigned char User_Manufacture[52] ;
extern const unsigned char User_McuType[20] ;
extern const unsigned char User_ModuleName[20] ;
extern const unsigned char User_ModuleNo[20] ;
extern const unsigned char User_Product[20] ;
extern const unsigned char User_Version[20] ;

extern const char User_Default[] ;
extern const char Workout_Default[];

extern char TempFilename[256] ;//==> (Boot.c)
extern unsigned long CheckTime  ;

#define Max_Buffer 1024

void UsbFunction_Protection(void);

F_FILE *file90;

char USB_WriteBuffer[Max_Buffer] = {0};
char USB_ReadBuffer[Max_Buffer] = {0};
char USB_FileNameMerge[30] = {0};//255
char Save_USER = 1;
char Save_WorkoutSummary = 1;
//char Total_Program = 0;//==>Program数量
//char Prg_Name[10][20] = {0};//==>Prg文件名,最多可放10组
//char Program_Name[10][20] = {0};//==>prg档案内的PROGRAM名称,最多可放10组
//char Program_Table[7][50] = {0};//==>对应PROGRAM的1组Level Table
//char Program_Level[10] = {0};//==>Prg档案内的 LEVEL 值暂存
//char Program_Segamount[10] = {0};//==>Prg档内的 Segamount 值暂存
char Total_BIN = 0;//==>Bin档数量
char USB_Status = 0;//==>目前是否插入USB
char USB_FirstCheck = 0;//==>第一次确认随身碟档案
char USB_WrkFialCheck = 0;//==>第一次wrk档确认搜询
char Update_Ver[40] ;
char Update_Name[50] ;

unsigned short by_CheckSite = 0;//==>数据搜寻位置

extern unsigned char Bin_Name[10][40] = {0};//==>BIN档名称,最多可放10组 

// Add by Kunlung 20110125
volatile unsigned short USBProcessTime = 0 ;
volatile unsigned short USBProcessTime1 = 0 ;
volatile unsigned char USBPlugin = 0x5a ;
volatile unsigned char USBErrorCount = 0 ;
//------------------------------------------------------------------------------


struct
{
    unsigned short Year ;   // 2 byte         
    unsigned short Month ;  // 2 byte      
    unsigned short Date ;   // 2 byte      
    unsigned int Averpace ; // 2 byte 
    unsigned int Calories ; // 2 byte 
    unsigned int Distance ; // 2 byte 
    unsigned int Totalelev ;// 2 byte 
    unsigned int TotalTime ;// 2 byte 
} _WorkoutSummary;


  /*
void TEST_RW(void)
{
  
  //_WorkoutSummary.Year = 2010;
  //_WorkoutSummary.Month = 8;
  //_WorkoutSummary.Date = 9;
  //_WorkoutSummary.Averpace = 111;
  //_WorkoutSummary.Calories = 76;
  //_WorkoutSummary.Distance = 8;
  //_WorkoutSummary.Totalelev = 1003;
  //_WorkoutSummary.TotalTime = 300;
  
  
  //_WorkoutSummary.Year = 2010;
  //_WorkoutSummary.Month = 8;
  //_WorkoutSummary.Date = 9;
  //_WorkoutSummary.Averpace = 100;
  //_WorkoutSummary.Calories = 200;
  //_WorkoutSummary.Distance = 301;
  //_WorkoutSummary.Totalelev = 400;
  //_WorkoutSummary.TotalTime = 0;
  
  
  
  //==> ex. 取出所有PROGRAM名称然后在取单一TABLE
  //UsbFunction_Get_ProgramName(); 
  //UsbFunction_Get_Level_Table(2,3);//==>1:program number/ 1:Level
  
  //==> ex. 显示第1个PROGRAM的名称 (USB中最多可以放10组PROGRAM)
  //Led_String1(UsbFunction_ProgramNameMessage(1));
  
  //==> ex. 显示第1个BIN的名称 (USB中最多可以放10组符合条件的 .bin 檔)
  //==> Total_BIN 参数为 .bin 档案的数量,可供后续显示对应使用
  //Led_String1(UsbFunction_BinFileNameMessage(1));
  
  //==> ex. 将运动信息放入.wrk档内
  UsbFunction_WorkoutSummary();
  
  //==> ex. 强制软件更新为第3个.bin檔
  //UsbFunction_AssignSoftwareUpdate(3);
  
  
}
  */ 
// 20110704
//unsigned char WiFi_LCBFirstCheck = 0;//==>第一次确认WIFI档案
//unsigned char WiFi_UCBFirstCheck = 0;//==>第一次确认WIFI档案
//



//==============================================================================
//==>> UsbFunction_BinFileNameMessage()
//==>> 秀出BIN档名称用
//==>> by_D:BIN编号
//==>> 回传地址
//==============================================================================
unsigned char* UsbFunction_BinFileNameMessage(char by_D)
{
  if(by_D == 0 || by_D > Total_BIN) by_D = 1;
  return &Bin_Name[by_D - 1][0];
}

//==============================================================================
//==>> UsbFunction_ProgramNameMessage()
//==>> 秀出PROGRAM名称用
//==>> by_D:PROGRAM编号
//==>> 回传地址
//==============================================================================
//char* UsbFunction_ProgramNameMessage(char by_D)
//{
//  if(by_D == 0 || by_D > Total_Program) by_D = 1;
//  return Program_Name[by_D - 1];
//}
//==============================================================================
//==>> UsbFunction_GetProgramInformation()
//==>> 取得指定PROGRAM内的值
//==>> by_D1:PROGRAM编号
//==>> by_D2:回传的参数性质 1:LEVEL 2:Segamount
//==============================================================================
/*
char UsbFunction_GetProgramInformation(char by_D1,char by_D2)
{
  char by_D = 0;
  if(by_D1 != 0) by_D1 -= 1;
  
  switch(by_D2)
  {
      case 1:by_D = Program_Level[by_D1];
             break;
      case 2:by_D = Program_Segamount[by_D1];
             break;
  }
  
  return by_D;
}
*/
//==============================================================================
//==>> UsbFunction_GetTable()
//==>> 取得指定Table值
//==>> by_D1:信息编号
//==>> 0:Table Total time
//==>> 1:time
//==>> 2:incline
//==>> 3:speed
//==>> 4:resistance
//==>> 5:message
//==>> 6:repeat
//==>> by_D2:Segamount 第x排
//==>> PS. Table 总排数放在 Program_Table[0][1]
//==============================================================================
//char UsbFunction_GetTable(char by_D1,char by_D2)
//{ 
//  if(by_D1 > 6) return 0;
//  return Program_Table[by_D1][by_D2];
//}
//==============================================================================
//==>> UsbFunction_SetWorkoutSummaryNumber()
//==>> 将运动数据放入指定位置
//==>> by_D1:位置 
//==>> by_D2:所要放入的值
//==============================================================================
/*
void UsbFunction_SetWorkoutSummaryNumber(char by_D1,unsigned int by_D2)
{
  switch(by_D1)
  {
      case IN_Year:
              _WorkoutSummary.Year = by_D2;
              break;
      case IN_Month:
              _WorkoutSummary.Month = by_D2;
              break;
      case IN_Date:
              _WorkoutSummary.Date = by_D2;
              break;
      case IN_Averpace:
              _WorkoutSummary.Averpace = by_D2;
              break;
      case IN_Calories:
              _WorkoutSummary.Calories = by_D2;
              break;
      case IN_Distance:
              _WorkoutSummary.Distance = by_D2;
              break;
      case IN_Totalelev:
              _WorkoutSummary.Totalelev = by_D2;
              break;
      case IN_TotalTime:
              _WorkoutSummary.TotalTime = by_D2;
              break;
              
  }
}
*/
//==============================================================================
//==>> UsbFunction_ClearReadWriteBuffer()
//==>> 清除暂存区块
//==>> 0:写入区块 / 1:读出区块 / 2:Table区块
//==============================================================================
void UsbFunction_ClearReadWriteBuffer(char by_D)
{
  unsigned int i;
  //unsigned char j;
  
  if(by_D > 1)
  {
      /*
      for(i = 0 ; i < 50 ; i++)
      {
          for(j=0;j<=6;j++)
          {
              Program_Table[j][i] = 0;
          }
      }
      */
  }
  else
  {
      for(i = 0 ; i < Max_Buffer ; i++)
      {
          if(by_D == 0)
              USB_WriteBuffer[i] = 0;
          else
              USB_ReadBuffer[i] = 0;
      }
  }
}
//==============================================================================
//==>> UsbFunction_WriteInformation()
//==>> 写入各项信息用
//==>> by_Dat = 数据存储类别 
//==>> Workout summary : 0xF1
//==>> User : 0xF2
//==============================================================================
/*
void UsbFunction_WriteInformation(char by_Dat)
{
  F_FILE *file91;
  unsigned int i ;
  unsigned int by_Target = 0;
  unsigned int by_End = 0;
  unsigned int len ;
  char by_Data ;
  char by_Infor_End = 0;
  char const *w_WriteBuffer ;
  
  if(USB_Status == 0) return ;//==>当没有插USB时就不作动
  UsbFunction_ClearReadWriteBuffer(0);
  if(by_Dat == F_WorkoutSummary)
  {   
      //==>> 使用sprintf()指令将字符串内对应数值填入
      sprintf(USB_FileNameMerge,"/JHT/USERS/USER%d/peak%d.wrk",Save_USER,Save_WorkoutSummary);
      // <<=====================================================================
      file91 = f_open(USB_FileNameMerge,"w+");//==>开档并可写檔 
      if( !file91 )
      {//==>开档失败
          f_close(file91) ;
          return ;//_NoFileName_ ;
      }  
      by_Infor_End = 25;
      w_WriteBuffer = Workout_Default;
  }
  else if(by_Dat == F_User)
  {//==>目前预留不用
      return ;
  }
 
  if(by_Infor_End != 0)
  {
      for(i = 0 ; i < Max_Buffer ; i++)
      {
          by_Data = *(w_WriteBuffer + by_Target);
          USB_WriteBuffer[i] = by_Data;
          if(by_Data == '>')
          {
              by_End++;
              if(by_Dat == F_WorkoutSummary)
                  i = UsbFunction_WorkoutSummary(i,by_End);
              else
                  i = UsbFunction_WorkoutSummary(i,by_End);
          }
          if(by_End >= by_Infor_End)
              i = Max_Buffer;
          else
              by_Target++;
      }
  }
  
  len = __strlen(USB_WriteBuffer) + 1 ;//==>计算档案内的数据长度
  f_write(&USB_WriteBuffer[0],1,len,file91);//==>写入USB
  f_close(file91) ;//==>关档案
}
*/
//==============================================================================
//==>> UsbFunction_WorkoutSummary()
//==>> 写入Summary各项信息用
//==>> by_D1 = 数据存储至暂存区位置 
//==>> by_D2 = 第几笔数据储存比对 (使用'>'符号来决定第几笔信息项目) 
//==============================================================================
/*
unsigned int UsbFunction_WorkoutSummary(unsigned int by_D1,char by_D2)
{
    
    
    
    switch(by_D2)
    {
        case 3://==>Userid
               //USB_WorkoutSummary.UserID = {"USER1"};
                 
               USB_WriteBuffer[++by_D1] = 'U';
               USB_WriteBuffer[++by_D1] = 'S';
               USB_WriteBuffer[++by_D1] = 'E';
               USB_WriteBuffer[++by_D1] = 'R';
               USB_WriteBuffer[++by_D1] = Save_USER + '0';
               break;
        case 5://==>Model
               USB_WriteBuffer[++by_D1] = 'A';
               USB_WriteBuffer[++by_D1] = 'T';
               if(IO_Check_MachineType() == Type_AT1x)
                   USB_WriteBuffer[++by_D1] = '1';
               else
                   USB_WriteBuffer[++by_D1] = '3';
               USB_WriteBuffer[++by_D1] = 'x';
               break;
        case 8://==>Year
               by_D1 = UsbFunction_SetData(by_D1,_WorkoutSummary.Year,0);
               break;
        case 10://==>Month
               by_D1 = UsbFunction_SetData(by_D1,_WorkoutSummary.Month,0);
               break;
        case 12://==>Date
               by_D1 = UsbFunction_SetData(by_D1,_WorkoutSummary.Date,0);
               break;
        case 14://==>Averpace
               by_D1 = UsbFunction_SetData(by_D1,_WorkoutSummary.Averpace,2);
               break;
        case 16://==>Calories
               by_D1 = UsbFunction_SetData(by_D1,_WorkoutSummary.Calories,0);
               break;
        case 18://==>Distance
               by_D1 = UsbFunction_SetData(by_D1,_WorkoutSummary.Distance,1);
               break;
        case 20://==>Totalelev
               by_D1 = UsbFunction_SetData(by_D1,_WorkoutSummary.Totalelev,0);
               break;
        case 22://==>TotalTime
               by_D1 = UsbFunction_SetData(by_D1,_WorkoutSummary.TotalTime,0);
               break;
    }
    return by_D1;
}

*/

//==============================================================================
//==>> UsbFunction_SetData()
//==>> 将准备储存的数据进行转换为ASCII码
//==>> by_D1 = 数据存储位置
//==>> by_D2 = 待转换值
//==>> by_Dot = 小数点位数
//==============================================================================
/*
char UsbFunction_WorkoutSummary(void)
{
  F_FILE *file91;
  
  //char *_APM;
 
  unsigned short len ;
  
  SHT1381 RTC1 ;
  ReadHT1381(&RTC1) ;
  
  len = RTC1.Year;
  RTC1.Year = (len / 16) * 10;
  RTC1.Year += (len % 16);
  len = RTC1.Month;
  RTC1.Month = (len / 16) * 10;
  RTC1.Month += (len % 16);
  len = RTC1.Date;
  RTC1.Date = (len / 16) * 10;
  RTC1.Date += (len % 16);
  
  if(USB_Status == 0) return C_WriteFail;//==>当没有插USB时就不作动
  UsbFunction_ClearReadWriteBuffer(0);
  //==>> 使用sprintf()指令将字符串内对应数值填入
  sprintf(USB_FileNameMerge,"/MATRIX/USER/%02d%02d%02d%02d.wrk",RTC1.Month,RTC1.Date,RTC1.Year,Save_WorkoutSummary);
  Save_WorkoutSummary += 1;
  // <<=====================================================================
  file91 = f_open(USB_FileNameMerge,"w+");//==>开档并可写档 
  if( !file91 )
  {//==>开档失败
      f_close(file91) ;
      return C_OpenFail;
  }  
  sprintf(USB_WriteBuffer,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\r<workout>\n\r\t<userid>USER%d</userid>\n\r\t<model>LA Fitness T5x</model>\n\r",\
          Save_USER);
  len = __strlen(USB_WriteBuffer)  ;//==>计算数据长度
  f_write(&USB_WriteBuffer[0],1,len,file91);//==>写入USB
  UsbFunction_ClearReadWriteBuffer(0);
  sprintf(USB_WriteBuffer,"\t<summary>\n\r\t\t<Year>20%02d</Year>\n\r\t\t<Month>%02d</Month>\n\r\t\t<Date>%02d</Date>\n\r", \
          RTC1.Year,
          RTC1.Month,
          RTC1.Date);
  len = __strlen(USB_WriteBuffer) ;//==>计算数据长度
  f_write(&USB_WriteBuffer[0],1,len,file91);//==>写入USB                         
  UsbFunction_ClearReadWriteBuffer(0);                      
  sprintf(USB_WriteBuffer,"\t\t<Avgpace>%d:%02d</Avgpace>\n\r\t\t<Calories>%d</Calories>\n\r\t\t<Distance>%d.%02d</Distance>\n\r\t\t<Totalelev>%d</Totalelev>\n\r\t\t<TotalTime>%d</TotalTime>\n\r",\
          _WorkoutSummary.Averpace / 100,\
          _WorkoutSummary.Averpace % 100,     
          _WorkoutSummary.Calories,\
          _WorkoutSummary.Distance / 100,\
          _WorkoutSummary.Distance % 100, 
          _WorkoutSummary.Totalelev,\
          _WorkoutSummary.TotalTime);
  len = __strlen(USB_WriteBuffer) ;//==>计算数据长度
  f_write(&USB_WriteBuffer[0],1,len,file91);//==>写入USB                         
  UsbFunction_ClearReadWriteBuffer(0); 
  
  sprintf(USB_WriteBuffer,"\t</summary>\n\r</workout>");
  len = __strlen(USB_WriteBuffer) ;//==>计算数据长度 (+1 是为\0的截止)
  f_write(&USB_WriteBuffer[0],1,len,file91);//==>写入USB
  f_close(file91) ;//==>关档案
  BEEP_Proce(4,2,2);//==>完成后毕两声
  return C_WriteOK;
}
*/


//==============================================================================
//==>> UsbFunction_SetData()
//==>> 将准备储存的数据进行转换为ASCII码
//==>> by_D1 = 数据存储位置
//==>> by_D2 = 待转换值
//==>> by_Dot = 小数点位数
//==============================================================================
/*
unsigned int UsbFunction_SetData(unsigned int by_D1,unsigned int by_D2,char by_Dot)
{
  unsigned int by_S = 0;
  char by_Zero = 0;
  
  if(by_D2 > 0xffff) by_D2 = 0xffff;
  by_S = by_D2 / 10000;
  by_D2 %= 10000;
  
  if(by_S == 0) 
  {
      by_Zero++;
  }
  else  
  {
      USB_WriteBuffer[++by_D1] = '0' + by_S;
  }
  by_S = by_D2 / 1000;  
  by_D2 %= 1000;
  if(by_S == 0 && by_Zero == 1) 
  {
      by_Zero++;
  }
  else
  {
      USB_WriteBuffer[++by_D1] = '0' + by_S;
  }
  by_S = by_D2 / 100;  
  by_D2 %= 100;
  if(by_S == 0 && by_Zero == 2 && by_Dot != 2) 
  {
      by_Zero++;
  }
  else
  {
      USB_WriteBuffer[++by_D1] = '0' + by_S;
      if(by_Dot == 2)
      {
          USB_WriteBuffer[++by_D1] = '.';
      }
  }
  by_S = by_D2 / 10;  
  by_D2 %= 10; 
  if(by_S == 0 && by_Zero == 3 && by_Dot != 1) 
  {
      by_Zero++;
  }
  else
  {
      USB_WriteBuffer[++by_D1] = '0' + by_S;
      if(by_Dot == 1)
      {
          USB_WriteBuffer[++by_D1] = '.';
      }
  }
  
  USB_WriteBuffer[++by_D1] = '0' + by_D2;
 
  return by_D1;
}
*/
//==============================================================================
//==>> UsbFunction_SetUserNumber()
//==>> 将准备储存的USER位置放入
//==>> by_D1 = 第x组USER
//==>> by_D2 = 第x组Wortkout summary
//==============================================================================
/*
void UsbFunction_SetUserNumber(char by_D1,char by_D2)
{
  if(by_D1 == 0 || by_D1 > Max_USER) by_D1 = Max_USER;
  Save_USER = by_D1;
  if(by_D2 == 0 || by_D2 > Max_WorkoutSummary) by_D2 = Max_WorkoutSummary;
  Save_WorkoutSummary = by_D2;
}
*/
//==============================================================================
//==>> UsbFunction_CheckMessage()
//==>> 从档案中搜寻对应的字符串
//==>> by_D1 = 第x组字符串 
//==>> by_D2 = 搜寻起始位置
//==>> 0:Not word / 1:program / 2:models / 3:name / 4:levelamount / 5:segamount 
//==>> 6:levels / 7:package / 8:t
//==============================================================================
/*
char *USB_CheckMessage[]={USB_Message1,USB_Message2,USB_Message3,USB_Message4,\
                          USB_Message5,USB_Message6,USB_Message7,USB_Message8};

char UsbFunction_CheckMessage(char by_D1,unsigned int by_D2)
{
  unsigned int i,j;
  char by_Word = 0;
  char Check_Word[50] = {0};
  char *w_CheckWord;
  
  w_CheckWord = Check_Word;
  for(i = by_D2 ; i < Max_Buffer ; i++)
  {
      if(by_Word == 0 && USB_ReadBuffer[i] == '<')
      {
          for(j = 0;j < 50;j++)
          {
              Check_Word[j] = 0;
          }
          by_Word = 1;
      }
      else if(by_Word != 0)
      {
          if(USB_ReadBuffer[i] == '/')//==>设定结束用所以不理
              by_Word = 0;
          else if(USB_ReadBuffer[i] != '>')//==>非终止符
              *(w_CheckWord + (by_Word - 1)) = USB_ReadBuffer[i];
          else //==>终止符 '>'
          {
              *(w_CheckWord + (by_Word - 1)) = 0;
              by_Word = 0;
              switch(by_D1)
              {   
                  case 1:
                  case 2:
                  case 3:
                  case 4:
                  case 5:
                  case 6:
                  case 7:
                  case 8:if(UsbFunction_CheckWord(w_CheckWord,USB_CheckMessage[by_D1 - 1]))
                         {
                             by_CheckSite = i;
                             return by_D1;
                         }
                  default:
                         by_Word = 0;
                         break;
              }
          }
          if(by_Word != 0) by_Word += 1;
      }
      if(i >= 1023)
      {//==>当数据未取完时就在读取1k byte使用
          f_read(&USB_ReadBuffer[0],1,1024,file90); //==>读1024
          by_CheckSite = 0;
          i = 0;
      }
  }
  return 0;
}
*/
//==============================================================================
//==>> UsbFunction_CheckWord()
//==>> A and B字符串单一比对
//==>> A:pby_Dat / B:pby_Dat1
//==============================================================================
/*
char UsbFunction_CheckWord(char *pby_Dat,char *pby_Dat1)
{
  char i;
  char by_D;
  for(i = 0;i <= 50;i++)
  {
      by_D = *(pby_Dat + i);
      if(by_D == 0) return 1;
      if(by_D != *(pby_Dat1 + i)) return 0;
  }
  return 0;
}
*/
//==============================================================================
//==>> UsbFunction_Get_ProgramName()
//==>> 取出USB中所有的.prg档名称与所对应的PROGRAM名称
//==============================================================================
/*
char UsbFunction_Get_ProgramName(void)
{
  F_FIND find;
  char i,j;

  Total_Program = 0;
  for(i = 0 ; i < 10 ; i++)
  {//==>清除暂存区块
      for(j = 0 ; j < 20 ; j++)
      {
          Prg_Name[i][j] = 0;
          Program_Name[i][j] = 0;
      }
      Program_Level[i] = 0;
      Program_Segamount[i] = 0;
  }
  
  if(USB_Status == 0) return C_OpenFail;//==>当没有插USB时就不作动
  */
  //if(!f_findfirst("/MATRIX/PROGRAMS/*.prg",&find))
 /*
  {//==>开启所有.prg檔
      do
      {
          for(i = 0;i < 20;i++)
          {
              Prg_Name[Total_Program][i] = *(find.filename + i);//==>放入.prg檔名
              if(Prg_Name[Total_Program][i] == 0)
              {
                  i = 20;
                  //==>> 取出档案内的PROGRAM名称
                  sprintf(USB_FileNameMerge,"/MATRIX/PROGRAMS/%s",Prg_Name[Total_Program]);//==>整合檔名
                  file90 = f_open(USB_FileNameMerge,"r"); 
                  if( !file90 )
                  {//==>开档失败
                      f_close(file90) ;
                      return C_OpenFail;//_NoFileName_ ;
                  }
                  UsbFunction_ClearReadWriteBuffer(1);
                  f_read(&USB_ReadBuffer[0],1,1024,file90); //==>读1024
                  f_close(file90) ;
                  by_CheckSite = 0;
                  if(UsbFunction_CheckMessage(M_name,0))
                  {//==>PROGRAM名称放入暂存区
                      by_CheckSite += 1;
                      for(j = 0;j < 20;j++)
                      {
                          if(USB_ReadBuffer[by_CheckSite + j] == '<')
                              j = 20;
                          else
                              Program_Name[Total_Program][j] = USB_ReadBuffer[by_CheckSite + j];
                      }
                      by_CheckSite = 0;
                  }
                  if(UsbFunction_CheckMessage(M_levelamount,0))
                  {//==>取出LEVEL数
                     Program_Level[Total_Program] = UsbFunction_ASCIItoNumber();
                  }
                  if(UsbFunction_CheckMessage(M_segamount,0))
                  {//==>取出排数
                      Program_Segamount[Total_Program] = UsbFunction_ASCIItoNumber();
                  }
                  // <<=========================================================
              }
          }
          Total_Program += 1;
      } while (!f_findnext(&find));//==>下个.prg檔
  }
  UsbFunction_ClearReadWriteBuffer(2);//==>清除 Program_Table 区块
  return C_OpenOK;
}
*/
//==============================================================================
//==>> UsbFunction_Get_Level_Table()
//==>> 取出对应PROGRAM中的Table表
//==>> by_D1 : 第几个 PROGRAM
//==>> by_D2 : 第几个 LEVEL 的 TABLE
//==============================================================================
/*
char UsbFunction_Get_Level_Table(char by_D1,char by_D2)
{
  char i;
  
  if(USB_Status == 0 || Total_Program == 0) return C_OpenFail;//==>当没有插USB或没有PROGRAM时就不作动
  if(by_D1 > Total_Program) 
  {
      by_D1 = Total_Program;
  }
  else if(by_D1 == 0)
  {
      by_D1 = 1;
  }
  if(by_D2 > Program_Level[by_D1 - 1]) 
  {
      by_D2 = Program_Level[by_D1 - 1];
  }
  else if(by_D2 == 0)
  {
      by_D2 = 1;
  }
  
  
  //==>> 取出档案内的PROGRAM名称
  sprintf(USB_FileNameMerge,"/MATRIX/PROGRAMS/%s",Prg_Name[by_D1 - 1]);//==>整合檔名
  file90 = f_open(USB_FileNameMerge,"r"); 
  if( !file90 )
  {//==>开档失败
      f_close(file90) ;
      return C_OpenFail;//_NoFileName_ ;
  }
  
  UsbFunction_ClearReadWriteBuffer(1);
  f_read(&USB_ReadBuffer[0],1,1024,file90); //==>读1024
  by_CheckSite = 0;
  
  for(i = 0;i < by_D2;i++)
  {
      if(UsbFunction_CheckMessage(M_level,by_CheckSite) != M_level)
      {
          i-=1;
      }
  }
  UsbFunction_Table();
  f_close(file90) ;
  return C_OpenOK;
}
*/
//==============================================================================
//==>> UsbFunction_Table()
//==>> 取出1组Table表
//==============================================================================
/*
void UsbFunction_Table(void)
{
  char by_Dat = 0;
  char by_END = 0;
  char by_D;
  char by_N = 0;
  char by_St = 0;
  char by_Sum = 0;
  
  UsbFunction_ClearReadWriteBuffer(2);//==>清除 Program_Table 区块
  UsbFunction_CheckMessage(M_t,by_CheckSite);//==>取出总时间位置
  Program_Table[0][0] = UsbFunction_ASCIItoNumber();//==>Total Time放入暂存区
  while(by_END == 0)  
  {
      UsbFunction_CheckBuffer_1K();
      by_D = USB_ReadBuffer[by_CheckSite++];
      switch(by_D)
      {
          case '<':// 讯息框起始
                   by_N = 0;
                   by_St = 1;
                   break;
          case '>':// 讯息框结束
                   by_St = 0;
                   if(by_N != 0)
                   {//==>依对应ID将值ASCII转成十进制后放入暂存区块
                       by_Dat = UsbFunction_ASCIItoNumber();
                       Program_Table[by_N][by_Sum - 1] = by_Dat;
                       by_N = 0;
                   }
                   break;
          case 'e':// 不同排数切割
                   if(by_St == 1) by_Sum++;
                   break;
          case '/':// 结束判断
                   if(by_St == 1) by_St = 0;
                   break;
          case 'l':// </level>结束用
                   by_END = 1;
                   Program_Table[0][1] = by_Sum;//==>将排数放入此一位置暂存
                   break;
          case '1':// time
          case '2':// incline
          case '3':// speed
          case '4':// resistance
          case '6':// repeat
                   if(by_St == 1) by_N = by_D - '0';//==>对应ID
                   break;
          case '5':// Message         
                   break;
      }
  }
}
*/
//==============================================================================
//==>> UsbFunction_ASCIItoNumber()
//==>> 依序取 ?   并将ASCII转成十进制
//==============================================================================
/*
char UsbFunction_ASCIItoNumber(void)
{
  char by_Dat = 0;
  char i;
  char by_B;
  
  for(i = 0;i < 20;i++)
  {
      by_B = USB_ReadBuffer[by_CheckSite++];//==>依序取出
      if(by_B == '<')
      {//==>取值完成
          i = 20;
          
          if(by_CheckSite >= 1023)
          {//==>当数据未取完时就在读取1k byte使用
              UsbFunction_CheckBuffer_1K();    
          }
          else
              by_CheckSite -= 1;
      }
      else if(by_B >= '0' && by_B <= '9')
      {//==>为数值时
          if(by_Dat != 0)
          {
              by_Dat *= 10;
          }
          by_Dat += (by_B - '0');
      }
      UsbFunction_CheckBuffer_1K();    
  }
  return by_Dat;
}
*/
//==============================================================================
//==>> UsbFunction_CheckBuffer_1K()
//==>> 确认数据未完成抓取所以需要再读出 1k byte 时
//==============================================================================
/*
void UsbFunction_CheckBuffer_1K(void)
{
  if(by_CheckSite >= 1023)
  {//==>当数据未取完时就在读取1k byte使用
      f_read(&USB_ReadBuffer[0],1,1024,file90); //==>读1024
      by_CheckSite = 0;
  }
}
*/
//==============================================================================
//==>> UsbFunction_USB()
//==>> USB是否插拔确认使用,需要常驻执行才可正常判读使用
//==>> 输入值 by_D 为 USB 侦测确认判断用延迟参数,会影响不同 USB 随身碟的判读
//==>> 0xFFFF (建议使用此预设延迟参数可供兼容目前所有自购的USB随身碟)
//==============================================================================
void UsbFunction_USB(unsigned int by_D)
{
  int rc=0;
  //
  CheckTime += 1 ;
  if( CheckTime > by_D )
  {
      pmgr_pcd_task();//==> USB侦测用
      usbh_transfer_task();//==> USB侦测用
      CheckTime = 0 ;
  }
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
  {
      USBErrorCount =  0;     // Add by Kunlung 20110125
      if(USB_Status == 0) BEEP_Proce(2,2,2);//==>插入后毕一声
      USB_Status = 1;//==>插入  
      if( Boot_CheckJHTfile((char*)JHT_Path) == _JHTFileNotExist_ )
      {//==>MATRIX文件夹不存在时 
          Boot_CreateJHTfile(_JHTFileCreate_,"/MATRIX/USER/user.usr" ) ;
      }
      else
      {
          if(USB_WrkFialCheck == 0)
          {
              USB_WrkFialCheck = 1;
              UsbFunction_Get_WrkFileSerialNumber();
          }
      }
  }
  else 
  {
      // Modify by Kunlung 20110125  
      if( USB_Status == 1 || USBPlugin == 0xA5 )
      {
          USBPlugin = 0x5A ;  //
          USB_Status = 0;//==>拔除 
          IO_USB_ResetILIM() ;
          //
          rc=os_init();
          if(rc==0) rc=usbh_init();			    /* initialize USB host */
          if(rc==0) rc=usb_mst_init();			/* initalize mass storage */
          if(rc==0) rc=usbh_start();			  /* start USB host */
          if(rc==0)
              {
              f_init();
              f_enterFS();
              (void)f_initvolume(1,mst_initfunc,F_AUTO_ASSIGN);
              (void)f_chdrive(1);					  /* initialize file system */
              }
          //
          IO_USB_SetILIM() ;
      }
      //
      USB_FirstCheck = 0;
      USB_WrkFialCheck = 0;
      //
  }
  //
  UsbFunction_Protection();
  return ;
}


// Add by Kunlung 20110221
void UsbFunction_CheckConnected(unsigned short CheckTime,unsigned short CheckTime1)
{
  if( stm32uh_hub_connected() == 0 )
      {
      //
      USBProcessTime = 0 ;
      if( USBPlugin != 0xA5 && USBPlugin != 0xAA )
          {
          USBPlugin = 0x5A ;
          USBProcessTime1 = 0 ;
          }
      else
          {
          USBProcessTime1 += 1 ;
          // Check Timeout
          if( USBProcessTime1 >= CheckTime1 )
              {
              USBPlugin = 0xA5 ;
              IO_USB_ResetILIM() ;
              USBProcessTime1 = 0 ;
              USBErrorCount += 1 ;
              }
          //
          }
      //
      return ;
      }
  else
      {
       USBProcessTime1 = 0 ;  
      if( USB_Status == 0 )
          {
          USBPlugin = 0xAA ;
              USBProcessTime += 1 ;
          // Check Timeout
          if( USBProcessTime >= CheckTime )
              {
              USBPlugin = 0xA5 ;  
              IO_USB_ResetILIM() ;
              USBProcessTime = 0 ;
              USBErrorCount += 1 ;
              }
          //
          }
      else
          {
          USBPlugin = 0x5A ;
          USBProcessTime = 0 ;
          USBErrorCount = 0 ;
          }
      }
  //
  return ;
}

//==============================================================================
//==>> UsbFunction_SoftwareUpdate()
//==>> 主要软件更新时使用
//==>> by_ShowMessage 检视是否使用提示字符串与软件更新判断
//==>> 0:NO  单存用来检查所有的 .Bin 檔
//==>> 1:YES 除了检视所有 .Bin 文件外还提供是否更新的提示字符串用来做软件更新用
//==>> 2:多国语言 .Bin 档搜寻
//==>> 3:Logo图形 .Bin 档搜寻
//==============================================================================
void UsbFunction_SoftwareUpdate(char by_ShowMessage)
{
  char by_CheckSum = 0;
  
  
  //----------------------------------------------------------------------------
  // Add by Kunlung 20110704 Check WiFi 
  /*
#ifdef  _WiFi_UpdateUCBLCB_
  WiFiControlStatus *WiFiStatus ;
  WiFiStatus = WiFi_GetWiFiControlStatus() ;
  //

  // UCB Update
  if( WiFiStatus->b.WiFiUpdateUCB == 1 ) 
      {
      if( WiFi_UCBFirstCheck == 0 )
          {
          WiFi_UCBFirstCheck = 1 ;
          Show_Message(Message_Blank,0,0);
          while(by_CheckSum < 4 )
              {
              Show_Message(Message_WiFiDoYouWouldUpdate,0,0);
              if(Led_Get_String_End())
                  {
                  Led_Set_Reset();
                  //by_CheckSum++;
                  }
              //
              switch(Key_Proce())
                  {
                  case  K_ENTER :
                                if( Console_Sound(Data_Get,0) == 1 )
                                {
                                    while( Voice_GetStatus()==1 ) ;
                                }
                                else
                                {
                                    while(BEEP_Get_OK()== 1);  
                                }
                                Boot_UCBFirmwareUpdate( &Update_Name[0],0,1 ) ; 
                                break ;
                  case  K_BACK  :
                                WiFi_SetCommWorkMode(_WiFiUCBupdateEnd_) ;
                                by_CheckSum = 0xFA ;
                                break ;
                  }
              }          
          }
      }
  else
      WiFi_UCBFirstCheck = 0 ;

#endif
  */
  //--- 20110704 --END----------------------------------------------------------
  //----------------------------------------------------------------------------
  if(USB_Status == 1)
  {//==>当有插USB时就作动
      if(USB_FirstCheck == 0)
      {
          USB_FirstCheck = 1;
          // 2013.07.26
          switch(by_ShowMessage)
          {
            case 2:// 多国语言 Bin file check
                   if( Boot_CheckJHTfile((char*)LANG_PATH) == _JHTFileExist_ )
                   {//==>LANGUAGE档案夹存在时
                       if(UsbFunction_SearchBINfile((char*)LANG_BIN,&TempFilename[0],2) == _HaveBinFile_)    
                       {//==>当有 Bin 档案时
                       }
                   }
                   else 
                       UsbFunction_ClearFileName();
                   break;
            case 3:// LOGO图形 Bin file check
                   if( Boot_CheckJHTfile((char*)PATTERN_PATH) == _JHTFileExist_ )
                   {//==>LANGUAGE档案夹存在时
                       if(UsbFunction_SearchBINfile((char*)PATTERN_BIN,&TempFilename[0],3) == _HaveBinFile_)    
                       {//==>当有 Bin 档案时
                       }
                   }
                   else 
                       UsbFunction_ClearFileName();
                   break;
            default:
                   if( Boot_CheckJHTfile((char*)FW_PATH) == _JHTFileExist_ )
                    {//==>FW 档案夹存在时
                        if(UsbFunction_SearchBINfile((char*)FW_BIN,&TempFilename[0],1) == _HaveBinFile_)    
                        {//==>当有 Bin 档案时
                            if( TempFilename[0] != 0 )
                            {
                                if(by_ShowMessage == 1)
                                {//==>提供提示字符串与软件更新动作功能
                                    Show_Message(Message_Blank,0,0,Show_Blank);
                                    while(by_CheckSum < 2)
                                    {//==>有新软件可更新所以显示提示字符串跑x次
                                        UsbFunction_USB(0x500);
                                        if(USB_Status == 1)
                                        {//==>当有插USB时就作动
                                            Show_Message(Message_DoYouWouldUpdate,0,0,Show_Auto);
                                            if(Led_Get_String_End())
                                            {
                                                Led_Set_Reset();
                                                by_CheckSum++;
                                            }
                                            if(Key_Proce() == K_ENTER)
                                            {//==>按ENTER key更新软件
                                                Timer_Counter_Clear(0);
                                                while(!Timer_Counter(T_STEP,0,5));
                                                //if(Voice_GetSwitch() == _ON )
                                                //    while( Voice_GetStatus()==1 ) ;
                                                //else
                                                //    {
                                                        while(BEEP_Get_OK() == 1);  
                                                //    }
                                                Boot_UCBFirmwareUpdate( &Update_Name[0],0 ,0);//==>更新完不删档
                                            }
                                        }
                                        else by_CheckSum = 0xFA;
                                        // 2014.02.20-1
                                        if(!Check_SafeKey()) by_CheckSum = 100; 
                                        //
                                    }
                                }
                            }
                        }
                    }
                   else 
                       UsbFunction_ClearFileName();
                   break;
          }

      }
  }
}

//==============================================================================
//==>> UsbFunction_CheckBINFileInformation()
//==>> 确认 .bin 档案是否符合需求
//==>> 1. *filename 文件名
//==>> 2. *sAddr 比对的字符串
//==>> 3. *tAddr 比对字符串地址
//==>> 4. by_D1 比对模式 0:一般字符串比对 1:软件版本比对
//==============================================================================
char UsbFunction_CheckBINFileInformation(char *filename,char *sAddr,long tAddr ,char by_D1)
{
  //
  F_FIND find;
  F_FILE *file; 
  
  unsigned long filesize ;
  char Buffer[200] ;
  char *ptr ;
  char *ptr1 ;
  
  char i,j,by_Chang = 0;
  //
  
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
  {
      if(!f_findfirst((const char*)filename, &find))
      {
          file = f_open(filename,"r"); 
          if( !file )
          {
              f_close(file) ;
              return _NoFileName_ ;
          }
          else
          {
              if(!f_seek(file,tAddr,FN_SEEK_SET))//==>file 移到判断字符串地址
              {
                  __memset(&Buffer[0],0,200);// clear
                  filesize = f_read(&Buffer[0],1,200,file); // read 200 byte 
                  if(filesize == 200)
                  {
                  //----------------------------------------------------------------
                  // check Manufacture
                  ptr = sAddr ;
                  ptr1 = &Buffer[0] ;
                  while( *ptr != 0 )
                  {
                      if(by_D1 == 0)
                      {
                          if( *ptr != *ptr1 )
                          {
                              f_close(file) ;
                              return _DataNotMatch_ ;
                          }
                      }
                      else
                      {
                          if( *ptr < *ptr1 )
                          {//==>当软件版本有较大的版本时
                              
                              for(i = 0;i < 40;i++)
                              {
                                  if((Buffer[i] == 0)||(Update_Ver[i] != Buffer[i]))
                                  {
                                      if((Buffer[i] != 0)&&(Update_Ver[i] < Buffer[i]))
                                      {//==>比对是否有较新的版本时就取其版本与路径
                                          for(j = 0;j < 40;j++)
                                          {//==>版本
                                              if(Buffer[j] != 0)
                                              {
                                                  Update_Ver[j] = Buffer[j];
                                              }
                                              else by_Chang = 1;
                                              if(by_Chang != 0)
                                              {
                                                  if(Update_Ver[j] != 0)
                                                      Update_Ver[j] = 0;
                                                  else 
                                                      j = 40;
                                              }
                                          }
                                          for(j = 0;j < 50;j++)
                                          {//==>路径
                                              if(*(filename + j) != 0)
                                              {
                                                  Update_Name[j] = *(filename +j);
                                              }
                                              else
                                              {
                                                  Update_Name[j] = 0;
                                              }
                                          }
                                      }
                                      i = 40;
                                  }
                              }
                              
                              f_close(file) ;
                              return _DataNotMatch_ ;
                          }
                          else if( *ptr > *ptr1 )
                          {//==>既有版本比USB内的档还新版就不做
                              f_close(file) ;
                              return _DataMatch_ ;
                          }
                      }
                      ptr += 1 ;
                      ptr1 += 1 ;
                  }
                  
                  // 100.9.16
                  f_close(file) ;
                  if( *ptr1 != 0 )
                  {// 比对的字符串长度不相同时就判定Not Match
                      return _DataNotMatch_ ;
                  }
                  else return _DataMatch_ ;
                  //
                  }
              }
              
          }
      }
  }
  else
      return _USBNotPlugin_ ;
  f_close(file) ;
  return _DataNotMatch_ ;// 100.9.16
}
//==============================================================================
//==>> UsbFunction_SearchBINfile()
//==>> 确认所有 .bin 档案是否符合需求
//==============================================================================
// 2013.07.26
void UsbFunction_ClearFileName(void)
{
    unsigned char i = 0, j = 0;
    for(j = 0;j < 10;j++)
    {
        for(i = 0;i < 40;i++)
        {
            Bin_Name[j][i] = 0;//TempFilename[i];
            Update_Ver[i] = 0;
        }
    }
    Total_BIN = 0;
}
char UsbFunction_SearchBINfile(char*filename,char*retfilename,char by_Mode) 
{
  F_FIND find;
  char *pch ;
  char len , j;
  char by_Bin = 0;
  char by_BinFile = 0;
  
  unsigned short i;
  
  
  UsbFunction_ClearFileName();
  //
  if(!f_findfirst(filename,&find))
      {
      do
          {
          __strcpy(retfilename,filename) ;
          pch = __strstr(retfilename,"*.bin") ;
          len = __strlen(find.filename)+1 ;//==>计算长度
          __strncpy(pch,find.filename,len);  
          // 2013.07.26
          switch(by_Mode)
          {
            default:
            case 1:// 软件更新用
                   if( UsbFunction_CheckBINFileInformation(retfilename,(char*)&Boot_Version[0], ((unsigned long)Boot_Version & 0x000FFFFF), 0) == _DataMatch_ )
                   { 
                   if( UsbFunction_CheckBINFileInformation(retfilename,(char*)&User_Manufacture[0],_Info_ManufactureIndex_, 0) == _DataMatch_) 
                   {
                       if( UsbFunction_CheckBINFileInformation(retfilename,(char*)&User_McuType[0],_Info_McuTypeIndex_, 0) == _DataMatch_ )
                       {
                           if( UsbFunction_CheckBINFileInformation(retfilename,(char*)&User_ModuleName[0],_Info_ModuleNameIndex_, 0) == _DataMatch_ )
                           {
                               if( UsbFunction_CheckBINFileInformation(retfilename,(char*)&User_ModuleNo[0],_Info_ModuleNoIndex_, 0) == _DataMatch_ )
                               {
                                   if( UsbFunction_CheckBINFileInformation(retfilename,(char*)&User_Product[0],_Info_ProductIndex_, 0) == _DataMatch_ )
                                   {
                                       if( UsbFunction_CheckBINFileInformation(retfilename,(char*)&User_Version[0],_Info_VersionIndex_, 1) != _DataMatch_ )
                                       {//==>当有软件版本较新时
                                           by_Bin += 1;//==>有符合的版本
                                       }
                                   }
                               }
                               len = 0;
                               for(i = 0;i < 50;i++)
                               {
                                   if(TempFilename[i] == '/')
                                   {//==>文件夹层别判断
                                       len += 1;
                                   }
                                   if(len >= 4) 
                                   {//==>在第四层bin文件所在位置时
                                       len = i + 1;
                                       i = 50;
                                       if(by_BinFile <= 9)// 100.09.15
                                       {//==>可读取组数10组的范围限制
                                           for(j = 0;j < 40;j++)
                                           {
                                               if(TempFilename[j + len] != '.')
                                               {//==>取扩展名前的字符
                                                   Bin_Name[by_BinFile][j] = TempFilename[j + len];
                                               }
                                               else j = 40;
                                           }
                                           by_BinFile += 1;
                                           Total_BIN += 1;
                                           }
                                       }
                                   }
                               }
                           }
                       }
                   }
                   break;
            case 2:// 多国语言加载
            case 3:// LOGO图形加载 
                   len = 0;
                   for(i = 0;i < 50;i++)
                   {
                       if(TempFilename[i] == '/')
                       {//==>文件夹层别判断
                           len += 1;
                       }
                       if(len >= 4) 
                       {//==>在第四层bin文件所在位置时
                           len = i + 1;
                           i = 50;
                           if(by_BinFile <= 9)// 100.09.15
                           {//==>可读取组数10组的范围限制
                               for(j = 0;j < 40;j++)
                               {
                                   if(TempFilename[j + len] != '.')
                                   {//==>取扩展名前的字符
                                       Bin_Name[by_BinFile][j] = TempFilename[j + len];
                                   }
                                   else j = 40;
                               }
                               by_BinFile += 1;
                               Total_BIN += 1;
                           }
                       }
                   }
                   break;
          }

             //
          } while (!f_findnext(&find));
      }
  //
  if(by_Bin == 0)
      return  _NoBinFile_  ; 
  else
      return _HaveBinFile_ ;
}
//==============================================================================
//==>> UsbFunction_AssignSoftwareUpdate()
//==>> 指定BIN档更新软件
//==>> by_D : 指定档案编号
//==============================================================================
char UsbFunction_AssignSoftwareUpdate(char by_D)
{
  unsigned short i;
  
  Timer_Counter_Clear(0);
  while(!Timer_Counter(T_STEP,0,5));
  //if(Voice_GetSwitch() == _ON )
  //     while( Voice_GetStatus() ) ;
  //else
  //{
       while(BEEP_Get_OK() == 1); 
  //}
  //
  if(USB_Status == 0) return C_WriteFail;//==>当没有插USB时就不作动
  else
      {
      if(by_D != 0 && Total_BIN != 0 ) 
  {//==>USB插着且有符合条件的bin档并指定正确时
      for(i = 0;i < 255;i++)
      {
          if(Update_Name[i] != 0)
              Update_Name[i] = 0;
          else 
              i = 255;
      }
      sprintf(Update_Name,"/MATRIX/FW/UCB/%s.bin",Bin_Name[by_D - 1]);//==>整合檔名
          Boot_UCBFirmwareUpdate( &Update_Name[0],0,0 );//==>更新完不删档
          }
      }
      return C_WriteOK;
  }
//==============================================================================
//==>> UsbFunction_GetTotalBinFile()
//==>> 取得所有 .bin 档的数量
//==============================================================================
char UsbFunction_GetTotalBinFile(void)
{
  return Total_BIN;  
}
//==============================================================================
//==>> UsbFunction_Get_WrkFileSerialNumber()
//==>> 取出USB中最新的.wrk档流水号
//  UsbFunction_Get_MaxWrk_Name
//==============================================================================
void UsbFunction_Get_WrkFileSerialNumber(void)
{
  F_FIND find;
  char Wrk_File[30] = {0};
  char _Sum = 0;
  
  
  SHT1381 RTC1 ;
  ReadHT1381(&RTC1) ;
  
  _Sum = RTC1.Year;
  RTC1.Year = (_Sum / 16) * 10;
  RTC1.Year += (_Sum % 16);
  _Sum = RTC1.Month;
  RTC1.Month = (_Sum / 16) * 10;
  RTC1.Month += (_Sum % 16);
  _Sum = RTC1.Date;
  RTC1.Date = (_Sum / 16) * 10;
  RTC1.Date += (_Sum % 16);
  
  
  _Sum = 0;
  Save_WorkoutSummary = 0;
  sprintf(Wrk_File,"/MATRIX/USER/%02d%02d%02d??.wrk",RTC1.Month,RTC1.Date,RTC1.Year);//==>整合檔名
  if(!f_findfirst(Wrk_File,&find))
  {//==>取出所有同一日期的 wrk 档的最大序号
      do
      {  
          if(((*(find.filename + 6) >= '0')&&(*(find.filename + 6) <= '9'))&& \
             ((*(find.filename + 7) >= '0')&&(*(find.filename + 7) <= '9')))
          {//==>避免非正确的范围,如 A~Z
              _Sum = (*(find.filename + 6) - '0') * 10;
              _Sum += *(find.filename + 7) - '0';  
              if(_Sum != 0 && Save_WorkoutSummary < _Sum)
              {
                  Save_WorkoutSummary = _Sum;
              }
          }
      } while (!f_findnext(&find));//==>下个.wrk檔
  }
  
  if(Save_WorkoutSummary != 0)
  {
      if(Save_WorkoutSummary >= 99)//==>最大范围为 99 笔
          Save_WorkoutSummary = 1;
      else
          Save_WorkoutSummary += 1;
  }
  else Save_WorkoutSummary = 1;
}
//==============================================================================
//==>> UsbFunction_USB_Status()
//==>> 取得USB是否已插上的状态
//==>> 0:没插上 1:已插上
//==============================================================================
char UsbFunction_USB_Status(void)
{
  return USB_Status;
}


//#ifdef    _ExportAndInport_Mode0_

// Add by Kunlung 20110302
typedef struct {
  unsigned char Mark[4] ;
  unsigned char Reserved[16] ;
  unsigned long Version ;
  unsigned long ParameterCount ;
  unsigned long CheckSum ;
  unsigned char MachineName[100] ;
  unsigned long EndMark ;
} ExportInportFileDataHeader ;


typedef struct {
  unsigned char Name[100] ;
  unsigned long Value ;
  unsigned char Rev[36] ;
  unsigned long EndMark ;
} ExportInportFileData ;


#define   PARAMETER_FILENAME            "T1x.backup"
#define   LEDFileFormatVersion          1
#define   DataEndMark                   0x00000004
#define   FileDataHeaderSize            136
#define   FileDataSize                  144


// String Name
//==============================================================================
// EEPROM 20110315
#define SETUP_DEFAULT_AGE_NAME                    "defaultage"
#define SETUP_DEFAULT_WEIGHT_NAME                 "defaultweight"
#define SETUP_DEFAULT_HEIGHT_NAME                 "defaultheight"
#define SETUP_MAX_TIME_NAME                       "maxtime"
#define SETUP_DEFAULT_TIME_NAME                   "defaulttime"
#define SETUP_LANGUAGE_NAME                       "language"
#define SETUP_ACCUMULATED_TIME_NAME               "accumuatedtime"
#define SETUP_ACCUMULATED_DISTANCE_NAME           "accumuateddistance"
#define SETUP_START_SPEED_NAME                    "startspeed"
#define SETUP_DEFAULT_FLOORS_NAME                 "defaultfloors"
#define SETUP_DEFAULT_CALORIES_NAME               "defaultcalories"
#define SETUP_MAX_SPEED_NAME                      "maxspeed"
#define SETUP_TIMER_UP_NAME                       "timerup"
#define SETUP_DEFAULT_LEVEL_NAME                  "defaultlevel"
#define SETUP_DEFAULT_CHANNEL_NAME                "defaultchannel"
#define SETUP_DEFAULT_VOLUME_NAME                 "defaultvolume"
#define SETUP_OUT_OF_ORDER_NAME                   "outoforder"
#define SETUP_SAFETY_SWITCH_NAME                  "safetyswitch"
#define SETUP_DEFAULT_GENDER_NAME                 "defaultgender"
#define SETUP_PAUSE_TIME_NAME                     "pausetime"
#define SETUP_SERIAL_NUMBER_NAME                  "serialnumber"
#define SETUP_MACHINE_TYPE                        "machinetype"
#define SETUP_CLUB_ID                             "clubid"
#define SETUP_TUNE_END_POINT_INCLINE_1_TOP        "incline1top"
#define SETUP_TUNE_END_POINT_INCLINE_1_BOTTOM     "incline1bottom"
#define SETUP_TUNE_END_POINT_INCLINE_2_TOP        "incline2top"
#define SETUP_TUNE_END_POINT_INCLINE_2_BOTTOM     "incline2bottom"
#define SETUP_RPM_LOW_LIMIT_CHARGE                "rpmcharge"
#define SETUP_RPM_LOW_LIMIT_RESISTANCE            "rpmresistance"
#define SETUP_INCLINE_STROKE                      "inclinestroke"
#define SETUP_DEFAULT_INCLINE                     "defaultincline"
#define SETUP_KEY_SOUND                           "keysound"
#define SETUP_SELF_POWER_KEEP_RUN_TIME            "SelfPowerRunTime"
#define SETUP_SELF_POWER_KEEP_SUMMARY_TIME        "SelfPowerSummaryTime"
#define SETUP_SELF_POWER_KEEP_HOME_TIME           "SelfPowerHomeTime"
#define SETUP_SELF_POWER_THRESHOLD                "SelfPowerThreshold"
#define SETUP_ERROR_CLASS_SHOW                    "ErrorClassShow"
#define SETUP_ADJUST_RESISTANCE_OFFSET            "AdjustResistanceOffset"
#define SETUP_STEPPER_INITIAL_RPM                 "StepperInitialRPM"
#define SETUP_SELF_POWER_ELECTROMAGNETIC_CURRENT  "SelfPowerElectromagneticCurrent"
#define SETUP_MIN_ABSOLUTE_INCLINE_ADC_NAME       "minabsoluteinclineadc"
#define SETUP_MAX_ABSOLUTE_INCLINE_ADC_NAME       "maxabsoluteinclineadc"
#define SETUP_UNIT_NAME                           "unit"
#define SETUP_EUPS_SLEEP_TIME_NAME                "sleeptime"
#define SETUP_RF_SERVICE_NAME                     "rfservice"
#define SETUP_BATTERY_VOLT                        "BatteryVolt"
#define SETUP_INCLINE_RPM                         "InclineRPM"
#define SETUP_INCLINE_RESET                       "InclineReset"
#define SETUP_BEST_STOP                           "BestStop"
#define SETUP_NAVY_TEST                           "NavyTest"
#define SETUP_MaxRPM_PARAMETER                    "MaxRpmParameter"
#define SETUP_MinRPM_PARAMETER                    "MinRpmParameter"
#define SETUP_CONSOLE_VERSION                     "ConsoleVersion"
#define SETUP_FRAME_VERSION                       "FrameVersion"
#define SETUP_FRAME_SERVER_NUMBER                 "FrameSerialNumber"
#define SETUP_AUDIO_SOURCE                        "AudioSource"
#define SETUP_INPUT_DEFAULT                       "InputDefaultVolume"
#define SETUP_MAX_VOLUME                          "MaximumVolume"
#define SETUP_OUTPUT_DEFAULT                      "OutputDefaultVolume"
#define SETUP_LOGO                                "Logo"
#define SETUP_TV_POWER                            "TVPower"
//
#define SETUP_MaxDF_Elevation                     "MaxDefaultElevation"
#define SETUP_Speaker                             "SpeakerSwitch"
#define SETUP_xID                                 "xIDSwitch"
#define SETUP_DemoMode                            "DemoModeSwitch"
#define SETUP_HJS_Switch                          "HJSSwitch"
#define SETUP_Wireless                            "WirelessSwitch"
#define SETUP_HJS_total                           "HJSTotal"
#define SETUP_HJS_CheckNumber                     "HJSCheckNumber"


#define SETUP_PARAMETER_NUMBER                    43
#define SETUP_INPORT_NUMBER                       31
#define SETUP_INPORT1_NUMBER                      38
const unsigned char *PStringName[SETUP_PARAMETER_NUMBER] = {
                                        SETUP_MAX_TIME_NAME,
                                        SETUP_DEFAULT_TIME_NAME,
                                        SETUP_DEFAULT_LEVEL_NAME,
                                        SETUP_DEFAULT_AGE_NAME,
                                        SETUP_DEFAULT_WEIGHT_NAME,
                                        SETUP_START_SPEED_NAME,
                                        SETUP_MAX_SPEED_NAME,
                                        SETUP_UNIT_NAME,
                                        SETUP_OUT_OF_ORDER_NAME,  
                                        SETUP_DEFAULT_GENDER_NAME,
                                        SETUP_LANGUAGE_NAME,
                                        SETUP_KEY_SOUND,
                                        SETUP_NAVY_TEST,
                                        SETUP_PAUSE_TIME_NAME,
                                        SETUP_BEST_STOP,
                                        SETUP_ERROR_CLASS_SHOW,
                                        SETUP_CLUB_ID,  
                                        SETUP_AUDIO_SOURCE,
                                        SETUP_EUPS_SLEEP_TIME_NAME,
                                        SETUP_INPUT_DEFAULT,
                                        SETUP_MAX_VOLUME,
                                        SETUP_OUTPUT_DEFAULT,    
                                        SETUP_LOGO,
                                        SETUP_TV_POWER,
                                        SETUP_MaxDF_Elevation,
                                        SETUP_Speaker,
                                        SETUP_xID,
                                        SETUP_DemoMode,
                                        SETUP_HJS_Switch,
                                        SETUP_Wireless, 
                                        SETUP_HJS_CheckNumber,
                                        //
                                        SETUP_FRAME_VERSION,
                                        SETUP_FRAME_SERVER_NUMBER,
                                        SETUP_CONSOLE_VERSION,
                                        SETUP_SERIAL_NUMBER_NAME,
                                        SETUP_ACCUMULATED_TIME_NAME,
                                        SETUP_ACCUMULATED_DISTANCE_NAME, 
                                        SETUP_HJS_total,
                                        //
                                        SETUP_MIN_ABSOLUTE_INCLINE_ADC_NAME,
                                        SETUP_MAX_ABSOLUTE_INCLINE_ADC_NAME,
                                        SETUP_MaxRPM_PARAMETER,
                                        SETUP_MinRPM_PARAMETER,
                                        SETUP_RF_SERVICE_NAME
                                      } ;

const unsigned short PStringIndex[SETUP_PARAMETER_NUMBER] = {
                                        EE_MAX_TIME,
                                        EE_USER_TIME,
                                        EE_LEVEL,
                                        EE_AGE,
                                        EE_WEIGHT,
                                        EE_StartSpeed,
                                        EE_MaxSpeed,
                                        EE_UNIT,
                                        EE_OutOfOrder,
                                        EE_GENDER,                                      
                                        EE_LANGUAGE,
                                        EE_BEEP,
                                        EE_NAVY_Test,
                                        EE_PauseTime,
                                        EE_InUser,
                                        EE_ErrorCode,
                                        EE_CLUB_ID,
                                        EE_Audio,
                                        EE_SleepTime,
                                        EE_VOLUME,
                                        EE_MaxVolume,
                                        EE_OutputVolume,
                                        EE_Logo,
                                        EE_TVPower,
                                        EE_MaxDF_Elevation,
                                        EE_Speaker,
                                        EE_xID,
                                        EE_DemoMode,
                                        EE_HJS_Switch,
                                        EE_Wireless, 
                                        EE_HJS_CheckNumber,
                                        //
                                        EE_FrameVersion,
                                        EE_FrameSerialNumber,
                                        EE_ConsoleVersion, 
                                        EE_SerialNumber, 
                                        EE_ACCUMULATED_TIME,
                                        EE_ACCUMULATED_DISTANCE,
                                        EE_HJS_total,
                                        //
                                        EE_LOW_ELEVATION,
                                        EE_MAX_ELEVATION,
                                        EE_MaxRPM_Parameter,
                                        EE_MinRPM_Parameter,
                                        EE_RF_SERVICE
                                      } ;

//==============================================================================
// Export EEPROM Data to USB
// File Format:
//             ModuleNo+0D+0A
//             EEPROM Data ( 1024 )
//             0D+0A+CHECK SUM+0D+0A
//==============================================================================
unsigned char UsbFunction_ExportEngineerParameter(void)
{
  F_FILE *file; 
  unsigned short len ;
  unsigned char i ;
  unsigned char j ;
  unsigned char TempFileHandle[150] ;
  ExportInportFileDataHeader *FileDataHeader ;
  ExportInportFileData       *FileData ;
  unsigned char *pData ;
  // Open File
  //
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
      file = f_open(PARAMETER_FILENAME,"w+"); 
      if( !file )
          {//==>开档失败
          f_close(file) ;
          return 0 ;
          } 
      else
          {
          FileDataHeader = (ExportInportFileDataHeader *)&TempFileHandle[0] ;
          // Save File Header
          __memset(&TempFileHandle[0],0,150) ;      // Clear buffer
          FileDataHeader->Mark[0] = 'L' ;
          FileDataHeader->Mark[1] = 'E' ;
          FileDataHeader->Mark[2] = 'D' ;
          sprintf((char*)&FileDataHeader->MachineName[0],"%s",User_ModuleNo);
          FileDataHeader->ParameterCount = SETUP_PARAMETER_NUMBER ;
          FileDataHeader->CheckSum = 0 ;
          FileDataHeader->Version = 0x00000010 ; // 1.0
          FileDataHeader->EndMark = DataEndMark ;
          len = FileDataHeaderSize ;
          if( stm32uh_hub_connected() == 0 )
              return 0 ; 
          
          f_write(&TempFileHandle[0],1,len,file); // ==> Write File Header
          // Write Data
          FileData = (ExportInportFileData *)&USB_WriteBuffer[0] ;
          for( i = 0 ; i < SETUP_PARAMETER_NUMBER ; i++ )
              {
              __memset(&USB_WriteBuffer[0],0,200) ;      // Clear buffer
              // copy prarmeter name to buffer  
              for( j = 0 ; j < 100 ; j++ )
                  {
                  if( *(PStringName[i]+j) != 0x00 )
                      {
                      FileData->Name[j] = *(PStringName[i]+j) ;
                      FileDataHeader->CheckSum += FileData->Name[j] ;
                      }
                  else
                      break ;
                  }
              //
              FileData->Value = EEPROM_ReadInformation(PStringIndex[i]) ;
              pData = (unsigned char *)&FileData->Value ;
              FileDataHeader->CheckSum += *(pData) ;
              FileDataHeader->CheckSum += *(pData+1) ;
              FileDataHeader->CheckSum += *(pData+2) ;
              FileDataHeader->CheckSum += *(pData+3) ;
              //
              FileData->EndMark = DataEndMark ;
              pData = (unsigned char *)&FileData->EndMark ;
              FileDataHeader->CheckSum += *(pData) ;
              FileDataHeader->CheckSum += *(pData+1) ;
              FileDataHeader->CheckSum += *(pData+2) ;
              FileDataHeader->CheckSum += *(pData+3) ;
              // Write Data
              len = FileDataSize ;
              if( stm32uh_hub_connected() == 0 )
                  return 0 ; 
              
              f_write(&USB_WriteBuffer[0],1,len,file); // ==> Write File Header
              }
          // Save Check Sum
          if(!f_seek(file,0,FN_SEEK_SET))
              {
              len = FileDataHeaderSize ;
              if( stm32uh_hub_connected() == 0 )
                  return 0 ; 
              
              f_write(&TempFileHandle[0],1,len,file); // ==> Write File Header
              }
          else
              {
              f_close(file) ;
              return 0 ;
              }
          //
          f_close(file) ;
          }
          return 1 ;
      }
  else
  {
      return 0;
  }
  // 
  
}


//==============================================================================
// Import EEPROM data from USB
//==============================================================================
unsigned char UsbFunction_InportEngineerParameter(unsigned char InportMode)
{
  F_FILE *file; 
  unsigned long len = 0 ;
  unsigned long DataLength = FileDataSize * SETUP_PARAMETER_NUMBER ;
  unsigned char TempData ;
  unsigned char TempFileHandle[150] ;
  unsigned long CheckSum = 0 ;
  unsigned char RetValue = 0 ;
  unsigned char i ;
  unsigned char j ;
  unsigned char InportNumber = SETUP_INPORT_NUMBER ;
  //
  if( InportMode == 1 )
      InportNumber = SETUP_INPORT1_NUMBER ;  
  //  
  ExportInportFileDataHeader *FileDataHeader ;
  ExportInportFileData       *FileData ;
  //
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
      file = f_open(PARAMETER_FILENAME,"r"); 
      if( !file )
          {//==>开档失败
          f_close(file) ;
          return 0 ;
          }
      else
          {
          FileDataHeader = (ExportInportFileDataHeader *)&TempFileHandle[0] ;
          // Save File Header
          __memset(&TempFileHandle[0],0,150) ;      // Clear buffer
          if( stm32uh_hub_connected() == 0 )
              {
              return 0 ; 
              }
          f_read(&TempFileHandle[0],1,FileDataHeaderSize,file); //==> Read 136 byte
          if( __memcmp(&FileDataHeader->Mark[0],"LED",4) == 0 )
              {
              if( __memcmp(&FileDataHeader->MachineName[0],&User_ModuleNo[0],20) == 0 ) 
                  {
                  if( FileDataHeader->ParameterCount == SETUP_PARAMETER_NUMBER )
                      {
                      TempData = 0 ;
                      // Calculator Check sum
                      do
                          {
                          CheckSum += TempData ;
                          len += 1 ;
                          //
                          if( stm32uh_hub_connected() == 0 )
                              {
                              return 0 ; 
                              }
                          // Check Length
                          if( (len-1) > DataLength )
                              {
                              f_close(file) ;
                              return 0 ;
                              }
                          //----------------------------------------------------
                          } while( f_read(&TempData,1,1,file) != 0 ) ;
                      //--------------------------------------------------------
                      if( CheckSum == FileDataHeader->CheckSum )
                          {
                          // Inport Data To EEPROM
                          FileData = (ExportInportFileData *)&USB_WriteBuffer[0] ;  
                          if(!f_seek(file,FileDataHeaderSize,FN_SEEK_SET))
                              {
                              for( j = 0 ; j < SETUP_PARAMETER_NUMBER ; j++ )
                                  {
                                  __memset(&USB_WriteBuffer[0],0,200) ;      // Clear buffer
                                  
                                  //
                                  if( stm32uh_hub_connected() == 0 )
                                      {
                                      return 0 ; 
                                      }
                                  //
                                  if( f_read(&USB_WriteBuffer[0],1,FileDataSize,file) != 0 )
                                      {
                                      if( FileData->EndMark == DataEndMark )
                                          {
                                          //
                                          for( i = 0 ; i < InportNumber ; i++ )
                                              {
                                              if( __memcmp(&FileData->Name[0],PStringName[i],__strlen((const char*)&FileData->Name[0])) == 0 )  
                                                  {
                                                  RetValue = 1 ;
                                                  EEPROM_WriteInformation(PStringIndex[i],FileData->Value) ;
                                                  break ;
                                                  }
                                              }
                                          }
                                      }
                                  else
                                      break ;
                                  }
                              }
                          }
                      //--------------------------------------------------------
                      }
                  }
              }
          // Close File
          f_close(file) ;
          //
          // Write Data to EEPROM and Read Initial
          if( RetValue == 1 )
              {   
              EEPROM_WriteParameter() ;
              EEPROM_Read_Initial() ;
              }
          //
          //    
          }
      }
  return RetValue ;
}



//#endif

//#ifdef    _ExportAndInport_Mode1_
//------------------------------------------------------------------------------
// Add by Kunlung 20110127
//#define   PARAMETER_FILENAME            "UCB_PARA.pam"
//==============================================================================
// Export EEPROM Data to USB
// File Format:
//             ModuleNo+0D+0A
//             EEPROM Data ( 1024 )
//             0D+0A+CHECK SUM+0D+0A
//==============================================================================
/*
unsigned char UsbFunction_ExportEngineerParameter(void)
{
  F_FILE *file; 
  unsigned short Addr = 0 ;
  unsigned char ExportData ;
  unsigned short len ;
  unsigned short CheckSum = 0 ;
  // Open File
  //
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
      file = f_open(PARAMETER_FILENAME,"w+"); 
      if( !file )
          {//==>开档失败
          f_close(file) ;
          return 0 ;
          } 
      else
          {
          //
          __memset(&USB_WriteBuffer[0],0,sizeof(USB_WriteBuffer)) ;  
          // Save ModuleNo+0D+0A
          sprintf(USB_WriteBuffer,"%s\r\n",User_ModuleNo);
          len = __strlen(USB_WriteBuffer) ;//==>计算数据长度
          if( stm32uh_hub_connected() == 0 )
              {
              //f_close(file) ;
              return 0 ; 
              } 
          f_write(&USB_WriteBuffer[0],1,len,file);//==>写入USB     
          // Save EEprom Data
          while( EEPROM_ExportEEMemoryData( Addr ,&ExportData) == 1 )
              {
              if( stm32uh_hub_connected() == 0 )
                  {
                  //f_close(file) ;
                  return 0 ; 
                  }
              //
              f_write(&ExportData,1,1,file);//==>写入USB 
              Addr += 1 ;
              CheckSum += ExportData ;
              }
          //--------------------------------------------------------------------
          // Check Sum
          CheckSum = 0xFFFF - CheckSum ;
          // Save ModuleNo+0D+0A+CheckSum+OD+OA
          sprintf(USB_WriteBuffer,"\r\n%x\r\n",CheckSum);
          len = __strlen(USB_WriteBuffer) ;//==>计算数据长度
          if( stm32uh_hub_connected() == 0 )
              {
              //f_close(file) ;
              return 0 ; 
              }
          f_write(&USB_WriteBuffer[0],1,len,file);//==>写入USB  
          //
          // Close File 
          f_close(file) ;
          }
      }
  // 
  return 1 ;
}
*/
//==============================================================================
// Import EEPROM data from USB
//
//==============================================================================
/*
unsigned char UsbFunction_InportEngineerParameter(void)
{
  F_FILE *file; 
  unsigned short Addr = 0 ;
  unsigned short i ;
  unsigned char Temp[8] ;
  unsigned short CheckSum ;
  unsigned short ShiftData ;
  unsigned short ReadCheckSum ;
  long Datalen ;
  //
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
      file = f_open(PARAMETER_FILENAME,"r"); 
      if( !file )
          {//==>开档失败
          f_close(file) ;
          return 0 ;
          }
      else
          {
          // check File format
           __memset(&USB_ReadBuffer[0],0,sizeof(USB_ReadBuffer)) ;  
          if( stm32uh_hub_connected() == 0 )
              {
              //f_close(file) ;
              return 0 ; 
              }
          f_read(&USB_ReadBuffer[0],1,100,file); //==>读1024
          for( i = 0 ; i < 20 ; i++ )
              {
              if( USB_ReadBuffer[i] != 0x0D && USB_ReadBuffer[i+1] != 0x0A )
                  {
                  if( USB_ReadBuffer[i] != User_ModuleNo[i] )
                      {
                      f_close(file) ;
                      return 0 ; 
                      }
                  }
              else
                  {
                  // Read All Data To Buffer
                  if( stm32uh_hub_connected() == 0 )
                      {
                      //f_close(file) ;
                      return 0 ; 
                      }
                  
                  if(!f_seek(file,(i+2),FN_SEEK_SET))
                      {
                      // Read Data   
                      if( stm32uh_hub_connected() == 0 )
                          {
                          //f_close(file) ;
                          return 0 ; 
                          }
                                           
                      Datalen = f_read(&USB_ReadBuffer[0],1,1024,file); //==>读1024
                      if( Datalen == 1024 ) // Check Length
                          {
                          // Read Check Sum
                          if( stm32uh_hub_connected() == 0 )
                              {
                              //f_close(file) ;
                              return 0 ; 
                              }
                          Datalen = f_read(&Temp[0],1,8,file); //==>读1024
                          if( Datalen == 8 )
                              {
                              CheckSum = 0 ;
                              ShiftData = 0x1000 ;
                              if( Temp[0] == 0x0d && Temp[1] == 0x0a && Temp[6] == 0x0d && Temp[7] == 0x0a )
                                  {
                                  for( i = 2 ; i < 6 ; i ++ )
                                      {
                                      if( ( Temp[i] < 0x61 && Temp[i] > 0x39 ) || Temp[i] > 0x66  || Temp[i] < 0x30 )
                                          {
                                          f_close(file) ;
                                          return 0 ; 
                                          }
                                      else
                                          {
                                          if( Temp[i] < 0x61 )
                                              {
                                              CheckSum += (unsigned short)(Temp[i]-0x30) * ShiftData ;
                                              }
                                          else
                                              {
                                                // -0x61+ 10 = -0x57
                                              CheckSum += (unsigned short)(Temp[i]-0x57) * ShiftData ;
                                              }
                                          }
                                      //
                                      ShiftData = ShiftData >> 4 ;
                                      //
                                      }
                                  //
                                  ReadCheckSum = 0 ;
                                  for( i = 0 ; i < 1024 ; i++ )
                                      ReadCheckSum += USB_ReadBuffer[i] ;
                                  //
                                  ReadCheckSum = 0xFFFF - ReadCheckSum ;
                                  if( CheckSum != ReadCheckSum )
                                      {
                                      f_close(file) ;
                                      return 0 ;
                                      }
                                  //    
                                  }
                              else
                                  {
                                  f_close(file) ;
                                  return 0 ; 
                                  }
                              }
                          else
                              {
                              f_close(file) ;
                              return 0 ; 
                              }
                          }
                      else
                          {
                          f_close(file) ;
                          return 0 ; 
                          }
                      //
                      }
                  else
                      {
                      f_close(file) ;
                      return 0 ; 
                      }
                  break ;
                  }
              }
          // Close File
          f_close(file) ;
          // Inport Data To Memory 
          Addr = 0 ;
          while( EEPROM_InportEEMemoryData( Addr, USB_ReadBuffer[Addr] ) == 1 )
              {
              Addr += 1 ; 
              } 
          // write to EEPROM
          EEPROM_WriteParameter() ;
          EEPROM_Read_Initial() ;
          //
          //    
          }
      }
  return 1 ;
}
*/

// 2014.10.22
#define WIFICFG_FILENAME    "wifi_set.cfg"
#define WIFICFG_SSID        "SSID="
#define WIFICFG_Key         "Key="
#define WIFICFG_Security    "Security="
#define WIFICFG_Encryption  "Encryption=AES"
#define WIFICFG_KeyIndex    "KeyIndex=1"
#define WIFICFG_TimeZone    "TimeZone="
// Security Type
#define _SECURITY_UNSECURED       "None"
#define _SECURITY_WEP40           "WEP"
#define _SECURITY_WEP104          "WEP-104Bits"
#define _SECURITY_WPA_PHRASE      "WPA-PSK"
#define _SECURITY_WPA2_PHRASE     "WPA2-PSK"
#define _SECURITY_WPA_AUTO_PHRASE "WPA_AUTO-PSK"
#define _SECURITY_AUTO_DETECT     "AUTO_DETECT"
#define NUM_SECURITY_TYPES        "NUM" 
//
//==============================================================================
//==>> UsbFunction_ExportWifiConfig()
//==>> 将WiFi连AP资料存到USB
//==============================================================================
char UsbFunction_ExportWifiConfig(void)
{
  F_FILE *file91;
  unsigned short len ;
  char TempData[50] ;
  
  //==>当没有插USB时就不作动
  if( Boot_CheckPenDriverPlunin() == _USBNotPlugin_ ) return C_WriteFail;
  //
  EEPROM_ReadInformation(EE_WiFi_SSID);
  EEPROM_ReadInformation(EE_WiFi_Password);
  EEPROM_ReadInformation(EE_WiFi_Security);
  UsbFunction_ClearReadWriteBuffer(0);
  //==>> 使用sprintf()指令将字符串内对应数值填入
  sprintf(USB_FileNameMerge,"/MATRIX/%s",WIFICFG_FILENAME);
  // <<=====================================================================
  file91 = f_open(USB_FileNameMerge,"w+");//==>开档并可写档 
  if( !file91 )
  {//==>开档失败
      f_close(file91) ;
      return C_OpenFail;
  }  
  
  switch(wifi_settings.security.security_type)
  {
      case 0:sprintf(TempData,"%s%s",WIFICFG_Security,_SECURITY_UNSECURED);
             break;
      case 1:sprintf(TempData,"%s%s",WIFICFG_Security,_SECURITY_WEP40);
             break;
      case 2:sprintf(TempData,"%s%s",WIFICFG_Security,_SECURITY_WEP104);
             break;
      case 3:sprintf(TempData,"%s%s",WIFICFG_Security,_SECURITY_WPA_PHRASE);
             break;
      case 4:sprintf(TempData,"%s%s",WIFICFG_Security,_SECURITY_WPA2_PHRASE);
             break;
      case 5:sprintf(TempData,"%s%s",WIFICFG_Security,_SECURITY_WPA_AUTO_PHRASE);
             break;
      case 6:sprintf(TempData,"%s%s",WIFICFG_Security,_SECURITY_AUTO_DETECT);
             break;
      case 7:sprintf(TempData,"%s%s",WIFICFG_Security,NUM_SECURITY_TYPES);
             break;
  }
  sprintf(USB_WriteBuffer,"[WIFI AP]\r\n%s%s\r\n%s\r\n%s\r\n%s%s\r\n%s\r\n%s%d\r\n",
          WIFICFG_SSID,wifi_settings.security.ssid,
          TempData,
          WIFICFG_Encryption,
          WIFICFG_Key,wifi_settings.security.security_key,
          WIFICFG_KeyIndex,
          WIFICFG_TimeZone,EEPROM_ReadInformation(EE_TZone)
          );
  len = __strlen(USB_WriteBuffer)  ;//==>计算数据长度
  f_write(&USB_WriteBuffer[0],1,len,file91);//==>写入USB
  UsbFunction_ClearReadWriteBuffer(0);
  f_close(file91) ;//==>关档案
  BEEP_Proce(4,2,2);//==>完成后毕两声
  return C_WriteOK;
}
//==============================================================================
//==>> UsbFunction_ReadWifiConfig()
//==>> 由USB将WiFi连AP数据取出并联机
//==============================================================================
char UsbFunction_ReadWifiConfig(void)
{
  F_FILE *file; 
  unsigned long filesize ;
  char *pch ;
  unsigned char DataLength ;
  char TempData[128] ;
  unsigned char _Sum = 0 ;
  
  //==>当没有插USB时就不作动
  if( Boot_CheckPenDriverPlunin() == _USBNotPlugin_ ) return C_WriteFail;
  //
  UsbFunction_ClearReadWriteBuffer(0);
  //==>> 使用sprintf()指令将字符串内对应数值填入
  sprintf(USB_FileNameMerge,"/MATRIX/%s",WIFICFG_FILENAME);
  // <<=====================================================================
  file = f_open(USB_FileNameMerge,"r"); 
  if( !file )
  {//==>开档失败
      f_close(file) ;
      return 0 ;
  }
  else
  {
      // check File format
      __memset(&USB_ReadBuffer[0],0,sizeof(USB_ReadBuffer)) ;
      if( stm32uh_hub_connected() == 0 )
      {
          return 0 ; 
      }
      //
      filesize = f_read(&USB_ReadBuffer[0],1,1024,file); //==>读1024
      if( filesize <= 1024 )
      { 
          // SSID  
          pch = __strstr(USB_ReadBuffer,WIFICFG_SSID) ;
          if( pch != NULL )
          {
              pch += 5 ;  
              __memset(&TempData[0],0,128) ;
              for( DataLength = 0 ; DataLength < 32 ; DataLength++ )
              {
                  if( *(pch+DataLength) == 0x0D && *(pch+DataLength+1) == 0x0A )
                  {
                      break ;
                  }
                  else
                      TempData[DataLength] = *(pch+DataLength) ;
              }
              // Set To WiFi Module Data Buffer   
              if( DataLength != 0 ) 
              {
                  __memset( wifi_settings.security.ssid, 0, 33 );
                  __strncpy( wifi_settings.security.ssid, TempData, __strlen(TempData));
              }
          }
          // Security
          pch = __strstr(USB_ReadBuffer,WIFICFG_Security) ;
          if( pch != NULL )
          {
              pch += 9 ;  
              __memset(&TempData[0],0,128) ;
              for( DataLength = 0 ; DataLength < 32 ; DataLength++ )
              {
                  if( *(pch+DataLength) == 0x0D && *(pch+DataLength+1) == 0x0A )
                      break ;
                  else
                      TempData[DataLength] = *(pch+DataLength) ;
              }
              // Set To WiFi Module Data Buffer   
              if( DataLength != 0 )
              {
                  if(strcmp(_SECURITY_UNSECURED,(char const*)&TempData[0]) == 0)
                      wifi_settings.security.security_type = WIFI_SECURITY_UNSECURED;
                  else if(strcmp(_SECURITY_WEP40,(char const*)&TempData[0]) == 0)  
                      wifi_settings.security.security_type = WIFI_SECURITY_WEP40;
                  else if(strcmp(_SECURITY_WEP104,(char const*)&TempData[0]) == 0)  
                      wifi_settings.security.security_type = WIFI_SECURITY_WEP104;
                  else if(strcmp(_SECURITY_WPA_PHRASE,(char const*)&TempData[0]) == 0)  
                      wifi_settings.security.security_type = WIFI_SECURITY_WPA_PHRASE;
                  else if(strcmp(_SECURITY_WPA2_PHRASE,(char const*)&TempData[0]) == 0)  
                      wifi_settings.security.security_type = WIFI_SECURITY_WPA2_PHRASE;
                  else if(strcmp(_SECURITY_WPA_AUTO_PHRASE,(char const*)&TempData[0]) == 0)  
                      wifi_settings.security.security_type = WIFI_SECURITY_WPA_AUTO_PHRASE;
                  else if(strcmp(_SECURITY_AUTO_DETECT,(char const*)&TempData[0]) == 0)  
                      wifi_settings.security.security_type = WIFI_SECURITY_AUTO_DETECT;
                  else if(strcmp(NUM_SECURITY_TYPES,(char const*)&TempData[0]) == 0)  
                      wifi_settings.security.security_type = NUM_WIFI_SECURITY_TYPES;
              }
          }
          // Key
          pch = __strstr(USB_ReadBuffer,WIFICFG_Key) ;
          if( pch != NULL )
          {
              pch += 4 ;  
              __memset(&TempData[0],0,128) ;
              for( DataLength = 0 ; DataLength < 32 ; DataLength++ )
              {
                  if( *(pch+DataLength) == 0x0D && *(pch+DataLength+1) == 0x0A )
                  {
                      break ;
                  }
                  else
                      TempData[DataLength] = *(pch+DataLength) ;
              }
              // Set To WiFi Module Data Buffer   
              if( DataLength != 0 )
              {
                  __memset( wifi_settings.security.security_key, 0, 255 );
                  __strncpy( wifi_settings.security.security_key, TempData, __strlen(TempData));
              }
          }
          // TimeZone
          pch = __strstr(USB_ReadBuffer,WIFICFG_TimeZone) ;
          if( pch != NULL )
          {
              pch += 9 ;  
              for( DataLength = 0 ; DataLength < 2 ; DataLength++ )
              {
                  if( *(pch+DataLength) == 0x0D && *(pch+DataLength+1) == 0x0A )
                      break ;
                  else
                  {
                      _Sum *= 10;
                      _Sum += *(pch+DataLength) - '0';
                  }
              }  
              //save wifi config to EEPRom.SSID Password has no parameter 
              EEPROM_WriteInformation(EE_WiFi_SSID,0);
              EEPROM_WriteInformation(EE_WiFi_Password,0);
              EEPROM_WriteInformation(EE_WiFi_Security,wifi_settings.security.security_type);
              EEPROM_SaveWorkInformation(EE_TZone,_Sum);
              WiFi_Information_TimeZoneUpdate(_Sum);
          }
          //
      }
      f_close(file) ;
      WIFIClearStateBuffer();//To ensure a successful connection,need clear state buffer
      ConnectWiFi_AP();      
      return 1 ;
  }
}
//





/*
//#endif

#ifdef WiFi_Module
#define   WIFICFG_FILENAME        "wifi_set.cfg"
#define   WIFICFG_SSID            "SSID="
#define   WIFICFG_Security        "Security="
#define   WIFICFG_Key             "Key="
#define   WIFICFG_Address         "IP Address="
#define   WIFICFG_Port            "Port="
// Security Type
#define   SecurityType0           "None"
#define   SecurityType1           "WEP"
#define   SecurityType2           "WEP-104Bits"
#define   SecurityType3           "WPA-KEY"
#define   SecurityType4           "WPA-PSK"
#define   SecurityType5           "WPA2-KEY"
#define   SecurityType6           "WPA2-PSK"
#define   SecurityType7           "WPA_AUTO-KEY"
#define   SecurityType8           "WPA_AUTO-PSK" 
//
#define   WEP_40Bits              1
#define   WEP_104Bits             2
#define   WPA_KEY                 3
#define   WPA_PHRASE              4
#define   WPA2_KEY                5
#define   WPA2_PHRASE             6
#define   WPA_AUTO_KEY            7
#define   WPA_AUTO_PHRASE         8


// 20120111

extern WiFiModule WiFiModuleData ;
extern volatile WiFiControlStatus   WiFiProcessStatus ;

//
unsigned char UsbFunction_ReadWifiConfig(void)
{
  //
  F_FILE *file; 
  unsigned long filesize ;
  char *pch ;
  unsigned char DataLength ;
  unsigned char TempData[128] ;
  unsigned short MulData = 0 ;
  unsigned short Data = 0 ;
  unsigned char i ;
  unsigned char SecurityType ;
  // 20120111 Add by Kunlung 
  unsigned char SetAPStatus = 0 ;
  unsigned char SetTCPIPStatus = 0 ;
  unsigned char RetuenStatus = 0 ;
  unsigned char TempKey[33] ;
  union {
    struct {
      unsigned char L:4;
      unsigned char H:4;
    } NB ;
    unsigned char Key ;
  } Mdata ;
  __memset(&TempKey[0],0,sizeof(TempKey)) ;
  //
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
      file = f_open(WIFICFG_FILENAME,"r"); 
      if( !file )
          {//==>开档失败
          f_close(file) ;
          return 0 ;
          }
      else
          {
          // check File format
          __memset(&USB_ReadBuffer[0],0,sizeof(USB_ReadBuffer)) ;
          if( stm32uh_hub_connected() == 0 )
              {
              //f_close(file) ;
              return 0 ; 
              }
          //
          filesize = f_read(&USB_ReadBuffer[0],1,1024,file); //==>读1024
          if( filesize <= 1024 )
              { 
              // SSID  
              pch = __strstr(USB_ReadBuffer,WIFICFG_SSID) ;
              if( pch != NULL )
                  {
                  pch += 5 ;  
                  __memset(&TempData[0],0,128) ;
                  for( DataLength = 0 ; DataLength < 32 ; DataLength++ )
                      {
                      if( *(pch+DataLength) == 0x0D && *(pch+DataLength+1) == 0x0A )
                          {
                          break ;
                          }
                      else
                          TempData[DataLength] = *(pch+DataLength) ;
                      }
                  // Set To WiFi Module Data Buffer   
                  if( DataLength != 0 ) 
                      {
                      WiFi_DefaultSSID(&TempData[0]) ;
                      SetAPStatus = 1 ; // 20120111
                      }
                  }
              
              if( SetAPStatus == 1 ) // 20120111
                  {
                  //
                  SetAPStatus = 0 ; // 20120111
              // Security
              pch = __strstr(USB_ReadBuffer,WIFICFG_Security) ;
              if( pch != NULL )
                  {
                  pch += 9 ;  
                  __memset(&TempData[0],0,128) ;
                  for( DataLength = 0 ; DataLength < 32 ; DataLength++ )
                      {
                      if( *(pch+DataLength) == 0x0D && *(pch+DataLength+1) == 0x0A )
                          {
                          break ;
                          }
                      else
                          TempData[DataLength] = *(pch+DataLength) ;
                      }
                  // Set To WiFi Module Data Buffer   
                  if( DataLength != 0 )
                      {
                          //
                          SetAPStatus = 1 ; // 20120111
                      // None 
                      SecurityType = 0 ;
                      if( __strcmp(SecurityType0,(char const*)&TempData[0]) == 0)  
                          WiFi_DefaultSecurityType(0) ;
                      else
                          {
                          SecurityType = WEP_40Bits ;
                          // WEP_40Bits
                          if( __strcmp(SecurityType1,(char const*)&TempData[0]) == 0)  
                              WiFi_DefaultSecurityType(1) ;
                          else
                              {
                              SecurityType = WEP_104Bits ;
                              // WEP_104Bits
                              if( __strcmp(SecurityType2,(char const*)&TempData[0]) == 0)  
                                  WiFi_DefaultSecurityType(2) ;
                              else
                                  {
                                  SecurityType = WPA_KEY ;
                                  // WPA_KEY
                                  if( __strcmp(SecurityType3,(char const*)&TempData[0]) == 0)  
                                      WiFi_DefaultSecurityType(3) ;
                                  else
                                      {
                                      SecurityType = WPA_PHRASE ;
                                      // WPA_PHRASE
                                      if( __strcmp(SecurityType4,(char const*)&TempData[0]) == 0)  
                                          WiFi_DefaultSecurityType(4) ;
                                      else
                                          {
                                          SecurityType = WPA2_KEY ;
                                          // WPA2_KEY
                                          if( __strcmp(SecurityType5,(char const*)&TempData[0]) == 0)  
                                              WiFi_DefaultSecurityType(5) ;
                                          else
                                              {
                                              SecurityType = WPA2_PHRASE ;
                                              // WPA2_PHRASE
                                              if( __strcmp(SecurityType6,(char const*)&TempData[0]) == 0)  
                                                  WiFi_DefaultSecurityType(6) ;
                                              else
                                                  {
                                                  SecurityType = WPA_AUTO_KEY ;
                                                  // WPA_AUTO_KEY
                                                  if( __strcmp(SecurityType7,(char const*)&TempData[0]) == 0)  
                                                      WiFi_DefaultSecurityType(7) ;
                                                  else
                                                      {
                                                      SecurityType = WPA_AUTO_PHRASE ;
                                                      // WPA_AUTO_PHRASE
                                                      if( __strcmp(SecurityType8,(char const*)&TempData[0]) == 0)  
                                                          WiFi_DefaultSecurityType(8) ;
                                                      else
                                                              {
                                                          SecurityType = 9 ;
                                                              //
                                                              SetAPStatus = 0 ; // 20120111
                                                              //
                                                              }
                                                      }
                                                  }
                                              }
                                          }
                                      }
                                  }
                              }
                          }
                      }
                  }
              
              // Key
              if( SecurityType < 9 )
                  {
                      //  
                      SetAPStatus = 0 ; // 20120111
                      //
              pch = __strstr(USB_ReadBuffer,WIFICFG_Key) ;
              if( pch != NULL )
                  {
                  pch += 4 ;  
                  __memset(&TempData[0],0,128) ;
                      if( SecurityType != 0 )
                          {
                          for( DataLength = 0 ; DataLength < 128 ; DataLength++ )
                      {
                      if( *(pch+DataLength) == 0x0D && *(pch+DataLength+1) == 0x0A )
                          {
                          break ;
                          }
                      else
                          TempData[DataLength] = *(pch+DataLength) ;
                      }
                          //
                          switch( SecurityType )
                              {
                              case  WEP_40Bits      :
                                                        if( DataLength == 5 ) // ASCII
                                                        {
                                                        WiFi_DefaultSecurityKey(&TempData[0]) ;
                                                            SetAPStatus = 1 ; // 20120111
                                                            }
                                                        else
                                                            {
                                                            if( DataLength == 10 ) // HEX
                                                                {
                                                                for( i = 0 ; i < 5 ; i++ )
                                                                    {
                                                                    //  
                                                                    if( TempData[i*2] >= '0' && TempData[i*2] <= '9' )
                                                                        {
                                                                        Mdata.NB.H = TempData[i*2] - '0' ;
                                                                        }
                                                                    else
                                                                        {
                                                                        if( TempData[i*2] >= 'A' && TempData[i*2] <= 'F' )
                                                                            {
                                                                            Mdata.NB.H = TempData[i*2] - 'A' + 0x0A ;
                                                                            }
                                                                        else
                                                                            {
                                                                            if( TempData[i*2] >= 'a' && TempData[i*2] <= 'f' )
                                                                                {
                                                                                Mdata.NB.H = TempData[i*2] - 'a' + 0x0A ;
                                                                                }
                                                                            else
                                                                                {
                                                                                break ;
                                                                                }
                                                                            }
                                                                        }
                                                                    //
                                                                    if( TempData[((i*2)+1)] >= '0' && TempData[((i*2)+1)] <= '9' )
                                                                        {
                                                                        Mdata.NB.L = TempData[((i*2)+1)] - '0' ;
                                                                        }
                                                                    else
                                                                        {
                                                                        if( TempData[((i*2)+1)] >= 'A' && TempData[((i*2)+1)] <= 'F' )
                                                                            {
                                                                            Mdata.NB.L = TempData[((i*2)+1)] - 'A' + 0x0A ;
                                                                            }
                                                                        else
                                                                            {
                                                                            if( TempData[((i*2)+1)] >= 'a' && TempData[((i*2)+1)] <= 'f' )
                                                                                {
                                                                                  Mdata.NB.L = TempData[((i*2)+1)] - 'a' + 0x0A ;
                                                                                }
                                                                            else
                                                                                {
                                                                                break ;
                                                                                }
                                                                            }
                                                                        }
                                                                    //
                                                                    TempKey[i] = Mdata.Key ;
                                                                    //
                                                                    }
                                                                //
                                                                if( i == 5 )
                                                                    {
                                                                    WiFi_DefaultSecurityKey(&TempKey[0]) ;
                                                                    SetAPStatus = 1 ; // 20120111
                                                                    }
                                                                }
                                                        }
                                                    break ;
                              case  WEP_104Bits     :
                                                        if( DataLength == 13 ) // ASCII
                                                        {
                                                        WiFi_DefaultSecurityKey(&TempData[0]) ;
                                                            SetAPStatus = 1 ; // 20120111
                                                            }
                                                        else
                                                            {
                                                            if( DataLength == 26 ) // HEX
                                                                {
                                                                for( i = 0 ; i < 13 ; i++ )
                                                                    {
                                                                    //  
                                                                    if( TempData[i*2] >= '0' && TempData[i*2] <= '9' )
                                                                        {
                                                                        Mdata.NB.H = TempData[i*2] - '0' ;
                                                                        }
                                                                    else
                                                                        {
                                                                        if( TempData[i*2] >= 'A' && TempData[i*2] <= 'F' )
                                                                            {
                                                                            Mdata.NB.H = TempData[i*2] - 'A' + 0x0A ;
                                                        }
                                                                        else
                                                                            {
                                                                            if( TempData[i*2] >= 'a' && TempData[i*2] <= 'f' )
                                                                                {
                                                                                Mdata.NB.H = TempData[i*2] - 'a' + 0x0A ;
                                                                                }
                                                                            else
                                                                                {
                                                                                break ;
                                                                                }
                                                                            }
                                                                        }
                                                                    //
                                                                    if( TempData[((i*2)+1)] >= '0' && TempData[((i*2)+1)] <= '9' )
                                                                        {
                                                                        Mdata.NB.L = TempData[((i*2)+1)] - '0' ;
                                                                        }
                                                                    else
                                                                        {
                                                                        if( TempData[((i*2)+1)] >= 'A' && TempData[((i*2)+1)] <= 'F' )
                                                                            {
                                                                            Mdata.NB.L = TempData[((i*2)+1)] - 'A' + 0x0A ;
                                                                            }
                                                                        else
                                                                            {
                                                                            if( TempData[((i*2)+1)] >= 'a' && TempData[((i*2)+1)] <= 'f' )
                                                                                {
                                                                                  Mdata.NB.L = TempData[((i*2)+1)] - 'a' + 0x0A ;
                                                                                }
                                                                            else
                                                                                {
                                                                                break ;
                                                                                }
                                                                            }
                                                                        }
                                                                    //
                                                                    TempKey[i] = Mdata.Key ;
                                                                    //
                                                                    }
                                                                //
                                                                if( i == 13 )
                                                                    {
                                                                    WiFi_DefaultSecurityKey(&TempKey[0]) ;
                                                                    SetAPStatus = 1 ; // 20120111
                                                        }
                                                                }
                                                            }
                                                        //
                                                    break ;
                              case  WPA_KEY         :
                              case  WPA2_KEY        :
                              case  WPA_AUTO_KEY    :
                                                        if( DataLength == 64 ) // HEX
                                                            {
                                                            TempKey[32] = 0 ;  
                                                            for( i = 0 ; i < 32 ; i++ )
                                                                {
                                                                //  
                                                                if( TempData[i*2] >= '0' && TempData[i*2] <= '9' )
                                                                    {
                                                                    Mdata.NB.H = TempData[i*2] - '0' ;
                                                                    }
                                                                else
                                                                    {
                                                                    if( TempData[i*2] >= 'A' && TempData[i*2] <= 'F' )
                                                                        {
                                                                        Mdata.NB.H = TempData[i*2] - 'A' + 0x0A ;
                                                                        }
                                                                    else
                                                                        {
                                                                        if( TempData[i*2] >= 'a' && TempData[i*2] <= 'f' )
                                                                            {
                                                                            Mdata.NB.H = TempData[i*2] - 'a' + 0x0A ;
                                                                            }
                                                                        else
                                                                            {
                                                                            break ;
                                                                            }
                                                                        }
                                                                    }
                                                                //
                                                                if( TempData[((i*2)+1)] >= '0' && TempData[((i*2)+1)] <= '9' )
                                                                    {
                                                                    Mdata.NB.L = TempData[((i*2)+1)] - '0' ;
                                                                    }
                                                                else
                                                                    {
                                                                    if( TempData[((i*2)+1)] >= 'A' && TempData[((i*2)+1)] <= 'F' )
                                                                        {
                                                                        Mdata.NB.L = TempData[((i*2)+1)] - 'A' + 0x0A ;
                                                                        }
                                                                    else
                                                                        {
                                                                        if( TempData[((i*2)+1)] >= 'a' && TempData[((i*2)+1)] <= 'f' )
                                                                            {
                                                                              Mdata.NB.L = TempData[((i*2)+1)] - 'a' + 0x0A ;
                                                                            }
                                                                        else
                                                                            {
                                                                            break ;
                                                                            }
                                                                        }
                                                                    }
                                                                //
                                                                TempKey[i] = Mdata.Key ;
                                                                //
                                                                }
                                                            //
                                                            if( i == 32 )
                                                        {
                                                                WiFi_DefaultSecurityKey(&TempKey[0]) ;
                                                                SetAPStatus = 1 ; // 20120111
                                                                }
                                                            //
                                                        }
                                                    break ;
                              case  WPA_PHRASE      :
                              case  WPA2_PHRASE     :                                                    
                              case  WPA_AUTO_PHRASE :
                                                        if( DataLength >= 8 && DataLength <= 63 )
                                                        {
                                                        WiFi_DefaultSecurityKey(&TempData[0]) ;
                                                            SetAPStatus = 1 ; // 20120111
                                                        }
                                                    break ;
                              }
                          }
                      else
                          {
                  // Set To WiFi Module Data Buffer   
                      WiFi_DefaultSecurityKey(&TempData[0]) ;
                              SetAPStatus = 1 ; // 20120111
                  }
                      }
                  }

                  }
              // Address
              pch = __strstr(USB_ReadBuffer,WIFICFG_Address) ;
              if( pch != NULL )
                  {
                  pch += 11 ;  
                  __memset(&TempData[0],0,128) ;
                  for( DataLength = 0 ; DataLength < 128 ; DataLength++ )
                      {
                      if( *(pch+DataLength) == 0x0D && *(pch+DataLength+1) == 0x0A )
                          {
                          break ;
                          }
                      else
                          TempData[DataLength] = *(pch+DataLength) ;
                      }
                  // Set To WiFi Module Data Buffer   
                  if( DataLength != 0 )
                      {
                      WiFi_DefaultTCPIPAddress(&TempData[0]) ;
                      SetTCPIPStatus = 1 ; // 20120111
                      }
                  }
              
              if( SetTCPIPStatus == 1 ) // 20120111
                  {
                  SetTCPIPStatus = 0 ; // 20120111
              // Port
              pch = __strstr(USB_ReadBuffer,WIFICFG_Port) ;
              if( pch != NULL )
                  {
                  pch += 5 ;  
                  __memset(&TempData[0],0,128) ;
                  for( DataLength = 0 ; DataLength < 4 ; DataLength++ )
                      {
                      if( *(pch+DataLength) == 0x0D && *(pch+DataLength+1) == 0x0A )
                          {
                          break ;  
                          }
                      else
                          {
                          if( *(pch+DataLength) >= '0' && *(pch+DataLength) <= '9' )
                              TempData[DataLength] = *(pch+DataLength)- '0' ;
                          else
                              break ;
                          }
                      }
                  // Set To WiFi Module Data Buffer
                  if( DataLength != 0 )
                      {
                      if( DataLength == 4 )
                          {
                          MulData = 1000 ;
                          Data = 0 ;
                          for( i = 0 ; i < DataLength ; i++ )
                              {
                              Data += (unsigned short)TempData[i]*MulData ;
                              MulData /= 10 ;
                              }
                          }
                      else
                          {
                          if( DataLength == 3 )
                              {
                              MulData = 100 ;
                              Data = 0 ;
                              for( i = 0 ; i < DataLength ; i++ )
                                  {
                                  Data += (unsigned short)TempData[i]*MulData ;
                                  MulData /= 10 ;
                                  }
                              }
                          else
                              {
                              if( DataLength == 2 )
                                  {
                                  MulData = 10 ;
                                  Data = 0 ;
                                  for( i = 0 ; i < DataLength ; i++ )
                                      {
                                      Data += (unsigned short)TempData[i]*MulData ;
                                      MulData /= 10 ;
                                      }
                                  }
                              else
                                  {
                                  Data = TempData[0] ;
                                  }
                              }
                          }
                      WiFi_DefaultTCPIPPort(Data) ;
                          SetTCPIPStatus = 1 ;
                      }
                  }
                      }
                  }
          //
          }
              }
  //----------------------------------------------------------------------------
  // 20120111
  if( SetAPStatus == 1 ) 
      {
      WiFiModuleData.IP[0] = 0 ;
      WiFiModuleData.IP[1] = 0 ;
      WiFiModuleData.IP[2] = 0 ;
      WiFiModuleData.IP[3] = 0 ;
      WiFiModuleData.IP[4] = 0 ;
      WiFiModuleData.IP[5] = 0 ;
      //
      WiFiProcessStatus.b.SetDisConnectAP = 1 ;
      Timer_Counter_Clear(0);
      while(!Timer_Counter(T_STEP,0,20)) ;
          //
      WiFiProcessStatus.b.SetSSID = 1 ;
      WiFiProcessStatus.b.SetSecurityType = 1 ;
      WiFiProcessStatus.b.SetSecurityKey = 1 ;
      RetuenStatus = 1 ;
          }
  if( SetTCPIPStatus == 1 ) 
      {
      WiFiProcessStatus.b.SetTCPIPAddress = 1 ;
      WiFiProcessStatus.b.SetTCPIPPort = 1 ;
      RetuenStatus = 1 ;
      }
  //----------------------------------------------------------------------------
  //
  f_close(file) ;
  //return 1 ;
  return RetuenStatus ;
  //  
}
*/


/*
#define   WIFI_FWFilename           "C30.bin"
#define   WIFI_FWStartAddr          0x00400
#define   WIFI_FWEndAddr            0x14800
#define   WIFI_FWBinStartAddress    (FileAddress*2)
#define   WIFI_FWSize               (WIFI_FWEndAddr-WIFI_FWStartAddr)
*/
/*
1.Flash StartCode:0x00400
2.Flash EndCode:0x14800
3.address+2
*/
/*
unsigned char UsbFunction_FirmwareUpdateWiFi(void)
{
  //
  F_FILE *file; 
  unsigned long filesize ;
  unsigned long FileAddress = WIFI_FWStartAddr ;
  unsigned char WorkStep ;
  unsigned char WiFiProgramStatus = 0 ;
  unsigned char RtyTime = 0;
  unsigned char ProgramMode = 0 ;
  unsigned short DataAddr = 0 ;
  unsigned short DataOffset = 0 ;        // Save Send Data Length
  unsigned long DataLength = 0 ;        // Data Total Length
  unsigned char Temp[20] = {0};
  //unsigned char DelayTime = 0 ;
  //unsigned long ReadFileSize = 0 ;
  unsigned char Loop = 1 ;
  unsigned char RetStatus = 0 ;
  unsigned long ReadCounter = 0 ;
  unsigned char WaitStatus = 0 ;
  //unsigned char ReadData[126] ;
  //
  WiFiControlStatus *WiFiStatus ;
  //          123456789012345678
  Led_String("Check WiFi FW File",0,0);
  WorkStep = 0 ;
  //
  WiFiStatus = WiFi_GetWiFiControlStatus() ;
  WiFi_SetCommWorkMode(_WiFiFWupdate_) ;
  Timer_Counter_Clear(0);
  //  
  DataLength = (WIFI_FWEndAddr - WIFI_FWStartAddr)/2 ;
  //
  while(Loop == 1)
      {
      //------------------------------------------------------------------------
      //20110330
      UsbFunction_USB(0x500) ;
      //------------------------------------------------------------------------
      switch(WorkStep)
          {
          case  0     :
                      // Check File Exist
                      if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
                          {
                          file = f_open(WIFI_FWFilename,"r"); 
                          if( !file )
                              {//==>开档失败
                              f_close(file) ;
                              //          123456789012345678
                              Led_String(" File Read Error  ",0,0);
                              WorkStep = 19 ;
                              //
                              }
                          else
                              {
                              //          123456789012345678
                              Led_String("Check WiFi Module ",0,0);
                              WorkStep = 1 ;
                              RtyTime = 0 ;
                              WaitStatus = 0 ;
                              }
                          }
                      else
                          {
                          RtyTime += 1 ;
                          if( RtyTime >= 10 )
                              {
                              //          123456789012345678
                              Led_String("  No USB Plug-in  ",0,0);
                              WorkStep = 19 ;
                              //
                              }
                          }
                      break ;              
          case  1     :
                      //
                      if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                          {
                          if( WaitStatus == 0 )
                              {
                              WiFiProgramStatus = 0xFF ;
                              WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                              WaitStatus = 1 ;
                              break ;
                              }
                          //
                          if( WiFiProgramStatus != 0x01 )
                              {
                              if( WiFiStatus->b.RxTimeoutOccur == 0 && WiFiStatus->b.RxCrcOccur == 0 )
                                  {
                                  RtyTime = 0 ;
                                  WorkStep = 2 ;
                                  }
                              else
                                  {
                                  if( WiFiProgramStatus != 0 )
                                      {
                                      RtyTime += 1 ;
                                      if( RtyTime > 1 )
                                          {
                                          f_close(file) ;
                                          //          123456789012345678
                                          Led_String(" Update Error 1-1 ",0,0);
                                          WorkStep = 19 ;
                                          //
                                          }
                                      else
                                          {
                                          WiFiProgramStatus = 0xFF ;
                                          WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                                          }
                                      }
                                  else
                                      {
                                      RtyTime = 0 ;
                                      WorkStep = 2 ;
                                      }
                                  }
                              }
                          else
                              {
                              WorkStep = 4 ;
                              }
                          }
                      break ;
          case  2     :
                      // Set Enter Program Mode
                      if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                          {
                          WiFi_SetCommWorkMode(_WiFiFWupdate_) ;
                          WiFi_SendEntryProgrammingMode(&WiFiProgramStatus) ;
                          WorkStep = 3 ;
                          WaitStatus = 0 ;
                          //
                          Timer_Counter_Clear(0);
                          while(!Timer_Counter(T_STEP,0,20)) ;
                          //
                          }
                      break;          
          case  3     :
                      // Wait Into ProgrammingMode
                      if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                          {
                          if( WaitStatus == 0 )
                              {
                              WiFiProgramStatus = 0 ;
                              WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                              WaitStatus = 1 ;
                              break ;
                              }
                          //
                          if( WiFiProgramStatus != 0x01 )
                              {
                              if( WiFiStatus->b.RxTimeoutOccur == 0 && WiFiStatus->b.RxCrcOccur == 0 )
                                  {
                                  f_close(file) ;
                                  //          123456789012345678
                                  Led_String(" Update Error 3-2 ",0,0);
                                  WorkStep = 19 ;
                                  }
                              else
                                  {
                                  RtyTime += 1 ;
                                  if( RtyTime > 1 )
                                      {
                                      f_close(file) ;
                                      //          123456789012345678
                                      Led_String(" Update Error 3-3 ",0,0);
                                      WorkStep = 19 ;
                                      }
                                  else
                                      {
                                      WiFiProgramStatus = 0 ;
                                      WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                                      }
                                  }
                              }
                          else
                              {
                              WorkStep = 4 ;
                              RtyTime = 0 ;
                              }
                          }
                      break ;  
          case  4     :
                      // Earse Flash
                      if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                          {
                          WiFiProgramStatus = 0 ;
                          WiFi_SendProgrammingEraseMode() ;
                          WorkStep = 5 ;
                          WaitStatus = 0 ;
                          //          123456789012345678
                          Led_String("    Erase Memory  ",0,0);
                          Timer_Counter_Clear(0);
                          while(!Timer_Counter(T_STEP,0,20)) ;
                          }
                      break ;
          case  5     :
                      // Wait Into ProgrammingMode
                      if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                          {
                          if( WaitStatus == 0 )
                              {
                              
                              if( WiFiStatus->b.RxTimeoutOccur == 1 || WiFiStatus->b.RxCrcOccur == 1 )
                                  {
                                  f_close(file) ;
                                  //          123456789012345678
                                  Led_String(" Update Error 5-1 ",0,0);
                                  WorkStep = 19 ;
                                  }
                              else
                                  {
                              WiFiProgramStatus = 0 ;
                              WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                              WaitStatus = 1 ;
                                  }
                              break ;
                              }
                          //
                          if( WiFiProgramStatus != 0x01 )
                              {
                              if( WiFiStatus->b.RxTimeoutOccur == 0 && WiFiStatus->b.RxCrcOccur == 0 )
                                  {
                                  f_close(file) ;
                                  //          123456789012345678
                                  Led_String(" Update Error 5-2 ",0,0);
                                  WorkStep = 19 ;
                                  //
                                  }
                              else
                                  {
                                  RtyTime += 1 ;
                                  if( RtyTime > 1 )
                                      {
                                      f_close(file) ;
                                      //          123456789012345678
                                      Led_String(" Update Error 5-3 ",0,0);
                                      WorkStep = 19 ;
                                      //
                                      }
                                  else
                                      {
                                      WiFiProgramStatus = 0 ;
                                      WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                                      }
                                  }
                              }
                          else
                              {
                              WorkStep = 6 ;
                              RtyTime = 0 ;
                              }
                          }  
                      break ;
          case  6     :
                      if( stm32uh_hub_connected() == 0 )
                          {
                          //          123456789012345678
                          Led_String(" Update Error 6-1 ",0,0);
                          WorkStep = 19 ;
                          //
                          }
                      
                      if(!f_seek(file,WIFI_FWBinStartAddress,FN_SEEK_SET))
                          {
                          FileAddress = WIFI_FWStartAddr ;
                          ProgramMode = 0 ;
                          WorkStep = 7 ;
                          }
                      else
                          {
                          f_close(file) ;
                          //          123456789012345678
                          Led_String(" Update Error 6-2 ",0,0);
                          WorkStep = 19 ;
                          //
                          }
                      break ;
          case  7     :
                      switch( ProgramMode )
                          {
                          case  1 :  
                                  // Check Program Status
                                  if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                                      {
                                      if( WaitStatus == 0 )
                                          {
                                          if( WiFiStatus->b.RxTimeoutOccur == 1 || WiFiStatus->b.RxCrcOccur == 1 )
                                              {
                                              f_close(file) ;
                                              //          123456789012345678
                                              Led_String("Update Error 7-2-1",0,0);
                                              WorkStep = 19 ;
                                              }
                                          else
                                              {
                                              WiFiProgramStatus = 0 ;
                                              WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                                              WaitStatus = 1 ;
                                              }
                                          break ;
                                          }
                                      //
                                      if( WiFiProgramStatus != 0x01 )
                                          {
                                          if( WiFiStatus->b.RxTimeoutOccur == 0 && WiFiStatus->b.RxCrcOccur == 0 )
                                              {
                                              f_close(file) ;
                                              //          123456789012345678
                                              Led_String("Update Error 7-2-2",0,0);
                                              WorkStep = 19 ;
                                              }
                                          else
                                              {
                                              RtyTime += 1 ;
                                              if( RtyTime > 1 )
                                                  {
                                                  f_close(file) ;
                                                  //          123456789012345678
                                                  Led_String("Update Error 7-2-3",0,0);
                                                  WorkStep = 19 ;
                                                  }
                                              else
                                                  {
                                                  WiFiProgramStatus = 0 ;
                                                  WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                                                  }
                                              }
                                          }
                                      else
                                          {
                                          RtyTime = 0 ;
                                          ProgramMode = 4 ;
                                          }
                                      }  
                                  break ;
                          case  2 :
                                  // Send Data
                                  if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                                      {
                                      WiFi_SendProgrammingData((unsigned char*)&USB_ReadBuffer[0],DataOffset) ;
                                      ProgramMode = 3 ;
                                      RtyTime = 0 ;
                                      WaitStatus = 0 ;
                                      }
                                  break ;
                          case  3 :
                                  // Check Program Status
                                  if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                                      {
                                      //
                                      if( WaitStatus == 0 )
                                          {
                                          if( WiFiStatus->b.RxTimeoutOccur == 1 || WiFiStatus->b.RxCrcOccur == 1 )
                                              {
                                              f_close(file) ;
                                              //          123456789012345678
                                              Led_String("Update Error 7-3-1",0,0);
                                              WorkStep = 19 ;
                                              }
                                          else
                                              {
                                              WiFiProgramStatus = 0 ;
                                              WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                                              WaitStatus = 1 ;
                                              }
                                          break ;
                                          }
                                      //
                                      if( WiFiProgramStatus != 0x01 )
                                          {
                                          if( WiFiStatus->b.RxTimeoutOccur == 0 && WiFiStatus->b.RxCrcOccur == 0 )
                                              {
                                              f_close(file) ;
                                              //          123456789012345678
                                              Led_String("Update Error 7-3-2",0,0);
                                              WorkStep = 19 ;
                                              }
                                          else
                                              {
                                              RtyTime += 1 ;
                                              if( RtyTime > 1 )
                                                  {
                                                  f_close(file) ;
                                                  //          123456789012345678
                                                  Led_String("Update Error 7-3-3",0,0);
                                                  WorkStep = 19 ;
                                                  }
                                              else
                                                  {
                                                  WiFiProgramStatus = 0 ;
                                                  WiFi_ReadProgrammingStatus(&WiFiProgramStatus) ;
                                                  }
                                              }
                                          }
                                      else
                                          {
                                          ProgramMode = 4 ;
                                          }
                                      }  
                                  break ;
                          case  4 :
                                  //123456789012345678
                                  // Debug
                                  //sprintf((char*)Temp,"%6x %4d %6x",DataLength,DataOffset,FileAddress);
                                  //                   123456789012345678
                                  sprintf((char*)Temp,"The progress-%3d%c-",(FileAddress*100/WIFI_FWEndAddr),'%');
                                  //
                                  Led_String(Temp,0,0);
                                  //
                                  //---------------------------------------------------------------------------
                                  if( DataLength != 0 )
                                      {
                                      ProgramMode = 2 ;
                                      //
                                      DataAddr = 0 ;
                                      DataOffset = 126 ;
                                      if( DataLength < 42 )
                                          {
                                          ReadCounter = DataLength ;
                                          DataOffset = DataLength * 3 ;
                                          FileAddress += (DataLength*2) ;
                                          DataLength = 0 ;
                                          }
                                      else
                                          {
                                          ReadCounter = 42 ;
                                          DataLength -= 42 ;
                                          FileAddress += (42*2) ;
                                          }
                                      // Read Data to Buffer
                                      while( ReadCounter != 0 )
                                          {
                                          if( stm32uh_hub_connected() == 0 )
                                              {
                                              //          123456789012345678
                                              Led_String("Update Error 7-0-0",0,0);
                                              WorkStep = 19 ;
                                              break ;
                                              //
                                              }
                                          
                                          filesize = f_read(&USB_ReadBuffer[DataAddr],1,4,file); 
                                          if( filesize == 4 )
                                              {
                                              DataAddr += 3 ;
                                              ReadCounter -= 1 ;
                                              }
                                          else
                                              {
                                              f_close(file) ;
                                              //          123456789012345678
                                              Led_String("  FW File Error   ",0,0);
                                              WorkStep = 19 ;
                                              break ;
                                              // 
                                              }
                                          } 
                                      //
                                      }
                                  else
                                      {
                                      WorkStep = 8 ;
                                      Timer_Counter_Clear(0);
                                      while(!Timer_Counter(T_STEP,0,10)) ;
                                      }
                                  break ;
                          case  0 :
                          default :
                                  // Set Address
                                  if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                                      {
                                      WiFi_SendProgrammingAddress((unsigned char*)&FileAddress) ;
                                      ProgramMode = 1 ;
                                      RtyTime = 0 ;
                                      WaitStatus = 0 ;
                                      }
                                  break ;                                      
                          }
                      break ;
          case  8     :
                      if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                          {
                          WiFi_SendLeaveProgrammingMode() ;
                          WorkStep = 9 ;
                          // wait Wifi Reset
                          Timer_Counter_Clear(0);
                          while(!Timer_Counter(T_STEP,0,20)) ;
                          //
                          }
                      break ;
          case  9     :
                      if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                          {
                          // End OK
                          RetStatus = 1 ;
                          WorkStep = 20 ;
                          //          123456789012345678
                          Led_String("   FW Update OK   ",0,0);
                          //
                          }
                      break ;
          case  10    :
                      if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                          {
                          WiFi_SendEraseAllFlash() ;
                          WorkStep = 11 ;
                          }
                      break ;
          case  11    :
                      if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
                          {
                          WorkStep = 20 ;
                          }
                      break ;
          case  19    :
                      // End Fail
                      RetStatus = 0 ;
          case  20    :        
                      WiFi_SetCommWorkMode(_WiFiGetStatus_) ;
                      Timer_Counter_Clear(0);
                      while(!Timer_Counter(T_STEP,0,10)) ;
                      //          123456789012345678
                      Led_String("                  ",0,0);
                      Timer_Counter_Clear(0);
                      while(!Timer_Counter(T_STEP,0,10)) ;
                      Loop = 0 ;
                      //
                      break ;                
          default     :
                      WorkStep = 0 ;
                      break ;
          }
      }
  //
  return RetStatus ;
}
*/
/*
// 20111213
//
extern u32 by_EE_ACCUMULATED_TIME; //= EEPROM_Read(EE_ACCUMULATED_TIME)
//extern u32 Bike_RPM_Accumulated,Bike_HR_Accumulated,Bike_Watts_Accumulated,Bike_METs_Accumulated ;
//extern u16 Bike_HRTimeCounter; //==>心跳时间计数器
//extern u16 Bike_InfoTimeCounter; //==>信息时间计数器
//extern u32 Bike_Speed_Accumulated;

//
extern const unsigned char WebWorkoutPacket[22];
extern const unsigned char WebErrorPacket[14] ;
extern const unsigned char WebServicePacket[14] ;
extern unsigned char const *WebSendDataPacketPtr ;
//
void UsbFunction_SaveWorkoutDataToWiFiAM(unsigned char Mode) // Modify by kunlung 20120214
{
  u8 TempDistanceUnit = 0 ;
  u32 _WiFi_AccDistance = EEPROM_ReadInformation(EE_ACCUMULATED_DISTANCE);// 0.001mile
  u32 _WiFi_AccTime = EEPROM_ReadInformation(EE_ACCUMULATED_TIME);// sec
  
  WiFiControlStatus *WiFiStatus ;
  WebFileDataType *WebFileAddr ;
  //
  Digital_Command(CmdGetVersion,0);
  //
  WiFiStatus = WiFi_GetWiFiControlStatus() ;
  WebFileAddr = WebFile_GetDataAddress() ;
  // 1 = mile
  // 2 = 0.1 mile
  // 3 = 0.01 mile
  // 4 = 0.001 mile
  // 33 = Km
  // 34 = 0.1 Km
  // 35 = 0.01 Km
  // 36 = Meter 
  if( EEPROM_ReadInformation(EE_UNIT) == C_KM )
  {// KM
      TempDistanceUnit = 32;
      _WiFi_AccDistance = (u32)((float)_WiFi_AccDistance * 1.609);
  }
  if( _WiFi_AccDistance <= 99999 )
      {
      WebFileAddr->distance = _WiFi_AccDistance;
      WebFileAddr->distance_unit = 0x04 + TempDistanceUnit;//0.001英哩
      }
  else
      {
      if( (_WiFi_AccDistance / 10 ) <= 99999 )
          {
          WebFileAddr->distance = _WiFi_AccDistance / 10;
          WebFileAddr->distance_unit = 0x03 + TempDistanceUnit;//0.01英哩
          }
      else
          {
          if( (_WiFi_AccDistance / 100) <= 99999 )
              {
              WebFileAddr->distance = _WiFi_AccDistance / 100;
              WebFileAddr->distance_unit = 0x02 + TempDistanceUnit;//0.1英哩
              }
          else
              {
              WebFileAddr->distance = _WiFi_AccDistance / 1000;
              WebFileAddr->distance_unit=0x01 + TempDistanceUnit;//1英哩
              }
          }
      }
  //
  WebFileAddr->Calories =  Console_Get_Calories();
  WebFileAddr->duration =  _WiFi_AccTime  ;// sec =>/ 3600 hour
  WebFileAddr->avg_speed = Console_Get_AvgSpeed() ;
  WebFileAddr->avg_HRT = CSAFE_GetInformation(_Get_HRAvg);//CSAFE_AvgHR() ;
  //
  //0x2D CSAFE model (required)The model value is the CSAFE 'Model' value assigned to a manufacturer's model. This value should be between 0 and 255 
  WebFileAddr->model = 0x2D ;
  WebFileAddr->module_id = 0x2D;//==> CSAFE model
  //----------------------------------------------------------------------------      
  WebFile_SetSerialNumber(0,"CTM530",EEPROM_ReadInformation(EE_ConsoleVersion)-1,EEPROM_ReadInformation(EE_SerialNumber)) ;
  WebFile_SetSerialNumber(1,"FTM501",EEPROM_ReadInformation(EE_FrameVersion)-1,EEPROM_ReadInformation(EE_FrameSerialNumber)) ;
  //
  ReadHT1381(&WebFileAddr->Clock) ;
  WebFileAddr->Clock.Year = __CharBCD2DEC(WebFileAddr->Clock.Year) ;
  WebFileAddr->Clock.Month = __CharBCD2DEC(WebFileAddr->Clock.Month) ;
  WebFileAddr->Clock.Date = __CharBCD2DEC(WebFileAddr->Clock.Date) ;
  WebFileAddr->Clock.Hours = __CharBCD2DEC(WebFileAddr->Clock.Hours) ;
  WebFileAddr->Clock.Minutes = __CharBCD2DEC(WebFileAddr->Clock.Minutes) ;
  WebFileAddr->Clock.Seconds = __CharBCD2DEC(WebFileAddr->Clock.Seconds) ;
  //
  //---------------------------------------------------------------------------  
  WebFile_SetServiceCode("S",0) ;
  WebFile_SetVersion() ;
  //
  WebFileAddr->Error = Digtial_Get_ErrorCode() ;
  //
  WebFileAddr->motor_temp = Digital_GetMCB_Information(5) / 10;// IGBT temp
  WebFileAddr->motor_temp_unit = 0;
  WebFileAddr->bus_current = Digital_GetMCB_Information(2) / 10;
  WebFileAddr->bus_current_unit = 97;// Amperes
  WebFileAddr->bus_voltage = Digital_GetMCB_Information(4) / 10;
  WebFileAddr->bus_voltage_unit = 99;// Volts
    
  //
  if( Mode == 1 )
  {// 上传 Error code 用
      WebSendDataPacketPtr = &WebErrorPacket[0] ;
  }
  else 
  {// 上传信息用
      if( Mode == 2 )
          WebSendDataPacketPtr = &WebServicePacket[0] ;
      else 
          WebSendDataPacketPtr = &WebWorkoutPacket[0] ;
  }
  
  //----------------------------------------------------------------------------      
  WiFiStatus->b.SendAMtoWeb = 1 ;
  WiFi_SetCommWorkMode(_WiFiAMMode_) ;
  return ;
}
*/
/*
unsigned char UsbFunction_WiFiEraseFlash(void)
{
  WiFiControlStatus *WiFiStatus ;
  unsigned char Loop = 1 ;
  unsigned char Step = 0 ;

  WiFiStatus = WiFi_GetWiFiControlStatus() ;
  WiFi_SetCommWorkMode(_WiFiManualMode_) ;
  Timer_Counter_Clear(0);
  while(!Timer_Counter(T_STEP,0,5)) ;
  
  while(Loop==1)
      {
      if( Step == 0 )
          {
          if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) )
              {
              Timer_Counter_Clear(0);
              WiFi_SendEraseAllFlash();
              Step = 1  ;
              }
          else
              {
              if(Timer_Counter(T_STEP,0,10)) ;
                  {
                  Loop = 2 ;
                  }
              }
          }
      else
          {
          if( (WiFiStatus->b.Tx != 0) && (WiFiStatus->b.Rx != 0 ) )
              {
              if(Timer_Counter(T_STEP,0,10)) ;
                  {
                  Loop = 2 ;
                  }
              }
          else
              Loop = 0 ;
          }
      }
  
  WiFi_SetCommWorkMode(_WiFiGetStatus_) ;
  Timer_Counter_Clear(0);
  while(!Timer_Counter(T_STEP,0,5)) ;
  //
  if( Loop == 2 )
      {
      return 0 ;
      }
  return 1 ;
}
*/
/*
unsigned char UsbFunction_GetWiFiAPSetting(void)
{
  unsigned char RetValue = 0 ;
  long len ;
  F_FILE *file ; 
  WiFiControlStatus *WiFiStatus ;
  unsigned char TempData[200] ;
  WiFiStatus = WiFi_GetWiFiControlStatus() ;
  //
  WiFiStatus->b.GetSSID = 1 ;
  WiFiStatus->b.GetSecurityType = 1 ;
  WiFiStatus->b.GetSecurityKey = 1 ;
  Timer_Counter_Clear(0);
  while(!Timer_Counter(T_STEP,0,20)) 
      {
      if( WiFiStatus->b.GetSSID == 0 && WiFiStatus->b.GetSecurityType == 0 && WiFiStatus->b.GetSecurityKey == 0 )
          break ;
      }
  //
  if( WiFiStatus->b.GetSSID != 0 || WiFiStatus->b.GetSecurityType != 0 && WiFiStatus->b.GetSecurityKey != 0 ) 
      {
      return RetValue ;
      }
  //------------------------------------------------------------------------
  //20110330
  UsbFunction_USB(0x500) ;
  //------------------------------------------------------------------------
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
      file = f_open("APSetData.txt","w"); 
      
      if( file != 0 )
          {
          __memset(&TempData[0],0,sizeof(TempData)) ;
          sprintf((char*)TempData,"SSID=%s\r\n",WiFiModuleData.APSSID) ;
          len = __strlen((char const*)TempData) ;//==>计算数据长度
          if( stm32uh_hub_connected() != 0 )
              {
              f_write(&TempData[0],1,len,file);//==>写入USB   
              }
          //     
          __memset(&TempData[0],0,sizeof(TempData)) ;
          switch( WiFiModuleData.APSecurityType )
              {
              case WEP_40Bits       :
                                    sprintf((char*)TempData,"SecurityType=%s\r\n",SecurityType1) ;
                                    break ;
              case WEP_104Bits      :
                                    sprintf((char*)TempData,"SecurityType=%s\r\n",SecurityType2) ;
                                    break ;
              case WPA_KEY          :
                                    sprintf((char*)TempData,"SecurityType=%s\r\n",SecurityType3) ;
                                    break ;
              case WPA_PHRASE       :
                                    sprintf((char*)TempData,"SecurityType=%s\r\n",SecurityType4) ;
                                    break ;
              case WPA2_KEY         :
                                    sprintf((char*)TempData,"SecurityType=%s\r\n",SecurityType5) ;
                                    break ;
              case WPA2_PHRASE      :
                                    sprintf((char*)TempData,"SecurityType=%s\r\n",SecurityType6) ;
                                    break ;
              case WPA_AUTO_KEY     :
                                    sprintf((char*)TempData,"SecurityType=%s\r\n",SecurityType7) ;
                                    break ;
              case WPA_AUTO_PHRASE  :
                                    sprintf((char*)TempData,"SecurityType=%s\r\n",SecurityType8) ;
                                    break ;
              case 0                :
                                    sprintf((char*)TempData,"SecurityType=%s\r\n",SecurityType0) ;
                                    break ;
              default               :
                                    sprintf((char*)TempData,"SecurityType=%d\r\n",WiFiModuleData.APSSID) ;
                                    break ;
              }
          
          len = __strlen((char const*)TempData) ;//==>计算数据长度
          if( stm32uh_hub_connected() != 0 )
              {
              f_write(&TempData[0],1,len,file);//==>写入USB   
              }
          
          __memset(&TempData[0],0,sizeof(TempData)) ;
          if( WiFiModuleData.APSecurityKey[0] == 0 )
              {
              sprintf((char*)TempData,"Key=%c\r\n",'0') ;
              }
          else
              {
              sprintf((char*)TempData,"Key=%s\r\n",WiFiModuleData.APSecurityKey) ;
              }
          len = __strlen((char const*)TempData) ;//==>计算数据长度
          if( stm32uh_hub_connected() != 0 )
              {
              f_write(&TempData[0],1,len,file);//==>写入USB   
              }
          // MAC
          __memset(&TempData[0],0,sizeof(TempData)) ;
          sprintf((char*)TempData,"MAC=%02X%02X%02X%02X%02X%02X\r\n",WiFiModuleData.MAC[0],WiFiModuleData.MAC[1],WiFiModuleData.MAC[2],WiFiModuleData.MAC[3],WiFiModuleData.MAC[4],WiFiModuleData.MAC[5]);
          len = __strlen((char const*)TempData) ;//==>计算数据长度
          if( stm32uh_hub_connected() != 0 )
              {
              f_write(&TempData[0],1,len,file);//==>写入USB   
              }
          //
          RetValue = 1 ;
         }
      f_close(file) ;
      }

  return RetValue ;
}


void UsbFunction_SetWiFiReConnect(void)
{
  //
  WiFiProcessStatus.b.SetConnectAP = 1 ;
  //
  return ;
}
*/

// 
/* Mask by Kunlung 20120111
bool UsbFunction_SaveWorkoutDataToWiFi( unsigned char *USBSaveStatus )
{
  //unsigned char Step = 0 ;
  unsigned char mode = 0 ;
  unsigned char Loop = 1 ;
  unsigned short len ;
  //unsigned long DataAddr ;
  //unsigned short DataLength ;
  unsigned char WebFileTempData[6500] ;
  bool RetStatus = TRUE;
  F_FILE *file; 
  file = 0 ;
  
  //WiFi_SetCommWorkMode(_WiFiAMMode_) ;
  //WiFiControlStatus *WiFiStatus ;
  //WiFiStatus = WiFi_GetWiFiControlStatus() ;
  //WiFiStatus->b.SendDataToWiFiAM = 1 ;
  
  //
  //Timer_Counter_Clear(0);
  //while( !Timer_Counter(T_STEP,0,1)) ;
  
  //
  //
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
      while( Loop == 1 )
          {
          //------------------------------------------------------------------------
          //20110330
          UsbFunction_USB(0x500) ;
          //------------------------------------------------------------------------
          if( mode == 0 )
              {
     
              //if( Step < 6 )
              //    {
              //    __memset(&WebFileTempData[0],0,sizeof(WebFileTempData)) ;
              //    WebFile_CreateStream(Step,&WebFileTempData[0]) ;
              //    Step += 1 ;
              //    mode = 1 ;
              //    }
              //else
              //    {
              //    f_close(file) ;
              //    Loop = 0 ;
              //    }
    
              ////if( Step != 6 )
              //    {
              __memset(&WebFileTempData[0],0,sizeof(WebFileTempData)) ;
              WebFile_CreateStream(6,&WebFileTempData[0]) ;
              //Step = 6 ;
              mode = 1 ;
                  
                  //Timer_Counter_Clear(0);
                  //}
              //else
                  //{
                  //if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) && (WiFiStatus->b.SendDataToWiFiAM == 1))
                  //    {
                  //    WiFiStatus->b.SendAMtoWeb = 0 ;
                  //    WiFi_SetAM_DataToWeb() ;
                      //
                  //    Timer_Counter_Clear(0);
                  //    while( !Timer_Counter(T_STEP,0,10))
                  //        {
                  //        if( WiFiStatus->b.SendAMtoWeb == 1 )
                  //            {
                  //            RetStatus = FALSE ;
                  //            break ;
                  //            }
                  //        }
                  //    WiFiStatus->b.SendDataToWiFiAM = 0 ;
                  //    Loop = 0 ;
                  //    }
                  
                  //}
              }
          else
              {
              // Save to Wifi module            
              //
              if( *USBSaveStatus != 0xA5 )
                  {
                  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
                      {
                      if( file == 0 )
                          file = f_open("webfile.xml","w"); 
                      
                      if( file != 0 )
                          {
                          len = __strlen((char const*)WebFileTempData) ;//==>计算数据长度
                          if( stm32uh_hub_connected() != 0 )
                              {
                              f_write(&WebFileTempData[0],1,len,file);//==>写入USB   
                              f_close(file) ;
                              *USBSaveStatus = 0xA5 ;
                              Loop = 0 ;
                              }
                          }
                      }
                  else
                      {
                      Loop = 0 ;
                      RetStatus = FALSE ;
                      }
                  }
              else
                  {
                  Loop = 0 ;
                  RetStatus = FALSE ;
                  }
              //
              
              //if( WiFiStatus->b.WiFiHWReady == 1 )
              //    {
              //    len = __strlen((char const*)WebFileTempData) ;//==>计算数据长度
              //    DataAddr = 0 ;
              //    Timer_Counter_Clear(0);
              //    while( len != 0 && len < 6500 )
              //        {
              //        if( (WiFiStatus->b.Tx == 0) && (WiFiStatus->b.Rx == 0 ) && (WiFiStatus->b.SendDataToWiFiAM == 1))
              //            {
              //            WiFiStatus->b.SendDataToWiFiAM = 0 ;
              //            DataLength = 100 ;  
              //            if( len < 100 )
              //                {
              //                DataLength = len ;
              //                }
                          //
              //            WiFi_SendAM_DataToModule(DataAddr,DataLength,&WebFileTempData[DataAddr]) ;
              //            DataAddr += DataLength ;
              //            if( len >= DataLength )
              //                len -= DataLength ;
              //            else
              //                len = 0 ;
                          //
              //            Timer_Counter_Clear(0);
                          //
              //            }
              //        else
                          {
                          // Check Delay Timeout
              //            if( Timer_Counter(T_STEP,0,30))
              //                {
              //                Loop = 0 ;
              //                RetStatus = FALSE ;
              //                break ;
              //                }                  
              //            }
              //        }
                  //
              //    mode = 0 ;
                  //
              //    }    
              //else
              //    {
                  //if( Timer_Counter(T_STEP,0,10))
                  //    {
              //    Loop = 0 ;
              //    RetStatus = FALSE ;
                  //    }
              //    }
              
              }
          } 
      }
  //
  //Timer_Close(0) ;
  //WiFi_SetCommWorkMode(_WiFiGetStatus_) ;
  //
  return RetStatus ;
}
*/


//#endif



void __memset(void *s, char c, long n)
{       /* store c throughout unsigned char s[n] */
  const unsigned char uc = c;
  unsigned char *su = (unsigned char *)s;
  
  for (; 0 < n; ++su, --n)
  *su = uc;
  return ;
}



void __memcpy(void *s1, const void *s2, long n)
{       /* copy char s2[n] to s1[n] in any order */
  char *su1 = (char *)s1;
  const char *su2 = (const char *)s2;
  
  for(; 0 < n; ++su1, ++su2, --n)
      *su1 = *su2;
  return ;
}


char __memcmp(const void *s1, const void *s2, long n)
{       /* compare unsigned char s1[n], s2[n] */
  const unsigned char *su1 = (const unsigned char *)s1;
  const unsigned char *su2 = (const unsigned char *)s2;
  
  for (; 0 < n; ++su1, ++su2, --n)
    if (*su1 != *su2)
      return (*su1 < *su2 ? -1 : +1);
  return (0);
}



long __strlen(const char *s)
{       /* find length of s[] */
  const char *sc;
  
  for (sc = s; *sc != '\0'; ++sc)
    ;
  return (sc - s);
}

char *__strchr(const char *s, int c)
{       /* find first occurrence of c in char s[] */
  const char ch = c;

  for (; *s != ch; ++s)
    if (*s == '\0')
      return (0);
  return ((char *)s);
}

char *__strstr(const char *s1, const char *s2)
{       /* find first occurrence of s2[] in s1[] */
  if (*s2 == '\0')
    return ((char *)s1);
  for (; (s1 = __strchr(s1, *s2)) != 0; ++s1)
  {     /* match rest of prefix */
    const char *sc1, *sc2;

    for (sc1 = s1, sc2 = s2; ; )
      if (*++sc2 == '\0')
        return ((char *)s1);
      else if (*++sc1 != *sc2)
        break;
  }
  return (0);
}



char *__strcpy(char *s1, const char *s2)
{       /* copy char s2[] to s1[] */
  char *s = s1;
  
  for (s = s1; (*s++ = *s2++) != '\0'; )
    ;
  return (s1);
}

char *__strncpy(char *s1, const char *s2, long n)
{       /* copy char s2[max n] to s1[n] */
  char *s;
  
  for (s = s1; 0 < n && *s2 != '\0'; --n)
    *s++ = *s2++;       /* copy at most n chars from s2[] */
  for (; 0 < n; --n)
    *s++ = '\0';
  return (s1);
}

int __strcmp(const char *s1, const char *s2)
{       /* compare unsigned char s1[], s2[] */
  for (; *s1 == *s2; ++s1, ++s2)
    if (*s1 == '\0')
      return (0);
  return (*(unsigned char *)s1 < *(unsigned char *)s2 ? -1 : +1);
}


unsigned char __CharBCD2DEC(unsigned char s1)
{
  return(((s1 / 16) * 10)+(s1 % 16)) ;
}
      
void UsbFunction_ClearUSBFirstCheck(void)
{
  USB_FirstCheck = 0;
}
char *UsbFunction_BinFileAddress(char by_D,char by_D1)
{
  if(by_D == 2)
      sprintf(Update_Name,"/MATRIX/FW/LANGUAGE/%s.bin",Bin_Name[by_D1 - 1]);//==>整合檔名
  else
      sprintf(Update_Name,"/MATRIX/FW/PATTERN/%s.bin",Bin_Name[by_D1 - 1]);//==>整合檔名
  return Update_Name;
}
// 2014.01.29-2
// 清除先前所取得的Bin文件相关信息
void UsbFunction_ClearBinInfo(void)
{
  unsigned char i,j;
  
  for(j = 0;j < 10;j++)
  {
      for(i = 0;i < 40;i++)
      {
          Bin_Name[j][i] = 0;//TempFilename[i];
          Update_Ver[i] = 0;
      }
  }
  Total_BIN = 0;
}
//
/*******************************************************************************
* Function Name  : Console_DigitalError()
* Description    : 错误码
* Input          : by_D = Data_Set / Data_Get
* Output         : None
* Return         : None
*******************************************************************************/
struct{
  unsigned ERR_ON : 1;
  //unsigned ERR_OFF: 1;
}USB_Bit;
struct{
  unsigned char Status;
  unsigned char SignalCount;
  unsigned char ProtectionCount;
}USB_Data;

void UsbFunction_Protection(void)
{
  if(EEPROM_ReadInformation(EE_USB_Protection) == D_Enable)
  {
      switch(USB_Data.Status)
      {
          case 0:
                 if(IO_USB_ERR() == 0)
                 {// Have ERR
                     if(USB_Bit.ERR_ON == 0)
                     {
                         USB_Bit.ERR_ON=1;
                         USB_Data.Status=0;
                         USB_Data.SignalCount=0;
                         Timer_Counter_Clear(_Time_USBProtectionCheck);
                     }
                     if(Timer_Counter(T_STEP,_Time_USBProtectionCheck,1))
                     {
                         USB_Data.SignalCount++;
                         Timer_Counter_Clear(_Time_USBProtectionCheck);
                         if(USB_Data.SignalCount >= 5)
                         {// keep 500ms
                             USB_Data.Status=1;
                             // Cut the USB function
                             IO_USB_ResetEN();
                             //
                             USB_Data.ProtectionCount++;
                             USB_Data.SignalCount=0;
                         }
                     }
                 }
                 else if(USB_Data.ProtectionCount != 0)
                 {
                     if(USB_Bit.ERR_ON == 1)
                     {
                         USB_Bit.ERR_ON=0;
                         USB_Data.SignalCount=0;
                         Timer_Counter_Clear(_Time_USBProtectionCheck);
                     }
                     if(Timer_Counter(T_STEP,_Time_USBProtectionCheck,1))
                     {
                         USB_Data.SignalCount++;
                         Timer_Counter_Clear(_Time_USBProtectionCheck);
                         if(USB_Data.SignalCount >= 5)
                         {// keep 500ms
                             USB_Data.Status=2;
                             Timer_Counter_Clear(_Time_USBProtectionCheck);
                             USB_Data.SignalCount=0;
                         }
                     }
                 }
                 break;
          case 1: 
                 if(USB_Data.ProtectionCount > 3)
                 {
                     USB_Data.Status=5;
                     Digtial_Set_ErrorCode(0x0305);
                     Digital_ErrorCode_ChangToAddress();
                 }
                 else if(Timer_Counter(T_STEP,_Time_USBProtectionCheck,600))
                 {
                     USB_Data.Status=0;
                     USB_Data.SignalCount=0;
                     IO_USB_SetEN();
                 }
                 break;
          case 2:
                 if(IO_USB_ERR() == 0)
                 {// Have ERR
                     USB_Data.Status=0;
                     USB_Bit.ERR_ON=0;
                 }
                 else if(Timer_Counter(T_STEP,_Time_USBProtectionCheck,6000))
                 {
                     USB_Data.ProtectionCount=0;
                 }
                 break; 
      }
  }
  /*
  else
  {
      USB_Bit.ERR_ON=0;
      USB_Bit.ERR_OFF=0;
      USB_Data.Status=0;
      USB_Data.SignalCount=0;
      USB_Data.ProtectionCount=0;
      IO_USB_SetEN();
  } 
  */  
}
// start v2.1-13
// RSCU
void UsbFunction_RSCU_SoftwareUpdate(void)
{
    char empty[1] = {0};
    
    
    Boot_UCBFirmwareUpdate( &empty[0],0 ,1);
}
// end v2.1-13


