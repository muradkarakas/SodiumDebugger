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

#include "consoleLoger.h"

WORD GetConsoleColour(void) {
	WORD Attributes;
	CONSOLE_SCREEN_BUFFER_INFO Info;
	GetConsoleScreenBufferInfo(gHandleStdout, &Info);
	Attributes = Info.wAttributes;
	return Attributes;
}

void 
screenLogger(
	LogType logType, 
	char *message
) 
{
	// backup current console theme
	WORD attributes = GetConsoleColour();

	switch (logType) {
		case LOG_TYPE_INFO: 
		case LOG_TYPE_PROMPT: 
		case LOG_TYPE_DONE: {
			printf("%s\r\n", message);
			fflush(stdout);
			break;
		}
		case LOG_TYPE_ERROR: {
			fprintf(stderr, "%s\r\n", message);
			fflush(stdout);
			break;
		}
	}
}

void logger(LogType logType, char *first, ...) {

	va_list vl;
	char *str = first;
	
	va_start(vl, first);

	while (str) {
		if (str[0] != '\0') {
			screenLogger(logType, str);
		}
		str = va_arg(vl, char*);
	};
}