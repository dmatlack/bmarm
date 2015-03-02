#include <stdio.h>

#include "omap35x.h"

#undef putchar
int putchar(int c)
{
	if (c == '\n')
		uart3_write('\r');

	uart3_write(c);
	return 0;
}

int puts(const char *s)
{
	while (*s) putchar(*s++);

	uart3_write('\r');
	uart3_write('\n');

	return 0;
}

#define BANNER \
	"BBBBB   MM   MM    AA    RRRRR   MM   MM  \n" \
	"B    B  M M M M   A  A   R    R  M M M M  \n" \
	"B    B  M  M  M  A    A  R    R  M  M  M  \n" \
	"BBBBB   M  M  M  A    A  RRRRR   M  M  M  \n" \
	"B    B  M     M  AAAAAA  R    R  M     M  \n" \
	"B    B  M     M  A    A  R    R  M     M  \n" \
	"BBBBB   M     M  A    A  R    R  M     M  \n" \

int main(void)
{
	uart3_init();

	printf("Hello World!\n");
	printf("\n\n" BANNER "\n\n");
	printf("by David Matlack\n");

	while (1);
	return 0;
}
