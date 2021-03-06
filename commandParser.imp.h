/**
 * This file is part of Sodium Language project
 *
 * Copyright � 2020 Murad Karaka� <muradkarakas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License v3.0
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 *	https://choosealicense.com/licenses/gpl-3.0/
 */

#pragma once



/*	Returns token code */
int					parseAndExecuteCommandString(char *commandStr);
void				writeSyntaxError(TokenCommand cmd);

/*	LEXER & PARSER IMPLEMENTATIONS */

typedef void*		yyscan_t;
typedef struct		yy_buffer_state *YY_BUFFER_STATE;

int					cmdlex_destroy(yyscan_t yyscanner);
void				cmd_delete_buffer(YY_BUFFER_STATE  b, yyscan_t yyscanner);
void				commandParseFree( void *p, void(*freeProc)(void*));
void				*commandParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE));
void				commandParseFree(void *p, void(*freeProc)(void*) );
int					cmdlex(yyscan_t yyscanner);
char				*cmdget_text(yyscan_t yyscanner);
int					cmdget_leng(yyscan_t yyscanner);
//int					cmdlex_init_extra(TokenCommand * yy_user_defined, yyscan_t* ptr_yy_globals);
YY_BUFFER_STATE		cmd_scan_string(const char * yystr, yyscan_t yyscanner);
void				cmdset_out(FILE *  _out_str, yyscan_t yyscanner);
void				commandParse(void *yyp, int yymajor, TokenCommand yyminor, TokenCommand *Command);
void				commandParseTrace(FILE *TraceFILE, char *zTracePrompt);