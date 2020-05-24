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

#pragma once


/*	returns 0 if success*/
int		openDebugeeNamedPipeConnection();
void	closeDebugeeNamedPipeConnection();

/*	Caller must free return value.
	Function adds \r\n sequence to the end of the command.
*/
char	* sendCommandToDebugee(const char *command);
