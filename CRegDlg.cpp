// CRegDlg.cpp: 实现文件
//

#include "pch.h"
#include "wmex1.h"
#include "afxdialogex.h"
#include "CRegDlg.h"
#include <winsock.h>
#include <mysql.h>
extern User* user;

// CRegDlg 对话框

IMPLEMENT_DYNAMIC(CRegDlg, CDialogEx)

CRegDlg::CRegDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REG_DIALOG, pParent)
	, m_regtype(FALSE)
{

}

CRegDlg::~CRegDlg()
{
}

void CRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REG_NAME, m_regname);
	DDX_Text(pDX, IDC_REG_PSW, m_regpsw);
	DDX_Text(pDX, IDC_REG_PHONE, m_regphone);
	DDX_Text(pDX, IDC_REG_ID, m_regid);
	DDX_Radio(pDX, IDC_REGBUY_RADIO, m_regtype);
}


BEGIN_MESSAGE_MAP(CRegDlg, CDialogEx)
	ON_BN_CLICKED(IDC_REGCANCLE_BUTTON, &CRegDlg::OnBnClickedRegcancleButton)
	ON_BN_CLICKED(IDC_REGOK_BUTTON, &CRegDlg::OnBnClickedRegokButton)
END_MESSAGE_MAP()


// CRegDlg 消息处理程序

void CRegDlg::OnBnClickedRegcancleButton()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}

void CRegDlg::OnBnClickedRegokButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//for (int i = 0; i < 10; i++) {
	//	if (user[i].find(m_regname)) {
	//		MessageBox(TEXT("已存在该用户！"), TEXT("提示"), MB_OK);
	//		// 打开商店列表对话框
	//		//CShopListDialog shopListDialog;
	//		//shopListDialog.DoModal();
	//		return;
	//	}
	//}
	//if (m_regname.IsEmpty() || m_regpsw.IsEmpty() || m_regphone.IsEmpty() || m_regid.IsEmpty()) {
	//	MessageBox(TEXT("请填写完整信息！"), TEXT("提示"), MB_OK);
	//	return;
	//}

	//for (int i = 0; i < 10; i++) {
	//	if (user[i].username.IsEmpty()) {
	//		user[i].username = m_regname;
	//		user[i].psw = m_regpsw;
	//		user[i].phone = m_regphone;
	//		user[i].idcode = m_regid;
	//		user[i].usertype = m_regtype;
	//		MessageBox(TEXT("注册成功！"), TEXT("提示"), MB_OK);
	//		EndDialog(0);
	//		return;
	//	}
	//}


    // 连接数据库
    MYSQL m_sql;
    mysql_init(&m_sql);
    mysql_set_character_set(&m_sql, "gb2312"); // 设置字符集，防止中文乱码

    if (!mysql_real_connect(&m_sql, "localhost", "root", "123456", "wmex", 3306, NULL, 0)) {
        CString errMsg;
        errMsg.Format(TEXT("数据库连接失败: %s"), mysql_error(&m_sql));
        AfxMessageBox(errMsg);
        mysql_close(&m_sql);
        return;
    }

    // 1. 检查用户名是否已存在
    CStringA queryCheck;
    queryCheck.Format("SELECT COUNT(*) FROM users WHERE username='%s'",
        CStringA(m_regname));

    if (mysql_query(&m_sql, queryCheck)) {
        AfxMessageBox(TEXT("查询用户失败！"));
        mysql_close(&m_sql);
        return;
    }

    MYSQL_RES* result = mysql_store_result(&m_sql);
    if (!result) {
        AfxMessageBox(TEXT("获取查询结果失败！"));
        mysql_close(&m_sql);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int userCount = atoi(row[0]);
    mysql_free_result(result);

    if (userCount > 0) {
        MessageBox(TEXT("已存在该用户！"), TEXT("提示"), MB_OK);
        mysql_close(&m_sql);
        return;
    }

    // 2. 验证输入完整性
    if (m_regname.IsEmpty() || m_regpsw.IsEmpty() || m_regphone.IsEmpty() || m_regid.IsEmpty()) {
        MessageBox(TEXT("请填写完整信息！"), TEXT("提示"), MB_OK);
        mysql_close(&m_sql);
        return;
    }

    // 3. 插入新用户
    CStringA queryInsert;
    queryInsert.Format("INSERT INTO users (username, password, usertype, phone, idcode) "
        "VALUES ('%s', '%s', %d, '%s', '%s')",
        CStringA(m_regname),
        CStringA(m_regpsw),
        m_regtype,
        CStringA(m_regphone),
        CStringA(m_regid));

    if (mysql_query(&m_sql, queryInsert)) {
        CString errMsg;
        errMsg.Format(TEXT("注册失败: %s"), mysql_error(&m_sql));
        AfxMessageBox(errMsg);
    }
    else {
        if (mysql_affected_rows(&m_sql) > 0) {
            MessageBox(TEXT("注册成功！"), TEXT("提示"), MB_OK);
            EndDialog(0);
        }
        else {
            MessageBox(TEXT("注册失败，请重试！"), TEXT("错误"), MB_OK | MB_ICONERROR);
        }
    }

    // 关闭数据库连接
    mysql_close(&m_sql);
}
