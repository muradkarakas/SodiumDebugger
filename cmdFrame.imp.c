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

#include "cmdFrame.imp.h"
#include "cmdBacktrace.imp.h"

BOOL cmdFrameImp(TokenCommand *Command) {
	//	Check parameters for error conditions

	// Parameters are correct. lets execute the show command
	CallStack *frame = getFrameByFrameNumber(Command->params->p.frame_cmd_params.frameNo);
	if (frame) {
		//printFrame(frame);
		//printFrameBreakpointLine(frame);
		gActiveFrame = Command->params->p.frame_cmd_params.frameNo;
		printf("{ \"frameset\" : \"%d\" }\r\n", gActiveFrame);
		free(frame);
		// setting last frame number to a global variable. "info args" and "info local" will need them
	}
	else {
		gActiveFrame = -1;
		printf("\nFrame not found: %d\r\n", Command->params->p.frame_cmd_params.frameNo);
	}

	fflush(stdout);
	return TRUE; 
}

void printFrameBreakpointLine(CallStack *frame) {
	char *lineStr = getFrameBreakpointLine(frame);
	if (lineStr) {
		ILOG(lineStr);
		free(lineStr);
	}
}

char *getFrameBreakpointLine(CallStack *frame) {
	// ":16:160:beg:0x4015d8"
	char *command = malloc(1000);
	strcpy_s(command, 1000, "");

	if (frame == NULL || frame->functionDebugInfo == NULL) {
		ELOG("Internal error.");
		return command;
	}

	Function_Debug_Info *debugInfo = _rpm(frame->functionDebugInfo, sizeof(Function_Debug_Info));
	if (debugInfo) {
		char lineStr[10] = "";
		PreObjectFunction  *preObjectFunction = _rpm(frame->preObjectFunction, sizeof(PreObjectFunction));
		if (preObjectFunction) {
			int lineNo = debugInfo->lineNumberInner + preObjectFunction->lineNumberOuter;
			_itoa_s(lineNo, lineStr, 10, 10);
			if (debugInfo->fileURI[0] != L'\0') {
				strcat_s(command, 1000, "\32\32");
				char fUri[MAX_PATH] = "";
				wcstombs_s(NULL, fUri, MAX_PATH, debugInfo->fileURI, wcslen(debugInfo->fileURI));
				strcat_s(command, 1000, fUri);
				strcat_s(command, 1000, ":");
				strcat_s(command, 1000, lineStr);
				strcat_s(command, 1000, ":1:beg:0x00000");
			}
			else {
				strcat_s(command, 1000, " at noavailable.c:1");
			}
			free(preObjectFunction);
		}
		
		free(debugInfo);
	}
	else {
		ELOG("\n\nDebugInfo cannot be obtained\n\n");
	}

	return command;
}

/*void printFrame(CallStack *frame) {
	char *lineStr = getFrameLine(frame);
	if (lineStr) {
		if (lineStr[0] != '\0')
			ILOG(lineStr);
		free(lineStr);
	}
}*/

/*char *getFrameLine(CallStack *frame) {
	//	Return value format:
	//		"#0  0x0000 in test (sym=0x00000, data=...) at macro.c:242\n"

	char *command = malloc(1000);
	strcpy_s(command, 1000, "");

	if (frame == NULL || frame->closureName == NULL) {
		ELOG("Internal error. Invalid parameter.");
		return command;
	}

	char *funcName = _rpm(frame->closureName, MAX_FUNCTION_NAME_LENGTH);
	if (funcName) {
		char *stackId = getFrameId(frame);
		if (stackId) {
			strcpy_s(command, 1000, "#");
			strcat_s(command, 1000, stackId);
			strcat_s(command, 1000, " 0x0000 in ");
			strcat_s(command, 1000, funcName);
			strcat_s(command, 1000, " (");
			strcat_s(command, 1000, ")");
			if (frame->functionDebugInfo) {
				Function_Debug_Info *debugInfo = _rpm(frame->functionDebugInfo, sizeof(Function_Debug_Info));
				if (debugInfo) {
					char lineStr[10] = "";
					PreObjectFunction* preObjectFunction = _rpm(frame->preObjectFunction, sizeof(PreObjectFunction));
					if (preObjectFunction) {
						int lineNo = debugInfo->lineNumberInner + preObjectFunction->lineNumberOuter;
						_itoa_s(lineNo, lineStr, 10, 10);
						if (debugInfo->fileURI[0] != L'\0') {
							strcat_s(command, 1000, " at ");
							char fUri[MAX_PATH] = "";
							wcstombs_s(NULL, fUri, MAX_PATH, debugInfo->fileURI, wcslen(debugInfo->fileURI));
							strcat_s(command, 1000, fUri);
							strcat_s(command, 1000, ":");
							strcat_s(command, 1000, lineStr);
						}
						else {
							strcat_s(command, 1000, " at noavailable.c:1");
						}
						free(preObjectFunction);
					}
					
					free(debugInfo);
				}
				else {
					ELOG("\n\nDebugInfo cannot be obtained\n\n");
				}
			}
			strcat_s(command, 1000, "\r\n");
			free(stackId);
		}
		free(funcName);
	}

	return command;
}*/

CallStack *getFrameByFrameNumber(int frameNumber) {
	CallStack *ret = NULL;

	HTSQLPage *page = GetCurrentPage(gAttachedLocalSessionInstance);
	if (page) {
		CallStack *cls = getCurrentClosure(page);
		while (cls) {

			char *stackId = getFrameId(cls);
			if (stackId) {
				if (frameNumber == atoi(stackId)) {
					free(stackId);
					ret = cls;
					break;
				}
				free(stackId);
			}

			// get down 
			CallStack*tmp = getDownFrame(cls);
			if (tmp) {
				free(cls);
				cls = tmp;
			}
			else {
				free(cls);
				cls = NULL;
			}
		}
		free(page);
	}
	else {
		ELOG("\n\nPage cannot be obtained\n\n");
	}

	return ret;
}

char *getFrameId(CallStack *frame) {
	char *stackId = calloc(1, 10);
	if (frame == NULL || frame->preObjectFunction == NULL) {
		return stackId;
	}
	PreObjectFunction *preFunction = _rpm(frame->preObjectFunction, sizeof(PreObjectFunction));

	if (preFunction) {
		_itoa_s(preFunction->callStackId, stackId, 10, 10);
	}
	return stackId;
}

CallStack*getDownFrame(CallStack*frame) {
	CallStack*ret = NULL;

	if (frame && frame->prev) {

		CallStack *previous = _rpm(frame->prev, sizeof(CallStack));

		if (previous) {
			ret = previous;
		}
	}
	return ret;
}
