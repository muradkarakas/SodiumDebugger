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

#include "cmdSet.imp.h"

BOOL cmdSetImp(TokenCommand *Command) {
	// check parameters

	// go
	if (Command->params->p.set_cmd_params.prompt[0] != '\0') {
		strcpy_s(gPrompt, MAX_PROMPT_LENGTH, Command->params->p.set_cmd_params.prompt);
		ILOG("\n\n....prompt is set....\n\n");
	}
	return TRUE;
}