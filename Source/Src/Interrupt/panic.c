/***************************************************************************//**
 * @file panic.c
 * 
 * @see panic.h
 *
 * @author Alexy Torres Aurora Dugo
 *
 * @date 16/12/2017
 *
 * @version 1.0
 *
 * @brief Panic feature of the kernel.
 * 
 * @details Kernel panic functions. Displays the CPU registers, the faulty 
 * instruction, the interrupt ID and cause.
 * 
 * @copyright Alexy Torres Aurora Dugo
 ******************************************************************************/

#include <Interrupt/interrupts.h> /* cpu_state_t, stack_state_t, PANIC_INT_LINE */
#include <IO/kernel_output.h>     /* kernel_printf */
#include <Lib/stdint.h>           /* Generic int types */
#include <Cpu/cpu.h>              /* hlt cpu_cli */
#include <Drivers/graphic.h>      /* color_scheme_t */

/* RTLK configuration file */
#include <config.h>

/* Header file */
#include <Interrupt/panic.h>

/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/** @brief Stores the current kernel panic error code. */
static uint32_t panic_code = 0;

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

/* TODO REMODE THAT */
int32_t get_pid(void)
{
    return 0;
}

void panic(cpu_state_t* cpu_state, uint32_t int_id, stack_state_t* stack_state)
{
    uint32_t CR0;
    uint32_t CR2;
    uint32_t CR3;
    uint32_t CR4;

    uint32_t error_code;
    uint32_t instruction;
    uint32_t current_cpu_id = 0;
    colorscheme_t panic_scheme;

    panic_scheme.background = BG_BLUE;
    panic_scheme.foreground = FG_WHITE;
    panic_scheme.vga_color  = 1;

    set_color_scheme(panic_scheme);

    /* Test mode probing */
    if(panic_code == 666)
    {
        kernel_printf("[TESTMODE] PANIC\n");
    }

    kernel_printf("#=============================    KERNEL PANIC    =========="
                    "==================#\n");
    kernel_printf("|                                                           "
                    "                  |\n");
    kernel_printf("| Reason: ");
    switch(int_id)
    {
        case 0:
            kernel_printf("Division by zero                        ");
            break;
        case 1:
            kernel_printf("Single-step interrupt                   ");
            break;
        case 2:
            kernel_printf("Non maskable interrupt                  ");
            break;
        case 3:
            kernel_printf("Breakpoint                              ");
            break;
        case 4:
            kernel_printf("Overflow                                ");
            break;
        case 5:
            kernel_printf("Bounds                                  ");
            break;
        case 6:
            kernel_printf("Invalid Opcode                          ");
            break;
        case 7:
            kernel_printf("Coprocessor not available               ");
            break;
        case 8:
            kernel_printf("Double fault                            ");
            break;
        case 9:
            kernel_printf("Coprocessor Segment Overrun             ");
            break;
        case 10:
            kernel_printf("Invalid Task State Segment              ");
            break;
        case 11:
            kernel_printf("Segment not present                     ");
            break;
        case 12:
            kernel_printf("Stack Fault                             ");
            break;
        case 13:
            kernel_printf("General protection fault                ");
            break;
        case 14:
            kernel_printf("Page fault                              ");
            break;
        case 16:
            kernel_printf("Math Fault                              ");
            break;
        case 17:
            kernel_printf("Alignment Check                         ");
            break;
        case 18:
            kernel_printf("Machine Check                           ");
            break;
        case 19:
            kernel_printf("SIMD Floating-Point Exception           ");
            break;
        case 20:
            kernel_printf("Virtualization Exception                ");
            break;
        case 21:
            kernel_printf("Control Protection Exception            ");
            break;
        case PANIC_INT_LINE:
            kernel_printf("Panic generated by the kernel           ");
            break;
        default:
            kernel_printf("Unknown                                 ");
    }

    __asm__ __volatile__("push %eax");
    __asm__ __volatile__("movl %cr0, %eax");
    __asm__ __volatile__("movl %%eax, %0" : "=rm"(CR0));
    __asm__ __volatile__("movl %cr2, %eax");
    __asm__ __volatile__("movl %%eax, %0" : "=rm"(CR2));
    __asm__ __volatile__("movl %cr3, %eax");
    __asm__ __volatile__("movl %%eax, %0" : "=rm"(CR3));
    __asm__ __volatile__("movl %cr4, %eax");
    __asm__ __volatile__("movl %%eax, %0" : "=rm"(CR4));
    __asm__ __volatile__("pop %eax");

    /* If int is generated by the kernel, then the error code is contained in
     * the error code memory address, otherwise we use the interrupt error code.
     */
    if(int_id == PANIC_INT_LINE)
    {
        error_code = panic_code;
    }
    else
    {
        error_code = stack_state->error_code;
    }

    instruction = *((uint32_t*)stack_state->eip);

    kernel_printf("INT ID: 0x%02x                |\n", int_id);
    kernel_printf("| Instruction [EIP]: 0x%08x                   Error code: "
                    "0x%08x      |\n", stack_state->eip, error_code);
    kernel_printf("|                                                           "
                    "                  |\n");
    kernel_printf("|================================= CPU STATE ==============="
                    "==================|\n");
    kernel_printf("|                                                           "
                    "                  |\n");
    kernel_printf("| EAX: 0x%08x  |  EBX: 0x%08x  |  ECX: 0x%08x  |  EDX: "
                    "0x%08x |\n", cpu_state->eax, cpu_state->ebx, 
                    cpu_state->ecx,  cpu_state->edx);
    kernel_printf("| ESI: 0x%08x  |  EDI: 0x%08x  |  EBP: 0x%08x  |  ESP: "
                    "0x%08x |\n", cpu_state->esi, cpu_state->edi, 
                    cpu_state->ebp, cpu_state->esp);
    kernel_printf("| CR0: 0x%08x  |  CR2: 0x%08x  |  CR3: 0x%08x  |  CR4: "
                    "0x%08x |\n", CR0, CR2, CR3, CR4);
    kernel_printf("| EFLAGS: 0x%08x  |                                         "
                    "              |\n", stack_state->eflags);
    kernel_printf("|                                                           "
                    "                  |\n");
    kernel_printf("|============================= SEGMENT REGISTERS ==========="
                    "==================|\n");
    kernel_printf("|                                                           "
                    "                  |\n");
    kernel_printf("| CS: 0x%04x  |  DS: 0x%04x  |  SS: 0x%04x                  "
                    "                  |\n", stack_state->cs & 0xFFFF, 
                    cpu_state->ds & 0xFFFF, cpu_state->ss & 0xFFFF);
    kernel_printf("| ES: 0x%04x  |  FS: 0x%04x  |  GS: 0x%04x                  "
                    "                  |\n", cpu_state->es & 0xFFFF , 
                    cpu_state->fs & 0xFFFF , cpu_state->gs & 0xFFFF);
    kernel_printf("|                                                           "
                    "                  |\n");
    kernel_printf("|============================== ADDITIONAL INFO ============"
                    "==================|\n");
    kernel_printf("|                                                           "
                    "                  |\n");
    kernel_printf("| Core ID: 0x%08x                                           "
                    "              |\n", current_cpu_id);
    kernel_printf("| Thread:  %09u                                             "
                    "             |\n", get_pid());
    kernel_printf("| Inst:    %02x %02x %02x %02x                              "
                    "                          |\n", instruction & 0xFF, 
                    (instruction >> 8) & 0xFF, 
                    (instruction >> 16) & 0xFF, 
                    (instruction >> 24) & 0xFF);
    kernel_printf("|                                                           "
                    "                  |\n");
    kernel_printf("|                                                           "
                    "                  |\n");
    kernel_printf("#==========================================================="
                    "==================#");

    /* We will never return from interrupt */
    while(1)
    {
        cpu_cli();
        cpu_hlt();
    }
}

void kernel_panic(const uint32_t error_code)
{
    /* Save the error code to memory */
    panic_code = error_code;

    /* Raise INT */
    __asm__ __volatile__("int %0" :: "i" (PANIC_INT_LINE));
}

/*
#=============================    KERNEL PANIC    ============================#
|                                                                             |
| Reason: Panic generated by the kernel           INT ID: 0x2a                |
| Instruction [EIP]: 0x00205bb6                   Error code: 0x00000004      |
|                                                                             |
|================================= CPU STATE =================================|
|                                                                             |
| EAX: 0x0020a028  |  EBX: 0x0020a028  |  ECX: 0x00000320  |  EDX: 0x00000004 |
| ESI: 0x0020b004  |  EDI: 0x0020b000  |  EBP: 0x00000000  |  ESP: 0x00210878 |
| CR0: 0x00000000  |  CR2: 0x00000000  |  CR3: 0x00000000  |  CR4: 0x00206958 |
| EFLAGS: 0x00000016  |                                                       |
|                                                                             |
|============================= SEGMENT REGISTERS =============================|
|                                                                             |
| CS: 0x0008  |  DS: 0x0010  |  SS: 0x0010                                    |
| ES: 0x0010  |  FS: 0x0010  |  GS: 0x0010                                    |
|                                                                             |
|============================== ADDITIONAL INFO ==============================|
|                                                                             |
| Core ID: 0x00000000                                                         |
| Thread:  000000000                                                          |
| Inst:    c3 66 90 66                                                        |
|                                                                             |
|                                                                             |
#=============================================================================#
*/