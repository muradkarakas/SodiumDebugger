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

#include "cmdWhatis.imp.h"
#include "cmdInfoUtils.h"


BOOL cmdWhatisImp(TokenCommand *Command) 
{
	char outputBuffer[2048] = "";

	// check parameters
	if (Command->params->p.whatis_cmd_params.variableName[0] == '\0') {
		ELOG("\nVariable name required. Usage:\nwhatis count\n\n");
		Command->errorCode = COMMAND_ERROR_CODE_INVALID_PARAMETER;
		return FALSE;
	}
	// go

	// getting type of the variable if it is exists. If it is not exists, function will return -3
	ObjectGenericType typeOfVariable = getTypeOfPostObjectGeneric(Command->params->p.whatis_cmd_params.variableName);
	
	if (typeOfVariable == -3) {
		// variable not found in local scope. Searching for global variable
		BOOL isFound = _cmdWhatisImpGlobal(Command, typeOfVariable, outputBuffer);
		//if (isFound == FALSE)
		//	ILOG("No symbol \"", Command->params->p.whatis_cmd_params.variableName, "\" in current context.");
	}
	else {
		// variable found in local scope. 
		_cmdWhatisImp(Command->params->p.whatis_cmd_params.variableName, typeOfVariable, outputBuffer);
	}

	if (outputBuffer[0] != '\0') {
		printf_s("{ \"watch\": %s }\r\n", outputBuffer);
	}
	else {
		printf_s("{ \"watch\": { \"name\" : \"%s\", \"value\" : \"Not Found\" } }\r\n", Command->params->p.whatis_cmd_params.variableName);
	}

	fflush(stdout);
	
	return TRUE;
}

BOOL 
_cmdWhatisImpGlobal(
	TokenCommand *Command, 
	ObjectGenericType typeOfVariable,
	char *outputBuffer
)
{
	PreObjectGeneric *current = NULL, *next = NULL;
	HTSQLPage *page = GetCurrentPage(gAttachedLocalSessionInstance);
	if (page) {

		// Step.1: Looking for page object
		if (page->rootPreObjectGeneric) {
			current = (PreObjectGeneric *)_rpm(page->rootPreObjectGeneric, sizeof(PreObjectGeneric));
			while (current) {
				char *varName = (current->objectName) ? (char*) _rpm(current->objectName, MAX_VARIABLE_NAME_LENGTH) : _strdup("");

				if (varName) {
					if (strcmp(varName, Command->params->p.whatis_cmd_params.variableName) == 0) {
						if (current->value.preObjectVariable) {
							PreObjectVariable *preObjectVariable = (PreObjectVariable *)_rpm(current->value.preObjectVariable, sizeof(PreObjectVariable));
							showPreObjectVariable(preObjectVariable, outputBuffer);
							free(preObjectVariable);
						}
					}
					free(varName);
				}
				
				next = current->next;
				free(current);
				
				if (next) {
					current = (PreObjectGeneric *)_rpm(next, sizeof(PreObjectGeneric));
				}
				else {
					current = NULL;
				}				
			}
 		}

		// Step.2: Looking for root objects
		/*
		if (gAttachedLocalSessionInstance->rootPage->rootPreObjectGeneric) {
			current = (PreObjectGeneric *)_rpm(gAttachedLocalSessionInstance->rootPage->rootPreObjectGeneric, sizeof(PreObjectGeneric));
			while (current) {
				char *varName = (current->objectName) ? (char*)_rpm(current->objectName, MAX_VARIABLE_NAME_LENGTH) : _strdup("");

				if (strcmp(current->objectName, Command->params->p.whatis_cmd_params.variableName) == 0) {
					if (current->preObjectVariable) {
						PreObjectVariable *preObjectVariable = (PreObjectVariable *) _rpm(current->preObjectVariable, sizeof(PreObjectVariable));
						showPreObjectVariable(current);
						free(preObjectVariable);
					}										
				}
				next = current->next;
				free(current);
				free(varName);
				if (next) {
					current = (PreObjectGeneric *)_rpm(next, sizeof(PreObjectGeneric));
				}
				else {
					current = NULL;
				}
			}
		}
		*/

		free(page);
	}
	return FALSE;
}

void showPreObjectVariable(PreObjectVariable *preObjectVariable, char *outputBuffer) {
	if (preObjectVariable == NULL || preObjectVariable->variableName == NULL)
		return;
	
	char *varName = (char*)_rpm(preObjectVariable->variableName, MAX_VARIABLE_NAME_LENGTH);

	switch (preObjectVariable->preObjectVariableType) {
		case PREOBJECTTYPE_VARIABLE_NUMBER: {
			char* varValue = (char*)_rpm(preObjectVariable->value.text, MAX_VARIABLE_VALUE_LENGTH);
			ShowDebugeeLocalVariableNameAndValue(outputBuffer, "number", varName, varValue);
			free(varValue);
			break;
		}
		case PREOBJECTTYPE_VARIABLE_VARCHAR: {
			char* varValue = (char*)_rpm(preObjectVariable->value.text, MAX_VARIABLE_VALUE_LENGTH);
			ShowDebugeeLocalVariableNameAndValue(outputBuffer, "char", varName, varValue);
			free(varValue);
			break;
		}
		case PREOBJECTTYPE_VARIABLE_DATE: {
			char* varValue = (char*)_rpm(preObjectVariable->value.text, MAX_VARIABLE_VALUE_LENGTH);
			ShowDebugeeLocalVariableNameAndValue(outputBuffer, "date", varName, varValue);
			free(varValue);
			break;
		}
		case PREOBJECTTYPE_VARIABLE_VOID: {
			ShowDebugeeLocalVariableNameAndValue(outputBuffer, "void", varName, "{void}");
			break;
		}
		case PREOBJECTTYPE_VARIABLE_BOOL: {
			char* varValue = (char*)_rpm(preObjectVariable->value.text, MAX_VARIABLE_VALUE_LENGTH);
			ShowDebugeeLocalVariableNameAndValue(outputBuffer, "bool", varName, varValue);
			free(varValue);
			break;
		}
		case PREOBJECTTYPE_VARIABLE_REDIS: {
			ShowDebugeeLocalVariableNameAndValue(NULL, "redis", varName, "{redis object}");
			break;
		}
		case PREOBJECTTYPE_VARIABLE_RESULTSET: {
			ShowDebugeeLocalVariableNameAndValue(NULL, "resultset", varName, "{resultset object}");
			break;
		}
	}

	if (varName)
		free(varName);
}


void _cmdWhatisImp(char *varName, ObjectGenericType typeOfVariable, char * outputBuffer) {

	switch (typeOfVariable) {
		case POST_OBJECT_GENERIC_TYPE_VARIABLE:
		case POST_OBJECT_GENERIC_TYPE_FUNCTION_ARGS: {
			PostObjectGeneric *postObjectGeneric = getPostObjectGeneric(varName);
			if (postObjectGeneric) {
				PreObjectVariable *preObjectVariable = getPreObjectVariable(postObjectGeneric);
				if (preObjectVariable) {
					showPreObjectVariable(preObjectVariable, outputBuffer);
					free(preObjectVariable);
				}				
				free(postObjectGeneric);
			}
			break;
		}
		case POST_OBJECT_GENERIC_TYPE_FUNCTION: {
			ShowDebugeeLocalVariableNameAndValue(outputBuffer, "function", varName, "function");
			break;
		}
		default: {
			ELOG("\nError occured. Unknown type of variable : ", varName);
			break;
		}
	}


}