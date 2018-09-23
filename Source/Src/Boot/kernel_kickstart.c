/***************************************************************************//**
 * @file kernel_kickstart.c
 *
 * @author Alexy Torres Aurora Dugo
 *
 * @date 15/12/2017
 *
 * @version 1.0
 *
 * @brief Kernel's main boot sequence.
 * 
 * @warning At this point interrupts should be disabled.
 * 
 * @details Kernel's booting sequence. Initializes the rest of the kernel after
 *  GDT, IDT and TSS initialization. Initializes the hardware and software
 * core of the kernel.
 * 
 * @copyright Alexy Torres Aurora Dugo
 ******************************************************************************/

#include <Cpu/cpu.h>          /* detect_cpu() */
#include <IO/kernel_output.h> /* kernel_output() */
#include <Drivers/pic.h>      /* init_pic(); */

/* RTLK configuration file */
#include <config.h>

#if TEST_MODE_ENABLED
#include <Tests/test_bank.h>
#endif

/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

/** 
 * @brief Main boot sequence, C kernel entry point.
 * 
 * @details Main boot sequence, C kernel entry point. Initializes each basic 
 * drivers for the kernel, then init the scheduler and start the system.
 * 
 * @warning This function should never return. In case of return, the kernel
 * should be able to catch the return as an error.
 */
void kernel_kickstart(void)
{
    OS_RETURN_E err;

    #if TEST_MODE_ENABLED
    loader_ok_test();
    idt_ok_test();
    gdt_ok_test();
    output_test();
    #endif

    kernel_printf("------------------------------ Kickstarting RTLK -----------"
                  "--------------------\n");

    /* Launch CPU detection routine */
    if(detect_cpu(1) != OS_NO_ERR)
    {
        kernel_error("Could not detect CPU, halting.");
        return;
    }

    #if TEST_MODE_ENABLED
    pic_driver_test();
    #endif

    /* Init PIC */
    err = init_pic();
    if(err == OS_NO_ERR)
    {
        kernel_success("PIC Initialized\n");
    }
    else
    {
        kernel_error("PIC Initialization error [%d]\n", err);
        return;
    }


    
    

    return;
}
