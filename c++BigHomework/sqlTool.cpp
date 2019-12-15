#include "sqlTool.h"
#include "stdafx.h"



char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

BOOL  sqlOperation(char *SQL, sqlite3 **conn){

	sqlite3_stmt* stmt = NULL;
	int a = sqlite3_prepare_v2(*conn, SQL, strlen(SQL), &stmt, NULL);
	if (sqlite3_prepare_v2(*conn, SQL, strlen(SQL), &stmt, NULL) != SQLITE_OK) {
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(*conn);
		
		return FALSE;
	}
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		sqlite3_close(*conn);
		
		return FALSE;
	}
	//释放创建表语句对象的资源。
	sqlite3_finalize(stmt);
	
	return TRUE;

}


BOOL QueryUser(sqlite3 *db, std::string sql, std::vector<User> &u)
{
	
	int count = 0;
	char *pErrMsg = 0;
	string m_SqlCommand(sql);
	char** pResult;
	int nRow;
	int nCol;
	int nResult = sqlite3_get_table(db,
		m_SqlCommand.c_str(),
		&pResult, &nRow, &nCol, &pErrMsg);
	if (nResult != SQLITE_OK)
	{
		//执行失败，需要清理错误码的内存空间
		sqlite3_free(pErrMsg);
		return FALSE;
	}
	int nIndex = nCol;
	int cnt = 0;
	for (int i = 0; i < nRow; i++)
	{
		User user; 
		for (int j = 0; j < nCol; j++)
		{
			std::string key(pResult[j]);  //转换成字符串才能与加好连接
			std::string value(pResult[nIndex]);
			
			if (key == std::string("uId")){
				user.uId = atoi(value.c_str());
			}
			else if (key == std::string("username"))
			{
				user.username = value;
			}
			else if (key == std::string("phone"))
			{
				user.phone = value;
			}
			else if (key == std::string("depId"))
			{
				user.depId = atoi(value.c_str());
			}
			else if (key == std::string("role"))
			{
				user.role = value;
			}
			else if (key == std::string("password"))
			{
				user.password = value;
			}
			
			++nIndex;
		}
		u.push_back(user);
		
	}
	sqlite3_free_table(pResult);
	return TRUE;
}


BOOL QueryDep(sqlite3 *db, std::string sql, std::vector<Department> &d)
{

	int count = 0;
	char *pErrMsg = 0;
	string m_SqlCommand(sql);
	char** pResult;
	int nRow;
	int nCol;
	int nResult = sqlite3_get_table(db,
		m_SqlCommand.c_str(),
		&pResult, &nRow, &nCol, &pErrMsg);
	if (nResult != SQLITE_OK)
	{
		//执行失败，需要清理错误码的内存空间
		sqlite3_free(pErrMsg);
		return FALSE;
	}
	int nIndex = nCol;
	int cnt = 0;
	for (int i = 0; i < nRow; i++)
	{
		Department department;
		for (int j = 0; j < nCol; j++)
		{
			std::string key(pResult[j]);  //转换成字符串才能与加好连接
			std::string value(pResult[nIndex]);

			if (key == std::string("depId")){
				department.depId = atoi(value.c_str());
			}
			else if (key == std::string("depName"))
			{
				department.depName = value;
			}
			

			++nIndex;
		}
		d.push_back(department);

	}
	sqlite3_free_table(pResult);
	return TRUE;
}

std::string searchDepNameById(int depId){
	
	sqlite3* conn = NULL;
	//创建或打开数据库
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);
		return NULL;
	}
	std::string SQLString = "SELECT depName FROM department WHERE depId=" + to_string(depId);
	vector<Department> department;
	BOOL res = QueryDep(conn, SQLString, department);

	return department[0].depName;

}