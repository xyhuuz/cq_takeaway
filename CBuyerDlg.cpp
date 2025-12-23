// CBuyerDlg.cpp: 实现文件
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <mysql.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include "pch.h"
#include "wmex1.h"
#include "afxdialogex.h"
#include "CBuyerDlg.h"
#include "CBuyerInfo.h"
#include "mysql.h"
#include <string>


// CBuyerDlg 对话框

IMPLEMENT_DYNAMIC(CBuyerDlg, CDialogEx)

CBuyerDlg::CBuyerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BUYERMAIN_DIALOG, pParent)
	, m_sum(0)
{

}

CBuyerDlg::~CBuyerDlg()
{
}

void CBuyerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SUM, m_sum);
	DDX_Control(pDX, IDC_FILTER_COMBO, m_combo);
	DDX_Control(pDX, IDC_GOODS_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CBuyerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_QUIT_BUTTON, &CBuyerDlg::OnBnClickedQuitButton)
    ON_BN_CLICKED(IDC_FILTER_BUTTON, &CBuyerDlg::OnBnClickedFilterButton)
    ON_BN_CLICKED(IDC_ADD_BUTTON, &CBuyerDlg::OnBnClickedAddButton)
    ON_BN_CLICKED(IDC_PAY_BUTTON, &CBuyerDlg::OnBnClickedPayButton)
    ON_BN_CLICKED(IDC_BUYERINFO_BUTTON, &CBuyerDlg::OnBnClickedBuyerinfoButton)
END_MESSAGE_MAP()


// CBuyerDlg 消息处理程序

void CBuyerDlg::OnBnClickedQuitButton()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDOK);
}

BOOL CBuyerDlg::OnInitDialog()
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


	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("商品名称"), LVCFMT_LEFT, 120);
	m_list.InsertColumn(1, _T("价格"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(2, _T("库存"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(3, _T("描述"), LVCFMT_LEFT, 200);

    //设置行高
    CImageList   m_l;
    m_l.Create(1, 100, TRUE | ILC_COLOR32, 1, 0);   //设置表格的高度 
    m_list.SetImageList(&m_l, LVSIL_SMALL);
    //设置字体
    CFont m_font;
    m_font.CreatePointFont(140, TEXT("新宋体")); //设置标题栏的高度及字体
    m_list.SetFont(&m_font);

    m_list.SetColumnWidth(0, 300); //设置列宽
    m_list.SetColumnWidth(1, 100);
    m_list.SetColumnWidth(2, 100);
    m_list.SetColumnWidth(3, 565);

    // 加载商品分类
    LoadCategories();

    // 初始化总价
    m_sum = 0.0;
    UpdateSumDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBuyerDlg::LoadCategories()
{
    m_combo.ResetContent();
    m_combo.AddString(_T("所有分类"));  // 添加"所有分类"选项

    MYSQL m_sql;
    mysql_init(&m_sql);
    mysql_set_character_set(&m_sql, "gb2312"); // 设置字符集，防止中文乱码

    //添加数据库
    if (!mysql_real_connect(&m_sql, "localhost", "root", "123456", "wmex", 3306, NULL, 0)) {
        CString errMsg;
        errMsg.Format(_T("数据库连接失败: %s"), mysql_error(&m_sql));
        AfxMessageBox(errMsg);
        mysql_close(&m_sql);
        return;
    }

    CStringA query("SELECT DISTINCT belongs FROM goods WHERE belongs IS NOT NULL");
    if (mysql_real_query(&m_sql, query, query.GetLength()) == 0)
    {
        MYSQL_RES* result = mysql_store_result(&m_sql);
        if (result)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result)))
            {
                CString category(row[0]);
                m_combo.AddString(category);
            }
            mysql_free_result(result);
        }
    }
    m_combo.SetCurSel(0);  // 默认选中第一个选项
}

void CBuyerDlg::LoadGoodsByCategory(const CString& category)
{
    m_list.DeleteAllItems();
    m_itemIdMap.RemoveAll();


    MYSQL m_sql;
    mysql_init(&m_sql);
    mysql_set_character_set(&m_sql, "gb2312"); // 设置字符集，防止中文乱码

    //添加数据库
    if (!mysql_real_connect(&m_sql, "localhost", "root", "123456", "wmex", 3306, NULL, 0)) {
        CString errMsg;
        errMsg.Format(_T("数据库连接失败: %s"), mysql_error(&m_sql));
        AfxMessageBox(errMsg);
        mysql_close(&m_sql);
        return;
    }

    CStringA query;
    if (category == _T("所有分类"))
    {
        query = "SELECT id, name, price, amount, des FROM goods";
    }
    else
    {
        CStringA categoryA(category);
        query.Format("SELECT id, name, price, amount, des FROM goods WHERE belongs='%s'", categoryA.GetString());
    }

    if (mysql_real_query(&m_sql, query, query.GetLength()) == 0)
    {
        MYSQL_RES* result = mysql_store_result(&m_sql);
        if (result)
        {
            MYSQL_ROW row;
            int nItem = 0;
            while ((row = mysql_fetch_row(result)))
            {
                int id = atoi(row[0]);
                CString name(row[1]);
                CString price(row[2]);
                CString amount(row[3]);
                CString des(row[4] ? row[4] : "");

                // 添加商品到列表
                int nIndex = m_list.InsertItem(nItem, name);
                m_list.SetItemText(nIndex, 1, price);
                m_list.SetItemText(nIndex, 2, amount);
                m_list.SetItemText(nIndex, 3, des);

                // 保存列表项ID与商品ID的映射
                m_itemIdMap.SetAt(nIndex, id);

                nItem++;
            }
            mysql_free_result(result);
        }
    }
}

void CBuyerDlg::UpdateSumDisplay()
{
    CString sumStr;
    sumStr.Format(_T("%.2f"), m_sum);
    SetDlgItemText(IDC_SUM, sumStr);
}

// 过滤按钮点击事件
void CBuyerDlg::OnBnClickedFilterButton()
{
    CString selectedCategory;
    m_combo.GetLBText(m_combo.GetCurSel(), selectedCategory);
    LoadGoodsByCategory(selectedCategory);
}

#include <ws2tcpip.h>

void CBuyerDlg::OnBnClickedAddButton()
{
    POSITION pos = m_list.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(_T("请先选择一件商品"));
        return;
    }

    int nItem = m_list.GetNextSelectedItem(pos);
    int goodsId;
    if (!m_itemIdMap.Lookup(nItem, goodsId))
    {
        AfxMessageBox(_T("获取商品ID失败"));
        return;
    }

    // 创建套接字
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        AfxMessageBox(_T("WSAStartup failed"));
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        AfxMessageBox(_T("Socket creation failed"));
        WSACleanup();
        return;
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);


    if (InetPton(AF_INET, _T("127.0.0.1"), &serv_addr.sin_addr) <= 0) {
        AfxMessageBox(_T("Invalid address/ Address not supported"));
        closesocket(sock);
        WSACleanup();
        return;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        AfxMessageBox(_T("连接服务器失败"));
        closesocket(sock);
        WSACleanup();
        return;
    }

    // purchase|username|goodsId
    CStringA request;
    request.Format("purchase|%s|%d", m_BuyerName, goodsId); 
    send(sock, request.GetString(), request.GetLength(), 0);

    // 接收响应
    char buffer[1024] = { 0 };
    int valread = recv(sock, buffer, sizeof(buffer), 0);
    closesocket(sock);
    WSACleanup();

    if (valread <= 0) {
        AfxMessageBox(_T("与服务器通信失败"));
        return;
    }

    std::string response(buffer, valread);
    size_t delimiter = response.find('|');
    if (delimiter == std::string::npos) {
        AfxMessageBox(_T("无效的服务器响应"));
        return;
    }

    std::string status = response.substr(0, delimiter);
    std::string message = response.substr(delimiter + 1);

    if (status == "1") {
        // 购买成功，更新本地UI
        CString priceStr = m_list.GetItemText(nItem, 1);
        double price = _ttof(priceStr);
        m_sum += price;
        UpdateSumDisplay();

        // 更新库存显示
        CString amountStr = m_list.GetItemText(nItem, 2);
        int amount = _ttoi(amountStr) - 1;
        if (amount < 0) amount = 0;
        amountStr.Format(_T("%d"), amount);
        m_list.SetItemText(nItem, 2, amountStr);

        AfxMessageBox(_T("商品已添加到购物车"));
    }
    else {
        AfxMessageBox(CString(message.c_str()));
    }
}




void CBuyerDlg::OnBnClickedPayButton()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_sum <= 0) {
        AfxMessageBox(_T("请先添加商品"));
        return;
    }
    else{
        AfxMessageBox(_T("支付成功"));
    }
}

void CBuyerDlg::OnBnClickedBuyerinfoButton()
{
    // TODO: 在此添加控件通知处理程序代码
    CBuyerInfo dlg;
    dlg.m_user = m_BuyerName;
    dlg.m_id = m_id;
    dlg.m_phone = m_phone;
    dlg.m_psw = m_psw;
    dlg.DoModal();
    m_BuyerName = dlg.m_user;
    m_id = dlg.m_id;
    m_phone = dlg.m_phone;
    m_psw = dlg.m_psw;
}
