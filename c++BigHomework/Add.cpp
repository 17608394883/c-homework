// Add.cpp : 实现文件
//

#include "stdafx.h"
#include "c++BigHomework.h"
#include "Add.h"
#include "afxdialogex.h"


// CAdd 对话框

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


// CAdd 消息处理程序


BOOL CAdd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	sqlite3* conn = NULL;
	std::string SQLString = "SELECT * FROM department";
	vector<Department> department;

	//创建或打开数据库
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		AfxMessageBox(TEXT("创建或者打开数据库失败"));
		
	}

	QueryDep(conn, SQLString, department);

	for (int i = 0; i < department.size(); i++)
	{
		CString depName(department[i].depName.c_str());
		m_group.AddString(depName);
	}

	m_group.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

std::string getDepIdByName(std::string departmentName){

	sqlite3* conn = NULL;
	std::string SQLString = "SELECT * FROM department";
	vector<Department> deps;

	//创建或打开数据库
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		AfxMessageBox(TEXT("创建或者打开数据库失败"));
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
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CString depName;
	CString username;
	CString phoneNumber;
	CString password;
	//获取下拉框内容
	int index = m_group.GetCurSel();
	m_group.GetLBText(index, depName);

	//获取用户名  电话号码
	username = m_username;
	phoneNumber = m_phone;
	password = m_pswd;

	std::string name = CT2A(username.GetString());
	std::string phone = CT2A(phoneNumber.GetString());
	std::string pswd = CT2A(password.GetString());

	std::string department = CT2A(depName.GetString());
	std::string depId =getDepIdByName(department);

	sqlite3* conn = NULL;
	//创建或打开数据库
	int result = sqlite3_open("D:\\phone.db", &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);
		
		AfxMessageBox(TEXT("数据库打开失败！"));
	}

	
	
	std::string SQLString = "INSERT INTO user(username, phone, depId, role, password) values('"+name+"', '"+phone+"', '"+depId+"', '0', '"+pswd+"')" ;
	BOOL insertUserResult = sqlOperation(const_cast<char *>(SQLString.c_str()), &conn);
	//插入失败
	if (insertUserResult == FALSE)
		AfxMessageBox(TEXT("添加失败！请检查电话号码和姓名是否重复"));
	else{
		AfxMessageBox(TEXT("添加成功！"));
	}
}
