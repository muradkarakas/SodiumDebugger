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

#include "workerProcessThread.h"
#include "debugeeInterface.h"

DWORD WINAPI  
WorkerProcessThreadFunction(
	LPVOID lpParam
) 
{
	if (gDebugeePID != 0) {
		// GDB respobse
		
		_diSetRootSessionInstance();
		if (gRootSessionLocalInstance == NULL) {
			ELOG("\nNot attached\n\n");
		}
		BOOL ret = DebugActiveHTSQLProcess(gDebugeePID);
		if (ret) {
			DEBUG_EVENT DebugEv = { 0 };
			DebugSetProcessKillOnExit(FALSE);
			EnterDebugLoop(&DebugEv);
		}
	}
	return 0;
}

BOOL DebugActiveHTSQLProcess(DWORD dwProcessId) {
	BOOL ret;
	_diClearAllBreakpoints();
	ret = DebugActiveProcess(dwProcessId);
	return ret;
}

void killWorkerProcessThread(void) {
	if (gDebugeePID != 0) {
		DebugActiveProcessStop(gDebugeePID);
		gDebugeePID = 0;
	}
}

DWORD 
GetSodiumServerProcessId() 
{
	DWORD ret = 0;
	char   *psBuffer = _getSodiumServerProcessId();

	if (psBuffer)
		ret = atoi((char*)(psBuffer));

	return ret;
	/*
	char   psBuffer[128];

	FILE   *pPipe = _popen("powershell \"Get-Process SodiumServer | Format-List Id\"", "rt");
	if (pPipe == NULL)
		return 0;

	// read 3nd line
	fgets(psBuffer, 128, pPipe);
	fgets(psBuffer, 128, pPipe);
	fgets(psBuffer, 128, pPipe);

	DWORD ret = atoi((char*)(&psBuffer[5]));

	_pclose(pPipe);

	return ret;
	*/
}

