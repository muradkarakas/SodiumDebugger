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