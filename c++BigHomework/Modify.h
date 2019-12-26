#pragma once
#include "afxwin.h"
#include "Resource.h"

using namespace std;

// CModify 对话框

class CModify : public CDialogEx
{
	DECLARE_DYNAMIC(CModify)

public:
	CModify(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModify();

// 对话框数据
	enum { IDD = IDD_MODIFY_DIALOG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
