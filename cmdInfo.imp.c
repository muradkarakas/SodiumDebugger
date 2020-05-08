#include "pch.h"

#include "cmdInfo.imp.h"
 
BOOL 
cmdInfoImp(
	TokenCommand *Command
) 
{
	BOOL	success = FALSE;
	char outputBufferArgs[2048] = "";
	char outputBufferLocals[2048] = "";
	char outputBufferGlobals[2048] = "";
	char outputBufferFrame[2048] = "";

	//	CHECK PARAMETERS FOR ERROR
	if (Command->params->p.info_cmd_params.paramType == CMD_INFO_PARAM_TYPE_NOTSET) {
		ILOG("\nInvalid usage; Supply a paramater name. Valid parameters are:\nshow locals;\nshow args;\n\n");
		Command->errorCode = COMMAND_ERROR_CODE_INVALID_PARAMETER;
		return FALSE;
	}

	int ret = _diRefreshActiveSessionInstance();
	if (ret != 0) {
		return FALSE;
	}

	// PARAMETERS ARE CORRECT. LETS EXECUTE COMMAND
	switch (Command->params->p.info_cmd_params.paramType) {
		case CMD_INFO_PARAM_TYPE_ARGS: {
			success = cmdInfoImp_Args(Command, outputBufferArgs);
			printf("{ \"args\": %s }\r\n",
				((outputBufferArgs[0] == '\0') ? "[]" : outputBufferArgs)
			);
			break;
		}
		case CMD_INFO_PARAM_TYPE_LOCALS: {
			success = cmdInfoImp_Locals(Command, outputBufferLocals);
			printf("{ \"locals\": %s }\r\n",
				((outputBufferLocals[0] == '\0') ? "[]" : outputBufferLocals)
			);
			break;
		}
		case CMD_INFO_PARAM_TYPE_FRAME: {
			success = cmdInfoImp_Frame(Command, outputBufferFrame);
			printf("{ \"frames\": %s }\r\n",
				((outputBufferFrame[0] == '\0') ? "[]" : outputBufferFrame)
			);
			break;
		}	
		case CMD_INFO_PARAM_TYPE_GLOBALS: {
			success = cmdInfoImp_Globals(Command, outputBufferGlobals);
			printf("{ \"globals\": %s }\r\n",
				((outputBufferGlobals[0] == '\0') ? "[]" : outputBufferGlobals)
			);
			break;
		}								 
	}

	fflush(stdout);

	/*printf("{ \"args\": %s, \"locals\": %s, \"frames\": %s, \"globals\": %s }\r\n", 
		((outputBufferArgs[0] == '\0') ? "[]": outputBufferArgs),
		((outputBufferLocals[0] == '\0') ? "[]" : outputBufferLocals),
		((outputBufferFrame[0] == '\0') ? "[]" : outputBufferFrame),
		((outputBufferGlobals[0] == '\0') ? "[]" : outputBufferGlobals)
	);*/

	return success;
}

