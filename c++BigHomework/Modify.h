#pragma once
#include "afxwin.h"
#include "Resource.h"

using namespace std;

// CModify �Ի���

class CModify : public CDialogEx
{
	DECLARE_DYNAMIC(CModify)

public:
	CModify(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModify();

// �Ի�������
	enum { IDD = IDD_MODIFY_DIALOG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	
	CString m_username;
	CString m_password;
	CComboBox m_group;
	CString m_depName;
	CString m_phone;
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedButton1();
};
