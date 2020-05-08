#include "pch.h"

#include "debugEvents.h"

void EnterDebugLoop(const LPDEBUG_EVENT DebugEv) {
	BOOL result;
	DWORD   dwConsoleThreadId;
	DWORD dwContinueStatus = DBG_CONTINUE; // exception continuation
	gIsDebugeeInSuspendedMode = FALSE;

	for (; gDebugeePID != 0; )
	{
		result = WaitForDebugEvent(DebugEv, INFINITE);
		if (!result) {
			ErrorExit("WaitForDebugEvent");
		}
		gIsDebugeeInSuspendedMode = TRUE;
		
		if (DebugEv->dwDebugEventCode == OUTPUT_DEBUG_STRING_EVENT) {

			TerminateThread(gThreads[hConsoleThread], 0);
			WaitForSingleObject(gThreads[hConsoleThread], INFINITE);
			
			dwContinueStatus = ProcessDebugEvent(DebugEv);
			
			//	If we get OUTPUT_DEBUG_STRING_EVENT event from tbe HTSQL, we will not resume HTSQL and wil not accept more commands 
			//	from the Code::Blocks IDE until the one of the some specific commands (which are CMD_NEXT, CMD_STEP, CMD_FINISH). 
			//	Then function will quit with a return value.
			int reasonToExit = ConsoleThreadFunction(NULL);

			//	Reason why the function "consoleThreadFunction" is exited is important. Return value is used to determine why the function is exited. 
			//	This value is also used to determine what the incoming parameter of the same function will be. 
			//	This time same function will be launched in a thread context.
			void *threadFuncParam = (reasonToExit != CMD_CONTINUE && reasonToExit != CMD_TBREAK) ? (void*) 9 : NULL;

			// launching console thread again but with "(void*) 9" value as a parameter value to the "consoleThreadFunction" function 
			gThreads[hConsoleThread] = CreateThread(NULL, 0, ConsoleThreadFunction, threadFuncParam, 0, &dwConsoleThreadId);
			if (gThreads[hConsoleThread]) {
				ResumeThread(gThreads[hConsoleThread]);
			}
		}
		else {
			dwContinueStatus = ProcessDebugEvent(DebugEv);
		}
		
		gIsDebugeeInSuspendedMode = FALSE;		
		ContinueDebugEvent(DebugEv->dwProcessId,
				DebugEv->dwThreadId,
				dwContinueStatus);

	}
}

DWORD 
OnOutputDebugStringEvent(
	const LPDEBUG_EVENT de
) 
{
	OUTPUT_DEBUG_STRING_INFO DebugString = de->u.DebugString;

	BYTE* pBuffer = (BYTE*)malloc(DebugString.nDebugStringLength);

	SIZE_T bytesRead;
	/* get process handle from process id */
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, de->dwProcessId);

	ReadProcessMemory(
		processHandle,
		DebugString.lpDebugStringData,
		pBuffer,
		DebugString.nDebugStringLength,
		&bytesRead);

	if (bytesRead == 0) {
		ELOG("\nError occured in ReadProcessMemory");
	}
	else {
		ILOG(pBuffer);
	}

	CloseHandle(processHandle);
	free(pBuffer);
	return DBG_CONTINUE;
}

DWORD OnCreateThreadDebugEvent(const LPDEBUG_EVENT de) {
	//printf_s("\nstarted... OnCreateThreadDebugEvent");
	return DBG_CONTINUE;
}

DWORD OnCreateProcessDebugEvent(const LPDEBUG_EVENT de) {
	//printf_s("\nstarted... OnCreateProcessDebugEvent");
	return DBG_CONTINUE;
}

DWORD OnExitThreadDebugEvent(const LPDEBUG_EVENT de) {
	//printf_s("\nstarted... OnExitThreadDebugEvent");
	return DBG_CONTINUE;
}

DWORD OnExitProcessDebugEvent(const LPDEBUG_EVENT de) {
	//printf_s("\nstarted... OnExitProcessDebugEvent");
	return DBG_CONTINUE;
}

DWORD OnLoadDllDebugEvent(const LPDEBUG_EVENT de) {
	//printf_s("\nstarted... OnLoadDllDebugEvent");
	return DBG_CONTINUE;
}

DWORD OnUnloadDllDebugEvent(const LPDEBUG_EVENT de) {
	//printf_s("\nstarted... OnUnloadDllDebugEvent");
	return DBG_CONTINUE;
}

DWORD OnRipEvent(const LPDEBUG_EVENT de) {
	//printf_s("\nstarted... OnRipEvent");
	return DBG_CONTINUE;
}

DWORD ProcessDebugEvent(LPDEBUG_EVENT DebugEv) {

	DWORD dwContinueStatus = DBG_CONTINUE;

	switch (DebugEv->dwDebugEventCode) {
		case OUTPUT_DEBUG_STRING_EVENT: {
			dwContinueStatus = OnOutputDebugStringEvent(DebugEv);
			break;
		}
		case CREATE_THREAD_DEBUG_EVENT: {
			dwContinueStatus = OnCreateThreadDebugEvent(DebugEv);
			break;
		}
		case CREATE_PROCESS_DEBUG_EVENT: {
			dwContinueStatus = OnCreateProcessDebugEvent(DebugEv);
			break;
		}
		case EXIT_THREAD_DEBUG_EVENT: {
			dwContinueStatus = OnExitThreadDebugEvent(DebugEv);
			break;
		}
		case EXIT_PROCESS_DEBUG_EVENT: {
			dwContinueStatus = OnExitProcessDebugEvent(DebugEv);
			break;
		}
		case LOAD_DLL_DEBUG_EVENT: {
			dwContinueStatus = OnLoadDllDebugEvent(DebugEv);
			break;
		}
		case UNLOAD_DLL_DEBUG_EVENT: {
			dwContinueStatus = OnUnloadDllDebugEvent(DebugEv);
			break;
		}
		case RIP_EVENT: {
			dwContinueStatus = OnRipEvent(DebugEv);
			break;
		}
		case EXCEPTION_DEBUG_EVENT: {
			switch (DebugEv->u.Exception.ExceptionRecord.ExceptionCode) {
				case EXCEPTION_ACCESS_VIOLATION: {
					// First chance: Pass this on to the system. 
					// Last chance: Display an appropriate error. 
					break;
				}
				case EXCEPTION_BREAKPOINT: {
					// First chance: Display the current 
					// instruction and register values. 
					break;
				}
				case EXCEPTION_DATATYPE_MISALIGNMENT: {
					// First chance: Pass this on to the system. 
					// Last chance: Display an appropriate error. 
					break;
				}
				case EXCEPTION_SINGLE_STEP: {
					// First chance: Update the display of the 
					// current instruction and register values. 
					break;
				}
				case DBG_CONTROL_C: {
					// First chance: Pass this on to the system. 
					// Last chance: Display an appropriate error. 
					break;
				}
				default: {
					// Handle other exceptions. 
					break;
				}
			}
			break;
		}
	}

	return dwContinueStatus;
}




void ErrorExit(LPTSTR lpszFunction) {
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}