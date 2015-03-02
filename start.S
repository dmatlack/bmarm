.text

/*
 * Referencing
 * https://www.sourceware.org/binutils/docs-2.12/as.info/ARM-Directives.html#ARM%20Directives
 *
 * The .code directive selects the instruction set being generated. 16 for
 * Thumb, 32 for ARM.
 */
.code 32

.extern main

/*
 * .global makes the symbol visible to ld (allowing other partial programs,
 * which are linked in, to reference it).
 */
.global _start

/*
 * .func emits debugging information and is ignored if the file is not
 * assembled with debugging enabled. The function should be terminated
 * with .endfunc
 */
.func _start

_start:
	ldr r0,=main
	bx r0

	/*
	 * Spin forever...
	 */
1:
	b 1b

/*
 * .size name, expression
 *
 * This is used to set the size associated with a symbol, used for
 * debugging.
 */
.size _start, . - _start

.endfunc /* _start */

/*
 * .end marks the end of an assembly file and tells the assembler not
 * to process anything past the .end directive.
 */
.end
