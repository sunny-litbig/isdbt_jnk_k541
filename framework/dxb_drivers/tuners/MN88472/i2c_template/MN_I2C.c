/*****************************************************************************************
 *
 * FILE NAME          : MN_I2C.C
 * 
 ****************************************************************************************/
#ifndef __KERNEL__
#define LOG_TAG	"MN88472"
#include <utils/Log.h>

#include <stdio.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <sys/time.h>
#else
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/i2c.h>

#define I2C_PORT    1
#endif

#include "MN_DMD_common.h"
#include "MN_DMD_driver.h"
#include "MN_I2C.h"

//#define DMD_I2C_DEBUG
#ifdef DMD_I2C_DEBUG
DMD_u32_t	dmd_i2c_debug_flag = 1;
#endif
/* **************************************************** */
/* **************************************************** */
#define DMD_I2C_MAXSIZE     DMD_REGISTER_MAX

#define     DEBUG_PRITF     ALOGD

//#define MXL101SF_CHIP_ADDR   0x60
#define I2C_SLAVE   0x0703  /* Change slave address         */
#define I2C_WRITE 0
#define I2C_READ  1

int g_i2c_fd = -1, g_i2c_slave_addr = -1;
int Ctrl_SetI2CAddr(char I2CSlaveAddr)
{
#ifndef __KERNEL__
	int err;
	if(g_i2c_slave_addr == I2CSlaveAddr)	  
		return 0;
		
	g_i2c_slave_addr = I2CSlaveAddr;							
	//DEBUG_PRITF("[%s] CHIP ADDR 0x%X \n", __func__, g_i2c_slave_addr);
	err = ioctl(g_i2c_fd, I2C_SLAVE /*IOCTL_SET_SLAVE_ADDR*/, g_i2c_slave_addr); /* 7*/
	if(err < 0)
	{
	    DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);
		return 1;
	}
#else
    g_i2c_slave_addr = I2CSlaveAddr;
#endif
	return 0;
}

void DMD_API DMD_I2C_close(void)
{
#ifndef __KERNEL__
    if(g_i2c_fd >= 0)
        close(g_i2c_fd);
#endif
    g_i2c_fd = -1;
}
/*! I2C Initialize Function*/
DMD_ERROR_t DMD_API DMD_I2C_open(void)
{
	//TODO:	Please call I2C initialize API here
	//this function is called by DMD_API_open
#ifndef __KERNEL__
    DMD_ERROR_t ret = DMD_E_OK;
    int i2c_port = 1, err;	
	char filename[20];
	g_i2c_fd = -1;
    g_i2c_slave_addr = -1;
    DMD_I2C_close();

	sprintf(filename, "/dev/i2c-%d", i2c_port);
	g_i2c_fd = open(filename,O_RDWR);
	if (g_i2c_fd < 0)
	{
	    DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);
	    return DMD_E_ERROR;
	}   
#endif
	return DMD_E_OK;
}
/*! Write 1byte */
DMD_ERROR_t DMD_I2C_Write(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t data )
{
	//TODO:	Please call I2C 1byte Write API
	//DMD_ERROR_t status = DMD_E_OK;
	int	err;
	char buf[2];

#ifndef __KERNEL__
	Ctrl_SetI2CAddr(slvadr);	

    buf[0] = adr;
    buf[1] = data;

    err= write(g_i2c_fd, buf, 2);
    if(err != 2)
    {
        DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);

        return DMD_E_ERROR;
    }
#else
	struct i2c_msg msg;
	struct i2c_adapter *i2c_handle;

	Ctrl_SetI2CAddr(slvadr);	

	buf[0] = adr;
	buf[1] = data;

	// write value
	memset(&msg, 0, sizeof(msg));
	msg.addr = slvadr;
	msg.flags = 0;
	msg.buf = buf;
	msg.len = 2;

	i2c_handle = i2c_get_adapter(I2C_PORT);
	err = i2c_transfer(i2c_handle, &msg, 1);
	if(err < 0){
		printk(" %s: writereg error, errno is %d \n", __FUNCTION__, err);
		return DMD_E_ERROR;
	}
#endif
    
    #ifdef DMD_I2C_DEBUG
	if( dmd_i2c_debug_flag )
		DEBUG_PRITF("W:%02x %02x %02x\n",slvadr,adr,data);
    #endif

	return DMD_E_OK;
}

/*! Read 1byte */
DMD_ERROR_t DMD_I2C_Read(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t *data )
{
	//TODO:	call I2C 1byte Write API
	//DMD_ERROR_t status = DMD_E_OK;
    int	err;
    char buf[2];    

#ifndef __KERNEL__
    Ctrl_SetI2CAddr(slvadr);	

    buf[0] = adr;
    err= write(g_i2c_fd, buf, 1);
    if (err!=1)
    {	
        /* ERROR HANDLING: i2c transaction failed */ 
        DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);
        return DMD_E_ERROR;
    }  

    err= read(g_i2c_fd, data, 1);
    if (err!=1)
    {	
        /* ERROR HANDLING: i2c transaction failed */ 
        DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);
        return DMD_E_ERROR;
    }  
    #ifdef DMD_I2C_DEBUG
	if( dmd_i2c_debug_flag )
		DEBUG_PRITF("R:%02x,%02x,%02x\n",slvadr,adr,*data);
    #endif
#else
	struct i2c_msg msg;
	struct i2c_adapter *i2c_handle;

	Ctrl_SetI2CAddr(slvadr);	

	buf[0] = adr;

	// write value
	memset(&msg, 0, sizeof(msg));
	msg.addr = slvadr;
	msg.flags = 0;
	msg.buf = buf;
	msg.len = 1;

	i2c_handle = i2c_get_adapter(I2C_PORT);
	err = i2c_transfer(i2c_handle, &msg, 1);
	if(err<0){
		printk(" %s: writereg error, errno is %d \n", __FUNCTION__, err);
		return DMD_E_ERROR;
	}

	// read value
	memset(&msg, 0, sizeof(msg));
	msg.addr = slvadr;
	msg.flags |=  I2C_M_RD;  //write  I2C_M_RD=0x01
	msg.buf = buf;
	msg.len = 1;

	i2c_handle = i2c_get_adapter(I2C_PORT);
	err = i2c_transfer(i2c_handle, &msg, 1);
	if(err != 1)
	{
		printk("mxl101_readregister reg failure!\n");
		return DMD_E_ERROR;
	}

	*data = buf[0];
#endif

	return DMD_E_OK;
}
/* 
Write/Read any Length to DMD and Tuner;
DMD_u8_t slvadr : DMD's slave address.   
DMD_u8_t adr    : DMD's REG address; 

For writing CMD to tuner, 
DMD_u8_t slvadr is DMD's slave address;
DMD_u8_t adr is DMD's register TCBCOM(0xF7)
!!!Tuner's slave addr. and REG addr. are as continous data< upper layer : data[x]> 
*/
DMD_ERROR_t DMD_I2C_Write_Anylenth(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen)
{

	//TODO:	Please call I2C Read/Write API here
#ifdef already_add_BE_API
	DMD_u8_t max_length_once = 127; //the maximum bytes BE be able to write to device, one time. 
	DMD_u8_t* curwdata;
	if(wlen == 0)
	{
	}
	else if(wlen < max_length_once)
	{
	 	twsbSetDatas(slvadr, adr, wdata, wlen);
	}
	else
	{
	 	curwdata = wdata;
	 	while(wlen>=max_length_once)
		{
			twsbSetDatas(slvadr, adr, curwdata, length);
			wlen -= max_length_once;
			curwdata += length;
		}
		if(wlen > 0)
		{
		 	twsbSetDatas(slvadr, adr, curwdata, wlen);
		}
	}			
#endif

	return DMD_E_OK;
}

/*
twsbGetData_MN88472_TCB(slvadr, adr, wdata)
This is specific function for MN88472 to read Tuner reg's data.
? If you use MN88472 to control Tuner, you may need to create new I2C bottom code.

There are two steps to read Tuner, take Mxl603 for example. (See Tuner I2C read protocal as below)
Step 1 : Tell Tuner which Reg to read 
      -> Pls. call function DMD_I2C_Write_Anylenth()
Step 2 : Read Tuner Reg's data
      -> Pls. call function DMD_TCBI2C_Read()
------ Tuner Mxl603 I2C read protocal -------
// 8 bit Register Read Protocol:
  Step1
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFB| |RegAddr   | |P|
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |SLAVE |         |A|    |A|          |A| |
  // +------+-+-----+-+-+----+-+----------+-+-+
  Step2
  // +------+-+-----+-+-+-----+--+-+
  // |MASTER|S|SADDR|R| |     |MN|P|
  // +------+-+-----+-+-+-----+--+-+
  // |SLAVE |         |A|Data |  | |
  // +------+---------+-+-----+--+-+
  // SADDR(I2c slave address), S(Start condition), 
  // A(Slave Ack), MN(Master NACK),  P(Stop condition)

// [ Pls pay attention here ! This is what customer need complete ]
------ Step1 I2C commnication between BE and DMD -------
???? : ? SADDR-M , 0xFB ? R-addr-M ????????DMD,DMD?????????Tuner
  // +------+-+-----+-+-+----+-+----------+-+-++------+-+-----+-+-+----
  // |BE |S|SADDR|W| |R-addr| |SADDR-M|W| |0xFB| |R-addr-M| |P|
  // +------+-+-----+-+-+----+-+----------+-+-++------+-+-----+-+-+----
  // |DMD|         |A|      |A|         |A|    |A|        |A|    
  // +------+-+-----+-+-+----+-+----------+-+-++------+-+-----+-+-+----
  SADDR(DMD's slave address), R-addr(DMD's reg addr. -> TCBCOM ),
  SADDR-M(Tuner's slave address)
  R-addr-M(tell Tuner which reg address to read out)
    
------ Step2 I2C commnication between BE and DMD -------
???? : ??? "BE" ?????? BE I2C ???? 
  // +------+-+-----+-+-+----+-+----------+-+-++------+-+-----+-+-+----
  // |BE |S|SADDR|W| |R-addr| |SADDR-M|R| |S|SADDR|R| |      |MN|P|
  // +------+-+-----+-+-+----+-+----------+-+-++------+-+-----+-+-+----
  // |DMD|         |A|      |A|         |A|         |A| Data |  | |
  // +------+-+-----+-+-+----+-+----------+-+-++------+-+-----+-+-+----

SADDR(DMD's slave address), R-addr(DMD's reg addr. -> TCBCOM¢®E¡Ëc¢®E¢®©­0xF7¢®E¡Ëc¡§Io ),
SADDR-M(Tuner's slave address)

  You can also refer to file "PRODUCT_SPECIFICATIONS_MN88472_ver041_120120.pdf"
  page 34 to see the TCB control flow.
*/
DMD_ERROR_t DMD_TCBI2C_Read(DMD_u8_t slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen)
{
#ifdef already_add_BE_API
    DMD_u8_t* tuner_slave_addr = wdata;
	if(rlen)
	{
		*rdata=twsbGetData_MN88472_TCB(slvadr, adr, tuner_slave_addr); //sample, pls. use your API 
	}
	else
	{
		printf(" ########### There is no Rlen indicated while reading Tuner !!! ##################### ");
	}
#endif
	return DMD_E_OK;
}

/*! Write/Read any Length*/
// This is general API for you to communicate with external device which DIRECTLY connect to BE,
// However, to communicate with Tuner through DMD MN88472, you may need to modify Bottom code of I2C sequence. 
DMD_ERROR_t DMD_I2C_WriteRead(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen)
{
	//TODO:	Please call I2C Read/Write API here
	//unsigned int	wl;
	unsigned int i, err;

#ifndef __KERNEL__
	char wd[DMD_REGISTER_MAX + 2]; //32 is for saving address
	char rd[DMD_REGISTER_MAX];

    Ctrl_SetI2CAddr(slvadr);	

    wd[0] = adr;
	for(i=0;i<wlen;i++) 
	    wd[i+1] = (char) wdata[i];
#if 0
   	for(i=0;i<wlen;i++) 
         DMD_I2C_Write(	slvadr ,adr , wdata[i] );
#else    
    //usbi2c_send( slvadr, adr, 1,wd  ,(u8)wlen, rd, (u8)rlen, WAIT_SEND, OFF_400K, LAST_WAIT);
    err= write(g_i2c_fd, wd, wlen+1);
    if(err != (wlen+1))
    {
        DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);
        return DMD_E_ERROR;
    }
#endif    
    if( rlen == 0 || !rdata)
        return DMD_E_OK;
    
    err= read(g_i2c_fd, rd, rlen);
    if (err!=rlen)
    {	
        /* ERROR HANDLING: i2c transaction failed */ 
        DEBUG_PRITF("[%s:%d] erro\n", __func__, __LINE__);
        return DMD_E_ERROR;
    }  
#else
	char *wd = kzalloc(DMD_REGISTER_MAX + 2, GFP_KERNEL);
	char *rd = kzalloc(DMD_REGISTER_MAX, GFP_KERNEL);

	struct i2c_msg msg;
	struct i2c_adapter *i2c_handle;

	Ctrl_SetI2CAddr(slvadr);

	wd[0] = adr;
	for(i=0;i<wlen;i++) 
		wd[i+1] = (char) wdata[i];

	// write value
	memset(&msg, 0, sizeof(msg));
	msg.addr = slvadr;
	msg.flags = 0;
	msg.buf = wd;
	msg.len = wlen+1;

	i2c_handle = i2c_get_adapter(I2C_PORT);
	err = i2c_transfer(i2c_handle, &msg, 1);
	if(err<0){
		kfree(wd);
		kfree(rd);
		printk(" %s: writereg error, errno is %d \n", __FUNCTION__, err);
		return DMD_E_ERROR;
	}

	if( rlen == 0 || !rdata)
	{
		kfree(wd);
		kfree(rd);
		return DMD_E_OK;
	}

	// read value
	memset(&msg, 0, sizeof(msg));
	msg.addr = slvadr;
	msg.flags |=  I2C_M_RD;  //write  I2C_M_RD=0x01
	msg.buf = rd;
	msg.len = rlen;

	i2c_handle = i2c_get_adapter(I2C_PORT);
	err = i2c_transfer(i2c_handle, &msg, 1);
	if(err != rlen)
	{
		kfree(wd);
		kfree(rd);
		printk("mxl101_readregister reg failure!\n");
		return DMD_E_ERROR;
	}
#endif

#ifdef DMD_I2C_DEBUG
	if( dmd_i2c_debug_flag ){
		DEBUG_PRITF("W:%02x %02x:",slvadr,adr);
		for(i=0;i<wlen;i++) DEBUG_PRITF("%02x ",wd[i] );
		DEBUG_PRITF("\n");
		DEBUG_PRITF("R:%02x %02x:",slvadr,adr);
		for(i=0;i<rlen;i++) DEBUG_PRITF("%02x ",rd[i] );
		DEBUG_PRITF("\n");
	}
#endif
	for(i=0;i<rlen;i++) rdata[i] = (DMD_u8_t) rd[i];

#ifdef __KERNEL__
	kfree(wd);
	kfree(rd);
#endif

	return DMD_E_OK;
}

/*! Timer wait */
DMD_ERROR_t DMD_wait( DMD_u32_t msecond ){

	//TODO: call timer wait function 
#ifndef __KERNEL__
	usleep(msecond*1000);
#else
	udelay(msecond*1000);
#endif
	return DMD_E_OK;
}

/*! Get System Time (ms) */
DMD_ERROR_t DMD_timer( DMD_u32_t* tim ){
#ifndef __KERNEL__
    struct timeval tv;

    gettimeofday (&tv, NULL);
    *tim = (long long) tv.tv_sec * 1000 + tv.tv_usec / 1000;
#else
    *tim = jiffies_to_msecs(jiffies);
#endif
	return DMD_E_OK;
}
