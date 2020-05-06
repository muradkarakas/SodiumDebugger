#include "stdafx.h"

#include "commandUtils.h"



HTSQLPage *
GetCurrentPage(
	SodiumSession  *session
) 
{
	HTSQLPage *page = NULL;
	if (session && session->currentPage) {
		page = _rpm(session->currentPage, sizeof(HTSQLPage));
	}
	return page;
}

CallStack *getCurrentClosure(HTSQLPage *page) {
	CallStack *obj = NULL;
	if (page) {
		if (page->currentCallStack) {
			obj = _rpm(page->currentCallStack, sizeof(CallStack));
		}
		else {
			// ELOG("hmmm", NULL);
		}
	}
	return obj;
}




BOOL check_number(const char *s) {
	long long v;
	char *e;
	v = strtoll(s, &e, 10);
	if (errno != 0 || *e != '\0' || v < 0 || v > UINT_MAX) {
		return FALSE;
	}
	return TRUE;
}

void mkPathSeperator(char *path) {
	if (path == NULL) {
		return;
	}
	size_t size = strlen(path);
	for (int i = 0; i < size; i++) {
		if (path[i] == '/') {
			path[i] = '\\';
		}
	}
}