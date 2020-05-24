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

#include "cmdDelete.imp.h"

BOOL cmdDeleteImp(TokenCommand *Command) {

	// check paramters
	if (Command->params->p.delete_cmd_params.paramType == CMD_DELETE_PARAM_TYPE_NOTSET) {
		ILOG("\nInvalid usage.\r\n");
		Command->errorCode = COMMAND_ERROR_CODE_INVALID_PARAMETER;
		return FALSE;
	}

	// go

	if (Command->params->p.delete_cmd_params.paramType == CMD_DELETE_PARAM_TYPE_BREAKPOINTS) {
		deleteBreakpoint(Command);
	}
	 
	return TRUE;
}



void deleteBreakpoint(TokenCommand *Command) {
	void *adrInstance = NULL;

	if (gAttachedLocalSessionInstance == NULL) {
		ELOG("\n\nNo session attached\n\n");
		return;
	}

	// reread contents of the session instance
	int ret = _diRefreshActiveSessionInstance();
	if (ret != 0) {
		return;
	}

	for (int i = 0; i < MAX_BREAKPOINT_COUNT; i++) {
		if (gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.id == Command->params->p.delete_cmd_params.breakpointID) {
			gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.breakpointState = BREAKPOINT_STATE_DELETED;
			_wpm(gAttachedLocalSessionInstance->breakpointsRootAddress, gAttachedLocalSessionInstance->breakpointsRoot, sizeof(gAttachedLocalSessionInstance->breakpointsRoot));
		}
	}

}