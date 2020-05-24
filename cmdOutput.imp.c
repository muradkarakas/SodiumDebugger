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

#include "cmdOutput.imp.h"
#include "cmdInfoUtils.h"

BOOL cmdOutputImp(TokenCommand *Command) {

	// check parameters
	if (Command->params->p.whatis_cmd_params.variableName[0] == '\0') {
		ELOG("\nVariable name required. Usage:\nwhatis count\n\n");
		Command->errorCode = COMMAND_ERROR_CODE_INVALID_PARAMETER;
		return FALSE;
	}

	// go

	// getting type of variable if it is exists. If it is not exists, function will return 0
	int typeIfExists = getTypeOfPostObjectGeneric(Command->params->p.whatis_cmd_params.variableName);
	if (typeIfExists == -3)
		ILOG("No symbol \"", Command->params->p.whatis_cmd_params.variableName, "\" in current context.");
	else {
		if (typeIfExists >= 0) {
			PostObjectGeneric *postObjectGeneric = getPostObjectGeneric(Command->params->p.whatis_cmd_params.variableName);
			if (postObjectGeneric) {
				char *val = getVariableValue(postObjectGeneric, Command->params->p.whatis_cmd_params.variableName);
				if (val) {
					ILOG(val, "\r\n");
					free(val);
				}
				else {
					ILOG("\r\n");
				}
				free(postObjectGeneric);
			}
		}
		else
			ELOG("\nError occured");
	}

	

	return TRUE;
}