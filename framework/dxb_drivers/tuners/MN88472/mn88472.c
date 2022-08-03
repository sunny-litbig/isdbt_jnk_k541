/*
 *  mn88472.c
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

#include "dvb_frontend.h"
#include "tcc_fe.h"

#include "mn88472.h"


/*****************************************************************************
 * Log Message
 ******************************************************************************/
#define LOG_TAG    "[MN88472]"

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


/*****************************************************************************
 * Structures
 ******************************************************************************/
typedef struct mn88472_demod_t {
	struct i2c_adapter *i2c;

	int wait;

	DMD_PARAMETER_t param;
} mn88472_demod_t;

typedef struct mn88472_priv_t {
	struct mn88472_demod_t demod;
} mn88472_priv_t;


/*****************************************************************************
 * Variables
 ******************************************************************************/


/*****************************************************************************
 * External Functions
 ******************************************************************************/


/*****************************************************************************
 * Functions
 ******************************************************************************/
static void mn88472_fe_get_information(struct dvb_frontend* fe)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;

	DMD_get_info(&priv->demod.param, DMD_E_INFO_ALL);

	if (c->delivery_system == SYS_DVBT2)
	{
		// DTV_INVERSION
		switch (priv->demod.param.info[DMD_E_INFO_DVBT2_DAT_PLP_ROTATION])
		{
			case 0:     c->inversion = INVERSION_OFF;  break;
			case 1:     c->inversion = INVERSION_ON;   break;
		}

		// DTV_MODULATION
		switch (priv->demod.param.info[DMD_E_INFO_DVBT2_DAT_PLP_MOD])
		{
			case 0:     c->modulation = QPSK;       break;
			case 1:     c->modulation = QAM_16;     break;
			case 2:     c->modulation = QAM_64;     break;
			case 3:     c->modulation = QAM_256;    break;
		}

		// DTV_INNER_FEC
		switch (priv->demod.param.info[DMD_E_INFO_DVBT2_DAT_PLP_FEC_TYPE])
		{
			case 0:     c->fec_inner = FEC_NONE;  break;
			case 1:     c->fec_inner = FEC_1_2;   break;
		}

		// DTV_GUARD_INTERVAL
		switch (priv->demod.param.info[DMD_E_INFO_DVBT2_GI])
		{
			case 0:     c->guard_interval = GUARD_INTERVAL_1_32;      break;
			case 1:     c->guard_interval = GUARD_INTERVAL_1_16;      break;
			case 2:     c->guard_interval = GUARD_INTERVAL_1_8;       break;
			case 3:     c->guard_interval = GUARD_INTERVAL_1_4;       break;
			case 4:     c->guard_interval = GUARD_INTERVAL_1_128;     break;
			case 5:     c->guard_interval = GUARD_INTERVAL_19_128;    break;
			case 6:     c->guard_interval = GUARD_INTERVAL_19_256;    break;
		}

		// DTV_CODE_RATE_HP
		switch (priv->demod.param.info[DMD_E_INFO_DVBT2_DAT_PLP_COD])
		{
			case 0:     c->code_rate_HP = FEC_1_2;  break;
			case 1:     c->code_rate_HP = FEC_3_5;  break;
			case 2:     c->code_rate_HP = FEC_2_3;  break;
			case 3:     c->code_rate_HP = FEC_3_4;  break;
			case 4:     c->code_rate_HP = FEC_4_5;  break;
			case 5:     c->code_rate_HP = FEC_5_6;  break;
		}

		// DTV_TRANSMISSION_MODE
		switch (priv->demod.param.info[DMD_E_INFO_DVBT2_MODE])
		{
			case 0:     c->transmission_mode = TRANSMISSION_MODE_1K;  break;
			case 1:     c->transmission_mode = TRANSMISSION_MODE_2K;  break;
			case 2:     c->transmission_mode = TRANSMISSION_MODE_4K;  break;
			case 3:     c->transmission_mode = TRANSMISSION_MODE_8K;  break;
			case 4:     c->transmission_mode = TRANSMISSION_MODE_16K; break;
			case 5:     c->transmission_mode = TRANSMISSION_MODE_32K; break;
		}
	}
	else if (c->delivery_system == SYS_DVBT)
	{
		// DTV_MODULATION
		switch (priv->demod.param.info[DMD_E_INFO_DVBT_CONSTELLATION])
		{
			case 0:     c->modulation = QPSK;       break;
			case 1:     c->modulation = QAM_16;     break;
			case 2:     c->modulation = QAM_64;     break;
		}

		// DTV_GUARD_INTERVAL
		switch (priv->demod.param.info[DMD_E_INFO_DVBT_GI])
		{
			case 0:     c->guard_interval = GUARD_INTERVAL_1_32;  break;
			case 1:     c->guard_interval = GUARD_INTERVAL_1_16;  break;
			case 2:     c->guard_interval = GUARD_INTERVAL_1_8;   break;
			case 3:     c->guard_interval = GUARD_INTERVAL_1_4;   break;
		}

		// DTV_CODE_RATE_HP
		switch (priv->demod.param.info[DMD_E_INFO_DVBT_HP_CODERATE])
		{
			case 0:     c->code_rate_HP = FEC_1_2;  break;
			case 1:     c->code_rate_HP = FEC_2_3;  break;
			case 2:     c->code_rate_HP = FEC_3_4;  break;
			case 3:     c->code_rate_HP = FEC_5_6;  break;
			case 4:     c->code_rate_HP = FEC_7_8;  break;
		}

		// DTV_TRANSMISSION_MODE
		switch (priv->demod.param.info[DMD_E_INFO_DVBT_MODE])
		{
			case 0:     c->transmission_mode = TRANSMISSION_MODE_2K;  break;
			case 1:     c->transmission_mode = TRANSMISSION_MODE_8K;  break;
			case 2:     c->transmission_mode = TRANSMISSION_MODE_4K;  break;
		}
	}
}


/*****************************************************************************
 * MN88472 Frontend Functions
 ******************************************************************************/
static void mn88472_fe_release(struct dvb_frontend* fe)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;

	if (priv)
	{
		kfree(priv);
	}

	dprintk("%s\n", __FUNCTION__);
}

static int mn88472_fe_init(struct dvb_frontend* fe)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;

	DMD_open( &priv->demod.param );
	DMD_init( &priv->demod.param );

	priv->demod.param.system = DMD_E_DVBT;
	DMD_set_system( &priv->demod.param );

	dprintk("%s\n", __FUNCTION__);

	return 0;
}

static int mn88472_fe_sleep(struct dvb_frontend* fe)
{
	//mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;

	dprintk("%s\n", __FUNCTION__);

	return 0;
}

static int mn88472_fe_tune(struct dvb_frontend* fe, bool re_tune, unsigned int mode_flags, unsigned int *delay, fe_status_t *status)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;
	int old_bw;

	if (re_tune)
	{
		old_bw = priv->demod.param.bw;

		priv->demod.param.freq = c->frequency; // KHz
		priv->demod.param.funit = DMD_E_KHZ;

		switch(c->bandwidth_hz)
		{
		case 6000000:
			priv->demod.param.bw = DMD_E_BW_6MHZ;
			break;
		case 7000000:
			priv->demod.param.bw = DMD_E_BW_7MHZ;
			break;
		case 8000000:
		default:
			priv->demod.param.bw = DMD_E_BW_8MHZ;
			break;
		}

		if(c->delivery_system == SYS_DVBT)
		{
			// DVBT
			if(priv->demod.param.system == DMD_E_DVBT2 || old_bw != priv->demod.param.bw)
			{
				priv->demod.param.system = DMD_E_DVBT;
				DMD_set_system( &priv->demod.param );
			}
		}
		else if (c->delivery_system == SYS_DVBT2)
		{
			//DVBT2
			if(priv->demod.param.system == DMD_E_DVBT || old_bw != priv->demod.param.bw)
			{
				priv->demod.param.system = DMD_E_DVBT2;
				DMD_set_system( &priv->demod.param );
			}
		}

		DMD_tune( &priv->demod.param );

		priv->demod.wait = 200; // 2s
		*status = 0;

		dprintk("%s(delivery_system = %d, freq = %d KHz, bandwidth = %d Hz)\n", __FUNCTION__, c->delivery_system, c->frequency, c->bandwidth_hz);
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

static enum dvbfe_algo mn88472_fe_get_frontend_algo(struct dvb_frontend *fe)
{
	return DVBFE_ALGO_HW;
}

static int mn88472_fe_get_frontend(struct dvb_frontend *fe)
{
	//mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;

	dprintk("%s\n", __FUNCTION__);

	return 0;
}

static int mn88472_fe_read_status(struct dvb_frontend* fe, fe_status_t* status)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;

	if(DMD_device_scan( &priv->demod.param ) == DMD_E_OK)
	{
		*status = FE_HAS_LOCK|FE_HAS_SIGNAL|FE_HAS_CARRIER|FE_HAS_VITERBI|FE_HAS_SYNC;
	}
	else
	{
		*status = FE_TIMEDOUT;
	}

	dprintk("%s(status = %d)\n", __FUNCTION__, *status);

	return 0;
}

static int mn88472_fe_read_ber(struct dvb_frontend* fe, u32* ber)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;

	DMD_get_info( &priv->demod.param , DMD_E_INFO_BERRNUM );

	if(priv->demod.param.info[DMD_E_INFO_BITNUM] == 0 )
	{
		*ber = 100;
	}
	else
	{
		*ber = (int)(2500 * priv->demod.param.info[DMD_E_INFO_BERRNUM] / priv->demod.param.info[DMD_E_INFO_BITNUM]);
	}

	if (*ber > 100) *ber = 100;

	dprintk("%s(ber = %d)\n", __FUNCTION__, *ber);

	return 0;
}

static int mn88472_fe_read_signal_strength(struct dvb_frontend* fe, u16* strength)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;

	DMD_get_info( &priv->demod.param , DMD_E_INFO_CNR_INT );	

	*strength = priv->demod.param.info[DMD_E_INFO_CNR_INT] * 5;

	if (*strength > 100)       *strength = 100;

	dprintk("%s(strength = %d)\n", __FUNCTION__, *strength);

	return 0;
}

static int mn88472_fe_read_snr(struct dvb_frontend* fe, u16* snr)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;

	DMD_get_info( &priv->demod.param , DMD_E_INFO_CNR_INT );

	*snr = priv->demod.param.info[DMD_E_INFO_CNR_INT];

	dprintk("%s(snr = %d)\n", __FUNCTION__, *snr);

	return 0;
}

static int mn88472_fe_read_ucblocks(struct dvb_frontend* fe, u32* ucblocks)
{
	//mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;
	int ret;

	ret = mn88472_fe_read_ber(fe, ucblocks);

	dprintk("%s(ucblocks = %d, ret = %d)\n", __FUNCTION__, *ucblocks, ret);

	return 0;
}

static int mn88472_fe_i2c_gate_ctrl(struct dvb_frontend* fe, int enable)
{
	//mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;

	dprintk("%s(enable = %d)\n", __FUNCTION__, enable);

	return 0;
}

static int mn88472_fe_set_property(struct dvb_frontend* fe, struct dtv_property* tvp)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;
	const struct dtv_frontend_properties *c = &fe->dtv_property_cache;

	switch(tvp->cmd)
	{
	case DTV_DVBT2_PLP_ID_LEGACY:
		if (c->delivery_system == SYS_DVBT2)
		{
			DMD_set_info( &priv->demod.param , DMD_E_INFO_DVBT2_SELECTED_PLP , tvp->u.data );
		}
		break;
	}
	
	dprintk("%s(CMD = %d)\n", __FUNCTION__, tvp->cmd);

	return 0;
}

static int mn88472_fe_get_property(struct dvb_frontend* fe, struct dtv_property* tvp)
{
	mn88472_priv_t *priv = (mn88472_priv_t *)fe->demodulator_priv;
	const struct dtv_frontend_properties *c = &fe->dtv_property_cache;

	switch(tvp->cmd)
	{
	case DTV_DVBT2_PLP_ID_LEGACY:
		if (c->delivery_system == SYS_DVBT2 && tvp->u.buffer.reserved2 != NULL)
		{
			//Check MPLP information
			unsigned char pPLPIds[256];
			unsigned char pNumPLPs = 0;

			if( DMD_get_dataPLPs( pPLPIds, &pNumPLPs , &priv->demod.param ) == DMD_E_OK )
			{
				if (tvp->u.buffer.len < pNumPLPs)
				{
					pNumPLPs = tvp->u.buffer.len;
				}
				if (copy_to_user((void*)tvp->u.buffer.reserved2, pPLPIds, pNumPLPs) == 0)
				{
					tvp->u.buffer.len = pNumPLPs;
				}
				else
				{
					tvp->u.buffer.len = 0;
				}
			}
			else
			{
				tvp->u.buffer.len = 0;
			}
		}
		break;
	case DTV_FREQUENCY:
		mn88472_fe_get_information(fe);
		break;

	case DTV_PILOT:
		tvp->u.buffer.reserved1[0] = priv->demod.param.info[DMD_E_INFO_DVBT2_PILOT_PATTERN];
		break;
	}

	dprintk("%s(CMD = %d)\n", __FUNCTION__, tvp->cmd);

	return 0;
}


/*****************************************************************************
 * MN88472 OPS
 ******************************************************************************/
static struct dvb_frontend_ops mn88472_fe_ops = {

	.info = {
		.name = "TCC DVB-T2 (MN88472)",
		.type = FE_OFDM,
		.frequency_min = 51000,
		.frequency_max = 858000,
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

	.delsys = { SYS_DVBT, SYS_DVBT2 },

	.release = mn88472_fe_release,

	.init = mn88472_fe_init,
	.sleep = mn88472_fe_sleep,

	.tune = mn88472_fe_tune,
	.get_frontend_algo = mn88472_fe_get_frontend_algo,

	.get_frontend = mn88472_fe_get_frontend,

	.read_status = mn88472_fe_read_status,
	.read_ber = mn88472_fe_read_ber,
	.read_signal_strength = mn88472_fe_read_signal_strength,
	.read_snr = mn88472_fe_read_snr,
	.read_ucblocks = mn88472_fe_read_ucblocks,

	.i2c_gate_ctrl = mn88472_fe_i2c_gate_ctrl,

	.set_property = mn88472_fe_set_property,
	.get_property = mn88472_fe_get_property,
};


/*****************************************************************************
 * MN88472 Init
 ******************************************************************************/
static int mn88472_init(struct device_node *node, mn88472_priv_t *priv)
{
	struct device_node *adapter_np;

	adapter_np = of_parse_phandle(node, "i2c-parent", 0);
	if (adapter_np)
	{
		priv->demod.i2c = of_find_i2c_adapter_by_node(adapter_np);
	}

	dprintk("%s\n", __FUNCTION__);

	return 0;
}

/*****************************************************************************
 * MN88472 Probe
 ******************************************************************************/
static int mn88472_probe(tcc_fe_priv_t *inst)
{
	mn88472_priv_t *priv = kzalloc(sizeof(mn88472_priv_t), GFP_KERNEL);
	int retval;

	if (priv == NULL)
	{
		eprintk("%s(kzalloc fail)\n", __FUNCTION__);
		return -1;
	}

	retval = mn88472_init(inst->of_node, priv);
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
 * MN88472 Module Init/Exit
 ******************************************************************************/
static struct tcc_dxb_fe_driver mn88472_driver = {
	.probe = mn88472_probe,
	.compatible = "panasonic,mn88472",
	.fe_ops = &mn88472_fe_ops,
};

static int __init mn88472_module_init(void)
{
	if (tcc_fe_register(&mn88472_driver) != 0)
		return -EPERM;

	dprintk("%s(date = %s)\n", __FUNCTION__, __DATE__);

	return 0;
}

static void __exit mn88472_module_exit(void)
{
	tcc_fe_unregister(&mn88472_driver);

	dprintk("%s\n", __FUNCTION__);
}

module_init(mn88472_module_init);
module_exit(mn88472_module_exit);

MODULE_DESCRIPTION("TCC DVBT2 (MN88472)");
MODULE_AUTHOR("C2-G1-3T");
MODULE_LICENSE("GPL");
