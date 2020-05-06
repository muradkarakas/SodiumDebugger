#include "stdafx.h"

#include "..\SodiumShared\SodiumShared.h"

#include "cmdBreak.imp.h"
 

BOOL cmdBreakImp(TokenCommand *Command) {

	// check paramters

	if (Command->params->p.break_cmd_params.fullFilePath[0] == '\0') {
		ILOG("\nInvalid usage. File path provided. Example: \nbreak \"C:\\projects\\sodium\\Sodium-Site\\apps\\deps\\logon.sqlx:2\"\n\n");
		Command->errorCode = COMMAND_ERROR_CODE_INVALID_PARAMETER;
		return FALSE;
	}
	if (Command->params->p.break_cmd_params.lineNumber == 0) {
		ILOG("\nInvalid usage. No line number provided. Example: \nbreak \"C:/projects/sodium/Sodium-Site/apps/deps/logon.sqlx:2\"\n\n");
		Command->errorCode = COMMAND_ERROR_CODE_INVALID_PARAMETER;
		return FALSE;
	}

	mkPathSeperator(Command->params->p.break_cmd_params.fullFilePath);

	// last check and go
	if (gAttachedLocalSessionInstance == NULL) {
		gBreakpointsLocalPool[gBreakpointsLocalPoolCount].lineNumber = Command->params->p.break_cmd_params.lineNumber;
		strncpy_s(gBreakpointsLocalPool[gBreakpointsLocalPoolCount].fullFilePath, MAX_PATH, Command->params->p.break_cmd_params.fullFilePath, MAX_PATH);
		gBreakpointsLocalPoolCount++;
	}
	else {
		_cmdBreakImp(Command->params->p.break_cmd_params);
	}
	
	return TRUE;
}

void sendPooledBreakpointCommandsToDebugee() {
	if (gBreakpointsLocalPoolCount > 0 && gIsDebugeeInSuspendedMode == FALSE) {
		// There is "break" commands which is not executed before. We'll execute it after 
		for (int i = 0; i < gBreakpointsLocalPoolCount; i++) {
			_cmdBreakImp(gBreakpointsLocalPool[i]);
		}
		gBreakpointsLocalPoolCount = 0;
	}
}

void _cmdBreakImp(CMD_BREAK_PARAMS breakCommandParams) {
	int sBreakpointId = AddNewBreakpoint(breakCommandParams);
	if (sBreakpointId > 1) {
		WriteBreakpointAddedSuccessfullyResponse(breakCommandParams, sBreakpointId);
		//sBreakpointId++;
	}
}

int 
AddNewBreakpoint(
	CMD_BREAK_PARAMS breakCommandParams
) 
{
	char lineStr[10];
	
	if (gAttachedLocalSessionInstance == NULL) {
		ELOG("\n\nNo session attached\n\n");
		return 0;
	}

	// reread contents of the session instance
	int ret = _diRefreshActiveSessionInstance();
	if (ret != 0) {
		return -1;
	}

	_itoa_s(breakCommandParams.lineNumber, lineStr, 10, 10);

	for (int i = 0; i < MAX_BREAKPOINT_COUNT; i++) {
		if (gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.breakpointState == BREAKPOINT_STATE_DELETED) {
			mbstowcs_s(NULL, gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.fileURI, MAX_PATH, breakCommandParams.fullFilePath, MAX_PATH);
			strncpy_s(gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.lineNumber, BREAKPOINT_LINENUMBER_LENGTH, lineStr, 10);
			gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.id = gBreakpointId;
			gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.breakpointState = BREAKPOINT_STATE_ACTIVE;
			gAttachedLocalSessionInstance->breakpointsRoot[i].breakpoint.breakpointType = BREAKPOINT_TYPE_PERMINENT;
			if (gAttachedLocalSessionInstance->breakpointsRootAddress) {
				_wpm(gAttachedLocalSessionInstance->breakpointsRootAddress, gAttachedLocalSessionInstance->breakpointsRoot, sizeof(gAttachedLocalSessionInstance->breakpointsRoot));
			}			
			return gBreakpointId++;
		}
	}
	return -2;
}



void 
WriteBreakpointAddedSuccessfullyResponse(
	CMD_BREAK_PARAMS breakCommandParams, 
	int pBreakpointId
) 
{
	char lineStr[10];
	char breakpointIDStr[10];
	char breakpointCommand[1000];

	_itoa_s(breakCommandParams.lineNumber, lineStr, 10, 10);
	_itoa_s(pBreakpointId, breakpointIDStr, 10, 10);

	mkLowerCaseDriveLetterA(breakCommandParams.fullFilePath);

	// first line
	strcpy_s(breakpointCommand, 1000, "Breakpoint ");
	strcat_s(breakpointCommand, 1000, breakpointIDStr);
	strcat_s(breakpointCommand, 1000, " at 0x0000:  file ");
	strcat_s(breakpointCommand, 1000, breakCommandParams.fullFilePath);
	strcat_s(breakpointCommand, 1000, ", line ");
	strcat_s(breakpointCommand, 1000, lineStr);
	strcat_s(breakpointCommand, 1000, ".\r\n");
	// second line
	ILOG(breakpointCommand);
	//	Breakpoint 4 at 0x4015cf: file C:\temp\test\main.c, line 11.
}



