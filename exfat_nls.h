/************************************************************************/
/*                                                                      */
/*  PROJECT : exFAT & FAT12/16/32 File System                           */
/*  FILE    : exfat_nls.h                                               */
/*  PURPOSE : Header File for exFAT NLS Manager                         */
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

#ifndef _EXFAT_NLS_H
#define _EXFAT_NLS_H

#include <linux/nls.h>

#include "exfat_config.h"
#include "exfat_global.h"
#include "exfat_api.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  /*----------------------------------------------------------------------*/
	/*  Constant & Macro Definitions                                        */
	/*----------------------------------------------------------------------*/

#define NUM_UPCASE              2918

#define DOS_CUR_DIR_NAME        ".          "
#define DOS_PAR_DIR_NAME        "..         "

#if (FFS_CONFIG_LITTLE_ENDIAN == 1)
#define UNI_CUR_DIR_NAME        ".\0"
#define UNI_PAR_DIR_NAME        ".\0.\0"
#else
#define UNI_CUR_DIR_NAME        "\0."
#define UNI_PAR_DIR_NAME        "\0.\0."
#endif

#define NLS_LOSSY_TOOSHORT							1
#define	NLS_LOSSY_TOOLONG								2
#define NLS_LOSSY_CHAR_TO_UNI_ERROR		3
#define NLS_LOSSY_ERROR									4 // generic error code

#define SET_LOSSY(lossy, value)			\
	do {														\
		if (lossy == 0)	lossy = value;			\
	} while(0)											\
 
	/*----------------------------------------------------------------------*/
	/*  Type Definitions                                                    */
	/*----------------------------------------------------------------------*/

	/* DOS name stucture */
	typedef struct {
		UINT8       name[DOS_NAME_LENGTH];
		UINT8       name_case;
	} DOS_NAME_T;

	/* unicode name stucture */
	typedef struct {
		UINT16      name[MAX_NAME_LENGTH];
		UINT16      name_hash;
		UINT8       name_len;
	} UNI_NAME_T;

	/*----------------------------------------------------------------------*/
	/*  External Function Declarations                                      */
	/*----------------------------------------------------------------------*/

	/* NLS management function */
	UINT16 nls_upper(struct super_block *sb, UINT16 a);
	INT32  nls_dosname_cmp(struct super_block *sb, UINT8 *a, UINT8 *b);
	INT32  nls_uniname_cmp(struct super_block *sb, UINT16 *a, UINT16 *b);
	void   nls_uniname_to_dosname(struct super_block *sb, DOS_NAME_T *p_dosname, UNI_NAME_T *p_uniname, INT32 *p_lossy);
	void   nls_dosname_to_uniname(struct super_block *sb, UNI_NAME_T *p_uniname, DOS_NAME_T *p_dosname);
	void   nls_uniname_to_cstring(struct super_block *sb, UINT8 *p_cstring, UNI_NAME_T *p_uniname);
	void   nls_cstring_to_uniname(struct super_block *sb, UNI_NAME_T *p_uniname, UINT8 *p_cstring, INT32 *p_lossy);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EXFAT_NLS_H */

/* end of exfat_nls.h */
