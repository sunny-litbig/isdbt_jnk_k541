/*
 * Copyright (C) 2013 Telechips, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __SUBTITLE_MEMORY_H__
#define __SUBTITLE_MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
DEFINITION OF STRUCTURE
****************************************************************************/
typedef struct{
	unsigned int use;
	unsigned int *phy;
	unsigned int *vir;
}SUB_MEM_TYPE;
	
typedef struct{
	unsigned int *vmem_addr;
	unsigned int vmap_addr;
	unsigned int vmap_size;
	unsigned int ccfb_w;
	unsigned int ccfb_h;
	unsigned int total;
	unsigned int cur_index;
	SUB_MEM_TYPE *mem;
}SUB_MEM_MGR_TYPE;

extern int subtitle_memory_create(int width, int height);
extern int subtitle_memory_linux_create(int width, int height);
extern int subtitle_memory_destroy(void);
extern int subtitle_memory_get_handle(void);
extern int subtitle_memory_put_handle(int handle);
extern unsigned int* subtitle_memory_get_vaddr(int handle);
extern unsigned int* subtitle_memory_get_paddr(int handle);
extern unsigned int subtitle_memory_sub_width(void);
extern unsigned int subtitle_memory_sub_height(void);
extern unsigned int subtitle_memory_sub_clear(int handle);
extern void subtitle_memory_get_used_count(const char *file, int line);
extern int subtitle_memory_get_clear_handle(void);
extern int subtitle_memory_memset(int handle, int size);
#ifdef __cplusplus
}
#endif

#endif	/* __SUBTITLE_MEMORY_H__ */

