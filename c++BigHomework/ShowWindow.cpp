// ShowWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "c++BigHomework.h"
#include "ShowWindow.h"
#include "afxdialogex.h"
#include "resource.h"
#include "Add.h"
#include "Modify.h"

// CShowWindow �Ի���

IMPLEMENT_DYNAMIC(CShowWindow, CDialogEx)

CShowWindow::CShowWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowWindow::IDD, pParent)
	, m_username(_T(""))
	, m_phone(_T(""))
{

}

CShowWindow::~CShowWindow()
{
}

void CShowWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_group);

	DDX_Text(pDX, IDC_EDIT_NAME, m_username);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_phone);
	DDX_Control(pDX, LIST_PHONE, m_list);
}


BEGIN_MESSAGE_MAP(CShowWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD, &CShowWindow::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CShowWindow::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BUTTON1, &CShowWindow::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShowWindow::OnBnClickedButton2)
END_MESSAGE_MAP()


// CShowWindow ��Ϣ�������


void CShowWindow::OnBnClickedBtnAdd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CAdd *addDlg = new CAdd;

	addDlg->Create(IDD_ADD_DIALOG);
	addDlg->ShowWindow(SW_SHOW);
}


void CShowWindow::OnBnClickedBtnModify()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CModify *modifyDlg = new CModify;

	modifyDlg->Create(IDD_MODIFY_DIALOG);
	modifyDlg->ShowWindow(SW_SHOW);

}










BOOL CShowWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	startSet();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CShowWindow::show(CString username, CString phoneNumber, CString depName,int type){
	
	
	sqlite3* conn = NULL;
	//����������ݿ�
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		MessageBox(TEXT("�������ߴ����ݿ�ʧ��"));
	}

	std::string SQLString;
	//����������
	if (type==1)
	{
		std::string name = CT2A(username.GetString());
		std::string phone = CT2A(phoneNumber.GetString());
		std::string department = CT2A(depName.GetString());

		std::string depId = getDepIdByName(department);

		SQLString = "SELECT * FROM user WHERE depId = " + depId;
		if (name != string(""))
		{
			SQLString += " AND username LIKE '%" + name + "%'";
		}
		if (phone != string(""))
		{
			SQLString += " AND phone LIKE '%" + phone + "%'";
		}
	}
	else{
		 SQLString = "SELECT * FROM user";
	}
	


	vector<User> user;
	BOOL res = QueryUser(conn, SQLString, user);


	int j;

	//��������
	for (int i = 0; i < user.size(); i++) {
		j = 0;

		CString name(user[i].username.c_str());
		CString phone(user[i].phone.c_str());
		CString depName(searchDepNameById(user[i].depId).c_str());

		CString role;
		if (user[i].role == std::string("1"))
		{
			role = "����Ա";
		}
		else
		{
			role = "��ͨԱ��";
		}

		m_list.InsertItem(i, name);
		//������һ�������е�ֵ
		m_list.SetItemText(i, ++j, phone);
		m_list.SetItemText(i, ++j, depName);
		m_list.SetItemText(i, ++j, role);

	}
}

void CShowWindow::startSet(){

	/*************************���ñ�ͷ***********************************************/
	
	CString str[] = { TEXT("����"), TEXT("��ϵ��ʽ"), TEXT("����"), TEXT("��ɫ"), };
	for (int i = 0; i < 4; i++) {
		//���ñ�ͷ  ����1 ����   2����  3���뷽ʽ   4�п��
		m_list.InsertColumn(i, str[i], LVCFMT_LEFT, 100);
	}
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	/***********************���ò���������*********************************************************/
	sqlite3* conn = NULL;
	std::string SQLString = "SELECT * FROM department";
	vector<Department> department;

	//����������ݿ�
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		MessageBox(TEXT("�������ߴ����ݿ�ʧ��"));
	}

	QueryDep(conn, SQLString, department);

	for (int i = 0; i < department.size();i++)
	{
		CString depName(department[i].depName.c_str());
		m_group.AddString(depName);
	}

	m_group.SetCurSel(0);

}


std::string CShowWindow::getDepIdByName(std::string departmentName){

	sqlite3* conn = NULL;
	std::string SQLString = "SELECT * FROM department";
	vector<Department> deps;

	//����������ݿ�
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		MessageBox(TEXT("�������ߴ����ݿ�ʧ��"));
	}

	QueryDep(conn, SQLString, deps);

	for (int i = 0; i < deps.size();i++)
	{
		if (deps[i].depName == departmentName)
			return to_string(deps[i].depId);
	}

	return NULL;
}





//std::string SQLString = "CREATE TABLE   test (name CHAR(124),phone CHAR(124),time CHAR(124), status CHAR(4))";
//BOOL createTableResult = sqlOperation(const_cast<char *>(SQLString.c_str()), &conn);
////����ʧ��
//if (createTableResult == FALSE)
//	MessageBox(TEXT("������ʧ��"));

void CShowWindow::OnBnClickedButton1()
{
	//���
	m_list.DeleteAllItems();

	UpdateData(TRUE);

	CString depName;
	CString username;
	CString phoneNumber;

	//��ȡ����������
	int index = m_group.GetCurSel();
	m_group.GetLBText(index, depName);

	//��ȡ�û���  �绰����
	username = m_username;
	phoneNumber = m_phone;


	show(username, phoneNumber, depName, 1);
}


void CShowWindow::OnBnClickedButton2()
{
	//���
	m_list.DeleteAllItems();
	CString str("");
	show(str, str, str, 0);
}
