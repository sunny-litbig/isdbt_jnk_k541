/**

  Copyright (C) 2009-2010 Telechips Inc.

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/

/******************************************************************************
* include 
******************************************************************************/
#define LOG_TAG	"TCC_UTIL"
#include <utils/Log.h>
#include "TCCMemory.h"
#include "TCC_Util.h"

const char *MMOUNT = "/proc/mounts";

MOUNTP *dfopen()
{
	MOUNTP *MP;    // /proc/mounts 파일을 연다.
	MP = (MOUNTP *)TCC_fo_malloc(__func__, __LINE__,sizeof(MOUNTP));
	if(MP == NULL){
		return NULL;
	}
	
	if(!(MP->fp = fopen(MMOUNT, "r")))
	{
		TCC_fo_free (__func__, __LINE__,MP);
		return NULL;
	}
	else
		return MP;
}

MOUNTP *dfget(MOUNTP *MP, int deviceType)
{
	char buf[256];
	char realdir[MOUNT_DIRECTORY_LENGTH] = {0,};
	struct statfs lstatfs;
	struct stat lstat;
	int is_root = 0;

	memset(&lstatfs, 0, sizeof(struct statfs));
	memset(&lstat, 0, sizeof(struct stat));
	switch(deviceType)
	{
		case 0:
			if (readlink(STORAGE_NAND, realdir, MOUNT_DIRECTORY_LENGTH) == 0)
				strcpy(realdir, STORAGE_NAND);
			break;
		case 1:
			strcpy(realdir, STORAGE_SDCARD);
			break;
		default:
			return NULL;
	}

	// /proc/mounts로 부터 마운트된 파티션의 정보를 얻어온다.
	while(fgets(buf, 255, MP->fp))
	{
		is_root = 0;
		sscanf(buf, "%s%s%s",MP->devname, MP->mountdir, MP->fstype);
	#if 0
		if (strcmp(MP->mountdir,"/") == 0)
			is_root=1;
	#endif

		if(strcmp(MP->mountdir, realdir) != 0)
			continue;

		if (stat(MP->devname, &lstat) == 0 || is_root)
		{
			if (strstr(buf, MP->mountdir) && (S_ISBLK(lstat.st_mode) || strcmp("fuse", MP->fstype) == 0) || is_root)
			{
				// 파일시스템의 총 할당된 크기와 사용량을 구한다.
				statfs(MP->mountdir, &lstatfs);
				MP->size.blocks = lstatfs.f_blocks * (lstatfs.f_bsize/1024);
				MP->size.avail  = lstatfs.f_bavail * (lstatfs.f_bsize/1024);
				rewind(MP->fp);
				return MP;
			}
		}
	}
	rewind(MP->fp);
	return NULL;
}

MOUNTP *dfgetex(MOUNTP *MP,  unsigned char *pStrPath)
{
	char buf[256], devname[DEVICE_NAME_LENGTH], mountdir[MOUNT_DIRECTORY_LENGTH], fstype[FILESYSTEM_LENGTH];
	char realdir[MOUNT_DIRECTORY_LENGTH] = {0,};
	struct statfs lstatfs;
	struct stat lstat;
	int match_path_len = 0, mount_path_len;

	memset(&lstatfs, 0, sizeof(struct statfs));
	if (strncmp(STORAGE_NAND, pStrPath, strlen(STORAGE_NAND)) != 0 || readlink(STORAGE_NAND, realdir, MOUNT_DIRECTORY_LENGTH) == 0)
		strcpy(realdir, pStrPath);

	// /proc/mounts로 부터 마운트된 파티션의 정보를 얻어온다.
	while(fgets(buf, 255, MP->fp))
	{		
		sscanf(buf, "%s%s%s",MP->devname, MP->mountdir, MP->fstype);    
		mount_path_len = strlen(MP->mountdir);
		if(mount_path_len>1 && strncmp(MP->mountdir, realdir, mount_path_len) == 0)
        {
            if( mount_path_len > match_path_len )
            {
                match_path_len = mount_path_len;
                strcpy(devname, MP->devname);
                strcpy(mountdir, MP->mountdir);
				strcpy(fstype, MP->fstype);
            }
        }
	}
    if(match_path_len)
    {
        strcpy(MP->devname, devname);
        strcpy(MP->mountdir, mountdir);
		strcpy(MP->fstype, fstype);
#if 0
        if (stat(MP->devname, &lstat) == 0 && S_ISBLK(lstat.st_mode))
#else
		if (strcmp("fuse", MP->fstype) == 0 || strcmp("ufsd", MP->fstype) == 0 || strcmp("ext4", MP->fstype) == 0 || strcmp("vfat", MP->fstype) == 0)
#endif
		{
			// 파일시스템의 총 할당된 크기와 사용량을 구한다.
			statfs(MP->mountdir, &lstatfs);
			MP->size.byteperblock = lstatfs.f_bsize;
			MP->size.blocks = lstatfs.f_blocks * (lstatfs.f_bsize/1024);
			MP->size.avail  = lstatfs.f_bavail * (lstatfs.f_bsize/1024);
			rewind(MP->fp);
			return MP;
		}
	}
	rewind(MP->fp);
	return NULL;
}


int dfclose(MOUNTP *MP)
{
	if(MP->fp != 0)
		fclose(MP->fp);
	TCC_fo_free (__func__, __LINE__,MP);
	return 1;
}

