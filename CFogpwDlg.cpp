// CFogpwDlg.cpp: 实现文件
//

#include "pch.h"
#include "wmex1.h"
#include "afxdialogex.h"
#include "CFogpwDlg.h"
#include <winsock.h>
#include <mysql.h>

// CFogpwDlg 对话框

IMPLEMENT_DYNAMIC(CFogpwDlg, CDialogEx)

CFogpwDlg::CFogpwDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FOGPSW_DIALOG, pParent)
{

}

CFogpwDlg::~CFogpwDlg()
{
}

void CFogpwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FOGNAME_EDIT, m_fogname);
	DDX_Text(pDX, IDC_FOGID_EDIT, m_fogid);
	DDX_Text(pDX, IDC_FOGPSW_EDIT, m_fogpwd);
	DDX_Text(pDX, IDC_FOGAPSW_EDIT, m_fogapwd);
}


BEGIN_MESSAGE_MAP(CFogpwDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FOGCANCLE_BUTTON, &CFogpwDlg::OnBnClickedFogcancleButton)
	ON_BN_CLICKED(IDC_FOGOK_BUTTON, &CFogpwDlg::OnBnClickedFogokButton)
    //ON_NOTIFY(NM_RCLICK, IDC_GOODSINFO_LIST, &CFogpwDlg::OnNMRClickGoodsinfoList)
END_MESSAGE_MAP()


// CFogpwDlg 消息处理程序

void CFogpwDlg::OnBnClickedFogcancleButton()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}

void CFogpwDlg::OnBnClickedFogokButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

    // 连接数据库
    MYSQL m_sql;  // 声明数据库对象
    mysql_init(&m_sql); // 初始化数据库对象
    mysql_set_character_set(&m_sql, "gb2312"); // 设置字符集，防止中文乱码

    if (!mysql_real_connect(&m_sql, "localhost", "root", "123456", "wmex", 3306, NULL, 0)) {
        CString errMsg;
        errMsg.Format(TEXT("数据库连接失败: %s"), mysql_error(&m_sql));
        AfxMessageBox(errMsg);
        mysql_close(&m_sql);
        return;
    }

    // 1. 检查用户是否存在
    CStringA queryCheckUser;
    queryCheckUser.Format("SELECT COUNT(*) FROM users WHERE username='%s'",
        CStringA(m_fogname));

    if (mysql_query(&m_sql, queryCheckUser)) {
        AfxMessageBox(_T("查询用户失败！"));
        mysql_close(&m_sql);
        return;
    }

    MYSQL_RES* result = mysql_store_result(&m_sql);
    if (!result) {
        AfxMessageBox(_T("获取查询结果失败！"));
        mysql_close(&m_sql);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int userCount = atoi(row[0]);
    mysql_free_result(result);

    if (userCount == 0) {
        MessageBox(_T("不存在该用户！"), _T("提示"), MB_OK);
        mysql_close(&m_sql);
        return;
    }

    // 2. 验证输入完整性
    if (m_fogname.IsEmpty() || m_fogpwd.IsEmpty() || m_fogapwd.IsEmpty() || m_fogid.IsEmpty()) {
        MessageBox(_T("请填写完整信息！"), _T("提示"), MB_OK);
        mysql_close(&m_sql);
        return;
    }

    // 3. 验证身份证号是否正确
    CStringA queryCheckId;
    queryCheckId.Format("SELECT idcode FROM users WHERE username='%s'",
        CStringA(m_fogname));

    if (mysql_query(&m_sql, queryCheckId)) {
        AfxMessageBox(_T("验证身份证号失败！"));
        mysql_close(&m_sql);
        return;
    }

    result = mysql_store_result(&m_sql);
    if (!result) {
        AfxMessageBox(_T("获取查询结果失败！"));
        mysql_close(&m_sql);
        return;
    }

    row = mysql_fetch_row(result);
    CStringA dbIdCode = row ? row[0] : "";
    mysql_free_result(result);

    if (dbIdCode != CStringA(m_fogid)) {
        MessageBox(_T("身份证号不正确！"), _T("提示"), MB_OK);
        mysql_close(&m_sql);
        return;
    }

    // 4. 更新密码
    CStringA queryUpdate;
    queryUpdate.Format("UPDATE users SET password='%s' WHERE username='%s'",
        CStringA(m_fogpwd), CStringA(m_fogname));

    if (mysql_query(&m_sql, queryUpdate)) {
        CString errMsg;
        errMsg.Format(_T("密码更新失败: %s"), mysql_error(&m_sql));
        AfxMessageBox(errMsg);
    }
    else {
        if (mysql_affected_rows(&m_sql) > 0) {
            MessageBox(_T("密码修改完成"), _T("提示"), MB_OK);
        }
        else {
            MessageBox(_T("密码修改失败，请重试！"), _T("错误"), MB_OK | MB_ICONERROR);
        }
    }

    // 关闭数据库连接
    mysql_close(&m_sql);
}

