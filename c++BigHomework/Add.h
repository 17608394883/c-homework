#pragma once

#include "afxwin.h"
#include "afxdialogex.h"
#include "resource.h"
#include "c++BigHomework.h"

// CAdd 对话框

class CAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CAdd)

public:
	CAdd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdd();

// 对话框数据
	enum { IDD = IDD_ADD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_username;
	CString m_phone;
	CString m_pswd;
	virtual BOOL OnInitDialog();
	CComboBox m_group;
	afx_msg void OnBnClickedButton1();
};
