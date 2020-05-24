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



#define COMMAND_STR_SIZE	512
#define PARAM_STR_SIZE		512
#define MAX_PROMPT_LENGTH	100

// command constants
#define COMRET_SESSION_NOT_FOUND	(-3)
#define COMRET_SUCCESS				(0)



/*	*****************************************************************************************
	COMMAND SPECIFIC DATA STRUCTURE:	clearAllBreakpoints
	****************************************************************************************/
typedef struct _CMD_CLEAR_ALL_BREAKPOINTS_PARAMS {
	//	File name musb be lower case
	char	fullFilePath[MAX_PATH];	
} CMD_CLEAR_ALL_BREAKPOINTS_PARAMS;

/*	*****************************************************************************************
	COMMAND SPECIFIC DATA STRUCTURE:	break
	****************************************************************************************/
typedef struct _CMD_BREAK_PARAMS {
	//	File name musb be lower case
	char	fullFilePath[MAX_PATH];	
	int		lineNumber;
} CMD_BREAK_PARAMS;


/*	*****************************************************************************************
	COMMAND SPECIFIC DATA STRUCTURE:	source
	****************************************************************************************/
typedef struct _CMD_SOURCE_PARAMS {
	char	fullFilePath[MAX_PATH];
} CMD_SOURCE_PARAMS;


/*	*****************************************************************************************
	COMMAND SPECIFIC DATA STRUCTURE:	frame
	****************************************************************************************/
typedef struct _CMD_FRAME_PARAMS {
	int	frameNo;
} CMD_FRAME_PARAMS;



/*	*****************************************************************************************
	CMD_BACKTRACE_PARAMS SPECIFIC DATA STRUCTURE:	backtrace
	****************************************************************************************/
typedef struct _CMD_BACKTRACE_PARAMS {
	int	frameCount;	
	BOOL full;
} CMD_BACKTRACE_PARAMS;


/*	*****************************************************************************************
	CMD_CATCH_PARAMS SPECIFIC DATA STRUCTURE:	catch
	****************************************************************************************/
typedef struct _CMD_CATCH_PARAMS {
	BOOL throw;
} CMD_CATCH_PARAMS;


/*	*****************************************************************************************
	CMD_SET_PARAMS SPECIFIC DATA STRUCTURE:	set
	****************************************************************************************/
typedef struct _CMD_SET_PARAMS {
	BOOL	confirm_off;
	int		width;
	BOOL	breakpoint_pending_on;
	BOOL	print_asm_demangle_on;
	BOOL	unwindonsignal_on;
	int		print_element;
	BOOL	disassembly_flavor_att;
	char    prompt[MAX_PROMPT_LENGTH];
} CMD_SET_PARAMS;



/*	*****************************************************************************************
	CMD_OUTPUT_PARAMS SPECIFIC DATA STRUCTURE:	output
	****************************************************************************************/
typedef struct _CMD_OUTPUT_PARAMS {
	char	variableName[MAX_VARIABLE_NAME_LENGTH +2];	// 2 addtional bytes are reserved for '\0' or "\r\n" characters 
} CMD_OUTPUT_PARAMS;


/*	*****************************************************************************************
	CMD_WHATIS_PARAMS SPECIFIC DATA STRUCTURE:	whatis
	****************************************************************************************/
typedef struct _CMD_WHATIS_PARAMS {
	char	variableName[MAX_VARIABLE_NAME_LENGTH +2];	// 2 addtional bytes are reserved for '\0' or "\r\n" characters 
} CMD_WHATIS_PARAMS;


/*	*****************************************************************************************
	COMMAND SPECIFIC DATA STRUCTURE:	attach
	****************************************************************************************/
typedef struct _CMD_ATTACH_PARAMS {
	char	sessionId[SESSION_ID_LENGTH+2];	// 2 addtional bytes are reserved for '\0' or "\r\n" characters 
} CMD_ATTACH_PARAMS;

/*	*****************************************************************************************
	COMMAND SPECIFIC DATA STRUCTURE:	show
	****************************************************************************************/
typedef enum _SHOW_CMD_PARAM_SESSION_COUNT_VALUE {
	SHOW_CMD_PARAM_SESSION_COUNT_VALUE_NOTSET,
	SHOW_CMD_PARAM_SESSION_COUNT_VALUE_SET
} SHOW_CMD_PARAM_SESSION_COUNT_VALUE;

typedef struct _CMD_SHOW_PARAMS {
	SHOW_CMD_PARAM_SESSION_COUNT_VALUE	sessionCountValue;
	char		customParam[PARAM_STR_SIZE];
	BOOL		version;
} CMD_SHOW_PARAMS;


/*	*****************************************************************************************
	COMMAND SPECIFIC DATA STRUCTURE:	delete
	****************************************************************************************/
typedef enum _CMD_DELETE_PARAM_TYPE {
	CMD_DELETE_PARAM_TYPE_NOTSET,
	CMD_DELETE_PARAM_TYPE_BREAKPOINTS,
	CMD_DELETE_PARAM_TYPE_BREAKPOINTNO
} CMD_DELETE_PARAM_TYPE;

typedef struct _CMD_DELETE_PARAMS {
	CMD_DELETE_PARAM_TYPE	paramType;
	int						breakpointID;
} CMD_DELETE_PARAMS;



/*	*****************************************************************************************
	COMMAND SPECIFIC DATA STRUCTURE:	info
	****************************************************************************************/
typedef enum _CMD_INFO_PARAM_TYPE {
	CMD_INFO_PARAM_TYPE_NOTSET,
	CMD_INFO_PARAM_TYPE_LOCALS,
	CMD_INFO_PARAM_TYPE_ARGS,
	CMD_INFO_PARAM_TYPE_FRAME,
	CMD_INFO_PARAM_TYPE_GLOBALS
} CMD_INFO_PARAM_TYPE;

typedef struct _CMD_INFO_PARAMS {
	CMD_INFO_PARAM_TYPE	paramType;
} CMD_INFO_PARAMS;


/*	*****************************************************************************************
	GENERIC: ERROR DATA STRUCTURE
	****************************************************************************************/
typedef enum _CommandErrorCodes {
	COMMAND_ERROR_CODE_NOERROR,
	COMMAND_ERROR_CODE_SYNTAX_ERROR,
	COMMAND_ERROR_CODE_INVALID_PARAMETER
} CommandErrorCode;

/*	*****************************************************************************************
	GENERIC: COMMAND PARAMETER DATA STRUCTURE
	****************************************************************************************/
typedef struct _TokenCommandParams {
	union {
		CMD_SHOW_PARAMS show_cmd_params;
		CMD_INFO_PARAMS info_cmd_params;
		CMD_ATTACH_PARAMS attach_cmd_params;
		CMD_BACKTRACE_PARAMS backtrace_cmd_params;
		CMD_FRAME_PARAMS frame_cmd_params;
		CMD_SOURCE_PARAMS source_cmd_params;
		CMD_BREAK_PARAMS break_cmd_params;
		CMD_BREAK_PARAMS tbreak_cmd_params;
		CMD_DELETE_PARAMS delete_cmd_params;
		CMD_WHATIS_PARAMS whatis_cmd_params;
		CMD_OUTPUT_PARAMS output_cmd_params;
		CMD_SET_PARAMS set_cmd_params;
		CMD_CATCH_PARAMS catch_cmd_params;
		CMD_CLEAR_ALL_BREAKPOINTS_PARAMS clearallbreakpoint_cmd_params;
	} p;
} TokenCommandParams;


/*	*****************************************************************************************
	GENERIC: COMMAND DATA STRUCTURE
	****************************************************************************************/
typedef struct _TokenCommand {
	int		tokenCode;
	int		tokenStrLength;
	char	tokenStr[COMMAND_STR_SIZE];
	TokenCommandParams	*params;
	CommandErrorCode	errorCode;
} TokenCommand;
