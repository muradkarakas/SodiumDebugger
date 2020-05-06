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