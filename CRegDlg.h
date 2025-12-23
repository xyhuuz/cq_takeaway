#pragma once
#include "afxdialogex.h"
#include "User.h"


// CRegDlg 对话框

class CRegDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegDlg)

public:
	CRegDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRegDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_regname;
	CString m_regpsw;
//	CEdit m_regphone;
	CString m_regphone;
	CString m_regid;
	BOOL m_regtype;
	afx_msg void OnBnClickedRegcancleButton();
	afx_msg void OnBnClickedRegokButton();
};
