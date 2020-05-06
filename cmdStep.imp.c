#include "stdafx.h"

#include "cmdStep.imp.h"

BOOL cmdStepImp(TokenCommand *Command) {

	// check parameters

	// go

	// reread contents of the session instance
	int ret = _diRefreshActiveSessionInstance();
	if (ret != 0) {
		return FALSE;
	}

	if (gAttachedLocalSessionInstance && gAttachedLocalSessionInstance->sessionDebugInfo) {
		Session_Debug_Info *sessionDebugInfo = _rpm(gAttachedLocalSessionInstance->sessionDebugInfo, sizeof(Session_Debug_Info));
		if (sessionDebugInfo) {
			sessionDebugInfo->sessionDebugStepType = DEBUG_STEP_TYPE_STEP_IN;
			_wpm(gAttachedLocalSessionInstance->sessionDebugInfo, sessionDebugInfo, sizeof(Session_Debug_Info));
			free(sessionDebugInfo);
		}
	}

	return TRUE;
}