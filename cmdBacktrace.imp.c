#include "stdafx.h"

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

