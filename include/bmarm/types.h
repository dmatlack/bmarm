#ifndef _BMARM_TYPES_H_
#define _BMARM_TYPES_H_

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

#define bool _Bool
#define true 1
#define false 0

/* from the linux kernel */
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

/* Compile-time assertions for types.h. */
static inline void ____check_types____(void)
{
	BUILD_BUG_ON(sizeof(u8) != 1);
	BUILD_BUG_ON(sizeof(u16) != 2);
	BUILD_BUG_ON(sizeof(u32) != 4);
}

/* for variadic functions */
typedef __builtin_va_list va_list;
#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)
#define va_copy(d,s)	__builtin_va_copy(d,s)

#endif /* !_BMARM_TYPES_H_ */
