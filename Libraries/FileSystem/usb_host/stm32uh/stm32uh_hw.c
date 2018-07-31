/****************************************************************************
 *
 *            Copyright (c) 2003-2009 by HCC Embedded
 *
 * This software is copyrighted by and is the sole property of
 * HCC.  All rights, title, ownership, or other interests
 * in the software remain the property of HCC.  This
 * software may only be used in accordance with the corresponding
 * license agreement.  Any unauthorized use, duplication, transmission,
 * distribution, or disclosure of this software is expressly forbidden.
 *
 * This Copyright notice may not be removed or modified without prior
 * written consent of HCC.
 *
 * HCC reserves the right to modify this software without notice.
 *
 * HCC Embedded
 * Budapest 1133
 * Vaci Ut 110
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/
#include "usb_host.h"
#include "stm32uh_hw.h"
#include "stm32_reg.h"

//#define   EVM

static int _suspend=USBH_SUCCESS;

int stm32uh_hw_init (void)
{
  /**USB_OTG_FS GPIO Configuration    
    PA11     ------> USB_OTG_FS_DM
    PA12     ------> USB_OTG_FS_DP 
    */
  HCC_GPIO_MODER(A) &= ~(0x03c00000);            /* PA11 PA12 AF mode*/
  HCC_GPIO_MODER(A) |= (0x02800000);
  HCC_GPIO_OSPEEDR(A) &= ~(0x03c00000);            /* PA11 PA12 High speed*/
  HCC_GPIO_OSPEEDR(A) |= (0x03c00000);
  HCC_GPIO_PUPDR(A) &= ~(0x03c00000);            /* PA11 PA12 no pull*/
  HCC_GPIO_AFRH(A) &= ~(0x000ff000);            /* PA11 PA12 OTG FS function*/
  HCC_GPIO_AFRH(A) |= (0x000aa000);
  
  HCC_C(HCC_RCC_AHB2ENR,OTGFSEN);		  /* stop OTG FS clock */
  HCC_W(HCC_RCC_PLLCFGR,PLLQ,5);                  /* set OTG prescaler PLLQ to /5 -> 48MHz Main PLL */			  
  HCC_S(HCC_RCC_AHB2ENR,OTGFSEN);		  /* start OTG FS clock */
  HCC_S(HCC_RCC_APB2ENR,SYSCFGEN);              /*enable system confgig controller clock */
  asm("nop");                                   /* Delay after an RCC peripheral clock enabling */ \
  HCC_S(HCC_RCC_AHB2RSTR,OTGFSRST);		/* reset OTG module */
  HCC_C(HCC_RCC_AHB2RSTR,OTGFSRST);		/* get OTG out of reset */
#ifdef  EVM
  HCC_RCC_AHB1ENR|=HCC_V(IOPA,1)|HCC_V(IOPC,1);	/* enable PORTA, PORTC */
  HCC_GPIO_MODER(C) &= ~(0x00000300);
  HCC_GPIO_MODER(C) |= (0x00000100);
  HCC_GPIO_OTYPER(C) &= ~(0x00000010);
  HCC_GPIO_OSPEEDR(C) &= ~(0x00000300);
  HCC_GPIO_PUPDR(C) &= ~(0x00000300);            /* PC4 push-pull output */
#else  
  HCC_S(HCC_OTG_GUSBCFG,FHMOD) ;                  /* Set Force host mode */
  HCC_RCC_AHB1ENR|=HCC_V(IOPA,1)|HCC_V(IOPE,1) ;	/* enable PORTA,PORTE */
  HCC_GPIO_MODER(E) &= ~(0x00000c00);            /* PE5 push-pull output */
  HCC_GPIO_MODER(E) |= (0x00000400);
  HCC_GPIO_OTYPER(E) &= ~(0x00000020);
  HCC_GPIO_OSPEEDR(E) &= ~(0x00000c00);
  HCC_GPIO_PUPDR(E) &= ~(0x00000c00);		       
  HCC_GPIO_BSRR(E)=((1u<<5)<<16);		          /* Disable PE5 - Reset VBUS on */
  //
#endif  
  return 0;
}


int stm32uh_hw_start (void)
{
  HCC_S(HCC_OTG_GCCFG,PWRDWN);			    /* deactivate power down */
#ifdef  EVM
  HCC_GPIO_BSRR(C)=((1u<<9)<<16);		    /* reset PC9 - set VBUS on */
#else
  HCC_GPIO_BSRR(E)=((1u<<5));	          /* Enable PE5 - set VBUS on */
#endif  
  return 0;
}

int stm32uh_hw_stop (void)
{
  return 0;
}

void stm32uh_hw_resume (void)
{
  _suspend=USBH_SUCCESS;
}

void stm32uh_hw_suspend (void)
{
  _suspend=USBH_ERR_SUSPENDED;
}

int stm32uh_hw_state (void)
{
  return _suspend;
}

