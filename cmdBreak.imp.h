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

