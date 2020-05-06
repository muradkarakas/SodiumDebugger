#pragma once


/*	Returns TRUE if command is executed successfully, otherwise returns FALSE */
BOOL cmdFrameImp(TokenCommand *Command);

/*	if it finds the frame stated by "frameNumber" parameter, return PreObjectGeneric local instance. Otherwise returns NULL.
	Caller must FREE the value returned	if it is not null	*/
CallStack *getFrameByFrameNumber(int frameNumber);

/*	Always return a string. If error occured, then string will be the empty. 
	In all cases, Caller must FREE the value returned	*/
//char *getFrameLine(CallStack *frame);

/*	Always return a string. If error occured, then string will be the empty.
	In all cases, Caller must FREE the value returned	*/
char *getFrameBreakpointLine(CallStack *frame);

void printFrameBreakpointLine(CallStack *frame);

//void printFrame(CallStack *frame);

/*	Caller must free return memory	*/
CallStack *getDownFrame(CallStack *callStack);

/*	Caller must free return memory	*/
char *getFrameId(CallStack *callStack);
