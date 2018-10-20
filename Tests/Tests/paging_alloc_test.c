
#include <Lib/stdio.h>
#include <Lib/stdint.h>
#include <Lib/stddef.h>
#include <IO/kernel_output.h>
#include <Tests/test_bank.h>
#include <Memory/paging_alloc.h>

#if PAGING_ALLOC_TEST == 1
extern void testmode_paging_add_page(uint32_t start, uint32_t size);
extern mem_area_t* testmode_paging_get_area(void);
extern const mem_area_t* paging_get_free_frames(void);
extern const mem_area_t* paging_get_free_pages(void);
void paging_alloc_test(void)
{

    const mem_area_t* frames;
    const mem_area_t* pages;
    const mem_area_t* cursor;

    kernel_printf("[TESTMODE] Paging Alloc Tests\n");

    frames = paging_get_free_frames();
    pages = paging_get_free_pages();

    cursor = pages;
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    cursor = frames;
    while(cursor)
    {
        kernel_printf("[TESTMODE] Frame range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    testmode_paging_add_page(4, 5);
    testmode_paging_add_page(13, 20);

    cursor = testmode_paging_get_area();
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_printf("\n --- \n");

    testmode_paging_add_page(10, 1);

    cursor = testmode_paging_get_area();
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_printf("\n --- \n");

    testmode_paging_add_page(11, 1);

    cursor = testmode_paging_get_area();
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_printf("\n --- \n");

    testmode_paging_add_page(9, 1);

    cursor = testmode_paging_get_area();
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_printf("\n --- \n");

    testmode_paging_add_page(3, 1);

    cursor = testmode_paging_get_area();
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_printf("\n --- \n");

    testmode_paging_add_page(12, 1);

    cursor = testmode_paging_get_area();
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_printf("\n --- \n");

    testmode_paging_add_page(1, 1);

    cursor = testmode_paging_get_area();
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_printf("\n --- \n");

    testmode_paging_add_page(0, 1);

    cursor = testmode_paging_get_area();
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }


    kernel_printf("\n --- \n");

    testmode_paging_add_page(101, 1);

    cursor = testmode_paging_get_area();
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_printf("\n[TESTMODE]Now testing frame allocation \n");
    uint32_t* frame;
    kernel_printf("[TESTMODE]Silent alloc\n");
    for(uint32_t i = 0; i < 12226; ++i)
    {
        frame = kernel_paging_alloc_frame(NULL);
    }
    for(uint32_t i = 0; i < 30; ++i)
    {
        frame = kernel_paging_alloc_frame(NULL);
        kernel_printf("[TESTMODE]Allocated 0x%08x\n", (uint32_t)frame);
    }

    kernel_paging_free_frame((void*)0x03FDD000);
    kernel_paging_free_frame((void*)0x03FDA000);

    frames = paging_get_free_frames();

    cursor = frames;
    while(cursor)
    {
        kernel_printf("[TESTMODE] Frame range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_paging_free_frame((void*)0x03FDB000);
    kernel_paging_free_frame((void*)0x03FDC000);

    frames = paging_get_free_frames();

    kernel_printf(" --- \n");

    cursor = frames;
    while(cursor)
    {
        kernel_printf("[TESTMODE] Frame range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    frame = kernel_paging_alloc_frame(NULL);
    kernel_printf("[TESTMODE]Allocated 0x%08x\n", (uint32_t)frame);

    kernel_paging_free_frame((void*)0x03FD1000);

    frame = kernel_paging_alloc_frame(NULL);
    kernel_printf("[TESTMODE]Allocated 0x%08x\n", (uint32_t)frame);



    kernel_printf("\n[TESTMODE]Now testing page allocation \n");
    uint32_t* page;
    kernel_printf("[TESTMODE]Silent alloc\n");
    for(uint32_t i = 0; i < 0xE0000 - 10; ++i)
    {
        page = kernel_paging_alloc_page(NULL);
    }
    for(uint32_t i = 0; i < 11; ++i)
    {
        page = kernel_paging_alloc_page(NULL);
        kernel_printf("[TESTMODE]Allocated 0x%08x\n", (uint32_t)page);
    }
    kernel_printf("[TESTMODE]Silent alloc\n");
    for(uint32_t i = 0; i < 0x1EFFF - 10; ++i)
    {
        page = kernel_paging_alloc_page(NULL);
    }
    for(uint32_t i = 0; i < 11; ++i)
    {
        page = kernel_paging_alloc_page(NULL);
        kernel_printf("[TESTMODE]Allocated 0x%08x\n", (uint32_t)page);
    }


    kernel_paging_free_page((void*)0x03FDD000);
    kernel_paging_free_page((void*)0x03FDA000);

    pages = paging_get_free_pages();

    cursor = pages;
    while(cursor)
    {
        kernel_printf("[TESTMODE] Page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    kernel_paging_free_page((void*)0x03FDB000);
    kernel_paging_free_page((void*)0x03FDC000);

    pages = paging_get_free_pages();

    kernel_printf(" --- \n");

    cursor = pages;
    while(cursor)
    {
        kernel_printf("[TESTMODE] page range 0x%08x -> 0x%08x\n",
        cursor->start, cursor->start + cursor->size);
        cursor = cursor->next;
    }

    page = kernel_paging_alloc_page(NULL);
    kernel_printf("[TESTMODE]Allocated 0x%08x\n", (uint32_t)page);

    kernel_paging_free_page((void*)0x03FD1000);

    page = kernel_paging_alloc_page(NULL);
    kernel_printf("[TESTMODE]Allocated 0x%08x\n", (uint32_t)page);


    while(1)
    {
        __asm__ ("hlt");
    }
}
#else
void paging_alloc_test(void)
{
}
#endif