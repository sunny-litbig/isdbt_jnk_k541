/*****************************************************************************************
 *
 * FILE NAME          : MxL603_OEM_Drv.c
 * 
 * AUTHOR             : Mahendra Kondur
 *
 * DATE CREATED       : 12/23/2011  
 *
 * DESCRIPTION        : This file contains I2C driver and Sleep functins that 
 *                      OEM should implement for MxL603 APIs
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/
#ifndef __KERNEL__
#define LOG_TAG	"MXL603"
#include <utils/Log.h>

#include <stdio.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <sys/time.h>
#else
#include <linux/delay.h>
#include <linux/i2c.h>

#define     I2C_PORT   1
#define     printf     printk
#endif

#include "MxL603_OEM_Drv.h"

#define     DEBUG_PRITF     printf

//#define MXL101SF_CHIP_ADDR   0x60
#define I2C_SLAVE   0x0703  /* Change slave address         */
#define I2C_WRITE 0
#define I2C_READ  1

extern int g_i2c_fd, g_i2c_slave_addr;
extern int Ctrl_SetI2CAddr(char I2CSlaveAddr);


MXL_STATUS Ctrl_I2cConnect(UINT8 devId)
{
	MXL_STATUS status = MXL_TRUE;    
#ifndef __KERNEL__
	int i2c_port = 1, err;	
	char filename[20];
    
    g_i2c_fd = -1;
    g_i2c_slave_addr = -1;

	sprintf(filename, "/dev/i2c-%d", i2c_port);
	g_i2c_fd = open(filename,O_RDWR);
	if (g_i2c_fd < 0)
	{
	    DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);
	    return MXL_FALSE;
	}
#endif
	Ctrl_SetI2CAddr(devId);	
	return status;
}

MXL_STATUS Ctrl_I2cDisConnect()
{
    MXL_STATUS status = MXL_TRUE;    
#ifndef __KERNEL__
    if(g_i2c_fd >= 0)
        close(g_i2c_fd);
#endif
    g_i2c_fd = -1;
    return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_OEM_ReadRegister
--| 
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C read operation.
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_OEM_ReadRegister(UINT8 devId, UINT8 RegAddr, UINT8 *DataPtr)
{
  // OEM should implement I2C read protocol that complies with MxL603 I2C
  // format.

  // 8 bit Register Read Protocol:
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFB| |RegAddr   | |P|
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |SLAVE |         |A|    |A|          |A| |
  // +------+-+-----+-+-+----+-+----------+-+-+
  // +------+-+-----+-+-+-----+--+-+
  // |MASTER|S|SADDR|R| |     |MN|P|
  // +------+-+-----+-+-+-----+--+-+
  // |SLAVE |         |A|Data |  | |
  // +------+---------+-+-----+--+-+
  // Legends: SADDR(I2c slave address), S(Start condition), MA(Master Ack), MN(Master NACK), 
  // P(Stop condition)

	MXL_STATUS status = MXL_TRUE;
	int	err;
	char buf[2];
  
#ifndef __KERNEL__
	Ctrl_SetI2CAddr(devId);	
	buf[0] = 0xFB;
    buf[1] = RegAddr;
    err= write(g_i2c_fd, buf, 2);
    if (err!=2)
	{	
	    /* ERROR HANDLING: i2c transaction failed */ 
	    DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);
	    return MXL_FALSE;
	}  

	err= read(g_i2c_fd, DataPtr, 1);
	if (err!=1)
	{	
	    /* ERROR HANDLING: i2c transaction failed */ 
	    DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);
	    return MXL_FALSE;
	}  
#else
	struct i2c_msg msg;
	struct i2c_adapter *i2c_handle;

	Ctrl_SetI2CAddr(devId);	
	buf[0] = 0xFB;
	buf[1] = RegAddr;

	// write value
	memset(&msg, 0, sizeof(msg));
	msg.addr = devId;
	msg.flags = 0;
	msg.buf = buf;
	msg.len = 2;

	i2c_handle = i2c_get_adapter(I2C_PORT);
	err = i2c_transfer(i2c_handle, &msg, 1);
	if(err < 0){
		printk(" %s: writereg error, errno is %d \n", __FUNCTION__, err);
		return MXL_FALSE;
	}

	// read value
	memset(&msg, 0, sizeof(msg));
	msg.addr = devId;
	msg.flags |=  I2C_M_RD;  //write  I2C_M_RD=0x01
	msg.buf = DataPtr;
	msg.len = 1;

	i2c_handle = i2c_get_adapter(I2C_PORT);
	err = i2c_transfer(i2c_handle, &msg, 1);
	if(err != 1)
	{
		printk("mxl101_readregister reg failure!\n");
		return MXL_FALSE;
	}
#endif

/* If OEM data is implemented, customer needs to use OEM data structure related operation 
   Following code should be used as a reference. 
   For more information refer to sections 2.5 & 2.6 of MxL603_mxLWare_API_UserGuide document.

  UINT8 i2cSlaveAddr;
  UINT8 i2c_bus;
  user_data_t * user_data = (user_data_t *) MxL603_OEM_DataPtr[devId];
 
  if (user_data)
  {
    i2cSlaveAddr = user_data->i2c_address;           // get device i2c address
    i2c_bus = user_data->i2c_bus;                   // get device i2c bus  
  
    sem_up(user_data->sem);                         // up semaphore if needed

    // I2C Write operation 
    status = USER_I2C_READ_FUNCTION(i2cSlaveAddr, i2c_bus, RegAddr, DataPtr);
    
    sem_down(user_data->sem);                       // down semaphore
    user_data->i2c_cnt++;                           // user statistics
  }

*/

  /* If OEM data is not required, customer should treat devId as I2C slave Address */

 
  
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_OEM_WriteRegister
--| 
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C write operation.
--|
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_OEM_WriteRegister(UINT8 devId, UINT8 RegAddr, UINT8 RegData)
{
  
  // OEM should implement I2C write protocol that complies with MxL603 I2C
  // format.

  // 8 bit Register Write Protocol:
  // +------+-+-----+-+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |RegAddr   | |RegData(L)| |P|
  // +------+-+-----+-+-+----------+-+----------+-+-+
  // |SLAVE |         |A|          |A|          |A| |
  // +------+---------+-+----------+-+----------+-+-+
  // Legends: SADDR (I2c slave address), S (Start condition), A (Ack), N(NACK), 
  // P(Stop condition)

	MXL_STATUS status = MXL_TRUE;
	int	err;
	char buf[2];

#ifndef __KERNEL__
	Ctrl_SetI2CAddr(devId);	
	buf[0] = RegAddr;
	buf[1] = RegData;

	err= write(g_i2c_fd, buf, 2);
	if(err != 2)
	{
	    DEBUG_PRITF("[%s:%d] error\n", __func__, __LINE__);
	    return MXL_FALSE;
	}
#else
	struct i2c_msg msg;
	struct i2c_adapter *i2c_handle;

	Ctrl_SetI2CAddr(devId);	
	buf[0] = RegAddr;
	buf[1] = RegData;

	// write value
	memset(&msg, 0, sizeof(msg));
	msg.addr = devId;
	msg.flags = 0;
	msg.buf = buf;
	msg.len = 2;

	i2c_handle = i2c_get_adapter(I2C_PORT);
	err = i2c_transfer(i2c_handle, &msg, 1);
	if(err < 0){
		printk(" %s: writereg error, errno is %d \n", __FUNCTION__, err);
		status = MXL_FALSE;
	}
#endif

/* If OEM data is implemented, customer needs to use OEM data structure related operation 
   Following code should be used as a reference. 
   For more information refer to sections 2.5 & 2.6 of MxL603_mxLWare_API_UserGuide document.

  UINT8 i2cSlaveAddr;
  UINT8 i2c_bus;
  user_data_t * user_data = (user_data_t *) MxL603_OEM_DataPtr[devId];
 
  if (user_data)
  {
    i2cSlaveAddr = user_data->i2c_address;           // get device i2c address
    i2c_bus = user_data->i2c_bus;                   // get device i2c bus  
  
    sem_up(user_data->sem);                         // up semaphore if needed

    // I2C Write operation 
    status = USER_I2C_WRITE_FUNCTION(i2cSlaveAddr, i2c_bus, RegAddr, RegData);
    
    sem_down(user_data->sem);                       // down semaphore
    user_data->i2c_cnt++;                           // user statistics
  }

*/

  /* If OEM data is not required, customer should treat devId as I2C slave Address */

    return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_OEM_Sleep
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete sleep operation. WaitTime is in ms unit
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

void MxLWare603_OEM_Sleep(UINT16 DelayTimeInMs)
{
  // OEM should implement sleep operation 
#ifndef __KERNEL__
  usleep(DelayTimeInMs*1000);
#else
  udelay(DelayTimeInMs*1000);
#endif
}
