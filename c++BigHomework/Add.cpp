// Add.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "c++BigHomework.h"
#include "Add.h"
#include "afxdialogex.h"


// CAdd �Ի���

IMPLEMENT_DYNAMIC(CAdd, CDialogEx)

CAdd::CAdd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdd::IDD, pParent)
	, m_username(_T(""))
	, m_phone(_T(""))
	, m_pswd(_T(""))
{

}

CAdd::~CAdd()
{
}

void CAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT2, m_phone);
	DDX_Text(pDX, IDC_EDIT5, m_pswd);
	DDX_Control(pDX, IDC_COMBO1, m_group);
}


BEGIN_MESSAGE_MAP(CAdd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAdd::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAdd ��Ϣ�������


BOOL CAdd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	sqlite3* conn = NULL;
	std::string SQLString = "SELECT * FROM department";
	vector<Department> department;

	//����������ݿ�
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		AfxMessageBox(TEXT("�������ߴ����ݿ�ʧ��"));
		
	}

	QueryDep(conn, SQLString, department);

	for (int i = 0; i < department.size(); i++)
	{
		CString depName(department[i].depName.c_str());
		m_group.AddString(depName);
	}

	m_group.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

std::string getDepIdByName(std::string departmentName){

	sqlite3* conn = NULL;
	std::string SQLString = "SELECT * FROM department";
	vector<Department> deps;

	//����������ݿ�
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		AfxMessageBox(TEXT("�������ߴ����ݿ�ʧ��"));
	}

	QueryDep(conn, SQLString, deps);

	for (int i = 0; i < deps.size(); i++)
	{
		if (deps[i].depName == departmentName)
			return to_string(deps[i].depId);
	}

	return NULL;
}

void CAdd::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CString depName;
	CString username;
	CString phoneNumber;
	CString password;
	//��ȡ����������
	int index = m_group.GetCurSel();
	m_group.GetLBText(index, depName);

	//��ȡ�û���  �绰����
	username = m_username;
	phoneNumber = m_phone;
	password = m_pswd;

	std::string name = CT2A(username.GetString());
	std::string phone = CT2A(phoneNumber.GetString());
	std::string pswd = CT2A(password.GetString());

	std::string department = CT2A(depName.GetString());
	std::string depId =getDepIdByName(department);

	sqlite3* conn = NULL;
	//����������ݿ�
	int result = sqlite3_open("D:\\phone.db", &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(conn);
		
		AfxMessageBox(TEXT("���ݿ��ʧ�ܣ�"));
	}

	
	
	std::string SQLString = "INSERT INTO user(username, phone, depId, role, password) values('"+name+"', '"+phone+"', '"+depId+"', '0', '"+pswd+"')" ;
	BOOL insertUserResult = sqlOperation(const_cast<char *>(SQLString.c_str()), &conn);
	//����ʧ��
	if (insertUserResult == FALSE)
		AfxMessageBox(TEXT("���ʧ�ܣ�����绰����������Ƿ��ظ�"));
	else{
		AfxMessageBox(TEXT("��ӳɹ���"));
	}
}
