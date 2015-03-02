#ifndef _BMARM_OMAP35X_H_
#define _BMARM_OMAP35X_H_

/*
 * UART
 *
 * The UART has two 64-byte FIFO queues (one for TX, one for RX). Each
 * FIFO has a programmable "threshold" that affects when interrupts are
 * sent.
 *
 * For the transmit FIFO case, an interrupt is generated when the TX
 * FIFO crosses the threshold level. The interrupt line is active until
 * a sufficient number of elements is transmitted to go below the FIFO
 * threshold.
 *
 * Polled mode can be enabled to disable interrupts. The status of the
 * receiver and transmitted can be checked by polling the line status
 * register, LSR_REG.
 *
 * The UART can also operate via DMA.
 */
#define UART3_BASE 0x49020000

#define SYSC_REG 0x54 /* System Configuration */
#define     SOFTRESET 1

#define SYSS_REG 0x58 /* System Status */
#define     RESETDONE 0

#define LCR_REG 0x0C /* Line Control Regsister */
#define     DIV_EN        7
#define     BREAK_EN      6
#define     PARITY_TYPE2  5
#define     PARITY_TYPE1  4
#define     PARITY_EN     3
#define     NB_STOP       2
#define     CHAR_LENGTH   0
#define         CHAR_LENGTH_MASK ((1 << 2) - 1)

#define EFR_REG 0x08 /* Enhanced Feature Register */
#define     AUTO_CTS_EN     7
#define     AUTO_RTS_EN     6
#define     SPEC_CHAR       5
#define     ENHANCED_EN     4
#define     SW_FLOW_CONTROL 0

#define MCR_REG 0x10 /* Modem Control Register */
#define     TCR_TLR     6
#define     XON_EN      5
#define     LOOPBACK_EN 4
#define     CD_STS_CH   3
#define     RI_STS_CH   2
#define     RTS         1
#define     DTR         0

#define FCR_REG 0x08 /* FIFO Control Register */
#define     RX_FIFO_TRIG  6
#define     TX_FIFO_TRIG  4
#define     DMA_MODE      3
#define     TX_FIFO_CLEAR 2
#define     RX_FIFO_CLEAR 1
#define     FIFO_EN       0

#define TLR_REG 0x1C /* Trigger Level Register */
#define     RX_FIFO_TRIG_DMA 4
#define     TX_FIFO_TRIG_DMA 0

#define SCR_REG 0x40 /* Supplementary Control Register */
#define     RX_TRIG_GRANU1  7
#define     TX_TRIG_GRANU1  6
#define     RX_CTS_WU_EN    4
#define     TX_EMPTY_CTL_IT 3
#define     DMA_MODE_2      1
#define     DMA_MODE_CTL    0

#define MDR1_REG 0x20 /* Mode Definition Register 1 */
#define     FRAME_END_MODE  7
#define     SIP_MODE        6
#define     SCT             5
#define     SET_TXIR        4
#define     IR_SLEEP        3
#define     MODE_SELECT     0
#define         MODE_SELECT_MASK ((1 << 3) - 1)

#define DLL_REG 0x00 /* Divisor Latches Low */
#define     CLOCK_LSB 0

#define DLH_REG 0x04 /* Divisor Latches High */
#define     CLOCK_MSB 0

#define LSR_REG 0x14 /* Line Status Register */
#define     RX_FIFO_STS    7
#define     TX_SR_E        6
#define     TX_FIFO_E      5
#define     RX_BI          4
#define     RX_FE          3
#define     RX_PE          2
#define     RX_OE          1
#define     RX_FIFO_E      0

#define THR_REG 0x00 /* Transmit Holding Register */

#define IER_REG 0x04 /* Interrupt Enable Register */

/* Register Access Modes (written into LCR register) */
#define CONFIG_A       0x80
#define CONFIG_B       0xBF
#define OPERATIONAL    0x00

/* UART Functional Modes */
#define UART_16X       0x0
#define SIR            0x1
#define UART_16X_AUTO  0x2
#define UART_13X       0x3
#define MIR            0x4
#define FIR            0x5
#define CIR            0x6
#define DISABLED       0x7

/* 8 bits per word, no parity bit, 1 stop bit */
#define LCR_8N1 0x3

#ifndef ASSEMBLER
#include <stdint.h>

/*
 * Referencing arch/arm/include/asm/barrier.h from the Linux Kernel.
 */
#define barrier() asm volatile("" ::: "memory")

/*
 * The DMB instruction Ensures that any memory access before the instruction
 * complete before any memory access after the instruction.
 */
#define dmb() asm volatile ("dmb" ::: "memory")

/*
 * The DSB instruction will not complete until all memory accesses
 * before the instruction have completed.
 */
#define dsb(option) asm volatile ("dsb "#option ::: "memory")

#define read_barrier()   dsb()
#define write_barrier()  dsb(st)

void uart3_init(void);
void uart3_write(uint8_t byte);

#endif /* !ASSEMBLER */

#endif /* !_BMARM_OMAP35X_H_ */
