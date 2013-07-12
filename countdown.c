#include <stdio.h>
#include <stdlib.h>

// for an overview of the solution see README.md

// these functions deliberately don't take any parameters because they are only supposed to put the
// return address on the stack; additional parameters would actually upend the stack and break the solution
typedef void (*fptr) (void);
fptr	trampoline,			// the address of the trampoline instruction
		post_trampoline,	// the address of the next instruction after the trampoline instruction
		trampoline_target;	// the address we want the trampoline to redirect to

void trampoline_wrapper(void)
{
	// we have 2 calls to trampoline_target() here; only the 2nd one is the real one
	// the 1st one is only executed once (during initialization) and it only exists 
	// to put the address of the trampoline instruction on the stack for set_trampoline()
	trampoline_target();
	trampoline_target();	// this line *is* the trampoline
}

void countdown(int n);

// in set_trampoline() and set_post_trampoline() there is no purpose for the parameter 'n' 
// other than to be an anchor for accessing the return address on the call stack 
// (and no parameter is actually placed on the stack to be 'n' either when they are called)
void set_post_trampoline(int n)
{
	post_trampoline = *(fptr *) (((char *) &n) - sizeof(fptr));
	trampoline_target = (fptr) countdown;
}

void set_trampoline(int n)
{
	trampoline = *(fptr *) (((char *) &n) - sizeof(fptr));
	trampoline_target = (fptr) set_post_trampoline;
}

void init_trampoline(void)
{
	// the sequence of initialization is to call set_trampoline() and then set_post_trampoline()
	// to get the correct addresses we call them the same way the trampline is called (via trampoline_wrapper())
	trampoline_target = (fptr) set_trampoline;
	trampoline_wrapper();
}

void countdown(int n)
{
	fptr original_return_address;
	fptr *preturn_address = (fptr *) (((char *) &n) - sizeof(fptr));

	// if the return address on the call stack is something other than post_trampoline
	// that means we are called by the user so we should save their return address
	if (*preturn_address != post_trampoline)
		original_return_address = *preturn_address;

	printf("%d\n", n);
	if (--n <= 0)
		*preturn_address = original_return_address;	// we are done so put back the user's return address
	else
		*preturn_address = trampoline;				// we aren't done yet so return to the trampoline
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
