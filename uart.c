#include "arm.h"
#include "omap35x.h"
#include "types.h"

static inline u8 reg_read(unsigned int reg)
{
	u8 *addr = (void *) UART3_BASE + reg;

	read_barrier();

	return *addr;
}

static inline void reg_write(unsigned int reg, u8 value)
{
	u8 *addr = (void *) UART3_BASE + reg;

	*addr = value;

	/*
	 * Since register programming on UART is stateful (e.g.
	 * MCR_REG.TCR_TCLR can only be set if EFR_REG.ENHANCED_EN
	 * is set), attach a write barrier to all register writes.
	 * This ensures writes happen in the order they are written
	 * in the source.
	 */
	write_barrier();
}

static inline void reg_set_bit(unsigned int reg, int bit)
{
	u8 v = reg_read(reg);

	reg_write(reg, v | (1 << bit));
}

static inline void reg_clear_bit(unsigned int reg, int bit)
{
	u8 v = reg_read(reg);

	reg_write(reg, v & ~(1 << bit));
}

static inline int reg_check_bit(unsigned int reg, int bit)
{
	u8 v = reg_read(reg);

	return (v >> bit) & 1;
}

static inline void set_uart3_mode(int mode)
{
	u8 mdr1 = reg_read(MDR1_REG);

	mdr1 &= ~MODE_SELECT_MASK;
	mdr1 |= (mode << MODE_SELECT);

	reg_write(MDR1_REG, mdr1);
}

static u8 set_baud(int baud)
{
	u8 lsb, msb;
	int required_mode;

	reg_write(LCR_REG, CONFIG_B);

	switch (baud) {
	case (115200): {
		lsb = 0x1a;
		msb = 0x00;
		required_mode = UART_16X;
		break;
	}
	default:
		return DISABLED;
	}

	reg_write(DLL_REG, lsb);
	reg_write(DLH_REG, msb);

	return required_mode;
}

void uart3_init(void)
{
	u8 mode;

	/*
	 * Wait until TX FIFO hold and shift registers are empty.
	 */
	while (!reg_check_bit(LSR_REG, TX_SR_E));
		; /* busy-wait */

	/*
	 * Clear all the UART registers.
	 */
	reg_set_bit(SYSC_REG, SOFTRESET);
	while (!reg_check_bit(SYSS_REG, RESETDONE))
		; /* busy-wait */

	/*
	 * Disable UART so that we can program the uart protocol (baud, flow
	 * control, etc.)
	 */
	set_uart3_mode(DISABLED);

	reg_write(LCR_REG, OPERATIONAL);
	reg_write(MCR_REG, RTS | DTR);
	reg_write(FCR_REG, FIFO_EN | TX_FIFO_CLEAR | RX_FIFO_CLEAR);
	mode = set_baud(115200);
	reg_write(LCR_REG, LCR_8N1);

	set_uart3_mode(mode);
}

void uart3_write(u8 byte)
{
	while (!reg_check_bit(LSR_REG, TX_FIFO_E));
		; /* busy-wait */

	reg_write(THR_REG, byte);
}
