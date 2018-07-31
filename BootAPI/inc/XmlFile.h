

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XMLFILE_H
#define __XMLFILE_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


const char *const ROOT_Path = {"/"};
const char *const JHT_Path = {"/MATRIX"};
const char *const FW_PATH  = {"/MATRIX/FW"};
const char *const FWF_PATH  = {"/MATRIX/FWF"};
const char *const FW_BIN = {"/MATRIX/FW/UCB/*.bin"} ;
const char *const FWF_BIN = {"/MATRIX/FWF/UCB/*.bin"} ;


const char *const PROGRAM_PATH = {"/MATRIX/PROGRAMS"};

const char *const USERS_Path = {"/MATRIX/USER"};
/*
const char *const USER_Path[4] = {
    "/JHT/USERS/USER1",
    "/JHT/USERS/USER2",
    "/JHT/USERS/USER3",
    "/JHT/USERS/USER4"
};

const char *const USER_File[4] = {
    "/JHT/USERS/USER1/USER1.usr",
    "/JHT/USERS/USER2/USER2.usr",
    "/JHT/USERS/USER3/USER3.usr",
    "/JHT/USERS/USER4/USER4.usr"
};
*/


const char User_Default[] = {
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\r\
<user>\n\r\
\t<name></name>\n\r\
\t<userid></userid>\n\r\
\t<fitlinxxid></fitlinxxid>\n\r\
\t<firstname></firstname>\n\r\
\t<lastname></lastname>\n\r\
\t<kg></kg>\n\r\
\t<age></age>\n\r\
\t<gender></gender>\n\r\
\t<goal>\n\r\
\t\t<kCal></kCal>\n\r\
\t\t<sec></sec>\n\r\
\t\t<km></km>\n\r\
\t\t<watt></watt>\n\r\
\t</goal>\n\r\
\t<goalRemain>\n\r\
\t\t<kCal></kCal>\n\r\
\t\t<sec></sec>\n\r\
\t\t<km></km>\n\r\
\t\t<watt></watt>\n\r\
\t</goalRemain>\n\r\
</user>\n\r"
};

const char Workout_Default[] = {
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\r\
<workout>\n\r\
\t<userid></userid>\n\r\
\t<model></model>\n\r\
\t<summary>\n\r\
\t\t<Year></Year>\n\r\
\t\t<Month></Month>\n\r\
\t\t<Date></Date>\n\r\
\t\t<Avgpace></Avgpace>\n\r\
\t\t<Calories></Calories>\n\r\
\t\t<Distance></Distance>\n\r\
\t\t<Totalelev></Totalelev>\n\r\
\t\t<TotalTime></TotalTime>\n\r\
\t</summary>\n\r\
</workout>"
};


#endif /* __XMLFILE_H */

/******************* (C) COPYRIGHT 2010 Johnson Fitness *****END OF FILE****/
