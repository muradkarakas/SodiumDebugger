#include "stdafx.h"

#include "cmdInfo.imp.h"
#include "cmdInfoUtils.h"

char *getVariableValue(PostObjectGeneric *postObjectGeneric, char *pVariableName) {
	char *varValue = NULL; 
	
	if (pVariableName == NULL)
		return NULL;

	PreObjectVariable *var = (PreObjectVariable *)_rpm(postObjectGeneric->value.preObjectVariable, sizeof(PreObjectVariable));
	if (var) {
		if (var->variableName) {
			char *varName = (char*)_rpm(var->variableName, MAX_VARIABLE_NAME_LENGTH);
			if (varName) {
				if (strcmp(varName, pVariableName) == 0) {
					switch (var->preObjectVariableType) {
						case PREOBJECTTYPE_VARIABLE_VARCHAR:
						case PREOBJECTTYPE_VARIABLE_DATE:
						case PREOBJECTTYPE_VARIABLE_BOOL:
						case PREOBJECTTYPE_VARIABLE_NUMBER: {
							if (var->value.text)
								varValue = (char*)_rpm(var->value.text, MAX_VARIABLE_NAME_LENGTH);
							break;
						}
					}
				}
				free(varName);
			}
		}
		free(var);
	}
	return varValue;
}

PostObjectGeneric *
getPostObjectGeneric(
	char *pVariableName
)
{
	PostObjectGeneric *retValue = NULL;
	PostObjectGeneric *tmp = NULL;

	if (pVariableName == NULL)
		return NULL;

	if (gIsDebugeeInSuspendedMode == FALSE) {
		// debugee must be is suspended state in order to get correct values
		return NULL;
	}

	if (_diRefreshActiveSessionInstance() == 0) {
		// current MKSession instance obtained
		if (gAttachedLocalSessionInstance) {
			// getting current HTSQLPage instance
			HTSQLPage *page = GetCurrentPage(gAttachedLocalSessionInstance);
			if (page) {
				if (page->currentCallStack) {
					CallStack* cls = _rpm(page->currentCallStack, sizeof(CallStack));
					if (cls) {
						// HTSQLPage instance obtained
						if (cls->rootPostObjectGeneric) {
							// there is more than one local variable or argument
							PostObjectGeneric* postObjectGeneric = (PostObjectGeneric*)_rpm(cls->rootPostObjectGeneric, sizeof(PostObjectGeneric));
							while (postObjectGeneric) {
								if (postObjectGeneric->objectName) {
									char* varName = (char*)_rpm(postObjectGeneric->objectName, MAX_VARIABLE_NAME_LENGTH);
									if (varName) {
										if (strcmp(varName, pVariableName) == 0) {
											retValue = postObjectGeneric;
											break;
										}
										free(varName);
									}
								}
								tmp = postObjectGeneric->next;
								free(postObjectGeneric);
								postObjectGeneric = (PostObjectGeneric*)_rpm(tmp, sizeof(PostObjectGeneric));
							}
						}
						free(cls);
					}
				}

				
				free(page);
			}
		}
	}
	else {
		ELOG("\nError refreshing active session");
	}

	return retValue;
}


ObjectGenericType getTypeOfPostObjectGeneric(char *pVariableName) {
	int retValue = -3;
	PostObjectGeneric *tmp = NULL;

	if (pVariableName == NULL)
		return -2;

	if (gIsDebugeeInSuspendedMode == FALSE) {
		// debugee must be is suspended state in order to get correct values
		return -1;
	}

	if (_diRefreshActiveSessionInstance() == 0) {
		// current MKSession instance obtained
		if (gAttachedLocalSessionInstance) {
			// getting current HTSQLPage instance
			HTSQLPage *page = GetCurrentPage(gAttachedLocalSessionInstance);
			if (page) {
				if (page->currentCallStack) {
					CallStack* cls = _rpm(page->currentCallStack, sizeof(CallStack));
					if (cls) {
						// HTSQLPage instance obtained
						if (cls->rootPostObjectGeneric) {
							// there is more than one local variable or argument
							PostObjectGeneric *postObjectGeneric = (PostObjectGeneric*)_rpm(cls->rootPostObjectGeneric, sizeof(PostObjectGeneric));
							while (postObjectGeneric) {
								if (postObjectGeneric->objectName) {
									char *varName = (char*)_rpm(postObjectGeneric->objectName, MAX_VARIABLE_NAME_LENGTH);
									if (varName) {
										if (strcmp(varName, pVariableName) == 0)
											retValue = postObjectGeneric->objectGenericType;
										free(varName);
									}
								}
								tmp = postObjectGeneric->next;
								free(postObjectGeneric);
								postObjectGeneric = (PostObjectGeneric*)_rpm(tmp, sizeof(PostObjectGeneric));
							}
						}
						free(cls);
					}
				}
				free(page);
			}
		}
	}
	else {
		ELOG("\nError refreshing active session");
	}
	return retValue;
}

void
ShowDebugeeGlobalVariables(
	int typeOfVariableToShow,
	char* outputBuffer
)
{
	size_t sizeOfOutputBuffer = strlen(outputBuffer);
	_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, 
		"[ { \"name\" : \"not_implemented_yet\", \"value\" : \"not_implemented_yet\", \"type\" : \"char\" } ]");
}

void 
ShowDebugeeLocalVariables(
	int typeOfVariableToShow,
	char *outputBuffer
) 
{
	PostObjectGeneric *tmp = NULL;
	BOOL	isAtLeastOneVariableShowed = FALSE;
	
	if (gIsDebugeeInSuspendedMode == FALSE) {
		// debugee must be is suspended state in order to get correct values
		return;
	}

	int ret = _diRefreshActiveSessionInstance();
	if (ret == 0) {

		// current MKSession instance obtained
		if (gAttachedLocalSessionInstance) {

			// getting current HTSQLPage instance
			HTSQLPage *page = GetCurrentPage(gAttachedLocalSessionInstance);
			if (page) {
				CallStack *frame = getFrameByFrameNumber(gActiveFrame);

				if (frame) {
					size_t sizeOfOutputBuffer = 0;
					if (frame->rootPostObjectGeneric) {
						sizeOfOutputBuffer = strlen(outputBuffer);
						_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, "[ ");

						PostObjectGeneric* postObjectGeneric = (PostObjectGeneric*)_rpm(frame->rootPostObjectGeneric, sizeof(PostObjectGeneric));
						while (postObjectGeneric) {
							if (postObjectGeneric->objectGenericType == typeOfVariableToShow) {
								if (isAtLeastOneVariableShowed) {
									// put an ',' character betwen variable json object string. Etc. { }, { },
									sizeOfOutputBuffer = strlen(outputBuffer);
									_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, ", ");
								}
								ShowDebugeeLocalVariable(postObjectGeneric, outputBuffer);
								isAtLeastOneVariableShowed = TRUE;
							}							
							tmp = postObjectGeneric->next;
							free(postObjectGeneric);
							postObjectGeneric = (PostObjectGeneric*)_rpm(tmp, sizeof(PostObjectGeneric));
						}

						// sending response to the IDE
						sizeOfOutputBuffer = strlen(outputBuffer);
						_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, " ]");
					}

					free(frame);
				}
				free(page);
			}
		}
	}
	else {
		ELOG("\nError refreshing active session");
	}

	if (isAtLeastOneVariableShowed == FALSE) {
		if (typeOfVariableToShow == POST_OBJECT_GENERIC_TYPE_FUNCTION_ARGS)
			NULL; //  ILOG("No arguments.\r\n");
		else if (typeOfVariableToShow == POST_OBJECT_GENERIC_TYPE_VARIABLE)
			NULL; //ILOG("No locals.\r\n");
		else
			ILOG("Unknown type of variable is requested.");
	}
}


void 
ShowDebugeeLocalVariableNameAndValue(
	char *outputBuffer,
	char *vartype, 
	char *name, 
	char *value
) 
{
	size_t sizeOfOutputBuffer = strlen(outputBuffer);

	if (name[0] != ':') {
		_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, "{ \"type\": \"%s\", \"name\": \"%s\", \"value\": \"%s\" }", vartype, name, (value) ? value : "");
	}
	else {
		_sprintf_p(&outputBuffer[sizeOfOutputBuffer], 2048 - sizeOfOutputBuffer, "{ \"type\": \"%s\", \"name\": \"%s\", \"value\": \"%s\" }", vartype, &name[1], (value) ? value : "");
	}
	/*if (name[0] != ':')
		ILOG(name, " = ", (value) ? value : "", "\r\n");
	else
		ILOG(&name[1], " = ", (value) ? value : "", "\r\n");*/
}

PREOBJECTVARIABLETYPE getVariablePrimitiveTypeOfPostObjectGeneric(PostObjectGeneric *postObjectGeneric) {
	PREOBJECTVARIABLETYPE retval = -2;
	if (postObjectGeneric == NULL || postObjectGeneric->value.preObjectVariable == NULL)
		return -1;

	PreObjectVariable *object = (PreObjectVariable *)_rpm(postObjectGeneric->value.preObjectVariable, sizeof(PreObjectVariable));
	if (object) {
		retval = object->preObjectVariableType;
		free(object);
	}
	return retval;
}

PreObjectVariable *getPreObjectVariable(PostObjectGeneric *postObjectGeneric) {

	if (postObjectGeneric == NULL || postObjectGeneric->value.preObjectVariable == NULL)
		return NULL;

	PreObjectVariable *object = (PreObjectVariable *)_rpm(postObjectGeneric->value.preObjectVariable, sizeof(PreObjectVariable));
	
	return object;
}


void 
ShowDebugeeLocalVariable(
	PostObjectGeneric	*postObjectGeneric,
	char				*outputBuffer
) 
{
	if (postObjectGeneric->objectGenericType == POST_OBJECT_GENERIC_TYPE_FUNCTION_ARGS ||
		postObjectGeneric->objectGenericType == POST_OBJECT_GENERIC_TYPE_VARIABLE) {

		PreObjectVariable *var = (PreObjectVariable *)_rpm(postObjectGeneric->value.preObjectVariable, sizeof(PreObjectVariable));
		if (var) {
			if (var->variableName) {
				char *varName = (char*)_rpm(var->variableName, MAX_VARIABLE_NAME_LENGTH);
				char *varValue = NULL;
				
				if (varName /*&& varName[0] != ':'*/) {
					if (var->value.text)
						varValue = (char*)_rpm(var->value.text, MAX_VARIABLE_NAME_LENGTH);

					switch (var->preObjectVariableType) {
						case PREOBJECTTYPE_VARIABLE_VARCHAR: {
							ShowDebugeeLocalVariableNameAndValue(outputBuffer, "char", varName, varValue);
							break;
						}
						case PREOBJECTTYPE_VARIABLE_DATE: {
							ShowDebugeeLocalVariableNameAndValue(outputBuffer, "date", varName, varValue);
							break;
						}
						case PREOBJECTTYPE_VARIABLE_BOOL: {
							ShowDebugeeLocalVariableNameAndValue(outputBuffer, "bool", varName, varValue);
							break;
						}
						case PREOBJECTTYPE_VARIABLE_NUMBER: {
							ShowDebugeeLocalVariableNameAndValue(outputBuffer, "number", varName, varValue);
							break;
						}
						case PREOBJECTTYPE_VARIABLE_REDIS: {
							ShowDebugeeLocalVariableNameAndValue(outputBuffer, "redis", varName, "{redis object}");
							break;
						}
						case PREOBJECTTYPE_VARIABLE_RESULTSET: {
							ShowDebugeeLocalVariableNameAndValue(outputBuffer, "resultset", varName, "{resultset object}");
							break;
						}
						case PREOBJECTTYPE_VARIABLE_VOID: {
							ShowDebugeeLocalVariableNameAndValue(outputBuffer, "void", varName, "null");
							break;
						}
					}				
				}

				if (varName)
					free(varName);
				if (varValue)
					free(varValue);
			}
			free(var);
		}

	}
	else {
		// Unknown varible type
		ELOG("\nUnknown local variable type");
	}

}