#pragma once

#include "cmdInfo.Frame.imp.h"
#include "cmdInfo.Locals.imp.h"
#include "cmdInfo.Globals.imp.h"
#include "cmdInfo.Args.imp.h"
#include "cmdInfoUtils.h"

/*	Returns TRUE if command is executed successfully, otherwise returns FALSE */
BOOL cmdInfoImp(TokenCommand *Command);


