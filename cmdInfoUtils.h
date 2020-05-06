#pragma once

void
ShowDebugeeGlobalVariables(
	int typeOfVariableToShow,
	char* outputBuffer
	);

/*
	typeOfVariableToShow:
		Valid constant values for 
			POST_OBJECT_GENERIC_TYPE_VARIABLE			shows local variables
			POST_OBJECT_GENERIC_TYPE_FUNCTION_ARGS		show current function arguments

*/
void 
ShowDebugeeLocalVariables(
	int typeOfVariableToShow,
	char* outputBuffer
);

/*	For arguments, look at the comment of the showDebugeeLocalVariables function	*/
void 
ShowDebugeeLocalVariable(
	PostObjectGeneric	*postObjectGeneric,
	char				*outputBuffer
);

void 
ShowDebugeeLocalVariableNameAndValue(
	char* outputBuffer,
	char *vartype,
	char *name, 
	char *value
);

/*
	Returns one of these 
		POST_OBJECT_GENERIC_TYPE_VARIABLE (0)
		POST_OBJECT_GENERIC_TYPE_FUNCTION_ARGS (2)
		POST_OBJECT_GENERIC_TYPE_RESULTSET
		POST_OBEJCT_TYPE_FUNCTION
	-1	-> if function is called while debugger is not in suspended state
	-2	-> if parameter is NULL
	-3	-> if variable does not exists
*/
ObjectGenericType		getTypeOfPostObjectGeneric(char *pVariableName);
PostObjectGeneric		*getPostObjectGeneric(char *pVariableName);
char					*getVariableValue(PostObjectGeneric *postObjectGeneric, char *pVariableName);
/*	Returns -1 if postObjectGeneric is null or its strucure	is invalid */
PREOBJECTVARIABLETYPE	getVariablePrimitiveTypeOfPostObjectGeneric(PostObjectGeneric *postObjectGeneric);
PreObjectVariable		*getPreObjectVariable(PostObjectGeneric *postObjectGeneric);

