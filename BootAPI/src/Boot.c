/**
  ******************************************************************************
  * @file    Boot.c
  * @author  JHT Team
  * @version V1.0.0
  * @date    03/12/2010
  * @brief   
  ******************************************************************************
*/ 

//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------
// File System and usb host system
#include "hcc_types.h"
#include "usb_host.h"
#include "pmgr.h"
#include "usb_mst.h"
#include "fat.h"
#include "mst.h"
#include "os.h"
#include "BOOTflash.h"
#include "XmlFile.h"
#include "Boot.h"
#include "UCBInfo.h"
#include "stm32_reg.h"
#include "HT1381.h"
//------------------------------------------------------------------------------
#include "Mx25L1606E.h"


//------------------------------------------------------------------------------

#define _VolumeNumber_        1


//------------------------------------------------------------------------------
char TempFilename[256] ; 
unsigned char flashData[_PP_PageSize_];

unsigned long CheckTime = 0 ;
//------------------------------------------------------------------------------
/* Declares a section */
char Boot_CheckUserProgram(void) ;
char Boot_UpdateUserProgram(char*filename,char DeleteFile,char Source)  ;
char Boot_ReadFileData(char*filename, uint32_t Address, uint16_t RetSize, uint16_t *RetBuffer, char Source);
char Boot_CheckPenDriverPlunin(void) ;
char Boot_CheckBINFileInformation(char *filename,char *sAddr,long tAddr) ; // filename: "/xxx/xxx.bin, sAddr: source addres, tAddr: file data index address
char Boot_CheckJHTfile(char *filename) ;
char Boot_CreateJHTfile(char Mode,char*filename) ;
char Boot_SearchBINfile(char*filename,char*retfilename) ;
char Boot_CheckUserInformation(const unsigned char *Src,const unsigned char *Dist) ;
void Boot_DisableIRQn(char All) ;
void Boot_UCBFirmwareUpdate(char*filename,char DeleteFile,char Source) ;
void Boot_MoveUCBvectorToRAM(void) ;
//

//extern void WiFiBoot_UsartInitial(void) ;
//extern unsigned char WiFiBoot_CheckUCBupdate(void) ;
//extern unsigned char WiFiBoot_UCBUpdateProcess(void) ;
//extern unsigned char WiFiBoot_UpdataUCB(void) ;
extern void WiFi_TargetLed(unsigned char TargetLed);
//------------------------------------------------------------------------------
HAL_StatusTypeDef Boot_EarseUserProgram()
{
  HAL_StatusTypeDef status;
  //----------------------------------------------------------------
  /* Unlock the Flash Program Erase controller */
  BootFLASH_Unlock();
  // Earse Flash
  status = BootFLASH_EarseUserProgram(StartAddr,EndAddr) ;
  // 
  return status ;
}



//------------------------------------------------------------------------------
char Boot_CheckUserInformation(const unsigned char *Src,const unsigned char *Dist) 
{
  while( *Src != 0 )
      {
      if( *Src != *Dist )
          {
          return _DataNotMatch_ ;
          }
      Src += 1 ;
      Dist += 1 ;
      } 
  return _DataMatch_;
}


//------------------------------------------------------------------------------
void Boot_UCBFirmwareUpdate( char*filename, char DeleteFile, char Source)
{
  char Status ;
  //
  WiFi_TargetLed(1); // LED Display  Fan_Green
  //
  Boot_DisableIRQn(1) ; // Disable All IRQ except OTG 
  //
  Status = Boot_UpdateUserProgram(filename,DeleteFile,Source) ;
  // if Error then Erase Flash Memory
  if( Status != _FirmwareUpdateOK_ )
      Boot_EarseUserProgram() ;
  
  // SWReset
  Boot_DisableIRQn(1) ;
  //SCB->AIRCR = 0x05fa0004 ; // Software Reset   
  //while(1) ;
  Boot_CallApplication(BOOT_START_ADDRESS);  //
}



//------------------------------------------------------------------------------
char Boot_CheckUserProgram()
{
  unsigned long *pulApp;
  char ret_status ;
  ret_status = _UserAPIError_ ;
  //----------------------------------------------------------------------------
  if( Boot_CheckUserInformation(&Boot_Manufacture[0],(const unsigned char *)(0x08000000+_Info_ManufactureIndex_)) == _DataMatch_ )
      if( Boot_CheckUserInformation(&Boot_McuType[0],(const unsigned char *)(0x08000000+_Info_McuTypeIndex_)) == _DataMatch_ )
          if( Boot_CheckUserInformation(&Boot_ModuleName[0],(const unsigned char *)(0x08000000+_Info_ModuleNameIndex_)) == _DataMatch_ )
              //if( Boot_CheckUserInformation(&Boot_ModuleNo[0],(const unsigned char *)(0x08000000+_Info_ModuleNoIndex_)) == _DataMatch_ ) 
                  //if( Boot_CheckUserInformation(&Boot_Product[0],(const unsigned char *)(0x08000000+_Info_ProductIndex_)) == _DataMatch_ )                            
                      ret_status = _UserAPIOk_ ;
  /*                      
  ptr = &Boot_Manufacture[0] ;
  ptr1 = (const unsigned char *)(0x08000000+_Info_ManufactureIndex_) ;
  while( *ptr != 0 )
      {
      if( *ptr != *ptr1 )
          {
          return _UserAPIError_ ;
          }
      ptr += 1 ;
      ptr1 += 1 ;
      } 
  */        
  //----------------------------------------------------------------------------
  // Check User Main Function Address is OK
  // See if the first location is 0xfffffffff or something that does not
  // look like a stack pointer, or if the second location is 0xffffffff or
  // something that does not look like a reset vector.
  //
  if(  ret_status == _UserAPIOk_ )
      {
      pulApp = (unsigned long *)APP_START_ADDRESS;
      if((pulApp[0] == 0xffffffff) || ((pulApp[0] & 0xfff00000) != 0x20000000) ||
          (pulApp[1] == 0xffffffff) || ((pulApp[1] & 0xfffff000) < 0x0800D000) )  
          {
          //
          // App starting stack pointer or PC is not valid, so force an update.
          //
          ret_status = _UserAPIError_ ;
          }
      }
  //----------------------------------------------------------------------------
  return ret_status ;
}


//------------------------------------------------------------------------------
char Boot_CheckPenDriverPlunin(void)
{
  if(!f_checkvolume(_VolumeNumber_))
      {
      //
      if( f_getdrive() != _VolumeNumber_ )
            f_chdrive(_VolumeNumber_) ;
      //
      return(_USBPlugin_) ;
      }
  //
  return(_USBNotPlugin_) ;
}


      
//------------------------------------------------------------------------------
char Boot_CreateJHTfile(char Mode,char*filename)
{
  F_FILE *file;
 // unsigned int i;
  unsigned long count ;
#ifdef  _Debuf_API_
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
#endif 
  switch(Mode)
    {
    case  _JHTFileCreate_   ://==>建立文件夹
                            f_mkdir("/MATRIX") ;
                            f_mkdir("/MATRIX/USER") ;
                            f_mkdir("/MATRIX/FW") ;
                            f_mkdir("/MATRIX/FW/UCB") ;
                            break ;
    case  _JHTFileUser_     :
                            #ifdef  _Debug_JHTFile_
                            
                            #else  

                            file = f_open(filename,"w"); 
                            if( !file )
                                {
                                f_close(file) ;
                                return _NoFileName_ ;
                                }
                            else
                                {
                                count = strlen( &User_Default[0] ); 
                                f_write(&User_Default[0],1,count,file) ;
                                }
                            f_close(file) ;
                            #endif      
                            break ;
    case  _JHTFileProg_     :
                            #ifdef  _Debug_JHTFile_
                            #else      
                            #endif
                            break ;
    case  _JHTFileWork_     :
                            #ifdef  _Debug_JHTFile_
                            #else 

                            file = f_open(filename,"w"); 
                            if( !file )
                                {
                                f_close(file) ;
                                return _NoFileName_ ;
                                }
                            else
                                {
                                count = strlen( &Workout_Default[0] ); 
                                f_write(&Workout_Default[0],1,count,file) ;
                                }
                            f_close(file) ;

                            #endif
                            break ;
    default                 :
                            break ;
    }
#ifdef  _Debuf_API_
      }
  else
      return _USBNotPlugin_ ;
#endif
  return 0 ; 
}
  


//------------------------------------------------------------------------------
char Boot_CheckJHTfile(char *filename)
{
  F_FIND find;
#ifdef  _Debuf_API_
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
#endif    
    
      if(!f_findfirst(filename, &find)) 
          return  _JHTFileExist_ ;
        
#ifdef  _Debuf_API_
      }
  else
      return _USBNotPlugin_ ;
#endif 
  return _JHTFileNotExist_ ;
}



//------------------------------------------------------------------------------

char Boot_CheckBINFileInformation(char *filename,char *sAddr,long tAddr )
{
  //
  F_FIND find;
  F_FILE *file; 
  char Buffer[200] ;
  char *ptr ;
  char *ptr1 ;
  //
#ifdef  _Debuf_API_
  if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
#endif        
     
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
                  f_read(&Buffer[0],1,0x80,file); //read 128 byte 
                  //----------------------------------------------------------------
                  // check Manufacture
                  ptr = sAddr ;
                  ptr1 = &Buffer[0] ;
                  while( *ptr != 0 )
                      {
                      if( *ptr != *ptr1 )
                          {
                          f_close(file) ;
                          return _DataNotMatch_ ;
                          }
                      ptr += 1 ;
                      ptr1 += 1 ;
                      }
                  // 20110727  Check length
                  if( *ptr1 != 0 )
                      {
                      f_close(file) ;
                      return _DataNotMatch_ ;
                      }
                  //  
                  }
              f_close(file) ;
              }
          }
#ifdef  _Debuf_API_      
      }
  else
      return _USBNotPlugin_ ;
#endif  
  //
  return _DataMatch_ ;
}

//------------------------------------------------------------------------------
/* Define the STM32F10x FLASH Page Size depending on the used STM32 device */

//
char Boot_UpdateUserProgram(char*filename,char DeleteFile,char Source)
{
    uint16_t Buffer[1024] ;
    uint16_t *ptr ;
    uint32_t FlashAddress ;
    // Flash
    long SearchIndex ;
    uint32_t DataCounter = 0x00 ;
    //__IO uint32_t NbrOfPage = 0x00;
    volatile HAL_StatusTypeDef FLASHStatus;
    //
    FLASHStatus = Boot_EarseUserProgram() ;
    if( FLASHStatus != HAL_OK )
    {
        return _FlashEarseError_ ;
    }
    //----------------------------------------------------------------
    FlashAddress = StartAddr ; 
    SearchIndex = (StartAddr & 0x000FFFFF) ;
    while(1)
    {
        WiFi_TargetLed(2); // LED Display  Fan_Orange
        if(Boot_ReadFileData(filename, SearchIndex, 2048, Buffer, Source) == 0) /* read 2048 byte flash block size 2Kbyte*/
            return _FirmwareUpdateError_ ;
        ptr = &Buffer[0] ;
        for( DataCounter = 0 ; DataCounter < 1024 ; DataCounter++)
        {
            FLASHStatus = BootFLASH_ProgramHalfWord((FlashAddress+(DataCounter*2)),*ptr ) ;
            if( FLASHStatus != HAL_OK )
            {
                return _FirmwareUpdateError_ ;
            }
            ptr++ ;
        }
        FlashAddress += 2048 ;
        SearchIndex += 2048;
        if( FlashAddress >= InitialTableAddr )
            break ;
    }
    //----------------------------------------------------------------
    SearchIndex = Init_Table_Addr & 0x000FFFFF;
    if(Boot_ReadFileData(filename, SearchIndex, 2048, Buffer, Source) == 0) /* read 2048 byte flash block size 2Kbyte*/
        return _FirmwareUpdateError_ ;
    ptr = &Buffer[0] ;
    FlashAddress = InitialTableAddr ;
    for( DataCounter = 0 ; DataCounter < 1024 ; DataCounter++)
    {
        FLASHStatus = BootFLASH_ProgramHalfWord((FlashAddress+(DataCounter*2)),*ptr ) ;
        if( FLASHStatus != HAL_OK )
        {
            return _FirmwareUpdateError_ ;
        }
        ptr++ ;
    }
    //---------------------------------------------------------------
    SearchIndex = InfomationTableAddr & 0x000FFFFF;
    if(Boot_ReadFileData(filename, SearchIndex, 2048, Buffer, Source) == 0) /* read 2048 byte flash block size 2Kbyte*/
        return _FirmwareUpdateError_ ;
    
    ptr = &Buffer[0] ;
    FlashAddress = InfomationTableAddr ;
    for( DataCounter = 0 ; DataCounter < 1024 ; DataCounter++)
    {
        FLASHStatus = BootFLASH_ProgramHalfWord((FlashAddress+(DataCounter*2)),*ptr ) ;
        if( FLASHStatus != HAL_OK )
        {
            return _FirmwareUpdateError_ ;
        }
        ptr++ ;
    }
    //----------------------------------------------------------------
    if( DeleteFile != 0 && Source == 0)
    {// USB
        f_delete((const char*)filename);
    }
    return _FirmwareUpdateOK_ ;
}

char Boot_ReadFileData(char*filename, uint32_t Address, uint16_t RetSize, uint16_t *RetBuffer, char Source)
{// Return==> 1: read successfully, 0: failure
    F_FIND find;
    F_FILE *file;
    
    char Result = 0;
    
    switch(Source)
    {
        default:
        case 0: // USB
            //WiFi_TargetLed(1); // LED Display
            if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
            {
                if(!f_findfirst((const char*)filename, &find))
                {
                    file = f_open(filename,"r");
                    if( file )
                    {
                        if(!f_seek(file,Address,FN_SEEK_SET))
                        {
                            f_read(&RetBuffer[0],sizeof(RetBuffer[0]), RetSize/sizeof(RetBuffer[0]), file); /* read 2048 byte flash block size 2Kbyte*/
                            Result = 1; // Success
                        }
                    }
                    f_close(file) ;
                }
            }
            break;
        case 1: // EEPROM
            //WiFi_TargetLed(2); // LED Display
            for(int i=0; i<RetSize/sizeof(RetBuffer[0]); i++)
            {
                RetBuffer[i] = Mx25L1606E_ReadWord(EEPRomPartition_StartAddress_RSCU + Address + i*2);
            }
            Result = 1; // Success
            break;
        case 2: // EEPRom Back Up
          {
            if(Address == (Init_Table_Addr&0x000FFFFF))
            {
              for(char i=0; i < 2; i++)
              {
                RetBuffer[i] = Mx25L1606E_ReadWord(EEPRomPartition_StartAddress_Backup + (InitialTableAddr&0x000FFFFF) + i*2);
              }
              uint32_t * p = (uint32_t  *)RetBuffer ;
              if( *p != 0 && *p != 0xFFFFFFFF )
              {
                Address = (InitialTableAddr&0x000FFFFF);
              }
            }
            for(int i=0; i<RetSize/sizeof(RetBuffer[0]); i++)
            {
                RetBuffer[i] = Mx25L1606E_ReadWord(EEPRomPartition_StartAddress_Backup + Address + i*2);
            }
            Result = 1; // Success
          }
            break;
    }
    
    return Result;
}

//------------------------------------------------------------------------------
void Boot_CallApplication(unsigned long ulStartAddr)
{
  //
  // Set the vector table to the beginning of the app in flash.
  //
  SCB->VTOR = ulStartAddr ;
  //
  // Load the stack pointer from the application's vector table.
  //
  __asm("    ldr     r1, [r0]\n"
        "    mov     sp, r1");

  //
  // Load the initial PC from the application's vector table and branch to
  // the application's entry point.
  //
  __asm("    ldr     r0, [r0, #4]\n"
        "    bx      r0\n");
  
  return ;
}


//------------------------------------------------------------------------------

char Boot_SearchBINfile(char*filename,char*retfilename) 
{
  F_FIND find;
  char *pch ;
  char len ;
  //
  if(!f_findfirst(filename,&find))
      {
       do
       {
          // 20140205
          // Copies src to the beginning of dest 
          strcpy(retfilename,filename) ;
          pch = strstr(retfilename,"*.bin") ;
          len = strlen(find.filename)+1 ;//==>计算长度
          strncpy(pch,find.filename,len);
          if( Boot_CheckBINFileInformation(retfilename,(char*)&Boot_Manufacture[0],_Info_ManufactureIndex_) == _DataMatch_ ) 
          {  
              if( Boot_CheckBINFileInformation(retfilename,(char*)&Boot_McuType[0],_Info_McuTypeIndex_) == _DataMatch_ )
              {
                  if( Boot_CheckBINFileInformation(retfilename,(char*)&Boot_ModuleName[0],_Info_ModuleNameIndex_) == _DataMatch_ )
                  {
                       if( Boot_CheckBINFileInformation(retfilename,(char*)&Boot_Version[0], ((unsigned long)Boot_Version & 0x000FFFFF)) == _DataMatch_ )
                       {
                           return _HaveBinFile_ ;
                       }
                  }
              }
          }
       } while (!f_findnext(&find));
      }
  //
  return  _NoBinFile_  ; 
}

//------------------------------------------------------------------------------
void Boot_DisableIRQn(char All)
{
  SCB->SHCSR &= ~SCB_SHCSR_SYSTICKACT_Msk ;
  SysTick->VAL   = 0 ;                      /* Load the SysTick Counter Value */
  SysTick->CTRL  = 0 ;
  if( All != 0 )
      {
      NVIC->ICER[0] = 0xFFFFFFFF ;
      NVIC->ICER[1] = 0xFFFFFFFF ;
      NVIC->ICER[2] = 0xFFFFFFFF ;
      }
  else
      {
      NVIC->ICER[0] = 0xFFFFFFFF ; //  0~31
      NVIC->ICER[1] = 0xFFFFFFFF ; // 32~63
      NVIC->ICER[2] = 0xFFFFFFF7 ; // 64~67 //OTG = 67
      }
  
  return ;
}


//------------------------------------------------------------------------------
int Boot_InitialUSB() 
{
  int rc=0;
  // Add RTC Hardware GPIO define
  HT1381_HWInitial() ; 
  //
  rc=os_init();
  if(rc==0) rc=usbh_init();			    /* initialize USB host */
  if(rc==0) rc=usb_mst_init();			/* initalize mass storage */
  if(rc==0) rc=usbh_start();			  /* start USB host */
  if(rc==0)
      {
      f_init();
      f_enterFS();
      (void)f_initvolume(_VolumeNumber_,mst_initfunc,F_AUTO_ASSIGN);
      (void)f_chdrive(_VolumeNumber_);					  /* initialize file system */
      }
  return rc;  
}


//------------------------------------------------------------------------------
void Boot_MoveUCBvectorToRAM()
{
  uint32_t size = 0x200 ;
  uint32_t const * src = (uint32_t const *)0x0800D000 ;
  uint32_t * dest = (uint32_t *)0x2000C000 ;
  do
    {
      *dest++ = *src++;
      size -= 4;
    } while (size != 0); 
  
  return ;
}


#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)
void Boot_NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (NVIC_PriorityGroup & (uint32_t)0x07UL);             /* only values 0..7 are used          */
  /* Check the parameters */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  reg_value  =  SCB->AIRCR;                                                   /* read old register configuration    */
  reg_value &= ~((uint32_t)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk)); /* clear bits to change               */
  reg_value  =  (reg_value                                   |
                ((uint32_t)0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                (PriorityGroupTmp << 8U)                      );              /* Insert write key and priorty group */
  SCB->AIRCR =  reg_value;
}


void Boot_NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  /* Check the parameters */
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

void Boot_usbmain(void)
{
  //
  Boot_DisableIRQn(1) ;
  Boot_NVIC_SetVectorTable(FLASH_BASE,0) ;
  // Configure one bit for preemption priority 
  Boot_NVIC_PriorityGroupConfig(NVIC_PRIORITYGROUP_4);
  // 
  Boot_InitialUSB() ; 
  
  // TPS2546 (USB)
  /* PC4 PA9 PA10 push-pull output */
  /*USB CTRL(PC4) set Data Line,S0(PA9)S1(PA10) set 1 */
  
  
  HCC_GPIO_MODER(C) &= ~(0x00000300);
  HCC_GPIO_MODER(C) |= (0x00000100);
  HCC_GPIO_OTYPER(C) &= ~(0x00000010);
  HCC_GPIO_OSPEEDR(C) &= ~(0x00000300);
  HCC_GPIO_PUPDR(C) &= ~(0x00000300);
  
  HCC_GPIO_MODER(A) &= ~(0x003c0000);
  HCC_GPIO_MODER(A) |= (0x00140000);
  HCC_GPIO_OTYPER(A) &= ~(0x00000600);
  HCC_GPIO_OSPEEDR(A) &= ~(0x003c0000);
  HCC_GPIO_PUPDR(A) &= ~(0x003c0000);
  
  HCC_GPIO_BSRR(C)=(1u<<20);
  HCC_GPIO_BSRR(A)=(1u<<9);
  HCC_GPIO_BSRR(A)=(1u<<10);
  
  /*
  if( Boot_CheckUserProgram() == _UserAPIOk_ )
    Boot_CallApplication(APP_START_ADDRESS) ;
  */
  for(;;)
      {
      //------------------------------------------------------------------------
      // Delay time
      CheckTime += 1 ;
      while( CheckTime > 0x0500 )
          {
          pmgr_pcd_task();//==> USB侦测用
          usbh_transfer_task();//==> USB侦测用
          CheckTime = 0 ;
          }
      //  
      // <<==========================
      if( Boot_CheckPenDriverPlunin() == _USBPlugin_ )
      {
          if( Boot_CheckJHTfile((char*)JHT_Path) == _JHTFileExist_ )
          {  
              if( Boot_CheckJHTfile((char*)FW_PATH) == _JHTFileExist_ )
              {
                  if( Boot_SearchBINfile((char*)FW_BIN,&TempFilename[0]) == _HaveBinFile_ )
                  {
                      if( TempFilename[0] != 0 )
                      {
                          //Boot_UCBFirmwareUpdate( &TempFilename[0],1 );
                          Boot_UCBFirmwareUpdate( &TempFilename[0],0 ,0 );//==>更新完不删档
                      }
                  }
              }
          }
          else
          {
              Boot_CreateJHTfile(_JHTFileCreate_,0 ) ;
          }
      }
      else
        Boot_UCBFirmwareUpdate( &TempFilename[0],0 ,2);
  }
}

unsigned char Boot_BackUp(unsigned char Ver, unsigned char Rel, unsigned char Build)
{
    unsigned char CheckOK = 1;
    unsigned long ProgramAddress = Init_Table_Addr&0x000FFFFF;
    const unsigned char* ptr = (const unsigned char*)Init_Table_Addr;
    unsigned char ConsoleVer[3] = {Ver, Rel, Build};
    Mx25L1606E_MajoritySectorErase(EEPRomPartition_StartAddress_Backup, EEPRomPartition_Size_Backup);
    Mx25L1606E_WriteBlock((EEPRomPartition_StartAddress_Backup+ProgramAddress),(EEPRomPartition_Size_Backup - _SectorBase_ - ProgramAddress) ,(uint8_t *)(BOOT_START_ADDRESS + ProgramAddress));
    WiFi_TargetLed(1);
    // Check
    for(int i=0; i<(EEPRomPartition_Size_Backup - _SectorBase_ - ProgramAddress); i++)
    {
        if(Mx25L1606E_ReadByte((EEPRomPartition_StartAddress_Backup+ProgramAddress)+i) != *(ptr+i))
        {
            CheckOK = 0;
            break; 
        }
    }
    if(CheckOK == 1)
    {
        // Back Up Information
        Mx25L1606E_WriteByte(EEPRomPartition_Information_StartAddress_Backup,_AlreadyBackUp_);
        Mx25L1606E_WriteBlock(EEPRomPartition_Information_StartAddress_Backup+1, 3, ConsoleVer);
        return CheckOK;
    }
    else
        return CheckOK;
    
}

void Boot_Initial_Recovery(void)
{
  unsigned short i ;
  
  HCC_RCC_AHB1ENR|=HCC_V(IOPA,1)|HCC_V(IOPB,1)|HCC_V(IOPC,1)|HCC_V(IOPD,1)|HCC_V(IOPE,1);
  // EEPRom
  
  // GPIO Initial Output PB12(CS), PB13(SK), PB14(DI); Input PB15(DO)  
  HCC_GPIO_MODER(B) &= ~(0xff000000); // DO Input floating  CS,SK,DI Output Pull-Up
  HCC_GPIO_MODER(B) |= (0x15000000);
  HCC_GPIO_OTYPER(B) &= ~(0x0000f000);
  HCC_GPIO_OSPEEDR(B) &= ~(0xff000000);
  HCC_GPIO_PUPDR(B) &= ~(0xff000000);
  /*
  // CoolDown Key, DA7 (D 9), SK2 (E 14)
  HCC_GPIO_CRH(D)&=~(0x000000f0); // PD9 Input floating
  HCC_GPIO_CRH(D)|= (0x00000040);
  HCC_GPIO_CRH(E)&=~(0x0F000000); // PE14 Output Pull-Up
  HCC_GPIO_CRH(E)|= (0x02000000);
  HCC_GPIO_BSRR(E)=(1u<<14); // Enable PE14 - set High
  */
  // Power On
  for( i = 0 ; i < 0xFFFF ;i++) ;
  
  HCC_GPIO_MODER(C) &= ~(0x00000c00); // PC5 push-pull output 
  HCC_GPIO_MODER(C) |= (0x00000400);
  HCC_GPIO_OTYPER(C) &= ~(0x00000020);
  HCC_GPIO_OSPEEDR(C) &= ~(0x00000c00);
  HCC_GPIO_PUPDR(C) &= ~(0x00000c00);
  HCC_GPIO_BSRR(C)=(1u<<5);      // Enable PC5 - set +12V on
  for( i = 0 ; i < 0xFFFF ;i++) ;
  
  HCC_GPIO_MODER(B) &= ~(0x00000c00); // PB5 push-pull output 
  HCC_GPIO_MODER(B) |= (0x00000400);
  HCC_GPIO_OTYPER(B) &= ~(0x00000020);
  HCC_GPIO_OSPEEDR(B) &= ~(0x00000c00);
  HCC_GPIO_PUPDR(B) &= ~(0x00000c00);
  HCC_GPIO_BSRR(B)=(1u<<5);   // Enable PB5 - set +5V on
  for( i = 0 ; i < 0xFFFF ;i++) ;
  
  // for LED Display IO  
  HCC_GPIO_MODER(E) &= ~(0x003fc000);// PE7~PB10 push-pull output 
  HCC_GPIO_MODER(E) |= (0x00154000);
  HCC_GPIO_OTYPER(E) &= ~(0x00000780);
  HCC_GPIO_OSPEEDR(E) &= ~(0x003fc000);
  HCC_GPIO_PUPDR(E) &= ~(0x003fc000); 
  WiFi_TargetLed(1); // LED Display  Fan_Green (Target Heart)
/*
  // Recovery
  if((GPIOD->IDR&0x0200)>>9 == 1)
        Boot_UCBFirmwareUpdate( &TempFilename[0],0 ,2);
  */
  //OE high
  HCC_GPIO_BSRR(E)=(1u<<7);
  // SDI low 
  HCC_GPIO_BSRR(E)=(1u<<26);
  for( i = 0 ; i < 0xFFFF ;i++) ;

}

//------------------------------------------------------------------------------
void main(void) 
{
  //
  Boot_NVIC_SetVectorTable(FLASH_BASE,0) ;
  /* Configure one bit for preemption priority */
  Boot_NVIC_PriorityGroupConfig(NVIC_PRIORITYGROUP_4);
  // Initial Check and Recovery
  Boot_Initial_Recovery();  
  //  
  if( Boot_CheckUserProgram() == _UserAPIOk_ )
    Boot_CallApplication(APP_START_ADDRESS) ;
  //
  Boot_usbmain() ;
}




#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


