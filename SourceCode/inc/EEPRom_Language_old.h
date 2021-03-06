//----- Language --------------------------------------------------------------
// EEPROM Start Address
#define   _Language_Block_    _Block0_Sector2_
#define   _Language_Size_     (_SectorBase_*25)  // 4k * 25 = 100K bytes // Sector 2 ~ Sector 26
// EEPROM Language Start Address
#define   _Language_Data_Address_    (_Language_Block_+_CheckStringSize_)
// EEPROM写入完成后，写入此字符串做开机语言确认用。
#define   _LanguageWriteOK_   "Has the data in the EEPROM"
#define   _CheckStringSize_   27  //字符串长度+结尾(0x00)(26 + 1)


//------- Status ---------------------------------------------------------------
#define   _Lang_Fail_                 0
#define   _Lang_Success_              1
#define   _Lang_USB_Fail_             2
#define   _Lang_OpenFile_Fail_        3
#define   _Lang_EEPRomData_Error_     4
#define   _Lang_Machine_Error_        5
#define   _Lang_WriteRRPRom_Error_    6
#define   _Lang_OverString_           7
#define   _Lang_Language_Error_       8
#define   _Lang_EEPROMData_Error_     9
#define   _Lang_FileOverSize_         10
#define   _Lang_Erase_Fail_           11
#define   _Lang_FileData_Error_       12 
#define   _Lang_FatternOver_          13
#define   _Lang_NoThisPattern_        14  

#define  _Lang_Start_Code_     0xA5
#define  _Lang_End_Code_       0x5A

//---- Data Index --------------------------------------------------------------
#define  _StartCode_       0
#define  _LangAmount_      1
#define  _StringAmount_    2
#define  _VersionHighByte_ 3
#define  _VersionLowByte_  4
#define  _StringName_      5
//---- String Type ------------------------------------------------------------
#define  _Lang_Language_Name_      1
#define  _Lang_Language_Data_      2

//------------------------------------------------------------------------------
#define   _TheWordsOfOneString_  202
extern unsigned char Lang_StringBuffer[_TheWordsOfOneString_];


unsigned char Open_UsbFile(char *by_UsbFileName);
void Close_UsbFile(void);
unsigned char Read_UsbFile(unsigned long by_FileAddress, unsigned char *by_DataBuff, unsigned short by_ReadSize);
//
unsigned char WriteLanguageToEEPRom(unsigned char *by_UcbMachineType);
unsigned char Lang_WriteLanguageToEEPRom(unsigned char *by_UcbMachineType, char *by_FileName);
void Language_ClearBasicInfo(void);
unsigned char Language_LanguageChecking(unsigned char by_NowLanguage);
unsigned char Language_GetString(unsigned char Lang_StringType, unsigned char Lang_StringNumber);
unsigned char Language_GetLanguageAmount(void);
unsigned char Language_GetStringAmount(void);
void Language_ReloadString(void);
unsigned short Language_Version(void);
unsigned char Language_EEPROMCheck(unsigned char *by_UCBTypeString);
unsigned char Erase_EEPROM_Data(unsigned long by_Address, unsigned long by_TotalSize);
unsigned char Language_Erase_AllData(void);

//------- Pattern --------------------------------------------------------------
// EEPROM Pattern Start Address
#define   _Pattern_Block_    (27*_SectorBase_)
#define   _Pattern_Size_     (_SectorBase_*50)  // 4k * 25 = 100K bytes // Sector 27 ~ Sector 76

//
#define   _PatternStartCode_    0x33
//
#define   _PatternNameSize_    52
//
#define   _PatternColumn_     200
extern unsigned short Pattern_PatternInfo[_PatternColumn_];
extern unsigned char Pattern_PatternName[_PatternNameSize_+1];
//
unsigned char WritePatternToEEPRom(void);
unsigned char Pattern_WritePatternToEEPRom(char *PatternFileName);
unsigned char Pattern_CheckingPattern(unsigned short by_NowPattern);
unsigned char Pattern_Erase_AllData(void);
unsigned short Pattern_GetPatternColumnAmount(void);
unsigned short Pattern_GetPatternAmount(void);
unsigned char Pattern_GetPatternMotion(void);
