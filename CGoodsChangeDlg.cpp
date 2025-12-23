// CGoodsChangeDlg.cpp: 实现文件
//

#include "pch.h"
#include "wmex1.h"
#include "afxdialogex.h"
#include "CGoodsChangeDlg.h"
#include "CSellerDlg.h"
#include "mysql.h"


// CGoodsChangeDlg 对话框

IMPLEMENT_DYNAMIC(CGoodsChangeDlg, CDialogEx)

CGoodsChangeDlg::CGoodsChangeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GOODSCHANGE_DIALOG, pParent)
	, m_name(_T(""))
	, m_goods(_T(""))
	, m_des(_T(""))
	, m_price(0)
	, m_amount(0)
{

}

CGoodsChangeDlg::~CGoodsChangeDlg()
{
}

void CGoodsChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CHANGENAME_EDIT, m_name);
	DDX_Text(pDX, IDC_CHANGEGOODS_EDIT, m_goods);
	DDX_Text(pDX, IDC_CHANGEDES_EDIT, m_des);
	//  DDX_Text(pDX, IDC_CHANGEAMOUNT_EDIT, amount);
	DDX_Text(pDX, IDC_CHANGEPRICE_EDIT, m_price);
	DDX_Text(pDX, IDC_CHANGEAMOUNT_EDIT, m_amount);
}


BEGIN_MESSAGE_MAP(CGoodsChangeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHANGEGCANCLE_BUTTON, &CGoodsChangeDlg::OnBnClickedChangegcancleButton)
	ON_BN_CLICKED(IDC_CHANGEGOK_BUTTON, &CGoodsChangeDlg::OnBnClickedChangegokButton)
END_MESSAGE_MAP()


// CGoodsChangeDlg 消息处理程序

void CGoodsChangeDlg::OnBnClickedChangegcancleButton()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDCANCEL);
	//CSellerDlg::OnInitDialog();
	CSellerDlg* pParent = (CSellerDlg*)GetParent();
	if (pParent) {
		pParent->ReinitializeDialog();
	}
	CDialogEx::OnCancel();
}

void CGoodsChangeDlg::OnBnClickedChangegokButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_goods.IsEmpty()) {
		AfxMessageBox(_T("商品名称不能为空！"));
		return;
	}

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


    //检查商品是否存在（根据belongs和name）
    CStringA queryCheck;
    queryCheck.Format("SELECT id FROM goods WHERE belongs='%s' AND name='%s'",
        CStringA(m_name), CStringA(m_goods));

    if (mysql_query(&m_sql, queryCheck)) {
        AfxMessageBox(_T("查询商品失败！"));
        mysql_close(&m_sql);
        return;
    }

    MYSQL_RES* result = mysql_store_result(&m_sql);
    if (!result) {
        AfxMessageBox(_T("获取查询结果失败！"));
        mysql_close(&m_sql);
        return;
    }

    //判断是否存在该商品
    if (mysql_num_rows(result) == 0) {
        AfxMessageBox(_T("未找到该商品，请检查商品名称和所有者！"));
        mysql_free_result(result);
        mysql_close(&m_sql);
        return;
    }

    //更新商品信息
    CStringA queryUpdate;
    queryUpdate.Format("UPDATE goods SET "
        "des='%s', amount=%d, price=%.2f "
        "WHERE belongs='%s' AND name='%s'",
        CStringA(m_des),    // 商品描述
        m_amount,          // 库存数量
        m_price,           // 商品价格
        CStringA(m_name),   // 所有者
        CStringA(m_goods)); // 商品名称

    if (mysql_query(&m_sql, queryUpdate)) {
        CString errMsg;
        errMsg.Format(_T("更新商品失败: %s"), mysql_error(&m_sql));
        AfxMessageBox(errMsg);
    }
    else {
        if (mysql_affected_rows(&m_sql) > 0) {
            AfxMessageBox(_T("商品信息更新成功！"));
        }
        else {
            AfxMessageBox(_T("商品信息未发生变化！"));
        }
    }

    //清理资源
    mysql_free_result(result);
    mysql_close(&m_sql);
}
