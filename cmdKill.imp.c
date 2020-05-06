#include "stdafx.h"

#include "cmdKill.imp.h"

BOOL cmdKillImp(TokenCommand *Command) {

	ILOG("\nIt was a pleasure spending time with you\n\n");
	TerminateThread(gThreads[hWorkerProcessThread], 0);
	
	return TRUE;
}
