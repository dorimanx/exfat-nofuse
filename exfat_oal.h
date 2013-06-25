/************************************************************************/
/*                                                                      */
/*  PROJECT : exFAT & FAT12/16/32 File System                           */
/*  FILE    : exfat_oal.h                                               */
/*  PURPOSE : Header File for exFAT OS Adaptation Layer                 */
/*            (Semaphore Functions & Real-Time Clock Functions)         */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*  NOTES                                                               */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*  REVISION HISTORY (Ver 0.9)                                          */
/*                                                                      */
/*  - 2010.11.15 [Joosun Hahn] : first writing                          */
/*                                                                      */
/************************************************************************/

#ifndef _EXFAT_OAL_H
#define _EXFAT_OAL_H

#include "exfat_config.h"
#include "exfat_global.h"
#include <linux/version.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  /*----------------------------------------------------------------------*/
	/*  Constant & Macro Definitions (Configurable)                         */
	/*----------------------------------------------------------------------*/

	/*----------------------------------------------------------------------*/
	/*  Constant & Macro Definitions (Non-Configurable)                     */
	/*----------------------------------------------------------------------*/

	/*----------------------------------------------------------------------*/
	/*  Type Definitions                                                    */
	/*----------------------------------------------------------------------*/

	typedef struct {
		UINT16      sec;        /* 0 ~ 59               */
		UINT16      min;        /* 0 ~ 59               */
		UINT16      hour;       /* 0 ~ 23               */
		UINT16      day;        /* 1 ~ 31               */
		UINT16      mon;        /* 1 ~ 12               */
		UINT16      year;       /* 0 ~ 127 (since 1980) */
	} TIMESTAMP_T;

	/*----------------------------------------------------------------------*/
	/*  External Function Declarations                                      */
	/*----------------------------------------------------------------------*/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37)
#define DECLARE_MUTEX(m) DEFINE_SEMAPHORE(m)
#endif

	INT32 sm_init(struct semaphore *sm);
	INT32 sm_P(struct semaphore *sm);
	void  sm_V(struct semaphore *sm);

	TIMESTAMP_T *tm_current(TIMESTAMP_T *tm);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _EXFAT_OAL_H */

/* end of exfat_oal.h */
