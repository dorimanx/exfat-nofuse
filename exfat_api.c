/*
 *  Copyright (C) 2012-2013 Samsung Electronics Co., Ltd.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/************************************************************************/
/*                                                                      */
/*  PROJECT : exFAT & FAT12/16/32 File System                           */
/*  FILE    : exfat_api.c                                               */
/*  PURPOSE : exFAT API Glue Layer                                      */
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

#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>

#include "exfat_version.h"
#include "exfat_config.h"
#include "exfat_data.h"
#include "exfat_oal.h"

#include "exfat_nls.h"
#include "exfat_api.h"
#include "exfat_super.h"
#include "exfat_core.h"

/*----------------------------------------------------------------------*/
/*  Constant & Macro Definitions                                        */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*  Global Variable Definitions                                         */
/*----------------------------------------------------------------------*/

extern FS_STRUCT_T      fs_struct[];

extern struct semaphore z_sem;

/*----------------------------------------------------------------------*/
/*  Local Variable Definitions                                          */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*  Local Function Declarations                                         */
/*----------------------------------------------------------------------*/

/*======================================================================*/
/*  Global Function Definitions                                         */
/*    - All functions for global use have same return value format,     */
/*      that is, FFS_SUCCESS on success and several FS error code on    */
/*      various error condition.                                        */
/*======================================================================*/

/*----------------------------------------------------------------------*/
/*  exFAT Filesystem Init & Exit Functions                              */
/*----------------------------------------------------------------------*/

s32 FsInit(void)
{
	s32 i;

	/* initialize all volumes as un-mounted */
	for (i = 0; i < MAX_DRIVE; i++) {
		fs_struct[i].mounted = FALSE;
		fs_struct[i].sb = NULL;
		sm_init(&(fs_struct[i].v_sem));
	}

	return ffsInit();
}

s32 FsShutdown(void)
{
	s32 i;

	/* unmount all volumes */
	for (i = 0; i < MAX_DRIVE; i++) {
		if (!fs_struct[i].mounted)
			continue;

		ffsUmountVol(fs_struct[i].sb);
	}

	return ffsShutdown();
}

/*----------------------------------------------------------------------*/
/*  Volume Management Functions                                         */
/*----------------------------------------------------------------------*/

/* FsMountVol : mount the file system volume */
s32 FsMountVol(struct super_block *sb)
{
	s32 err, drv;

	sm_P(&z_sem);

	for (drv = 0; drv < MAX_DRIVE; drv++) {
		if (!fs_struct[drv].mounted)
			break;
	}

	if (drv >= MAX_DRIVE)
		return FFS_ERROR;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[drv].v_sem));

	err = buf_init(sb);
	if (!err)
		err = ffsMountVol(sb, drv);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[drv].v_sem));

	if (!err) {
		fs_struct[drv].mounted = TRUE;
		fs_struct[drv].sb = sb;
	} else {
		buf_shutdown(sb);
	}

	sm_V(&z_sem);

	return err;
} /* end of FsMountVol */

/* FsUmountVol : unmount the file system volume */
s32 FsUmountVol(struct super_block *sb)
{
	s32 err;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	sm_P(&z_sem);

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsUmountVol(sb);
	buf_shutdown(sb);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	fs_struct[p_fs->drv].mounted = FALSE;
	fs_struct[p_fs->drv].sb = NULL;

	sm_V(&z_sem);

	return err;
} /* end of FsUmountVol */

/* FsGetVolInfo : get the information of a file system volume */
s32 FsGetVolInfo(struct super_block *sb, VOL_INFO_T *info)
{
	s32 err;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of pointer parameters */
	if (info == NULL)
		return FFS_ERROR;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsGetVolInfo(sb, info);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsGetVolInfo */

/* FsSyncVol : synchronize a file system volume */
s32 FsSyncVol(struct super_block *sb, s32 do_sync)
{
	s32 err;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsSyncVol(sb, do_sync);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsSyncVol */


/*----------------------------------------------------------------------*/
/*  File Operation Functions                                            */
/*----------------------------------------------------------------------*/

/* FsCreateFile : create a file */
s32 FsLookupFile(struct inode *inode, char *path, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of pointer parameters */
	if ((fid == NULL) || (path == NULL) || (*path == '\0'))
		return FFS_ERROR;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsLookupFile(inode, path, fid);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsLookupFile */

/* FsCreateFile : create a file */
s32 FsCreateFile(struct inode *inode, char *path, u8 mode, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of pointer parameters */
	if ((fid == NULL) || (path == NULL) || (*path == '\0'))
		return FFS_ERROR;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsCreateFile(inode, path, mode, fid);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsCreateFile */

s32 FsReadFile(struct inode *inode, FILE_ID_T *fid, void *buffer, u64 count, u64 *rcount)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of the given file id */
	if (fid == NULL)
		return FFS_INVALIDFID;

	/* check the validity of pointer parameters */
	if (buffer == NULL)
		return FFS_ERROR;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsReadFile(inode, fid, buffer, count, rcount);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsReadFile */

s32 FsWriteFile(struct inode *inode, FILE_ID_T *fid, void *buffer, u64 count, u64 *wcount)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of the given file id */
	if (fid == NULL)
		return FFS_INVALIDFID;

	/* check the validity of pointer parameters */
	if (buffer == NULL)
		return FFS_ERROR;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsWriteFile(inode, fid, buffer, count, wcount);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsWriteFile */

/* FsTruncateFile : resize the file length */
s32 FsTruncateFile(struct inode *inode, u64 old_size, u64 new_size)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	DPRINTK("FsTruncateFile entered (inode %p size %llu)\n", inode, new_size);

	err = ffsTruncateFile(inode, old_size, new_size);

	DPRINTK("FsTruncateFile exitted (%d)\n", err);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsTruncateFile */

/* FsMoveFile : move(rename) a old file into a new file */
s32 FsMoveFile(struct inode *old_parent_inode, FILE_ID_T *fid, struct inode *new_parent_inode, struct dentry *new_dentry)
{
	s32 err;
	struct super_block *sb = old_parent_inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of the given file id */
	if (fid == NULL)
		return FFS_INVALIDFID;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsMoveFile(old_parent_inode, fid, new_parent_inode, new_dentry);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsMoveFile */

/* FsRemoveFile : remove a file */
s32 FsRemoveFile(struct inode *inode, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of the given file id */
	if (fid == NULL)
		return FFS_INVALIDFID;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsRemoveFile(inode, fid);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsRemoveFile */

/* FsSetAttr : set the attribute of a given file */
s32 FsSetAttr(struct inode *inode, u32 attr)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsSetAttr(inode, attr);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsSetAttr */

/* FsReadStat : get the information of a given file */
s32 FsReadStat(struct inode *inode, DIR_ENTRY_T *info)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsGetStat(inode, info);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsReadStat */

/* FsWriteStat : set the information of a given file */
s32 FsWriteStat(struct inode *inode, DIR_ENTRY_T *info)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	DPRINTK("FsWriteStat entered (inode %p info %p\n", inode, info);

	err = ffsSetStat(inode, info);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	DPRINTK("FsWriteStat exited (%d)\n", err);

	return err;
} /* end of FsWriteStat */

/* FsMapCluster : return the cluster number in the given cluster offset */
s32 FsMapCluster(struct inode *inode, s32 clu_offset, u32 *clu)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of pointer parameters */
	if (clu == NULL)
		return FFS_ERROR;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsMapCluster(inode, clu_offset, clu);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsMapCluster */

/*----------------------------------------------------------------------*/
/*  Directory Operation Functions                                       */
/*----------------------------------------------------------------------*/

/* FsCreateDir : create(make) a directory */
s32 FsCreateDir(struct inode *inode, char *path, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of pointer parameters */
	if ((fid == NULL) || (path == NULL) || (*path == '\0'))
		return FFS_ERROR;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsCreateDir(inode, path, fid);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsCreateDir */

/* FsReadDir : read a directory entry from the opened directory */
s32 FsReadDir(struct inode *inode, DIR_ENTRY_T *dir_entry)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of pointer parameters */
	if (dir_entry == NULL)
		return FFS_ERROR;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsReadDir(inode, dir_entry);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsReadDir */

/* FsRemoveDir : remove a directory */
s32 FsRemoveDir(struct inode *inode, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* check the validity of the given file id */
	if (fid == NULL)
		return FFS_INVALIDFID;

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	err = ffsRemoveDir(inode, fid);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return err;
} /* end of FsRemoveDir */

EXPORT_SYMBOL(FsMountVol);
EXPORT_SYMBOL(FsUmountVol);
EXPORT_SYMBOL(FsGetVolInfo);
EXPORT_SYMBOL(FsSyncVol);
EXPORT_SYMBOL(FsLookupFile);
EXPORT_SYMBOL(FsCreateFile);
EXPORT_SYMBOL(FsReadFile);
EXPORT_SYMBOL(FsWriteFile);
EXPORT_SYMBOL(FsTruncateFile);
EXPORT_SYMBOL(FsMoveFile);
EXPORT_SYMBOL(FsRemoveFile);
EXPORT_SYMBOL(FsSetAttr);
EXPORT_SYMBOL(FsReadStat);
EXPORT_SYMBOL(FsWriteStat);
EXPORT_SYMBOL(FsMapCluster);
EXPORT_SYMBOL(FsCreateDir);
EXPORT_SYMBOL(FsReadDir);
EXPORT_SYMBOL(FsRemoveDir);

#ifdef CONFIG_EXFAT_KERNEL_DEBUG
/* FsReleaseCache: Release FAT & buf cache */
s32 FsReleaseCache(struct super_block *sb)
{
	FS_INFO_T *p_fs = &(EXFAT_SB(sb)->fs_info);

	/* acquire the lock for file system critical section */
	sm_P(&(fs_struct[p_fs->drv].v_sem));

	FAT_release_all(sb);
	buf_release_all(sb);

	/* release the lock for file system critical section */
	sm_V(&(fs_struct[p_fs->drv].v_sem));

	return 0;
}
/* FsReleaseCache */

EXPORT_SYMBOL(FsReleaseCache);
#endif /* CONFIG_EXFAT_KERNEL_DEBUG */

/*======================================================================*/
/*  Local Function Definitions                                          */
/*======================================================================*/

/* end of exfat_api.c */
