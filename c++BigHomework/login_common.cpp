#include "login_common.h"
#include "stdafx.h"

//����û��������Ƿ���ȷ
BOOL check(CString username, CString password, int user)
{
	sqlite3* conn = NULL;
	//����������ݿ�
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(conn);
	}

	if (user == 1)
	{

	}
	else//�����ǹ���Ա
	{

	}

	return 1;
}

void  getConn(sqlite3** conn)
{
	//����������ݿ�
	int result = sqlite3_open(G2U("D:\\phone.db"), conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(*conn);
		*conn = NULL;
	}
}