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

/*	
 * Returns TRUE if command is executed successfully, otherwise returns FALSE 
 */
BOOL cmdBreakImp(TokenCommand *Command);

void _cmdBreakImp(CMD_BREAK_PARAMS breakCommandParams);


/*	Returns breakpoint id. If value returned is lower than 2, it means error occured, otherwise it is a valid break point id.
	0->	Process memory cannot be read
*/
int 
AddNewBreakpoint(
	CMD_BREAK_PARAMS breakCommandParams
);

void 
WriteBreakpointAddedSuccessfullyResponse(
	CMD_BREAK_PARAMS breakCommandParams, 
	int pBreakpointId
);



void sendPooledBreakpointCommandsToDebugee();

