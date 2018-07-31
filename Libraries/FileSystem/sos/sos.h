/****************************************************************************
 *
 *            Copyright (c) 2003 by HCC Embedded
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
 * Budapest 1132
 * Victor Hugo Utca 11-15
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/
#ifndef __SOS_H
#define __SOS_H

#include <stdio.h>
#include <string.h>
#include "sos_conf.h"
#include "arg.h"
#include "cmd.h"
#include "../fat/src/common/fat.h"
#include "UserDefine.h"

#define SOS_KEY_CR 13
#define SOS_KEY_LF 10
#define SOS_KEY_SPACE ' '
#define SOS_KEY_TAB '\t'
#define SOS_KEY_BACKSPACE 8
#define SOS_KEY_CTRL_A 1
#define SOS_KEY_CTRL_Z 26

void sos_init (void);
void sos_run (void);
void sos_msg (char *msg);

#endif