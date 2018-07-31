/**
  ******************************************************************************
  * @file    stm32f10x_flash.c
  * @author  MCD Application Team
  * @version V3.2.0
  * @date    07/06/2018
  * @brief   This file provides all the FLASH firmware functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "Bootflash.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup FLASH 
  * @brief FLASH driver modules
  * @{
  */ 

/** @defgroup FLASH_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Defines
  * @{
  */ 

/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask         ((uint32_t)0x00000038)
#define ACR_HLFCYA_Mask          ((uint32_t)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((uint32_t)0xFFFFFFEF)

/* Flash Access Control Register bits */
#define ACR_PRFTBS_Mask          ((uint32_t)0x00000020) 

/* Flash Control Register bits */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE) 
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_SER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)


/* FLASH Mask */
#define RDPRT_Mask               ((uint32_t)0x00000002)
#define WRP0_Mask                ((uint32_t)0x000000FF)
#define WRP1_Mask                ((uint32_t)0x0000FF00)
#define WRP2_Mask                ((uint32_t)0x00FF0000)
#define WRP3_Mask                ((uint32_t)0xFF000000)

/* FLASH Keys */
#define RDP_Key                  ((uint16_t)0x00A5)
//#define FLASH_KEY1               ((uint32_t)0x45670123)
//#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

/* Delay definition */   
#define EraseTimeout             ((uint32_t)0x00B80000)//0x000B0000
#define ProgramTimeout           ((uint32_t)0x00002000)

/*Sector 0~3 16K, Sector 4 64K,Sector 5~11 128K,*/
#define MID_SECTOR_SADDR         0x08010000
#define BIG_SECTOR_SADDR         0x08020000
#define SMALL_SECTOR_SIZE        16384          //16K
#define BIG_SECTOR_SIZE          131072         //128K

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Variables
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_FunctionPrototypes
  * @{
  */
  
/**
  * @}
  */

/** @defgroup FLASH_Private_Functions
  * @{
  */


/**
  * @brief  Unlocks the FLASH Program Erase Controller.
  * @param  None
  * @retval None
  */
void BootFLASH_Unlock(void)
{
  /* Authorize the FPEC Access */
  if((FLASH->CR & FLASH_CR_LOCK) != RESET)
  {
    /* Authorize the FLASH Registers access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }
}

/**
  * @brief  Locks the FLASH Program Erase Controller.
  * @param  None
  * @retval None
  */
void BootFLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the FCR */
  FLASH->CR |= CR_LOCK_Set;
}

/**
  * @brief  Erases a specified FLASH page.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: HAL_BUSY, 
  *   HAL_ERROR, HAL_OK or HAL_TIMEOUT.
  */
HAL_StatusTypeDef BootFLASH_EraseSector(uint32_t Sector)
{
  HAL_StatusTypeDef status = HAL_OK;
  /* Check the parameters */
  assert_param(IS_FLASH_SECTOR(Sector));
  /* Wait for last operation to be completed */
  status = BootFLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == HAL_OK)
  { 
    /* If the previous operation is completed, proceed to erase the sector */
    CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
    FLASH->CR |= FLASH_PSIZE_WORD;      //VoltageRange = FLASH_VOLTAGE_RANGE_3,so flash size = FLASH_PSIZE_WORD
    CLEAR_BIT(FLASH->CR, FLASH_CR_SNB);
    FLASH->CR |= FLASH_CR_SER | (Sector << POSITION_VAL(FLASH_CR_SNB));
    FLASH->CR |= FLASH_CR_STRT;
    
    /* Wait for last operation to be completed */
    status = BootFLASH_WaitForLastOperation(EraseTimeout);
    if(status != HAL_TIMEOUT)
    {
      /* if the erase operation is completed, disable the SER Bit */
      FLASH->CR &= CR_SER_Reset;
    }
  }
  /* Return the Erase Status */
  return status;
}



/**
  * @brief  Programs a half word at a specified address.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: 
  *   HAL_ERROR, HAL_OK or HAL_TIMEOUT. 
  */
HAL_StatusTypeDef BootFLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  HAL_StatusTypeDef status = HAL_OK;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));
  /* Wait for last operation to be completed */
  status = BootFLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == HAL_OK)
  {
    /* if the previous operation is completed, proceed to program the new data */
    FLASH->CR &= (~FLASH_CR_PSIZE_Msk);
    FLASH->CR |= CR_PG_Set;
    FLASH->CR |= FLASH_PSIZE_HALF_WORD;
  
    *(__IO uint16_t*)Address = Data;
    /* Wait for last operation to be completed */
    status = BootFLASH_WaitForLastOperation(ProgramTimeout);
    if(status != HAL_TIMEOUT)
    {
      /* if the program operation is completed, disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
  } 
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Clears the FLASH’s pending flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:         
  *     @arg FLASH_FLAG_PGERR: FLASH Program error flag       
  *     @arg FLASH_FLAG_WRPRTERR: FLASH Write protected error flag      
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag           
  * @retval None
  */
void BootFLASH_ClearFlag(uint16_t FLASH_FLAG)
{
  /* Check the parameters */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;
  
  /* Clear the flags */
  FLASH->SR = FLASH_FLAG;
}

/**
  * @brief  Returns the FLASH Status.
  * @param  None
  * @retval FLASH Status: The returned value can be: HAL_BUSY, 
  *   HAL_ERROR or HAL_OK
  */
HAL_StatusTypeDef BootFLASH_GetStatus(void)
{
  HAL_StatusTypeDef flashstatus = HAL_OK;
  
  if((FLASH->SR & FLASH_FLAG_BSY) != RESET) 
  {
    flashstatus = HAL_BUSY;
  }
  else if((FLASH->SR & FLASH_FLAG_EOP) != RESET)
  {  
    flashstatus = HAL_OK;
  }  
  else if((FLASH->SR & (FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | \
    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR)) != RESET )
  {
    flashstatus = HAL_ERROR;
  }
  
  /* Return the Flash Status */
  return flashstatus;
}

/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @param  Timeout: FLASH progamming Timeout
  * @retval FLASH Status: The returned value can be: 
  *   HAL_ERROR, HAL_OK or HAL_TIMEOUT.
  */
HAL_StatusTypeDef BootFLASH_WaitForLastOperation(uint32_t Timeout)
{ 
  HAL_StatusTypeDef status = HAL_OK;
   
  /* Check for the Flash Status */
  status = BootFLASH_GetStatus();
  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == HAL_BUSY) && (Timeout != 0x00))
  {
    status = BootFLASH_GetStatus();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = HAL_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}

uint8_t BootFLASH_GetSector(uint32_t Addr)
{
  uint8_t Sectors = 0;

  if(Addr < MID_SECTOR_SADDR)//sector size == 16K
  {
    Sectors = (Addr & 0x000FFFFF)/SMALL_SECTOR_SIZE ;
  }
  else if(Addr < BIG_SECTOR_SADDR)//sector size == 64K
  {
    Sectors = FLASH_SECTOR_4;
  }
  else //sector size == 128K
  {
    Sectors = FLASH_SECTOR_4 + (Addr & 0x000FFFFF)/BIG_SECTOR_SIZE;
  }
  return Sectors;
}
/**
  * @}
  */
HAL_StatusTypeDef BootFLASH_EarseUserProgram(uint32_t SAddr,uint32_t EAddr)
{ 
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t DataCounter ;
  __IO uint8_t EndOfSector ;
  //
  DataCounter = BootFLASH_GetSector(SAddr);//Start sector

  /* Define the number of page to be erased */
  EndOfSector = BootFLASH_GetSector(EAddr) ;//End of sector
  
  /* Clear All pending flags */
  BootFLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |\
                      FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR| FLASH_FLAG_PGSERR);  	
  status = HAL_OK ;
  /* Erase the user program FLASH sector */
  for(; (DataCounter <= EndOfSector) && (status == HAL_OK); DataCounter++)
  {
    status = BootFLASH_EraseSector( DataCounter );
  }
  
  /* Return the operation status */
  return status;
}




/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
