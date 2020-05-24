/**
 * This file is part of Sodium Language project
 *
 * Copyright © 2020 Murad Karakaş <muradkarakas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License v3.0
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 *	https://choosealicense.com/licenses/gpl-3.0/
 */

#include "pch.h"

#include "main.h"

/*
	returns 10: if SodiumServer.exe is not running or not accessible due to security reason, etc
*/
int 
main(
	int argc, 
	char *argv[]
) 
{
	DWORD   dwConsoleThreadId;

	// setting standart input and output handles 
	SetIORedirectionHandles();

	// find out Sodium server process id
	gDebugeePID = GetSodiumServerProcessId();
	if (gDebugeePID == 0) {
		return DEBUGGER_EXIT_REASON_SODIUM_SERVER_NOT_ACCESSIBLE_OR_NOT_RUNNING;
	}

	/* launching console thread */
	gThreads[hConsoleThread] = CreateThread(NULL, 0, ConsoleThreadFunction, NULL, 0, &dwConsoleThreadId);
	if (gThreads[hConsoleThread]) {
		ResumeThread(gThreads[hConsoleThread]);
	}

	WorkerProcessThreadFunction(NULL);

	return 0;
}


