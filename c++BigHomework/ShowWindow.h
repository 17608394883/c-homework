#pragma once
#include "stdafx.h"
#include "Resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <string>
// CShowWindow 对话框

class CShowWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CShowWindow)

public:
	CShowWindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowWindow();

// 对话框数据
	enum { IDD = IDD_MAINWINDOW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnModify();
	afx_msg void OnBnClickedBtnSearch();
	CComboBox m_group;
	virtual BOOL OnInitDialog();
//	CListCtrl m_list;
	void show(CString username, CString phoneNumber, CString depName,int type);
	void startSet();
	std::string getDepIdByName(std::string department);
	CString m_username;
	CString m_phone;
	afx_msg void OnBnClickedBtnSearchall();
	afx_msg void OnBnClickedButton1();
	CListCtrl m_list;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMClickPhone(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnRemove();
	int userRole;
	afx_msg void OnBnClickedBtnRemove2();
	afx_msg void OnBnClickedBtnRemove3();
};
