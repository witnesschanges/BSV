
// BSVDlg.cpp : 实现文件
//
#include <iostream>
#include <math.h>
#include "stdafx.h"
#include "BSV.h"
#include "BSVDlg.h"
#include "afxdialogex.h"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "externFunc.h"
#include "Cali_ParaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBSVDlg* g_pBSVDlg = NULL;
CBSVDlg* g_pBSVThread = NULL;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CBSVDlg 对话框
CBSVDlg::CBSVDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBSVDlg::IDD, pParent),
	m_EVKillIAA(false, TRUE, NULL, NULL),
	m_EVDeadIAA(true, TRUE, NULL, NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//左相机变量初始化
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pBSVDlg = this;

	m_LeftCamera.Initial();
	m_RightCamera.Initial();
	m_LeftImage.Initial();
	m_RightImage.Initial();
}

void CBSVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBSVDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenCamera, &CBSVDlg::OnBnClickedOpencamera)
	ON_BN_CLICKED(IDC_CapVideo, &CBSVDlg::OnBnClickedCapvideo)
	ON_BN_CLICKED(IDC_SetCamera, &CBSVDlg::OnBnClickedSetcamera)
	ON_BN_CLICKED(IDC_CircleDetect, &CBSVDlg::OnBnClickedCircledetect)
	ON_BN_CLICKED(IDC_OpenCamera2, &CBSVDlg::OnBnClickedOpencamera2)
	ON_BN_CLICKED(IDC_CapVideo2, &CBSVDlg::OnBnClickedCapvideo2)
	ON_BN_CLICKED(IDC_SetCamera2, &CBSVDlg::OnBnClickedSetcamera2)
	ON_BN_CLICKED(IDC_StopDetect, &CBSVDlg::OnBnClickedStopdetect)
	ON_BN_CLICKED(IDC_Open2Camera, &CBSVDlg::OnBnClickedOpen2camera)
	ON_BN_CLICKED(IDC_Cap2Video, &CBSVDlg::OnBnClickedCap2video)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CircleDetect2, &CBSVDlg::OnBnClickedCircledetect2)
	ON_BN_CLICKED(IDC_StopDetect2, &CBSVDlg::OnBnClickedStopdetect2)
	ON_BN_CLICKED(IDC_Circle2Detect, &CBSVDlg::OnBnClickedCircle2detect)
	ON_BN_CLICKED(IDC_Calibration, &CBSVDlg::OnBnClickedCalibration)
//ON_BN_CLICKED(IDC_BUTTON2, &CBSVDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_SavePic, &CBSVDlg::OnBnClickedSavepic)
ON_BN_CLICKED(IDC_SavePic2, &CBSVDlg::OnBnClickedSavepic2)
ON_BN_CLICKED(IDC_Calibration2, &CBSVDlg::OnBnClickedCalibration2)
//ON_BN_CLICKED(IDC_BUTTON2, &CBSVDlg::OnBnClickedButton2)
//ON_BN_CLICKED(IDC_BUTTON10, &CBSVDlg::OnBnClickedButton10)
END_MESSAGE_MAP()

// CBSVDlg 消息处理程序
BOOL CBSVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//打开控制台
	AllocConsole();
	freopen("CONOUT$", "a+", stdout);

	//固定Picture Control控件的大小
	CRect rectL;  
	GetDlgItem(IDC_LeftPic)->GetWindowRect(&rectL);
	ScreenToClient(&rectL);
	GetDlgItem(IDC_LeftPic)->MoveWindow(rectL.left, rectL.top, 520, 390, true);

	CRect rectR;  
	GetDlgItem(IDC_RightPic)->GetWindowRect(&rectR);
	ScreenToClient(&rectR);
	GetDlgItem(IDC_RightPic)->MoveWindow(rectR.left, rectR.top, 520, 390, true);

	GetDlgItem(IDC_CapVideo)->EnableWindow(false);
	GetDlgItem(IDC_SetCamera)->EnableWindow(false);
	GetDlgItem(IDC_StopDetect)->EnableWindow(false);

	GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
	GetDlgItem(IDC_SetCamera2)->EnableWindow(false);
	GetDlgItem(IDC_StopDetect2)->EnableWindow(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBSVDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//释放内存资源
	if (m_LeftCamera.CameraHandle != NULL)
	{
		MVStopGrab(m_LeftCamera.CameraHandle);
		MVCloseCam(m_LeftCamera.CameraHandle);
		m_LeftCamera.CameraHandle = NULL;
	}

	delete[] m_RightImage.ShowDIBBits;
	m_LeftImage.ShowDIBBits = NULL;
	delete[] m_LeftImage.DIBBits;
	m_LeftImage.DIBBits = NULL;
	delete[] m_LeftImage.OriDIBBits;
	m_LeftImage.OriDIBBits = NULL;

	//清除定时器
	if (m_TimerFlag)
	{
		KillTimer(1);
		m_TimerFlag = NULL;
	}

	if (m_TimerFlag2)
	{
		KillTimer(2);
		m_TimerFlag2 = NULL;
	}

	Blob_ReleaseLeftBlobSeq();
	Blob_ReleaseRightBlobSeq();

	CDialogEx::OnClose();
}

void CBSVDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CBSVDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBSVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
 *函数说明：相机回调函数,在相机工作在连续与触发模式下时，每获得一帧图像就会调用该函数说明。
 *输入参数：无
 *		
 *输出参数：
 LPBYTE pbuf       图像数据区
 int nW            图像的宽
 int nH            图像的高
 int nBitCount     图像的像素位数
 int nCameraIndex  相机编号，从1开始计数。
 FILE *pFile       保存检测信息的文件指针
 *返回值：无。
*/
int LeftCallbackFunction(MV_IMAGE_INFO *pInfo,long nUserVal)
	// int index, LPSTR pData, int Length, void* pUserData )
{
	if(g_pBSVDlg->m_LeftImage.OriDIBBits != NULL)
	{
		//拷贝图像内存
		g_pBSVDlg->m_LeftCamera.SectionCopy.Lock();

		//为原始图像赋值，如果是24位RGB格式，则直接转换
		if( g_pBSVDlg->m_LeftCamera.PixelFormat == PixelFormat_Mono8 )
		{
			memcpy(g_pBSVDlg->m_LeftImage.OriDIBBits,pInfo->pImageBuffer,
				g_pBSVDlg->m_LeftImage.OriWidth*g_pBSVDlg->m_LeftImage.OriHeight);
		}
		else
		{
			int count = (((g_pBSVDlg->m_LeftImage.OriWidth*g_pBSVDlg->m_LeftImage.Bit)+31)/32)*4;
			MVBayerToBGR(g_pBSVDlg->m_LeftCamera.CameraHandle,pInfo->pImageBuffer,g_pBSVDlg->m_LeftImage.OriDIBBits,count,
				g_pBSVDlg->m_LeftImage.OriWidth,g_pBSVDlg->m_LeftImage.OriHeight,g_pBSVDlg->m_LeftCamera.PixelFormat);
		}

		LPSTR itemBits;
		itemBits = new char[g_pBSVDlg->m_LeftImage.OriWidth*g_pBSVDlg->m_LeftImage.OriHeight*(g_pBSVDlg->m_LeftImage.Bit/8)];
		memcpy(itemBits,g_pBSVDlg->m_LeftImage.OriDIBBits,g_pBSVDlg->m_LeftImage.OriWidth*g_pBSVDlg->m_LeftImage.OriHeight*(g_pBSVDlg->m_LeftImage.Bit/8)); 
		for (int i=0;i<g_pBSVDlg->m_LeftImage.OriHeight;i++)
		{
			for (int j=0;j<g_pBSVDlg->m_LeftImage.OriWidth;j++)
			{
				if (g_pBSVDlg->m_LeftImage.Bit == 8)
				{
					g_pBSVDlg->m_LeftImage.ShowDIBBits[i*g_pBSVDlg->m_LeftImage.OriWidth+j] = 
						itemBits[(g_pBSVDlg->m_LeftImage.OriHeight-i-1)*g_pBSVDlg->m_LeftImage.OriWidth+j];
				}
				else if (g_pBSVDlg->m_LeftImage.Bit == 24)
				{
					g_pBSVDlg->m_LeftImage.ShowDIBBits[(i*g_pBSVDlg->m_LeftImage.OriWidth+j)*3] = 
						itemBits[((g_pBSVDlg->m_LeftImage.OriHeight-i-1)*g_pBSVDlg->m_LeftImage.OriWidth+j)*3];
					g_pBSVDlg->m_LeftImage.ShowDIBBits[(i*g_pBSVDlg->m_LeftImage.OriWidth+j)*3+1] = 
						itemBits[((g_pBSVDlg->m_LeftImage.OriHeight-i-1)*g_pBSVDlg->m_LeftImage.OriWidth+j)*3+1];
					g_pBSVDlg->m_LeftImage.ShowDIBBits[(i*g_pBSVDlg->m_LeftImage.OriWidth+j)*3+2] = 
						itemBits[((g_pBSVDlg->m_LeftImage.OriHeight-i-1)*g_pBSVDlg->m_LeftImage.OriWidth+j)*3+2];
				}
			}
		}
		delete []itemBits;
		g_pBSVDlg->m_LeftCamera.SectionCopy.Unlock();

		g_pBSVDlg->m_LeftImage.BufHandle = true;//左图像可以进行处理标识
		g_pBSVDlg->DrawLeftCamera();
	}
	return 1;
}

//*****以下函数都针对右相机编写*****//
int RightCallbackFunction(MV_IMAGE_INFO *pInfo,long nUserVal)
	// int index, LPSTR pData, int Length, void* pUserData )
{
	if(g_pBSVDlg->m_RightImage.OriDIBBits != NULL)//有问题
	{
		//拷贝图像内存
		g_pBSVDlg->m_RightCamera.SectionCopy.Lock();

		//为原始图像赋值，如果是24位RGB格式，则直接转换
		if( g_pBSVDlg->m_RightCamera.PixelFormat == PixelFormat_Mono8 )
		{
			memcpy(g_pBSVDlg->m_RightImage.OriDIBBits,pInfo->pImageBuffer,
				g_pBSVDlg->m_RightImage.OriWidth*g_pBSVDlg->m_RightImage.OriHeight);
		}
		else
		{
			int count = (((g_pBSVDlg->m_RightImage.OriWidth*g_pBSVDlg->m_RightImage.Bit)+31)/32)*4;
			MVBayerToBGR(g_pBSVDlg->m_RightCamera.CameraHandle,pInfo->pImageBuffer,g_pBSVDlg->m_RightImage.OriDIBBits,count,
				g_pBSVDlg->m_RightImage.OriWidth,g_pBSVDlg->m_RightImage.OriHeight,g_pBSVDlg->m_RightCamera.PixelFormat);
		}

		LPSTR itemBits;
		itemBits = new char[g_pBSVDlg->m_RightImage.OriWidth*g_pBSVDlg->m_RightImage.OriHeight*(g_pBSVDlg->m_RightImage.Bit/8)];
		memcpy(itemBits,g_pBSVDlg->m_RightImage.OriDIBBits,g_pBSVDlg->m_RightImage.OriWidth*g_pBSVDlg->m_RightImage.OriHeight*(g_pBSVDlg->m_RightImage.Bit/8)); 
		for (int i=0;i<g_pBSVDlg->m_RightImage.OriHeight;i++)
		{
			for (int j=0;j<g_pBSVDlg->m_RightImage.OriWidth;j++)
			{
				if (g_pBSVDlg->m_RightImage.Bit == 8)
				{
					g_pBSVDlg->m_RightImage.ShowDIBBits[i*g_pBSVDlg->m_RightImage.OriWidth+j] = 
						itemBits[(g_pBSVDlg->m_RightImage.OriHeight-i-1)*g_pBSVDlg->m_RightImage.OriWidth+j];
				}
				else if (g_pBSVDlg->m_RightImage.Bit == 24)
				{
					g_pBSVDlg->m_RightImage.ShowDIBBits[(i*g_pBSVDlg->m_RightImage.OriWidth+j)*3] = 
						itemBits[((g_pBSVDlg->m_RightImage.OriHeight-i-1)*g_pBSVDlg->m_RightImage.OriWidth+j)*3];
					g_pBSVDlg->m_RightImage.ShowDIBBits[(i*g_pBSVDlg->m_RightImage.OriWidth+j)*3+1] = 
						itemBits[((g_pBSVDlg->m_RightImage.OriHeight-i-1)*g_pBSVDlg->m_RightImage.OriWidth+j)*3+1];
					g_pBSVDlg->m_RightImage.ShowDIBBits[(i*g_pBSVDlg->m_RightImage.OriWidth+j)*3+2] = 
						itemBits[((g_pBSVDlg->m_RightImage.OriHeight-i-1)*g_pBSVDlg->m_RightImage.OriWidth+j)*3+2];
				}
			}
		}
		delete []itemBits;
		g_pBSVDlg->m_RightCamera.SectionCopy.Unlock();

		g_pBSVDlg->m_RightImage.BufHandle = true;//左图像可以进行处理标识
		g_pBSVDlg->DrawRightCamera();
	}
	return 1;
}

/*
*函数说明：显示左相机图像
*输入参数：无
*          
*输出参数：无。
*返 回 值：无。
*/
void CBSVDlg::DrawLeftCamera()
{
	m_LeftCamera.Section.Lock();
	CDC *pdc = GetDlgItem(IDC_LeftPic)->GetDC();//获得DC
	CRect rc;
	GetDlgItem(IDC_LeftPic)->GetClientRect(&rc);//获得图像显示区域
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR );//设置模式

	//显示图像
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(),rc.Height(),
		0,0,m_LeftImage.OriWidth,m_LeftImage.OriHeight,
		m_LeftImage.ShowDIBBits,(LPBITMAPINFO) m_LeftImage.BmpInfo, DIB_RGB_COLORS, SRCCOPY); 
	
	//显示十字参考线
	//CPen pPenBlue;
	//pPenBlue.CreatePen(PS_SOLID,1,RGB(0,0,128));
	//CPen *pOldPen=pdc->SelectObject(&pPenBlue);
	//double dbRateX = (double)rc.Width() /(double)m_LeftImage.Width;
	//double dbRateY = (double)rc.Height() /(double)m_LeftImage.Height;
	//double x1 = m_LeftImage.Width*dbRateX;
	//double x2 = x1/2;
	//double y1 = m_LeftImage.Height*dbRateY;
	//double y2 = y1/2;
	//pdc->MoveTo (0,y2);
	//pdc->LineTo (x1,y2);
	//pdc->MoveTo (x2,0);
	//pdc->LineTo (x2,y1);
	//pdc->SelectObject(pOldPen);
	//pPenBlue.DeleteObject();

	GetDlgItem(IDC_LeftPic)->ReleaseDC(pdc);	
	m_LeftCamera.Section.Unlock();
}

/*
*函数说明：显示右相机图像
*输入参数：无
*          
*输出参数：无。
*返 回 值：无。
*/
void CBSVDlg::DrawRightCamera()
{
	m_RightCamera.Section.Lock();
	CDC *pdc = GetDlgItem(IDC_RightPic)->GetDC();//获得DC
	CRect rc;
	GetDlgItem(IDC_RightPic)->GetClientRect(&rc);//获得图像显示区域
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;//设置模式

	//显示图像
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(),rc.Height(),
		0,0,m_RightImage.OriWidth,m_RightImage.OriHeight,
		m_RightImage.ShowDIBBits,(LPBITMAPINFO) m_RightImage.BmpInfo, DIB_RGB_COLORS, SRCCOPY); 
	
	//显示十字参考线
	//CPen pPenBlue;
	//pPenBlue.CreatePen(PS_SOLID,1,RGB(0,0,128));
	//CPen *pOldPen=pdc->SelectObject(&pPenBlue);
	//double dbRateX = (double)rc.Width() /(double)m_RightImage.Width;
	//double dbRateY = (double)rc.Height() /(double)m_RightImage.Height;
	//double x1 = m_RightImage.Width*dbRateX;
	//double x2 = x1/2;
	//double y1 = m_RightImage.Height*dbRateY;
	//double y2 = y1/2;
	//pdc->MoveTo (0,y2);
	//pdc->LineTo (x1,y2);
	//pdc->MoveTo (x2,0);
	//pdc->LineTo (x2,y1);
	//pdc->SelectObject(pOldPen);
	//pPenBlue.DeleteObject();

	GetDlgItem(IDC_RightPic)->ReleaseDC(pdc);	
	m_RightCamera.Section.Unlock();
}

void CBSVDlg::OpenCamera(Camera &camera, UINT32 openCameraId, UINT32 capVideoId, UINT32 setCameraId)
{
	MVSTATUS_CODES hr;
	if (camera.CameraHandle == NULL)	//启动相机
	{
		hr = MVOpenCamByIndex(0, &camera.CameraHandle);
		if (hr != MVST_SUCCESS)
		{
			MessageBox("未找到相机，请确认设备连接和IP设置！", "BSV", MB_ICONWARNING);
			return;
		}
		else if (hr == MVST_SUCCESS)
		{
			GetDlgItem(openCameraId)->SetWindowText("关闭相机");
			GetDlgItem(capVideoId)->EnableWindow(true);
			GetDlgItem(setCameraId)->EnableWindow(true);
		}
	}
	else	//关闭相机
	{
		hr = MVStopGrab(camera.CameraHandle);
		hr = MVCloseCam(camera.CameraHandle);
		camera.CameraHandle = NULL;

		GetDlgItem(openCameraId)->SetWindowText("启动相机");
		GetDlgItem(capVideoId)->EnableWindow(false);
		GetDlgItem(setCameraId)->EnableWindow(false);
	}
}

void CBSVDlg::OnBnClickedOpencamera()
{
	// TODO: 在此添加控件通知处理程序代码
	//启动左相机
	OpenCamera(m_LeftCamera, IDC_OpenCamera, IDC_CapVideo, IDC_SetCamera);

}

void CBSVDlg::OnBnClickedCapvideo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_LeftCamera.CameraHandle != NULL)
	{
		MVGetWidth(m_LeftCamera.CameraHandle, &m_LeftImage.OriWidth);  //获得图像长宽
		MVGetHeight(m_LeftCamera.CameraHandle,&m_LeftImage.OriHeight);
		MVGetPixelFormat(m_LeftCamera.CameraHandle,&m_LeftCamera.PixelFormat);  //获得数据格式
		if( m_LeftCamera.PixelFormat == PixelFormat_Mono8 )
		{
			m_LeftImage.Bit = 8;
		}
		else
		{
			m_LeftImage.Bit = 24;
		}
		
		MVSTATUS_CODES hr;
		hr =  MVStartGrab(m_LeftCamera.CameraHandle, LeftCallbackFunction, (long)this); //设置回调函数
		if (hr == MVST_SUCCESS)
		{	
			//原始图像
			m_LeftImage.BmpInfo->bmiHeader.biBitCount = m_LeftImage.Bit;
			m_LeftImage.BmpInfo->bmiHeader.biWidth = m_LeftImage.OriWidth;
			m_LeftImage.BmpInfo->bmiHeader.biHeight = m_LeftImage.OriHeight;

			//为图像显示的图像数据分配内存空间
			delete[] m_LeftImage.ShowDIBBits;
			m_LeftImage.ShowDIBBits = NULL;
			m_LeftImage.ShowDIBBits = new char[m_LeftImage.OriWidth * m_LeftImage.OriHeight * (m_LeftImage.Bit/8)];

			delete[] m_LeftImage.OriDIBBits;
			m_LeftImage.OriDIBBits = NULL;
			m_LeftImage.OriDIBBits = new char[m_LeftImage.OriWidth * m_LeftImage.OriHeight * (m_LeftImage.Bit/8)];

			//为进行处理的图像赋值
			m_LeftImage.Width=m_LeftImage.OriWidth;
			m_LeftImage.Height=m_LeftImage.OriHeight;

			delete[] m_LeftImage.DIBBits;
			m_LeftImage.DIBBits = NULL;
			m_LeftImage.DIBBits = new char[m_LeftImage.Width * m_LeftImage.Height * (m_LeftImage.Bit/8)];

			//开始采集图像后，不再接受更新的“连续采集”指令
			GetDlgItem(IDC_CapVideo)->EnableWindow(false);
			//GetDlgItem(IDC_Btn_SetCamera)->EnableWindow(false);
		}
	}
}

void CBSVDlg::OnBnClickedSetcamera()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_LeftCamera.CameraHandle == NULL )
	{
		MessageBox("尚未启动相机，无法配置！","BSV",MB_ICONWARNING);
	}
	else
	{
		//SetCameraDlg Dlg;	//新建相机配置对话框
		//if (Dlg.DoModal())
		//{
		//}
		MessageBox("暂无此功能");
	}
}

void CBSVDlg::OnBnClickedCircledetect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_LeftImage.ShowDIBBits == NULL)
	{
		MessageBox("无图像数据！","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		SetTimer(1, 300, NULL);//设置定时器1
		m_TimerFlag = true;

		GetDlgItem(IDC_CircleDetect)->EnableWindow(false);
		GetDlgItem(IDC_StopDetect)->EnableWindow(true);
	}
}

void CBSVDlg::OnBnClickedStopdetect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_TimerFlag)
	{
		KillTimer(1);
		m_TimerFlag = false;
	}

	GetDlgItem(IDC_CircleDetect)->EnableWindow(true);
	GetDlgItem(IDC_StopDetect)->EnableWindow(false);
}

/*
*函数功能：响应定时器消息处理函数
*参数说明：
nIDEvent 定时器标识符。在一个窗体内可以使用多个定时器
         ，不同的定时器根据nIDEvent来区分。
*/
void CBSVDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)//nIDEvent 为定时器事件ID，1，2，3
	{
	case 1:
		{
			if( m_LeftCamera.PixelFormat != PixelFormat_Mono8 )
				return;

			Mat LSrcImg;
			LSrcImg.create(m_LeftImage.OriHeight, m_LeftImage.OriWidth, CV_8UC1);

			g_pBSVDlg->m_LeftCamera.SectionCopy.Lock();
			memcpy(LSrcImg.data,m_LeftImage.ShowDIBBits,m_LeftImage.OriWidth*m_LeftImage.OriHeight*(m_LeftImage.Bit/8));
			g_pBSVDlg->m_LeftCamera.SectionCopy.Unlock();
	
			Mat LFilImg;
			//双边滤波，保边去噪
			//bilateralFilter(LSrcImg, LFilImg, 25, 25*2, 25/2);
			//高斯滤波
			GaussianBlur(LSrcImg, LFilImg, Size(7,7), 0, 0);

			Mat LBinImg;
			//自适应阈值分割图像	
			//adaptiveThreshold(LFilImg, LBinImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, 
			//CV_THRESH_BINARY, 7, 5);	
			//固定阈值分割图像 lq:二值化图像
			threshold(LFilImg, LBinImg, 100, 255, CV_THRESH_BINARY);
	
			//Blob_ReleaseLeftBlobSeq();
			//Detect_LeftCircleDetect(LBinImg, 0, 255, 0.6, 0.6);
			//Blob_DenoisingLeftArea(1000, 8000);
			
			if (!m_LeftCamera.BlobSeq.IsEmpty())
			{
				ShowLeftCircles();
			}
			LSrcImg.release();
			LFilImg.release();
		}
		break;
	case 2:
		{
			if( m_RightCamera.PixelFormat != PixelFormat_Mono8 )
				return;

			Mat RSrcImg;
			RSrcImg.create(m_RightImage.OriHeight, m_RightImage.OriWidth, CV_8UC1);
			g_pBSVDlg->m_RightCamera.SectionCopy.Lock();
			memcpy(RSrcImg.data,m_RightImage.ShowDIBBits,m_RightImage.OriWidth*m_RightImage.OriHeight*(m_RightImage.Bit/8));
			g_pBSVDlg->m_RightCamera.SectionCopy.Unlock();
	
			Mat RFilImg;
			//双边滤波，保边去噪
			//bilateralFilter(RSrcImg, RFilImg, 25, 25*2, 25/2);
			//高斯滤波
			GaussianBlur(RSrcImg, RFilImg, Size(7,7), 0, 0);

			Mat RBinImg;
			//自适应阈值分割图像
			//adaptiveThreshold(RFilImg, RBinImg, 255, CV_ADAPTIVE_THRESH_MEAN_C,
			//CV_THRESH_BINARY, 7, 5);
			//固定阈值分割图像 lq:二值化图像
			threshold(RFilImg, RBinImg, 100, 255, CV_THRESH_BINARY);
			
			Blob_ReleaseRightBlobSeq();
			Detect_RightCircleDetect(RBinImg, 0, 255, 0.6, 0.6);
			Blob_DenoisingRightArea(1000, 8000);

			if (!m_RightCamera.BlobSeq.IsEmpty())
			{
				ShowRightCircles();
			}
			RSrcImg.release();
			RFilImg.release();
			RBinImg.release();
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CBSVDlg::OnBnClickedOpencamera2()
{
	// TODO: 在此添加控件通知处理程序代码
	//启动右相机	
	OpenCamera(m_RightCamera, IDC_OpenCamera2, IDC_CapVideo2, IDC_SetCamera2);
}

void CBSVDlg::OnBnClickedCapvideo2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_RightCamera.CameraHandle != NULL)
	{
		MVGetWidth(m_RightCamera.CameraHandle, &m_RightImage.OriWidth);  //获得图像长宽
		MVGetHeight(m_RightCamera.CameraHandle,&m_RightImage.OriHeight);
		MVGetPixelFormat(m_RightCamera.CameraHandle,&m_RightCamera.PixelFormat);  //获得数据格式
		if( m_RightCamera.PixelFormat == PixelFormat_Mono8 )
		{
			m_RightImage.Bit = 8;
		}
		else
		{
			m_RightImage.Bit = 24;
		}
		
		MVSTATUS_CODES hr2;
		hr2 =  MVStartGrab(m_RightCamera.CameraHandle, RightCallbackFunction, (long)this); //设置回调函数
		if (hr2 == MVST_SUCCESS)
		{	
			//原始图像
			m_RightImage.BmpInfo->bmiHeader.biBitCount = m_RightImage.Bit;
			m_RightImage.BmpInfo->bmiHeader.biWidth = m_RightImage.OriWidth;
			m_RightImage.BmpInfo->bmiHeader.biHeight = m_RightImage.OriHeight;

			//为图像显示的图像数据分配内存空间
			delete[] m_RightImage.ShowDIBBits;
			m_RightImage.ShowDIBBits = NULL;
			m_RightImage.ShowDIBBits = new char[m_RightImage.OriWidth * m_RightImage.OriHeight * (m_RightImage.Bit/8)];

			delete[] m_RightImage.OriDIBBits;
			m_RightImage.OriDIBBits = NULL;
			m_RightImage.OriDIBBits = new char[m_RightImage.OriWidth * m_RightImage.OriHeight * (m_RightImage.Bit/8)];

			//为进行处理的图像赋值
			m_RightImage.Width=m_RightImage.OriWidth;
			m_RightImage.Height=m_RightImage.OriHeight;

			delete[] m_RightImage.DIBBits;
			m_RightImage.DIBBits = NULL;
			m_RightImage.DIBBits = new char[m_RightImage.Width * m_RightImage.Height * (m_RightImage.Bit/8)];

			//开始采集图像后，不再接受更新的“连续采集”指令
			GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
			//GetDlgItem(IDC_Btn_SetCamera)->EnableWindow(false);
		}
	}
}

void CBSVDlg::OnBnClickedSetcamera2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_RightCamera.CameraHandle == NULL )
	{
		MessageBox("尚未启动相机，无法配置！","BSV",MB_ICONWARNING);
	}
	else
	{
		//SetCameraDlg Dlg;	//新建相机配置对话框
		//if (Dlg.DoModal())
		//{
		//}
		MessageBox("暂无此功能");
	}
}

void CBSVDlg::OnBnClickedOpen2camera()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedOpencamera();
	OnBnClickedOpencamera2();
}

void CBSVDlg::OnBnClickedCap2video()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedCapvideo();
	OnBnClickedCapvideo2();
}

/*
*函数功能：释放BlobSeq中的Blob数据内存，清除BlobSeq中的数据
*参数说明
CPtrArray* BlobSeq	   待释放内存的团块队列。
*/
/*左相机*/
void CBSVDlg::Blob_ReleaseLeftBlobSeq()
{
	if (!m_LeftCamera.BlobSeq.IsEmpty())
	{
		m_LeftCamera.BlobSeq.RemoveAll();
		m_LeftCamera.BlobSeq.FreeExtra();
	}
}

/*右相机*/
void CBSVDlg::Blob_ReleaseRightBlobSeq()
{
	if (!m_RightCamera.BlobSeq.IsEmpty())
	{
		m_RightCamera.BlobSeq.RemoveAll();
		m_RightCamera.BlobSeq.FreeExtra();
	}
}

/*
*函数功能：检测圆形特征
*参数说明
const CvArr* SrcImage         待处理的灰度图像或二值化图像
double lowthresh, highthresh  自适应Canny边缘检测的高低阈值
int LeastPixelNum             目标轮廓包含的最小像素点数目
double AspectRatio            纵横比
double Circularity            圆形度
*/
/*左相机检测圆形*/
void CBSVDlg::Detect_LeftCircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity)
{
	Mat EdgeImg;
	Canny(SrcImg, EdgeImg, lowthresh, highthresh);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(EdgeImg, contours, hierarchy, CV_RETR_LIST,
		CV_CHAIN_APPROX_NONE, cvPoint(0,0));
	EdgeImg.release();

	double area, length, cir, width, height, asp;
	RotatedRect rect;
	int cnt = 0;
	int ln = contours.size();
	m_LeftCamera.BlobSeq.SetSize(ln,10);
	for (int i=0; i<ln; i++)
	{
		if (contours[i].size() > 10)
		{
			area = contourArea(contours[i]);		//第i个轮廓的面积
			length = arcLength(contours[i],true);	//第i个轮廓的周长
			cir = (12.56*area)/(length*length);		//第i个轮廓的圆形度
			if (cir > Circularity)
			{
				rect = fitEllipse(contours[i]);
				width = rect.size.width;	//外接矩形的宽度
				height = rect.size.height;	//外接矩形的高度
				asp = height/width;			//纵横比
				if (asp>AspectRatio && asp<(1.0/AspectRatio))
				{
					Blob pB;
					pB.BlobWidth = width;
					pB.BlobHeight = height;
					pB.Area = area;
					pB.Perimeter = length;
					pB.AspectRatio = asp;
					pB.Circularity = cir;
					pB.BlobX = rect.center.x;
					pB.BlobY = rect.center.y;

					m_LeftCamera.BlobSeq.SetAtGrow(cnt,pB);
					cnt++;
				}
			}
		}
	}
	m_LeftCamera.BlobSeq.FreeExtra();
}

/*右相机检测圆形*/
void CBSVDlg::Detect_RightCircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity)
{
	Mat EdgeImg;
	Canny(SrcImg, EdgeImg, lowthresh, highthresh);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(EdgeImg, contours, hierarchy, CV_RETR_LIST,
		CV_CHAIN_APPROX_NONE, cvPoint(0,0));
	EdgeImg.release();

	double area, length, cir, width, height, asp;
	RotatedRect rect;
	int cnt = 0;
	int ln = contours.size();
	m_RightCamera.BlobSeq.SetSize(ln,10);
	for (int i=0; i<ln; i++)
	{
		if (contours[i].size() > 10)
		{
			area = contourArea(contours[i]);		//第i个轮廓的面积
			length = arcLength(contours[i],true);	//第i个轮廓的周长
			cir = (12.56*area)/(length*length);		//第i个轮廓的圆形度
			if (cir > Circularity)
			{
				rect = fitEllipse(contours[i]);
				width = rect.size.width;	//外接矩形的宽度
				height = rect.size.height;	//外接矩形的高度
				asp = height/width;			//纵横比
				if (asp>AspectRatio && asp<(1.0/AspectRatio))
				{
					Blob pB;
					pB.BlobWidth = width;
					pB.BlobHeight = height;
					pB.Area = area;
					pB.Perimeter = length;
					pB.AspectRatio = asp;
					pB.Circularity = cir;
					pB.BlobX = rect.center.x;
					pB.BlobY = rect.center.y;

					m_RightCamera.BlobSeq.SetAtGrow(cnt,pB);
					cnt++;
				}
			}
		}
	}
	m_RightCamera.BlobSeq.FreeExtra();
}

////*函数功能：定时器2回调函数
//void CALLBACK CBSVDlg::Timer2(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime )
//{
//	
//}

/*
*函数功能：清除团块队列中面积不在指定范围内的团块数据
*参数说明
CPtrArray* BlobSeq     待处理的团块队列
int MinArea,MaxArea    指定的团块面积范围
*/
/*左相机*/
void CBSVDlg::Blob_DenoisingLeftArea(int MinArea, int MaxArea)
{
	if (!m_LeftCamera.BlobSeq.IsEmpty())
	{
		int ln = m_LeftCamera.BlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = m_LeftCamera.BlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				m_LeftCamera.BlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		m_LeftCamera.BlobSeq.FreeExtra();
	}
}

/*右相机*/
void CBSVDlg::Blob_DenoisingRightArea(int MinArea, int MaxArea)
{
	if (!m_RightCamera.BlobSeq.IsEmpty())
	{
		int ln = m_RightCamera.BlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = m_RightCamera.BlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				m_RightCamera.BlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		m_RightCamera.BlobSeq.FreeExtra();
	}
}

void CBSVDlg::ShowLeftCircles()
{
	CDC *pdc = GetDlgItem(IDC_LeftPic)->GetDC();		//获得ＤＣ
	CRect rc;
	GetDlgItem(IDC_LeftPic)->GetClientRect(&rc);		//获得图像显示区域
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;	//设置模式

	// 创建红色画笔对象
	CBrush BrushRed(RGB(255, 0, 0));
	CBrush *pOldBrush;
	pOldBrush = pdc->SelectObject(&BrushRed);

	double dbRateX = (double)rc.Width()/(double)m_LeftImage.OriWidth;
	double dbRateY = (double)rc.Height()/(double)m_LeftImage.OriHeight;
	int ln = m_LeftCamera.BlobSeq.GetSize();
	for (int i=0;i<ln;i++)
	{
		Blob pB = m_LeftCamera.BlobSeq.GetAt(i);
		int x, y;
		x = (int)(pB.BlobX*dbRateX);
		y = (int)((m_LeftImage.OriHeight - pB.BlobY)*dbRateY);
		if(x<6)
		{
			x = 6;
		}
		else if(x>rc.Width()-6)
		{
			x = rc.Width()-6;
		}
		if(y<6)
		{
			y = 6;
		}
		else if(y>rc.Height() - 6)
		{
			y = rc.Height()-6;
		}
		pdc->Ellipse(x-5, y-5, x+5, y+5);
	}

	pdc->SelectObject(pOldBrush);
	BrushRed.DeleteObject();
	ReleaseDC(pdc);
}

void CBSVDlg::ShowRightCircles()
{
	CDC *pdc = GetDlgItem(IDC_RightPic)->GetDC();		//获得ＤＣ
	CRect rc;
	GetDlgItem(IDC_RightPic)->GetClientRect(&rc);		//获得图像显示区域
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;	//设置模式

	// 创建红色画笔对象
	CBrush BrushRed(RGB(255, 0, 0));
	CBrush *pOldBrush;
	pOldBrush = pdc->SelectObject(&BrushRed);

	double dbRateX = (double)rc.Width()/(double)m_RightImage.OriWidth;
	double dbRateY = (double)rc.Height()/(double)m_RightImage.OriHeight;
	int ln = m_RightCamera.BlobSeq.GetSize();
	for (int i=0;i<ln;i++)
	{
		Blob pB = m_RightCamera.BlobSeq.GetAt(i);
		int x, y;
		x = (int)(pB.BlobX*dbRateX);
		y = (int)((m_RightImage.OriHeight - pB.BlobY)*dbRateY);
		if(x<6)
		{
			x = 6;
		}
		else if(x>rc.Width()-6)
		{
			x = rc.Width()-6;
		}
		if(y<6)
		{
			y = 6;
		}
		else if(y>rc.Height() - 6)
		{
			y = rc.Height()-6;
		}
		pdc->Ellipse(x-5, y-5, x+5, y+5);
	}

	pdc->SelectObject(pOldBrush);
	BrushRed.DeleteObject();
	ReleaseDC(pdc);
}

void CBSVDlg::OnBnClickedCircledetect2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_RightImage.ShowDIBBits == NULL)
	{
		MessageBox("无图像数据！","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		SetTimer(2, 300, NULL);//lq:设置定时器2
		m_TimerFlag2 = true;

		GetDlgItem(IDC_CircleDetect2)->EnableWindow(false);
		GetDlgItem(IDC_StopDetect2)->EnableWindow(true);
	}
}

void CBSVDlg::OnBnClickedStopdetect2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_TimerFlag2)
	{
		KillTimer(2);
		m_TimerFlag2 = false;
	}

	GetDlgItem(IDC_CircleDetect2)->EnableWindow(true);
	GetDlgItem(IDC_StopDetect2)->EnableWindow(false);
}

void CBSVDlg::OnBnClickedCircle2detect()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedCircledetect();
	OnBnClickedCircledetect2();
}

CString format_fraction(double data)
{
	CString result;     //格式化之后的数据有效位数为6
	
	//暂存输入数据并存为整形
	int tmp;
	tmp = abs((int)data);

	if (tmp/10000>=1)
	{
		result.Format("%.0f", data);
		return result;
	}
	if (tmp/1000>=1)
	{
		result.Format("%.1f", data);
		return result;
	}
	if (tmp/100>=1)
	{
		result.Format("%.2f", data);
		return result;
	}
	if (tmp/10>=1)
	{
		result.Format("%.3f", data);
		return result;
	}
    else
	{
		result.Format("%.4f", data);
		return result;
	}
}
///*
//*函数说明：左相机标定完成函数。
//*输入参数：无
//*输出参数: 无
//*返 回 值：TRUE表示标定成功,FALSE表示失败。
//*/
//BOOL CBSVDlg::CalibFinishLeftCamera()
//{
//	//清除记录左图像特征圆坐标点
//	Blob_ReleaseLeftBlobSeq();	//lq:有改动
//	//清除记录各个图像的标靶坐标
//	m_LeftCalibration.CalibDraw = false;//置绘制左相机标定标志
//	OnPaint();
//
//    int left_num=0;
//	left_num=m_LeftCalibration.CoordSeq.GetSize();
//	
//	//相机标定参数
//	CString strCalibrate = "";
//	double zero(0.0);
//	double unit(1.0);
//	double **left_re = NULL;
//	left_re=new double*[left_num];
//	int k;
//	for (k=0;k<left_num;k++)
//	{
//		left_re[k]=new double[99];
//	}
//	intrinsic_calibration_result leftICR;
//	
//    CPtrArray * leftx = & m_LeftCalibration.CoordSeq;
//	if (!Calibration_LeftCamera(leftx,leftICR,left_re))
//	{
//		for (int i=0;i<m_LeftCalibration.CoordSeq.GetSize();i++)
//		{
//			Image_Coordation*	pIC;
//			pIC=(Image_Coordation*)m_LeftCalibration.CoordSeq.GetAt(i);
//			delete pIC;
//			pIC=NULL;
//		}
//		m_LeftCalibration.CoordSeq.RemoveAll();	
//		if(left_re != NULL)
//		{
//			for (k=0;k<left_num;k++)
//			{
//				delete []left_re[k];
//				left_re[k] = NULL;
//			}
//			delete []left_re;
//			left_re = NULL;
//		}
//    	GetDlgItem(IDC_LIST_BVS_ListFinish)->ShowWindow(SW_HIDE);//标定完成数据显示List
//	    m_static_FinishText.ShowWindow(SW_SHOW);//标定完成提示静态框
//
//		m_static_FinishText.SetWindowText(STR_CalibFailText);//STR_CalibFailText应该指的是“标定失败”
//
//		return false; 
//	}
//	else
//	{
//		GetDlgItem(IDC_STATIC_ShouDInfor)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_LIST_BVS_ListFinish)->ShowWindow(SW_SHOW);//标定完成数据显示List
//		m_static_FinishText.ShowWindow(SW_SHOW);//标定完成提示静态框
//		
//		int left_grade;
//		CString ColumnName;
//		left_grade=Calibration_ComputeEvaluate(left_re,left_num);
//		
//		//显示相机标定参数
//	
//
//		CString s;
//		s.Format("%s\n%s\nk1=%s, k2=%s\n",STR_LCalibFinishText_1,STR_LCalibFinishText_2,format_fraction(leftICR.k1), format_fraction(leftICR.k2)) ;
//	//	s.Format("左像机标定结果:\n畸变系数:\nk1=%s, k2=%s\n",format_fraction(leftICR.k1), format_fraction(leftICR.k2)) ;
//	
//		strCalibrate += s;
//		
//		s.Format("%s\n",STR_LCalibFinishText_3);
//	//	s = "左像机内参矩阵A:\n";
//		strCalibrate += s;
//		
//		s.Format("%s %s  %s\n%s %s  %s\n%s %s  %s\n",format_fraction(leftICR.fx) , format_fraction(leftICR.skewness), 
//			format_fraction(leftICR.u0), format_fraction(zero),  format_fraction(leftICR.fy),
//			format_fraction(leftICR.v0), format_fraction(zero), format_fraction(zero), format_fraction(unit));
//		strCalibrate += s;
//		
//		s.Format("%s%s",STR_LCalibFinishText_4,g_strEvaluate[left_grade]);
//	//	s = "左像机评估结果:" + strEvaluate[left_grade];
//		strCalibrate += s;
//		m_static_FinishText.SetWindowText(strCalibrate);
//
//		//清空所有列
//		while(m_list_Finish.DeleteColumn(0))
//		{
//			
//		}
//		m_list_Finish.DeleteAllItems();
//		m_list_Finish.InsertColumn(0, STR_LCalibFinishList_1,LVCFMT_IMAGE|LVCFMT_LEFT);  //插入第1列
//		
//		//如果只标定左像机
//		int i;
//		for (i = 1; i <= left_num; i++)
//		{
//			ColumnName.Format("%s%d",STR_LCalibFinishList_2, i);
//			m_list_Finish.InsertColumn(i, ColumnName);//插入第1列
//			
//		}
//		
//		m_list_Finish.SetColumnWidth(0, 70);
//		int j;
//		for(j = 1; j <= left_num; j++)
//		{
//			m_list_Finish.SetColumnWidth(j, 60);//设置列的宽
//		}
//		
//		for (i = 0; i < 99; i++)
//		{
//			//读字段的值
//			ColumnName.Format("%d", i);
//			
//			m_list_Finish.InsertItem(i,0);//插入一行
//			m_list_Finish.SetItemText(i,0,ColumnName);//其中的Counter为行号；0为列号即字段号
//			
//			for(int k = 1; k <= left_num; k++)
//			{
//				ColumnName.Format("%.3f", left_re[k-1][i]);
//				m_list_Finish.SetItemText(i,k,ColumnName);//其中的Counter为行号；0为列号即字段号
//			}		
//		}
//		if(left_re != NULL)
//		{
//			for (k=0;k<left_num;k++)
//			{
//				delete []left_re[k];
//				left_re[k] = NULL;
//			}
//			delete []left_re;
//			left_re = NULL;
//		}
//		for ( i=0;i<m_LeftCalibration.CoordSeq.GetSize();i++)
//		{
//			Image_Coordation*	pIC;
//			pIC=(Image_Coordation*)m_LeftCalibration.CoordSeq.GetAt(i);
//			delete pIC;
//			pIC=NULL;
//		}
//		m_LeftCalibration.CoordSeq.RemoveAll();
//		
//		return true;
//	}
//	return false;
//}
///*
//*函数说明：右相机标定完成函数。
//*输入参数：无
//*输出参数: 无
//*返 回 值：TRUE表示标定成功,FALSE表示失败。
//*/
//BOOL CBSVDlg::CalibFinishRightCamera()
//{
//	//清除记录右图像特征圆坐标点
//	Blob_ReleaseLeftBlobSeq();
//	//清除记录各个图像的标靶坐标
//	m_RightCalibration.CalibDraw = false;//置绘制右相机标定标志
//	OnPaint();
//
//    int right_num=0;
//	right_num=m_RightCalibration.CoordSeq.GetSize();
//	//相机标定参数
//	CString strCalibrate = "";
//	double zero(0.0);
//	double unit(1.0);
//	double **right_re = NULL;
//	right_re=new double*[right_num];
//	int k;
//	for (k=0;k<right_num;k++)
//	{
//		right_re[k]=new double[99];
//	}
//	intrinsic_calibration_result rightICR;
//	
//	if (!Calibration_RightCamera(&m_RightCalibration.CoordSeq,rightICR,right_re))
//	{
//		for (int i=0;i<m_RightCalibration.CoordSeq.GetSize();i++)
//		{
//			Image_Coordation*	pIC;
//			pIC=(Image_Coordation*)m_RightCalibration.CoordSeq.GetAt(i);
//			delete pIC;
//			pIC=NULL;
//		}
//		m_RightCalibration.CoordSeq.RemoveAll();	
//		if(right_re != NULL)
//		{
//			for (k=0;k<right_num;k++)
//			{
//				delete []right_re[k];
//				right_re[k] = NULL;
//			}
//			delete []right_re;
//			right_re = NULL;
//		}
//    	GetDlgItem(IDC_LIST_BVS_ListFinish)->ShowWindow(SW_HIDE);//标定完成数据显示List
//	    m_static_FinishText.ShowWindow(SW_SHOW);//标定完成提示静态框
//
//		m_static_FinishText.SetWindowText(STR_CalibFailText);
//
//		return false;
//	}
//	else
//	{
//		GetDlgItem(IDC_STATIC_ShouDInfor)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_LIST_BVS_ListFinish)->ShowWindow(SW_SHOW);//标定完成数据显示List
//	    m_static_FinishText.ShowWindow(SW_SHOW);//标定完成提示静态框
//
//		
//		int right_grade;
//		CString ColumnName;
//		right_grade=Calibration_ComputeEvaluate(right_re,right_num);
//		
//		//显示相机标定参数
//	
//
//		CString s;
//		s.Format("%s\n%s\nk1=%s, k2=%s\n",STR_RCalibFinishText_1,STR_RCalibFinishText_2,format_fraction(rightICR.k1), format_fraction(rightICR.k2)) ;
//	//	s.Format("右相机标定结果:\n畸变系数:\nk1=%s, k2=%s\n",format_fraction(rightICR.k1), format_fraction(rightICR.k2)) ;
//	
//		strCalibrate += s;
//		
//		s.Format("%s\n",STR_RCalibFinishText_3);
//	//	s = "右相机内参矩阵A:\n";
//		strCalibrate += s;
//		
//		s.Format("%s %s  %s\n%s %s  %s\n%s %s  %s\n",format_fraction(rightICR.fx) , format_fraction(rightICR.skewness), 
//			format_fraction(rightICR.u0), format_fraction(zero),  format_fraction(rightICR.fy),
//			format_fraction(rightICR.v0), format_fraction(zero), format_fraction(zero), format_fraction(unit));
//		strCalibrate += s;
//		
//		s.Format("%s%s",STR_RCalibFinishText_4,g_strEvaluate[right_grade]);
//	//	s = "右相机评估结果:" + strEvaluate[right_grade];
//		strCalibrate += s;
//		m_static_FinishText.SetWindowText(strCalibrate);
//
//		//清空所有列
//		while(m_list_Finish.DeleteColumn(0))
//		{
//			
//		}
//		m_list_Finish.DeleteAllItems();
//		m_list_Finish.InsertColumn(0, STR_RCalibFinishList_1,LVCFMT_IMAGE|LVCFMT_LEFT);  //插入第1列
//		
//		int i;
//		for (i = 1; i <= right_num; i++)
//		{
//			ColumnName.Format("%s%d",STR_RCalibFinishList_2, i);
//			m_list_Finish.InsertColumn(i, ColumnName);//插入第1列
//			
//		}
//		
//		m_list_Finish.SetColumnWidth(0, 70);
//		for(int j = 1; j <= right_num; j++)
//		{
//			m_list_Finish.SetColumnWidth(j, 60);//设置列的宽
//		}
//		
//		for (i = 0; i < 99; i++)
//		{
//			//读字段的值
//			ColumnName.Format("%d", i);
//			
//			m_list_Finish.InsertItem(i,0);//插入一行
//			m_list_Finish.SetItemText(i,0,ColumnName);//其中的Counter为行号；0为列号即字段号
//			int k;
//			for(k = 1; k <= right_num; k++)
//			{
//				ColumnName.Format("%.3f", right_re[k-1][i]);
//				m_list_Finish.SetItemText(i,k,ColumnName);//其中的Counter为行号；0为列号即字段号
//			}		
//		}
//		if(right_re != NULL)
//		{
//			for (k=0;k<right_num;k++)
//			{
//				delete []right_re[k];
//				right_re[k] = NULL;
//			}
//			delete []right_re;
//			right_re = NULL;
//		}
//		for ( i=0;i<m_RightCalibration.CoordSeq.GetSize();i++)
//		{
//			Image_Coordation*	pIC;
//			pIC=(Image_Coordation*)m_RightCalibration.CoordSeq.GetAt(i);
//			delete pIC;
//			pIC=NULL;
//		}
//		m_RightCalibration.CoordSeq.RemoveAll();
//		return true;
//	}
//
//   return false;
//}

void CBSVDlg::OnBnClickedCalibration()
{
	 //TODO: 在此添加控件通知处理程序代码
	CString a, b ;
	a.Format(_T("LCamera_calibdata.txt"));
	b.Format(_T("LCamera_caliberation_result.txt"));
	string A((LPCTSTR)a);
	string B((LPCTSTR)b);

	m_Calibration.Initial();
	CCali_ParaDlg dlg;
	dlg.DoModal();
	/*在这一段应该加一段判断语句，如果数据没有输入完全就不执行下面的标定语句*/
	if(m_Calibration.RowCornerNum==0|m_Calibration.ColumCornerNum==0|m_Calibration.GridWidth==0|m_Calibration.GridHeight==0)
	{
		AfxMessageBox(_T("未完整输入参数"));
	}
	else
	{
		CalibrationLeft(A,B,m_Calibration.RowCornerNum,m_Calibration.ColumCornerNum,m_Calibration.GridWidth,m_Calibration.GridHeight);
	}
}

void CBSVDlg::OnBnClickedCalibration2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString a, b ;
	a.Format(_T("RCamera_calibdata.txt"));
	b.Format(_T("RCamera_caliberation_result.txt"));
	string A((LPCTSTR)a);
	string B((LPCTSTR)b);

	CCali_ParaDlg dlg;
	dlg.DoModal();
	CalibrationLeft(A,B,m_Calibration.RowCornerNum,m_Calibration.ColumCornerNum,m_Calibration.GridWidth,m_Calibration.GridHeight);
}

void CBSVDlg::OnBnClickedSavepic()
{
	// TODO: 在此添加控件通知处理程序代码
	//保存图像在Ori_LImage中，并为图片命名Lchess*.bmp
	if(m_LeftCamera.CameraHandle == NULL)
	{
		AfxMessageBox("没有打开左相机");
		return;
	}
	m_LeftCamera.ImageIndex++;

	if( m_LeftCamera.PixelFormat != PixelFormat_Mono8 )
		return;
	Mat LImg;
	LImg.create(m_LeftImage.OriHeight, m_LeftImage.OriWidth, CV_8UC1);

	g_pBSVDlg->m_LeftCamera.SectionCopy.Lock();
	memcpy(LImg.data,m_LeftImage.ShowDIBBits,m_LeftImage.OriWidth*m_LeftImage.OriHeight*(m_LeftImage.Bit/8));
	g_pBSVDlg->m_LeftCamera.SectionCopy.Unlock();

	std::stringstream StrStm;
	string imageFileName;
	StrStm.clear();
 	imageFileName.clear();
	string LfilePath="Ori_LImage\\chess";
	StrStm << m_LeftCamera.ImageIndex;
 	StrStm>>imageFileName;
	LfilePath += imageFileName;
	LfilePath+= ".bmp";
	imwrite(LfilePath,LImg);
}

void CBSVDlg::OnBnClickedSavepic2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_RightCamera.CameraHandle == NULL)
	{
		AfxMessageBox("没有打开右相机");
		return;
	}
	m_RightCamera.ImageIndex++;

	if( m_RightCamera.PixelFormat != PixelFormat_Mono8 )
				return;
	Mat RImg;
	RImg.create(m_RightImage.OriHeight, m_RightImage.OriWidth, CV_8UC1);

	g_pBSVDlg->m_RightCamera.SectionCopy.Lock();
	memcpy(RImg.data,m_RightImage.ShowDIBBits,m_RightImage.OriWidth*m_RightImage.OriHeight*(m_RightImage.Bit/8));
	g_pBSVDlg->m_RightCamera.SectionCopy.Unlock();

	std::stringstream StrStm;
	string imageFileName;
	StrStm.clear();
 	imageFileName.clear();
	string RfilePath="Ori_RImage\\chess";
	StrStm << m_RightCamera.ImageIndex;
 	StrStm>>imageFileName;
	RfilePath += imageFileName;
	RfilePath+= ".bmp";
	imwrite(RfilePath,RImg);
}

//void CBSVDlg::OnBnClickedButton2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	
//}

void Image::Initial()
{
	InitialBmpInfo();
	OriDIBBits = NULL;
	DIBBits = NULL;
	ShowDIBBits = NULL;
	Width = 0;
	Height = 0;
	RectLeft = 0;
	RectBottom = 0;
	Bit = 8;
	OriWidth = 1600;
	OriHeight = 1200;
	BufHandle = false;
}

void Image::InitialBmpInfo()
{
	BmpInfo = (LPBITMAPINFO) new
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];		//设置图像信息头
	BmpInfo->bmiHeader.biBitCount = 8;
	BmpInfo->bmiHeader.biClrImportant = 0;
	BmpInfo->bmiHeader.biClrUsed = 0;
	BmpInfo->bmiHeader.biCompression = BI_RGB;
	BmpInfo->bmiHeader.biPlanes = 1;
	BmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BmpInfo->bmiHeader.biXPelsPerMeter = 0;
	BmpInfo->bmiHeader.biYPelsPerMeter = 0;
	BmpInfo->bmiHeader.biSizeImage = 0;
	BmpInfo->bmiHeader.biHeight = OriHeight;
	BmpInfo->bmiHeader.biWidth = OriWidth;
	for (int i = 0; i < 256; i++)
	{
		BmpInfo->bmiColors[i].rgbBlue = i;
		BmpInfo->bmiColors[i].rgbGreen = i;
		BmpInfo->bmiColors[i].rgbRed = i;
		BmpInfo->bmiColors[i].rgbReserved = 0;
	}
}

void Camera::Initial()
{
	CameraHandle = NULL;
	ImageIndex = 0;
}

void Calibration::Initial()
{
	RowCornerNum = 0;
	ColumCornerNum = 0;
	GridWidth = 0;
	GridHeight = 0;
}
