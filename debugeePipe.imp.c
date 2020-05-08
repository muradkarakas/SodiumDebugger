#include "pch.h"

#include "debugeePipe.imp.h"


void closeDebugeeNamedPipeConnection() {
	if (gNamedPipeHandle != NULL)
		CloseHandle(gNamedPipeHandle);
	gNamedPipeHandle = NULL;
}

int openDebugeeNamedPipeConnection() {

	gNamedPipeHandle = CreateFile(
		lpszPipename,   // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE,
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		0,              // default attributes 
		NULL);          // no template file 

	// Break if the pipe handle is valid. 
	if (gNamedPipeHandle == INVALID_HANDLE_VALUE) {
		gNamedPipeHandle = NULL;
		wchar_t errorText[256];	
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorText, sizeof(errorText), NULL);
		return 1;
	}

	// Exit if an error other than ERROR_PIPE_BUSY occurs. 
	if (GetLastError() != 0) {
		//_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
		wchar_t errorText[256];	\
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorText, sizeof(errorText), NULL);
		return 1;
	}

	return 0;
}



char 
*sendCommandToDebugee(
	const char *command
) 
{
	char *chBuf = calloc(1, BUFSIZE);
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;

	if (!WaitNamedPipe(lpszPipename, 5000)) {
		char errorText[256];	
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorText, sizeof(errorText), NULL);
		ELOG("\r\nWaitNamedPipe failed\r\n", errorText);
		return NULL;
	}

	// The pipe connected; change to message-read mode. 
	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		gNamedPipeHandle,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess) {
		ELOG("\r\nSetNamedPipeHandleState failed.\r\n");
		return NULL;
	}

	// Send a message to the pipe server. 

	cbToWrite = (lstrlen(command) + 1) * sizeof(char);

	fSuccess = WriteFile(
		gNamedPipeHandle,                  // pipe handle 
		command,             // message 
		cbToWrite,              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 
	if (!fSuccess) {
		ELOG("\r\nWriteFile to pipe failed.\r\n");
		return NULL;
	}

	do
	{
		// Read from the pipe. 
		fSuccess = ReadFile(
			gNamedPipeHandle,    // pipe handle 
			chBuf,    // buffer to receive reply 
			BUFSIZE,  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);    // not overlapped 

		if (!fSuccess && GetLastError() != ERROR_MORE_DATA) {
			wchar_t errorText[256];	\
			FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorText, sizeof(errorText), NULL);
			ELOG("\r\nRead from file failed.\r\n");
			break;
		}

	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

	if (!fSuccess) {
		wchar_t errorText[256];	\
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorText, sizeof(errorText), NULL);
		ELOG("\r\nRead from file failed.\r\n");
		return NULL;
	}

	if (cbRead > 0) {
		chBuf[cbRead] = '\0';
	}
	return chBuf;
}