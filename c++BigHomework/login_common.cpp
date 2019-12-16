#include "login_common.h"
#include "stdafx.h"

//检查用户名密码是否正确
BOOL check(CString username, CString password, int user)
{
	sqlite3* conn = NULL;
	//创建或打开数据库
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);
	}

	if (user == 1)
	{

	}
	else//否则是管理员
	{

	}

	return 1;
}

void  getConn(sqlite3** conn)
{
	//创建或打开数据库
	int result = sqlite3_open(G2U("D:\\phone.db"), conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(*conn);
		*conn = NULL;
	}
}