/*
 *  s5h1411.c
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
#include <linux/videodev2.h>

#include "dvb_frontend.h"
#include "tcc_fe.h"

#include "s5h1411.h"


/*****************************************************************************
 * Log Message
 ******************************************************************************/
#define LOG_TAG    "[S5H1411]"

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
#define TUNE_MODE_DIGITAL             0
#define TUNE_MODE_ANALOG              1

#define S5H1411_I2C_TOP_ADDR (0x32 >> 1)
#define S5H1411_I2C_QAM_ADDR (0x34 >> 1)

// serial/parallel output
#define S5H1411_PARALLEL_OUTPUT 0
#define S5H1411_SERIAL_OUTPUT   1

// GPIO Setting
#define S5H1411_GPIO_OFF 0
#define S5H1411_GPIO_ON  1

// MPEG signal timing
#define S5H1411_MPEGTIMING_CONTINOUS_INVERTING_CLOCK       0
#define S5H1411_MPEGTIMING_CONTINOUS_NONINVERTING_CLOCK    1
#define S5H1411_MPEGTIMING_NONCONTINOUS_INVERTING_CLOCK    2
#define S5H1411_MPEGTIMING_NONCONTINOUS_NONINVERTING_CLOCK 3

// IF Freq for QAM and VSB in KHz
#define S5H1411_IF_3250  3250
#define S5H1411_IF_3500  3500
#define S5H1411_IF_4000  4000
#define S5H1411_IF_5380  5380
#define S5H1411_IF_44000 44000
#define S5H1411_VSB_IF_DEFAULT S5H1411_IF_44000
#define S5H1411_QAM_IF_DEFAULT S5H1411_IF_44000

// Spectral Inversion
#define S5H1411_INVERSION_OFF 0
#define S5H1411_INVERSION_ON  1

// Return lock status based on tuner lock, or demod lock
#define S5H1411_TUNERLOCKING 0
#define S5H1411_DEMODLOCKING 1


/*****************************************************************************
 * Structures
 ******************************************************************************/
struct s5h1411_config {
	u8 output_mode;   // serial/parallel output
	u8 gpio;          // GPIO Setting
	u16 mpeg_timing;  // MPEG signal timing
	u16 qam_if;       // IF Freq for QAM and VSB in KHz
	u16 vsb_if;
	u8 inversion;     // Spectral Inversion
	u8 status_mode;   // Return lock status based on tuner lock, or demod lock
};

typedef struct s5h1411_demod_t {
	struct i2c_adapter *i2c;

	int wait;

	const struct s5h1411_config *config;
	fe_modulation_t current_modulation;
	unsigned int first_tune:1;
	u32 current_frequency;
	int if_freq;
	u8 inversion;

	int gpio_fe_power;
	int gpio_fe_reset;
} s5h1411_demod_t;

typedef struct s5h1411_priv_t {
	struct s5h1411_demod_t demod;
	int tune_mode;
} s5h1411_priv_t;


/*****************************************************************************
 * Variables
 ******************************************************************************/
/* Register values to initialise the demod, defaults to VSB */
static struct init_tab {
	u8	addr;
	u8	reg;
	u16	data;
} init_tab[] = {
	{ S5H1411_I2C_TOP_ADDR, 0x00, 0x0071, },
	{ S5H1411_I2C_TOP_ADDR, 0x08, 0x0047, },
	{ S5H1411_I2C_TOP_ADDR, 0x1c, 0x0400, },
	{ S5H1411_I2C_TOP_ADDR, 0x1e, 0x0370, },
	{ S5H1411_I2C_TOP_ADDR, 0x1f, 0x342c, },
	{ S5H1411_I2C_TOP_ADDR, 0x24, 0x0231, },
	{ S5H1411_I2C_TOP_ADDR, 0x25, 0x1011, },
	{ S5H1411_I2C_TOP_ADDR, 0x26, 0x0f07, },
	{ S5H1411_I2C_TOP_ADDR, 0x27, 0x0f04, },
	{ S5H1411_I2C_TOP_ADDR, 0x28, 0x070f, },
	{ S5H1411_I2C_TOP_ADDR, 0x29, 0x2820, },
	{ S5H1411_I2C_TOP_ADDR, 0x2a, 0x102e, },
	{ S5H1411_I2C_TOP_ADDR, 0x2b, 0x0220, },
	{ S5H1411_I2C_TOP_ADDR, 0x2e, 0x0d0e, },
	{ S5H1411_I2C_TOP_ADDR, 0x2f, 0x1013, },
	{ S5H1411_I2C_TOP_ADDR, 0x31, 0x171b, },
	{ S5H1411_I2C_TOP_ADDR, 0x32, 0x0e0f, },
	{ S5H1411_I2C_TOP_ADDR, 0x33, 0x0f10, },
	{ S5H1411_I2C_TOP_ADDR, 0x34, 0x170e, },
	{ S5H1411_I2C_TOP_ADDR, 0x35, 0x4b10, },
	{ S5H1411_I2C_TOP_ADDR, 0x36, 0x0f17, },
	{ S5H1411_I2C_TOP_ADDR, 0x3c, 0x1577, },
	{ S5H1411_I2C_TOP_ADDR, 0x3d, 0x081a, },
	{ S5H1411_I2C_TOP_ADDR, 0x3e, 0x77ee, },
	{ S5H1411_I2C_TOP_ADDR, 0x40, 0x1e09, },
	{ S5H1411_I2C_TOP_ADDR, 0x41, 0x0f0c, },
	{ S5H1411_I2C_TOP_ADDR, 0x42, 0x1f10, },
	{ S5H1411_I2C_TOP_ADDR, 0x4d, 0x0509, },
	{ S5H1411_I2C_TOP_ADDR, 0x4e, 0x0a00, },
	{ S5H1411_I2C_TOP_ADDR, 0x50, 0x0000, },
	{ S5H1411_I2C_TOP_ADDR, 0x5b, 0x0000, },
	{ S5H1411_I2C_TOP_ADDR, 0x5c, 0x0008, },
	{ S5H1411_I2C_TOP_ADDR, 0x57, 0x1101, },
	{ S5H1411_I2C_TOP_ADDR, 0x65, 0x007c, },
	{ S5H1411_I2C_TOP_ADDR, 0x68, 0x0512, },
	{ S5H1411_I2C_TOP_ADDR, 0x69, 0x0258, },
	{ S5H1411_I2C_TOP_ADDR, 0x70, 0x0004, },
	{ S5H1411_I2C_TOP_ADDR, 0x71, 0x0007, },
	{ S5H1411_I2C_TOP_ADDR, 0x76, 0x00a9, },
	{ S5H1411_I2C_TOP_ADDR, 0x78, 0x3141, },
	{ S5H1411_I2C_TOP_ADDR, 0x7a, 0x3141, },
	{ S5H1411_I2C_TOP_ADDR, 0xb3, 0x8003, },
	{ S5H1411_I2C_TOP_ADDR, 0xb5, 0xa6bb, },
	{ S5H1411_I2C_TOP_ADDR, 0xb6, 0x0609, },
	{ S5H1411_I2C_TOP_ADDR, 0xb7, 0x2f06, },
	{ S5H1411_I2C_TOP_ADDR, 0xb8, 0x003f, },
	{ S5H1411_I2C_TOP_ADDR, 0xb9, 0x2700, },
	{ S5H1411_I2C_TOP_ADDR, 0xba, 0xfac8, },
	{ S5H1411_I2C_TOP_ADDR, 0xbe, 0x1003, },
	{ S5H1411_I2C_TOP_ADDR, 0xbf, 0x103f, },
	{ S5H1411_I2C_TOP_ADDR, 0xce, 0x2000, },
	{ S5H1411_I2C_TOP_ADDR, 0xcf, 0x0800, },
	{ S5H1411_I2C_TOP_ADDR, 0xd0, 0x0800, },
	{ S5H1411_I2C_TOP_ADDR, 0xd1, 0x0400, },
	{ S5H1411_I2C_TOP_ADDR, 0xd2, 0x0800, },
	{ S5H1411_I2C_TOP_ADDR, 0xd3, 0x2000, },
	{ S5H1411_I2C_TOP_ADDR, 0xd4, 0x3000, },
	{ S5H1411_I2C_TOP_ADDR, 0xdb, 0x4a9b, },
	{ S5H1411_I2C_TOP_ADDR, 0xdc, 0x1000, },
	{ S5H1411_I2C_TOP_ADDR, 0xde, 0x0001, },
	{ S5H1411_I2C_TOP_ADDR, 0xdf, 0x0000, },
	{ S5H1411_I2C_TOP_ADDR, 0xe3, 0x0301, },
	{ S5H1411_I2C_QAM_ADDR, 0xf3, 0x0000, },
	{ S5H1411_I2C_QAM_ADDR, 0xf3, 0x0001, },
	{ S5H1411_I2C_QAM_ADDR, 0x08, 0x0600, },
	{ S5H1411_I2C_QAM_ADDR, 0x18, 0x4201, },
	{ S5H1411_I2C_QAM_ADDR, 0x1e, 0x6476, },
	{ S5H1411_I2C_QAM_ADDR, 0x21, 0x0830, },
	{ S5H1411_I2C_QAM_ADDR, 0x0c, 0x5679, },
	{ S5H1411_I2C_QAM_ADDR, 0x0d, 0x579b, },
	{ S5H1411_I2C_QAM_ADDR, 0x24, 0x0102, },
	{ S5H1411_I2C_QAM_ADDR, 0x31, 0x7488, },
	{ S5H1411_I2C_QAM_ADDR, 0x32, 0x0a08, },
	{ S5H1411_I2C_QAM_ADDR, 0x3d, 0x8689, },
	{ S5H1411_I2C_QAM_ADDR, 0x49, 0x0048, },
	{ S5H1411_I2C_QAM_ADDR, 0x57, 0x2012, },
	{ S5H1411_I2C_QAM_ADDR, 0x5d, 0x7676, },
	{ S5H1411_I2C_QAM_ADDR, 0x04, 0x0400, },
	{ S5H1411_I2C_QAM_ADDR, 0x58, 0x00c0, },
	{ S5H1411_I2C_QAM_ADDR, 0x5b, 0x0100, },
};

/* VSB SNR lookup table */
static struct vsb_snr_tab {
	u16	val;
	u16	data;
} vsb_snr_tab[] = {
	{  0x39f, 300, },
	{  0x39b, 295, },
	{  0x397, 290, },
	{  0x394, 285, },
	{  0x38f, 280, },
	{  0x38b, 275, },
	{  0x387, 270, },
	{  0x382, 265, },
	{  0x37d, 260, },
	{  0x377, 255, },
	{  0x370, 250, },
	{  0x36a, 245, },
	{  0x364, 240, },
	{  0x35b, 235, },
	{  0x353, 230, },
	{  0x349, 225, },
	{  0x340, 320, },
	{  0x337, 215, },
	{  0x327, 210, },
	{  0x31b, 205, },
	{  0x310, 200, },
	{  0x302, 195, },
	{  0x2f3, 190, },
	{  0x2e4, 185, },
	{  0x2d7, 180, },
	{  0x2cd, 175, },
	{  0x2bb, 170, },
	{  0x2a9, 165, },
	{  0x29e, 160, },
	{  0x284, 155, },
	{  0x27a, 150, },
	{  0x260, 145, },
	{  0x23a, 140, },
	{  0x224, 135, },
	{  0x213, 130, },
	{  0x204, 125, },
	{  0x1fe, 120, },
	{      0,   0, },
};

/* QAM64 SNR lookup table */
static struct qam64_snr_tab {
	u16	val;
	u16	data;
} qam64_snr_tab[] = {
	{  0x0001,   0, },
	{  0x0af0, 300, },
	{  0x0d80, 290, },
	{  0x10a0, 280, },
	{  0x14b5, 270, },
	{  0x1590, 268, },
	{  0x1680, 266, },
	{  0x17b0, 264, },
	{  0x18c0, 262, },
	{  0x19b0, 260, },
	{  0x1ad0, 258, },
	{  0x1d00, 256, },
	{  0x1da0, 254, },
	{  0x1ef0, 252, },
	{  0x2050, 250, },
	{  0x20f0, 249, },
	{  0x21d0, 248, },
	{  0x22b0, 247, },
	{  0x23a0, 246, },
	{  0x2470, 245, },
	{  0x24f0, 244, },
	{  0x25a0, 243, },
	{  0x26c0, 242, },
	{  0x27b0, 241, },
	{  0x28d0, 240, },
	{  0x29b0, 239, },
	{  0x2ad0, 238, },
	{  0x2ba0, 237, },
	{  0x2c80, 236, },
	{  0x2d20, 235, },
	{  0x2e00, 234, },
	{  0x2f10, 233, },
	{  0x3050, 232, },
	{  0x3190, 231, },
	{  0x3300, 230, },
	{  0x3340, 229, },
	{  0x3200, 228, },
	{  0x3550, 227, },
	{  0x3610, 226, },
	{  0x3600, 225, },
	{  0x3700, 224, },
	{  0x3800, 223, },
	{  0x3920, 222, },
	{  0x3a20, 221, },
	{  0x3b30, 220, },
	{  0x3d00, 219, },
	{  0x3e00, 218, },
	{  0x4000, 217, },
	{  0x4100, 216, },
	{  0x4300, 215, },
	{  0x4400, 214, },
	{  0x4600, 213, },
	{  0x4700, 212, },
	{  0x4800, 211, },
	{  0x4a00, 210, },
	{  0x4b00, 209, },
	{  0x4d00, 208, },
	{  0x4f00, 207, },
	{  0x5050, 206, },
	{  0x5200, 205, },
	{  0x53c0, 204, },
	{  0x5450, 203, },
	{  0x5650, 202, },
	{  0x5820, 201, },
	{  0x6000, 200, },
	{  0xffff,   0, },
};

/* QAM256 SNR lookup table */
static struct qam256_snr_tab {
	u16	val;
	u16	data;
} qam256_snr_tab[] = {
	{  0x0001,   0, },
	{  0x0970, 400, },
	{  0x0a90, 390, },
	{  0x0b90, 380, },
	{  0x0d90, 370, },
	{  0x0ff0, 360, },
	{  0x1240, 350, },
	{  0x1345, 348, },
	{  0x13c0, 346, },
	{  0x14c0, 344, },
	{  0x1500, 342, },
	{  0x1610, 340, },
	{  0x1700, 338, },
	{  0x1800, 336, },
	{  0x18b0, 334, },
	{  0x1900, 332, },
	{  0x1ab0, 330, },
	{  0x1bc0, 328, },
	{  0x1cb0, 326, },
	{  0x1db0, 324, },
	{  0x1eb0, 322, },
	{  0x2030, 320, },
	{  0x2200, 318, },
	{  0x2280, 316, },
	{  0x2410, 314, },
	{  0x25b0, 312, },
	{  0x27a0, 310, },
	{  0x2840, 308, },
	{  0x29d0, 306, },
	{  0x2b10, 304, },
	{  0x2d30, 302, },
	{  0x2f20, 300, },
	{  0x30c0, 298, },
	{  0x3260, 297, },
	{  0x32c0, 296, },
	{  0x3300, 295, },
	{  0x33b0, 294, },
	{  0x34b0, 293, },
	{  0x35a0, 292, },
	{  0x3650, 291, },
	{  0x3800, 290, },
	{  0x3900, 289, },
	{  0x3a50, 288, },
	{  0x3b30, 287, },
	{  0x3cb0, 286, },
	{  0x3e20, 285, },
	{  0x3fa0, 284, },
	{  0x40a0, 283, },
	{  0x41c0, 282, },
	{  0x42f0, 281, },
	{  0x44a0, 280, },
	{  0x4600, 279, },
	{  0x47b0, 278, },
	{  0x4900, 277, },
	{  0x4a00, 276, },
	{  0x4ba0, 275, },
	{  0x4d00, 274, },
	{  0x4f00, 273, },
	{  0x5000, 272, },
	{  0x51f0, 272, },
	{  0x53a0, 270, },
	{  0x5520, 269, },
	{  0x5700, 268, },
	{  0x5800, 267, },
	{  0x5a00, 266, },
	{  0x5c00, 265, },
	{  0x5d00, 264, },
	{  0x5f00, 263, },
	{  0x6000, 262, },
	{  0x6200, 261, },
	{  0x6400, 260, },
	{  0xffff,   0, },
};


/*****************************************************************************
 * External Functions
 ******************************************************************************/


/*****************************************************************************
 * Functions
 ******************************************************************************/
static int s5h1411_writereg(s5h1411_demod_t *demod, u8 addr, u8 reg, u16 data)
{
	int ret;
	u8 buf[] = { reg, data >> 8,  data & 0xff };

	struct i2c_msg msg = { .addr = addr, .flags = 0, .buf = buf, .len = 3 };

	ret = i2c_transfer(demod->i2c, &msg, 1);

	if (ret != 1)
		printk(KERN_ERR "%s: writereg error 0x%02x 0x%02x 0x%04x, "
		       "ret == %i)\n", __func__, addr, reg, data, ret);

	return (ret != 1) ? -1 : 0;
}

static u16 s5h1411_readreg(s5h1411_demod_t *demod, u8 addr, u8 reg)
{
	int ret;
	u8 b0[] = { reg };
	u8 b1[] = { 0, 0 };

	struct i2c_msg msg[] = {
		{ .addr = addr, .flags = 0, .buf = b0, .len = 1 },
		{ .addr = addr, .flags = I2C_M_RD, .buf = b1, .len = 2 } };

	ret = i2c_transfer(demod->i2c, msg, 2);

	if (ret <= 0)
		printk(KERN_ERR "%s: readreg error (ret == %i)\n",
			__func__, ret);
	return (b1[0] << 8) | b1[1];
}

static int s5h1411_softreset(s5h1411_demod_t *demod)
{
	s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xf7, 0);
	s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xf7, 1);
	return 0;
}

static int s5h1411_set_if_freq(s5h1411_demod_t *demod, int KHz)
{
	switch (KHz) {
	case 3250:
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x38, 0x10d5);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x39, 0x5342);
		s5h1411_writereg(demod, S5H1411_I2C_QAM_ADDR, 0x2c, 0x10d9);
		break;
	case 3500:
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x38, 0x1225);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x39, 0x1e96);
		s5h1411_writereg(demod, S5H1411_I2C_QAM_ADDR, 0x2c, 0x1225);
		break;
	case 4000:
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x38, 0x14bc);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x39, 0xb53e);
		s5h1411_writereg(demod, S5H1411_I2C_QAM_ADDR, 0x2c, 0x14bd);
		break;
	case 5380:
	case 44000:
	default:
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x38, 0x1be4);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x39, 0x3655);
		s5h1411_writereg(demod, S5H1411_I2C_QAM_ADDR, 0x2c, 0x1be4);
		break;
	}

	demod->if_freq = KHz;

	return 0;
}

static int s5h1411_set_mpeg_timing(s5h1411_demod_t *demod, int mode)
{
	u16 val;

	val = s5h1411_readreg(demod, S5H1411_I2C_TOP_ADDR, 0xbe) & 0xcfff;

	switch (mode) {
	case S5H1411_MPEGTIMING_CONTINOUS_INVERTING_CLOCK:
		val |= 0x0000;
		break;
	case S5H1411_MPEGTIMING_CONTINOUS_NONINVERTING_CLOCK:
		val |= 0x1000;
		break;
	case S5H1411_MPEGTIMING_NONCONTINOUS_INVERTING_CLOCK:
		val |= 0x2000;
		break;
	case S5H1411_MPEGTIMING_NONCONTINOUS_NONINVERTING_CLOCK:
		val |= 0x3000;
		break;
	default:
		return -EINVAL;
	}

	/* Configure MPEG Signal Timing charactistics */
	return s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xbe, val);
}

static int s5h1411_set_spectralinversion(s5h1411_demod_t *demod, int inversion)
{
	u16 val;

	val = s5h1411_readreg(demod, S5H1411_I2C_TOP_ADDR, 0x24) & ~0x1000;

	if (inversion == 1)
		val |= 0x1000; /* Inverted */

	demod->inversion = inversion;

	return s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x24, val);
}

static int s5h1411_set_serialmode(s5h1411_demod_t *demod, int serial)
{
	u16 val;

	val = s5h1411_readreg(demod, S5H1411_I2C_TOP_ADDR, 0xbd) & ~0x100;

	if (serial == 1)
		val |= 0x100;

	return s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xbd, val);
}

static int s5h1411_enable_modulation(s5h1411_demod_t *demod, fe_modulation_t m)
{
	if ((demod->first_tune == 0) && (m == demod->current_modulation)) {
		return 0;
	}

	switch (m) {
	case VSB_8:
		s5h1411_set_if_freq(demod, demod->config->vsb_if);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x00, 0x71);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xf6, 0x00);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xcd, 0xf1);
		break;
	case QAM_64:
	case QAM_256:
	case QAM_AUTO:
		s5h1411_set_if_freq(demod, demod->config->qam_if);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0x00, 0x0171);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xf6, 0x0001);
		s5h1411_writereg(demod, S5H1411_I2C_QAM_ADDR, 0x16, 0x1101);
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xcd, 0x00f0);
		break;
	default:
		return -EINVAL;
	}

	demod->current_modulation = m;
	demod->first_tune = 0;

	s5h1411_softreset(demod);

	return 0;
}

static int s5h1411_i2c_gate_ctrl(s5h1411_demod_t *demod, int enable)
{
	if (enable)
		return s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xf5, 1);
	else
		return s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xf5, 0);
}

static int s5h1411_set_gpio(s5h1411_demod_t *demod, int enable)
{
	u16 val;

	val = s5h1411_readreg(demod, S5H1411_I2C_TOP_ADDR, 0xe0) & ~0x02;

	if (enable)
		return s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xe0, val | 0x02);
	else
		return s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xe0, val);
}

static int s5h1411_set_powerstate(s5h1411_demod_t *demod, int enable)
{
	if (enable)
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xf4, 1);
	else {
		s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xf4, 0);
		s5h1411_softreset(demod);
	}

	return 0;
}

static int s5h1411_register_reset(s5h1411_demod_t *demod)
{
	return s5h1411_writereg(demod, S5H1411_I2C_TOP_ADDR, 0xf3, 0);
}

static int s5h1411_vsb_lookup_snr(s5h1411_demod_t *demod, u16 *snr, u16 v)
{
	int i, ret = -EINVAL;
	dprintk("%s()\n", __func__);

	for (i = 0; i < ARRAY_SIZE(vsb_snr_tab); i++) {
		if (v > vsb_snr_tab[i].val) {
			*snr = vsb_snr_tab[i].data;
			ret = 0;
			break;
		}
	}
	dprintk("%s() snr=%d\n", __func__, *snr);
	return ret;
}

static int s5h1411_qam256_lookup_snr(s5h1411_demod_t *demod, u16 *snr, u16 v)
{
	int i, ret = -EINVAL;
	dprintk("%s()\n", __func__);

	for (i = 0; i < ARRAY_SIZE(qam256_snr_tab); i++) {
		if (v < qam256_snr_tab[i].val) {
			*snr = qam256_snr_tab[i].data;
			ret = 0;
			break;
		}
	}
	return ret;
}

static int s5h1411_qam64_lookup_snr(s5h1411_demod_t *demod, u16 *snr, u16 v)
{
	int i, ret = -EINVAL;
	dprintk("%s()\n", __func__);

	for (i = 0; i < ARRAY_SIZE(qam64_snr_tab); i++) {
		if (v < qam64_snr_tab[i].val) {
			*snr = qam64_snr_tab[i].data;
			ret = 0;
			break;
		}
	}
	return ret;
}

static int s5h1411_read_snr(s5h1411_demod_t *demod, u16 *snr)
{
	u16 reg;

	switch (demod->current_modulation) {
	case QAM_64:
		reg = s5h1411_readreg(demod, S5H1411_I2C_TOP_ADDR, 0xf1);
		return s5h1411_qam64_lookup_snr(demod, snr, reg);
	case QAM_256:
		reg = s5h1411_readreg(demod, S5H1411_I2C_TOP_ADDR, 0xf1);
		return s5h1411_qam256_lookup_snr(demod, snr, reg);
	case VSB_8:
		reg = s5h1411_readreg(demod, S5H1411_I2C_TOP_ADDR,
			0xf2) & 0x3ff;
		return s5h1411_vsb_lookup_snr(demod, snr, reg);
	default:
		break;
	}
	return -EINVAL;
}

static int s5h1411_read_ucblocks(s5h1411_demod_t *demod, u32 *ucblocks)
{
	*ucblocks = s5h1411_readreg(demod, S5H1411_I2C_TOP_ADDR, 0xc9);

	return 0;
}

static int s5h1411_hw_reset(s5h1411_demod_t *demod)
{
	tcc_gpio_config(demod->gpio_fe_reset, GPIO_FN(0));
	gpio_request(demod->gpio_fe_reset, NULL);
	gpio_direction_output(demod->gpio_fe_reset, 1);
	msleep(50);
	gpio_set_value(demod->gpio_fe_reset, 0);
	msleep(50);
	gpio_set_value(demod->gpio_fe_reset, 1);

	return 0;
}

static int s5h1411_power(s5h1411_demod_t *demod, int enable)
{
	if (enable)
	{
		tcc_gpio_config(demod->gpio_fe_power, GPIO_FN(0));
		gpio_request(demod->gpio_fe_power, NULL);
		gpio_direction_output(demod->gpio_fe_power, 1);
	}
	else
	{
		tcc_gpio_config(demod->gpio_fe_power, GPIO_FN(0));
		gpio_request(demod->gpio_fe_power, NULL);
		gpio_set_value(demod->gpio_fe_power, 0);
	}

	return 0;
}


/*****************************************************************************
 * S5H1411 Frontend Functions
 ******************************************************************************/
static void s5h1411_fe_release(struct dvb_frontend* fe)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;

	if (priv)
	{
		s5h1411_power(&priv->demod, 0);

		kfree(priv);
	}

	dprintk("%s\n", __FUNCTION__);
}

static int s5h1411_fe_init(struct dvb_frontend* fe)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;
	int i;

	s5h1411_set_powerstate(&priv->demod, 0);
	s5h1411_register_reset(&priv->demod);

	for (i = 0; i < ARRAY_SIZE(init_tab); i++)
		s5h1411_writereg(&priv->demod, init_tab[i].addr, init_tab[i].reg, init_tab[i].data);

	priv->demod.current_modulation = VSB_8;
	priv->demod.first_tune = 1;

	if (priv->demod.config->output_mode == S5H1411_SERIAL_OUTPUT)
		s5h1411_set_serialmode(&priv->demod, 1);
	else
		s5h1411_set_serialmode(&priv->demod, 0);

	s5h1411_set_spectralinversion(&priv->demod, priv->demod.config->inversion);
	s5h1411_set_if_freq(&priv->demod, priv->demod.config->vsb_if);
	s5h1411_set_gpio(&priv->demod, priv->demod.config->gpio);
	s5h1411_set_mpeg_timing(&priv->demod, priv->demod.config->mpeg_timing);
	s5h1411_softreset(&priv->demod);

	s5h1411_i2c_gate_ctrl(&priv->demod, 0);

	dprintk("%s\n", __FUNCTION__);

	return 0;
}

static int s5h1411_fe_sleep(struct dvb_frontend* fe)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;

	s5h1411_set_powerstate(&priv->demod, 1);

	dprintk("%s\n", __FUNCTION__);

	return 0;
}

static int s5h1411_fe_tune(struct dvb_frontend* fe, bool re_tune, unsigned int mode_flags, unsigned int *delay, fe_status_t *status)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;
	struct analog_parameters params;

	if (re_tune)
	{
		s5h1411_softreset(&priv->demod);

		priv->demod.current_frequency = c->frequency;

		s5h1411_enable_modulation(&priv->demod, c->modulation);

		if (priv->tune_mode == TUNE_MODE_DIGITAL)
		{
			if (fe->ops.tuner_ops.set_params)
			{
				if (fe->ops.i2c_gate_ctrl)
					fe->ops.i2c_gate_ctrl(fe, 1);

				fe->ops.tuner_ops.set_params(fe);

				if (fe->ops.i2c_gate_ctrl)
					fe->ops.i2c_gate_ctrl(fe, 0);
			}
		}
		else // TUNE_MODE_ANALOG
		{
			if(fe->ops.tuner_ops.set_analog_params)
			{
				params.mode = V4L2_TUNER_ANALOG_TV;
				params.std = V4L2_STD_MN;
				params.frequency = (unsigned int)(c->frequency / 62500);

				if(fe->ops.i2c_gate_ctrl)
					fe->ops.i2c_gate_ctrl(fe, 1);

				fe->ops.tuner_ops.set_analog_params(fe, &params);
	
				if(fe->ops.i2c_gate_ctrl)
					fe->ops.i2c_gate_ctrl(fe, 0);
			}
		}

		s5h1411_softreset(&priv->demod);

		priv->demod.wait = 200; // 2s
		*status = 0;

		dprintk("%s(frequency = %d Hz, modulation = %d, tune_mode = %d)\n", __FUNCTION__, c->frequency, c->modulation, priv->tune_mode);
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

static enum dvbfe_algo s5h1411_fe_get_frontend_algo(struct dvb_frontend *fe)
{
	return DVBFE_ALGO_HW;
}

static int s5h1411_fe_get_frontend(struct dvb_frontend* fe)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;

	c->frequency = priv->demod.current_frequency;
	c->modulation = priv->demod.current_modulation;

	dprintk("%s(frequency = %d Hz, modulation = %d)\n", __FUNCTION__, c->frequency, c->modulation);

	return 0;
}

static int s5h1411_fe_read_status(struct dvb_frontend* fe, fe_status_t* status)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;
	u16 reg;
	u32 tuner_status = 0;

	*status = 0;

	switch (priv->demod.current_modulation)
	{
	case QAM_64:
	case QAM_256:
		reg = s5h1411_readreg(&priv->demod, S5H1411_I2C_TOP_ADDR, 0xf0);
		if (reg & 0x10) /* QAM FEC Lock */
			*status |= FE_HAS_SYNC | FE_HAS_LOCK;
		if (reg & 0x100) /* QAM EQ Lock */
			*status |= FE_HAS_VITERBI | FE_HAS_CARRIER | FE_HAS_SIGNAL;
		break;
	case VSB_8:
		reg = s5h1411_readreg(&priv->demod, S5H1411_I2C_TOP_ADDR, 0xf2);
		if (reg & 0x1000) /* FEC Lock */
			*status |= FE_HAS_SYNC | FE_HAS_LOCK;
		if (reg & 0x2000) /* EQ Lock */
			*status |= FE_HAS_VITERBI | FE_HAS_CARRIER | FE_HAS_SIGNAL;

		reg = s5h1411_readreg(&priv->demod, S5H1411_I2C_TOP_ADDR, 0x53);
		if (reg & 0x1) /* AFC Lock */
			*status |= FE_HAS_SIGNAL;
		break;
	default:
		return -EINVAL;
	}

	switch (priv->demod.config->status_mode)
	{
	case S5H1411_DEMODLOCKING:
		if (*status & FE_HAS_VITERBI)
			*status |= FE_HAS_CARRIER | FE_HAS_SIGNAL;
		break;
	case S5H1411_TUNERLOCKING:
		if (fe->ops.tuner_ops.get_status) {
			if (fe->ops.i2c_gate_ctrl)
				fe->ops.i2c_gate_ctrl(fe, 1);

			fe->ops.tuner_ops.get_status(fe, &tuner_status);

			if (fe->ops.i2c_gate_ctrl)
				fe->ops.i2c_gate_ctrl(fe, 0);
		}
		if (tuner_status)
			*status |= FE_HAS_CARRIER | FE_HAS_SIGNAL;
		break;
	}

	dprintk("%s(status = %d)\n", __FUNCTION__, *status);

	return  0;
}

static int s5h1411_fe_read_ber(struct dvb_frontend* fe, u32* ber)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;
	int ret;

	ret = s5h1411_read_ucblocks(&priv->demod, ber);

	dprintk("%s(ber = %d, ret = %d)\n", __FUNCTION__, *ber, ret);

	return ret;
}

static int s5h1411_fe_read_signal_strength(struct dvb_frontend* fe, u16* strength)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;
	int ret;
	u16 snr;
	u32 tmp;

	ret = s5h1411_read_snr(&priv->demod, &snr);
	if (0 == ret)
	{
		/* Convert from SNR in dB * 10 to 8.24 fixed-point */
		tmp = (snr * ((1 << 24) / 10));

		/* Convert from 8.24 fixed-point to
		 * scale the range 0 - 35*2^24 into 0 - 65535*/
		if (tmp >= 8960 * 0x10000)
			*strength = 0xffff;
		else
			*strength = tmp / 8960;
	}

	dprintk("%s(strength = %d, ret = %d)\n", __FUNCTION__, *strength, ret);

	return ret;
}

static int s5h1411_fe_read_snr(struct dvb_frontend* fe, u16* snr)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;
	int ret;

	ret = s5h1411_read_snr(&priv->demod, snr);

	dprintk("%s(snr = %d, ret = %d)\n", __FUNCTION__, *snr, ret);

	return ret;
}

static int s5h1411_fe_read_ucblocks(struct dvb_frontend* fe, u32* ucblocks)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;
	int ret;

	ret = s5h1411_read_ucblocks(&priv->demod, ucblocks);

	dprintk("%s(ucblocks = %d, ret = %d)\n", __FUNCTION__, *ucblocks, ret);

	return ret;
}

static int s5h1411_fe_i2c_gate_ctrl(struct dvb_frontend* fe, int enable)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;
	int ret;

	ret = s5h1411_i2c_gate_ctrl(&priv->demod, enable);

	dprintk("%s(enable = %d, ret = %d)\n", __FUNCTION__, enable, ret);

	return ret;
}

static int s5h1411_fe_set_property(struct dvb_frontend* fe, struct dtv_property* tvp)
{
	s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;

	switch(tvp->cmd)
	{
	case DTV_TUNE:
		priv->tune_mode = tvp->u.data;
		break;
	}

	dprintk("%s(CMD = %d)\n", __FUNCTION__, tvp->cmd);

	return 0;
}

static int s5h1411_fe_get_property(struct dvb_frontend* fe, struct dtv_property* tvp)
{
	//s5h1411_priv_t *priv = (s5h1411_priv_t *)fe->demodulator_priv;

	dprintk("%s(CMD = %d)\n", __FUNCTION__, tvp->cmd);

	return 0;
}

static s5h1411_priv_t *gPriv;
static int s5h1411_fe_callback(void *adapter_priv, int component, int cmd, int arg)
{
	s5h1411_priv_t *priv = gPriv;
	unsigned short val;

	if (component == DVB_FRONTEND_COMPONENT_TUNER)
	{
		if (cmd == XC5000_TUNER_RESET)
		{
			val = s5h1411_readreg(&priv->demod, S5H1411_I2C_TOP_ADDR, 0xe0) & ~0x02;

			s5h1411_writereg(&priv->demod, S5H1411_I2C_TOP_ADDR, 0xe0, val);
			msleep(330);

			s5h1411_writereg(&priv->demod, S5H1411_I2C_TOP_ADDR, 0xe0, val | 0x02);
			msleep(330);
		}
	}

	dprintk("%s(component = %d, cmd = %d, arg = %d)\n", __FUNCTION__, component, cmd, arg);

	return 0;
}


/*****************************************************************************
 * S5H1411 OPS
 ******************************************************************************/
static struct dvb_frontend_ops s5h1411_fe_ops = {

	.info = {
		.name = "Samsung S5H1411 QAM/8VSB Frontend",
		.type = FE_QPSK,
		.frequency_min = 54000000,
		.frequency_max = 858000000,
		.frequency_stepsize	= 62500,
		.caps =
			FE_CAN_QAM_64 | FE_CAN_QAM_256 | FE_CAN_8VSB
	},

	.delsys = { SYS_ATSC, SYS_DVBC_ANNEX_B },

	.release = s5h1411_fe_release,

	.init = s5h1411_fe_init,
	.sleep = s5h1411_fe_sleep,

	.tune = s5h1411_fe_tune,
	.get_frontend_algo = s5h1411_fe_get_frontend_algo,

	.get_frontend = s5h1411_fe_get_frontend,

	.read_status = s5h1411_fe_read_status,
	.read_ber = s5h1411_fe_read_ber,
	.read_signal_strength = s5h1411_fe_read_signal_strength,
	.read_snr = s5h1411_fe_read_snr,
	.read_ucblocks = s5h1411_fe_read_ucblocks,

	.i2c_gate_ctrl = s5h1411_fe_i2c_gate_ctrl,

	.set_property = s5h1411_fe_set_property,
	.get_property = s5h1411_fe_get_property,
};


/*****************************************************************************
 * XC5000 Tuner Config
 ******************************************************************************/
static struct xc5000_config tuner_cfg = {
        .i2c_address      = 0x61,
        .if_khz           = 5380,
};


/*****************************************************************************
 * S5H1411 Init
 ******************************************************************************/
static struct s5h1411_config demod_cfg = {
        .output_mode   = S5H1411_SERIAL_OUTPUT,
        .gpio          = S5H1411_GPIO_ON,
        .vsb_if        = S5H1411_IF_44000,
        .qam_if        = S5H1411_IF_44000,
        .inversion     = S5H1411_INVERSION_OFF,
        .status_mode   = S5H1411_TUNERLOCKING,
        .mpeg_timing   = S5H1411_MPEGTIMING_NONCONTINOUS_NONINVERTING_CLOCK,
};

static int s5h1411_init(struct device_node *node, s5h1411_priv_t *priv)
{
	struct device_node *adapter_np;
	u16 reg;

	priv->demod.config = &demod_cfg;
	priv->demod.current_modulation = VSB_8;
	priv->demod.inversion = priv->demod.config->inversion;

	adapter_np = of_parse_phandle(node, "i2c-parent", 0);
	if (adapter_np)
	{
		priv->demod.i2c = of_find_i2c_adapter_by_node(adapter_np);
	}

	priv->demod.gpio_fe_power = of_get_named_gpio(node, "gpios",   0);
	priv->demod.gpio_fe_reset = of_get_named_gpio(node, "gpios",   1);

	s5h1411_power(&priv->demod, 1);
	msleep(10);
	s5h1411_hw_reset(&priv->demod);	
	msleep(1);

	/* check if the demod exists */
	reg = s5h1411_readreg(&priv->demod, S5H1411_I2C_TOP_ADDR, 0x05);
	if (reg != 0x0066)
	{
		eprintk("%s(Fail to check the demod - reg = 0x%x)\n", __FUNCTION__, reg);
		return -1;
	}

	s5h1411_set_powerstate(&priv->demod, 1);

	dprintk("%s\n", __FUNCTION__);

	return 0;
}


/*****************************************************************************
 * S5H1411 Probe
 ******************************************************************************/
static int s5h1411_probe(tcc_fe_priv_t *inst)
{
	s5h1411_priv_t *priv = kzalloc(sizeof(s5h1411_priv_t), GFP_KERNEL);
	int retval;

	if (priv == NULL)
	{
		eprintk("%s(kzalloc fail)\n", __FUNCTION__);
		return -1;
	}

	retval = s5h1411_init(inst->of_node, priv);
	if (retval != 0)
	{
		eprintk("%s(Init Error %d)\n", __FUNCTION__, retval);
		kfree(priv);
		return -1;
	}

	inst->fe.demodulator_priv = priv;
	inst->fe.callback = s5h1411_fe_callback;

	priv->tune_mode = TUNE_MODE_DIGITAL;

	dvb_attach(xc5000_attach, &inst->fe, priv->demod.i2c, &tuner_cfg);

	gPriv = priv;

	dprintk("%s\n", __FUNCTION__);

	return 0;
}


/*****************************************************************************
 * S5H1411 Module Init/Exit
 ******************************************************************************/
static struct tcc_dxb_fe_driver s5h1411_driver = {
	.probe = s5h1411_probe,
	.compatible = "samsung,s5h1411",
	.fe_ops = &s5h1411_fe_ops,
};

static int __init s5h1411_module_init(void)
{
	if (tcc_fe_register(&s5h1411_driver) != 0)
		return -EPERM;

	dprintk("%s(date = %s)\n", __FUNCTION__, __DATE__);

	return 0;
}

static void __exit s5h1411_module_exit(void)
{
	tcc_fe_unregister(&s5h1411_driver);

	dprintk("%s\n", __FUNCTION__);
}

module_init(s5h1411_module_init);
module_exit(s5h1411_module_exit);

MODULE_DESCRIPTION("TCC ATSC (S5H1411)");
MODULE_AUTHOR("C2-G1-3T");
MODULE_LICENSE("GPL");
