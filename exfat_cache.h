/************************************************************************/
/*                                                                      */
/*  PROJECT : exFAT & FAT12/16/32 File System                           */
/*  FILE    : exfat_cache.h                                             */
/*  PURPOSE : Header File for exFAT Cache Manager                       */
/*            (FAT Cache & Buffer Cache)                                */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*  NOTES                                                               */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*  REVISION HISTORY (Ver 0.9)                                          */
/*                                                                      */
/*  - 2010.11.15 [Sung-Kwan Kim] : first writing                        */
/*                                                                      */
/************************************************************************/

#ifndef _EXFAT_CACHE_H
#define _EXFAT_CACHE_H

#include "exfat_config.h"
#include "exfat_global.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  /*----------------------------------------------------------------------*/
	/*  Constant & Macro Definitions                                        */
	/*----------------------------------------------------------------------*/

#define LOCKBIT                 0x01
#define DIRTYBIT                0x02

	/*----------------------------------------------------------------------*/
	/*  Type Definitions                                                    */
	/*----------------------------------------------------------------------*/

	typedef struct __BUF_CACHE_T {
		struct __BUF_CACHE_T *next;
		struct __BUF_CACHE_T *prev;
		struct __BUF_CACHE_T *hash_next;
		struct __BUF_CACHE_T *hash_prev;
		INT32                drv;
		UINT32               sec;
		UINT32               flag;
		struct buffer_head   *buf_bh;
	} BUF_CACHE_T;

	/*----------------------------------------------------------------------*/
	/*  External Function Declarations                                      */
	/*----------------------------------------------------------------------*/

	INT32  buf_init(struct super_block *sb);
	INT32  buf_shutdown(struct super_block *sb);
	INT32 FAT_read(struct super_block *sb, UINT32 loc, UINT32 *content);
	void   FAT_write(struct super_block *sb, UINT32 loc, UINT32 content);
	UINT8 *FAT_getblk(struct super_block *sb, UINT32 sec);
	void   FAT_modify(struct super_block *sb, UINT32 sec);
	void   FAT_release_all(struct super_block *sb);
	void   FAT_sync(struct super_block *sb);
	UINT8 *buf_getblk(struct super_block *sb, UINT32 sec);
	void   buf_modify(struct super_block *sb, UINT32 sec);
	void   buf_lock(struct super_block *sb, UINT32 sec);
	void   buf_unlock(struct super_block *sb, UINT32 sec);
	void   buf_release(struct super_block *sb, UINT32 sec);
	void   buf_release_all(struct super_block *sb);
	void   buf_sync(struct super_block *sb);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _EXFAT_CACHE_H */

/* end of exfat_cache.h */
