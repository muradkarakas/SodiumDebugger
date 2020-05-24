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

#include "cmdInfo.Args.imp.h"


BOOL 
cmdInfoImp_Args(
	TokenCommand *Command,
	char * outputBuffer
) 
{	
	// show all arguments
	ShowDebugeeLocalVariables(POST_OBJECT_GENERIC_TYPE_FUNCTION_ARGS, outputBuffer);
	
	return TRUE;
}

