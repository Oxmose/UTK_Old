/***************************************************************************//**
 * @file paging_alloc.h
 *
 * @see paging_alloc.c
 *
 * @author Alexy Torres Aurora Dugo
 *
 * @date 20/10/2018
 *
 * @version 1.0
 *
 * @brief Kernel memory physical frame and virtual page allocator.
 *
 * @details Kernel memory physical frame and virtual page allocator. This module
 * allows to* allocate and deallocate physical memory frame and virtual pages.
 *
 * @copyright Alexy Torres Aurora Dugo
 ******************************************************************************/

#ifndef __PAGING_ALLOC_H_
#define __PAGING_ALLOC_H_

#include <Lib/stddef.h> /* OS_RETURN_E */
#include <Lib/stdint.h> /* Generic int types */

/*******************************************************************************
 * CONSTANTS
 ******************************************************************************/

/*******************************************************************************
 * STRUCTURES
 ******************************************************************************/

struct mem_area
{
    uint32_t start;

    uint32_t size;

    struct mem_area* next;
    struct mem_area* prev;
};

typedef struct mem_area mem_area_t;

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

OS_RETURN_E paging_alloc_init(void);

void* kernel_paging_alloc_frames(const uint32_t frame_count, OS_RETURN_E* err);

OS_RETURN_E kernel_paging_free_frames(void* frame_addr,
                                     const uint32_t frame_count);

void* kernel_paging_alloc_pages(const uint32_t page_count, OS_RETURN_E* err);

OS_RETURN_E kernel_paging_free_pages(void* page_addr,
                                     const uint32_t page_count);

#endif /* __PAGING_ALLOC_H_ */