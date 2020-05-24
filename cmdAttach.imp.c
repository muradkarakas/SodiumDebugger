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

#include "cmdAttach.imp.h"
#include "cmdBreak.imp.h"

BOOL cmdAttachImp(TokenCommand *Command) {
	 
	//	Check parameters for error conditions
	if (Command->params->p.attach_cmd_params.sessionId[0] == '\0') {
		ERRLOG("\nNo session id supplied. Usage:\nattach session\n\n");
		Command->errorCode = COMMAND_ERROR_CODE_INVALID_PARAMETER;
		return FALSE;
	}

	// Parameters are correct. lets execute the show command
	int ret = _diSetActiveSessionInstance(Command->params->p.attach_cmd_params.sessionId);
	if (ret == COMRET_SESSION_NOT_FOUND) {		
		ERRLOG("No session found: %s\n\n", Command->params->p.attach_cmd_params.sessionId);
		return FALSE;
	}
	
	INFOLOG("[New Thread %d]\r\n", gDebugeePID);

	sendPooledBreakpointCommandsToDebugee();

	return (ret == COMRET_SUCCESS);
}