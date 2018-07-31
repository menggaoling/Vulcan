/***************************************************************************
 *
 *            Copyright (c) 2003-2008 by HCC Embedded
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

#ifndef _USB_MST_H_
#define _USB_MST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hcc_types.h"

hcc_u8 usb_mst_state (void *dev_hdl);
int usb_mst_transfer(void *dev_hdl, hcc_u8 cmd_length, void *cmd, hcc_u8 dir, hcc_u32 buf_length, void *buf);
int usb_mst_init (void);
int usb_mst_suspend (hcc_u8 dev);
int usb_mst_resume (hcc_u8 dev);

#ifdef __cplusplus
}
#endif

#endif
/****************************** END OF FILE **********************************/
