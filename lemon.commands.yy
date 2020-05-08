%include
{ 
#include "pch.h"

#include <stdio.h>
#include <assert.h>

#include "..\SodiumExtension\mkSessionDataStructures.h"

#include "commandsDataStructures.h"
#include "commandParser.imp.h"
#include "commands.h"
#include "consoleThread.h"
#include "consoleLoger.h"
#include "lemon.commands.h"
}

%name commandParse

%extra_argument { TokenCommand *Command }

%token_type		{ TokenCommand }

%default_type	{ TokenCommand }

%syntax_error	{
	Command->errorCode = COMMAND_ERROR_CODE_SYNTAX_ERROR;
}


start	::= validcommands.
start	::= invalidcommands.

/*	*****************************************************************
	VALID COMMANDs
	****************************************************************/
validcommands ::= validcommands command.
validcommands ::= command.
{
	BOOL success = cmdImp(Command); 
	if (success) {
		// command is executed successfully
		//DLOG("\nDone\n\n", NULL);
	}
}

command		::= info.
command		::= kill.
command		::= exit.
command		::= quit.
command		::= show.
command		::= attach.
command		::= detach.
command		::= cmdexecute.
command		::= backtrace.
command		::= frame.
command		::= continue.
command		::= source.
command		::= break.
command		::= tbreak.
command		::= delete.
command		::= next.
command		::= step.
command		::= finish.
command		::= dummy.
command		::= whatis.
command		::= output.
command		::= set.
command		::= catch.

/*	*****************************************************************
	Custom Commands for VS Code Debugger Adaptor
	****************************************************************/
command		::= clearallbreakpoints.

/*	*****************************************************************
	clearallbreakpoints	Command	variants
	****************************************************************/
	clearallbreakpoints	::= CMD_CLEAR_ALL_BREAKPOINTS clearallbreakpointsparam	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_CLEAR_ALL_BREAKPOINTS;
}
clearallbreakpointsparam	::= clearallbreakpointsparamdoublequote	CMD_CLEARALLBREAKPOINT_PARAM_FULLFILEPATH(FULLFILEPATH_VALUE) clearallbreakpointsparamdoublequote.
{
	char* fullFilePath = FULLFILEPATH_VALUE.tokenStr;
	strncpy_s(Command->params->p.clearallbreakpoint_cmd_params.fullFilePath, MAX_PATH, fullFilePath, FULLFILEPATH_VALUE.tokenStrLength);
}
clearallbreakpointsparamdoublequote	::= CMD_CLEARALLBREAKPOINT_PARAM_DOUBLEQUOTE.
{

}

/*	*****************************************************************
	catch	Command	variants
	****************************************************************/
catch			::= CMD_CATCH		catchparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_CATCH;
}
catchparams	::= CMD_CATCH_THROW.
{
	Command->params->p.catch_cmd_params.throw = TRUE;
}


/*	*****************************************************************
	set	Command	variants
	****************************************************************/
set			::= CMD_SET		setparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_SET;
}
setparams	::= CMD_SET_CONFIRM CMD_SET_OFF.
{
	Command->params->p.set_cmd_params.confirm_off = TRUE;
}
setparams	::= CMD_SET_WIDTH CMD_SET_NUMERIC(WIDTH).
{
	char *width = WIDTH.tokenStr;
	Command->params->p.set_cmd_params.width = atoi(width);
}
setparams	::= CMD_SET_HEIGHT CMD_SET_NUMERIC(HEIGHT).
{
	char *height = HEIGHT.tokenStr;
	Command->params->p.set_cmd_params.width = atoi(height);
}
setparams	::= CMD_SET_BREAKPOINT CMD_SET_PENDING CMD_SET_ON.
{
	Command->params->p.set_cmd_params.breakpoint_pending_on = TRUE;
}
setparams	::= CMD_SET_PRINT CMD_SET_ASM_DEMANGLE CMD_SET_ON.
{
	Command->params->p.set_cmd_params.print_asm_demangle_on = TRUE;
}
setparams	::= CMD_SET_PRINT CMD_SET_ELEMENTS CMD_SET_NUMERIC(ELEMENT).
{
	char *element = ELEMENT.tokenStr;
	Command->params->p.set_cmd_params.print_element = atoi(element);
}
setparams	::= CMD_SET_UNWINDONSIGNAL CMD_SET_ON.
{
	Command->params->p.set_cmd_params.unwindonsignal_on = TRUE;
}
setparams	::= CMD_SET_DISASSEMBLY_FLAVOR CMD_SET_ATT.
{
	Command->params->p.set_cmd_params.disassembly_flavor_att = TRUE;
}
setparams	::= CMD_SET_PROMPT CMD_SET_PROMPT_STRING(PROMPT).
{
	char *prompt = PROMPT.tokenStr;
	strcpy_s(Command->params->p.set_cmd_params.prompt, MAX_PROMPT_LENGTH, prompt);
}


/*	*****************************************************************
	output	Command	variants
	****************************************************************/
output			::= CMD_OUTPUT		outputparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_OUTPUT;
}
outputparams	::= CMD_OUTPUT_VARIABLE_NAME(VARIABLE_NAME).
{
	char *variableName = VARIABLE_NAME.tokenStr;
	strncpy_s(Command->params->p.output_cmd_params.variableName, MAX_VARIABLE_NAME_LENGTH, variableName, MAX_VARIABLE_NAME_LENGTH);
	Command->params->p.output_cmd_params.variableName[MAX_VARIABLE_NAME_LENGTH] = '\0';
}
outputparams	::= .
{
	Command->params->p.output_cmd_params.variableName[0] = '\0';
}

/*	*****************************************************************
	whatis	Command	variants
	****************************************************************/
whatis			::= CMD_WHATIS		whatisparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_WHATIS;
}
whatisparams	::= CMD_WHATIS_VARIABLE_NAME(VARIABLE_NAME).
{
	char *variableName = VARIABLE_NAME.tokenStr;
	strncpy_s(Command->params->p.whatis_cmd_params.variableName, MAX_VARIABLE_NAME_LENGTH, variableName, MAX_VARIABLE_NAME_LENGTH);
	Command->params->p.whatis_cmd_params.variableName[MAX_VARIABLE_NAME_LENGTH] = '\0';
}
whatisparams	::= .
{
	Command->params->p.whatis_cmd_params.variableName[0] = '\0';
}

/*	*****************************************************************
	quit	Command	variants
	****************************************************************/
quit		::= CMD_QUIT	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_QUIT;
}

/*	*****************************************************************
	dummy	Command	variants
	****************************************************************/
dummy		::= CMD_DUMMY	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_DUMMY;
}

/*	*****************************************************************
	next	Command	variants
	****************************************************************/
next		::= CMD_NEXT	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_NEXT;
}

/*	*****************************************************************
	finish	Command	variants
	****************************************************************/
finish		::= CMD_FINISH	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_FINISH;
}


/*	*****************************************************************
	step	Command	variants
	****************************************************************/
step		::= CMD_STEP	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_STEP;
}

/*	*****************************************************************
	delete	Command	variants
	****************************************************************/
	delete		::= CMD_DELETE	deleteparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_DELETE;
}
deleteparams	::= CMD_DELETE_PARAM_BREAKPOINTS CMD_DELETE_PARAM_NUMERIC(BREAKPOINTID).
{
	char *breakpointID = BREAKPOINTID.tokenStr;
	Command->params->p.delete_cmd_params.paramType = CMD_DELETE_PARAM_TYPE_BREAKPOINTS;
	Command->params->p.delete_cmd_params.breakpointID = atoi(breakpointID);
}
deleteparams	::= .
{
	Command->params->p.delete_cmd_params.paramType = CMD_DELETE_PARAM_TYPE_NOTSET;
}

/*	*****************************************************************
	break	Command	variants
	****************************************************************/
	break		::= CMD_BREAK		breakparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_BREAK;
}
breakparams	::= breakdoublequote	CMD_BREAK_PARAM_FULLFILEPATH(FULLFILEPATH_VALUE) breakdoublequote.
{	
	char *fullFilePath = FULLFILEPATH_VALUE.tokenStr;

	char *colonPos = strrchr(fullFilePath, ':');
	if (colonPos) {
		*colonPos = (char) '\0';
		colonPos++;
	}
	strncpy_s(Command->params->p.break_cmd_params.fullFilePath, MAX_PATH, fullFilePath, FULLFILEPATH_VALUE.tokenStrLength);
	//Command->params->p.break_cmd_params.fullFilePath[SESSION_ID_LENGTH] = '\0';
	Command->params->p.break_cmd_params.lineNumber = atoi(colonPos);
}
breakdoublequote	::= CMD_BREAK_PARAM_DOUBLEQUOTE.
{
	int test = 1;
	int a  = test * 2;
}


/*	*****************************************************************
	tbreak	Command	variants
	****************************************************************/
tbreak		::= CMD_TBREAK		tbreakparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_TBREAK;
}
tbreakparams	::= tbreakdoublequote	CMD_TBREAK_PARAM_FULLFILEPATH(FULLFILEPATH_VALUE) tbreakdoublequote.
{	
	char *fullFilePath = FULLFILEPATH_VALUE.tokenStr;

	char *colonPos = strrchr(fullFilePath, ':');
	if (colonPos) {
		*colonPos = (char) '\0';
		colonPos++;
	}
	strncpy_s(Command->params->p.tbreak_cmd_params.fullFilePath, MAX_PATH, fullFilePath, FULLFILEPATH_VALUE.tokenStrLength);
	Command->params->p.tbreak_cmd_params.lineNumber = atoi(colonPos);
}
tbreakdoublequote	::= CMD_TBREAK_PARAM_DOUBLEQUOTE.


/*	*****************************************************************
	source	Command	variants
	****************************************************************/
source		::= CMD_SOURCE		sourceparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_SOURCE;
}
sourceparams	::= CMD_SOURCE_PARAM_FULLFILEPATH(FULLFILEPATH_VALUE).
{	
	char *fullFilePath = FULLFILEPATH_VALUE.tokenStr;
	strncpy_s(Command->params->p.source_cmd_params.fullFilePath, MAX_PATH, fullFilePath, FULLFILEPATH_VALUE.tokenStrLength);
	Command->params->p.source_cmd_params.fullFilePath[SESSION_ID_LENGTH] = '\0';
}


/*	*****************************************************************
	continue	Command	variants
	****************************************************************/
continue	::= CMD_CONTINUE	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_CONTINUE;
}

/*	*****************************************************************
	frame	Command	variants
	****************************************************************/
	frame		::= CMD_FRAME		frameparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_FRAME;
}
frameparams	::= CMD_FRAME_PARAM_FRAMENO(FRAMENO_VALUE).
{	
	Command->params->p.frame_cmd_params.frameNo = atoi(FRAMENO_VALUE.tokenStr);
}

/*	*****************************************************************
	backtrace	Command	variants
	****************************************************************/
backtrace		::= CMD_BACKTRACE		backtraceparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_BACKTRACE;
}
backtraceparams	::= CMD_BACKTRACE_PARAM_FRAMECOUNT(FRAMECOUNT_VALUE).
{	
	Command->params->p.backtrace_cmd_params.frameCount = atoi(FRAMECOUNT_VALUE.tokenStr);
}
backtraceparams	::= CMD_BACKTRACE_PARAM_OTHERS(FRAMECOUNT_VALUE).
{
	char *otherParamsStr = FRAMECOUNT_VALUE.tokenStr;
	if (strcmp(otherParamsStr, "full") == 0) {
		Command->params->p.backtrace_cmd_params.full = TRUE;
	}
}
backtraceparams	::= .
{
	Command->params->p.backtrace_cmd_params.frameCount = -1;
	Command->params->p.backtrace_cmd_params.full = FALSE;
}

/*	*****************************************************************
	attach	Command	variants
	****************************************************************/
attach			::= CMD_ATTACH		attachparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_ATTACH;
}
attachparams	::= CMD_ATTACH_PARAM_SESSION_ID(SESSION_ID_VALUE).
{
	char *lSessionId = SESSION_ID_VALUE.tokenStr;
	strncpy_s(Command->params->p.attach_cmd_params.sessionId, SESSION_ID_LENGTH+2, lSessionId, SESSION_ID_LENGTH);
	Command->params->p.attach_cmd_params.sessionId[SESSION_ID_LENGTH] = '\0';
}
attachparams	::= .
{
	Command->params->p.attach_cmd_params.sessionId[0] = '\0';
}


/*	*****************************************************************
	detach	Command	variants
	****************************************************************/
	detach		::= CMD_DETACH	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_DETACH;
}

/*	*****************************************************************
	kill	Command	variants
	****************************************************************/
kill		::= CMD_KILL	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_KILL;
}

/*	*****************************************************************
	info	Command	variants
	****************************************************************/
info		::= CMD_INFO	infoparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_INFO;
}
infoparams	::= CMD_INFO_PARAM_LOCALS.
{
	Command->params->p.info_cmd_params.paramType = CMD_INFO_PARAM_TYPE_LOCALS;
}
infoparams	::= CMD_INFO_PARAM_ARGS.
{
	Command->params->p.info_cmd_params.paramType = CMD_INFO_PARAM_TYPE_ARGS;
}
infoparams	::= CMD_INFO_PARAM_GLOBALS.
{
	Command->params->p.info_cmd_params.paramType = CMD_INFO_PARAM_TYPE_GLOBALS;
}
infoparams	::= CMD_INFO_PARAM_FRAME.
{
	Command->params->p.info_cmd_params.paramType = CMD_INFO_PARAM_TYPE_FRAME;
}


/*	*****************************************************************
	show	Command	variants
	****************************************************************/
show		::= CMD_SHOW	showparams	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_SHOW;
}
showparams	::= CMD_SHOW_PARAM_SESSION_COUNT.
{
	Command->params->p.show_cmd_params.sessionCountValue = SHOW_CMD_PARAM_SESSION_COUNT_VALUE_SET;
}
showparams	::= CMD_SHOW_PARAM_VERSION.
{
	Command->params->p.show_cmd_params.version = TRUE;
}


/*	*****************************************************************
	exit	Command	
	****************************************************************/
exit		::= CMD_EXIT	semicolon	cmdexecute	CMD_END_OF_FILE.
{
	Command->tokenCode = CMD_EXIT;
}


/*	*****************************************************************
	INVALID COMMANDs
	****************************************************************/
invalidcommands	::= invalidchars(CMD)	CMD_END_OF_FILE.
{
	writeSyntaxError(CMD);
}
invalidcommands	::= invalidchars(CMD)	cmdexecute	CMD_END_OF_FILE.
{
	writeSyntaxError(CMD);
}

invalidchars	::= invalidchars	invalidchar.
invalidchars	::= invalidchar.
invalidchar		::=	CMD_ANY_CHARACTER.


/*	*****************************************************************
	GENERIC RULEs
	****************************************************************/
cmdexecute		::= cmdexecute	cmdexecutechar.
cmdexecute		::= cmdexecutechar.
cmdexecutechar	::= CMD_EXECUTE.

semicolon		::= CMD_SEMI_COLON.
semicolon		::= .