#pragma once
#include "afxdialogex.h"


// CBuyerDlg 对话框

class CBuyerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBuyerDlg)


public:
	CBuyerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBuyerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUYERMAIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedQuitButton();
	double m_sum;
	CString m_BuyerName;
	CString m_id;
	CString m_phone;
	CString m_psw;
	CComboBox m_combo;
	CListCtrl m_list;
	CMap<int, int, int, int> m_itemIdMap;  // 用于存储列表项ID与商品ID的映射
	virtual BOOL OnInitDialog();

	// 加载商品分类到combo控件
	void LoadCategories();
	// 根据分类加载商品列表
	void LoadGoodsByCategory(const CString& category);
	// 更新总价显示
	void UpdateSumDisplay();
	afx_msg void OnBnClickedFilterButton();
	afx_msg void OnBnClickedAddButton();
	afx_msg void OnBnClickedPayButton();
	afx_msg void OnBnClickedBuyerinfoButton();
};
