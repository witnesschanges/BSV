// Cali_ParaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BSV.h"
#include "Cali_ParaDlg.h"
#include "afxdialogex.h"
#include "BSVDlg.h"

// CCali_ParaDlg �Ի���

IMPLEMENT_DYNAMIC(CCali_ParaDlg, CDialogEx)

CCali_ParaDlg::CCali_ParaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCali_ParaDlg::IDD, pParent)
	, m_PicNum(0)
{

	m_row_corner_num = 0;
	m_column_corner_num = 0;
	m_grid_width = 0;
	m_grid_height = 0;
}

CCali_ParaDlg::~CCali_ParaDlg()
{
}

void CCali_ParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_1EDIT, m_row_corner_num);
	DDX_Text(pDX, IDC_2EDIT, m_column_corner_num);
	DDX_Text(pDX, IDC_3EDIT, m_grid_width);
	DDX_Text(pDX, IDC_4EDIT, m_grid_height);
	DDX_Text(pDX, IDC_0EDIT, m_PicNum);
}


BEGIN_MESSAGE_MAP(CCali_ParaDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CaliStart, &CCali_ParaDlg::OnBnClickedCalistart)
END_MESSAGE_MAP()


// CCali_ParaDlg ��Ϣ�������


void CCali_ParaDlg::OnBnClickedCalistart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  
    ((CBSVDlg*)GetParent())->m_Calibration.RowCornerNum = m_row_corner_num;
	((CBSVDlg*)GetParent())->m_Calibration.ColumCornerNum = m_column_corner_num;  
	((CBSVDlg*)GetParent())->m_Calibration.GridWidth = m_grid_width;  
	((CBSVDlg*)GetParent())->m_Calibration.GridHeight = m_grid_height;  
	OnOK();
}
