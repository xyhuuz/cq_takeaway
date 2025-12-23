// CSellerInfo.cpp: 实现文件
//

#include "pch.h"
#include "wmex1.h"
#include "afxdialogex.h"
#include "CSellerInfo.h"
#include "mysql.h"


// CSellerInfo 对话框

IMPLEMENT_DYNAMIC(CSellerInfo, CDialogEx)

CSellerInfo::CSellerInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELLERINFO_DIALOG, pParent)
	, m_user(_T(""))
	, m_psw(_T(""))
	, m_id(_T(""))
	, m_phone(_T(""))
{

}

CSellerInfo::~CSellerInfo()
{
}

void CSellerInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME_EDIT, m_user);
	DDX_Text(pDX, IDC_PSW_EDIT, m_psw);
	DDX_Text(pDX, IDC_ID_EDIT, m_id);
	DDX_Text(pDX, IDC_PHONE_EDIT, m_phone);
}


BEGIN_MESSAGE_MAP(CSellerInfo, CDialogEx)
	ON_BN_CLICKED(IDC_OK_BUTTON, &CSellerInfo::OnBnClickedOkButton)
	ON_BN_CLICKED(IDC_CHANGE_BUTTON, &CSellerInfo::OnBnClickedChangeButton)
END_MESSAGE_MAP()


// CSellerInfo 消息处理程序

void CSellerInfo::OnBnClickedOkButton()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDOK);
}

void CSellerInfo::OnBnClickedChangeButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	//连接数据库
	MYSQL m_sql;
	mysql_init(&m_sql);
	mysql_set_character_set(&m_sql, "gb2312");

	if (!mysql_real_connect(&m_sql, "localhost", "root", "123456", "wmex", 3306, NULL, 0)) {
		CString errMsg;
		errMsg.Format(_T("数据库连接失败: %s"), mysql_error(&m_sql));
		AfxMessageBox(errMsg);
		mysql_close(&m_sql);
		return;
	}

	// 构建SQL更新语句
	CStringA query;
	query.Format("UPDATE users SET idcode='%s', password='%s', phone='%s' WHERE username='%s'",
		CStringA(m_id).GetString(),
		CStringA(m_psw).GetString(),
		CStringA(m_phone).GetString(),
		CStringA(m_user).GetString());

	// 执行SQL语句
	if (mysql_query(&m_sql, query) != 0) {
		CString errMsg;
		errMsg.Format(_T("更新数据失败: %s"), mysql_error(&m_sql));
		AfxMessageBox(errMsg);
	}
	else {
		if (mysql_affected_rows(&m_sql) > 0) {
			AfxMessageBox(_T("用户信息更新成功"));
			EndDialog(IDOK);
		}
		else {
			AfxMessageBox(_T("您未做任何修改！"));
		}
	}
	//m_user=;
	//m_psw.Empty();
	//m_id.Empty();
	//m_phone.Empty();
	// 关闭数据库连接
	mysql_close(&m_sql);
	//UpdateData(TRUE);
}
