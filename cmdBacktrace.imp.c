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

#include "cmdBacktrace.imp.h"
#include "cmdFrame.imp.h"

BOOL cmdBacktraceImp(TokenCommand *Command) {

	//	Check parameters for error conditions
	if (gRootSessionLocalInstance == NULL) {
		return FALSE;
	}

	// LETS GO
	int frameNumber = 0;
	CallStack *frame = getFrameByFrameNumber(frameNumber);
	while (frame) {
		//printFrame(frame);
		free(frame);

		frameNumber++;
		frame = getFrameByFrameNumber(frameNumber);
	}
	
	return TRUE;
}

