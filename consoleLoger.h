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

#pragma once

typedef enum _LogType {
	LOG_TYPE_INFO,
	LOG_TYPE_ERROR,
	LOG_TYPE_PROMPT,
	LOG_TYPE_DONE
} LogType;

// Prompt logger
#define	PLOG(first, ...)	 logger(LOG_TYPE_PROMPT, first, __VA_ARGS__, NULL)

// Done Successfully logger
#define DLOG(first, ...)	 logger(LOG_TYPE_PROMPT, first, __VA_ARGS__, NULL)

// Info message logger
#define	ILOG(first, ...)	 logger(LOG_TYPE_INFO, first, __VA_ARGS__, NULL)
#define	INFOLOG(first, ...)	 { printf(first, __VA_ARGS__); fflush(stdout); }

// Error message logger
#define	ELOG(first, ...)	 logger(LOG_TYPE_ERROR, first, __VA_ARGS__, NULL)
#define	ERRLOG(first, ...)	 { printf(first, __VA_ARGS__); fflush(stdout); }


void logger(LogType logType, char *first, ...);

void screenLogger(LogType logType, char *message);


