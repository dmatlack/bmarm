#include "beagleboard.h"

.text

/*
 * Referencing
 * https://www.sourceware.org/binutils/docs-2.12/as.info/ARM-Directives.html#ARM%20Directives
 *
 * The .code directive selects the instruction set being generated. 16 for
 * Thumb, 32 for ARM.
 */
.code 32

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
	/*
	 * Write "Hello!" to the UART
	 */
	ldr r0,=UART3_BASE
	mov r1,#'H'
	strb r1,[r0]
	mov r1,#'e'
	strb r1,[r0]
	mov r1,#'l'
	strb r1,[r0]
	mov r1,#'l'
	strb r1,[r0]
	mov r1,#'o'
	strb r1,[r0]
	mov r1,#'!'

	/*
	 * Spin forever...
	 */
1:
	B 1b

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
