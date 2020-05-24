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

#include "cmdShow.imp.h"

BOOL cmdShowImp(TokenCommand *Command) {

	BOOL	success = FALSE; 

	//	Check parameters for error conditions

	// reread contents of the session instance
	int ret = _diRefreshActiveSessionInstance();
	if (ret != 0) {
		return FALSE;
	}

	// Parameters are correct. lets execute the show command
	if (Command->params->p.show_cmd_params.sessionCountValue == SHOW_CMD_PARAM_SESSION_COUNT_VALUE_SET) {
		success = cmdShowSessionCount(Command);
	} else if (Command->params->p.show_cmd_params.version == TRUE) {
		// implementation
	}
	return success;
}

BOOL cmdShowSessionCount(TokenCommand *Command) {
	BOOL success = FALSE;
	char sesCount[5];

	int sc = _diGetSessionCount();
	if (sc == -1) {
		ELOG("\n\nCannot obtain a valid session structure\n\n");
	}
	else {
		_itoa_s(sc, sesCount, 5, 10);
		ILOG("\n\nActive session count is ", sesCount, "\n\n");
		success = TRUE;
	}	
	return success;
}