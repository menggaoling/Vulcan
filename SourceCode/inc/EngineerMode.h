#ifndef _EngineerMode_H_
#define _EngineerMode_H_

char EngineerMode_SpecialMode(unsigned short by_KeyInput);
char EngineerMode_Manager(void);
char EngineerMode_Engineer(void);
char EngineerMode_Service(void);
char EngineerMode_Test(void);
void EngineerMode_AutoCalibration(void);
void EngineerMode_RunningTestMode(void);
char EngineerMode_SerialNumber(void);
char EngineerMode_Accumulated(char by_D);
void EngineerMode_RTCSysTime(char by_D);
void EngineerMode_RPMParameter(void);
void Target_SoftwareUpdate(u8 by_M);
void EngineerMode_SetMode(char by_D);
char EngineerMode_GetMode(void);
void EngineerMode_ExportOrImport(unsigned char Index);
void EngineerMode_RSCUpdate(unsigned char by_Mode);
void EngineerMode_BackUpProcess(unsigned char Index);
#endif 
