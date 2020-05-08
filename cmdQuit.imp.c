#include "pch.h"

#include "cmdQuit.imp.h"


BOOL cmdQuitImp(TokenCommand *Command) {
	ILOG("\nIt was a pleasure spending time with you\n\n");
	TerminateThread(gThreads[hWorkerProcessThread], 0);
	return TRUE;
}