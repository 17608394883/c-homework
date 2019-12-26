// Modify.cpp : 实现文件
//

#include "stdafx.h"
// CModify 对话框

IMPLEMENT_DYNAMIC(CModify, CDialogEx)

CModify::CModify(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModify::IDD, pParent)
	, m_username(_T(""))
	, m_password(_T(""))
	, m_depName(_T(""))
	, m_phone(_T(""))
{

}

CModify::~CModify()
{
}

void CModify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT5, m_password);
	DDX_Control(pDX, IDC_COMBO1, m_group);
	DDX_Text(pDX, IDC_EDIT6, m_depName);
	DDX_Text(pDX, IDC_EDIT2, m_phone);
}


BEGIN_MESSAGE_MAP(CModify, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CModify::OnBnClickedButton1)
END_MESSAGE_MAP()


// CModify 消息处理程序


BOOL CModify::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	UpdateData(TRUE);
	m_password = TEXT("不修改");
	UpdateData(FALSE);



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
	m_group.AddString(m_depName);
	for (int i = 0; i < department.size(); i++)
	{
		CString depName(department[i].depName.c_str());
		if (depName != m_depName)
		{
			m_group.AddString(depName);
		}
	
	}
	
	m_group.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
std::string getDepIdByName11(std::string departmentName){

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


void CModify::OnBnClickedButton1()
{
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
	password = m_password;

	std::string name = CT2A(username.GetString());
	std::string phone = CT2A(phoneNumber.GetString());
	std::string pswd = CT2A(password.GetString());

	std::string department = CT2A(depName.GetString());
	std::string depId = getDepIdByName11(department);

	sqlite3* conn = NULL;
	//创建或打开数据库
	int result = sqlite3_open("D:\\phone.db", &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		AfxMessageBox(TEXT("数据库打开失败！"));
	}


	
	std::string SQLString = "update user set phone = '" + phone + "' , depId='" + depId + "' where username = '" + name + "'";
	if (pswd!=string("不修改"))
	{
		SQLString = "update user set phone = '" + phone + "' , depId='" + depId + "' , password='" + pswd + "' where username = '" + name + "'";
	}

	//std::string SQLString = "INSERT INTO user(username, phone, depId, role, password) values('" + name + "', '" + phone + "', '" + depId + "', '0', '" + pswd + "')";
	
	BOOL insertUserResult = sqlOperation(const_cast<char *>(SQLString.c_str()), &conn);
	//插入失败
	if (insertUserResult == FALSE)
		AfxMessageBox(TEXT("修改失败！,请检查电话号码是否冲突"));
	else{
		AfxMessageBox(TEXT("修改成功！"));
	}
}
