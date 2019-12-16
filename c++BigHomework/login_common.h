#include "stdafx.h"
#include "sqlite3.h"

BOOL check(CString username, CString password, int user);

void  getConn(sqlite3** conn);