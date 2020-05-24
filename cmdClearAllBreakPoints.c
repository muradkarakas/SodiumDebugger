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

#include "cmdClearAllBreakPoints.h"

BOOL
cmdClearAllBreakPoints(
	TokenCommand* Command
) 
{
	int breakPointsClearedCount = 0;

	// last check and go
	if (gAttachedLocalSessionInstance == NULL) {
		// nothing to delete
	}
	else {
		breakPointsClearedCount = ClearAllBreakPoints(Command);
		gBreakpointsLocalPoolCount = 0;
	}

	printf("%d breakpoints cleared\r\n", breakPointsClearedCount);
	fflush(stdout);

	return TRUE;
}

int
ClearAllBreakPoints(
	TokenCommand* Command
)
{
	int breakPointsClearedCount = 0;
	if (gAttachedLocalSessionInstance == NULL) {
		ELOG("\n\nNo session attached\n\n");
		return 0;
	}

	// reread contents of the session instance
	int ret = _diRefreshActiveSessionInstance();
	if (ret != 0) {
		
		return 0;
	}

	for (int i = 0; i < MAX_BREAKPOINT_COUNT; i++) {
		if (gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.breakpointState != BREAKPOINT_STATE_DELETED) {
			wchar_t wcFilePath[MAX_PATH];
			mbstowcs_s(NULL, wcFilePath, MAX_PATH, Command->params->p.clearallbreakpoint_cmd_params.fullFilePath, MAX_PATH);
			if (wcscmp(gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.fileURI, wcFilePath) == 0) {
				wcsncpy_s(gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.fileURI, MAX_PATH, L"\0", MAX_PATH);
				strncpy_s(gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.lineNumber, BREAKPOINT_LINENUMBER_LENGTH, "\0", BREAKPOINT_LINENUMBER_LENGTH);
				gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.id = 0;
				gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.breakpointState = BREAKPOINT_STATE_DELETED;
				gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.breakpointType = BREAKPOINT_TYPE_TEMPORARY;
				breakPointsClearedCount++;
			}
		}
		if (gAttachedLocalSessionInstance->breakpointsRootAddress) {
			_wpm(gAttachedLocalSessionInstance->breakpointsRootAddress, gAttachedLocalSessionInstance->breakpointsRoot, sizeof(gAttachedLocalSessionInstance->breakpointsRoot));
		}
	}
	return breakPointsClearedCount;
}