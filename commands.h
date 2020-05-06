#pragma once


/*	This is the entry function for all commands.
	This function makes required communication between debugger and debugee before command execution.
	Returns TRUE if command is executed successfully, otherwise returns FALSE	*/
BOOL cmdImp(TokenCommand *Command);