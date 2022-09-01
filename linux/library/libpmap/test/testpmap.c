#include <stdio.h>
#include <pmap.h>

#define PMAP_NAME "subtitle"
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

	pmap_get_info(PMAP_NAME, &pmap_video);
	printf("[%s] BASE_ADDR : 0x%08X, BASE_SIZE : 0x%08X\n", PMAP_NAME, DEST_PHY_BASE_ADDR, DEST_PHY_BASE_SIZE);
}

