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


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <WinBase.h>
#include <malloc.h>
#include <string.h>
#include <strsafe.h>

// INCLUDEs from htsql.dll project
#include "..\SodiumExtension\mkDataStructures.h"
#include "..\SodiumExtension\mkSessionDataStructures.h"
#include "..\SodiumExtension\mkConstants.h"
#include "..\SodiumExtension\serverCommands.h"

#define BUFSIZE 512

#define hWorkerProcessThread	0
#define hConsoleThread			1

/*	If any type of Debug Event occurs in debugee process, this boolean value will be TRUE until it is processed and debugee is resumed. 
	Just before debugger process resume, this value will be set to FALSE again.
	This variable is created for the reasons listed below :
		1.	Before communicating with debugee process via named pipe, programmer must check the value of this variable. 
			If it is TRUE, it means that all threads in debugee process is in suspended state and cannot reply to debugger's commands. 
			This also means that debugee and debugger is in deadlock state.
*/

typedef struct _TokenCommand	TokenCommand;
typedef enum _LogType			LogType;


#include "commandsDataStructures.h"

#include "commandUtils.h"

#include "debugeeInterface.h"

#include "lemon.commands.h"

// Current project's includes
#include "debugeeInterfaceDataStructures.h"
#include "debugeePipe.imp.h"
#include "debugeeInterface.h"
#include "debugEvents.h"

#include "consoleDataStructures.h"
#include "consoleThread.h"
#include "consoleLoger.h"

#include "workerProcessThread.h"

#include "commandParser.imp.h"
#include "commandUtils.h"

#include "cmdBacktrace.imp.h"
#include "cmdShow.imp.h"
#include "cmdExit.imp.h"
#include "cmdInfo.imp.h"
#include "cmdInfoUtils.h"
#include "cmdKill.imp.h"
#include "cmdAttach.imp.h"
#include "cmdDetach.imp.h"
#include "cmdBacktrace.imp.h"
#include "cmdFrame.imp.h"
#include "cmdContinue.imp.h"
#include "cmdSource.imp.h"
#include "cmdBreak.imp.h"
#include "cmdTbreak.imp.h"
#include "cmdDelete.imp.h"
#include "cmdNext.imp.h"
#include "cmdDummy.imp.h"
#include "cmdQuit.imp.h"
#include "cmdStep.imp.h"
#include "cmdFinish.imp.h"
#include "cmdWhatis.imp.h"
#include "cmdOutput.imp.h"
#include "cmdCatch.imp.h"
#include "cmdSet.imp.h"
#include "cmdClearAllBreakPoints.h"

#include "globals.h"

