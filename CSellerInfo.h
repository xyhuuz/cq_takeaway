#pragma once
#include "afxdialogex.h"


// CSellerInfo 对话框

class CSellerInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSellerInfo)

public:
	/*CString m_SellerName;
	CString m_psw;
	CString m_phone;
	CString m_id;*/
	CSellerInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSellerInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELLERINFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOkButton();
	CString m_user;
	CString m_psw;
	CString m_id;
	CString m_phone;
	afx_msg void OnBnClickedChangeButton();
};
