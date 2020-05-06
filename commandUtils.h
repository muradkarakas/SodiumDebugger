#pragma once


/*	Caller must FREE return value	*/
HTSQLPage *
GetCurrentPage(
	SodiumSession *session
);

/*	Caller must FREE return value	*/
CallStack			*getCurrentClosure(HTSQLPage *page);


BOOL				check_number(const char *s);
void				mkPathSeperator(char *path);