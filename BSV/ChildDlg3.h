#pragma once


// CChildDlg3 对话框

class CChildDlg3 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlg3)

public:
	CChildDlg3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlg3();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton13();
};
