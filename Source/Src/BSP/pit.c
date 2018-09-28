/***************************************************************************//**
 * @file pit.c
 * 
 * @see pit.h
 *
 * @author Alexy Torres Aurora Dugo
 *
 * @date 17/12/2017
 *
 * @version 1.0
 *
 * @brief PIT (Programmable interval timer) driver.
 * 
 * @details PIT (Programmable interval timer) driver. Used as the basic timer
 * source in the kernel. This driver provides basic access to the PIT.
 * 
 * @copyright Alexy Torres Aurora Dugo
 ******************************************************************************/

#include <Cpu/cpu.h>              /* cpu_outb */
#include <IO/kernel_output.h>     /* kernel_printf */
#include <Interrupt/interrupts.h> /* register_interrupt, cpu_state,
                                   * stack_state, set_IRQ_mask, 
                                   * kernel_interrupt_set_irq_eoi */
#include <Lib/stdint.h>           /* Generioc int types */
#include <Lib/stddef.h>           /* OS_RETURN_E */

/* RTLK configuration file */
#include <config.h>

/* Header include */
#include <BSP/pit.h>

/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/** @brief Keeps track on the PIT enabled state. */
static uint32_t disabled_nesting;

/** @brief Keeps track of the PIT tick frequency. */
static uint32_t tick_freq;

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

/**
 * @brief Initial PIT interrupt handler.
 * 
 * @details PIT interrupt handler set at the initialization of the PIT. 
 * Dummy routine setting EOI.
 *
 * @param[in] cpu_state The cpu registers before the interrupt.
 * @param[in] int_id The interrupt line that called the handler.
 * @param[in] stack_state The stack state before the interrupt.
 */
static void dummy_handler(cpu_state_t* cpu_state, uint32_t int_id,
                          stack_state_t* stack_state)
{
    (void)cpu_state;
    (void)int_id;
    (void)stack_state;

    /* EOI */
    kernel_interrupt_set_irq_eoi(PIT_IRQ_LINE);
}

OS_RETURN_E pit_init(void)
{
    OS_RETURN_E err;

    /* Init system times */
    disabled_nesting = 1;

    /* Set PIT frequency */
    err = pit_set_freq(PIT_INIT_FREQ);
    if(err != OS_NO_ERR)
    {
        return err;
    }

    /* Set PIT interrupt handler */
    err = kernel_interrupt_register_handler(PIT_INTERRUPT_LINE, dummy_handler);
    if(err != OS_NO_ERR)
    {
        return err;
    }

    #ifdef PIT_KERNEL_DEBUG
    kernel_serial_debug("PIT Initialization\n");
    #endif

    /* Enable PIT IRQ */
    return pit_enable();
}

OS_RETURN_E pit_enable(void)
{
    if(disabled_nesting > 0)
    {
        --disabled_nesting;
    }
    if(disabled_nesting == 0)
    {
        #ifdef PIT_KERNEL_DEBUG
        kernel_serial_debug("Enable PIT\n");
        #endif
        return kernel_interrupt_set_irq_mask(PIT_IRQ_LINE, 1);
    }

    return OS_NO_ERR;
}

OS_RETURN_E pit_disable(void)
{
    OS_RETURN_E err;

    if(disabled_nesting < UINT32_MAX)
    {
        ++disabled_nesting;
    }

    #ifdef PIT_KERNEL_DEBUG
    kernel_serial_debug("Disable PIT (%d)\n", disabled_nesting);
    #endif
    err = kernel_interrupt_set_irq_mask(PIT_IRQ_LINE, 0);

    return err;
}

OS_RETURN_E pit_set_freq(const uint32_t freq)
{
    OS_RETURN_E err;

    if(freq < PIT_MIN_FREQ || freq > PIT_MAX_FREQ)
    {
        return OS_ERR_OUT_OF_BOUND;
    }

    /* Disable PIT IRQ */
    err = pit_disable();
    if(err != OS_NO_ERR)
    {
        return err;
    }

    tick_freq  = freq;

    /* Set clock frequency */
    uint16_t tick_freq = (uint16_t)((uint32_t)PIT_QUARTZ_FREQ / freq);
    cpu_outb(PIT_COMM_SET_FREQ, PIT_COMM_PORT);
    cpu_outb(tick_freq & 0x00FF, PIT_DATA_PORT);
    cpu_outb(tick_freq >> 8, PIT_DATA_PORT);


    #ifdef PIT_KERNEL_DEBUG
    kernel_serial_debug("New PIT frequency set (%d)\n", freq);
    #endif

    /* Enable PIT IRQ */
    return pit_enable();
}

OS_RETURN_E set_pit_handler(void(*handler)(
                                 cpu_state_t*,
                                 uint32_t,
                                 stack_state_t*
                                 ))
{
    OS_RETURN_E err;

    if(handler == NULL)
    {
        return OS_ERR_NULL_POINTER;
    }

    err = pit_disable();
    if(err != OS_NO_ERR)
    {
        return err;
    }

    /* Remove the current handler */
    err = kernel_interrupt_remove_handler(PIT_INTERRUPT_LINE);
    if(err != OS_NO_ERR)
    {
        pit_enable();
        return err;
    }

    err = kernel_interrupt_register_handler(PIT_INTERRUPT_LINE, handler);
    if(err != OS_NO_ERR)
    {
        pit_enable();
        return err;
    }

    #ifdef PIT_KERNEL_DEBUG
    kernel_serial_debug("New PIT handler set (0x%08x)\n", handler);
    #endif

    return pit_enable();
}

OS_RETURN_E remove_pit_handler(void)
{
    #ifdef PIT_KERNEL_DEBUG
    kernel_serial_debug("Default PIT handler set\n");
    #endif
    return set_pit_handler(dummy_handler);
}