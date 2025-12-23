// CGoodsAddDlg.cpp: 实现文件
//

#include "pch.h"
#include "wmex1.h"
#include "afxdialogex.h"
#include "CGoodsAddDlg.h"
#include "mysql.h"
#include "CSellerDlg.h"


// CGoodsAddDlg 对话框

IMPLEMENT_DYNAMIC(CGoodsAddDlg, CDialogEx)

CGoodsAddDlg::CGoodsAddDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GOODSADD_DIALOG, pParent)
	, m_name(_T(""))
	, m_goods(_T(""))
	, m_des(_T(""))
	, m_amount(0)
	, m_price(0)
{

}

CGoodsAddDlg::~CGoodsAddDlg()
{
}

void CGoodsAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDNAME_EDIT, m_name);
	DDX_Text(pDX, IDC_ADDGOODS_EDIT, m_goods);
	DDX_Text(pDX, IDC_ADDDES_EDIT, m_des);
	DDX_Text(pDX, IDC_ADDAMOUNT_EDIT, m_amount);
	DDX_Text(pDX, IDC_ADDPRICE_EDIT, m_price);
}


BEGIN_MESSAGE_MAP(CGoodsAddDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADDGCANCLE_BUTTON, &CGoodsAddDlg::OnBnClickedAddgcancleButton)
	ON_BN_CLICKED(IDC_ADDGOK_BUTTON, &CGoodsAddDlg::OnBnClickedAddgokButton)
END_MESSAGE_MAP()


// CGoodsAddDlg 消息处理程序

void CGoodsAddDlg::OnBnClickedAddgcancleButton()
{
	//TODO: 在此添加控件通知处理程序代码
	EndDialog(IDCANCEL);
	//CSellerDlg::OnInitDialog();
	CSellerDlg* pParent = (CSellerDlg*)GetParent();
	if (pParent) {
		pParent->ReinitializeDialog();
	}
	CDialogEx::OnCancel();
}

void CGoodsAddDlg::OnBnClickedAddgokButton()
{
	//TODO: 在此添加控件通知处理程序代码
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


	CStringA query;
	query.Format("INSERT INTO goods (name, belongs, des, amount, price) "
		"VALUES ('%s', '%s', '%s', %d, %.2f)",
		CStringA(m_goods),   // 商品名称
		CStringA(m_name),    // 所属用户
		CStringA(m_des),     // 商品描述
		m_amount,            // 库存数量
		m_price);           // 商品价格

	//执行SQL
	if (mysql_query(&m_sql, query)) {
		CString errMsg;
		errMsg.Format(_T("添加商品失败: %s"), mysql_error(&m_sql));
		AfxMessageBox(errMsg);
	}
	else {
		AfxMessageBox(_T("商品添加成功！"));

		 //清空输入框
		m_goods.Empty();
		m_des.Empty();
		m_amount = 0;
		m_price = 0.0;
		UpdateData(FALSE); // 将变量值更新到控件
	}

	 //5. 关闭数据库连接
	mysql_close(&m_sql);
}
