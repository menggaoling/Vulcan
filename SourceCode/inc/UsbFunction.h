#ifndef _USBFunction_H_
#define _USBFunction_H_


#define F_WorkoutSummary   0xF1
#define F_User             0xF2
//==============================================================================
// USER 数据存入 USB 最大范围? 
#define Max_USER           4
#define Max_WorkoutSummary 5
//==============================================================================
// 从档案中搜寻字符串参数
#define M_program     1
#define M_models      2
#define M_name        3
#define M_levelamount 4
#define M_segamount   5
#define M_levels      6
#define M_level       7
#define M_t       8
#define M_e       9
#define M_1       10
#define M_2       11
#define M_3       12
#define M_4       13
#define M_5       14
#define M_6       15
//==============================================================================
#define USB_Message1 "program"
#define USB_Message2 "models"
#define USB_Message3 "name"
#define USB_Message4 "levelamount"
#define USB_Message5 "segamount"
#define USB_Message6 "levels"
#define USB_Message7 "level"
#define USB_Message8 "t"
#define USB_Message9 "e"
#define USB_Message10 "1"
#define USB_Message11 "2"
#define USB_Message12 "3"
#define USB_Message13 "4"
#define USB_Message14 "5"
#define USB_Message15 "6"
//==============================================================================
#define IN_Year      1
#define IN_Month     2
#define IN_Date      3
#define IN_Averpace  4
#define IN_Calories  5
#define IN_Distance  6
#define IN_Totalelev 7
#define IN_TotalTime 8
//==============================================================================
#define C_WriteFail 0
#define C_WriteOK   1
#define C_OpenFail  2
#define C_OpenOK    3


/*
typedef struct 
{
    unsigned int Year ;              
    unsigned int Month ;         
    unsigned int Date ;          
    unsigned int Averpace ;
    unsigned int Calories ;
    unsigned int Distance ;
    unsigned int Totalelev ;
    unsigned int TotalTime ;
} USB_Information ;
*/



void TEST_RW(void);
void UsbFunction_ClearReadWriteBuffer(char by_D);
//void UsbFunction_WriteInformation(char by_Dat);
//unsigned int UsbFunction_SetData(unsigned int by_D1,unsigned int by_D2,char by_Dot);
//void UsbFunction_SetUserNumber(char by_D1,char by_D2);
//char UsbFunction_CheckMessage(char by_D1,unsigned int by_D2);
//char UsbFunction_CheckWord(char *pby_Dat,char *pby_Dat1);
//char UsbFunction_Get_ProgramName(void);

//void UsbFunction_Table(void);
//char UsbFunction_ASCIItoNumber(void);
//char UsbFunction_Get_Level_Table(char by_D1,char by_D2);
//void UsbFunction_CheckBuffer_1K(void);


//unsigned int UsbFunction_WorkoutSummary(unsigned int by_D1,char by_D2);
//char UsbFunction_WorkoutSummary(void);
//void UsbFunction_SetWorkoutSummaryNumber(char by_D1,unsigned int by_D2);
//char UsbFunction_GetProgramInformation(char by_D1,char by_D2);
//char UsbFunction_GetTable(char by_D1,char by_D2);
//char* UsbFunction_ProgramNameMessage(char by_D);
unsigned char* UsbFunction_BinFileNameMessage(char by_D);
char UsbFunction_AssignSoftwareUpdate(char by_D);
char UsbFunction_GetTotalBinFile(void);
void UsbFunction_USB(unsigned int by_D);
void UsbFunction_Get_WrkFileSerialNumber(void);
void UsbFunction_SoftwareUpdate(char by_ShowMessage);
char UsbFunction_USB_Status(void); // Modify by Kunlung 20110125

// Add by Kunlung 20110221
void UsbFunction_CheckConnected(unsigned short CheckTime,unsigned short CheckTime1 ) ;

// Add by Kunlung 20110127
unsigned char UsbFunction_ExportEngineerParameter(void) ;
//#ifdef    _ExportAndInport_Mode0_
unsigned char UsbFunction_InportEngineerParameter(unsigned char InportMode) ;
//#endif
//#ifdef    _ExportAndInport_Mode1_
//unsigned char UsbFunction_InportEngineerParameter(void) ;
//#endif
char UsbFunction_ReadWifiConfig(void);
char UsbFunction_ExportWifiConfig(void);



/*
#ifdef WiFi_Module
unsigned char UsbFunction_ReadWifiConfig(void);
unsigned char UsbFunction_FirmwareUpdateWiFi(void);
void UsbFunction_SaveWorkoutDataToWiFiAM(unsigned char Mode) ; // 20120214
unsigned char UsbFunction_WiFiEraseFlash(void) ; // 20120111
unsigned char UsbFunction_GetWiFiAPSetting(void) ;// 20120111
void UsbFunction_SetWiFiReConnect(void) ; // 20120117
// 20110829
// Mask by Kunlung 20120111
bool UsbFunction_SaveWorkoutDataToWiFi( unsigned char *USBSaveStatus ) ;
//
#endif
*/

void __memset(void *s, char c, long n);
void __memcpy(void *s1, const void *s2, long n) ;
char __memcmp(const void *s1, const void *s2, long n) ;
long __strlen(const char *s) ;
char *__strchr(const char *s, int c) ;
char *__strstr(const char *s1, const char *s2);
char *__strcpy(char *s1, const char *s2) ;
char *__strncpy(char *s1, const char *s2, long n) ;
int __strcmp(const char *s1, const char *s2) ;
unsigned char __CharBCD2DEC(unsigned char s1) ;

// 2013.07.26
void UsbFunction_ClearUSBFirstCheck(void);
char *UsbFunction_BinFileAddress(char by_D,char by_D1);
// 2014.01.29
void UsbFunction_ClearBinInfo(void);
// start v2.1-13
void UsbFunction_RSCU_SoftwareUpdate(void);
void UsbFunction_Protection(void);
//
#endif // _USBFunction_H_
