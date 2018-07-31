

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UCBINFO_H
#define __UCBINFO_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
// Infomation 
//#pragma location = ".bootinfo"
__root const unsigned char Boot_Manufacture[52]   @ ".bootinfo"  = "Johnson Health Tech. Co., Ltd." ;
__root const unsigned char Boot_McuType[20]       @ ".bootinfo"= "STM32F205VC_SUM1801A" ;//==>MCU型号+硬件版号
__root const unsigned char Boot_ModuleName[20]    @ ".bootinfo"= "2018 Vulcan" ;
__root const unsigned char Boot_ModuleNo[20]      @ ".bootinfo"= "Vulcan";
__root const unsigned char Boot_Product[20]       @ ".bootinfo"= "Matrix" ;
__root const unsigned char Boot_Version[20]       @ ".bootinfo" = "V1.001" ;

// array size 必须是四的倍数
__root const unsigned char User_Manufacture[52]  @ ".userinfo" = "Johnson Health Tech. Co., Ltd." ;
__root const unsigned char User_McuType[20]      @ ".userinfo" = "STM32F205VC_SUM1801A" ;
__root const unsigned char User_ModuleName[20]   @ ".userinfo" = "2018 Vulcan";	
__root const unsigned char User_ModuleNo[20]     @ ".userinfo" = "Vulcan";	
__root const unsigned char User_Product[20]      @ ".userinfo" = "Matrix" ;
__root const unsigned char User_Version[20]      @ ".userinfo" = "V001.01-20180702" ;

/*
const unsigned char Boot_Manufacture[52] = "Johnson Health Tech. Co., Ltd." ;
const unsigned char Boot_McuType[20] = "STM32F105VC" ;
const unsigned char Boot_ModuleName[20] = "Premium T5x" ;
const unsigned char Boot_ModuleNo[20] = "TM" ;
const unsigned char Boot_Product[20] = "Matrix" ;


// array size 必须是四的倍数
const unsigned char User_Manufacture[52]  @ ".userinfo" = "Johnson Health Tech. Co., Ltd." ;
const unsigned char User_McuType[20]      @ ".userinfo" = "STM32F105VC" ;
const unsigned char User_ModuleName[20]   @ ".userinfo" = "Premium T5x" ;
const unsigned char User_ModuleNo[20]     @ ".userinfo" = "TM" ;
const unsigned char User_Product[20]      @ ".userinfo" = "Matrix" ;
const unsigned char User_Version[20]      @ ".userinfo" = "S100-20100715" ;
*/

#endif /* __UCBINFO_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
