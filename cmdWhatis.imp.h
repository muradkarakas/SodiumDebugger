#pragma once


/*	Returns TRUE if command is executed successfully, otherwise returns FALSE */
BOOL cmdWhatisImp(TokenCommand *Command);

void _cmdWhatisImp(char *varName, ObjectGenericType typeOfVariable, char * outputBuffer);

BOOL 
_cmdWhatisImpGlobal(
	TokenCommand *Command, 
	ObjectGenericType typeOfVariable,
	char *outputBuffer
);

void showPreObjectVariable(PreObjectVariable *preObjectVariable, char * outputBuffer);
