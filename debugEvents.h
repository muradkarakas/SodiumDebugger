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



DWORD			ProcessDebugEvent(LPDEBUG_EVENT DebugEv);
void			EnterDebugLoop(const LPDEBUG_EVENT DebugEv);

DWORD			OnOutputDebugStringEvent(const LPDEBUG_EVENT de);

DWORD			OnCreateThreadDebugEvent(const LPDEBUG_EVENT de);
DWORD			OnExitThreadDebugEvent(const LPDEBUG_EVENT de);

DWORD			OnCreateProcessDebugEvent(const LPDEBUG_EVENT de);
DWORD			OnExitProcessDebugEvent(const LPDEBUG_EVENT de);

DWORD			OnLoadDllDebugEvent(const LPDEBUG_EVENT de);
DWORD			OnUnloadDllDebugEvent(const LPDEBUG_EVENT de);

DWORD			OnRipEvent(const LPDEBUG_EVENT de);

void			ErrorExit(LPTSTR lpszFunction);