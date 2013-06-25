/************************************************************************/
/*                                                                      */
/*  PROJECT : exFAT & FAT12/16/32 File System                           */
/*  FILE    : exfat_data.h                                              */
/*  PURPOSE : Header File for exFAT Configuable Constants               */
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

#ifndef _EXFAT_DATA_H
#define _EXFAT_DATA_H

#include "exfat_config.h"
#include "exfat_global.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  /*======================================================================*/
	/*                                                                      */
	/*                        FFS CONFIGURATIONS                            */
	/*                  (CHANGE THIS PART IF REQUIRED)                      */
	/*                                                                      */
	/*======================================================================*/

	/* max number of block devices                      */
#define MAX_DEVICE              2

	/* max number of volumes on all block devices       */
#define MAX_DRIVE               2

	/* max number of open files                         */
#define MAX_OPEN                20

	/* max number of root directory entries in FAT12/16 */
	/* (should be an exponential value of 2)            */
#define MAX_DENTRY              512

	/* cache size (in number of sectors)                */
	/* (should be an exponential value of 2)            */
#define FAT_CACHE_SIZE          128
#define FAT_CACHE_HASH_SIZE     64
#define BUF_CACHE_SIZE          256
#define BUF_CACHE_HASH_SIZE     64

	/* default mount options                            */
#define DEFAULT_CODEPAGE        437
#define DEFAULT_IOCHARSET       "utf8"

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EXFAT_DATA_H */

/* end of exfat_data.h */
