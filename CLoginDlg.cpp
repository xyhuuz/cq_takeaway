// CLoginDlg.cpp: 实现文件
//
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

#include "pch.h"
#include "framework.h"


#include "wmex1.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"
#include "CRegDlg.h"
#include "CFogpwDlg.h"
#include "CSellerDlg.h"
#include "CBuyerDlg.h"
#include <winsock.h>
#include <mysql.h>
extern User* user;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



class CNailongDlg : public CDialogEx
{
public:
	CNailongDlg();
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAILONG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_nlpic;
};

CNailongDlg::CNailongDlg() : CDialogEx(IDD_NAILONG_DIALOG)
{
	
}

void CNailongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_nlpic);
}

BOOL CNailongDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
// TODO:  在此添加额外的初始化
	HICON hIcon = (HICON)::LoadImage(
		NULL,
		_T("res\\nl.ico"),  // ICO文件路径
		IMAGE_ICON,
		200, 200,  // 目标尺寸
		LR_LOADFROMFILE | LR_DEFAULTCOLOR
	);
	if (hIcon) {
		m_nlpic.SetIcon(hIcon);  // 设置图标
	}
	return TRUE;
}


BEGIN_MESSAGE_MAP(CNailongDlg, CDialogEx)
END_MESSAGE_MAP()



// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN_DIALOG, pParent)
	, m_type(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	/*user[0].username = TEXT("us1");
	user[0].psw = TEXT("123456");
	user[0].idcode = TEXT("371122200510086313");
	user[0].phone = TEXT("6536962");
	user[0].usertype = 0;*/


	if (m_image.Load(_T("res\\ico.bmp")) != S_OK) {
		AfxMessageBox(_T("图片加载失败！"));
		return;
	}

	m_font.CreateFont(
		40,                        // 字体高度（单位：逻辑像素）
		0,                         // 宽度（0表示自动）
		0,                         // 旋转角度（0表示水平）
		0,                         // 方向角度
		FW_BOLD,                   // 粗细（FW_NORMAL为正常，FW_BOLD为加粗）
		FALSE,                     // 是否斜体
		FALSE,                     // 是否下划线
		FALSE,                     // 是否删除线
		DEFAULT_CHARSET,          // 字符集
		OUT_DEFAULT_PRECIS,        // 输出精度
		CLIP_DEFAULT_PRECIS,       // 剪裁精度
		DEFAULT_QUALITY,           // 质量
		DEFAULT_PITCH | FF_SWISS,  // 字体系列
		_T("Arial")                // 字体名称
	);
}

CLoginDlg::~CLoginDlg()
{
	//delete[] user;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UESRNAME, m_username);
	DDX_Text(pDX, IDC_PSW, m_psw);
	DDX_Radio(pDX, IDC_BUYER_RADIO, m_type);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUYER_RADIO, &CLoginDlg::OnBnClickedBuyerRadio)
	ON_BN_CLICKED(IDC_SELLER_RADIO, &CLoginDlg::OnBnClickedSellerRadio)
	ON_BN_CLICKED(IDC_LOGIN_BUTTON, &CLoginDlg::OnBnClickedLoginButton)
	ON_BN_CLICKED(IDC_REG_BUTTON, &CLoginDlg::OnBnClickedRegButton)
	ON_BN_CLICKED(IDC_FOGPSW_BUTTON, &CLoginDlg::OnBnClickedFogpswButton)
	ON_WM_PAINT()
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_TITLE, &CLoginDlg::OnStnClickedTitle)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

void CLoginDlg::OnBnClickedBuyerRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_type = FALSE;
}

void CLoginDlg::OnBnClickedSellerRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_type = TRUE;
}

bool CLoginDlg::GetUserInfo(const CStringA& username, CStringA& phone, CStringA& idcode)
{
	MYSQL mysql;
	mysql_init(&mysql);
	mysql_set_character_set(&mysql, "gb2312");

	if (!mysql_real_connect(&mysql, "localhost", "root", "123456", "wmex", 3306, NULL, 0)) {
		return false;
	}

	CStringA query;
	query.Format("SELECT phone, idcode FROM users WHERE username='%s'", username);

	if (mysql_query(&mysql, query)) {
		mysql_close(&mysql);
		return false;
	}

	MYSQL_RES* result = mysql_store_result(&mysql);
	if (!result) {
		mysql_close(&mysql);
		return false;
	}

	MYSQL_ROW row = mysql_fetch_row(result);
	if (!row) {
		mysql_free_result(result);
		mysql_close(&mysql);
		return false;
	}

	phone = row[0];
	idcode = row[1];

	mysql_free_result(result);
	mysql_close(&mysql);
	return true;
}








void CLoginDlg::OnBnClickedLoginButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取用户输入
	// 这里添加登录逻辑
	// 
	// 1.0 内存版本
	//bool flag = false;
	//for (int i = 0; i < 10; i++) {
	//	if (user[i].find(m_username, m_psw, m_type)) {
	//		MessageBox(TEXT("登录成功！"), TEXT("提示"), MB_OK);
	//		// 打开商店列表对话框
	//		//CShopListDialog shopListDialog;
	//		//shopListDialog.DoModal();
	//		flag = true;
	//		break;
	//	}	
	//}
	//if (!flag) {
	//	MessageBox(TEXT("用户名或密码错误！"), TEXT("错误"), MB_OK | MB_ICONERROR);
	//}




//	// 连接数据库
//	MYSQL m_sql;
//	mysql_init(&m_sql);
//	mysql_set_character_set(&m_sql, "gb2312"); // 设置字符集，防止中文乱码
//
//	if (!mysql_real_connect(&m_sql, "localhost", "root", "123456", "wmex", 3306, NULL, 0)) {
//		CString errMsg;
//		errMsg.Format(_T("数据库连接失败: %s"), mysql_error(&m_sql));
//		AfxMessageBox(errMsg);
//		mysql_close(&m_sql);
//		return;
//	}
//
//	// 1. 查询用户信息
//	CStringA query;
//	query.Format("SELECT password, usertype, phone, idcode FROM users WHERE username='%s'",
//		CStringA(m_username));
//
//	if (mysql_query(&m_sql, query)) {
//		AfxMessageBox(_T("查询用户失败！"));
//		mysql_close(&m_sql);
//		return;
//	}
//
//	MYSQL_RES* result = mysql_store_result(&m_sql);
//	if (!result) {
//		AfxMessageBox(_T("获取查询结果失败！"));
//		mysql_close(&m_sql);
//		return;
//	}
//
//	MYSQL_ROW row = mysql_fetch_row(result);
//	if (!row) {
//		MessageBox(_T("用户名或密码错误！"), _T("错误"), MB_OK | MB_ICONERROR);
//		mysql_free_result(result);
//		mysql_close(&m_sql);
//		return;
//	}
//
//	// 2. 验证密码和用户类型
//	CStringA dbPassword = row[0];
//	int dbUserType = atoi(row[1]);
//	CStringA dbphone = row[2];
//	CStringA dbidcode = row[3];
//	mysql_free_result(result);
//
//	if (dbPassword == CStringA(m_psw) && dbUserType == m_type) {
//		MessageBox(_T("登录成功！"), _T("提示"), MB_OK);
//		// 可以在这里保存登录状态或跳转到主界面
//		if (m_type == 0) {
//			// 买家
//			CBuyerDlg buyerDlg;
//			buyerDlg.m_BuyerName = m_username;
//			buyerDlg.m_psw = m_psw;
//			buyerDlg.m_phone = dbphone;
//			buyerDlg.m_id = dbidcode;
//			buyerDlg.DoModal();
//		}
//		else {
//			// 卖家
//			CSellerDlg sellerDlg;
//			sellerDlg.m_SellerName = m_username;
//			sellerDlg.m_psw = m_psw;
//			sellerDlg.m_phone = dbphone;
//			sellerDlg.m_id = dbidcode;
//			sellerDlg.DoModal();
//		}
//	}
//	else {
//		MessageBox(_T("用户名或密码错误！"), _T("错误"), MB_OK | MB_ICONERROR);
//	}
//
//	// 关闭数据库连接
//	mysql_close(&m_sql);
//}
//

// 初始化Winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		AfxMessageBox(_T("WSAStartup failed"));
		return;
	}

// 创建socket
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		AfxMessageBox(_T("Socket creation failed"));
		WSACleanup();
		return;
	}
//#define SERVER_IP "127.0.0.1"
//#define SERVER_PORT 8888
//#define BUFFER_SIZE 1024

// 设置服务器地址
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

// 连接服务器
	if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		AfxMessageBox(_T("服务器连接失败！"));
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

// 准备发送数据: username|password|usertype
	CStringA sendData;
	sendData.Format("%s|%s|%d",
		CStringA(m_username),
		CStringA(m_psw),
		m_type);

// 发送数据到服务器
	if (send(clientSocket, sendData, sendData.GetLength(), 0) == SOCKET_ERROR) {
		AfxMessageBox(_T("发送失败！"));
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

// 接收服务器响应
	char buffer[BUFFER_SIZE] = { 0 };
	int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
	if (bytesReceived <= 0) {
		AfxMessageBox(_T("服务器无响应！"));
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

// 解析服务器响应: status|message
	CStringA response(buffer);
	int pipePos = response.Find('|');
	if (pipePos == -1) {
		AfxMessageBox(_T("Invalid server response"));
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

	int status = atoi(response.Left(pipePos));
	CStringA message = response.Mid(pipePos + 1);

// 关闭socket
	closesocket(clientSocket);
	WSACleanup();

	if (status == 1) {
		// 登录成功
		MessageBox(_T("登录成功！"), _T("提示"), MB_OK);

		// 获取用户额外信息
		CStringA phone, idcode;
		if (GetUserInfo(CStringA(m_username), phone, idcode)) {
			if (m_type == 0) {
				// 买家
				CBuyerDlg buyerDlg;
				buyerDlg.m_BuyerName = m_username;
				buyerDlg.m_psw = m_psw;
				buyerDlg.m_phone = phone;
				buyerDlg.m_id = idcode;
				buyerDlg.DoModal();
			}
			else {
				// 卖家
				CSellerDlg sellerDlg;
				sellerDlg.m_SellerName = m_username;
				sellerDlg.m_psw = m_psw;
				sellerDlg.m_phone = phone;
				sellerDlg.m_id = idcode;
				sellerDlg.DoModal();
			}
		}
		else {
			AfxMessageBox(_T("获取用户信息失败"));
		}
	}
	else {
		// 登录失败
		MessageBox(CString(message), _T("错误"), MB_OK | MB_ICONERROR);
	}


}
void CLoginDlg::OnBnClickedRegButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(TEXT("注册界面开发中..."), TEXT("提示"), MB_OK);
	CRegDlg regDlg;
	regDlg.DoModal();
}

void CLoginDlg::OnBnClickedFogpswButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(TEXT("注册界面开发中..."), TEXT("提示"), MB_OK);
	CFogpwDlg fogpwDlg;
	fogpwDlg.DoModal();
}

void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetDlgItem(IDC_LOGOPIC)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CDC* pDC = GetDC();
	m_image.StretchBlt(pDC->m_hDC, rect.left, rect.top, rect.Width(), rect.Height(),
		0, 0, m_image.GetWidth(), m_image.GetHeight(), SRCCOPY);
	ReleaseDC(pDC);
	GetDlgItem(IDC_TITLE)->SetFont(&m_font);
}

void CLoginDlg::OnStnClickedTitle()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	ShowWindow(SW_SHOW);
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLoginDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_APP_ABOUT, _T("关于(&A)"));

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x, point.y, this);
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == 'A' || pMsg->wParam == 'a') {
			m_nACount++;  // 增加计数器

			// 如果连续按了5次
			if (m_nACount >= 5) {
				AfxMessageBox(TEXT("检测到连续按a键5次！"));  // 弹出对话框
				CNailongDlg nailongDlg;
				nailongDlg.DoModal();
				m_nACount = 0;  // 重置计数器
				return TRUE;
			}
		}
		else {
			// 如果按下的是其他键，重置计数器
			m_nACount = 0;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);  // 其他消息正常处理
}