/**
 * This file is part of Sodium Language project
 *
 * Copyright © 2020 Murad Karakaþ <muradkarakas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License v3.0
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 *	https://choosealicense.com/licenses/gpl-3.0/
 */

#include "pch.h"

#include "cmdContinue.imp.h"


BOOL cmdContinueImp(TokenCommand *Command) {
	
	//	check parameters

	//	go

	int ret = _diRefreshActiveSessionInstance();
	if (ret != 0) {
		return FALSE;
	}

	if (gAttachedLocalSessionInstance) {
		if (gAttachedLocalSessionInstance->sessionDebugInfo) {
			Session_Debug_Info *sessionDebugInfo = _rpm(gAttachedLocalSessionInstance->sessionDebugInfo, sizeof(Session_Debug_Info));
			if (sessionDebugInfo) {
				sessionDebugInfo->sessionDebugStepType = DEBUG_STEP_TYPE_STEP_CONTUNIE;
				_wpm(gAttachedLocalSessionInstance->sessionDebugInfo, sessionDebugInfo, sizeof(Session_Debug_Info));
				ILOG("Continuing.\r\n");
				free(sessionDebugInfo);
			}
			else {
				ELOG("\nSession debug info cannot be obtained.\n");
			}
		}
		else {
			ELOG("\nNo session debug info available.\n");
		}
	}
	else {
		ELOG("\nNo session attached.\n");
	}
	
	return TRUE;
}

