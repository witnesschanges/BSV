// ChildDlg2.cpp : 实现文件
//

#include "stdafx.h"
#include "BSV.h"
#include "BSVDlg.h"
#include "ChildDlg2.h"
#include "afxdialogex.h"


// CChildDlg2 对话框

IMPLEMENT_DYNAMIC(CChildDlg2, CDialogEx)

CChildDlg2::CChildDlg2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlg2::IDD, pParent)
{

}

CChildDlg2::~CChildDlg2()
{
}

void CChildDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlg2, CDialogEx)
	ON_BN_CLICKED(IDC_CircleDetect, &CChildDlg2::OnBnClickedCircledetect)
	ON_BN_CLICKED(IDC_OpenCamera, &CChildDlg2::OnBnClickedOpencamera)
	ON_BN_CLICKED(IDC_CapVideo, &CChildDlg2::OnBnClickedCapvideo)
	ON_WM_CREATE()
//	ON_BN_CLICKED(IDC_StopCamera, &CChildDlg2::OnBnClickedStopcamera)
END_MESSAGE_MAP()

BOOL CChildDlg2::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CChildDlg2::OnBnClickedCircledetect()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	if (pBSVDlg->m_LeftShowDIBBits == NULL)
	{
		MessageBox("无图像数据！","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		pBSVDlg->SetTimer(1, 300, NULL);//设置定时器1
		pBSVDlg->m_TimerFlag = true;

		GetDlgItem(IDC_CircleDetect)->EnableWindow(false);
		GetDlgItem(IDC_StopDetect)->EnableWindow(true);
	}
}


void CChildDlg2::OnBnClickedOpencamera()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->OnBnClickedOpencamera();
	//启动左相机	
	//MVSTATUS_CODES hr;
	//if (pBSVDlg->m_pLCamera == NULL)	//启动相机
	//{
	//	hr = MVOpenCamByIndex(0, &pBSVDlg->m_pLCamera );		
	//	if (hr != MVST_SUCCESS)
	//	{
	//		MessageBox("未找到相机，请确认设备连接和IP设置！","BSV",MB_ICONWARNING);
	//		return;
	//	}
	//	else if (hr == MVST_SUCCESS)
	//	{
	//		GetDlgItem(IDC_OpenCamera)->SetWindowText("关闭相机");
	//		//GetDlgItem(IDC_CapVideo)->EnableWindow(true);
	//		//GetDlgItem(IDC_SetCamera)->EnableWindow(true);
	//		//GetDlgItem(IDC_StopCamera)->EnableWindow(false);
	//	}
	//}
	//else	//关闭相机
	//{
	//	hr = MVStopGrab(pBSVDlg->m_pLCamera);		 
	//	hr = MVCloseCam(pBSVDlg->m_pLCamera);	
	//	pBSVDlg->m_pLCamera = NULL;

	//	//GetDlgItem(IDC_OpenCamera)->SetWindowText("启动相机");
	//	GetDlgItem(IDC_CapVideo)->EnableWindow(false);
	//	GetDlgItem(IDC_SetCamera)->EnableWindow(false);
	//}
}


void CChildDlg2::OnBnClickedCapvideo()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->ShowLeftPic();
	CDialogEx::OnCancel();
}


int CChildDlg2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


//void CChildDlg2::OnBnClickedStopcamera()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
