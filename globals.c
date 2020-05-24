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

#include "globals.h"


volatile BOOL	gIsDebugeeInSuspendedMode = FALSE;
HANDLE			gThreads[2] = { 0 };
HANDLE			gNamedPipeHandle = NULL;
LPTSTR			lpszPipename = "\\\\.\\pipe\\nextDebuggerServer";
DWORD			gDebugeePID = 0;
SodiumSession		*gRootSessionLocalInstance = NULL;
SodiumSession	*gRootSessionRemoteAddress = NULL;
SodiumSession	*gAttachedLocalSessionInstance = NULL;

int					gBreakpointsLocalPoolCount = 0;
CMD_BREAK_PARAMS	gBreakpointsLocalPool[100];

int				gBreakpointId = 2;
int				gActiveFrame = -1;

char			gPrompt[MAX_PROMPT_LENGTH] = "Soduim Debugger: ";