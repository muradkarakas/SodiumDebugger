#include "pch.h"

#include "commandParser.imp.h"

int parseAndExecuteCommandString(char *commandStr) {
		
	// lex scanner declaration
	yyscan_t scanner;
	// command info
	TokenCommand Command = { 0 };
	TokenCommand token = { 0 };

	// parameter info
	TokenCommandParams params = { 0 };
	// parser declaration
	void *pParser = NULL;

	YY_BUFFER_STATE cmd_scan_state;
	
	cmdlex_init_extra(&Command, &scanner);

	cmd_scan_state = cmd_scan_string(commandStr, scanner);

	Command.params = &params;

	//	get memory for parser
	pParser = commandParseAlloc(malloc);
	
	do {
		token.tokenCode = cmdlex(scanner);
		token.tokenStrLength = cmdget_leng(scanner);
		if (token.tokenStrLength >= COMMAND_STR_SIZE) {
			goto ret;    
		}
		strncpy_s(token.tokenStr, COMMAND_STR_SIZE, cmdget_text(scanner), token.tokenStrLength);
		token.tokenStr[token.tokenStrLength] = '\0';

  		commandParse(pParser, token.tokenCode, token, &Command);
	} while (token.tokenCode != CMD_END_OF_FILE /*&& token.tokenCode != CMD_EXIT*/);
	
	if (token.tokenCode == CMD_END_OF_FILE) {
		TokenCommand finisher = { 0 };
		finisher.params = &params;
		commandParse(pParser, finisher.tokenCode, finisher, &Command);
	}
	
	if (Command.errorCode == COMMAND_ERROR_CODE_SYNTAX_ERROR) {
		ELOG("Syntax error !\r\n");
	}
ret:
	commandParseFree(pParser, free);

	cmd_delete_buffer(cmd_scan_state, scanner);
	cmdlex_destroy(scanner);

	return Command.tokenCode;
}


void writeSyntaxError(TokenCommand cmd) {	
	ELOG("\nInvalid command : ", cmd.tokenStr, "\n\n");
	cmd.errorCode = COMMAND_ERROR_CODE_SYNTAX_ERROR;
}