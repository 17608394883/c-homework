
// c++BigHomework.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcBigHomeworkApp: 
// �йش����ʵ�֣������ c++BigHomework.cpp
//

class CcBigHomeworkApp : public CWinApp
{
public:
	CcBigHomeworkApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CcBigHomeworkApp theApp;