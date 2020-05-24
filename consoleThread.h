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

#include <Windows.h>


#pragma once


/* Debugger application's standart input and ouputs */
void	SetIORedirectionHandles();

/*	Returns command read as a NULL terminated string. Caller must FREE return value; */
char * 
ReadIDECommand();

/*   */
void
WritePrompt(
	BOOL bypassSuspendenThread
);

/*	*/
char			*getResponse(char *command);

/*	*/
void			writeResponse(const char *response);

/*	*/
void			writePrompt(BOOL bypassSuspendenThread);

/*	*/
DWORD WINAPI	ConsoleThreadFunction(LPVOID lpParam);