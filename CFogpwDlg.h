#pragma once
#include "afxdialogex.h"
#include "User.h"


// CFogpwDlg 对话框

class CFogpwDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFogpwDlg)

public:
	CFogpwDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFogpwDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOGPSW_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedFogcancleButton();
	CString m_fogname;
	CString m_fogid;
	CString m_fogpwd;
	CString m_fogapwd;
	afx_msg void OnBnClickedFogokButton();
//	afx_msg void OnNMRClickGoodsinfoList(NMHDR* pNMHDR, LRESULT* pResult);
};

extern User* user;