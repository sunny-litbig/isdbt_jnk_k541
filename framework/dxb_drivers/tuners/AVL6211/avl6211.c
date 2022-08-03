/*
 *  avl6211.c
 *
 *  Written by C2-G1-3T
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.=
 */

#include <linux/of_i2c.h>
#include <linux/of_gpio.h>

#include "dvb_frontend.h"
#include "tcc_fe.h"

#include "avl6211.h"


/*****************************************************************************
 * Log Message
 ******************************************************************************/
#define LOG_TAG    "[AVL6211]"

static int dev_debug = 0;

module_param(dev_debug, int, 0644);
MODULE_PARM_DESC(dev_debug, "Turn on/off device debugging (default:off).");

#define dprintk(msg...)                                \
{                                                      \
	if (dev_debug)                                     \
		printk(KERN_INFO LOG_TAG "(D)" msg);           \
}

#define eprintk(msg...)                                \
{                                                      \
	printk(KERN_INFO LOG_TAG " (E) " msg);             \
}


/*****************************************************************************
 * Defines
 ******************************************************************************/
// tune_mode
#define TUNE_MODE_NORMAL              0
#define TUNE_MODE_BLINDSCAN           1

// blindscan_state
#define BLINDSCAN_STATE_STOP          0
#define BLINDSCAN_STATE_START         1
#define BLINDSCAN_STATE_CHECK_STATE   2
#define BLINDSCAN_STATE_GET_PROGRESS  3
#define BLINDSCAN_STATE_GET_INFO      4
#define BLINDSCAN_STATE_CANCEL        5


/*****************************************************************************
 * structures
 ******************************************************************************/
typedef struct avl6211_demod_t {
	struct i2c_adapter *i2c;

	int wait;
	int blindscan_state;

	struct AVL_DVBSx_Chip AVLChip;
	struct AVL_Tuner Tuner;
	struct AVL_DVBSx_BlindScanAPI_Setting BSsetting;
	int index;
	u32 config_idx;

	int gpio_fe_power;
	int gpio_fe_reset;
	int gpio_fe_lock;
	int gpio_fe_fault;
	int gpio_lnb_power;
	int gpio_lnb_sp1v;
	int gpio_lnb_s18v;
	int gpio_ant_ctrl;
} avl6211_demod_t;

typedef struct avl6211_priv_t {
	struct avl6211_demod_t demod;
	int tune_mode;
} avl6211_priv_t;


/*****************************************************************************
 * Variables
 ******************************************************************************/


/*****************************************************************************
 * External Functions
 ******************************************************************************/


 /*****************************************************************************
 * Functions
 ******************************************************************************/
static int AVL6211_Reset(avl6211_demod_t *demod)
{
	tcc_gpio_config(demod->gpio_fe_reset, GPIO_FN(0));
	gpio_request(demod->gpio_fe_reset, NULL);
	gpio_direction_output(demod->gpio_fe_reset, 0);
	AVL_DVBSx_IBSP_Delay(300);
	gpio_direction_output(demod->gpio_fe_reset, 1);

	return 0;
}

static int AVL6211_PowerCtrl(avl6211_demod_t *demod, int on)
{
	tcc_gpio_config(demod->gpio_fe_power, GPIO_FN(0));
	gpio_request(demod->gpio_fe_power, NULL);
	gpio_direction_output(demod->gpio_fe_power, on);

	return 0;
}

static int AVL6211_LnbPowerCtrl(avl6211_demod_t *demod, int on)
{
	tcc_gpio_config(demod->gpio_lnb_power, GPIO_FN(0));
	gpio_request(demod->gpio_lnb_power, NULL);
	gpio_direction_output(demod->gpio_lnb_power, on);

	return 0;
}


/*****************************************************************************
 * AVL6211 Frontend Functions
 ******************************************************************************/
static void avl6211_fe_release(struct dvb_frontend* fe)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	if (priv)
	{
		kfree(priv);
	}

	dprintk("%s\n", __FUNCTION__);
}

static int avl6211_fe_init(struct dvb_frontend* fe)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	priv->tune_mode = TUNE_MODE_NORMAL;
	priv->demod.blindscan_state = BLINDSCAN_STATE_STOP;

	tcc_gpio_config(priv->demod.gpio_fe_lock, GPIO_FN(0));
	gpio_request(priv->demod.gpio_fe_lock, NULL);
	gpio_direction_input(priv->demod.gpio_fe_lock);

	tcc_gpio_config(priv->demod.gpio_fe_fault, GPIO_FN(0));
	gpio_request(priv->demod.gpio_fe_fault, NULL);
	gpio_direction_input(priv->demod.gpio_fe_fault);

	tcc_gpio_config(priv->demod.gpio_lnb_sp1v, GPIO_FN(0));
	gpio_request(priv->demod.gpio_lnb_sp1v, NULL);
	gpio_direction_input(priv->demod.gpio_lnb_sp1v);

	tcc_gpio_config(priv->demod.gpio_lnb_s18v, GPIO_FN(0));
	gpio_request(priv->demod.gpio_lnb_s18v, NULL);
	gpio_direction_input(priv->demod.gpio_lnb_s18v);

	AVL6211_Reset(&priv->demod);
	AVL_DVBSx_IBSP_Delay(100);
	AVL6211_PowerCtrl(&priv->demod, 0);
    //This function do all the initialization work.It should be called only once at the beginning.It needn't be recalled when we want to lock a new channel.
	r = AVL6211_Initialize(&priv->demod.AVLChip, &priv->demod.Tuner, priv->demod.config_idx);
	if(AVL_DVBSx_EC_OK == r)
	{
		dprintk("%s(Initialization success)\n", __FUNCTION__);
		AVL_DVBSx_IBSP_Delay(200);
	}
	else
	{
		eprintk("%s(Initialization Err:0x%x)\n", __FUNCTION__, r);
	}

	return (r);
}

static int avl6211_fe_sleep(struct dvb_frontend* fe)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	if (priv->demod.blindscan_state != BLINDSCAN_STATE_STOP)
	{
		AVL_DVBSx_IBlindScanAPI_Exit(&priv->demod.AVLChip, &priv->demod.BSsetting);
	}

	AVL6211_PowerCtrl(&priv->demod, 1);

	dprintk("%s\n", __FUNCTION__);

	return 0;
}


static int avl6211_fe_set_tune(struct dvb_frontend* fe, bool re_tune, unsigned int mode_flags, unsigned int *delay, fe_status_t *status)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;
	AVL_DVBSx_ErrorCode r;

	if (re_tune)
	{
		r = AVL6211_LockChannel(&priv->demod.AVLChip, &priv->demod.Tuner, c->frequency, c->symbol_rate);
		if (AVL_DVBSx_EC_OK == r)
		{
			if (c->symbol_rate < 5000000)
				priv->demod.wait = 500; // 5000 ms
			else if (c->symbol_rate < 10000000)
				priv->demod.wait = 60; // 600 ms
			else
				priv->demod.wait = 25; // 250ms

			dprintk("%s(freq = %d KHz, symbol_rate = %d Hz)\n", __FUNCTION__, c->frequency, c->symbol_rate);
		}
		else
		{
			priv->demod.wait = 1;

			eprintk("%s(Lock Err = 0x%x, freq = %d KHz, symbol_rate = %d Hz)\n", __FUNCTION__, r, c->frequency, c->symbol_rate);
		}
		*status = 0;
	}
	else
	{
		fe->ops.read_status(fe, status);
		if (*status != 0)
		{
			priv->demod.wait = 0;
		}
		if (priv->demod.wait > 0)
		{
			priv->demod.wait--;
			if (priv->demod.wait == 0)
			{
				*status = FE_TIMEDOUT;
			}
		}
	}

	if (priv->demod.wait > 0)
	{
		*delay = HZ / 100; // 10 ms
	}
	else
	{
		*delay = 3 * HZ; // 3 s
	}

	return 0;
}

static int avl6211_fe_blindscan(struct dvb_frontend* fe, bool re_tune, unsigned int mode_flags, unsigned int *delay, fe_status_t *status)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;
	struct AVL_DVBSx_Channel *pChannel;
	AVL_DVBSx_ErrorCode r;

	*status = 0;
	if (re_tune)
	{
		priv->demod.index = 0;
		AVL_DVBSx_IBlindScanAPI_Initialize(&priv->demod.BSsetting);//this function set the parameters blind scan process needed.
		r = AVL6211_BlindScan_Reset(&priv->demod.AVLChip, &priv->demod.BSsetting);
		if (AVL_DVBSx_EC_OK == r)
		{
			priv->demod.blindscan_state = BLINDSCAN_STATE_START;
			dprintk("%s(blind scan - reset)\n", __FUNCTION__);
		}
		else
		{
			priv->demod.blindscan_state = BLINDSCAN_STATE_CANCEL;
			eprintk("%s(blind scan - reset error![%d])\n", __FUNCTION__, r);
		}
	}
	else
	{
		switch (priv->demod.blindscan_state)
		{
			case BLINDSCAN_STATE_START:
			{
				r = AVL_DVBSx_IBlindScanAPI_Start(&priv->demod.AVLChip, &priv->demod.Tuner, &priv->demod.BSsetting);
				if (AVL_DVBSx_EC_OK == r)
				{
					priv->demod.blindscan_state = BLINDSCAN_STATE_CHECK_STATE;
					dprintk("%s(blind scan - start)\n", __FUNCTION__);
				}
				else
				{
					priv->demod.blindscan_state = BLINDSCAN_STATE_CANCEL;
					eprintk("%s(blind scan - start error![%d])\n", __FUNCTION__, r);
				}
				break;
			}
			case BLINDSCAN_STATE_CHECK_STATE:
			{
				r = AVL_DVBSx_IBlindScanAPI_GetCurrentScanStatus(&priv->demod.AVLChip, &priv->demod.BSsetting);
				switch (r)
				{
					case AVL_DVBSx_EC_OK:
						priv->demod.blindscan_state = BLINDSCAN_STATE_GET_PROGRESS;
						dprintk("%s(blind scan - get state OK)\n", __FUNCTION__);
						break;
					case AVL_DVBSx_EC_Running:
						dprintk("%s(blind scan - get state Wait)\n", __FUNCTION__);
						break;
					default:
						priv->demod.blindscan_state = BLINDSCAN_STATE_CANCEL;
						eprintk("%s(blind scan - get state error(0x%x)\n", __FUNCTION__, r);
				}
				break;
			}
			case BLINDSCAN_STATE_GET_PROGRESS:
			{
				c->frequency = AVL_DVBSx_IBlindscanAPI_GetProgress(&priv->demod.BSsetting);
				c->symbol_rate = priv->demod.BSsetting.m_uiChannelCount;
				*status = FE_HAS_LOCK;
				if (priv->demod.index < priv->demod.BSsetting.m_uiChannelCount)
				{
					priv->demod.blindscan_state = BLINDSCAN_STATE_GET_INFO;
				}
				else if (c->frequency == 100)
				{
					priv->demod.blindscan_state = BLINDSCAN_STATE_STOP;
				}
				else
				{
					priv->demod.blindscan_state = BLINDSCAN_STATE_START;
				}
				dprintk("%s(blind scan - progress %d %d\n", __FUNCTION__, c->frequency, c->symbol_rate);
				break;
			}
			case BLINDSCAN_STATE_GET_INFO:
			{
				if (priv->demod.index < priv->demod.BSsetting.m_uiChannelCount)
				{
					pChannel = &priv->demod.BSsetting.channels[priv->demod.index++];
					c->frequency = pChannel->m_uiFrequency_kHz;
					c->symbol_rate = pChannel->m_uiSymbolRate_Hz;
					dprintk("%s(blind scan - get info (%d/100) Index=%d, Freq=%dMHz, Sym=%dKHz)\n",
							__FUNCTION__,
							AVL_DVBSx_IBlindscanAPI_GetProgress(&priv->demod.BSsetting),
							priv->demod.index,
							pChannel->m_uiFrequency_kHz/1000,
							pChannel->m_uiSymbolRate_Hz/1000);
					*status = FE_HAS_LOCK | FE_HAS_SIGNAL;
				}
				if (priv->demod.index == priv->demod.BSsetting.m_uiChannelCount)
				{
					if (AVL_DVBSx_IBlindscanAPI_GetProgress(&priv->demod.BSsetting) == 100)
					{
						priv->demod.blindscan_state = BLINDSCAN_STATE_STOP;
					}
					else
					{
						priv->demod.blindscan_state = BLINDSCAN_STATE_START;
					}
				}
				else
				{
					priv->demod.blindscan_state = BLINDSCAN_STATE_GET_PROGRESS;
				}
				break;
			}
			case BLINDSCAN_STATE_CANCEL:
				r = AVL_DVBSx_IBlindScanAPI_Exit(&priv->demod.AVLChip, &priv->demod.BSsetting);
				*status = FE_TIMEDOUT;
				if (AVL_DVBSx_EC_OK == r)
				{
					dprintk("%s(blind scan - cancel)\n", __FUNCTION__);
				}
				else
				{
					eprintk("%s(blind scan - cancel error![%d])\n", __FUNCTION__, r);
				}
				priv->demod.blindscan_state = BLINDSCAN_STATE_STOP;
				break;
		}
	}
	if (priv->demod.blindscan_state == BLINDSCAN_STATE_STOP)
	{
		*delay = 3 * HZ; // 3s
	}
	else
	{
		*delay = 1;
	}
	return 0;
}

static int avl6211_fe_tune(struct dvb_frontend* fe, bool re_tune, unsigned int mode_flags, unsigned int *delay, fe_status_t *status)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	if (re_tune || priv->tune_mode == TUNE_MODE_NORMAL)
	{
		if (priv->demod.blindscan_state != BLINDSCAN_STATE_STOP)
		{
			AVL_DVBSx_IBlindScanAPI_Exit(&priv->demod.AVLChip, &priv->demod.BSsetting);
			priv->demod.blindscan_state = BLINDSCAN_STATE_STOP;
		}
	}

	if (priv->tune_mode == TUNE_MODE_NORMAL)
		avl6211_fe_set_tune(fe, re_tune, mode_flags, delay, status);
	else
		avl6211_fe_blindscan(fe, re_tune, mode_flags, delay, status);

	dprintk("%s\n", __FUNCTION__);

	return 0;
}

static enum dvbfe_algo avl6211_fe_get_frontend_algo(struct dvb_frontend *fe)
{
	return DVBFE_ALGO_HW;
}

static int avl6211_fe_get_frontend(struct dvb_frontend *fe)
{
	//avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	dprintk("%s\n", __FUNCTION__);

	return 0;
}

static int avl6211_fe_read_status(struct dvb_frontend* fe, fe_status_t* status)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	if(AVL6211_GETLockStatus(&priv->demod.AVLChip) == 1)
	{
		*status = FE_HAS_LOCK|FE_HAS_SIGNAL|FE_HAS_CARRIER|FE_HAS_VITERBI|FE_HAS_SYNC;
	}
	else
	{
		*status = 0;
	}

	dprintk("%s(status = %d)\n", __FUNCTION__, *status);

	return  0;
}

static int avl6211_fe_read_ber(struct dvb_frontend* fe, u32* ber)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

#if 0
	*ber = AVL_Get_Quality_Percent(&priv->demod.AVLChip);
#else
	*ber = AVL6211_GETBer(&priv->demod.AVLChip); //This function should only be called if the input signal is a DVBS signal
#endif

	dprintk("%s(ber = %d)\n", __FUNCTION__, *ber);

	return 0;
}

static int avl6211_fe_read_signal_strength(struct dvb_frontend* fe, u16* strength)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

#if 1
	*strength = AVL_Get_Level_Percent(&priv->demod.AVLChip);
#else
	*strength = AVL6211_GETSignalLevel(&priv->demod.AVLChip);
#endif

	dprintk("%s(strength = %d)\n", __FUNCTION__, *strength);

	return 0;
}

static int avl6211_fe_read_snr(struct dvb_frontend* fe, u16* snr)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

#if 0
	*snr = AVL_Get_Quality_Percent(&priv->demod.AVLChip);
#else
	*snr = AVL6211_GETSnr(&priv->demod.AVLChip)/100;
#endif

	dprintk("%s(snr = %d)\n", __FUNCTION__, *snr);

	return 0;
}

static int avl6211_fe_read_ucblocks(struct dvb_frontend* fe, u32* ucblocks)
{
	//avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	avl6211_fe_read_ber(fe, ucblocks);

	dprintk("%s(ucblocks = %d)\n", __FUNCTION__, *ucblocks);

	return 0;
}

static int avl6211_fe_diseqc_reset_overload(struct dvb_frontend* fe)
{
	//avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	dprintk("%s\n", __FUNCTION__);

	return  0;
}

static int avl6211_fe_diseqc_send_master_cmd(struct dvb_frontend* fe, struct dvb_diseqc_master_cmd* cmd)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uchar ucData[8];
	struct AVL_DVBSx_Diseqc_TxStatus TxStatus;
	int i;

	for(i=0;i<cmd->msg_len;i++)
	{
		ucData[i]=cmd->msg[i];
		dprintk("%x ",cmd->msg[i]);
	}
	
	r = AVL_DVBSx_IDiseqc_SendModulationData(ucData, cmd->msg_len, &priv->demod.AVLChip);
	if(r != AVL_DVBSx_EC_OK)
	{
		eprintk("%s(Send data Err:0x%x)\n", __FUNCTION__, r);
		return (r);
	}

	i = 100;
	do
	{
		i--;
		AVL_DVBSx_IBSP_Delay(1);
		r = AVL_DVBSx_IDiseqc_GetTxStatus(&TxStatus, &priv->demod.AVLChip);
	}
	while((TxStatus.m_TxDone != 1) && i);
	if(r != AVL_DVBSx_EC_OK)
	{
		eprintk("%s(Output data Err:0x%x)\n", __FUNCTION__, r);
	}

	dprintk("%s\n", __FUNCTION__);

	return (r);
}

static int avl6211_fe_diseqc_recv_slave_reply(struct dvb_frontend* fe, struct dvb_diseqc_slave_reply* reply)
{
	//avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	dprintk("%s\n", __FUNCTION__);

	return  0;
}

static int avl6211_fe_diseqc_send_burst(struct dvb_frontend* fe, fe_sec_mini_cmd_t minicmd)
{
	#define TONE_COUNT				8

	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
 	struct AVL_DVBSx_Diseqc_TxStatus sTxStatus;
	AVL_uchar ucTone = 0;
	int i;

	if(minicmd == SEC_MINI_A)
		ucTone = 1;
	else if(minicmd == SEC_MINI_B)
		ucTone = 0;

  	r = AVL_DVBSx_IDiseqc_SendTone(ucTone, TONE_COUNT, &priv->demod.AVLChip);
	if(r != AVL_DVBSx_EC_OK)
	{
		eprintk("%s(Send tone %d Err:0x%x)\n", __FUNCTION__, ucTone, r);
		return (r);
	}

	i = 100;
    do
    {
    	i--;
		AVL_DVBSx_IBSP_Delay(1);
	    r = AVL_DVBSx_IDiseqc_GetTxStatus(&sTxStatus, &priv->demod.AVLChip);   //Get current status of the Diseqc transmitter data FIFO.
    }
    while((sTxStatus.m_TxDone != 1) && i);			//Wait until operation finished.
    if(r != AVL_DVBSx_EC_OK)
    {
		eprintk("%s(Output tone %d Err:0x%x)\n", __FUNCTION__, ucTone, r);
    }

	dprintk("%s(minicmd = %d, ret = %d)\n", __FUNCTION__, minicmd, r);

	return (r);
}

static int avl6211_fe_set_tone(struct dvb_frontend* fe, fe_sec_tone_mode_t tone)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	if(tone == SEC_TONE_ON)
	{
		r = AVL_DVBSx_IDiseqc_StartContinuous(&priv->demod.AVLChip);
		if(r != AVL_DVBSx_EC_OK)
		{
			eprintk("%s(Diseqc Start Err:0x%x)\n", __FUNCTION__, r);
		}	
	}
	else
	{
		r = AVL_DVBSx_IDiseqc_StopContinuous(&priv->demod.AVLChip);
		if(r != AVL_DVBSx_EC_OK)
		{
			eprintk("%s(Diseqc Stop Err:0x%x)\n", __FUNCTION__, r);
		}	
	}

	dprintk("%s(tone = %d, ret = %d)\n", __FUNCTION__, tone, r);

	return r;
}

static int avl6211_fe_set_voltage(struct dvb_frontend* fe, fe_sec_voltage_t voltage)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;	
	AVL_uchar nValue = 1;

	if(voltage == SEC_VOLTAGE_OFF)
	{
		AVL6211_LnbPowerCtrl(&priv->demod, 0);//lnb power off
	}
	else
	{
		AVL6211_LnbPowerCtrl(&priv->demod, 1);//lnb power on

		if(voltage ==  SEC_VOLTAGE_13)
			nValue = 0;
		else if(voltage == SEC_VOLTAGE_18)
			nValue = 1;

		r = AVL_DVBSx_IDiseqc_SetLNBOut(nValue, &priv->demod.AVLChip);
		if(r != AVL_DVBSx_EC_OK)
		{
			eprintk("%s(SetLNBOut Err:0x%x)\n", __FUNCTION__, r);
		}
	}

	dprintk("%s(voltage = %d, ret = %d)\n", __FUNCTION__, voltage, r);

	return r;
}

static int avl6211_fe_enable_high_lnb_voltage(struct dvb_frontend* fe, long arg)
{
	//avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	dprintk("%s\n", __FUNCTION__);

	return  0;
}

static int avl6211_fe_i2c_gate_ctrl(struct dvb_frontend *fe, int enable)
{
	//avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	dprintk("%s\n", __FUNCTION__);

	return 0;
}

static int avl6211_fe_set_property(struct dvb_frontend* fe, struct dtv_property* tvp)
{
	avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	switch(tvp->cmd)
	{
	case DTV_CLEAR:
		priv->tune_mode = TUNE_MODE_NORMAL;
		break;
	case DTV_TUNE:
		priv->tune_mode = tvp->u.data;
		break;
	}

	dprintk("%s(CMD = %d)\n", __FUNCTION__, tvp->cmd);

	return 0;
}

static int avl6211_fe_get_property(struct dvb_frontend* fe, struct dtv_property* tvp)
{
	//avl6211_priv_t *priv = (avl6211_priv_t *)fe->demodulator_priv;

	dprintk("%s(CMD = %d)\n", __FUNCTION__, tvp->cmd);

	return 0;
}


/*****************************************************************************
 * AVL6211 OPS
 ******************************************************************************/
static struct dvb_frontend_ops avl6211_fe_ops = {

	.info = {
		.name = "TCC DVB-S2 (AVL6211)",
		.type = FE_QPSK,
		.frequency_min = 850000,
		.frequency_max = 2300000,
		.caps =
			FE_CAN_FEC_1_2 | FE_CAN_FEC_2_3 | FE_CAN_FEC_3_4 |
			FE_CAN_FEC_5_6 | FE_CAN_FEC_7_8 | FE_CAN_FEC_AUTO |
			FE_CAN_QPSK | FE_CAN_QAM_AUTO |
			FE_CAN_TRANSMISSION_MODE_AUTO |
			FE_CAN_GUARD_INTERVAL_AUTO |
			FE_CAN_HIERARCHY_AUTO |
			FE_CAN_RECOVER |
			FE_CAN_MUTE_TS
	},

	.delsys = { SYS_DVBS, SYS_DVBS2 },

	.release = avl6211_fe_release,

	.init = avl6211_fe_init,
	.sleep = avl6211_fe_sleep,

	.tune = avl6211_fe_tune,
	.get_frontend_algo = avl6211_fe_get_frontend_algo,

	.get_frontend = avl6211_fe_get_frontend,

	.read_status = avl6211_fe_read_status,
	.read_ber = avl6211_fe_read_ber,
	.read_signal_strength = avl6211_fe_read_signal_strength,
	.read_snr = avl6211_fe_read_snr,
	.read_ucblocks = avl6211_fe_read_ucblocks,

	.diseqc_reset_overload = avl6211_fe_diseqc_reset_overload,
	.diseqc_send_master_cmd = avl6211_fe_diseqc_send_master_cmd,
	.diseqc_recv_slave_reply = avl6211_fe_diseqc_recv_slave_reply,
	.diseqc_send_burst = avl6211_fe_diseqc_send_burst,
	.set_tone = avl6211_fe_set_tone,
	.set_voltage = avl6211_fe_set_voltage,
	.enable_high_lnb_voltage = avl6211_fe_enable_high_lnb_voltage,
	.i2c_gate_ctrl = avl6211_fe_i2c_gate_ctrl,

	.set_property = avl6211_fe_set_property,
	.get_property = avl6211_fe_get_property,
};


/*****************************************************************************
 * AVL6211 Init
 ******************************************************************************/
static int avl6211_init(struct device_node *node, avl6211_priv_t *priv)
{
	struct device_node *adapter_np;

	adapter_np = of_parse_phandle(node, "i2c-parent", 0);
	if (adapter_np)
	{
		priv->demod.i2c = of_find_i2c_adapter_by_node(adapter_np);
	}

	priv->demod.gpio_lnb_sp1v  = of_get_named_gpio(node, "gpios",   0); // SP1V_SELE
	priv->demod.gpio_lnb_s18v  = of_get_named_gpio(node, "gpios",   1); // S18V
	priv->demod.gpio_fe_power  = of_get_named_gpio(node, "gpios",   2); // SLEEP
	priv->demod.gpio_fe_lock   = of_get_named_gpio(node, "gpios",   3); // LOCK
	priv->demod.gpio_fe_reset  = of_get_named_gpio(node, "gpios",   4); // RESET
	priv->demod.gpio_fe_fault  = of_get_named_gpio(node, "gpios",   5); // FAULT
	priv->demod.gpio_lnb_power = of_get_named_gpio(node, "gpios",   6); // MAIN_VEN(lnb_power)

	of_property_read_u32(node, "config-idx", &priv->demod.config_idx);

	dprintk("%s\n", __FUNCTION__);

	return 0;
}


/*****************************************************************************
 * AVL6211 Probe
 ******************************************************************************/
static int avl6211_probe(tcc_fe_priv_t *inst)
{
	avl6211_priv_t *priv = kzalloc(sizeof(avl6211_priv_t), GFP_KERNEL);
	int retval;

	if (priv == NULL)
	{
		eprintk("%s(kzalloc fail)\n", __FUNCTION__);
		return -1;
	}

	retval = avl6211_init(inst->of_node, priv);
	if (retval != 0)
	{
		eprintk("%s(Init Error %d)\n", __FUNCTION__, retval);
		kfree(priv);
		return -1;
	}

	inst->fe.demodulator_priv = priv;

	dprintk("%s\n", __FUNCTION__);

	return 0;
}


/*****************************************************************************
 * AVL6211 Module Init/Exit
 ******************************************************************************/
static struct tcc_dxb_fe_driver avl6211_driver = {
	.probe = avl6211_probe,
	.compatible = "availink,avl6211",
	.fe_ops = &avl6211_fe_ops,
};

static int __init avl6211_module_init(void)
{
	if (tcc_fe_register(&avl6211_driver) != 0)
		return -EPERM;

	dprintk("%s(date = %s)\n", __FUNCTION__, __DATE__);

	return 0;
}

static void __exit avl6211_module_exit(void)
{
	tcc_fe_unregister(&avl6211_driver);

	dprintk("%s\n", __FUNCTION__);
}

module_init(avl6211_module_init);
module_exit(avl6211_module_exit);

MODULE_DESCRIPTION("TCC DVBS2 (AVL6211)");
MODULE_AUTHOR("C2-G1-3T");
MODULE_LICENSE("GPL");
