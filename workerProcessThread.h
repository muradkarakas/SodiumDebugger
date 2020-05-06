#pragma once


DWORD			GetSodiumServerProcessId();

void			killWorkerProcessThread(void);
DWORD WINAPI	WorkerProcessThreadFunction(LPVOID lpParam);

BOOL			DebugActiveHTSQLProcess(DWORD dwProcessId);