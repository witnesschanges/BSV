// ChildDlg1.cpp : 实现文件
//

#include "stdafx.h"
#include "BSV.h"
#include "ChildDlg1.h"
#include "afxdialogex.h"


// CChildDlg1 对话框

IMPLEMENT_DYNAMIC(CChildDlg1, CDialogEx)

CChildDlg1::CChildDlg1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlg1::IDD, pParent)
{

}

CChildDlg1::~CChildDlg1()
{
}

void CChildDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlg1, CDialogEx)
//	ON_BN_CLICKED(IDC_BUTTON12, &CChildDlg1::OnBnClickedButton12)
END_MESSAGE_MAP()


// CChildDlg1 消息处理程序


//void CChildDlg1::OnBnClickedButton12()
//{
//	// TODO: 在此添加控件通知处理程序代码B
//	AfxMessageBox(_T("TRY!"));
//}
