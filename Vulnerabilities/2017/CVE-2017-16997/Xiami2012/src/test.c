#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "foo.h"

int foo();

int main(void)
{
	printf(">> EUID = %u:\t", geteuid());
	if (foo() == FOO_GOOD)
	{
		puts("Correct libfoo.so loaded.");
		return 0;
	}
	else
	{
		puts("libfoo.so under current working directory loaded!!!");
		return 1;
	}
}
