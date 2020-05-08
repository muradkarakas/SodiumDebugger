#include "pch.h"

#include "cmdInfo.Frame.imp.h"

BOOL 
cmdInfoImp_Frame(
	TokenCommand *Command,
	char* outputBuffer
) 
{
	if (gIsDebugeeInSuspendedMode == FALSE) {
		// debugee must be is suspended state in order to get correct stack frames.
		//ELOG("\r\nIllegal use of cmdInfoImp_Frame function. Debugee threads must be in suspended state\r\n");
		return FALSE;
	}
	HTSQLPage *page = GetCurrentPage(gAttachedLocalSessionInstance);
	if (page) {
		size_t sizeOfOutputBuffer = 0;

		sizeOfOutputBuffer = strlen(outputBuffer);
		_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, "[ ");

		CallStack *cls = _rpm(page->currentCallStack, sizeof(CallStack));
		if (cls) {
			while (cls) 
			{	
				char *funcName = _rpm(cls->closureName, MAX_FUNCTION_NAME_LENGTH);
				if (funcName) {

					sizeOfOutputBuffer = strlen(outputBuffer);
					_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, "{ ");

					sizeOfOutputBuffer = strlen(outputBuffer);
					_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, "\"procedure\": \"%s\"", funcName);
				
					if (cls->functionDebugInfo) {
						Function_Debug_Info *debugInfo = _rpm(cls->functionDebugInfo, sizeof(Function_Debug_Info));
						if (debugInfo) {
							char lineStr[10] = "", stackId[10] = "0";

							PreObjectFunction *preObjectFunction = _rpm(cls->preObjectFunction, sizeof(PreObjectFunction));
							if (preObjectFunction) {

								int lineNo = debugInfo->lineNumberInner + preObjectFunction->lineNumberOuter;
								_itoa_s(lineNo, lineStr, 10, 10);

								sizeOfOutputBuffer = strlen(outputBuffer);
								_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, ", \"line\": \"%s\"", lineStr);

								PreObjectFunction* preFunction = _rpm(cls->preObjectFunction, sizeof(PreObjectFunction));
								if (preFunction) {
									_itoa_s(preFunction->callStackId, stackId, 10, 10);

									sizeOfOutputBuffer = strlen(outputBuffer);
									_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, ", \"stackid\": \"%s\"", stackId);
								}

								if (debugInfo->fileURI[0] != L'\0') {
									// replacing '\' character with '$' 
									wchar_t* pos = wcsstr(debugInfo->fileURI, L"\\");
									while (pos) {
										*pos = L'$';
										pos = wcsstr(pos + 1, L"\\");
									}
									sizeOfOutputBuffer = strlen(outputBuffer);
									_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, ", \"file\": \"%ws\"", debugInfo->fileURI);
								}
								else {
									ELOG("\nNo source file name obtained\n");
									return FALSE;
								}

								free(preObjectFunction);
							}
							
							free(debugInfo);
						}
					}
					else {
						ERRLOG("\r\nfunctionDebugInfo structure not valid in cmdInfoImp_Frame function\r\n");
						return FALSE;
					}

					free(funcName);
				}
				else {
					ERRLOG("\r\nCurrent callstack function name cannot be obtained in cmdInfoImp_Frame function\r\n");
					return FALSE;
				}

				sizeOfOutputBuffer = strlen(outputBuffer);
				_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, " }");
			
				if (cls->prev) {
					sizeOfOutputBuffer = strlen(outputBuffer);
					_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, ", ");
				}

				CallStack *tmp = cls->prev;
				free(cls);
				cls = NULL;
				if (tmp != NULL)
					cls = _rpm(tmp, sizeof(CallStack));
			}
		
		} else {
			ELOG("\r\nCurrent callstack function instance cannot be obtained in cmdInfoImp_Frame function\r\n");
			return FALSE;
		}

		sizeOfOutputBuffer = strlen(outputBuffer);
		_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, " ]");

		free(page);
	}
	else {
		ELOG("\r\nPage object cannot be obtained in function cmdInfoImp_Frame\r\n");
		return FALSE;
	}

	return TRUE;
}

