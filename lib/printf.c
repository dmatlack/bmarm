#include <bmarm/types.h>

/* define these to use printf */
extern int putchar(int c);
extern int puts(const char *s);

#define PRINTF_BUFFER_MAX 128
static char printf_buffer[PRINTF_BUFFER_MAX];
int printf_index = 0;

static void flush(void)
{
	int i;

	for (i = 0; i < printf_index; i++)
		putchar(printf_buffer[i]);

	printf_index = 0;
}

static void print_char(char c)
{
	printf_buffer[printf_index++] = c;

	if (printf_index == PRINTF_BUFFER_MAX || c == '\n')
		flush();
}

enum {
	UPPER_CASE,
	LOWER_CASE,
};
static void print_number(unsigned long long arg, int base, int c)
{
	static const char lower[] = "0123456789abcdef";
	static const char upper[] = "0123456789ABCDEF";
	const char *to_char;
	char scratch[64];
	int i = 0;

	to_char = (c == UPPER_CASE) ? upper : lower;

	do {
		scratch[i++] = to_char[arg % base];
	} while (arg /= base);

	while (--i >= 0)
		print_char(scratch[i]);
}

int vprintf(const char *fmt, va_list ap)
{
	const char *s = fmt;

	while (*s) {
		char c = *s++;

		if (c != '%') {
			print_char(c);
			continue;
		}

		c = *s++;
		switch (c) {
		case 'c': {
			print_char(va_arg(ap, int));
			break;
		}
		case 's': {
			char *arg = va_arg(ap, char *);

			while (*arg)
				print_char(*arg++);

			break;
		}
		case 'd': {
			int arg = va_arg(ap, int);

			if (arg < 0) {
				print_char('-');
				arg = -arg;
			}

			print_number(arg, 10, 0);
			break;
		}
		case 'x': {
			unsigned int arg = va_arg(ap, unsigned int);

			print_number(arg, 16, LOWER_CASE);
			break;
		}
		case 'X': {
			unsigned int arg = va_arg(ap, unsigned int);

			print_number(arg, 16, UPPER_CASE);
			break;
		}
		case 'p': {
			void *arg = va_arg(ap, void *);

			print_char('0');
			print_char('x');
			print_number((unsigned long) arg, 16, LOWER_CASE);
			break;
		}
		default: {
			const char *error = "UNKNOWN_FMT_CHAR";

			while (*error)
				print_char(*error++);
			break;
		}}
	}

	return 0;
}

int printf(const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vprintf(fmt, ap);
	va_end(ap);

	return ret;
}
