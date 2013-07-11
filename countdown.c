#include <stdio.h>
#include <stdlib.h>

typedef void (*fptr) (void);

fptr trampoline_target, pre_trampoline, post_trampoline;

void trampoline_wrapper(void)
{
	trampoline_target();
	trampoline_target();
}

void countdown(int n);

void init_post_trampoline(int n)
{
	post_trampoline = *(fptr *) (&n - 1);
	trampoline_target = (fptr) countdown;
}

void init_pre_trampoline(int n)
{
	pre_trampoline = *(fptr *) (&n - 1);
	trampoline_target = (fptr) init_post_trampoline;
}

void init_trampoline(void)
{
	trampoline_target = (fptr) init_pre_trampoline;
	trampoline_wrapper();
}

void countdown(int n)
{
	fptr original_return_address;
	fptr *preturn_address = (fptr *) (&n - 1);
	if (*preturn_address != post_trampoline)
		original_return_address = *preturn_address;

	printf("%d\n", n);
	if (--n <= 0)
		*preturn_address = original_return_address;
	else
		*preturn_address = pre_trampoline;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <positive integer>+\n", argv[0]);
		return 1;
	}

	init_trampoline();
	printf("Starting...\n"); 
	for (argv++; *argv; argv++)
	{
		int n = atoi(*argv);
		countdown (n);
		printf("*****\n"); 
	}
	printf("Finished.\n"); 
	return 0;
}
