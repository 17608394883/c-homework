#include "stdafx.h"

void  getConn(sqlite3** conn)
{
	//����������ݿ�
	int result = sqlite3_open(G2U("D:\\phone.db"), conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(*conn);
		*conn = NULL;
	}
}