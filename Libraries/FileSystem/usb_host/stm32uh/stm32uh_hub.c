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
#include "hcc_types.h"
#include "os.h"
#include "stm32uh_hub.h"
#include "stm32_reg.h"
#include "stm32uh_hw.h"
#include "pmgr.h"

static void *hub_handler;
static hcc_u32 connected;

#define RX_FIFO_SIZE			(0x100/4)
#define NP_TX_FIFO_SIZE			(0x180/4)
#define P_TX_FIFO_SIZE			(0x280/4)

/*
** HUB ISR handler
*/
void stm32uh_hub_isr (void)
{
#if OS_INTERRUPT_ENABLE
  HCC_C(HCC_OTG_GINTMSK,HPRTINT);	/* disable further interrupts */
#endif
  connected=HCC_R(HCC_OTG_HPRT,PCSTS);
  (void)os_event_set_int(pmgr_event_bit);
}


/*
** Reset port
*/
static int stm32uh_hub_port_reset (void *dev_hdl, hcc_u8 port)
{
  hcc_u32 hprt;
  int DelayState=0;  //Jason 20100924
  
  int count = 0;

  (void)dev_hdl;
  (void)port;

  os_delay(200);			/* wait 200ms to stabilize power */
  HCC_C(HCC_OTG_HPRT,PENA);
  HCC_S(HCC_OTG_HPRT,PRST);
  os_delay(20);				/* wait 20ms */
  HCC_C(HCC_OTG_HPRT,PRST);

  HCC_OTG_GRXFSIZ=HCC_V(RXFD,RX_FIFO_SIZE);
  HCC_OTG_GNPTXFSIZ=HCC_V(NPTXFD,NP_TX_FIFO_SIZE)|HCC_V(NPTXFSA,RX_FIFO_SIZE);
  HCC_OTG_HPTXFSIZ=HCC_V(PTXFSIZ,P_TX_FIFO_SIZE)|HCC_V(PTXSA,RX_FIFO_SIZE+NP_TX_FIFO_SIZE);

  HCC_OTG_GRSTCTL=HCC_V(TXFNUM,0x10)|HCC_V(TXFFLSH,1);

  do
  {
      if (++count > 200000)
      {
          break;
      }
  } while(HCC_OTG_GRSTCTL & HCC_M(TXFFLSH));

  HCC_OTG_GRSTCTL=HCC_V(RXFFLSH,1);

  count = 0;
  
  do
  {
      if (++count > 200000)
      {
          break;
      }
  } while(HCC_OTG_GRSTCTL & HCC_M(RXFFLSH));
  
  hprt=HCC_OTG_HPRT;
  if (hprt&HCC_M(PENA))		/* port enabled (needs to be after reset) */
  {
    hprt=HCC_R(hprt,PSPD);	/* read port speed */
    if (hprt==HCC_PSPD_FULL)
    {
      HCC_OTG_HCFG=HCC_V(FSLSPCS,1)|HCC_V(FSLSS,1);
      HCC_OTG_HFIR=HCC_V(FRIVL,48000);
     DelayState= stm32uh_delay(100);		//Jason /* wait 100ms after reset */
      if (DelayState)return USBH_FULL_SPEED;
	  else   return USBH_SPEED_ERROR;		//20101220 Jason--
    }
    else if (hprt==HCC_PSPD_LOW)
    {
      HCC_OTG_HCFG=HCC_V(FSLSPCS,2)|HCC_V(FSLSS,1);
      HCC_OTG_HFIR=HCC_V(FRIVL,6000);
     DelayState= stm32uh_delay(100);	//Jason	/* wait 100ms after reset */
      if (DelayState)return USBH_LOW_SPEED;
	  else   return USBH_SPEED_ERROR;		//20101220 Jason--
      //return USBH_LOW_SPEED;
    }
  }
  return USBH_SPEED_ERROR;
}


/*
** Disable port
*/
static int stm32uh_hub_port_disable (void *dev_hdl, hcc_u8 port)
{
  (void)dev_hdl;
  (void)port;
  HCC_C(HCC_OTG_HPRT,PENA);
  return USBH_SUCCESS;
}


/*
** Suspend port
*/
static int stm32uh_hub_suspend (void *dev_hdl, hcc_u8 port)
{
  (void)dev_hdl;
  HCC_S(HCC_OTG_HPRT,PSUSP);		/* suspend */
  return USBH_SUCCESS;
}


/*
** Resume port
*/
static int stm32uh_hub_resume (void *dev_hdl, hcc_u8 port)
{
  (void)dev_hdl;
  //int DelayState=0;
  HCC_S(HCC_OTG_HPRT,PRES);			/* set resume */
  stm32uh_delay(20);	//DelayState= 			/* delay 20 ms */
  HCC_C(HCC_OTG_HPRT,PRES);			/* clear resume */
  while (HCC_OTG_HPRT&HCC_M(PSUSP))	/* wait while in suspended state */
    ;
  return USBH_SUCCESS;
}


/*
** Check device connect,disconnect,overcurrent.
*/
static int stm32uh_hub_scan (void *dev_hdl, t_hub_scan_inf *hsi)
{
  int rc=0;
  hcc_u32 hprt=HCC_OTG_HPRT;
  static hcc_u8 cstate=0;


  if ((hprt&(HCC_M(PCDET)|HCC_M(POCCHNG))) || (HCC_R(hprt,PCSTS)!=cstate))
  {
    hsi->state=0;
    hsi->port=0;
    rc=1;

    if (hprt&HCC_M(POCCHNG))
    {
      hsi->state=USBH_STATE_OVERCURRENT;
    }
    else
    {
      if (hprt&HCC_M(PCSTS))
      {
        hsi->state=USBH_STATE_CONNECTED;
        cstate=1;
      }
      else
      {
        hsi->state=USBH_STATE_DISCONNECTED;
        cstate=0;
      }
    }
  }

  /* clear interrupts. For some reason PENA needs to be set to 0 in order to properly clear interrupts */
  /* if set and port was enabled, the port will become disabled. The documentation says PENA can only */
  /* be cleared by the SW in order to disable the port but not in this particular case. HW bug??? */
  HCC_OTG_HPRT=hprt&~HCC_M(PENA);
#if OS_INTERRUPT_ENABLE
  HCC_S(HCC_OTG_GINTMSK,HPRTINT);	/* enable port interrupt */
#endif

  return rc;
}


/*
** Initialize root HUB
*/
int stm32uh_hub_init (void)
{
  connected=0;
  HCC_C(HCC_OTG_HPRT,PPWR);			/* disable power */
  hub_handler=pmgr_add_hub(NULL, stm32uh_hub_scan, stm32uh_hub_port_reset, stm32uh_hub_port_disable, stm32uh_hub_suspend, stm32uh_hub_resume);
  if (hub_handler==NULL) return USBH_ERR_HOST;
  return USBH_SUCCESS;
}


/*
** Start root HUB
*/
int stm32uh_hub_start (void)
{
  HCC_S(HCC_OTG_HPRT,PPWR);			/* enable power */
#if OS_INTERRUPT_ENABLE
  HCC_S(HCC_OTG_GINTMSK,HPRTINT);	/* enable interrupt */
#endif
  return USBH_SUCCESS;
}


/*
** Stop root HUB
*/
int stm32uh_hub_stop (void)
{
  HCC_C(HCC_OTG_HPRT,PPWR);			/* disable power */
#if OS_INTERRUPT_ENABLE
  HCC_C(HCC_OTG_GINTMSK,HPRTINT);	/* disable interrupt */
#endif
  return USBH_SUCCESS;
}


/*
** Checks if device is connected
*/
int stm32uh_hub_connected (void)
{
 //20110112 By Jason -- return connected;

 //20110112 By Jason --{
 
 
    if (HCC_R(HCC_OTG_HPRT,PCSTS)!=1)
	connected=0;
   return connected;
  // else return 0;
   //20110112 By Jason--}
}

