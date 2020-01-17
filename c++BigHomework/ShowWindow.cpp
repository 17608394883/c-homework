// ShowWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <shlobj.h>
bool getAllUser(vector<User> &user)
{
	sqlite3* conn = NULL;
	//����������ݿ�
	int result = sqlite3_open(G2U("D:\\phone.db"), &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(conn);
		return 0;
	}
	std::string SQLString;
	//����
	SQLString = "SELECT username, phone, depId, role FROM user";

	BOOL res = QueryUser(conn, SQLString, user);
	return 1;
}
// CShowWindow �Ի���

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
	modifyDlg->m_phone = modiyPhone;
	modifyDlg->m_username = modiyName;
	modifyDlg->m_depName = modiyDepName;

	modifyDlg->Create(IDD_MODIFY_DIALOG);
	modifyDlg->ShowWindow(SW_SHOW);

}










BOOL CShowWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	startSet();

	if (userRole==0)
	{
		GetDlgItem(IDC_STATIC_ADMIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_MODIFY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_REMOVE)->ShowWindow(SW_HIDE);
	}

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


void CShowWindow::OnNMClickPhone(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//��ȡѡ�еĵ�һ��λ��
	POSITION p = m_list.GetFirstSelectedItemPosition();
	if (p == NULL)
	{
		TRACE("û���б�ѡ��!");
	}
	//��ȡ��ѡȡ��λ�õ��±�(��0��ʼ��)
	int index = m_list.GetNextSelectedItem(p);
	//���ѡ�е�����
	//�õ���index��.��0�е�����(�±��0��ʼ)
	CString FirstColumn = m_list.GetItemText(index, 0);
	//�õ���index��,��1�е�����
	modiyName = m_list.GetItemText(index, 0);
	modiyPhone = m_list.GetItemText(index, 1);//0 modiyName   1 modiyDepId     
	modiyDepName = m_list.GetItemText(index, 2);
	

	*pResult = 0;
}


void CShowWindow::OnBnClickedBtnRemove()
{
	std::string name = CT2A(modiyName.GetString());

	sqlite3* conn = NULL;
	//����������ݿ�
	int result = sqlite3_open("D:\\phone.db", &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
	if (result != SQLITE_OK) {
		sqlite3_close(conn);

		AfxMessageBox(TEXT("���ݿ��ʧ�ܣ�"));
	}



	std::string SQLString = "delete from user  where username = '" + name + "'";
	

	BOOL insertUserResult = sqlOperation(const_cast<char *>(SQLString.c_str()), &conn);
	//ʧ��
	if (insertUserResult == FALSE)
		AfxMessageBox(TEXT("ɾ��ʧ�ܣ�"));
	else{
		AfxMessageBox(TEXT("ɾ���ɹ���"));
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
	bi.lpszTitle = _T("&Ntilde;&iexcl;&Ocirc;&ntilde;&sup2;&yen;&sup3;&ouml;����&Acirc;��&frac34;&para;");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString FileName = path();
	FileName.Append(L"employer.xls");
	CString sDriver = L"MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����	

	CString FILEEXCEL;
	FILEEXCEL.Format(L"DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, FileName, FileName);


	CDatabase date;
	CString ssql;
	if (date.OpenEx(FILEEXCEL, CDatabase::noOdbcDialog))
	{
		date.ExecuteSQL(L"CREATE TABLE ��¼��(���� text, �绰 text, ���� text, �û����� text);");

		//��������    // ����������ݿ������ʹ�����Ʒ���
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
				role = "����Ա";
			}
			else
			{
				role = "��ͨԱ��";
			}
			ssql.Format(L"INSERT INTO ��¼��(����, �绰,����, �û�����) VALUES('%s','%s','%s','%s');", name, phone, depName, role);
			date.ExecuteSQL(ssql);
		}
	}
	else
	{
		AfxMessageBox(L"û��Excel����");
	}

	AfxMessageBox(L"������ɣ�");
}


void CShowWindow::OnBnClickedBtnRemove3()
{
	CString FileName;//����·��
	CFileDialog opendlg(TRUE, _T("*"), _T("*.xls"), OFN_OVERWRITEPROMPT, _T("�����ļ�(*.*;)|*.*||"), NULL);
	if (opendlg.DoModal() == IDOK)
	{
		FileName = opendlg.GetPathName();
	}
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		CString sDriver = L"MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����	
		CString FILEEXCEL;
		FILEEXCEL.Format(L"DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, FileName, FileName);
		CDatabase date;
		if (date.OpenEx(FILEEXCEL, CDatabase::noOdbcDialog))
		{
			vector<User> users;
			CRecordset *recordset = new CRecordset(&date);
			recordset->Open(CRecordset::snapshot, L"select ����, �绰, ���� from ��¼��", 0); 
			while (!recordset->IsEOF()) //�����¼��Ϊ�ջ���û�й������һ����¼
			{
				CString username;
				CString phone;
				CString department;
				CString role;
				recordset->GetFieldValue(L"����", username);
				recordset->GetFieldValue(L"�绰", phone);
				recordset->GetFieldValue(L"����", department);
				USES_CONVERSION;
				string depId = getDepIdByName(W2CA(department));
				User user(W2CA(username), W2CA(phone), atoi(depId.c_str()));
				users.push_back(user);
				//����
				sqlite3* conn = NULL;
				//����������ݿ�
				int result = sqlite3_open("D:\\phone.db", &conn); //���·���������ģ����Բ���ת�룬�������������ȫ��ת��
				if (result != SQLITE_OK) {
					sqlite3_close(conn);

					AfxMessageBox(TEXT("���ݿ��ʧ�ܣ�"));
				}
				string name = W2CA(username);
				string ph = W2CA(phone);
				string pswd = "123456";
				std::string SQLString = "INSERT INTO user(username, phone, depId, role, password) values('" + name + "', '" + ph.substr(0,ph.length()-2) + "', '" + depId + "', '0', '" + pswd + "')";
				BOOL insertUserResult = sqlOperation(const_cast<char *>(SQLString.c_str()), &conn);
				//����ʧ��
				if (insertUserResult == FALSE)
					AfxMessageBox(TEXT("����ʧ�ܣ�����绰����������Ƿ��ظ�"));

				recordset->MoveNext(); //�ƶ�����һ��
			}

		}
		else
		{
			AfxMessageBox(L"û��Excel����");
		}
	
		AfxMessageBox(L"������ɣ�");
}