#pragma once
#include "afxdialogex.h"


// CGoodsAddDlg 对话框

class CGoodsAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGoodsAddDlg)

public:
	CGoodsAddDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGoodsAddDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOODSADD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_goods;
	CString m_des;
	int m_amount;
	double m_price;
	afx_msg void OnBnClickedAddgcancleButton();
	afx_msg void OnBnClickedAddgokButton();
};
