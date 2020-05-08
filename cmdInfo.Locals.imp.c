#include "pch.h"

#include "cmdInfo.Locals.imp.h"


BOOL 
cmdInfoImp_Locals(
	TokenCommand *Command,
	char *outputBuffer
) 
{
	// show all variables
	ShowDebugeeLocalVariables(POST_OBJECT_GENERIC_TYPE_VARIABLE, outputBuffer);
	
	return TRUE;
}