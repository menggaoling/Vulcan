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
#include "stm32_reg.h"
#include "stm32uh_hub.h"
#include "pmgr.h"
#include "os.h"
#include "stm32uh.h"
// 2018.04.26
#include "stm32f2xx_hal.h"
// 

#define MAX_RETRIES				3

/* timeout in ms for channel response, if no response within this time then treat transfer as erroneous */
#define CHANNEL_TIMEOUT			10

/* non periodic transfers can only be scheduled if at least NP_FT_REMAIN clock cycles are left in the frame */
/* this is required because if a non-periodic transfer doesn't fit in the current frame and will be executed in */
/* the next frame then all periodic transfers scheduled for the next frame will generate frame overrun error */
#define NP_FT_REMAIN			0x1000

/* number of channels available for transfers */
#define TRANSFER_CHANNELS		1

/* Minimum poll interval for interrupt endpoints */
#define MIN_INTERVAL			10



#define ISO_BASE				0
#define INT_BASE				MAX_ISO_TRANSFERS
#define NP_BASE					(INT_BASE+MAX_INT_TRANSFERS)


#define CONTROL_EP				(MAX_DEVICE+1)
#define TOTAL_EP				(CONTROL_EP+MAX_EP)
#define TOTAL_TRANSFERS			(MAX_NP_TRANSFERS+MAX_INT_TRANSFERS+MAX_ISO_TRANSFERS)


#define TR_ST_STOPPED			0
#define TR_ST_WAITING			1
#define TR_ST_RUNNING			2
#define TR_ST_TIMEOUT     3     // Added by Jason 20110101--

#define SCHED_INT_OFF			0
#define SCHED_INT_ON			1


/* FIFO */
typedef struct {
  hcc_u8 r,w;
  void *ptr[TOTAL_EP];
} t_fifo;


/* transfer descriptor */
typedef struct {
  hcc_u8 ch;			/* transfer channel */
  hcc_u8 ppos;			/* pending position */

  hcc_u8 dir;			/* current direction */
  hcc_u8 retry;			/* no. retries */
  hcc_u8 nak;			/* last transfer was NAK-ed */
  hcc_u16 ch_timeout;	/* channel timeout */
  hcc_u32 timeout;		/* timeout */

  hcc_u16 npkt;
  hcc_u32 tsize;		/* transfer size */
  hcc_u32 cframe;		/* current frame */

  hcc_u32 hcchar;		/* HCCHAR register value */
  hcc_u32 hctsiz;		/* HCTSIZE register value */
} t_td;


/* endpoint descriptor */
typedef struct {
  hcc_u8 used;			/* used entry */

  hcc_u8 daddr;			/* device address */
  hcc_u8 ep;			/* endpoint address */
  hcc_u8 type;			/* endpoint type */
  hcc_u8 dir;			/* direction */
  hcc_u16 psize;		/* packet size */
  hcc_u8 interval;		/* poll interval */

  hcc_u8 speed;			/* speed */
  hcc_u8 hub_daddr;		/* HUB address */

  hcc_u32 nframe;		/* no. frames till next transfer - for INT endpoints */
  hcc_u8 pid;			/* curent PID value */

  t_usbh_transfer *uht;	/* transfer */
  t_td td;				/* current transfer - describes the channel */
} t_ed;


/* UHT driver area */
typedef struct {
  volatile hcc_u32 state;	/* state of the transfer */
  t_usbh_transfer *nuht;	/* next uht - for ISO transfers */
} t_uht_da;

/************************************************************************************************************************/
/********************************************** Local variables *********************************************************/
/************************************************************************************************************************/

static t_ed ed[TOTAL_EP];					/* endpoint descriptor */
static t_ed *ch_ed[TRANSFER_CHANNELS];		/* channel to ed */
static t_ed *pending_ed[TOTAL_TRANSFERS];	/* pending ED list (NP/ISO/INT) */
static hcc_uint np_pending_ed_cnt;			/* non periodic pending ED count */
static hcc_uint sched_int_state;			/* schedule state */
static t_fifo c_uht;						/* completed UHT */

/***** mutexes and events *****/
static OS_MUTEX_TYPE *stm32uh_mutex;
static OS_EVENT_BIT_TYPE stm32uh_event_bit;

/************************************************************************************************************************/

static void _stm32uh_transfer_start (t_ed *aed, hcc_u8 ch);
static void _stm32uh_transfer_init (t_ed *aed);

/************************************************************************************************************************/

/*
** Push to FIFO
** Write can't overwrite unread data because these functions are used for completed transfers
** and it can't have more elements than TOTAL_EP (if RTOS is used transfer task is always
** the highest priority, so another transfer can't be started before handling the completed transfer)
*/
static void _stm32uh_push (t_fifo *fifo, void *ptr)
{
  fifo->ptr[fifo->w]=ptr;
  if (++fifo->w==TOTAL_EP) fifo->w=0;
}

/*
** Pop from FIFO
*/
static void *_stm32uh_pop (t_fifo *fifo)
{
  void *ptr=fifo->ptr[fifo->r];
  if (ptr)
  {
    fifo->ptr[fifo->r]=NULL;
    if (++fifo->r==TOTAL_EP) fifo->r=0;
  }
  return ptr;
}


/*
** Free channel - set waiting state for the belonging transfer
*/
static void _stm32uh_free_channel (hcc_u8 ch)
{
  ch_ed[ch]->td.ch=(hcc_u8)USBH_INVALID;
  ((t_uht_da *)ch_ed[ch]->uht->driver_area)->state=TR_ST_WAITING;	/* put transfer to waiting state */
  ch_ed[ch]=NULL;				/* free channel */
  HCC_Cb(HCC_OTG_HAINTMSK,ch);	/* disable interrupt generation for channel */
  HCC_OTG_HCCHAR(ch)=0;
}


/*
** Remove transfer
** Input: ch - channel
*/
static void _stm32uh_remove_transfer (hcc_u8 ch)
{
  t_ed *aed=ch_ed[ch];
  t_usbh_transfer *uht=aed->uht;
//  u32 D_Loop = 0;// 2013.11.26
  
  HCC_OTG_HCCHAR(ch)=HCC_M(CHENA)|HCC_M(CHDIS);	/* disable channel */
//  while ((HCC_OTG_HCINT(ch)&HCC_M(CHH))==0)		/* wait till channel is disabled */
//  {
//      // 2013.11.26
//      D_Loop++;
//      if(D_Loop >= 0xFFFF)
//      {
//          D_Loop = 0;
//          SCB->AIRCR = 0x05fa0004 ; // Software Reset
//          break;
//      }
//      //
//  }
  HCC_OTG_HCINT(ch)=HCC_OTG_HCINT(ch);			/* clear pending interrupts */
  if (aed->td.dir!=USBH_DIR_IN)
  {												/* flush TxFIFO */
    if (aed->type==USBH_TYPE_ISO) HCC_OTG_GRSTCTL=HCC_V(TXFNUM,1)|HCC_V(TXFFLSH,1);
                             else HCC_OTG_GRSTCTL=HCC_V(TXFFLSH,1);
  }
  else
  {
    HCC_OTG_GRSTCTL=HCC_V(RXFFLSH,1);			/* flush RxFIFO */
  }
  _stm32uh_free_channel(ch);
  pending_ed[aed->td.ppos]=NULL;				/* remove from pending list */
  if (aed->td.ppos>=NP_BASE) --np_pending_ed_cnt;

  uht->rc=USBH_ERR_TRANSFER;					/* stop transfer and mark it erroneous */
  aed->uht=NULL;
  _stm32uh_push(&c_uht,uht);
}


/*
** schedule transfers
*/
static hcc_uint _stm32uh_schedule (hcc_uint *np_pos)
{
  hcc_uint rc=0;
  hcc_u8 ch;
  hcc_uint pos=ISO_BASE;
  hcc_uint n;

  for (ch=0;ch_ed[ch] && ch<TRANSFER_CHANNELS;ch++)
    ;
  if (ch<TRANSFER_CHANNELS)
  {
    for (n=0; n<TOTAL_TRANSFERS && ch<TRANSFER_CHANNELS; n++)
    {
      if (pending_ed[pos] && pending_ed[pos]->nframe==0 && pending_ed[pos]->td.ch==(hcc_u8)USBH_INVALID)
      {
        if (((t_uht_da *)(pending_ed[pos]->uht->driver_area))->state==TR_ST_WAITING)
        {
          for (;ch_ed[ch] && ch<TRANSFER_CHANNELS;ch++)
            ;
          if (ch==TRANSFER_CHANNELS) break;				/* no more channels available */
          if (pending_ed[pos]->td.nak==0) _stm32uh_transfer_init(pending_ed[pos]);
          _stm32uh_transfer_start(pending_ed[pos],ch);
          ++rc;
          ++ch;
        }
      }

      /* get next pending position to check, periodic (ISO/INT) transfers are processed in order and */
      /* non-periodic transfers are processed as a ring buffer to equally distribute process time*/
      ++pos;
      if (pos==MAX_ISO_TRANSFERS+MAX_INT_TRANSFERS) pos=*np_pos;
      if (pos==TOTAL_TRANSFERS) pos=NP_BASE;
    }

    if (pos>=NP_BASE)
    {
      *np_pos=pos;
    }
  }

  return rc;
}


/*
** STM32 USBH interrupt handler
*/
OS_ISR_FN(stm32uh_isr_handler)
{
  static hcc_uint np_pos=NP_BASE;		/* non-periodic position */
  static hcc_uint p_sched_n=0;			/* periodic schedule required */

  t_ed *aed;
  hcc_u32 aframe;
  hcc_u32 rframe;
  hcc_u32 sts;
  hcc_u8 se=0;

  sts=HCC_OTG_GINTSTS;

  rframe=HCC_OTG_HFNUM;
  aframe=HCC_R(rframe,FRNUM);			/* get current frame number */
  rframe=HCC_R(rframe,FTREM);			/* get remaining time in the current frame */

  if (sts&HCC_M(HPRTINT))
  {
    stm32uh_hub_isr();					/* host port interrupt */
  }

  if (stm32uh_hub_connected())				/* check if connected, unfortunately if a device is unplugged when */
  {										/* a transfer is active, channel will not indicate this. */
    sched_int_state=SCHED_INT_ON;

    /* SOF - check periodic list */
  if (sts&HCC_M(SOF))
  // if ((sts&HCC_M(SOF))&&(HCC_R(HCC_OTG_HPRT,PCSTS)));//--2010101112 By Jason 
    {
      hcc_u32 msk;
      hcc_u8 ch;
      hcc_uint i;

      for (i=0;i<MAX_ISO_TRANSFERS+MAX_INT_TRANSFERS;i++)
      {									/* go through periodic transfers and check if any needs to be scheduled */
        hcc_uint pos=ISO_BASE+i;
        aed=pending_ed[pos];
        if (aed)
        {
          if (aed->nframe && --aed->nframe==0)
          {
            /* needs to be xecuted in the current frame, so set odd/even execution accordingly */
            if (aframe&1) HCC_S(aed->td.hcchar,ODDFRM);
                     else HCC_C(aed->td.hcchar,ODDFRM);
            ++p_sched_n;				/* increase no. periodic schedules required */
          }
        }
      }

      /* check for timed out transfers - msk contains enabled channels */
      for (msk=HCC_OTG_HAINTMSK,ch=0;msk;msk>>=1,ch++)
      {
        aed=ch_ed[ch];
        /* if transfer is active for this endpoint and timed out */
        if (aed && aed->td.ch_timeout)
        {
          if (--aed->td.ch_timeout==0)
          {
            /* disable channel and retry transfer - next will be a CHH interrupt that will retry
               the transfer or return with an error */
            HCC_OTG_HCCHAR(ch)=HCC_M(CHENA)|HCC_M(CHDIS);
          }
        }
      }

      HCC_OTG_GINTSTS=HCC_V(SOF,1);					/* clear SOF interrupt */
    }

    /* host channel interrupt */
   if (sts&HCC_M(HCINT))

    {
      hcc_u8 hcint_check=0;
      hcc_u8 ch=0;
      hcc_u32 cch=HCC_OTG_HAINT;
      hcc_u32 hcint;
      hcc_u32 stsr=0;
      t_usbh_transfer *uht;
      int rc;

      for (;;)
      {
        if (sts&HCC_M(RXFLVL))			/* RxFIFO non-empty, go through it first */
        {
          stsr=HCC_OTG_GRXSTSP;			/* get RX status */
          sts=HCC_OTG_GINTSTS;
          ch=HCC_R(stsr,CHNUM);
        }
        else							/* if RxFIFO empty then remaining channels */
        {								/* can be processed */
          if (hcint_check==0)
          {
            hcint_check=1;
            ch=0;
            cch=HCC_OTG_HAINT;
          }
          else
          {
            ++ch;
            cch>>=1;
          }
          for (;cch && (cch&1)==0 && ch<TRANSFER_CHANNELS;ch++,cch>>=1)
            ;
          if (!cch) break;
        }

        hcint=HCC_OTG_HCINT(ch);
        HCC_OTG_HCINT(ch)=hcint;		/* clear interrupt */
        aed=ch_ed[ch];
        uht=aed->uht;
        rc=USBH_ERR_NOT_COMPLETE;

        if (hcint&HCC_M(XFRC))			/* transfer completed */
        {
          if (aed->td.dir!=USBH_DIR_IN)	/* if not OUT transfer */
          {
            if (aed->type!=USBH_TYPE_ISO)
            {							/* set new PID for non-ISO transfers */
              if (aed->td.npkt&1) aed->pid^=1;
            }
            uht->csize+=aed->td.tsize;
            uht->buf+=aed->td.tsize;	/* increase completed size and buffer pointer */
          }

          if (aed->type==USBH_TYPE_ISO)
          {
            aed->nframe=1;				/* set next execution time for ISO transfers */
          }
          if (uht->csize==uht->size || uht->rc!=USBH_ERR_NOT_COMPLETE)
          {								/* transfer finished */
            if (uht->rc==USBH_ERR_NOT_COMPLETE) rc=USBH_SUCCESS;
                                           else rc=uht->rc;
          }
          else
          {								/* not finished - evaluate rest of the channels */
            _stm32uh_free_channel(ch);
            if (aed->type==USBH_TYPE_ISO && uht->iso_pcnt) --uht->iso_pcnt;
            continue;
          }
        }
		
     
         else if(aed->td.dir==USBH_DIR_IN && (hcint&HCC_M(ACK))) 		 
        {								/* IN ACK-ed transfer */
          hcc_u16 size=HCC_R(stsr,BCNT);
          hcc_uint i;

          aed->pid^=1;							/* save PID */
          for (i=0;i<(size>>2);i++) 			/* retrieve data in 32 bit chunks */
          {
            *(hcc_u32 *)(uht->buf+(i<<2))=*(HCC_OTG_DFIFO(ch)+i);
          }
          i<<=2;
          if (i<size)							/* if remaining data */
          {
            hcc_u32 data=*(HCC_OTG_DFIFO(ch)+(i>>2));
            hcc_u8 *_data=(hcc_u8 *)&data;
            while (i<size)
            {
              *(uht->buf+i++)=*_data++;			/* retrieve it */
            }
          }
          aed->td.hctsiz=HCC_OTG_HCTSIZ(ch);	/* save new HCTSIZ register value */
          uht->csize+=size;						/* increase completed size */
          uht->buf+=size;

          /* read size < packet size and transfer is not complete => short packet */
          if (size<aed->psize && uht->csize!=uht->size)
          {
            uht->rc=USBH_SHORT_PACKET;
          }

          /* data left to read */
          /* currently not used because one transfer can not exceed max. packet size
          else if (HCC_R(aed->td.hctsiz,XFRSIZ))
          {
            HCC_OTG_HCCHAR(ch)=HCC_OTG_HCCHAR(ch);
          }
          */
	
          continue;
        }
         
        else if (hcint&HCC_M(NAK))					/* NAK-ed transfer */
        {
          if (aed->type!=USBH_TYPE_INT)				/* not INT transfer */
          {
            if (aed->td.timeout!=USBH_WAIT_FOREVER)	/* so, check timeout */
            {
              hcc_u32 d;
              if (aframe<aed->td.cframe) aframe+=HCC_FRNUM_m+1;
              d=aframe-aed->td.cframe;
              if (d<aed->td.timeout)
              {
                aed->td.timeout-=d;
                aed->td.cframe=aframe;
              }
              else
              {
                rc=USBH_ERR_TIMEOUT;				/* timeout error */
              }
            }

            if (rc!=USBH_ERR_TIMEOUT)
            {
              if (aed->td.dir!=USBH_DIR_IN)			/* not IN transfer */
              {										/* so, need to disable channel */
//                HCC_OTG_HCCHAR(ch)=HCC_M(CHENA)|HCC_M(CHDIS);	/* and retry transfer */
//                aed->td.nak=1;						/* set nak flag, next interrupt will be CHH */
                  _stm32uh_transfer_start(aed,ch);	/* restart transfer */
              }
              else
              {
                /* stop current transfer if others are pending or not enough time in the current frame */
//                if (p_sched_n+np_pending_ed_cnt>TRANSFER_CHANNELS || rframe<NP_FT_REMAIN)
//                {
//                  aed->td.nak=1;					/* set NAK flag */
//                  HCC_OTG_HCCHAR(ch)=HCC_M(CHENA)|HCC_M(CHDIS);	/* disable channel */
//                }
//                else
                {
                  _stm32uh_transfer_start(aed,ch);	/* restart transfer */
                }
              }
              continue;
            }
          }
          else
          {
            HCC_OTG_HCCHAR(ch)=HCC_M(CHENA)|HCC_M(CHDIS);	/* disable channel */
            aed->td.nak=1;									/* set nak flag to indicate it for the next */
            continue;										/* CHH interrupt and continue */
          }
        }
        else if (hcint&HCC_M(CHH))				/* erroneous transfer */
        {
          if (aed->td.dir!=USBH_DIR_IN)
          {										/* flush TxFIFO */
            if (aed->type==USBH_TYPE_ISO) HCC_OTG_GRSTCTL=HCC_V(TXFNUM,1)|HCC_V(TXFFLSH,1);
                                     else HCC_OTG_GRSTCTL=HCC_V(TXFFLSH,1);
          }

          if (aed->td.nak)						/* disabled because of a NAK */
          {
            if (aed->type==USBH_TYPE_INT)		/* INT transfer */
            {
              aed->nframe=aed->interval;		/* so, reset frame counter */
            }
            else
            {
              /* restart transfer only if enough channels available and enough time remains */
              if (p_sched_n+np_pending_ed_cnt<=TRANSFER_CHANNELS && rframe>=NP_FT_REMAIN)
              {
                _stm32uh_transfer_start(aed,ch);/* otherwise, restart transfer if enough channels */
              }
              else
              {
                _stm32uh_free_channel(ch);
              }
              continue;
            }
          }
          else
          {
            /* transfer error occured - restart if error was not stall and retries didn't expire */
            if ((hcint&HCC_M(STALL)) || --aed->td.retry==0)
            {
              rc=USBH_ERR_TRANSFER;				/* transfer error */
            }
            else
            {
              _stm32uh_transfer_start(aed,ch);	/* restart transfer */
              continue;
            }
          }
        }

        /* remove transfer if completed or not completed but INT transfer*/
        if (rc!=USBH_ERR_NOT_COMPLETE || aed->td.nak)	
        {
          _stm32uh_free_channel(ch);
        }

        if (rc!=USBH_ERR_NOT_COMPLETE)
        {
          aed->uht=((t_uht_da *)(aed->uht->driver_area))->nuht;
          if (aed->uht==NULL)					/* there is no next transfer */
          {
            if (aed->td.ppos>=NP_BASE) --np_pending_ed_cnt;		/* decrease no. NP transfers if needed */
            pending_ed[aed->td.ppos]=NULL;						/* remove from pending list if no more transfers are pending */
          }
          uht->rc=rc;
          _stm32uh_push(&c_uht,uht);	/* put UHT to completed FIFO */
          se=1;							/* send event to transfer task if callback required */
        }
      }
    }

    if (p_sched_n || np_pending_ed_cnt)
    {
      /* only schedule new transfers if enough time remains in the current frame */
      if (rframe>NP_FT_REMAIN)
      {
        hcc_uint n_sched;
        n_sched=_stm32uh_schedule(&np_pos);
        p_sched_n=(n_sched>p_sched_n)?0:p_sched_n-n_sched;
      }
    }
    else if (HCC_OTG_HAINTMSK==0)
    {
      sched_int_state=SCHED_INT_OFF;
    }
  }
 else
  {
    hcc_u32 msk;
    hcc_u8 ch;
    /* go through all active channels disable them and mark transfer erroneous */
    for (msk=HCC_OTG_HAINTMSK,ch=0;msk;msk>>=1,ch++)
    {
      _stm32uh_remove_transfer(ch);
      se=1;
    }

    sched_int_state=SCHED_INT_OFF;
    //HCC_OTG_GINTSTS=HCC_V(SOF,1);					/* clear SOF interrupt */
  }

//20110112 By Jason --{
 if (sched_int_state==SCHED_INT_ON)	
 	{
	if (!stm32uh_hub_connected())	
		{
		 hcc_u32 msk;
    		hcc_u8 ch;
   		 /* go through all active channels disable them and mark transfer erroneous */
    		for (msk=HCC_OTG_HAINTMSK,ch=0;msk;msk>>=1,ch++)
    		{
      		_stm32uh_remove_transfer(ch);
     		 se=1;
   		 }
	//	connected=0;
   		 sched_int_state=SCHED_INT_OFF;
		}
 	}

 //--20110112 By Jason --}

  if (se)
  {
    (void)os_event_set(1);
  }
}


/*
** Add endpoint (adds QueueHead to the list).
** INPUT: daddr - device address
**        ep - endpoint number
**        type - USBH_TYPE_CONTROL / USBH_TYPE_BULK / USBH_TYPE_INT / USBH_TYPE_ISO
**        dir - USBH_DIR_ANY / USBH_DIR_SETUP / USBH_DIR_OUT / USBH_DIR_IN
**        psize - maximum packet size
**        interval - poll interval for periodic endpoints
** RETURN: NULL - error / on success pointer to the endpoint
*/
void *stm32uh_add_ep (hcc_u8 daddr, hcc_u8 ep, hcc_u8 type, hcc_u8 dir, hcc_u16 psize, hcc_u8 interval)
{
  void *rc=NULL;
  hcc_uint i,z,f;
  hcc_u8 hub_daddr=0;	/* HUB address */
  hcc_u8 speed=0;
  hcc_u8 _type;
  t_ed *aed;

  if (stm32uh_hw_state()==USBH_ERR_SUSPENDED) return NULL;

  switch (type)
  {
    case USBH_TYPE_CONTROL: _type=HCC_EPTYP_CONTROL; break;
    case USBH_TYPE_ISO:     _type=HCC_EPTYP_ISO; break;
    case USBH_TYPE_BULK:    _type=HCC_EPTYP_BULK; break;
    case USBH_TYPE_INT:     _type=HCC_EPTYP_INTERRUPT; break;
    default: return NULL;
  }

  if (os_mutex_get(stm32uh_mutex)==OS_SUCCESS)
  {
    rc=NULL;
    z=f=TOTAL_EP;
    for (i=0;i<TOTAL_EP;i++)
    {
      if (ed[i].used)
      {
        if (ed[i].daddr==daddr)
        {
          /*if (ed[i].ep==ep && ed[i].dir==dir) break;*/
          if (ed[i].ep==0) z=i;
        }
      }
      else if (f==TOTAL_EP) f=i;
    }

    if (i==TOTAL_EP && f<TOTAL_EP)
    {
      aed=ed+f;
      rc=aed;

      aed->used=1;
      aed->daddr=daddr;
      aed->ep=ep;
      aed->type=type;
      aed->dir=dir;
      aed->psize=psize;
      if (interval<2) interval=MIN_INTERVAL;/* set minimum interval if required */
      aed->interval=interval;				/* save endpoint informarion */
      if (z<TOTAL_EP)						/* save hub address and speed if available */
      {
        aed->hub_daddr=hub_daddr;
        aed->speed=speed;
      }
      aed->nframe=0;
      aed->pid=0;							/* reset PID */

      aed->uht=NULL;
      aed->td.hcchar=HCC_V(CHENA,1u)|HCC_V(DAD,daddr)|HCC_V(EPTYP,_type)|HCC_V(EPNUM,ep)|HCC_V(MPSIZ,psize);
      if (speed==USBH_LOW_SPEED) aed->td.hcchar|=HCC_V(LSDEV,1);
    }

    (void)os_mutex_put(stm32uh_mutex);
  }

  return rc;
}



/*
** Remove all endpoints belonging to a device address
** INPUT: daddr - device address
** RERURN: USBH_SUCCESS
*/
int stm32uh_del_daddr (hcc_u8 daddr)
{
  int rc=USBH_ERR_NOT_AVAILABLE;
  t_ed *aed;
  hcc_uint i,j;

  if (stm32uh_hw_state()==USBH_ERR_SUSPENDED) return USBH_ERR_SUSPENDED;

  if (os_mutex_get(stm32uh_mutex)==OS_SUCCESS)
  {
#if OS_INTERRUPT_ENABLE
    HCC_C(HCC_OTG_GAHBCFG,GINT);/* disable interrupts to avoid parallel handling of the endpoint(s) from interrupt */
#endif
    for (i=0;i<TOTAL_EP;i++)
    {
      aed=ed+i;
      if (aed->daddr==daddr)	/* this endpoint needs to be deleted */
      {
        /* ongoing transfer, need to stop */
        if (aed->uht && ((t_uht_da *)aed->uht->driver_area)->state!=TR_ST_STOPPED)
        {
          t_usbh_transfer *uht;
          hcc_u8 ch;

          if (((t_uht_da *)aed->uht->driver_area)->state==TR_ST_RUNNING)
          {
            ch=aed->td.ch;
            _stm32uh_remove_transfer(ch);
            uht=_stm32uh_pop(&c_uht);
          }
          else
          {
            uht=aed->uht;
          }
          ((t_uht_da *)(uht->driver_area))->state=TR_ST_STOPPED;	/* set stopped state */
          if (uht->cb_fn)
          {
            (uht->cb_fn)(uht->cb_param,USBH_ERR_TRANSFER);
          }
        }

        for (j=0;j<TOTAL_TRANSFERS && pending_ed[j]!=aed;j++)
          ;
        if (j<TOTAL_TRANSFERS)
        {
          if (j>=NP_BASE) --np_pending_ed_cnt;
          pending_ed[j]=NULL;
        }
        aed->used=0;
      }
    }
#if OS_INTERRUPT_ENABLE
    HCC_S(HCC_OTG_GAHBCFG,GINT);
#endif
    (void)os_mutex_put(stm32uh_mutex);
  }
  else rc=USBH_ERR_RESOURCE;

  return rc;
}



/*
** Clear EP toggle value
** INPUT: ep_handle - endpoint handle
** RETURN: USBH_ERR_NOT_AVAILABLE - endpoint not found
**         USBH_SUCCESS
*/
int stm32uh_clear_toggle (void *ep_handle)
{
  int rc=USBH_ERR_NOT_AVAILABLE;

  if ((t_ed *)ep_handle>=ed && (t_ed *)ep_handle<(ed+TOTAL_EP))
  {
    if (os_mutex_get(stm32uh_mutex)==OS_SUCCESS)
    {
      ((t_ed *)ep_handle)->pid=0;
      (void)os_mutex_put(stm32uh_mutex);
      rc=USBH_SUCCESS;
    }
    else rc=USBH_ERR_RESOURCE;
  }

  return rc;
}



/*
** Start transfer
*/
static void _stm32uh_transfer_start (t_ed *aed, hcc_u8 ch)
{
  ch_ed[ch]=aed;								/* save ed associated to the allocated channel */
  aed->td.nak=0;
  aed->td.ch=ch;
  aed->td.ch_timeout=CHANNEL_TIMEOUT;			/* reset channel timeout */
  HCC_OTG_HCTSIZ(ch)=aed->td.hctsiz;
  HCC_OTG_HCCHAR(ch)=aed->td.hcchar;			/* start transfer */
  ((t_uht_da *)aed->uht->driver_area)->state=TR_ST_RUNNING;		/* set running transfer state */
  HCC_Sb(HCC_OTG_HAINTMSK,ch);					/* enable interrupt generation for channel i */
  if (aed->td.dir!=USBH_DIR_IN)
  {												/* write data to FIFO */
    hcc_uint i;
    for (i=0;i<(aed->td.tsize+3)>>2;i++) *(HCC_OTG_DFIFO(ch)+i)=*(hcc_u32 *)(aed->uht->buf+(i<<2));
  }
}


/*
** Init transfer
*/
static void _stm32uh_transfer_init (t_ed *aed)
{
  t_usbh_transfer *uht=aed->uht;
  hcc_u8 dpid;

  if (aed->type==USBH_TYPE_ISO && uht->iso_pcnt)
  {											/* calculate transfer size */
    aed->td.tsize=uht->iso_psize;
  }
  else
  {
    if (uht->size-uht->csize>aed->psize) aed->td.tsize=aed->psize;
                                    else aed->td.tsize=uht->size-uht->csize;
  }
  aed->td.timeout=uht->timeout;				/* set timeout */
  aed->td.retry=MAX_RETRIES;				/* reset retry counter */

  if (aed->td.dir==USBH_DIR_SETUP)
  {											/* set PID and DIR */
    dpid=HCC_DPID_SETUP;
    HCC_C(aed->td.hcchar,EPDIR);
  }
  else
  {
    if (aed->td.dir==USBH_DIR_IN) HCC_S(aed->td.hcchar,EPDIR);
                             else HCC_C(aed->td.hcchar,EPDIR);
    if (aed->pid==0) dpid=HCC_DPID_DATA0;
                else dpid=HCC_DPID_DATA1;
  }

  if (aed->td.tsize>aed->psize)				/* set no. packets */
    aed->td.npkt=(aed->td.tsize+aed->psize-1)/aed->psize;
  else
    aed->td.npkt=1;

  /* HCCHAR is precalculated in add EP and EPDIR is set above and save HCTSIZ */
  aed->td.hctsiz=HCC_V(DPID,dpid)|HCC_V(PKTCNT,aed->td.npkt)|HCC_V(XFRSIZ,aed->td.tsize);
  aed->td.cframe=HCC_R(HCC_OTG_HFNUM,FRNUM);/* save current frame for timeuot calculation */
}


/*
** Transfer task
*/
OS_TASK_FN(stm32uh_transfer_task)
{
  hcc_u32 TimeOut=0x10000;      // Added by Jason 20110101--
#if OS_TASK_POLL_MODE==0
  for (;;)
#endif
  {
    if (os_event_get(stm32uh_event_bit)==OS_SUCCESS)
    {
      t_usbh_transfer *uht;
      uht=_stm32uh_pop(&c_uht);
      while (uht)							/* while completed transfers */
      {
        //
        TimeOut-- ;      // Added by Jason 20110101--
        //
        t_uht_da *uht_da=(t_uht_da *)uht->driver_area;
        if (uht_da->nuht==NULL)
        {
          uht_da->state=TR_ST_STOPPED;		/* set stopped state if no more transfers */
          TimeOut=0x10000;      // Added by Jason 20110101--
        }
        // Added by Jason 20110101--
        if( TimeOut == 0 )
            {
            uht_da->state=TR_ST_TIMEOUT;	/* set running state if time out */
            }
        //
        if (uht->cb_fn)						/* execute callback if needed */
        {
          (uht->cb_fn)(uht->cb_param,uht->rc);
          TimeOut=0x10000;      // Added by Jason 20110101--
        }
        uht=_stm32uh_pop(&c_uht);			/* get next completed transfer */

      }
    }
	//201101017 By Jason--{
 else
 	{

	os_event_set(2);
      
 	}
	//20110117 By Jason --}
  }
}



/*
** Get status of a transfer
** INPUT: uht - transfer descriptor
**        mode - USBH_MODE_BLOCK/USBH_MODE_NON_BLOCK
** RETURN:USBH_SUCCESS
**        USBH_SHORT_PACKET
**        USBH_ERR_TRANSFER
**        USBH_ERR_TIMEOUT
**        USBH_ERR_NOT_COMPLETE
*/
int stm32uh_transfer_status (t_usbh_transfer *uht, hcc_u8 mode)
{
  int rc=USBH_SUCCESS;
  t_uht_da *uht_da=(t_uht_da *)uht->driver_area;
  hcc_u32		temp=0;
 
  for (;;)
  {
  //__printf1("uht_da->state:%d\r\n",uht_da->state);
  // __printf0("\r\n\r\nstm32uh_transfer_status\r\n");
  //temp--;
#if OS_TASK_POLL_MODE
    stm32uh_transfer_task();
#endif
    if (uht_da->state==TR_ST_STOPPED)		/* transfer finished */
    {
      rc=uht->rc;
	  //temp=0xffff;
      break;
    }

    if (mode==USBH_MODE_NON_BLOCK)
    {
      rc=USBH_ERR_NOT_COMPLETE;
	  //temp=0xffff;
      break;
    }
    // Added by Jason 20110101--
    #if 1
    if (uht_da->state==TR_ST_TIMEOUT)
        {
        return USBH_ERR_TIMEOUT;
        }
    else
        {
        temp += 1 ;
        if( temp > 0x50000 )
            return USBH_ERR_TIMEOUT;  
        }
    #endif
    //Jason	
  }

  return rc;
}


/*
** Start transfer
** INPUT: uht - transfer descriptor
**        mode - USBH_MODE_BLOCK/USBH_MODE_NON_BLOCK
** RETURN:
*/
int stm32uh_transfer (t_usbh_transfer *uht, hcc_u8 mode)
{
  int rc=USBH_SUCCESS;
  t_ed *aed;
  t_uht_da *uht_da=(t_uht_da *)(uht->driver_area);

  if (stm32uh_hw_state()==USBH_ERR_SUSPENDED) return USBH_ERR_SUSPENDED;
  if (stm32uh_hub_connected()==0) return USBH_ERR_TRANSFER;

  uht->csize=0;
  uht->rc=USBH_ERR_NOT_COMPLETE;
  uht_da->nuht=NULL;

  if (uht->eph==NULL) uht->eph=ed;
  aed=uht->eph;

  if (aed>=ed && aed<ed+TOTAL_EP)
  {
    if (aed->uht==NULL || aed->type==USBH_TYPE_ISO)
    {
      if (uht->size==USBH_SIZE_DEFAULT) uht->size=aed->psize;

#if OS_INTERRUPT_ENABLE
      /* disable interrupts - to avoid skip of this transfer for ISO endpoints and to avoid scheduling 2 places.
         If active transfer is last in the ISO chain and the completion interrupt occurs while adding nuht then
         this transfer will never be executed. Non periodic transfers will be scheduled from this function if
         scheduling is not active from the interrupt to check this sched_int_state needs to be compared to SCHED_NONE. */
      HCC_C(HCC_OTG_GAHBCFG,GINT);
#endif
      if (os_mutex_get(stm32uh_mutex)==OS_SUCCESS)
      {
        if (aed->uht==NULL)				/* there is no active transfer for this endpoint */
        {
          hcc_uint b,n,i;

          aed->uht=uht;
          aed->td.dir=(aed->dir==USBH_DIR_ANY)?uht->dir:aed->dir;
          if (uht->toggle!=USBH_TOGGLE_PRESERVE) aed->pid=uht->toggle;

          /* get start position and no. elements for this type of transfer */
          if (aed->type==USBH_TYPE_INT)
          {
            aed->nframe=aed->interval;	/* set frames left for execution */
            b=INT_BASE;
            n=MAX_INT_TRANSFERS;
          }
          else if (aed->type==USBH_TYPE_ISO)
          {
            if (uht->timeout==USBH_ISO_IMMEDIATE) aed->nframe=1;
                                             else aed->nframe=uht->timeout;
            b=ISO_BASE;
            n=MAX_ISO_TRANSFERS;
          }
          else
          {
            aed->nframe=0;
            b=NP_BASE;
            n=MAX_NP_TRANSFERS;
          }

          for (i=0;i<n && pending_ed[i+b];i++)
            ;											/* look for an empy position in the pending list */
          if (i<n)
          {
            pending_ed[b+i]=aed;						/* add ED to pending list */
            aed->td.ppos=b+i;							/* save position in pending list */
            aed->td.ch=(hcc_u8)USBH_INVALID;

            _stm32uh_transfer_init(aed);				/* initialize transfer - to take load of the interrupt */
            aed->td.nak=1;							/* and set NAK field to avoid reiinitialization from interrupt */

            if (b==NP_BASE)							/* non periodic transfer */
            {
              ++np_pending_ed_cnt;					/* increase number of elements in NP list */
              /* schedule non periodic transfer if interrupt is not scheduling and enough time remains */
              if (HCC_R(HCC_OTG_HFNUM,FTREM)>NP_FT_REMAIN && sched_int_state==SCHED_INT_OFF)
              {										/* so, need to schedule from here */
                hcc_u8 ch;
                for (ch=0;ch<TRANSFER_CHANNELS && ch_ed[ch];ch++)
                  ;									/* look for an empty position in ch_ed */
                if (ch<TRANSFER_CHANNELS)
                {
                  _stm32uh_transfer_start(aed,ch);	/* start transfer */
                  sched_int_state=SCHED_INT_ON;
                }
              }
              else
              {
                uht_da->state=TR_ST_WAITING;		/* can't start transfer now -> set waiting state */
              }
            }
            else
            {
              uht_da->state=TR_ST_WAITING;
            }
#if OS_INTERRUPT_ENABLE
            HCC_S(HCC_OTG_GAHBCFG,GINT);
#endif
            (void)os_mutex_put(stm32uh_mutex);

            if (rc==USBH_SUCCESS && mode==USBH_MODE_BLOCK)
            {
              rc=stm32uh_transfer_status(uht,mode);
            }
          }
          else
          {
#if OS_INTERRUPT_ENABLE
            HCC_S(HCC_OTG_GAHBCFG,GINT);
#endif
            (void)os_mutex_put(stm32uh_mutex);
            rc=USBH_ERR_TRANSFER_FULL;
          }
        }
        else							/* there is an active transfer for this endpoint */
        {								/* => set nuht */
          t_usbh_transfer *nuht=aed->uht;
          while (((t_uht_da *)(nuht->driver_area))->nuht) nuht=((t_uht_da *)(nuht->driver_area))->nuht;
          ((t_uht_da *)(nuht->driver_area))->nuht=uht;
          uht_da->state=TR_ST_WAITING;
#if OS_INTERRUPT_ENABLE
          HCC_S(HCC_OTG_GAHBCFG,GINT);
#endif
          (void)os_mutex_put(stm32uh_mutex);
        }
      }
      else rc=USBH_ERR_RESOURCE;
    }
    else rc=USBH_ERR_ONGOING_TRANSFER;
  }
  else rc=USBH_ERR_INVALID;

  return rc;
}


/*
** Delay ms number of milliseconds
*/
/*void stm32uh_delay (hcc_u32 ms)
{
  hcc_u32 frnum,afrnum;
 
  ++ms;
  frnum=HCC_R(HCC_OTG_HFNUM,FRNUM);
  while (ms)
  {
    afrnum=HCC_R(HCC_OTG_HFNUM,FRNUM);
    if (afrnum!=frnum)
    {
      --ms;
      frnum=afrnum;
    }
  }
}*/
// Jason
int stm32uh_delay (hcc_u32 ms)
{
  hcc_u32 frnum,afrnum;
  hcc_u32 temp=0xffff;	//Jason
  //hcc_u32 i=0;
  
  ++ms;
  frnum=HCC_R(HCC_OTG_HFNUM,FRNUM);
  while (ms)
  {
    afrnum=HCC_R(HCC_OTG_HFNUM,FRNUM);
  temp--;		//Jason
  if(temp==0)
      break;
    if (afrnum!=frnum)
    {
      --ms;
      temp=0xffff;	//Jason
      frnum=afrnum;
    }
  }
  //20101220 Jason--
  if (temp==0) 
    return DealyTimeOut;
  else return DealyTimeOK;
 // return DealyTimeOK;
   //--20101220 Jason
}



/*
** Set speed,HUB address and HUB speed for endpoint0
*/
int stm32uh_set_ep0_speed_hub (void *ep0_handle, hcc_u8 speed, hcc_u8 port, hcc_u8 hub_daddr, hcc_u8 hub_speed)
{
  int rc=USBH_ERR_NOT_AVAILABLE;
  t_ed *aed;

  (void)port;
  (void)hub_speed;

  if (stm32uh_hw_state()==USBH_ERR_SUSPENDED) return USBH_ERR_SUSPENDED;

  if (ep0_handle) aed=(t_ed *)ep0_handle;
             else aed=ed;
  if (aed>=ed && aed<(ed+TOTAL_EP))
  {
    aed->speed=speed;
    aed->hub_daddr=hub_daddr;
    rc=USBH_SUCCESS;
  }

  return rc;
}



/*
** Get info of an ep_handle
**
** Input: ep_handle - endpoint handler
** Output: daddr - device address
**         ep - endpoint number
**         type - endpoint type
**         dir - endpoint dir
**         psize - default packet size
** Return: USBH_SUCCESS / USB_ERR_NOT_AVAILABLE / USBH_ERR_DIR
*/
int stm32uh_get_ep_inf (void *ep_handle, hcc_u8 *daddr, hcc_u8 *ep, hcc_u8 *type, hcc_u8 *dir, hcc_u16 *psize)
{
  int rc=USBH_SUCCESS;
  t_ed *aed=(t_ed *)ep_handle;

  if (aed>=ed && aed<(ed+TOTAL_EP))
  {
    if (daddr) *daddr=aed->daddr;
    if (ep) *ep=aed->ep;
    if (type) *type=aed->type;
    if (dir) *dir=aed->dir;
    if (psize) *psize=aed->psize;
  }
  else rc=USBH_ERR_NOT_AVAILABLE;

  return rc;
}




/*
** Init STM32
*/
int stm32uh_init (void)
{
  int rc;

  rc=stm32uh_hw_init();/* init STM32 HW */
  if (rc) return rc;

  if (os_mutex_create(&stm32uh_mutex)) return USBH_ERR_RESOURCE;
  if (os_event_create(&stm32uh_event_bit)) return USBH_ERR_RESOURCE;

  rc=stm32uh_hub_init();
  if (rc) return rc;

  return USBH_SUCCESS;
}



/*
** Start STM32 host controller
*/
int stm32uh_start (hcc_u32 p)
{
  int rc;
  hcc_uint i;

  (void)p;
  _memset(ed,0,sizeof(ed));
  _memset(ch_ed,0,sizeof(ch_ed));
  _memset(&pending_ed,0,sizeof(pending_ed));
  _memset(&c_uht,0,sizeof(c_uht));
  sched_int_state=SCHED_INT_OFF;
  np_pending_ed_cnt=0;

  rc=stm32uh_hw_start();	/* init STM32 HW */
  if (rc) return rc;

//  HCC_S(HCC_OTG_GCCFG,NOVBUSSENS);  /*Disable vbus sense*/
  HCC_OTG_HCFG=HCC_V(FSLSPCS,1)|HCC_V(FSLSS,1);

  for (i=0;i<TRANSFER_CHANNELS;i++)					/* enable successful and abnormal completion */
  {													/* interrupts for all channels */
    HCC_OTG_HCINTMSK(i)=HCC_M(NAK)|HCC_M(ACK)|HCC_M(XFRC)|HCC_M(CHH);
  }

  HCC_S(HCC_OTG_GINTMSK,HCINT);						/* enable HCINT interrupts */
  HCC_S(HCC_OTG_GINTMSK,RXFLVL);					/* enable RXFLVL interrupts */
  HCC_S(HCC_OTG_GINTMSK,SOF);						/* enable SOF interrupts */
#if OS_INTERRUPT_ENABLE
  HCC_S(HCC_OTG_GAHBCFG,GINT);
#endif

  if (stm32uh_add_ep(0,0,USBH_TYPE_CONTROL,USBH_DIR_ANY,8,0)==NULL)
  {													/* add general control endpoint */
    return USBH_ERR_HOST;
  }

  rc=stm32uh_hub_start();							/* start root HUB */
  if (rc) return rc;


  rc=os_isr_init(STM32UH_HOST_ISR, stm32uh_isr_handler);
  if (rc) return rc;

  rc=os_isr_enable(STM32UH_HOST_ISR);
  if (rc) return rc;

  return USBH_SUCCESS;
}



/*
** Stop STM32 controller
*/
int stm32uh_stop (void)
{
  int rc;

  rc=os_isr_disable(STM32UH_HOST_ISR);				/* disable interrupts */
  if (rc) return rc;

  rc=stm32uh_hub_stop();							/* stop HUB */
  if (rc) return rc;

  return USBH_SUCCESS;
}

