/**
 * This file is part of Sodium Language project
 *
 * Copyright � 2020 Murad Karaka� <muradkarakas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License v3.0
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 *	https://choosealicense.com/licenses/gpl-3.0/
 */

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
