/************************************************************************/
/*                                                                      */
/*  PROJECT : exFAT & FAT12/16/32 File System                           */
/*  FILE    : exfat_data.c                                              */
/*  PURPOSE : exFAT Configuable Data Definitions                        */
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

#include "exfat_config.h"
#include "exfat_global.h"
#include "exfat_data.h"
#include "exfat_oal.h"

#include "exfat_blkdev.h"
#include "exfat_cache.h"
#include "exfat_nls.h"
#include "exfat_super.h"
#include "exfat.h"

/*======================================================================*/
/*                                                                      */
/*                    GLOBAL VARIABLE DEFINITIONS                       */
/*                                                                      */
/*======================================================================*/

/*----------------------------------------------------------------------*/
/*  File Manager                                                        */
/*----------------------------------------------------------------------*/

/* file system volume table */
FS_STRUCT_T fs_struct[MAX_DRIVE];

#if 0
/*----------------------------------------------------------------------*/
/*  Buffer Manager                                                      */
/*----------------------------------------------------------------------*/

/* FAT cache */
DECLARE_MUTEX(f_sem);
BUF_CACHE_T FAT_cache_array[FAT_CACHE_SIZE];
BUF_CACHE_T FAT_cache_lru_list;
BUF_CACHE_T FAT_cache_hash_list[FAT_CACHE_HASH_SIZE];

/* buf cache */
DECLARE_MUTEX(b_sem);
BUF_CACHE_T buf_cache_array[BUF_CACHE_SIZE];
BUF_CACHE_T buf_cache_lru_list;
BUF_CACHE_T buf_cache_hash_list[BUF_CACHE_HASH_SIZE];
#endif

/* end of exfat_data.c */
