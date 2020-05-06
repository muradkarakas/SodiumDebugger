#include "stdafx.h"

#include "cmdInfo.Args.imp.h"


BOOL 
cmdInfoImp_Args(
	TokenCommand *Command,
	char * outputBuffer
) 
{	
	// show all arguments
	ShowDebugeeLocalVariables(POST_OBJECT_GENERIC_TYPE_FUNCTION_ARGS, outputBuffer);
	
	return TRUE;
}

