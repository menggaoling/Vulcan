//------------------------------------------------------------------------------
// Version : 2
// 语言代码     : 从 1 开始编， 0代表英文，需放在主程序中当默认
// 一句语言字数 : 200
// 字符串代码     : 从 0 开始编
// 
// Language_LanguageChecking() : 设定语言,
// Language_GetString() : 字符串读取。需将Lang_StringBuffer[]代入，字符串数据会放在此缓存器
//                        使用时需设定字符串类型(Name or Data)、字符串代号
// Lang_StringBuffer[] : 字符串缓存器
//------------------------------------------------------------------------------
// 图档里面的名称 : 最多50字
// 图文件数据(不含名称与大小) :　200行 最多 400 Byte
// 图档编号 : 由 1 开始, 0代表MATRIX图档，需放在主程序中当默认
// 
// Pattern_WritePatternToEEPRom() : 刻录图档。将图案文件名(路径)输入即可刻录。
// Pattern_CheckingPattern() : 图档设定。会将指定的的图案名称、数据读出，并计算出总图案数。在开机、选图、刻录时都需执行。
// Pattern_PatternInfo[] : 图文件资料，一行16 dot。
// Pattern_PatternName[] : 图档名称
// Pattern_PatternColumnAmount : 图文件资料总行数
// Pattern_PatternAmount : 图档总数
// Pattern_PatternMotion : 图案显示动作 ==> 0 图案停止   1 图案连续转动   2 图案先转动在停止 
//           
//------------------------------------------------------------------------------
//#include  "stm32f10x_conf.h"
//#include  "hcc_types.h"
//#include  "usb_host.h"
//#include  "pmgr.h"
#include  "fat.h"
#include  "EEPRom.h"
#include  "Mx25L1606E.h"
#include  "UsbFunction.h"
#include  "EEPRom_Language.h"

//----- The words of one string ------------------------------------------------
//一句语言最多 200个英文字
unsigned char Lang_StringBuffer[_TheWordsOfOneString_];

// 一个图档最多 200 行(2 bytes)
unsigned short Pattern_PatternInfo[_PatternColumn_];  // 400 bytes
unsigned char Pattern_PatternName[_PatternNameSize_+1];

unsigned long Lang_CheckUSBTime = 0;
unsigned char Lang_LanguageAmount = 0;
unsigned char Lang_StringAmount = 0;
unsigned short Lang_Version = 0;
unsigned long by_LanguageIndexTableStartAddr = 0;
unsigned long by_LanguageDataStartAddr = 0;
unsigned long LanguageNameStartAddr = 0;

unsigned char Lang_EEPROM_State = 0;  // Fail
unsigned short Lang_NowLanguage = 0;

unsigned char Lang_ReloadString = 0;
unsigned char Lang_OldStringNumber = 0xFE;
unsigned char Lang_OldStringType = 0xFE;

unsigned char Lang_CheckState = _Lang_Fail_;  // 20140127

//
unsigned short Pattern_PatternColumnAmount = 0; // 图文件资料行数
unsigned short Pattern_PatternAmount = 0; // 图档总数
unsigned char Pattern_PatternMotion = 0; // 图案显示动作
// 
F_FIND usbfind;
F_FILE *usbfile;


//------ File ----------------------------------------------------------------//
unsigned char Open_UsbFile(char *by_UsbFileName)
{
    if(!f_findfirst(by_UsbFileName, &usbfind))
    {
        usbfile = f_open(by_UsbFileName,"r"); 
        if( !usbfile )
        {
            return _Lang_OpenFile_Fail_;
        }
        else
        {
            if(f_seek(usbfile,0,FN_SEEK_SET))          
            {
               return _Lang_OpenFile_Fail_;
            }
        }
    }
    
    return _Lang_Success_;
}
void Close_UsbFile(void)
{
    f_close(usbfile);
}
unsigned char Read_UsbFile(unsigned long by_FileAddress, unsigned char *by_DataBuff, unsigned short by_ReadSize)
{
    if(!f_seek(usbfile,by_FileAddress,FN_SEEK_SET))
         f_read(by_DataBuff,by_ReadSize,1,usbfile);
    else
    {
        f_close(usbfile) ;
        return _Lang_OpenFile_Fail_;
    }
    
    return _Lang_Success_;
}


//------------------------------------------------------------------------------
// 1. 清除EEPROM 空间 
// 2. 以 Sector(4K) 为单位
//
//------------------------------------------------------------------------------
unsigned char Erase_EEPROM_Data(unsigned long by_Address, unsigned long by_TotalSize)
{
    unsigned long i = 0;
    
    // Erase
    for(i=0;i<(by_TotalSize/_SectorBase_);i++)
    {// Erase sector 2~ 26(100K bytes) //Address 8192 ~ 110591
        Mx25L1606E_SectorErase(i+(by_Address/_SectorBase_));
    }
    // Check
  /*  for( i = 0 ; i < by_TotalSize ; i++ )
    {
        if( 0xFF != Mx25L1606E_ReadByte((by_Address+i)) )
            return _Lang_Erase_Fail_ ;
    }
 */   
    return _Lang_Success_;
}


//------------------------------------------------------------------------------
// 1. 将语言文件写入EEPROM中
//
// *by_UcbMachineType : 机种字符串，同于语言文件里的机种字符串
// *by_FileName : 文件名(含路径)
//
// Return : Implemented state
//------------------------------------------------------------------------------
unsigned char Lang_WriteLanguageToEEPRom(unsigned char *by_UcbMachineType, char *by_FileName)
{
    unsigned char by_Return;
    
    by_Return = Open_UsbFile(by_FileName);
    
    if(by_Return == _Lang_Success_)
        by_Return = WriteLanguageToEEPRom(by_UcbMachineType);
    
    Close_UsbFile();
    return by_Return;
  //
}
unsigned char WriteLanguageToEEPRom(unsigned char *by_UcbMachineType)
{
  unsigned long i, j, k;
  unsigned long by_Lang_Addr = 0;
  unsigned char by_BinData;
  unsigned char by_LanguageNumer = 0, by_StringNumber = 0;
  unsigned char by_Lang_Checksum = 0;
//  unsigned short by_LanguageVersion = 0;
  unsigned char by_Data[256];
  unsigned char *by_WriteOKString = _LanguageWriteOK_;

  
  if(Read_UsbFile(by_Lang_Addr, by_Data, _StringName_) != _Lang_Success_)
      return _Lang_OpenFile_Fail_;
    
  // Check Start Code (1 byte)
  if(by_Data[_StartCode_] != _Lang_Start_Code_)
  {
      return _Lang_FileData_Error_;
  }
  // 读取语言数 (1 byte)
  by_LanguageNumer = by_Data[_LangAmount_];
  // 读取字符串数 (1 byte)
  by_StringNumber = by_Data[_StringAmount_];
  // 读取版本 (2 byte) 
  //by_LanguageVersion = (by_Data[_VersionHighByte_]<<8) + by_Data[_VersionLowByte_];
  //
  by_Lang_Addr += _StringName_;
  by_Lang_Checksum = 0;
  for(i=0;i<_StringName_;i++)
    by_Lang_Checksum += by_Data[i];

  // 检查机型 (比对字符串)
  while(1)
  {
      if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
          return _Lang_OpenFile_Fail_;
          
      by_Lang_Checksum += by_BinData;
      if(by_BinData == 0)
          break;
      else if(by_BinData != *(by_UcbMachineType++))
      {// 机种错误
          return _Lang_Machine_Error_;
      }    
  }
  // 读取语言名称(N byte)语索引起始地址(4 bytes)来计算Checksum
  for(i=0; i<by_LanguageNumer; i++)
  { 
      by_BinData = 1; // 填入非0数字，才可执行while()
      while(by_BinData != 0)
      {
          if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
              return _Lang_OpenFile_Fail_;
          
          by_Lang_Checksum += by_BinData;
          if(by_BinData == 0)
          {
              for(j=0; j<4; j++)
              {
                  if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
                      return _Lang_OpenFile_Fail_;     
                  by_Lang_Checksum += by_BinData;
              }
              by_BinData = 0;
          }
      } 
  }
  // 比对Checksum
  if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
      return _Lang_OpenFile_Fail_;
  
  if(by_Lang_Checksum != by_BinData)
  {// 档案错误
      return _Lang_FileData_Error_;
  }
  //----- 第一个区块检查结束 ---------------------------------------------------
  //--- 检察每一个语言的索引表 -------------------------------------------------
  for(i=0; i<by_LanguageNumer; i++)
  {
      by_Lang_Checksum = 0;
      for(j=0; j<(by_StringNumber*4); j++)
      {
          if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
              return _Lang_OpenFile_Fail_;
          
          by_Lang_Checksum += by_BinData;
      }
      if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
          return _Lang_OpenFile_Fail_;
      
      if(by_Lang_Checksum != by_BinData)
      {// 档案错误
          return _Lang_FileData_Error_;
      }    
  }
  //--- 索引表检查结束 -------------------------------------------------------
  //--- 检查语言数据 ---------------------------------------------------------
  for(i=0; i<by_LanguageNumer; i++)
  {
      by_Lang_Checksum = 0;
      for(j=0; j<by_StringNumber; j++)
      {
          by_BinData = 1; // 填入非0数字，才可执行while()
          while(by_BinData != 0)
          {
              if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
                  return _Lang_OpenFile_Fail_;
              by_Lang_Checksum += by_BinData;
          }
      }
      //
      if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
          return _Lang_OpenFile_Fail_;
      if(by_Lang_Checksum != by_BinData)
      {// 档案错误
          return _Lang_FileData_Error_;
      } 
  }
  //--- 比对结束码 -------------------------------------------------------------
  if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
      return _Lang_OpenFile_Fail_;
  
  if(by_BinData != _Lang_End_Code_)
  {// 档案错误
      return _Lang_FileData_Error_;
  }
  if(by_Lang_Addr > _Language_Size_)
  {// 档案过大
      return _Lang_FileOverSize_;
  }
  //---- Bin档写入EEPROM ------------------------------------------------------------
  if(Erase_EEPROM_Data(_Language_Block_, _Language_Size_) != _Lang_Success_)
  {
      return _Lang_Erase_Fail_;
  }
  
  k=0;
  while(k<by_Lang_Addr)
  {
      if((by_Lang_Addr - k) <= 256)
      {
        
          if(Read_UsbFile(k, by_Data, (by_Lang_Addr - k)) != _Lang_Success_)
              return _Lang_WriteRRPRom_Error_;
          
          Mx25L1606E_WriteBlock((_Language_Data_Address_+k),(by_Lang_Addr - k) ,by_Data) ;
          break;
      }
      else
      {
          if(Read_UsbFile(k, by_Data, 256) != _Lang_Success_)
            return _Lang_WriteRRPRom_Error_;
  
          Mx25L1606E_WriteBlock((_Language_Data_Address_+k),256 ,by_Data) ;
          k += 256;
      }
  }
  //---- 写入确认 --------------------------------------------------------------
  for(k=0; k<by_Lang_Addr; k++)
  {
      if(Read_UsbFile(k, &by_BinData, 1) != _Lang_Success_)
          return _Lang_WriteRRPRom_Error_;

      if(by_BinData != Mx25L1606E_ReadByte((_Language_Data_Address_+k)))
      {
          return _Lang_WriteRRPRom_Error_;
      }
  }
  
  //---- 在EEPROM里写入确认字符串，以便开机时检查是否有Language ------------------
  Mx25L1606E_WriteBlock(_Language_Block_, (_CheckStringSize_-1), by_WriteOKString) ;
  Mx25L1606E_WriteByte((_Language_Block_+_CheckStringSize_-1), 0);
  
  for(k=0;k<(_CheckStringSize_-1);k++)
  {
      if(*(by_WriteOKString+k) != Mx25L1606E_ReadByte((_Language_Block_+k)))
        return _Lang_WriteRRPRom_Error_;
  }
  return _Lang_Success_;
}

void Language_ClearBasicInfo(void)
{
    Lang_LanguageAmount = 0;
    Lang_StringAmount = 0;
    Lang_Version = 0;
}

//------------------------------------------------------------------------------
// 1. 确认是否有Language在EEPROM
// 2. 比对输入的机种字符串与EEPROM的机种字符串是否一样 
//
// *by_UCBTypeString : 机种字符串，同于语言文件里的机种字符串
// Return : Implemented state
//------------------------------------------------------------------------------
unsigned char Language_EEPROMCheck(unsigned char *by_UCBTypeString)
{// 
    unsigned char by_CheckData[_CheckStringSize_];
    unsigned char i = 0;
    unsigned char *by_WriteOKString = _LanguageWriteOK_;
    
    Lang_CheckState = _Lang_Success_;  // 20140127
    //
    Mx25L1606E_ReadBlock( _Language_Block_,_CheckStringSize_, by_CheckData );
    // 
    for(i=0; i<(_CheckStringSize_-1); i++)
    {
        if(by_CheckData[i] != *(by_WriteOKString+i))
        {
            Lang_CheckState = _Lang_EEPROMData_Error_; // 20140127
            break;
            //return _Lang_EEPROMData_Error_;
        }
    }    
    //
    if(Lang_CheckState == _Lang_Success_)
    {// 比对机种字符串
        i = 0;
        while(1)
        {
            by_CheckData[0] = Mx25L1606E_ReadByte((_Language_Data_Address_ + _StringName_ + i));
            if(*(by_UCBTypeString + i) != by_CheckData[0])
            {
                Lang_CheckState = _Lang_Machine_Error_; // 20140127
                break;
                //return _Lang_Machine_Error_; 
            }
            else if(by_CheckData[0] == 0)
                break;
            i++;
        }
    }
    
    return Lang_CheckState; // 20140127
} 

//------------------------------------------------------------------------------
// 1. 检查语言数据正确性
// 2. 设定输入语言的相关数据与地址
// 
// by_NowLanguage : 语言代码( 1 ~ ), 0代表英文
// Return : Implemented state
//------------------------------------------------------------------------------
unsigned char Language_LanguageChecking(unsigned char by_NowLanguage)
{
    unsigned long by_Count = _Language_Data_Address_;
    unsigned long i = 0, j = 0;
    unsigned char by_Dat, by_Lang_Checksum = 0;
    
    if(Lang_CheckState != _Lang_Success_) // 20140127
    {
        Language_ClearBasicInfo();
        Lang_EEPROM_State = Lang_CheckState;
        return Lang_EEPROM_State;
    }

    Lang_NowLanguage = by_NowLanguage;
    Lang_ReloadString = 1;    
    Lang_EEPROM_State = _Lang_Success_;
       
    if(Mx25L1606E_ReadByte(by_Count) != _Lang_Start_Code_)
    {// Check StartCode // EEPROM 数据错误
        Language_ClearBasicInfo();
        
        Lang_EEPROM_State = _Lang_EEPROMData_Error_;
        return Lang_EEPROM_State;
    }
    else
    {// Check language name and String index start address
          by_Lang_Checksum = _Lang_Start_Code_; // 0
          by_Count = _Language_Data_Address_ + 1;
          Lang_LanguageAmount = Mx25L1606E_ReadByte(by_Count++); // 1
          Lang_StringAmount = Mx25L1606E_ReadByte(by_Count++); // 2
          by_Lang_Checksum = by_Lang_Checksum + Lang_LanguageAmount + Lang_StringAmount;
          //
          Lang_Version = 0;
          by_Dat = Mx25L1606E_ReadByte(by_Count++);
          by_Lang_Checksum += by_Dat;
          Lang_Version = (Lang_Version<<8) +by_Dat; 
          by_Dat = Mx25L1606E_ReadByte(by_Count++);
          by_Lang_Checksum += by_Dat;
          Lang_Version = (Lang_Version<<8) +by_Dat; 
          
          //
          while(1)
          {// 机型字符串
              by_Dat = Mx25L1606E_ReadByte(by_Count++);
              by_Lang_Checksum += by_Dat;
              if(by_Dat == 0)
              {
                  break;
              }
          }
          
          // 语言名称字符串
          LanguageNameStartAddr = by_Count;
          by_LanguageIndexTableStartAddr = 0;
          for(i=0; i<Lang_LanguageAmount; i++)
          {
              by_Dat = 1;
              while(by_Dat != 0)
              {
                  by_Dat = Mx25L1606E_ReadByte(by_Count++);
                  by_Lang_Checksum += by_Dat;
              }
              for(j=0; j<4; j++)
              {// Language Index Table Start Addr
                  by_Dat = Mx25L1606E_ReadByte(by_Count++);
                  by_Lang_Checksum += by_Dat;
                  
                  if(Lang_NowLanguage != 0 && i == (Lang_NowLanguage - 1))
                  {
                      by_LanguageIndexTableStartAddr = (by_LanguageIndexTableStartAddr<<8) + by_Dat;
                  }
              }             
          }
          by_Dat = Mx25L1606E_ReadByte(by_Count++);
          if(by_Lang_Checksum != by_Dat)
          {//EEPROM 数据错误
              Language_ClearBasicInfo();
              
              Lang_EEPROM_State = _Lang_EEPRomData_Error_;
              return Lang_EEPROM_State;
          }
    }
    //--------------------------------------------------------------------------
    if(Lang_NowLanguage == 0)
    {// 语言代号为0(English), 不检查index table and Data
        return _Lang_Success_;
    }
    // Check the language index table 
    by_LanguageIndexTableStartAddr += _Language_Data_Address_;
    by_Lang_Checksum = 0;
    by_Count = by_LanguageIndexTableStartAddr;
    by_LanguageDataStartAddr = 0;
    for(i=0; i<(Lang_StringAmount*4) ; i++)
    {
        by_Dat = Mx25L1606E_ReadByte(by_Count++);
        by_Lang_Checksum += by_Dat;
        
        if(i<4)
        {
            if(i == 0 && by_Dat != Lang_NowLanguage)
            {// 语言错误
                Lang_EEPROM_State = _Lang_Language_Error_;
                return Lang_EEPROM_State;
            }
            else
                by_LanguageDataStartAddr = (by_LanguageDataStartAddr<<8) + by_Dat;
        }
    }
    by_Dat = Mx25L1606E_ReadByte(by_Count++);
    if(by_Lang_Checksum != by_Dat)
    {//EEPROM 数据错误
        Lang_EEPROM_State = _Lang_Language_Error_;
        return Lang_EEPROM_State;
    }
    by_LanguageDataStartAddr = (by_LanguageDataStartAddr&0x00FFFFFF) + _Language_Data_Address_;
    //--------------------------------------------------------------------------
    // Check the language data 
    by_Lang_Checksum = 0;
    by_Count = by_LanguageDataStartAddr;
    for(i=0; i<Lang_StringAmount; i++)
    {
        by_Dat = 1;
        while(by_Dat != 0x00)
        {
            by_Dat = Mx25L1606E_ReadByte(by_Count++);
            by_Lang_Checksum += by_Dat;
        }
    }
    by_Dat = Mx25L1606E_ReadByte(by_Count++);
    if(by_Lang_Checksum != by_Dat)
    {//EEPROM 数据错误
        Lang_EEPROM_State = _Lang_Language_Error_;
        return Lang_EEPROM_State;
    }  
    
    Lang_EEPROM_State = Language_GetString(_Lang_Language_Name_,Lang_NowLanguage);
    return Lang_EEPROM_State;
}
//------------------------------------------------------------------------------
// 1. 从EEPROM里读取字符串，字符串放在 Lang_StringBuffer[] 此矩阵中
//
// Lang_StringType     : 字符串型态 ==> 语言名称 (_Lang_Language_Name_)
//                                ==> 各国语言字符串数据 (_Lang_Language_Data_)
// Lang_StringBuffer[]  : 字符串buffer
// Lang_StringNumber   : 字符串编号(代码)
//                       假如 Lang_StringType 是语言名称(1)，则此变量代表语言代码 ==> 由 1 开始
//                       假如 Lang_StringType 是各国语言字符串数据(2)，则此变量代表字符串编号 ==> 由 0 开始
// Return : Implemented state
//------------------------------------------------------------------------------
unsigned char Language_GetString(unsigned char Lang_StringType, unsigned char Lang_StringNumber)
{
    unsigned long by_StringDataStartAddr = 0;
    unsigned long by_StringIndexTableAddr = 0;
    unsigned char by_Dat;
    unsigned char i;
    unsigned short by_WordCount = 0;
    unsigned char *Lang_String;
    static unsigned char StringState = _Lang_Success_;
    
    if(Lang_CheckState != _Lang_Success_)// 20140127
    {
        return Lang_CheckState;
    }
    if(Lang_EEPROM_State != _Lang_Success_) 
    {
        return Lang_EEPROM_State;
        //return _Lang_EEPRomData_Error_;
    }
    
    if(Lang_NowLanguage < 1 || Lang_NowLanguage > Lang_LanguageAmount)
    {// 非EEPROM里储存的语言  20130128 
        Lang_ReloadString = 0;
        
        Lang_EEPROM_State = _Lang_Fail_;
        return Lang_EEPROM_State;
    }
    
    if(Lang_ReloadString == 1)
    {
        Lang_ReloadString = 0;
        Lang_OldStringNumber = Lang_StringNumber;
        Lang_OldStringType = Lang_StringType;
    }
    else if( Lang_StringNumber != Lang_OldStringNumber || Lang_StringType != Lang_OldStringType)
    {
        Lang_OldStringNumber = Lang_StringNumber;
        Lang_OldStringType = Lang_StringType; 
    }
    else
    {
        return StringState;
        //return _Lang_Success_;
    }
    
    Lang_String = Lang_StringBuffer;
    
    if(Lang_StringType == _Lang_Language_Name_)
    {
        if(Lang_StringNumber == 0 || Lang_StringNumber > Lang_LanguageAmount)
        {// Data Address Error // 编号错误
            StringState = _Lang_StringNumber_Error_;
            return StringState;
            //return _Lang_StringNumber_Error_;
        }
        
        by_StringDataStartAddr = LanguageNameStartAddr;  
        for(i=0; i<Lang_StringNumber; i++)
        {    
            by_Dat = 1;
            by_WordCount = 0;
            while(by_Dat != 0)
            {
                by_Dat = Mx25L1606E_ReadByte(by_StringDataStartAddr++);
                if(i == (Lang_StringNumber - 1))
                    *(Lang_String++) = by_Dat;
                
                by_WordCount += 1;
                if(by_WordCount >= _TheWordsOfOneString_)
                {// 字符串过长
                    StringState = _Lang_OverString_;
                    return StringState; 
                    //return _Lang_OverString_;
                }  
            }
            by_StringDataStartAddr += 4; 
                   
            if(i == (Lang_StringNumber - 1))       
                *Lang_String = 0;   
        }
    }
    else
    {
        by_StringIndexTableAddr = by_LanguageIndexTableStartAddr + Lang_StringNumber*4;
        if(Lang_StringNumber > (Lang_StringAmount - 1))
        {// Data Address Error // 编号错误
            StringState = _Lang_StringNumber_Error_;
            return StringState;
            //return _Lang_StringNumber_Error_;
        }
        //Index Table里的数据每4 bytes为一组，第1个byte语为语言编号,其他三个代表字符串数据的起始地址
        by_Dat = Mx25L1606E_ReadByte(by_StringIndexTableAddr++);
        if(by_Dat != Lang_NowLanguage) 
        {// Data Address Error // 语言错误
            StringState = _Lang_EEPRomData_Error_;
            return StringState; 
            //return _Lang_EEPRomData_Error_;
        }
        for(i=0; i< 3; i++)
        {
            by_Dat = Mx25L1606E_ReadByte(by_StringIndexTableAddr++);
            by_StringDataStartAddr = (by_StringDataStartAddr<<8) + by_Dat;
        }
        by_StringDataStartAddr += _Language_Data_Address_;   
        //
        by_Dat = 1;
        by_WordCount = 0;
        while(by_Dat != 0)
        {
            by_Dat = Mx25L1606E_ReadByte(by_StringDataStartAddr++);
            *(Lang_String++) = by_Dat;
            by_WordCount += 1;
            if(by_WordCount >= _TheWordsOfOneString_)
            {// 字符串过长
                StringState = _Lang_OverString_;
                return StringState; 
                //return _Lang_OverString_;
            }
        }
        *Lang_String = 0;
    }
    
    StringState = _Lang_Success_;
    return StringState; 
    //return _Lang_Success_;
}
//------------------------------------------------------------------------------
// 取得与语言的错误状态( 除了 1(_Lang_Success_)之外，其他皆为错误)
//------------------------------------------------------------------------------
unsigned char Language_Get_Error_State(void)
{
    return Lang_EEPROM_State;
}
//------------------------------------------------------------------------------
// 读取语言总数
//------------------------------------------------------------------------------
unsigned char Language_GetLanguageAmount(void)
{
    return Lang_LanguageAmount;
}
//------------------------------------------------------------------------------
// 读取单一语言字符串数
//------------------------------------------------------------------------------
unsigned char Language_GetStringAmount(void)
{
    return Lang_StringAmount;
}
//------------------------------------------------------------------------------
// 设定重新读取字符串的状态
//------------------------------------------------------------------------------
void Language_ReloadString(void)
{
    Lang_ReloadString = 1;
}
//------------------------------------------------------------------------------
// 读取语言文件版本
//------------------------------------------------------------------------------
unsigned short Language_Version(void)
{
    return Lang_Version;
}

//------------------------------------------------------------------------------
// 清除EEPROM里的语言数据
//------------------------------------------------------------------------------
unsigned char Language_Erase_AllData(void)
{
    if(Erase_EEPROM_Data(_Language_Block_, _Language_Size_) != _Lang_Success_)
    {
        return _Lang_Erase_Fail_;
    }
    return _Lang_Success_;
}
//------------------------------------------------------------------------------
// 清除EEPROM里的Logo资料
//------------------------------------------------------------------------------
unsigned char Pattern_Erase_AllData(void)
{
    if(Erase_EEPROM_Data(_Pattern_Block_, _Pattern_Size_) != _Lang_Success_)
    {
        return _Lang_Erase_Fail_;
    }
    return _Lang_Success_;
}


//==============================================================================

//------------------------------------------------------------------------------
// 将图档写入EERPOM中，如有相同的图案则不会储存
//
// *by_FileName : 文件名(含路径)
//  Return : Implemented state
//------------------------------------------------------------------------------
unsigned char Pattern_WritePatternToEEPRom(char *by_FileName)
{
    unsigned char by_Return;
    
    by_Return = Open_UsbFile(by_FileName);
    
    if(by_Return == _Lang_Success_)
        by_Return = WritePatternToEEPRom();
    
    Close_UsbFile();
    return by_Return;
  //
}

unsigned char WritePatternToEEPRom(void)
{
    unsigned long by_FilePatternAddr = 0;
    unsigned char by_Data = 0;
    unsigned char by_File_Checksum = 0;
    unsigned char by_DataBuff[256];
    unsigned char i = 0;
    unsigned short by_PatternSize = 0;
    unsigned long by_Pattern_Addr = _Pattern_Block_;
    unsigned char by_CheckData[_PatternNameSize_+4+3]; // 读取EERPOM的图档前面DATA
    unsigned char by_EE_Name_Count = 0, by_File_Name_Count = 0;
    unsigned long k;
    unsigned short by_WriteSize = 0;
     
    //------- 检查USB图档 ------------------------------------------------------
    // start byte 
    if(Read_UsbFile(by_FilePatternAddr++, &by_Data, 1) != _Lang_Success_)
        return _Lang_OpenFile_Fail_;   
  
    if(by_Data != _PatternStartCode_)
        return _Lang_FileData_Error_;

    by_File_Checksum += by_Data;
        
    // Pattern Name
    while(1)
    {
        if(Read_UsbFile(by_FilePatternAddr++, &by_Data, 1) != _Lang_Success_)
            return _Lang_OpenFile_Fail_; 
        
        by_File_Name_Count += 1;
        
        by_File_Checksum += by_Data;
        
        if(i >= _PatternNameSize_)
        {// 名称过长
            return _Lang_FileData_Error_;
        }
        else
        {
            by_DataBuff[i++] = by_Data;
        }
        if(by_Data == 0)
            break;  
    }
    // Pattern size
    for(i=0;i<2;i++)
    {
        if(Read_UsbFile(by_FilePatternAddr++, &by_Data, 1) != _Lang_Success_)
            return _Lang_OpenFile_Fail_; 
        
        by_File_Checksum += by_Data;
        by_PatternSize = (by_PatternSize<<8) + by_Data;
        
        if(by_PatternSize >= (_PatternColumn_*2))
        {
            return _Lang_FatternOver_;
        } 
    }
    // Pattern Motion
    if(Read_UsbFile(by_FilePatternAddr++, &by_Data, 1) != _Lang_Success_)
        return _Lang_OpenFile_Fail_; 
        
    by_File_Checksum += by_Data;
    // Pattern Data
    for(i=0; i<by_PatternSize; i++)
    {
        if(Read_UsbFile(by_FilePatternAddr++, &by_Data, 1) != _Lang_Success_)
            return _Lang_OpenFile_Fail_; 
        
        by_File_Checksum += by_Data;
    }
    // Checksum
    if(Read_UsbFile(by_FilePatternAddr++, &by_Data, 1) != _Lang_Success_)
        return _Lang_OpenFile_Fail_; 
    
    if(by_File_Checksum != by_Data)
    {
        return _Lang_FileData_Error_;
    }
    //----- 检查EEPROM里是否有相同的图档 ---------------------------------------   
    while(1)
    {
        // 读EEPROM里的资料
        Mx25L1606E_ReadBlock(by_Pattern_Addr, (_PatternNameSize_+4), by_CheckData);
        //写入图档
        if(by_CheckData[0] != _PatternStartCode_)
        {
            if(by_Pattern_Addr == _Pattern_Block_)
            {
                if(Erase_EEPROM_Data(_Pattern_Block_, _Pattern_Size_) != _Lang_Success_)
                {
                    return _Lang_Erase_Fail_;
                }
            }
            
            break;
        }
        else
        {// 名称检查字符串
            by_EE_Name_Count = 1;
            while(by_CheckData[by_EE_Name_Count++] != 0x00);
            by_EE_Name_Count -= 1;
            
            for(i=0;i<by_EE_Name_Count; i++)
            {
                if(by_CheckData[i+1] != by_DataBuff[i])
                {
                    break;
                }
            }
              
            if(i !=  by_EE_Name_Count || by_EE_Name_Count !=  by_File_Name_Count)     
            {// 图档不相同
                by_Pattern_Addr += (by_EE_Name_Count + 5 + (by_CheckData[by_EE_Name_Count+1]<<8) + by_CheckData[by_EE_Name_Count+2]);       
            }
            else
            {//以有相同图档
                return _Lang_Success_;
            }
        }
    }
    //----- 图档写入EEPROM -----------------------------------------------------
    k = 0;
    while(k < by_FilePatternAddr)
    {
        if((by_FilePatternAddr - k) <= 256)
            by_WriteSize = (by_FilePatternAddr - k);
        else
            by_WriteSize = 256;     
        
        // 读取USB的档案
        if(Read_UsbFile(k, by_DataBuff, by_WriteSize) != _Lang_Success_)
          return _Lang_WriteRRPRom_Error_;
  
        // 写入EEPROM 
        Mx25L1606E_WriteBlock((by_Pattern_Addr+k),by_WriteSize ,by_DataBuff) ;
          
        k += 256;
    }
    //---- 写入确认 --------------------------------------------------------------
    for(k=0; k<by_FilePatternAddr; k++)
    {
        if(Read_UsbFile(k, &by_Data, 1) != _Lang_Success_)
            return _Lang_WriteRRPRom_Error_;
  
        if(by_Data != Mx25L1606E_ReadByte((by_Pattern_Addr+k)))
        {
            return _Lang_WriteRRPRom_Error_;
        }
    }
    
    return _Lang_Success_;
}


//------------------------------------------------------------------------------
// 1. 检查指定的图形
// 2. 将指定图形的名称(Pattern_PatternName[])与图形数据(Pattern_PatternInfo[])读出
// 3. 与计算出图形总数量 ( Pattern_PatternAmount )
// 
// 
// by_NowPattern : 图形代号，由 1 开始编
//------------------------------------------------------------------------------
unsigned char Pattern_CheckingPattern(unsigned short by_NowPattern)
{
    unsigned long by_Address = 0;
    unsigned short by_PatternDataSize = 0;
    unsigned short j, k;
    unsigned char by_Data = 0;
    unsigned char by_Rtn = _Lang_Success_;
    unsigned char by_Checksum = 0;
    unsigned char by_Motion = 0;
    
    Pattern_PatternColumnAmount = 0;
    Pattern_PatternAmount = 0;
    Pattern_PatternMotion = 0;
    for(j=0;j<_PatternNameSize_ ;j++)
      Pattern_PatternName[j] = 0;
    
    // 找图档
    while(1)
    {
        by_Checksum = 0;
        by_Data = Mx25L1606E_ReadByte(( _Pattern_Block_ + (by_Address++)));
        by_Checksum += by_Data;
        if(by_Data != _PatternStartCode_)
        {// start Byte 错误
            if(Pattern_PatternAmount == 0)
                return _Lang_EEPROMData_Error_;
            else if(Pattern_PatternColumnAmount == 0)
            {
                return _Lang_NoThisPattern_;
            }
            else
                break;
        }
        else
        {
            j = 0;
            while(by_Data != 0)
            {
                by_Data = Mx25L1606E_ReadByte(( _Pattern_Block_ + (by_Address++)));
                by_Checksum += by_Data;
                //
                if(Pattern_PatternAmount == (by_NowPattern - 1))
                    Pattern_PatternName[j++] = by_Data;
                //
               // if(j >= _PatternNameSize_)
               //     return _Lang_EEPRomData_Error_;
            }
            // 图文件的资料数
            by_Data = Mx25L1606E_ReadByte(( _Pattern_Block_ + (by_Address++)));
            by_Checksum += by_Data;
            by_PatternDataSize = (by_Data<<8); 
            by_Data = Mx25L1606E_ReadByte(( _Pattern_Block_ + (by_Address++)));
            by_Checksum += by_Data;
            by_PatternDataSize += by_Data;
            
            //图案的显示动作
            by_Data = Mx25L1606E_ReadByte(( _Pattern_Block_ + (by_Address++)));
            by_Checksum += by_Data;
            by_Motion = by_Data;
            
            if(Pattern_PatternAmount == (by_NowPattern - 1))
            {// 放图文件资料
                Pattern_PatternMotion = by_Motion;
                Pattern_PatternColumnAmount = by_PatternDataSize>>1;
                for(k=0; k<Pattern_PatternColumnAmount; k++)
                { 
                    by_Data = Mx25L1606E_ReadByte(( _Pattern_Block_ + (by_Address++)));
                    by_Checksum += by_Data;
                    Pattern_PatternInfo[k] = (by_Data<<8);
                    by_Data = Mx25L1606E_ReadByte(( _Pattern_Block_ + (by_Address++)));
                    by_Checksum += by_Data;
                    Pattern_PatternInfo[k] += by_Data ;
                }
                by_Data = Mx25L1606E_ReadByte(( _Pattern_Block_ + (by_Address++)));
                if( by_Data != by_Checksum)
                {
                    by_Rtn = _Lang_EEPROMData_Error_;
                }
            }
            else
                by_Address += by_PatternDataSize + 1 ; // +1 ==> Checksum(1byte)
            
            Pattern_PatternAmount += 1;
        }
    }
    
    return by_Rtn;
}

//------------------------------------------------------------------------------
// 取得图档行数(一行 : 16 Dot)
//------------------------------------------------------------------------------
unsigned short Pattern_GetPatternColumnAmount(void)
{
    return Pattern_PatternColumnAmount;
}
//------------------------------------------------------------------------------
// 取得图档总数
//------------------------------------------------------------------------------
unsigned short Pattern_GetPatternAmount(void)
{
    return Pattern_PatternAmount;
}
//------------------------------------------------------------------------------
// 取得图案的显示动作
// Return :  0 图案停止
//           1 图案连续转动
//           2 图案先转动在停止 
//------------------------------------------------------------------------------
unsigned char Pattern_GetPatternMotion(void)
{
    return Pattern_PatternMotion;
}

