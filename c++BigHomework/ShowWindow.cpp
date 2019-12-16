// ShowWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "c++BigHomework.h"
#include "ShowWindow.h"
#include "afxdialogex.h"
#include "resource.h"
#include "Add.h"
#include "Modify.h"

// CShowWindow 对话框

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


// CShowWindow 消息处理程序


void CShowWindow::OnBnClickedBtnAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	CAdd *addDlg = new CAdd;

	addDlg->Create(IDD_ADD_DIALOG);
	addDlg->ShowWindow(SW_SHOW);
}


void CShowWindow::OnBnClickedBtnModify()
{
	// TODO:  在此添加控件通知处理程序代码
	CModify *modifyDlg = new CModify;

	modifyDlg->Create(IDD_MODIFY_DIALOG);
	modifyDlg->ShowWindow(SW_SHOW);

}










BOOL CShowWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	startSet();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CShowWindow::show(CString username, CString phoneNumber, CString depName,int type){
	
	
	sqlite3* conn = NULL;
	//创建或打开数据库
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		MessageBox(TEXT("创建或者打开数据库失败"));
	}

	std::string SQLString;
	//按条件查找
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

	//设置正文
	for (int i = 0; i < user.size(); i++) {
		j = 0;

		CString name(user[i].username.c_str());
		CString phone(user[i].phone.c_str());
		CString depName(searchDepNameById(user[i].depId).c_str());

		CString role;
		if (user[i].role == std::string("1"))
		{
			role = "管理员";
		}
		else
		{
			role = "普通员工";
		}

		m_list.InsertItem(i, name);
		//设置这一行其他列的值
		m_list.SetItemText(i, ++j, phone);
		m_list.SetItemText(i, ++j, depName);
		m_list.SetItemText(i, ++j, role);

	}
}

void CShowWindow::startSet(){

	/*************************设置表头***********************************************/
	
	CString str[] = { TEXT("姓名"), TEXT("联系方式"), TEXT("部门"), TEXT("角色"), };
	for (int i = 0; i < 4; i++) {
		//设置表头  参数1 索引   2内容  3对齐方式   4列宽度
		m_list.InsertColumn(i, str[i], LVCFMT_LEFT, 100);
	}
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	/***********************设置部门下拉框*********************************************************/
	sqlite3* conn = NULL;
	std::string SQLString = "SELECT * FROM department";
	vector<Department> department;

	//创建或打开数据库
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		MessageBox(TEXT("创建或者打开数据库失败"));
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

	//创建或打开数据库
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		MessageBox(TEXT("创建或者打开数据库失败"));
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
////创建失败
//if (createTableResult == FALSE)
//	MessageBox(TEXT("创建表失败"));

void CShowWindow::OnBnClickedButton1()
{
	//清空
	m_list.DeleteAllItems();

	UpdateData(TRUE);

	CString depName;
	CString username;
	CString phoneNumber;

	//获取下拉框内容
	int index = m_group.GetCurSel();
	m_group.GetLBText(index, depName);

	//获取用户名  电话号码
	username = m_username;
	phoneNumber = m_phone;


	show(username, phoneNumber, depName, 1);
}


void CShowWindow::OnBnClickedButton2()
{
	//清空
	m_list.DeleteAllItems();
	CString str("");
	show(str, str, str, 0);
}
