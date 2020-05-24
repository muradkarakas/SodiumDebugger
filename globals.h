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

DWORD		gDebugeePID;

volatile BOOL		gIsDebugeeInSuspendedMode;


#define		DEBUGGER_EXIT_REASON_SODIUM_SERVER_NOT_ACCESSIBLE_OR_NOT_RUNNING	10

HANDLE		gThreads[2];

HANDLE		gNamedPipeHandle;
LPTSTR		lpszPipename;

SodiumSession	*gRootSessionRemoteAddress;
SodiumSession	*gRootSessionLocalInstance;
SodiumSession	*gAttachedLocalSessionInstance;
char			gPrompt[MAX_PROMPT_LENGTH];

/* Debugger application's standart input and ouput handles. */
HANDLE		gHandleStdin;
HANDLE		gHandleStdout;

int			gBreakpointId;

/* If it is -1, Debugger command "frame X;" cannot find the frame requested in stack list */
int			gActiveFrame;

/*		
	RULE FOR HTSQL: 
	"break" command must be sent to the debugee (HTSQL) after "attach" command is executed.

	However, Some IDEs (etc Code::Blocks), executes "break" command for each break points before "attach" command executed.
	For that reason, instead of executing the break commands immediately, we must store all that commands in a debugger local variable.
	Debugger waits for attach command issued by the IDE, then executes all breaks stored in local variable.
	
	After attachment is made breakpointsLocalPoolCount variable value will be checked. If it is higher than 0, all stored values will be executed.

	break command example:
		break "C:/projects/sodium/Sodium-Site/apps/debug.sqlx:24"

*/
int					gBreakpointsLocalPoolCount;
CMD_BREAK_PARAMS	gBreakpointsLocalPool[100];