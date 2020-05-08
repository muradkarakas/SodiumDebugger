#include "pch.h"

#include "cmdSet.imp.h"

BOOL cmdSetImp(TokenCommand *Command) {
	// check parameters

	// go
	if (Command->params->p.set_cmd_params.prompt[0] != '\0') {
		strcpy_s(gPrompt, MAX_PROMPT_LENGTH, Command->params->p.set_cmd_params.prompt);
		ILOG("\n\n....prompt is set....\n\n");
	}
	return TRUE;
}