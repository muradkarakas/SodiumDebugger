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

#include "commands.h"


BOOL cmdImp(TokenCommand *Command) {
	BOOL	success = FALSE;  
	//Sleep(4000);
	if (Command->errorCode == COMMAND_ERROR_CODE_NOERROR) {

		switch (Command->tokenCode) {
			case CMD_SHOW: {
				success = cmdShowImp(Command);
				break;
			}
			case CMD_EXIT: {
				success = cmdExitImp(Command);
				break;
			}
			case CMD_INFO: {
				success = cmdInfoImp(Command);
				break;
			}
			case CMD_KILL: {
				success = cmdKillImp(Command);
				break;
			}
			case CMD_QUIT: {
				success = cmdQuitImp(Command);
				break;
			}
			case CMD_DETACH: {
				success = cmdDetachImp(Command);
				break;
			}
			case CMD_ATTACH: {
				success = cmdAttachImp(Command);
				break;
			}
			case CMD_BACKTRACE: {
				success = cmdBacktraceImp(Command);
				break;
			}
			case CMD_FRAME: {
				success = cmdFrameImp(Command);
				break;
			}
			case CMD_CONTINUE: {
				success = cmdContinueImp(Command);
				break;
			}
			case CMD_SOURCE: {
				success = cmdSourceImp(Command);
				break;
			}
			case CMD_FINISH: {
				success = cmdFinishImp(Command);
				break;
			}
			case CMD_BREAK: {
				success = cmdBreakImp(Command);
				break;
			}
			case CMD_TBREAK: {
				success = cmdTbreakImp(Command);
				break;
			}
			case CMD_DELETE: {
				success = cmdDeleteImp(Command);
				break;
			}
			case CMD_NEXT: {
				success = cmdNextImp(Command);
				break;
			}
			case CMD_STEP: {
				success = cmdStepImp(Command);
				break;
			}
			case CMD_WHATIS: {
				success = cmdWhatisImp(Command);
				break;
			}
			case CMD_OUTPUT: {
				success = cmdOutputImp(Command);
				break;
			}
			case CMD_DUMMY: {
				success = cmdDummyImp(Command);
				break;
			}
			case CMD_SET: {
				success = cmdSetImp(Command);
				break;
			}
			case CMD_CATCH: {
				success = cmdCatchImp(Command);
				break;
			}
			case CMD_CLEAR_ALL_BREAKPOINTS: {
				success = cmdClearAllBreakPoints(Command);
				break;
			}
			default: {
				break;
			}
		}
	}	

	return success;
}