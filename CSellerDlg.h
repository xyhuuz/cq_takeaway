#pragma once
#include "afxdialogex.h"


// CSellerDlg 对话框

class CSellerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSellerDlg)

public:
	CSellerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSellerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELLERMAIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	CString m_SellerName;
	CString m_psw;
	CString m_phone;
	CString m_id;
	CListCtrl m_goodsinfo;
	afx_msg void OnBnClickedSellercancleButton();
	virtual BOOL OnInitDialog();
	void ReinitializeDialog();
	bool DeleteFromDatabase(int nItem);
	afx_msg void OnNMRClickGoodsinfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileAdd();
	afx_msg void OnFileChange();
	afx_msg void OnFileDelete();
	afx_msg void OnBnClickedSellerinfoButton();
	afx_msg void OnDeleteListItem();  // 删除菜单项的处理函数
};

//extern int list_row;