#include <stdio.h>
#include <stdlib.h>

typedef void (*fptr) (void);

fptr trampoline, trampoline2;

void trampoline_wrapper(void)
{
	trampoline();
	trampoline();
}

void trampoline_noop(void)
{
}

void trampoline_setter(int n)
{
	trampoline2 = (fptr) ((int *) &n)[-1];
	trampoline = trampoline_noop;
}

void trampoline_init(void)
{
	trampoline = (fptr) trampoline_setter;
	trampoline_wrapper();
	trampoline = trampoline2;
}

void countdown(int n)
{
	static int return_address = 0;
	static fptr my_trampoline;
	if (return_address == 0)
	{
		return_address = ((int *) &n)[-1];
		my_trampoline = trampoline;
		trampoline = (fptr) countdown;
	}
	((int *) &n)[-1] = (int) my_trampoline;

	printf("%d\n", n);
	if (--n <= 0)
	{
		((int *) &n)[-1] = return_address;
		return_address = 0;
		trampoline = my_trampoline;
	}
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <positive integer>+\n", argv[0]);
		return 1;
	}

	trampoline_init();
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
