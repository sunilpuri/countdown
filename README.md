Countdown
=========

This project is a solution to the following programming puzzle:

	Write code that takes a number and counts down to 1, printing each number along the way. You can't use for
	loops, while loops, do-while loops, gotos or recursion.

---
The solution is based on a few key insights:

1) You need to rewrite the return address on the call stack in order to loop a function.

2) You cannot use that function's own address as the return address because the return address is eaten from the stack when the return is executed. As a result, the stack pointer would be pointing to a different location for the function's second iteration, leaving it look in the wrong locations for its state. The solution is to return to an in-between instruction that will re-call the function, effectively realigning the stack pointer for the function.In the code we refer to this in-between instruction as the 'trampoline'.

3) When a function returns all of its parameters and local variables still exist on the call stack at their same locations. We take advantage of this to continue our state from the previous iteration.

4) In order to know when it is the first iteration of the loop we look at the return address on the stack. If that return address is not the location after the 'trampoline' then we know we are being called by the user and should therefore save that return address so that we can return to it at the end.

---
This solution is based on a few assumptions:

1) The stack grows downwards. We could accommodate machines whose stack grows upwards by detecting this case in our initialization, calculating the appropriate offset of the return address on the stack, and saving that in a global variable.

2) The caller (not the callee) preserves state between function calls. Because we land on the 'trampoline' through our unconventional means any state would be lost if the caller had to preserve it.

3) All parameters are passed on the stack, not in registers. (This is related to the point above).

---
Platforms

64-bit Windows
The solution does not work when compiled with Visual Studio for 64-bit because VS uses different conventions than it does for 32-bit. Specifically it does not comply with assumptions #2 and #3 and there is no way to configure the compiler otherwise. MSDN says that VS is complying with x64's Application Binary Interface.

Mac OS X + Linux
The solution does not currently work for Mac OS X or Linux in either 32-bit or 64-bit mode. GCC appears to compile the code to access local variables at fixed locations and does not comply with assumption #2. I need to spend more time investigating the appropriate compiler settings and creating a makefile.
