#pragma once

#include "afxwin.h"
#include "afxdialogex.h"
#include "resource.h"
#include "c++BigHomework.h"

// CAdd �Ի���

class CAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CAdd)

public:
	CAdd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdd();

// �Ի�������
	enum { IDD = IDD_ADD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_username;
	CString m_phone;
	CString m_pswd;
	virtual BOOL OnInitDialog();
	CComboBox m_group;
	afx_msg void OnBnClickedButton1();
};
