#pragma once


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
};
