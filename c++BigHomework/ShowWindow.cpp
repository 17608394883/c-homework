// ShowWindow.cpp : 实现文件
//

#include "stdafx.h"
#include <shlobj.h>
bool getAllUser(vector<User> &user)
{
	sqlite3* conn = NULL;
	//创建或打开数据库
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);
		return 0;
	}
	std::string SQLString;
	//查找
	SQLString = "SELECT username, phone, depId, role FROM user";

	BOOL res = QueryUser(conn, SQLString, user);
	return 1;
}
// CShowWindow 对话框

IMPLEMENT_DYNAMIC(CShowWindow, CDialogEx)

CString modiyPhone("");
CString modiyName("");
CString modiyDepName("");

CShowWindow::CShowWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowWindow::IDD, pParent)
	, m_username(_T(""))
	, m_phone(_T(""))
	, userRole(0)
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
	ON_NOTIFY(NM_CLICK, LIST_PHONE, &CShowWindow::OnNMClickPhone)
	ON_BN_CLICKED(IDC_BTN_REMOVE, &CShowWindow::OnBnClickedBtnRemove)
	ON_BN_CLICKED(IDC_BTN_REMOVE2, &CShowWindow::OnBnClickedBtnRemove2)
	ON_BN_CLICKED(IDC_BTN_REMOVE3, &CShowWindow::OnBnClickedBtnRemove3)
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
	modifyDlg->m_phone = modiyPhone;
	modifyDlg->m_username = modiyName;
	modifyDlg->m_depName = modiyDepName;

	modifyDlg->Create(IDD_MODIFY_DIALOG);
	modifyDlg->ShowWindow(SW_SHOW);

}










BOOL CShowWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	startSet();

	if (userRole==0)
	{
		GetDlgItem(IDC_STATIC_ADMIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_MODIFY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_REMOVE)->ShowWindow(SW_HIDE);
	}

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


void CShowWindow::OnNMClickPhone(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//获取选中的的一个位置
	POSITION p = m_list.GetFirstSelectedItemPosition();
	if (p == NULL)
	{
		TRACE("没有行被选中!");
	}
	//获取刚选取的位置的下标(从0开始的)
	int index = m_list.GetNextSelectedItem(p);
	//获得选中的内容
	//得到第index行.第0列的内容(下标从0开始)
	CString FirstColumn = m_list.GetItemText(index, 0);
	//得到第index行,第1列的内容
	modiyName = m_list.GetItemText(index, 0);
	modiyPhone = m_list.GetItemText(index, 1);//0 modiyName   1 modiyDepId     
	modiyDepName = m_list.GetItemText(index, 2);
	

	*pResult = 0;
}


void CShowWindow::OnBnClickedBtnRemove()
{
	std::string name = CT2A(modiyName.GetString());

	sqlite3* conn = NULL;
	//创建或打开数据库
	int result = sqlite3_open("D:\\phone.db", &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		AfxMessageBox(TEXT("数据库打开失败！"));
	}



	std::string SQLString = "delete from user  where username = '" + name + "'";
	

	BOOL insertUserResult = sqlOperation(const_cast<char *>(SQLString.c_str()), &conn);
	//失败
	if (insertUserResult == FALSE)
		AfxMessageBox(TEXT("删除失败！"));
	else{
		AfxMessageBox(TEXT("删除成功！"));
	}
}

CString path()
{
	CString strFilePath;
	TCHAR pszBuffer[_MAX_PATH];
	BROWSEINFO bi;  LPITEMIDLIST pidl;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = pszBuffer;
	bi.lpszTitle = _T("&Ntilde;&iexcl;&Ocirc;&ntilde;&sup2;&yen;&sup3;&ouml;±í&Acirc;·&frac34;&para;");
	bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		if (SHGetPathFromIDList(pidl, pszBuffer))
		{
			CString strTemp(pszBuffer);   strFilePath = strTemp;
			if (strFilePath.GetLength() <= 1)
			{
			}
			else if (strFilePath.Right(1) != _T("\\"))
				strFilePath += _T("\\");
		}
	}
	return strFilePath;
}
void CShowWindow::OnBnClickedBtnRemove2()
{
	// TODO:  在此添加控件通知处理程序代码
	CString FileName = path();
	FileName.Append(L"employer.xls");
	CString sDriver = L"MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel安装驱动	

	CString FILEEXCEL;
	FILEEXCEL.Format(L"DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, FileName, FileName);


	CDatabase date;
	CString ssql;
	if (date.OpenEx(FILEEXCEL, CDatabase::noOdbcDialog))
	{
		date.ExecuteSQL(L"CREATE TABLE 记录表(姓名 text, 电话 text, 部门 text, 用户类型 text);");

		//插入数据    // 如需遍历数据库插入请使用类似方法
		vector<User> user;
		bool ret = getAllUser(user);
		int j;
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
			ssql.Format(L"INSERT INTO 记录表(姓名, 电话,部门, 用户类型) VALUES('%s','%s','%s','%s');", name, phone, depName, role);
			date.ExecuteSQL(ssql);
		}
	}
	else
	{
		AfxMessageBox(L"没有Excel驱动");
	}

	AfxMessageBox(L"导出完成！");
}


void CShowWindow::OnBnClickedBtnRemove3()
{
	CString FileName;//保存路径
	CFileDialog opendlg(TRUE, _T("*"), _T("*.xls"), OFN_OVERWRITEPROMPT, _T("所有文件(*.*;)|*.*||"), NULL);
	if (opendlg.DoModal() == IDOK)
	{
		FileName = opendlg.GetPathName();
	}
		// TODO:  在此添加控件通知处理程序代码
		CString sDriver = L"MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel安装驱动	
		CString FILEEXCEL;
		FILEEXCEL.Format(L"DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, FileName, FileName);
		CDatabase date;
		if (date.OpenEx(FILEEXCEL, CDatabase::noOdbcDialog))
		{
			vector<User> users;
			CRecordset *recordset = new CRecordset(&date);
			recordset->Open(CRecordset::snapshot, L"select 姓名, 电话, 部门 from 记录表", 0); 
			while (!recordset->IsEOF()) //如果记录不为空或者没有滚到最后一个记录
			{
				CString username;
				CString phone;
				CString department;
				CString role;
				recordset->GetFieldValue(L"姓名", username);
				recordset->GetFieldValue(L"电话", phone);
				recordset->GetFieldValue(L"部门", department);
				USES_CONVERSION;
				string depId = getDepIdByName(W2CA(department));
				User user(W2CA(username), W2CA(phone), atoi(depId.c_str()));
				users.push_back(user);
				//插入
				sqlite3* conn = NULL;
				//创建或打开数据库
				int result = sqlite3_open("D:\\phone.db", &conn); //如果路径不含中文，可以不用转码，保险起见，建议全部转码
				if (result != SQLITE_OK) {
					sqlite3_close(conn);

					AfxMessageBox(TEXT("数据库打开失败！"));
				}
				string name = W2CA(username);
				string ph = W2CA(phone);
				string pswd = "123456";
				std::string SQLString = "INSERT INTO user(username, phone, depId, role, password) values('" + name + "', '" + ph.substr(0,ph.length()-2) + "', '" + depId + "', '0', '" + pswd + "')";
				BOOL insertUserResult = sqlOperation(const_cast<char *>(SQLString.c_str()), &conn);
				//插入失败
				if (insertUserResult == FALSE)
					AfxMessageBox(TEXT("导入失败！请检查电话号码和姓名是否重复"));

				recordset->MoveNext(); //移动到下一个
			}

		}
		else
		{
			AfxMessageBox(L"没有Excel驱动");
		}
	
		AfxMessageBox(L"导入完成！");
}