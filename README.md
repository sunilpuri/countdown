Countdown
=========

This project is a solution to the following programming puzzle:

	Write code that takes a number and counts down to 1, printing each number along the way. You cannot use for
	loops, while loops, do-while loops, gotos or recursion.

### Key Insights
This solution is based on the following insights:

1. You need to rewrite the return address on the call stack in order to loop a function.

2. You cannot use that function's own address as the return address because the return address is eaten from the stack when the return is executed. As a result, the stack pointer would be pointing to a different location for the function's second iteration, leaving it looking in the wrong locations for its state. The solution is to return to an in-between instruction that will re-call the function (without passing any parameters). That will effectively realign the stack pointer for the function. In the code we refer to this in-between instruction as the 'trampoline'.

3. When a function returns to its caller all of its parameters and local variables still exist on the call stack at their same locations. We exploit this to continue our loop from the previous iteration.

4. In order to know when it is the first iteration of the loop we look at the return address on the stack. If that return address is not the location after the 'trampoline' instruction then we know we are being called by the user and should therefore save that return address so that we can return to it when we are done.

### Assumptions
This solution makes the following assumptions:

1. The stack grows downwards. We could accommodate machines whose stack grows upwards by detecting this case in our initialization, calculating the appropriate offset of the return address on the stack, and saving that in a global variable.

2. The caller (not the callee) preserves context between function calls. Because we return to the trampoline from a function that it didn't actually call, its context gets corrupted because the callee is restoring the context for a different caller.

3. All parameters are passed on the stack, not in registers. (This is related to the point above).

### Platforms

#### 64-bit Windows
The solution does not work on 64-bit Windows because [its calling convention is different](http://en.wikipedia.org/wiki/X86_calling_conventions#x86-64_calling_conventions). Specifically that convention does not comply with assumptions #2 and #3 and there is no way to configure the compiler otherwise. 

#### Mac OS X + Linux
The solution does not currently work for Mac OS X or Linux in either 32-bit or 64-bit mode. GCC appears to compile the code to access local variables at fixed locations and does not comply with assumption #2. I need to spend more time investigating the appropriate compiler settings and creating a makefile.
