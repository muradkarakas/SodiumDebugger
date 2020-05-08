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