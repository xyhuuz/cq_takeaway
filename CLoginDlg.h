#pragma once
#include "afxdialogex.h"
#include "User.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024


// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_DIALOG };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_username;
	CString m_psw;
	CImage m_image;
	CFont m_font;
	int m_nACount = 0;
//	BOOL m_type;

	BOOL m_type;
	afx_msg void OnBnClickedBuyerRadio();
	afx_msg void OnBnClickedSellerRadio();
	bool GetUserInfo(const CStringA& username, CStringA& phone, CStringA& idcode);
	afx_msg void OnBnClickedLoginButton();
	afx_msg void OnBnClickedRegButton();
	afx_msg void OnBnClickedFogpswButton();
	afx_msg void OnPaint();
	afx_msg void OnStnClickedTitle();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
