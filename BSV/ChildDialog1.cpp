// ChildDialog1.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "BSV.h"
#include "ChildDialog1.h"
#include "afxdialogex.h"
#include "BSVDlg.h"
#include "Cali_ParaDlg.h"
#include "global.h"
#include "Calibration.h"
#include "CircleCalibration.h"

// CChildDialog1 对话框

IMPLEMENT_DYNAMIC(CChildDialog1, CDialogEx)

CChildDialog1::CChildDialog1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDialog1::IDD, pParent)
{
	/*项目路径初始化*/
	ProjectPath.Format(_T("E:\\课题学习\\1-双棱镜\\5-双目视觉软件\\1-软件开发\\BSV\\BSV\\"));
}

CChildDialog1::~CChildDialog1()
{
}

void CChildDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDialog1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CChildDialog1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CChildDialog1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CChildDialog1::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CChildDialog1::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CChildDialog1::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CChildDialog1::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CChildDialog1::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CChildDialog1::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CChildDialog1::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON21, &CChildDialog1::OnBnClickedButton21)
END_MESSAGE_MAP()


// CChildDialog1 消息处理程序


void CChildDialog1::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->ShowLeftPic();
}


void CChildDialog1::OnBnClickedButton2()	
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->StopLeftCamera();
}


void CChildDialog1::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->OnBnClickedSavepic();
}


void CChildDialog1::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->OnBnClickedCornerdetect();
}


void CChildDialog1::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->OnBnClickedCircledetect();
}


void CChildDialog1::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->OnBnClickedStopdetect();
}


void CChildDialog1::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	CString a, b ;
	a.Format(_T("%s\\LCamera_calibdata.txt"),ProjectPath);
	b.Format(_T("%s\\LCamera_caliberation_result.txt"),ProjectPath);
	string A((LPCTSTR)a);
	string B((LPCTSTR)b);

	row_corner_num = 0;
	column_corner_num = 0;
	grid_width = 0;
	grid_height = 0;
	PicNum=0;
	CCali_ParaDlg dlg;
	dlg.DoModal();
	if(row_corner_num==0||column_corner_num==0||grid_width==0||grid_height==0||PicNum==0)
	{
		AfxMessageBox(_T("请将参数填写完整！"));
	}
	else
	{
		WriteTxt(A,PicNum);
		string LImgPath="Ori_LImage\\chess";
		Calibration(A,B,row_corner_num,column_corner_num,grid_width,grid_height,LImgPath,1);
	}
}


void CChildDialog1::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->OnBnClickedSetcamera();
}

//利用圆形标定板进行标定
void CChildDialog1::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	CString a, b ;
	a.Format(_T("%s\\LCamera_calibdata.txt"),ProjectPath);
	b.Format(_T("%s\\LCamera_caliberation_result.txt"),ProjectPath);
	string A((LPCTSTR)a);
	string B((LPCTSTR)b);

	row_corner_num = 0;
	column_corner_num = 0;
	grid_width = 0;
	grid_height = 0;
	PicNum=0;
	CCali_ParaDlg dlg;
	dlg.DoModal();
	if(row_corner_num==0||column_corner_num==0||grid_width==0||grid_height==0||PicNum==0)
	{
		AfxMessageBox(_T("请将参数填写完整！"));
	}
	else
	{
		WriteTxt(A,PicNum);
		string LImgPath="Ori_LImage\\chess";
		CircleCalibration(A,B,row_corner_num,column_corner_num,grid_width,grid_height,LImgPath,1);
	}
}


void CChildDialog1::OnBnClickedButton21()
{
	// TODO: 在此添加控件通知处理程序代码
	CString a, b, c, d, SCaliFile;
	a.Format(_T("%s\\LCamera_calibdata.txt"),ProjectPath);
	b.Format(_T("%s\\LCamera_caliberation_result.txt"),ProjectPath);
	string A((LPCTSTR)a);
	string B((LPCTSTR)b);
	c.Format(_T("%s\\RCamera_calibdata.txt"),ProjectPath);
	d.Format(_T("%s\\RCamera_caliberation_result.txt"),ProjectPath);
	string C((LPCTSTR)c);
	string D((LPCTSTR)d);
	SCaliFile.Format(_T("%s\\Stereo_caliberation_result.txt"),ProjectPath);

	double err = 0;
	row_corner_num = 0;
	column_corner_num = 0;
	grid_width = 0;
	grid_height = 0;
	PicNum==0;
	CCali_ParaDlg dlg;
	dlg.DoModal();
	if(row_corner_num==0|column_corner_num==0|grid_width==0|grid_height==0|PicNum==0)
	{
		AfxMessageBox(_T("请将参数填写完整！"));
	}
	else
	{
		cout<<"***开始左相机标定***"<<endl;
		cout<<endl;
		WriteTxt(A,PicNum);
		string LImgPath="Ori_LImage\\chess";
		CircleCalibration(A,B,row_corner_num,column_corner_num,grid_width,grid_height,LImgPath,1);
		
		cout<<endl;
		cout<<"***开始右相机标定，稍候2秒***"<<endl;
		cout<<endl;
		waitKey(2000);

		WriteTxt2(C,PicNum);
		string RImgPath="Ori_RImage\\chess";
		CircleCalibration(C,D,row_corner_num,column_corner_num,grid_width,grid_height,RImgPath,2);
	}

	StereoCircleCalibration(a,c,SCaliFile);
}
