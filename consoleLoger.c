#include "stdafx.h"

#include "consoleLoger.h"

WORD GetConsoleColour(void) {
	WORD Attributes;
	CONSOLE_SCREEN_BUFFER_INFO Info;
	GetConsoleScreenBufferInfo(gHandleStdout, &Info);
	Attributes = Info.wAttributes;
	return Attributes;
}

void 
screenLogger(
	LogType logType, 
	char *message
) 
{
	// backup current console theme
	WORD attributes = GetConsoleColour();

	switch (logType) {
		case LOG_TYPE_INFO: 
		case LOG_TYPE_PROMPT: 
		case LOG_TYPE_DONE: {
			printf("%s\r\n", message);
			fflush(stdout);
			break;
		}
		case LOG_TYPE_ERROR: {
			fprintf(stderr, "%s\r\n", message);
			fflush(stdout);
			break;
		}
	}
}

void logger(LogType logType, char *first, ...) {

	va_list vl;
	char *str = first;
	
	va_start(vl, first);

	while (str) {
		if (str[0] != '\0') {
			screenLogger(logType, str);
		}
		str = va_arg(vl, char*);
	};
}