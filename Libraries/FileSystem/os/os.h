/****************************************************************************
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
#ifndef _OS_H
#define _OS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hcc_types.h"
#include "os_nrtos.h"

#define OS_INTERRUPT_ENABLE		0 // 1 Jason
#define OS_TASK_POLL_MODE		  1

enum {
  OS_SUCCESS,
  OS_ERR_MUTEX,
  OS_ERR_EVENT,
  OS_ERR
};

int os_init (void);
void os_delay (hcc_u32 ms);

int os_isr_init (hcc_u32,void(*ih)(void));
int os_isr_enable (hcc_u32);
int os_isr_disable (hcc_u32);

int os_mutex_create(OS_MUTEX_TYPE **pmutex);
int os_mutex_get(OS_MUTEX_TYPE *pmutex);
int os_mutex_put(OS_MUTEX_TYPE *pmutex);

int os_event_create(OS_EVENT_BIT_TYPE *event_bit);
int os_event_get(OS_EVENT_BIT_TYPE event_bit);
int os_event_set(OS_EVENT_BIT_TYPE event_bit);
int os_event_set_int(OS_EVENT_BIT_TYPE event_bit);

void *os_lock_buffer (void *ptr, hcc_u32 *size);
void os_unlock_buffer (void *ptr, hcc_u32 size);
int os_buffer_init (void);


#ifdef __cplusplus
}
#endif

#endif

