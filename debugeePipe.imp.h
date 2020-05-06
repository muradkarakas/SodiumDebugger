#pragma once


/*	returns 0 if success*/
int		openDebugeeNamedPipeConnection();
void	closeDebugeeNamedPipeConnection();

/*	Caller must free return value.
	Function adds \r\n sequence to the end of the command.
*/
char	* sendCommandToDebugee(const char *command);
