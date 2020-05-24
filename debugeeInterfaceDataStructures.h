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

#define VARIABLE_NAME_SIZE	200
#define VARIABLE_VALUE_SIZE 500

typedef struct _Variable {
	char variableName[VARIABLE_NAME_SIZE];
	char variableValue[VARIABLE_VALUE_SIZE];
	struct _Variable *next;
} Variable;


