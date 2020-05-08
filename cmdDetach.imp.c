#include "pch.h"

#include "cmdDetach.imp.h"

BOOL cmdDetachImp(TokenCommand *Command) {
	killWorkerProcessThread();
	return TRUE;
}