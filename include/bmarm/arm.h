#ifndef __BMARM_ARM_H__
#define __BMARM_ARM_H__

/*
 * Referencing arch/arm/include/asm/barrier.h from the Linux Kernel.
 */
#define barrier() asm volatile("" ::: "memory")

/*
 * The DMB instruction ensures that any memory access before the instruction
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

#define PROC_FEATURE_0      "c1, 0"
#define PROC_FEATURE_1      "c1, 1"
#define DEBUG_FEATURE_0     "c1, 2"
#define AUXILIARY_FEATURE_0 "c1, 3"
#define MEMORY_FEATURE_0    "c1, 4"
#define MEMORY_FEATURE_1    "c1, 5"
#define MEMORY_FEATURE_2    "c1, 6"
#define MEMORY_FEATURE_3    "c1, 7"
#define ISA_FEATURE_0       "c2, 0"
#define ISA_FEATURE_1       "c2, 1"
#define ISA_FEATURE_2       "c2, 2"
#define ISA_FEATURE_3       "c2, 3"
#define ISA_FEATURE_4       "c2, 4"
#define ISA_FEATURE_5       "c2, 5"

/*
 * CPUID
 *
 * MRC (Move core register from Coprocessor)
 *
 * MRC{2}{<c>}{<q>} <coproc>, <opc1>, <Rt>, <CRn>, <CRm>{, <opc2>}
 *
 * <coproc> The name of the coprocessor, p0-p15
 * <opc1>   Coprocessor-specific opcode, 0-7
 * <Rt>     The destination ARM core register
 * <CRn>    Coprocessor register that contains the first operand
 * <CRm>    Additional source or destination coprocessor register
 * <opc2>   Coprocessor-specific opcode, 0-7 (if omitted, then 0)
 */
#define cpuid(_crm_and_opc2) ({						\
	u32 __result;							\
	asm volatile("mcr p15, 0, %[result], c0, " _crm_and_opc2	\
		     : [result]"=r"(__result)				\
		     :							\
		     : "memory");					\
	__result;							\
})

#endif /* !__BMARM_ARM_H__ */
