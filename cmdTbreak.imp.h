#pragma once


/*	Returns TRUE if command is executed successfully, otherwise returns FALSE */
BOOL cmdTbreakImp(TokenCommand *Command);

void _cmdTbreakImp(CMD_BREAK_PARAMS breakCommandParams);


int addNewTBreakpoint(CMD_BREAK_PARAMS breakCommandParams);

void WriteTbreakpointAddedSuccessfullyResponse(CMD_BREAK_PARAMS breakCommandParams, int pBreakpointId);


