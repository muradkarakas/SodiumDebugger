#pragma once

#define VARIABLE_NAME_SIZE	200
#define VARIABLE_VALUE_SIZE 500

typedef struct _Variable {
	char variableName[VARIABLE_NAME_SIZE];
	char variableValue[VARIABLE_VALUE_SIZE];
	struct _Variable *next;
} Variable;


