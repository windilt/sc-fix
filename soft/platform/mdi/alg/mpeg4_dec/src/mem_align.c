/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - Aligned Memory Allocator -
 *
 *  Copyright(C) 2002-2003 Edouard Gomez <ed.gomez@free.fr>
 *
 *  This program is free software ; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation ; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY ; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program ; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * $Id: mem_align.c,v 1.16 2004/03/22 22:36:24 edgomez Exp $
 *
 ****************************************************************************/
#ifndef SHEEN_VC_DEBUG 
#include "global.h"
#endif
 #ifdef MP4_3GP_SUPPORT

#include <stdlib.h>
#include <stdio.h>
#include "mem_align.h"

//typedef unsigned int	size_t;

#define MEM_SIZE_1 780*1024//1024 * 1024 //Total malloc:795204 byte
//char mem_malloc[MEM_SIZE_1] = {0};
//char *ptr_curr = mem_malloc;
int total_size = 0;
/*****************************************************************************
 * xvid_malloc
 *
 * This function allocates 'size' bytes (usable by the user) on the heap and
 * takes care of the requested 'alignment'.
 * In order to align the allocated memory block, the xvid_malloc allocates
 * 'size' bytes + 'alignment' bytes. So try to keep alignment very small
 * when allocating small pieces of memory.
 *
 * NB : a block allocated by xvid_malloc _must_ be freed with xvid_free
 *      (the libc free will return an error)
 *
 * Returned value : - NULL on error
 *                  - Pointer to the allocated aligned block
 *
 ****************************************************************************/

void *
xvid_malloc(unsigned int size,
			uint8_t alignment)
{
	uint8_t *mem_ptr;
#if 1
	if ((mem_ptr = (uint8_t *) mmc_MemMalloc(size )) != NULL)
	{
		return ((void *)mem_ptr);
	}
#else
	if (!alignment) {

		/* We have not to satisfy any alignment */
		//if ((mem_ptr = (uint8_t *) my_malloc(size + 1)) != NULL) {
		if(size + 1>mmc_MemGetFree()-32)return NULL;
		if ((mem_ptr = (uint8_t *) mmc_MemMalloc(size + 1)) != NULL) {
			/* Store (mem_ptr - "real allocated memory") in *(mem_ptr-1) */
			*mem_ptr = (uint8_t)1;

			/* Return the mem_ptr pointer */
			return ((void *)(mem_ptr+1));
		}
	} else {
		uint8_t *tmp;

		/* Allocate the required size memory + alignment so we
		 * can realign the data if necessary */
		//if ((tmp = (uint8_t *) my_malloc(size + alignment)) != NULL) {
		if(size + alignment>mmc_MemGetFree()-32)return NULL;
		if ((tmp = (uint8_t *) mmc_MemMalloc(size + alignment)) != NULL) {
			/* Align the tmp pointer */
			mem_ptr =
				(uint8_t *) ((ptr_t) (tmp + alignment - 1) &
							 (~(ptr_t) (alignment - 1)));

			/* Special case where malloc have already satisfied the alignment
			 * We must add alignment to mem_ptr because we must store
			 * (mem_ptr - tmp) in *(mem_ptr-1)
			 * If we do not add alignment to mem_ptr then *(mem_ptr-1) points
			 * to a forbidden memory space */
			if (mem_ptr == tmp)
				mem_ptr += alignment;

			/* (mem_ptr - tmp) is stored in *(mem_ptr-1) so we are able to retrieve
			 * the real malloc block allocated and free it in xvid_free */
			*(mem_ptr - 1) = (uint8_t) (mem_ptr - tmp);

			/* Return the aligned pointer */
			return ((void *)mem_ptr);
		}
	}
#endif

	return(NULL);
}

/*****************************************************************************
 * xvid_free
 *
 * Free a previously 'xvid_malloc' allocated block. Does not free NULL
 * references.
 *
 * Returned value : None.
 *
 ****************************************************************************/

void
xvid_free(void *mem_ptr)
{

	uint8_t *ptr;

	if (mem_ptr == NULL)
		return;

	/* Aligned pointer */
	ptr = mem_ptr;

	/* *(ptr - 1) holds the offset to the real allocated block
	 * we sub that offset os we free the real pointer */
	ptr -= *(ptr - 1);

	/* Free the memory */
	my_free(ptr);
}
void * my_malloc(unsigned int size)
{
	/*
	char *ptr_rtn = ptr_curr;
	ptr_curr += size;
	total_size += size;
	if (total_size > MEM_SIZE_1)
		return 0;
		
	return ptr_rtn;
	*/
	return 0;
}
void my_free(void *mem_ptr)
{	
	return;
}

#ifdef _JADE_

void fwrite1(void)
{
	return;
}
void puts1(void)
{
	return;
}
int clock1(void)
{
	//return sysGetTickCount();		// J.C.
}
#endif
#endif

