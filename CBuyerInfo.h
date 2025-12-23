#pragma once
#include "afxdialogex.h"


// CBuyerInfo 对话框

class CBuyerInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CBuyerInfo)

public:
	CBuyerInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBuyerInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUYERINFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_user;
	CString m_psw;
	CString m_id;
	CString m_phone;
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnBnClickedChangeButton();
};
