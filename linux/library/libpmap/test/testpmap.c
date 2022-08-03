#include <stdio.h>
#include <pmap.h>

static pmap_t pmap_video;
#define DEST_PHY_BASE_ADDR	pmap_video.base
#define DEST_PHY_BASE_SIZE		pmap_video.size

int main(void);
void test_pmap(void);


int main(void)
{
	test_pmap();

	return 0;
}

void test_pmap(void)
{

	pmap_get_info("video", &pmap_video);
	printf("[video] 0x%08X 0x%08X\n", DEST_PHY_BASE_ADDR, DEST_PHY_BASE_SIZE);
}

#if 0
base_addr  size       name
0x88000000 0x00000000 pmem
0x88000000 0x00000000 secured_inbuff
0x88000000 0x00000000 video_sbackup
0x88000000 0x00000000 overlay
0x88000000 0x00000000 overlay1
0x88000000 0x00000000 overlay_rot
0x88000000 0x03e00000 video
0x8be00000 0x00000000 video_thumb
0x8be00000 0x00000000 ump_reserved
0x8be00000 0x00000000 v4l2_video0
0x8be00000 0x00000000 v4l2_video1
0x8be00000 0x00000000 v4l2_video2
0x8be00000 0x00000000 v4l2_video3
0x8be00000 0x00000000 v4l2_video4
0x8be00000 0x00c00000 v4l2_vout
0x8ca00000 0x01000000 v4l2_subtitle
0x8da00000 0x00000000 fb_wmixer
0x8da00000 0x00100000 ram_console
0x8db00000 0x01000000 viqe
0x8eb00000 0x00000000 ext_camera
0x8eb00000 0x00800000 fb_video
0x8f300000 0x00000000 fb_scale
0x8f300000 0x00000000 fb_scale0
0x8f300000 0x00000000 fb_scale1
0x8f300000 0x00000000 fb_g2d0
0x8f300000 0x00000000 fb_g2d1
0x8f300000 0x00000000 video_dual
0x8eb00000 0x00000000 jpeg_header
0x8eb00000 0x00000000 jpeg_raw
0x8eb00000 0x00000000 jpeg_stream
0x8f300000 0x00100000 nand_mem
0x8f400000 0x00000000 jpg_enc_dxb
0x8f400000 0x00000000 jpg_raw_dxb
0x8f400000 0x00000000 video_ext
0x88000000 0x07400000 total
#endif