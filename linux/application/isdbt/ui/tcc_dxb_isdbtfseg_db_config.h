/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_isdbtfseg_ui_process.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef __TCC_DXB_ISDBTFSEG_DB_CONFIG_H__
#define __TCC_DXB_ISDBTFSEG_DB_CONFIG_H__

/* dupliacated from TsParse_ISDBT_DBLayer.h */

#define ISDBT_DB_PATH			"/tcc_data"
#define ISDBT_LOG_DB_PATH		"/tcc_gtk"
#define ISDBT_SHM_PATH			"/dev/shm"
#define ISDBT_MEMORYDB_PATH		":memory:"

#ifdef EPGDB_ON_RAMDISK
#define ISDBT_EPGDB_PATH		ISDBT_SHM_PATH"/ISDBTEPG.db"//ISDBT_DB_PATH"/ISDBTEPG.db"
#else
#define ISDBT_EPGDB_PATH		ISDBT_DB_PATH"/ISDBTEPG.db"
#endif
#define ISDBT_BASICDB_PATH		ISDBT_DB_PATH"/ISDBT.db"
#define ISDBT_CHDB_HOME_PATH	ISDBT_DB_PATH"/ISDBT_CH_HOME.db"
#define ISDBT_CHDB_AUTO_PATH	ISDBT_DB_PATH"/ISDBT_CH_AUTO.db"
#define ISDBT_LOGODB_PATH		ISDBT_LOG_DB_PATH"/ISDBTLogo.db"

#define SQLITE_DXB_ISDBTFSEG_FILTER_1			"((serviceID > 0 ) AND ((serviceType == 1) OR ((serviceType == 192) AND ((audioPID > 2 AND audioPID < 8191) OR (videoPID > 2 AND videoPID < 8191)))))"

#endif //__TCC_DXB_ISDBTFSEG_DB_CONFIG_H__
