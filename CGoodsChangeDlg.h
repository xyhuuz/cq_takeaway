#pragma once
#include "afxdialogex.h"


// CGoodsChangeDlg 对话框

class CGoodsChangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGoodsChangeDlg)

public:
	CGoodsChangeDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGoodsChangeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOODSCHANGE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChangegcancleButton();
	CString m_name;
	CString m_goods;
	CString m_des;
	double m_price;
	int m_amount;
	afx_msg void OnBnClickedChangegokButton();
};
