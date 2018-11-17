#pragma once


// CChildDialog1 对话框

class CChildDialog1 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDialog1)

public:
	CChildDialog1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDialog1();

// 对话框数据
	enum { IDD = IDD_ChildDlg1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();

public:
	int row_corner_num;			//标定图像内角点（不和边缘接触的角点）行数
	int column_corner_num;		//标定图像内角点（不和边缘接触的角点）列数
	int grid_width;				//实际测量得到的标定板上每个棋盘格的长
	int grid_height;			//实际测量得到的标定板上每个棋盘格的宽
	int PicNum;					//需要标定图片的数量
	
	afx_msg void OnBnClickedButton9();
};
