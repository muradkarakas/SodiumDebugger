#include "stdafx.h"

#include "cmdInfo.Globals.imp.h"


BOOL
cmdInfoImp_Globals(
	TokenCommand* Command,
	char* outputBuffer
	)
{
	// show all variables
	ShowDebugeeGlobalVariables(POST_OBJECT_GENERIC_TYPE_VARIABLE, outputBuffer);

	return TRUE;
}