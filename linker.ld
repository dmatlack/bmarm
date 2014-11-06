/*
 * Options for BFD (Binary File Descriptor)
 */
OUTPUT_FORMAT("elf32-littlearm", "elf32_bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(_start)

MEMORY
{
	/*
	 * Beagleboard Memory Map (according to U-boot):
	 *
	 * OMAP3 beagleboard.org # bdinfo
	 * [...]
	 * DRAM bank   = 0x00000000
	 * -> start    = 0x80000000
	 * -> size     = 0x10000000 <-- 256 MB
	 * DRAM bank   = 0x00000001
	 * -> start    = 0x90000000
	 * -> size     = 0x00000000 <-- 0 MB
	 * [...]
	 *
	 * We will boot on the Beagleboard with the help of U-boot. We'll
	 * be loaded at:
	 *
	 * OMAP3 beagleboard.org # env print
	 * [...]
	 * loadaddr=0x80200000
	 * [...]
	 *
	 * See http://www.denx.de/wiki/view/DULG/UBootEnvVariables
	 */
	RAM (rwx)  : ORIGIN = 0x80200000, LENGTH = 0xfe00000
}

SYS_STACK_SIZE = 4096;

SECTIONS
{
	/*
	 * Read-only code and data, executed from ROM.
	 */
	.text : {
		__text_load = LOADADDR(.text);
		__text_start = .;

		*(.text .text.*)
		*(.rodata .rodata.*)

		__text_end = .;
	} >RAM


	.data : {
		__data_load = LOADADDR (.data);
		__data_start = .;

		*(.data .data.*)
		. = ALIGN (4);

		__data_end = .;
	} >RAM

	.bss : {
		__bss_load = LOADADDR(.bss);
		__bss_start = . ;

		*(.bss .bss.*)
		. = ALIGN (4);

		__bss_end = .;
	} >RAM

	.stack : {
		__stack_load = LOADADDR(.stack);
		__stack_start = . ;

		. += SYS_STACK_SIZE;
		__sys_stack_top = .;

		__stack_end = .;
	} >RAM

	__end = .;
}
