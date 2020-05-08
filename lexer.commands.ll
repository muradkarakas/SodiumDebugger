%{
#include "pch.h"

#include "lemon.commands.h"
%}

%option extra-type="struct _command *"
%option never-interactive
%option nounistd
%option reentrant
%option noyywrap noinput nounput

%x	SC_CMD_SHOW_PARAMETERS
%x	SC_CMD_INFO_PARAMETERS
%x	SC_CMD_ATTACH_PARAMETERS
%x	SC_CMD_BACKTRACE_PARAMETERS
%x	SC_CMD_FRAME_PARAMETERS
%x	SC_CMD_SOURCE_PARAMETERS
%x	SC_CMD_BREAK_PARAMETERS
%x	SC_CMD_CLEARALLBREAKPOINT_PARAMETERS
%x	SC_CMD_TBREAK_PARAMETERS
%x	SC_CMD_DELETE_PARAMETERS
%x	SC_CMD_WHATIS_PARAMETERS
%x	SC_CMD_OUTPUT_PARAMETERS
%x  SC_CMD_SET_PARAMETERS
%x  SC_CMD_CATCH_PARAMETERS

DOUBLEQUOTE					"\""
COLON						":"
FULLFILEPATH_WITHCOLON		[\\a-zA-Z0-9\.\-_\(\): /]+
FULLFILEPATH_WITHOUTCOLON	[\\a-zA-Z0-9\.\-_\(\) /]+
NUMERIC						[0-9][0-9]*
IDENTIFIER					[a-zA-Z_][a-zA-Z0-9_]*
SPACE						[ \t]*
PROMPT_REGEX				[a-zA-Z0-9_.>:\(\)\-]+

CMD_SOURCE		{SPACE}"source"{SPACE}
CMD_CONTINUE	{SPACE}"continue"|"cont"{SPACE}
CMD_FRAME		{SPACE}"f"|"frame"{SPACE}
CMD_BACKTRACE	{SPACE}"bt"|"backtrace"{SPACE}
CMD_EXIT		{SPACE}"exit"{SPACE}
CMD_BREAK		{SPACE}"break"{SPACE}
CMD_TBREAK		{SPACE}"tbreak"{SPACE}
CMD_DELETE		{SPACE}"delete"{SPACE}
CMD_NEXT		{SPACE}"next"{SPACE}
CMD_STEP		{SPACE}"step"{SPACE}
CMD_FINISH		{SPACE}"finish"{SPACE}
CMD_DUMMY		{SPACE}"dummy"{SPACE}
CMD_QUIT		{SPACE}"quit"|"q"{SPACE}
CMD_WHATIS		{SPACE}"whatis"{SPACE}
CMD_OUTPUT		{SPACE}"output"{SPACE}
CMD_SET			{SPACE}"set"{SPACE}
CMD_CATCH		{SPACE}"catch"{SPACE}

/*	*****************************************************************
	Custom Commands for VS Code Debugger Adaptor
	****************************************************************/
CMD_CLEAR_ALL_BREAKPOINTS	{SPACE}"clearallbreakpoints"{SPACE}


BEGIN(INITIAL);
%%

";"									{
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										return CMD_EXECUTE;
									}
{SPACE}"info"{SPACE}				{
										BEGIN(SC_CMD_INFO_PARAMETERS);
										return CMD_INFO;
									}
{CMD_NEXT}							{
										return CMD_NEXT;
									}
{CMD_DUMMY}							{
										return CMD_DUMMY;
									}
{CMD_EXIT}							{
										return CMD_EXIT;
									}
{CMD_QUIT}							{
										return CMD_QUIT;
									}
{CMD_STEP}							{
										return CMD_STEP;
									}
{CMD_FINISH}						{
										return CMD_FINISH;
									}
{CMD_SET}							{
										BEGIN(SC_CMD_SET_PARAMETERS);
										return CMD_SET;
									}
{CMD_CATCH}							{
										BEGIN(SC_CMD_CATCH_PARAMETERS);
										return CMD_CATCH;
									}
{CMD_BREAK}							{
										BEGIN(SC_CMD_BREAK_PARAMETERS);
										return CMD_BREAK;
									}
{CMD_TBREAK}						{
										BEGIN(SC_CMD_TBREAK_PARAMETERS);
										return CMD_TBREAK;
									}
{CMD_WHATIS}						{
										BEGIN(SC_CMD_WHATIS_PARAMETERS);
										return CMD_WHATIS;
									}
{CMD_OUTPUT}						{
										BEGIN(SC_CMD_OUTPUT_PARAMETERS);
										return CMD_OUTPUT;
									}
{SPACE}"attach"{SPACE}				{
										BEGIN(SC_CMD_ATTACH_PARAMETERS);
										return CMD_ATTACH;
									}
{CMD_DELETE}						{
										BEGIN(SC_CMD_DELETE_PARAMETERS);
										return CMD_DELETE;
									}
{CMD_SOURCE}						{
										BEGIN(SC_CMD_SOURCE_PARAMETERS);
										return CMD_SOURCE;
									}
{CMD_FRAME}							{
										BEGIN(SC_CMD_FRAME_PARAMETERS);
										return CMD_FRAME;
									}
{CMD_BACKTRACE}						{
										BEGIN(SC_CMD_BACKTRACE_PARAMETERS);
										return CMD_BACKTRACE;
									}
{CMD_CONTINUE}						{
										return CMD_CONTINUE;
									}
{CMD_CLEAR_ALL_BREAKPOINTS}			{
										BEGIN(SC_CMD_CLEARALLBREAKPOINT_PARAMETERS);
										return CMD_CLEAR_ALL_BREAKPOINTS;
									}
{SPACE}"detach"{SPACE}				{
										return CMD_DETACH;
									}
{SPACE}"kill"{SPACE}				{
										return CMD_KILL;
									}
{SPACE}"show"{SPACE}				{
										BEGIN(SC_CMD_SHOW_PARAMETERS);
										return CMD_SHOW;
									}
<<EOF>>								{
										return CMD_END_OF_FILE;
									}
[^\r\n; ]+							{
										return CMD_ANY_CHARACTER;
}

<SC_CMD_DELETE_PARAMETERS>{
"breakpoints"						{
										return CMD_DELETE_PARAM_BREAKPOINTS;
									}
{NUMERIC}							{
										return CMD_DELETE_PARAM_NUMERIC;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}


<SC_CMD_TBREAK_PARAMETERS>{
{FULLFILEPATH_WITHCOLON}			{
										return CMD_TBREAK_PARAM_FULLFILEPATH;
									}
{DOUBLEQUOTE}						{
										return CMD_TBREAK_PARAM_DOUBLEQUOTE;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_CLEARALLBREAKPOINT_PARAMETERS>{
{FULLFILEPATH_WITHCOLON}			{
										return CMD_CLEARALLBREAKPOINT_PARAM_FULLFILEPATH;
									}
{DOUBLEQUOTE}						{
										return CMD_CLEARALLBREAKPOINT_PARAM_DOUBLEQUOTE;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_BREAK_PARAMETERS>{
{FULLFILEPATH_WITHCOLON}			{
										return CMD_BREAK_PARAM_FULLFILEPATH;
									}
{DOUBLEQUOTE}						{
										return CMD_BREAK_PARAM_DOUBLEQUOTE;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_SOURCE_PARAMETERS>{
{FULLFILEPATH_WITHCOLON}			{
										return CMD_SOURCE_PARAM_FULLFILEPATH;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_FRAME_PARAMETERS>{
{NUMERIC}							{
										return CMD_FRAME_PARAM_FRAMENO;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_BACKTRACE_PARAMETERS>{
{NUMERIC}							{
										return CMD_BACKTRACE_PARAM_FRAMECOUNT;
									}
{IDENTIFIER}						{
										return CMD_BACKTRACE_PARAM_OTHERS;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}


<SC_CMD_INFO_PARAMETERS>{
"locals"							{
										return CMD_INFO_PARAM_LOCALS;
									}
"args"								{
										return CMD_INFO_PARAM_ARGS;
									}
"globals"							{
										return CMD_INFO_PARAM_GLOBALS;
									}
"frame"|"f"							{
										return CMD_INFO_PARAM_FRAME;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}

}


<SC_CMD_CATCH_PARAMETERS>{
"throw"								{
										return CMD_CATCH_THROW;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_SET_PARAMETERS>{
"confirm"							{
										return CMD_SET_CONFIRM;
									}
"breakpoint"						{
										return CMD_SET_BREAKPOINT;
									}
"pending"							{
										return CMD_SET_PENDING;
									}
"print"								{
										return CMD_SET_PRINT;
									}
"elements"							{
										return CMD_SET_ELEMENTS;
									}
"asm-demangle"						{
										return CMD_SET_ASM_DEMANGLE;
									}
"unwindonsignal"					{
										return CMD_SET_UNWINDONSIGNAL;
									}
"disassembly-flavor"				{
										return CMD_SET_DISASSEMBLY_FLAVOR;
									}
"att"								{
										return CMD_SET_ATT;
									}
"on"								{
										return CMD_SET_ON;
									}
"off"								{
										return CMD_SET_OFF;
									}
"width"								{
										return CMD_SET_WIDTH;
									}
"height"							{
										return CMD_SET_HEIGHT;
									}
"prompt"							{
										return CMD_SET_PROMPT;
									}
{NUMERIC}							{
										return CMD_SET_NUMERIC;
									}
{PROMPT_REGEX}						{
										return CMD_SET_PROMPT_STRING;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_OUTPUT_PARAMETERS>{
{IDENTIFIER}						{
										return CMD_OUTPUT_VARIABLE_NAME;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_WHATIS_PARAMETERS>{
{IDENTIFIER}							{
										return CMD_WHATIS_VARIABLE_NAME;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_ATTACH_PARAMETERS>{
{NUMERIC}							{
										return CMD_ATTACH_PARAM_SESSION_ID;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

<SC_CMD_SHOW_PARAMETERS>{
"SessionCount"						{
										return CMD_SHOW_PARAM_SESSION_COUNT;
									}
"version"							{
										return CMD_SHOW_PARAM_VERSION;
									}
";"									{
										BEGIN(INITIAL);
										return CMD_SEMI_COLON;
									}
[\r\n]+								{
										BEGIN(INITIAL);
										return CMD_EXECUTE;
									}
}

%%

