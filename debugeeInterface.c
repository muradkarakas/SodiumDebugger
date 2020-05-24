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

#include "debugeeInterface.h"

char *_getSodiumServerProcessId() {
	char* reply = NULL;

	int success = openDebugeeNamedPipeConnection();
	if (success == 0) {
		reply = sendCommandToDebugee(PIPE_CMD_GET_PROCESS_ID);
		closeDebugeeNamedPipeConnection();
	}
	return reply;
}

void	_diClearAllBreakpoints() {
	if (gIsDebugeeInSuspendedMode == TRUE) {
		//	We cannot use named pipe since debugee is in suspended state and cannot reply. 
	}
	else {
		int success = openDebugeeNamedPipeConnection();
		if (success == 0) {
			char *reply = sendCommandToDebugee(PIPE_CMD_CLEAR_ALL_BREAKPOINTS);
			if (reply) {	
				free(reply);
			}
			closeDebugeeNamedPipeConnection();
		}
	}
}

char *_diAddBreakpoint(const char *bpStr) {
	char *reply = NULL;

	if (gRootSessionLocalInstance == NULL || bpStr == NULL) {
		return reply;
	}
	int success = openDebugeeNamedPipeConnection();
	if (success == 0) {
		reply = sendCommandToDebugee(bpStr);
		closeDebugeeNamedPipeConnection();
	}
	return reply;
}

int	_diGetSessionCount() {
	SodiumSession *session, *next = NULL;
	int        sessionCount = 0;
	if (gRootSessionLocalInstance == NULL) {
		// if gRootSessionLocalInstance is null, we assume that there is no active session.
		return 0;
	}
	session = gRootSessionLocalInstance;
	while (session) {
		sessionCount++;
		
		// check session structre is valid
		CHECK_SESSION(session);

		if (session->next) {
			next = (SodiumSession *) _rpm(session->next, sizeof(SodiumSession));
		}
		else {
			next = NULL;
		}
		if (gRootSessionLocalInstance != session) {
			free(session);
		}
		session = next;
	}
	return sessionCount;
}

void _wpm(void *dest, void *source, int sizeToWrite) {
	SIZE_T bytesWriten = 0;

	/* get process handle from process id */
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, gDebugeePID);
	BOOL ret = WriteProcessMemory(
				processHandle,
				dest,
				source,
				sizeToWrite,
				&bytesWriten);
	if (ret == FALSE || bytesWriten != sizeToWrite) {
		char errorText[256];	\
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, 
			GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			errorText, 
			sizeof(errorText), NULL);
		ELOG("Error occured: ", errorText);
		exit(1);
	}
	CloseHandle(processHandle);
}

void * _rpm(const void *addr, int sizeToRead) {

	SIZE_T bytesRead; 
	
	if (addr == NULL) {
		return NULL;
	}
 	void* pBuffer = (void*) malloc(sizeToRead);
	
	/* get process handle from process id */
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, gDebugeePID);

	ReadProcessMemory(
		processHandle,
		addr,
		pBuffer,
		sizeToRead,
		&bytesRead);

	if (bytesRead == 0) {
		char errorText[256];	
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorText, sizeof(errorText), NULL);
	}

	CloseHandle(processHandle);

	return pBuffer;
}

int _diRefreshActiveSessionInstance() {
	int ret = 0; 
	char *sessionId = NULL;

	if (gAttachedLocalSessionInstance == NULL || gAttachedLocalSessionInstance->id == NULL) {
		ELOG("\n\nNo session attached\n\n");
		ret = -1;
		goto retpoint;
	}
	sessionId = _rpm(gAttachedLocalSessionInstance->id, SESSION_ID_LENGTH + 1);
	if (sessionId == NULL) {
		ELOG("\n\nNo session attached\n\n");
		ret = -2;
		goto retpoint;
	}

	ret = _diSetActiveSessionInstance(sessionId);

	if (ret == COMRET_SESSION_NOT_FOUND || gAttachedLocalSessionInstance == NULL) {
		ELOG("\n\nNo session found\n\n");
		ret = -3;
		goto retpoint;
	}
retpoint:
	if (sessionId) {
		free(sessionId);
	}

	return ret;
}

int _diSetActiveSessionInstance(const char *pSessionId) {
	SodiumSession *session, *next = NULL;
	char	*lSessionId = NULL;

	if (gAttachedLocalSessionInstance) {
		free(gAttachedLocalSessionInstance);
	}
	if (gAttachedLocalSessionInstance == gRootSessionLocalInstance) {
		gRootSessionLocalInstance = NULL;
	}
	gAttachedLocalSessionInstance = NULL;

	// we must re-read the content of the root instance in case of its content maybe modified
	_diSetRootSessionInstance();
	if (gRootSessionLocalInstance == NULL) {
		// if gRootSessionLocalInstance is null, we assume that there is no active session.
		return COMRET_SESSION_NOT_FOUND;
	}	

	session = gRootSessionLocalInstance;
	while (session) {

		// check session structre is valid
		CHECK_SESSION(session);	

		lSessionId = (char *)_rpm(session->id, SESSION_ID_LENGTH+1);

		if (lSessionId && strncmp(lSessionId, pSessionId, SESSION_ID_LENGTH) == 0) {
			gAttachedLocalSessionInstance = session;
			break;
		}
		if (lSessionId) {
			free(lSessionId);
			lSessionId = NULL;
		}

		if (session->next) {
			next = (SodiumSession *)_rpm(session->next, sizeof(SodiumSession));
		}
		else {
			next = NULL;
		}
		if (gRootSessionLocalInstance != session) {
			free(session);
		}
		session = next;
	}

	if (lSessionId) {
		free(lSessionId);
		lSessionId = NULL;
	}

	if (gAttachedLocalSessionInstance == NULL) {
		// session not found
		return COMRET_SESSION_NOT_FOUND;
	}
	
	return COMRET_SUCCESS;
}

BOOL _isWorkerProcessThreadinSuspendedState() {
	// be sure IIS worker process is running
	DWORD threadStatus = WaitForSingleObject(hWorkerProcessThread, 0);
	if (threadStatus == WAIT_ABANDONED) {
		ILOG("Abondoned");
		return TRUE;
	}
	return FALSE;
}

void _diSetRootSessionInstance(void) {
		
	if (gIsDebugeeInSuspendedMode == TRUE) {
		//	We cannot use named pipe since debugee is in suspended state and cannot reply. 
		//	We'll use same address (gRootSessionRemoteAddress) which was set previously
	}
	else {
		int success = openDebugeeNamedPipeConnection();
		if (success == 0) {
			char *reply = sendCommandToDebugee(PIPE_CMD_GET_SESSION_ROOT_INSTANCE_ADDRESS);
			if (reply) {
				SodiumSession *tmpSessionAddress = (SodiumSession *)strtoull(reply, NULL, 16);
				if (tmpSessionAddress) {
					gRootSessionRemoteAddress = tmpSessionAddress;
				}
				else {
					// If server returns 0, it means there is no active session 
				}
				free(reply);
			}
			closeDebugeeNamedPipeConnection();
		}
	}

	if (gRootSessionLocalInstance) {
		free(gRootSessionLocalInstance);
		gRootSessionLocalInstance = NULL;
	}
	if (gRootSessionRemoteAddress) {
		gRootSessionLocalInstance = (SodiumSession *)_rpm((const void *)gRootSessionRemoteAddress, sizeof(SodiumSession));
	}
}