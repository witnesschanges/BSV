#pragma once


// CCali_ParaDlg 对话框

class CCali_ParaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCali_ParaDlg)

public:
	CCali_ParaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCali_ParaDlg();

// 对话框数据
	enum { IDD = IDD_CaliPara_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCalistart();
	int m_row_corner_num;
	int m_column_corner_num;
	int m_grid_width;
	int m_grid_height;
	int m_PicNum;
};
