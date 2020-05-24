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

/*	********************************************************************************************************************
	Functions in this file and related files start with '_' charactr which it means it is part of debugeeInterface 
	
	*******************************************************************************************************************/

/* Macro returns:
-1: if session structure is invalid
-2: if session id is invalid
*/
#define CHECK_SESSION(_session)																\
						{																	\
							if (_session->sessionReadyState < SESSION_READY_STATE_INIT ||	\
								_session->sessionReadyState > SESSION_READY_STATE_KILLED ||	\
								_session->id == NULL) {							\
								return -1;													\
							}																\
							char *tmpSessionId = NULL;													\
							tmpSessionId = (char*)_rpm(_session->id, SESSION_ID_LENGTH + 1);	\
							if (tmpSessionId && strlen(tmpSessionId) != SESSION_ID_LENGTH) {			\
								free(tmpSessionId);											\
								return -2;													\
							}																\
							free(tmpSessionId);												\
						}

// Read debuggee process memory
// Calles MUST release return value
void	*_rpm(const void *addr, int sizeToRead);

/*	Writes to debuggee process memory	
	dest:	A pointer to the base address in the specified process to which data is written. Before data transfer occurs, the system verifies that all data in the base address and memory of the specified size is accessible for write access, and if it is not accessible, the function fails.
	source:	A pointer to the buffer that contains data to be written in the address space of the specified process.
*/
void	_wpm(void *dest, void *source, int sizeToWrite);

/*	Note.1: Setting gRootSessionLocalInstance variable.
	Note.2:	First function to be called for HTSQL debugger
	Note.3: CHECK_SESSION macro is not called in that function. Use that macro in your function before use gRootSessionLocalInstance	*/
void	_diSetRootSessionInstance(void);

/*	Note.1: Setting gAttachedLocalSessionInstance variable to a specific session.
	Note.2: _diSetRootSessionInstance function should be called before that function call	
	Note.3:	Returns 0 for success
				   -3 for no session found			*/
int		 _diSetActiveSessionInstance(const char *pSessionId);

/*	Reread content of the gAttachedLocalSessionInstance. 
	gAttachedLocalSessionInstance value must be not null. 
	Calls _diSetActiveSessionInstance and returns its return value
	Return	0 for success,
			-1 if gAttachedLocalSessionInstance is NULL
			-2 if No session attached
			-3 if No session found
*/
int		_diRefreshActiveSessionInstance();

// Function returns -1 if session structure is invalid.  
int		_diGetSessionCount();

//  
char*	_getSodiumServerProcessId();

BOOL	_isWorkerProcessThreadinSuspendedState();


char	*_diAddBreakpoint(const char *bpStr);
void	_diClearAllBreakpoints();


