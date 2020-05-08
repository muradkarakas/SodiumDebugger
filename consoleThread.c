#include "pch.h"

#include "consoleThread.h"

DWORD WINAPI 
ConsoleThreadFunction(
	LPVOID lpParam
) 
{
	int reasonToExit = 0;
	__try {

		while (TRUE) {

			//WritePrompt(FALSE);

			// read command
			char *command = ReadIDECommand();

			if (command) {
				int tokenCode = parseAndExecuteCommandString(command);
				
				// quit if we get "exit" or "quit" command
				if (tokenCode == CMD_EXIT || tokenCode == CMD_QUIT) {
					reasonToExit = tokenCode;
					break;
				}

				// we will quit if we get NEXT, STEP or CONTINUE debug command and in debugee application in suspended state
				if (gIsDebugeeInSuspendedMode == TRUE) {
					if (tokenCode == CMD_NEXT || tokenCode == CMD_STEP || tokenCode == CMD_CONTINUE || tokenCode == CMD_FINISH) {
						reasonToExit = tokenCode;
						break;
					}
				}

				free(command);
			}
		}

	}
	__except (EXCEPTION_CONTINUE_EXECUTION) {
		ELOG("Error occured.\r\n");
	}

	return reasonToExit;
}

void 
WritePrompt(
	BOOL bypassSuspendenThread
) 
{
	if (_isWorkerProcessThreadinSuspendedState() == FALSE || bypassSuspendenThread) {
		if (gPrompt[0] != '\0') {
			PLOG(gPrompt);
		}
		return;
	}
}

char * 
ReadIDECommand() 
{
	BOOL	bSuccess;
	DWORD	dwRead;
	char	*command = malloc(COMMAND_TEXT_BUFFER_SIZE);

	if (command) {
		// reading command from command line or IDE
		bSuccess = ReadFile(gHandleStdin, command, COMMAND_TEXT_BUFFER_SIZE, &dwRead, NULL);
		if (dwRead > 0) {
			if (command[0] != '\r' && command[0] != '\n')
				command[dwRead] = '\0';
			//bSuccess = WriteFile(gHandleStdout, command, dwRead, &dwWritten, NULL);
		}
		else {
			ELOG("Unknown command received: ", command);
		}
	}
	else {
		// problem with memory allocation
		command = NULL;
	}
	
	return command;
}

char *getResponse(char *command) {
	char *response = NULL;

	parseAndExecuteCommandString(command);

	return response;
}

void writeResponse(const char *response) {

}


void SetIORedirectionHandles() {
	gHandleStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	gHandleStdin = GetStdHandle(STD_INPUT_HANDLE);
	if ((gHandleStdout == INVALID_HANDLE_VALUE) || (gHandleStdin == INVALID_HANDLE_VALUE)) {
		ExitProcess(1);
	}
}