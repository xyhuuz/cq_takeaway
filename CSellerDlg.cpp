// CSellerDlg.cpp: 实现文件
//

#include "pch.h"
#include "wmex1.h"
#include "afxdialogex.h"
#include "CSellerDlg.h"
#include "CGoodsAddDlg.h"
#include "CGoodsChangeDlg.h"
#include "mysql.h"
#include "CSellerInfo.h"


//extern int list_row = 0;
// CSellerDlg 对话框

IMPLEMENT_DYNAMIC(CSellerDlg, CDialogEx)

CSellerDlg::CSellerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELLERMAIN_DIALOG, pParent)
{
	//list_row = 0;
}

CSellerDlg::~CSellerDlg()
{
}

void CSellerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GOODSINFO_LIST, m_goodsinfo);
}


BEGIN_MESSAGE_MAP(CSellerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SELLERCANCLE_BUTTON, &CSellerDlg::OnBnClickedSellercancleButton)
	ON_NOTIFY(NM_RCLICK, IDC_GOODSINFO_LIST, &CSellerDlg::OnNMRClickGoodsinfoList)
	ON_COMMAND(ID_FILE_1, &CSellerDlg::OnFileAdd)
	ON_COMMAND(ID_FILE_2, &CSellerDlg::OnFileChange)
	ON_COMMAND(ID_FILE_3, &CSellerDlg::OnFileDelete)
	ON_COMMAND(ID_FILE_3, &CSellerDlg::OnDeleteListItem)  // ID_MENU_DELETE 是删除菜单项的ID
	ON_BN_CLICKED(IDC_SELLERINFO_BUTTON, &CSellerDlg::OnBnClickedSellerinfoButton)
END_MESSAGE_MAP()


// CSellerDlg 消息处理程序

void CSellerDlg::OnBnClickedSellercancleButton()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}

BOOL CSellerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 连接数据库
	MYSQL m_sql;
	mysql_init(&m_sql);
	mysql_set_character_set(&m_sql, "gb2312"); // 设置字符集，防止中文乱码

	//添加数据库
	if (!mysql_real_connect(&m_sql, "localhost", "root", "123456", "wmex", 3306, NULL, 0)) {
		CString errMsg;
		errMsg.Format(_T("数据库连接失败: %s"), mysql_error(&m_sql));
		AfxMessageBox(errMsg);
		mysql_close(&m_sql);
		return FALSE;
	}


	m_goodsinfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);	// 整行选择、网格线
	m_goodsinfo.InsertColumn(0, _T("商品"), LVCFMT_LEFT, 100);	// 插入第2列的列名
	m_goodsinfo.InsertColumn(1, _T("描述"), LVCFMT_LEFT, 100);	// 插入第3列的列名
	m_goodsinfo.InsertColumn(2, _T("库存"), LVCFMT_LEFT, 100);	// 插入第4列的列名
	m_goodsinfo.InsertColumn(3, _T("单价"), LVCFMT_LEFT, 100);	// 插入第5列的列名

	//设置行高
	CImageList   m_l;
	m_l.Create(1, 100, TRUE | ILC_COLOR32, 1, 0);   //设置表格的高度 
	m_goodsinfo.SetImageList(&m_l, LVSIL_SMALL);
	//设置字体
		CFont m_font;
	m_font.CreatePointFont(140, TEXT("新宋体")); //设置标题栏的高度及字体
	m_goodsinfo.SetFont(&m_font);

	m_goodsinfo.SetColumnWidth(0, 300); //设置列宽
	m_goodsinfo.SetColumnWidth(1, 572);
	m_goodsinfo.SetColumnWidth(2, 100);
	m_goodsinfo.SetColumnWidth(3, 100);

	//这是一个简单的内存示例
	//for (int i = 0; i < 1; i++)
	//{ //不断获取下一组结果
	//	
	//	m_goodsinfo.InsertItem(i, _T(""));						// 插入行
	//	m_goodsinfo.SetItemText(i, 0, TEXT("俞俊杰"));				// 设置第2列(学号)
	//	m_goodsinfo.SetItemText(i, 1, TEXT("俞俊杰,好吃"));	// 设置第3列(图片)
	//	//m_goodsinfo.SetItem(i, 1, LVIF_IMAGE | LVIF_TEXT, 0, 1, 0, 0, 0);
	//	m_goodsinfo.SetItemText(i, 2, TEXT("100"));	
	//	//m_goodsinfo.InsertColumn(0, TEXT(""), LVCFMT_LEFT);// 设置第4列(性别)
	//	m_goodsinfo.SetItemText(i, 3, TEXT("12"));			// 设置第5列(语文)
	//	list_row++;  //list_row为list Ctrl表格中显示的数据的行数，便于后期操作，在头文件中定义
	//}

	CStringA query;
	query.Format("SELECT name, des, amount, price FROM goods WHERE belongs='%s'",
		CStringA(m_SellerName)); // m_SellerName为当前登录用户名

	if (mysql_query(&m_sql, query)) {
		AfxMessageBox(_T("查询商品数据失败！"));
		mysql_close(&m_sql);
		return FALSE;
	}

	MYSQL_RES* result = mysql_store_result(&m_sql);
	if (!result) {
		AfxMessageBox(_T("获取查询结果失败！"));
		mysql_close(&m_sql);
		return FALSE;
	}

	// 遍历结果集并填充列表控件
	int row = 0;
	MYSQL_ROW dbRow;
	while ((dbRow = mysql_fetch_row(result))) {
		m_goodsinfo.InsertItem(row, _T("")); // 插入空行

		// 设置各列数据（注意NULL值处理）
		m_goodsinfo.SetItemText(row, 0, dbRow[0] ? CString(dbRow[0]) : _T(""));
		m_goodsinfo.SetItemText(row, 1, dbRow[1] ? CString(dbRow[1]) : _T(""));
		m_goodsinfo.SetItemText(row, 2, dbRow[2] ? CString(dbRow[2]) : _T("0"));
		m_goodsinfo.SetItemText(row, 3, dbRow[3] ? CString(dbRow[3]) : _T("0.00"));

		row++;
	}

	mysql_free_result(result);
	mysql_close(&m_sql);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSellerDlg::ReinitializeDialog()
{
	m_goodsinfo.DeleteAllItems(); //清空所有行（保留列标题）

	//再把列标题也删干净
	for (int i = m_goodsinfo.GetHeaderCtrl()->GetItemCount() - 1; i >= 0; i--) {
		m_goodsinfo.DeleteColumn(i);
	}
	//重新初始化列标题，从数据库读入
	this->OnInitDialog();
}

void CSellerDlg::OnNMRClickGoodsinfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//// TODO: 在此添加控件通知处理程序代码
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//if (pNMListView->iItem != -1) {

	//	CPoint pt;
	//	GetCursorPos(&pt);
	//	CMenu menu;
	//	menu.LoadMenu(IDR_MENU1);
	//	CMenu* popmenu;
	//	popmenu = menu.GetSubMenu(0);
	//	popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	//}

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CPoint pt;
	GetCursorPos(&pt);

	// 转换坐标并获取点击的行
	CPoint ptList = pt;
	m_goodsinfo.ScreenToClient(&ptList);
	LVHITTESTINFO hitTestInfo = { 0 };
	hitTestInfo.pt = ptList;
	int nItem = m_goodsinfo.HitTest(&hitTestInfo);

	if (nItem != -1) {
		// 清除所有选中，再选中当前行
		for (int i = 0; i < m_goodsinfo.GetItemCount(); i++) {
			m_goodsinfo.SetItemState(i, 0, LVIS_SELECTED);
		}
		m_goodsinfo.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);

		// 显示菜单
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CMenu* pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}

	*pResult = 0;
}

void CSellerDlg::OnFileAdd()
{
	// TODO: 在此添加命令处理程序代码
	CGoodsAddDlg dlg;
	dlg.DoModal();
}

void CSellerDlg::OnFileChange()
{
	// TODO: 在此添加命令处理程序代码
	CGoodsChangeDlg dlg;
	dlg.DoModal();
}

void CSellerDlg::OnFileDelete()
{
	// TODO: 在此添加命令处理程序代码
	CListCtrl* pList = &m_goodsinfo;  // 假设 m_goodsinfo 是关联的 CListCtrl 变量

	// 获取当前选中的行
	POSITION pos = pList->GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox(_T("请先选中要删除的行！"));
		return;
	}

	int nItem = pList->GetNextSelectedItem(pos);

	// 从数据库删除
	if (DeleteFromDatabase(nItem)) {  // 自定义函数，见下文
		pList->DeleteItem(nItem);     // 从列表控件中删除行
		AfxMessageBox(_T("删除成功！"));
	}
	else {
		AfxMessageBox(_T("删除失败！"));
	}
}

void CSellerDlg::OnBnClickedSellerinfoButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CSellerInfo dlg;
	dlg.m_user = m_SellerName;
	dlg.m_id = m_id;
	dlg.m_phone = m_phone;
	dlg.m_psw = m_psw;
	dlg.DoModal();
	m_SellerName = dlg.m_user;
	m_id = dlg.m_id;
	m_phone = dlg.m_phone;
	m_psw = dlg.m_psw;
}


void CSellerDlg::OnDeleteListItem() {
	
}


bool CSellerDlg::DeleteFromDatabase(int nItem) {
	// 1. 获取列表数据
	CString strGoodsName = m_goodsinfo.GetItemText(nItem, 0);
	CString strBelongs = m_SellerName;
	CStringA strGoodsNameUTF8 = CT2A(strGoodsName);
	CStringA strBelongsUTF8 = CT2A(strBelongs);

	// 验证数据有效性
	if (strGoodsName.IsEmpty() || strBelongs.IsEmpty()) {
		AfxMessageBox(_T("获取的商品信息不完整！"));
		return false;
	}

	// 2. 连接数据库
	MYSQL m_sql;
	mysql_init(&m_sql);
	if (!mysql_real_connect(&m_sql, "localhost", "root", "123456", "wmex", 3306, NULL, 0)) {
		CString errMsg;
		errMsg.Format(_T("数据库连接失败: %s"), mysql_error(&m_sql));
		AfxMessageBox(errMsg);
		return false;
	}

	// 必须设置字符集
	mysql_set_character_set(&m_sql, "gb2312");

	// 验证数据是否存在
	/*if (!VerifyDataMatch(&m_sql, strGoodsName, strBelongs)) {
		AfxMessageBox(_T("要删除的记录不存在或数据不匹配"));
		mysql_close(&m_sql);
		return false;
	}*/
	CStringA query;
	
	query.Format("DELETE FROM goods WHERE name='%s' AND belongs='%s'",
		strGoodsNameUTF8, strBelongsUTF8);
	mysql_commit(&m_sql);
	TRACE(_T("执行SQL: %s\n"), CString(query));

	int result = mysql_query(&m_sql, query);
	if (result != 0) {
		CString errMsg;
		errMsg.Format(_T("删除失败: %s"), mysql_error(&m_sql));
		AfxMessageBox(errMsg);
		mysql_close(&m_sql);
		return false;
	}

	// 检查实际影响的行数
	my_ulonglong affected_rows = mysql_affected_rows(&m_sql);
	TRACE(_T("影响的行数: %d\n"), affected_rows);

	mysql_close(&m_sql);
	return affected_rows > 0;
	
}