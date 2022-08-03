/*******************************************************************************
 *
 * FILE NAME          : MxL_Debug.h
 * 
 * AUTHOR             : Brenndon Lee
 *
 * DATE CREATED       : 10/25/2010
 *
 * DESCRIPTION        : Debug header files 
 *
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL_DEBUG_H__
#define __MXL_DEBUG_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/

#ifndef __KERNEL__
#include <stdio.h>
#include "MaxLinearDataTypes.h"

#define LOG_TAG	"MXLDEBUG"
#include <utils/Log.h>
#else
#include <linux/kernel.h>
#include "MaxLinearDataTypes.h"
#endif
/******************************************************************************
    Macros
******************************************************************************/

#ifndef __KERNEL__
#define MxL_DLL_DEBUG0 ALOGD
#else
#define MxL_DLL_DEBUG0 printk
#endif


#endif /* __MXL_DEBUG_H__*/
