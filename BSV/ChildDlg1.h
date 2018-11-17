#pragma once
#include "resource.h"
// CChildDlg1 对话框

class CChildDlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlg1)

public:
	CChildDlg1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlg1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedButton12();
};
