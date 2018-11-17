
// BSVDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "BSV.h"
#include "BSVDlg.h"
#include "afxdialogex.h"
#include "math.h"
#include "opencv2\core\core.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
//#include "Calibration.h"
#include "ConerDetect.h"
#include "Stereo.h"
#include "Cali_ParaDlg.h"
#include <iostream>
#include "global.h"


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
	, m_ObjectDistance(0)
	, m_ObjectHeight(0)
	, m_ObjectWidth(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	/*子窗口初始化*/
	//m_childdlg1 = NULL;
	//m_childdlg2 = NULL;
	//m_childdlg3 = NULL;

	/*项目路径初始化(相对路径)*/
	ProjectPath.Format(_T("..\\BSV\\"));
	/*左相机变量初始化*/
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pBSVDlg = this;
	 
	m_pLCamera = NULL;
	m_bLBufHandle = false;

	m_LeftOriDIBBits = NULL;
	m_LeftDIBBits = NULL;
	m_LeftShowDIBBits = NULL;

	m_LeftWidth = 0;      
	m_LeftHeight = 0;     
	m_LeftOriWidth = 1600;  
	m_LeftOriHeight = 1200;

	m_nLBit = 8;
	m_pLBmpInfo = NULL;//左图像信息头指针
	m_pLBmpInfo = (LPBITMAPINFO) new 
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];		//设置图像信息头
	m_pLBmpInfo->bmiHeader.biBitCount = 8;
	m_pLBmpInfo->bmiHeader.biClrImportant = 0;
	m_pLBmpInfo->bmiHeader.biClrUsed = 0;
	m_pLBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pLBmpInfo->bmiHeader.biPlanes = 1;
	m_pLBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pLBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pLBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pLBmpInfo->bmiHeader.biSizeImage = 0;
	m_pLBmpInfo->bmiHeader.biHeight = m_LeftOriHeight;
	m_pLBmpInfo->bmiHeader.biWidth = m_LeftOriWidth;
	for(int i= 0;i<256;i++)
	{
		m_pLBmpInfo->bmiColors[i].rgbBlue = i;
		m_pLBmpInfo->bmiColors[i].rgbGreen = i;
		m_pLBmpInfo->bmiColors[i].rgbRed = i;
		m_pLBmpInfo->bmiColors[i].rgbReserved = 0;
	}
	m_nLImageIndex = 0;
	/*右相机变量初始化*/
	m_pRCamera = NULL;
	m_bRBufHandle = false;

	m_RightOriDIBBits = NULL;
	m_RightDIBBits = NULL;
	m_RightShowDIBBits = NULL;

	m_RightWidth = 0;      
	m_RightHeight = 0;     
	m_RightOriWidth = 1600;  
	m_RightOriHeight = 1200;

	m_nRBit = 8;

	m_pRBmpInfo = (LPBITMAPINFO) new 
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];		//设置图像信息头
	m_pRBmpInfo->bmiHeader.biBitCount = 8;
	m_pRBmpInfo->bmiHeader.biClrImportant = 0;
	m_pRBmpInfo->bmiHeader.biClrUsed = 0;
	m_pRBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pRBmpInfo->bmiHeader.biPlanes = 1;
	m_pRBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pRBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pRBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pRBmpInfo->bmiHeader.biSizeImage = 0;
	m_pRBmpInfo->bmiHeader.biHeight = m_RightOriHeight;
	m_pRBmpInfo->bmiHeader.biWidth = m_RightOriWidth;
	for(int i= 0;i<256;i++)
	{
		m_pRBmpInfo->bmiColors[i].rgbBlue = i;
		m_pRBmpInfo->bmiColors[i].rgbGreen = i;
		m_pRBmpInfo->bmiColors[i].rgbRed = i;
		m_pRBmpInfo->bmiColors[i].rgbReserved = 0;
	}
	m_nRImageIndex = 0;

	/*右相机变量初始化*/
	string Cascade_mode ="..\\BSV\\cascade.xml"; 
	string window_name = "Face"; 
}

CBSVDlg::~CBSVDlg()
{
	OnClose();
}

void CBSVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ObjectDistance, m_ObjectDistance);
	DDX_Text(pDX, IDC_ObjectHeight, m_ObjectHeight);
	DDX_Text(pDX, IDC_ObjectWidth, m_ObjectWidth);
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
	ON_BN_CLICKED(IDC_SavePic, &CBSVDlg::OnBnClickedSavepic)
	ON_BN_CLICKED(IDC_SavePic2, &CBSVDlg::OnBnClickedSavepic2)
	ON_BN_CLICKED(IDC_Calibration2, &CBSVDlg::OnBnClickedCalibration2)
	ON_BN_CLICKED(IDC_CornerDetect, &CBSVDlg::OnBnClickedCornerdetect)
	ON_BN_CLICKED(IDC_CornerDetect2, &CBSVDlg::OnBnClickedCornerdetect2)
	ON_BN_CLICKED(IDC_StereoCalibration, &CBSVDlg::OnBnClickedStereocalibration)
	ON_BN_CLICKED(IDC_StereoMatch, &CBSVDlg::OnBnClickedStereomatch)
	ON_BN_CLICKED(IDC_StereoMeasure, &CBSVDlg::OnBnClickedStereomeasure)
	ON_BN_CLICKED(IDC_StopCamera, &CBSVDlg::OnBnClickedStopcamera)
	ON_BN_CLICKED(IDC_StopCamera2, &CBSVDlg::OnBnClickedStopcamera2)
	ON_BN_CLICKED(IDC_Stop2Camera, &CBSVDlg::OnBnClickedStop2camera)
	ON_BN_CLICKED(IDC_Save2Pic, &CBSVDlg::OnBnClickedSave2pic)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_TABBUTTON1, &CBSVDlg::OnBnClickedTabbutton1)
	ON_BN_CLICKED(IDC_TABBUTTON2, &CBSVDlg::OnBnClickedTabbutton2)
	ON_BN_CLICKED(IDC_TABBUTTON3, &CBSVDlg::OnBnClickedTabbutton3)
	ON_BN_CLICKED(IDC_FaceDetect, &CBSVDlg::OnBnClickedFacedetect)
	ON_WM_SIZE()
//	ON_BN_CLICKED(IDC_BUTTON20, &CBSVDlg::OnBnClickedButton20)
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

	CDialogEx::SetBackgroundColor(RGB(230,230,230)); 
	//打开控制台
	AllocConsole();
	freopen("CONOUT$", "a+", stdout);

	//创建子窗口
	//m_childdlg1 = new CChildDlg1;
	//m_childdlg1->Create(IDD_DIALOG1, GetDlgItem(IDC_MENUFORM));

	//m_childdlg2 = new CChildDlg2;
	//m_childdlg2->Create(IDD_DIALOG2, GetDlgItem(IDC_MENUFORM));

	//m_childdlg3 = new CChildDlg3;
	//m_childdlg3->Create(IDD_DIALOG3, GetDlgItem(IDC_MENUFORM));
	m_childdlg1.Create(IDD_DIALOG1, GetDlgItem(IDC_MENUFORM));
	m_childdlg2.Create(IDD_DIALOG2, GetDlgItem(IDC_MENUFORM));
	m_childdlg3.Create(IDD_DIALOG3, GetDlgItem(IDC_MENUFORM));
	m_Childdlg1.Create(IDD_ChildDlg1, GetDlgItem(IDC_MENUFORM));
	//CChildDialog1.Create(IDD_ChildDlg1, GetDlgItem(IDC_MENUFORM));
	//CRect rect;
	//GetDlgItem(IDC_MENUFORM)->GetWindowRect(&rect);
	//m_Childdlg1.MoveWindow(rect);


	//固定Picture Control控件的大小
	CRect rectL;  
	GetDlgItem(IDC_LeftPic)->GetWindowRect(&rectL);
	ScreenToClient(&rectL);  
	GetDlgItem(IDC_LeftPic)->MoveWindow(rectL.left, rectL.top, 520, 390, true);

	CRect rectR;  
	GetDlgItem(IDC_RightPic)->GetWindowRect(&rectR);
	ScreenToClient(&rectR);  
	GetDlgItem(IDC_RightPic)->MoveWindow(rectR.left, rectR.top, 520, 390, true);

	//获取对话框创建时的大小（用于窗体放大尺度不变）
	GetClientRect(&m_rect);

	//设置按钮控件初始可操作状态
	GetDlgItem(IDC_CapVideo)->EnableWindow(false);
	GetDlgItem(IDC_SetCamera)->EnableWindow(false);
	GetDlgItem(IDC_StopDetect)->EnableWindow(false);
	GetDlgItem(IDC_StopCamera)->EnableWindow(false);

	GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
	GetDlgItem(IDC_SetCamera2)->EnableWindow(false);
	GetDlgItem(IDC_StopDetect2)->EnableWindow(false);
	GetDlgItem(IDC_StopCamera2)->EnableWindow(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBSVDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//释放内存资源
	if (m_pLCamera != NULL)
	{
		MVStopGrab(m_pLCamera);
		MVCloseCam(m_pLCamera);
		m_pLCamera = NULL;
	}

	delete[] m_RightShowDIBBits;
	m_LeftShowDIBBits = NULL;	
	delete[] m_LeftDIBBits;
	m_LeftDIBBits = NULL;
	delete[] m_LeftOriDIBBits;
	m_LeftOriDIBBits = NULL;

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

	if (m_pLBmpInfo  != NULL)
	{
		delete[] m_pLBmpInfo;
		m_pLBmpInfo = NULL;
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
 *函数说明：左相机回调函数在相机工作在连续与触发模式下时，每获得一帧图像就会调用该函数说明。
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
	if(g_pBSVDlg->m_LeftOriDIBBits != NULL)
	{
		//拷贝图像内存
		g_pBSVDlg->m_CSectionCopyL.Lock();

		//为原始图像赋值，如果是24位RGB格式，则直接转换
		if( g_pBSVDlg->m_LPixelFormat == PixelFormat_Mono8 )
		{
			memcpy(g_pBSVDlg->m_LeftOriDIBBits,pInfo->pImageBuffer,
				g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_LeftOriHeight);
		}
		else
		{
			int count = (((g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_nLBit)+31)/32)*4;
			MVBayerToBGR(g_pBSVDlg->m_pLCamera,pInfo->pImageBuffer,g_pBSVDlg->m_LeftOriDIBBits,count,
				g_pBSVDlg->m_LeftOriWidth,g_pBSVDlg->m_LeftOriHeight,g_pBSVDlg->m_LPixelFormat);
		}

		LPSTR itemBits;
		itemBits = new char[g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_LeftOriHeight*(g_pBSVDlg->m_nLBit/8)];
		memcpy(itemBits,g_pBSVDlg->m_LeftOriDIBBits,g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_LeftOriHeight*(g_pBSVDlg->m_nLBit/8)); 
		for (int i=0;i<g_pBSVDlg->m_LeftOriHeight;i++)
		{
			for (int j=0;j<g_pBSVDlg->m_LeftOriWidth;j++)
			{
				if (g_pBSVDlg->m_nLBit == 8)
				{
					g_pBSVDlg->m_LeftShowDIBBits[i*g_pBSVDlg->m_LeftOriWidth+j] = 
						itemBits[(g_pBSVDlg->m_LeftOriHeight-i-1)*g_pBSVDlg->m_LeftOriWidth+j];
				}
				else if (g_pBSVDlg->m_nLBit == 24)
				{
					g_pBSVDlg->m_LeftShowDIBBits[(i*g_pBSVDlg->m_LeftOriWidth+j)*3] = 
						itemBits[((g_pBSVDlg->m_LeftOriHeight-i-1)*g_pBSVDlg->m_LeftOriWidth+j)*3];
					g_pBSVDlg->m_LeftShowDIBBits[(i*g_pBSVDlg->m_LeftOriWidth+j)*3+1] = 
						itemBits[((g_pBSVDlg->m_LeftOriHeight-i-1)*g_pBSVDlg->m_LeftOriWidth+j)*3+1];
					g_pBSVDlg->m_LeftShowDIBBits[(i*g_pBSVDlg->m_LeftOriWidth+j)*3+2] = 
						itemBits[((g_pBSVDlg->m_LeftOriHeight-i-1)*g_pBSVDlg->m_LeftOriWidth+j)*3+2];
				}
			}
		}
		delete []itemBits;
		g_pBSVDlg->m_CSectionCopyL.Unlock();

		g_pBSVDlg->m_bLBufHandle = true;//左图像可以进行处理标识
		g_pBSVDlg->DrawLeftCamera();
	}
	return 1;
}

/*
 *函数说明：右相机回调函数。
 */
int RightCallbackFunction(MV_IMAGE_INFO *pInfo,long nUserVal)
	// int index, LPSTR pData, int Length, void* pUserData )
{
	if(g_pBSVDlg->m_RightOriDIBBits != NULL)//有问题
	{
		//拷贝图像内存
		g_pBSVDlg->m_CSectionCopyR.Lock();

		//为原始图像赋值，如果是24位RGB格式，则直接转换
		if( g_pBSVDlg->m_RPixelFormat == PixelFormat_Mono8 )
		{
			memcpy(g_pBSVDlg->m_RightOriDIBBits,pInfo->pImageBuffer,
				g_pBSVDlg->m_RightOriWidth*g_pBSVDlg->m_RightOriHeight);
		}
		else
		{
			int count = (((g_pBSVDlg->m_RightOriWidth*g_pBSVDlg->m_nRBit)+31)/32)*4;
			MVBayerToBGR(g_pBSVDlg->m_pRCamera,pInfo->pImageBuffer,g_pBSVDlg->m_RightOriDIBBits,count,
				g_pBSVDlg->m_RightOriWidth,g_pBSVDlg->m_RightOriHeight,g_pBSVDlg->m_RPixelFormat);
		}

		LPSTR itemBits;
		itemBits = new char[g_pBSVDlg->m_RightOriWidth*g_pBSVDlg->m_RightOriHeight*(g_pBSVDlg->m_nRBit/8)];
		memcpy(itemBits,g_pBSVDlg->m_RightOriDIBBits,g_pBSVDlg->m_RightOriWidth*g_pBSVDlg->m_RightOriHeight*(g_pBSVDlg->m_nRBit/8)); 
		for (int i=0;i<g_pBSVDlg->m_RightOriHeight;i++)
		{
			for (int j=0;j<g_pBSVDlg->m_RightOriWidth;j++)
			{
				if (g_pBSVDlg->m_nRBit == 8)
				{
					g_pBSVDlg->m_RightShowDIBBits[i*g_pBSVDlg->m_RightOriWidth+j] = 
						itemBits[(g_pBSVDlg->m_RightOriHeight-i-1)*g_pBSVDlg->m_RightOriWidth+j];
				}
				else if (g_pBSVDlg->m_nRBit == 24)
				{
					g_pBSVDlg->m_RightShowDIBBits[(i*g_pBSVDlg->m_RightOriWidth+j)*3] = 
						itemBits[((g_pBSVDlg->m_RightOriHeight-i-1)*g_pBSVDlg->m_RightOriWidth+j)*3];
					g_pBSVDlg->m_RightShowDIBBits[(i*g_pBSVDlg->m_RightOriWidth+j)*3+1] = 
						itemBits[((g_pBSVDlg->m_RightOriHeight-i-1)*g_pBSVDlg->m_RightOriWidth+j)*3+1];
					g_pBSVDlg->m_RightShowDIBBits[(i*g_pBSVDlg->m_RightOriWidth+j)*3+2] = 
						itemBits[((g_pBSVDlg->m_RightOriHeight-i-1)*g_pBSVDlg->m_RightOriWidth+j)*3+2];
				}
			}
		}
		delete []itemBits;
		g_pBSVDlg->m_CSectionCopyR.Unlock();

		g_pBSVDlg->m_bRBufHandle = true;//左图像可以进行处理标识
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
	m_CSectionL.Lock();
	CDC *pdc = GetDlgItem(IDC_LeftPic)->GetDC();//获得DC
	CRect rc;
	GetDlgItem(IDC_LeftPic)->GetClientRect(&rc);//获得图像显示区域
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;//设置模式

	//显示图像
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(),rc.Height(),
		0,0,m_LeftOriWidth,m_LeftOriHeight,
		m_LeftShowDIBBits,(LPBITMAPINFO) m_pLBmpInfo, DIB_RGB_COLORS, SRCCOPY); 

	GetDlgItem(IDC_LeftPic)->ReleaseDC(pdc);	
	m_CSectionL.Unlock();
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
	m_CSectionR.Lock();
	CDC *pdc = GetDlgItem(IDC_RightPic)->GetDC();//获得DC
	CRect rc;
	GetDlgItem(IDC_RightPic)->GetClientRect(&rc);//获得图像显示区域
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;//设置模式

	//显示图像
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(),rc.Height(),
		0,0,m_RightOriWidth,m_RightOriHeight,
		m_RightShowDIBBits,(LPBITMAPINFO) m_pRBmpInfo, DIB_RGB_COLORS, SRCCOPY); 

	GetDlgItem(IDC_RightPic)->ReleaseDC(pdc);	
	m_CSectionR.Unlock();
}

/*
 *函数功能：响应定时器消息处理函数
 *参数说明：
 nIDEvent 定时器标识符，在一个窗体内可以使用多个定时器
          ，不同的定时器根据nIDEvent来区分。
*/
void CBSVDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)//nIDEvent 为定时器事件ID，1，2，3
	{
	case 1:	//左相机圆形检测
		{
			if( m_LPixelFormat != PixelFormat_Mono8 )
				return;

			Mat LSrcImg;
			LSrcImg.create(m_LeftOriHeight, m_LeftOriWidth, CV_8UC1);

			g_pBSVDlg->m_CSectionCopyL.Lock();
			memcpy(LSrcImg.data,m_LeftShowDIBBits,m_LeftOriWidth*m_LeftOriHeight*(m_nLBit/8));
			g_pBSVDlg->m_CSectionCopyL.Unlock();
	
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
			
			if (!m_LeftBlobSeq.IsEmpty())
			{
				ShowLeftCircles();
			}
			LSrcImg.release();
			LFilImg.release();
		}
		break;
	case 2:	//右相机圆形检测
		{
			if( m_RPixelFormat != PixelFormat_Mono8 )
				return;

			Mat RSrcImg;
			RSrcImg.create(m_RightOriHeight, m_RightOriWidth, CV_8UC1);
			g_pBSVDlg->m_CSectionCopyR.Lock();
			memcpy(RSrcImg.data,m_RightShowDIBBits,m_RightOriWidth*m_RightOriHeight*(m_nRBit/8));
			g_pBSVDlg->m_CSectionCopyR.Unlock();
	
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

			if (!m_RightBlobSeq.IsEmpty())
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

/*
 *函数功能：释放BlobSeq中的Blob数据内存，清除BlobSeq中的数据
 *参数说明
 CArray<Blob> m_LeftBlobSeq	待释放内存的左相机团块队列。
 */
/*左相机*/
void CBSVDlg::Blob_ReleaseLeftBlobSeq()
{
	if (!m_LeftBlobSeq.IsEmpty())
	{
		m_LeftBlobSeq.RemoveAll();
		m_LeftBlobSeq.FreeExtra();
	}
}

/*
 *函数功能：释放BlobSeq中的Blob数据内存，清除BlobSeq中的数据
 *参数说明
 CArray<Blob> m_RightBlobSeq	待释放内存的左相机团块队列。
 */
/*右相机*/
void CBSVDlg::Blob_ReleaseRightBlobSeq()
{
	if (!m_RightBlobSeq.IsEmpty())
	{
		m_RightBlobSeq.RemoveAll();
		m_RightBlobSeq.FreeExtra();
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
/*左相机*/
void CBSVDlg::Detect_LeftCircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity)
{
	Mat EdgeImg;
	Canny(SrcImg, EdgeImg, lowthresh, highthresh);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(EdgeImg, contours, hierarchy, CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE, cvPoint(0,0));
	
	//画出检测到的轮廓轮廓
	Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
	drawContours ( EdgeImg, contours,-1,color);
	namedWindow("检测轮廓",WINDOW_NORMAL);
	imshow("检测轮廓",EdgeImg);
	waitKey(500);
	EdgeImg.release();

	double area, length, cir, width, height, asp;
	RotatedRect rect;
	int cnt = 0;
	int ln = contours.size();
	m_LeftBlobSeq.SetSize(ln,10);
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

					m_LeftBlobSeq.SetAtGrow(cnt,pB);
					cnt++;

					//提取圆心值
					LBlob.BlobX = pB.BlobX;
					LBlob.BlobY = pB.BlobY;
				}
			}
		}
	}
	m_LeftBlobSeq.FreeExtra();
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
/*右相机*/
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
	m_RightBlobSeq.SetSize(ln,10);
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

					m_RightBlobSeq.SetAtGrow(cnt,pB);
					cnt++;
				}
			}
		}
	}
	m_RightBlobSeq.FreeExtra();
}

/*
 *函数功能：清除团块队列中面积不在指定范围内的团块数据
 *参数说明
 CArray<Blob> m_LeftBlobSeq     待处理的团块队列
 int MinArea,MaxArea    指定的团块面积范围
*/
/*左相机*/
void CBSVDlg::Blob_DenoisingLeftArea(int MinArea, int MaxArea)
{
	if (!m_LeftBlobSeq.IsEmpty())
	{
		int ln = m_LeftBlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = m_LeftBlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				m_LeftBlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		m_LeftBlobSeq.FreeExtra();
	}
}

/*
 *函数功能：清除团块队列中面积不在指定范围内的团块数据
 *参数说明
 CArray<Blob> m_RightBlobSeq     待处理的团块队列
 int MinArea,MaxArea    指定的团块面积范围
*/
/*右相机*/
void CBSVDlg::Blob_DenoisingRightArea(int MinArea, int MaxArea)
{
	if (!m_RightBlobSeq.IsEmpty())
	{
		int ln = m_RightBlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = m_RightBlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				m_RightBlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		m_RightBlobSeq.FreeExtra();
	}
}

/*
 *函数功能：在图像上绘出所检测圆形标志
 *参数说明
*/
/*左相机*/
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

	double dbRateX = (double)rc.Width()/(double)m_LeftOriWidth;
	double dbRateY = (double)rc.Height()/(double)m_LeftOriHeight;
	int ln = m_LeftBlobSeq.GetSize();
	for (int i=0;i<ln;i++)
	{
		Blob pB = m_LeftBlobSeq.GetAt(i);
		int x, y;
		x = (int)(pB.BlobX*dbRateX);
		y = (int)((m_LeftOriHeight - pB.BlobY)*dbRateY);
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
		pdc->Ellipse(x-5, y-5, x+5, y+5);//绘出圆形	
	}

	pdc->SelectObject(pOldBrush);
	BrushRed.DeleteObject();
	ReleaseDC(pdc);
}

/*
 *函数功能：在图像上绘出所检测圆形标志
 *参数说明
*/
/*右相机*/
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

	double dbRateX = (double)rc.Width()/(double)m_RightOriWidth;
	double dbRateY = (double)rc.Height()/(double)m_RightOriHeight;
	int ln = m_RightBlobSeq.GetSize();
	for (int i=0;i<ln;i++)
	{
		Blob pB = m_RightBlobSeq.GetAt(i);
		int x, y;
		x = (int)(pB.BlobX*dbRateX);
		y = (int)((m_RightOriHeight - pB.BlobY)*dbRateY);
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

/*
 *函数说明：以下皆为控件消息相应函数
 *输入参数：无       
 *输出参数：无。
 *返 回 值：无。
*/
void CBSVDlg::OnBnClickedOpencamera()
{
	// TODO: 在此添加控件通知处理程序代码
	//启动左相机	
	MVSTATUS_CODES hr;
	if (m_pLCamera == NULL)	//启动相机
	{
		hr = MVOpenCamByIndex(0, &m_pLCamera );		
		if (hr != MVST_SUCCESS)
		{
			MessageBox("未找到相机，请确认设备连接和IP设置！","BSV",MB_ICONWARNING);
			return;
		}
		else if (hr == MVST_SUCCESS)
		{
			GetDlgItem(IDC_OpenCamera)->SetWindowText("关闭相机");
			GetDlgItem(IDC_CapVideo)->EnableWindow(true);
			GetDlgItem(IDC_SetCamera)->EnableWindow(true);
			GetDlgItem(IDC_StopCamera)->EnableWindow(false);
		}
	}
	else	//关闭相机
	{
		hr = MVStopGrab(m_pLCamera);		 
		hr = MVCloseCam(m_pLCamera);	
		m_pLCamera = NULL;

		GetDlgItem(IDC_OpenCamera)->SetWindowText("启动相机");
		GetDlgItem(IDC_CapVideo)->EnableWindow(false);
		GetDlgItem(IDC_SetCamera)->EnableWindow(false);
	}
}


void CBSVDlg::OnBnClickedCapvideo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pLCamera != NULL)
	{
		MVGetWidth(m_pLCamera, &m_LeftOriWidth);  //获得图像长宽
		MVGetHeight(m_pLCamera,&m_LeftOriHeight);
		MVGetPixelFormat(m_pLCamera,&m_LPixelFormat);  //获得数据格式
		if( m_LPixelFormat == PixelFormat_Mono8 )
		{
			m_nLBit = 8;
		}
		else
		{
			m_nLBit = 24;
		}
		
		MVSTATUS_CODES hr;
		hr =  MVStartGrab(m_pLCamera, LeftCallbackFunction, (long)this); //设置回调函数
		if (hr == MVST_SUCCESS)
		{	
			//原始图像
			m_pLBmpInfo->bmiHeader.biBitCount = m_nLBit;
			m_pLBmpInfo->bmiHeader.biWidth = m_LeftOriWidth;
			m_pLBmpInfo->bmiHeader.biHeight = m_LeftOriHeight;

			//为图像显示的图像数据分配内存空间
			delete[] m_LeftShowDIBBits;	
			m_LeftShowDIBBits = NULL;	
			m_LeftShowDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];

			delete[] m_LeftOriDIBBits;	
			m_LeftOriDIBBits = NULL;						
			m_LeftOriDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];

			//为进行处理的图像赋值
			m_LeftRectLeft=0;  
			m_LeftRectBottom=0;
			m_LeftWidth=m_LeftOriWidth; 
			m_LeftHeight=m_LeftOriHeight;

			delete[] m_LeftDIBBits;	
			m_LeftDIBBits = NULL;					
			m_LeftDIBBits = new char[m_LeftWidth * m_LeftHeight * (m_nLBit/8)];

			//开始采集图像后，不再接受更新的“连续采集”指令
			GetDlgItem(IDC_CapVideo)->EnableWindow(false);
			//GetDlgItem(IDC_Btn_SetCamera)->EnableWindow(false);
		}
	}
}


void CBSVDlg::OnBnClickedSetcamera()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pLCamera == NULL )
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
	if (m_LeftShowDIBBits == NULL)
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


void CBSVDlg::OnBnClickedOpencamera2()
{
	// TODO: 在此添加控件通知处理程序代码
	//启动右相机	
	MVSTATUS_CODES hr;
	if (m_pRCamera == NULL)	//启动相机
	{
		hr = MVOpenCamByIndex(1, &m_pRCamera );		
		if (hr != MVST_SUCCESS)
		{
			MessageBox("未找到相机，请确认设备连接和IP设置！","BSV",MB_ICONWARNING);
			return;
		}
		else if (hr == MVST_SUCCESS)
		{
			GetDlgItem(IDC_OpenCamera2)->SetWindowText("关闭相机");
			GetDlgItem(IDC_CapVideo2)->EnableWindow(true);
			GetDlgItem(IDC_SetCamera2)->EnableWindow(true);
			GetDlgItem(IDC_StopCamera2)->EnableWindow(false);
		}
	}
	else	//关闭相机
	{
		hr = MVStopGrab(m_pRCamera);		 
		hr = MVCloseCam(m_pRCamera);	
		m_pRCamera = NULL;

		GetDlgItem(IDC_OpenCamera2)->SetWindowText("启动相机");
		GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
		GetDlgItem(IDC_SetCamera2)->EnableWindow(false);
	}
}


void CBSVDlg::OnBnClickedCapvideo2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pRCamera != NULL)
	{
		MVGetWidth(m_pRCamera, &m_RightOriWidth);  //获得图像长宽
		MVGetHeight(m_pRCamera,&m_RightOriHeight);
		MVGetPixelFormat(m_pRCamera,&m_RPixelFormat);  //获得数据格式
		if( m_RPixelFormat == PixelFormat_Mono8 )
		{
			m_nRBit = 8;
		}
		else
		{
			m_nRBit = 24;
		}
		
		MVSTATUS_CODES hr2;
		hr2 =  MVStartGrab(m_pRCamera, RightCallbackFunction, (long)this); //设置回调函数
		if (hr2 == MVST_SUCCESS)
		{	
			//原始图像
			m_pRBmpInfo->bmiHeader.biBitCount = m_nRBit;
			m_pRBmpInfo->bmiHeader.biWidth = m_RightOriWidth;
			m_pRBmpInfo->bmiHeader.biHeight = m_RightOriHeight;

			//为图像显示的图像数据分配内存空间
			delete[] m_RightShowDIBBits;	
			m_RightShowDIBBits = NULL;	
			m_RightShowDIBBits = new char[m_RightOriWidth * m_RightOriHeight * (m_nRBit/8)];

			delete[] m_RightOriDIBBits;	
			m_RightOriDIBBits = NULL;						
			m_RightOriDIBBits = new char[m_RightOriWidth * m_RightOriHeight * (m_nRBit/8)];

			//为进行处理的图像赋值
			m_RightRectLeft=0;
			m_RightRectBottom=0;
			m_RightWidth=m_RightOriWidth; 
			m_RightHeight=m_RightOriHeight;

			delete[] m_RightDIBBits;	
			m_RightDIBBits = NULL;					
			m_RightDIBBits = new char[m_RightWidth * m_RightHeight * (m_nRBit/8)];

			//开始采集图像后，不再接受更新的“连续采集”指令
			GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
			//GetDlgItem(IDC_Btn_SetCamera)->EnableWindow(false);
		}
	}
}


void CBSVDlg::OnBnClickedSetcamera2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pRCamera == NULL )
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
	//OnBnClickedCapvideo();
	OnBnClickedCapvideo2();
}


void CBSVDlg::OnBnClickedCircledetect2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_RightShowDIBBits == NULL)
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


/*这段代码貌似可以去掉*/
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


void CBSVDlg::OnBnClickedCalibration()
{
	 //TODO: 在此添加控件通知处理程序代码
	//CString a, b ;
	//a.Format(_T("%s\\LCamera_calibdata.txt"),ProjectPath);
	//b.Format(_T("%s\\LCamera_caliberation_result.txt"),ProjectPath);
	//string A((LPCTSTR)a);
	//string B((LPCTSTR)b);

	//row_corner_num = 0;
	//column_corner_num = 0;
	//grid_width = 0;
	//grid_height = 0;
	//PicNum==0;
	//CCali_ParaDlg dlg;
	//dlg.DoModal();
	//if(row_corner_num==0|column_corner_num==0|grid_width==0|grid_height==0|PicNum==0)
	//{
	//	AfxMessageBox(_T("请将参数填写完整！"));
	//}
	//else
	//{
	//	WriteTxt(A,PicNum);
	//	m_LImgPath="Ori_LImage\\chess";
	//	Calibration(A,B,row_corner_num,column_corner_num,grid_width,grid_height,m_LImgPath,1);
	//}
}


void CBSVDlg::OnBnClickedCalibration2()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString a,b;
	//a.Format(_T("%s\\RCamera_calibdata.txt"),ProjectPath);
	//b.Format(_T("%s\\RCamera_caliberation_result.txt"),ProjectPath);
	//string A((LPCTSTR)a);
	//string B((LPCTSTR)b);

	//row_corner_num = 0;
	//column_corner_num = 0;
	//grid_width = 0;
	//grid_height = 0;
	//PicNum==0;
	//CCali_ParaDlg dlg;
	//dlg.DoModal();
	//if(row_corner_num==0|column_corner_num==0|grid_width==0|grid_height==0|PicNum==0)
	//{
	//	AfxMessageBox(_T("请将参数填写完整！"));
	//}
	//else
	//{
	//	WriteTxt2(A,PicNum);
	//	m_RImgPath="Ori_RImage\\chess";
	//	Calibration(A,B,row_corner_num,column_corner_num,grid_width,grid_height,m_RImgPath,2);
	//}
}


void CBSVDlg::OnBnClickedSavepic()
{
	// TODO: 在此添加控件通知处理程序代码
	//保存图像在Ori_LImage中，并为图片命名Lchess*.bmp
	if(m_pLCamera == NULL)
	{
		AfxMessageBox("没有打开左相机");
		return;
	}
	m_nLImageIndex ++;

	if( m_LPixelFormat != PixelFormat_Mono8 )
		return;
	Mat LImg;
	LImg.create(m_LeftOriHeight, m_LeftOriWidth, CV_8UC1);

	g_pBSVDlg->m_CSectionCopyL.Lock();
	memcpy(LImg.data,m_LeftShowDIBBits,m_LeftOriWidth*m_LeftOriHeight*(m_nLBit/8));
	g_pBSVDlg->m_CSectionCopyL.Unlock();

	std::stringstream StrStm;
	string imageFileName;
	StrStm.clear();
 	imageFileName.clear();
	string LfilePath=ProjectPath+"Ori_LImage\\chess";
	StrStm<<m_nLImageIndex;
 	StrStm>>imageFileName;
	LfilePath += imageFileName;
	LfilePath+= ".bmp";
	imwrite(LfilePath,LImg);
}


void CBSVDlg::OnBnClickedSavepic2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pRCamera == NULL)
	{
		AfxMessageBox("没有打开右相机");
		return;
	}
	m_nRImageIndex ++;

	if( m_RPixelFormat != PixelFormat_Mono8 )
				return;
	Mat RImg;
	RImg.create(m_RightOriHeight, m_RightOriWidth, CV_8UC1);

	g_pBSVDlg->m_CSectionCopyR.Lock();
	memcpy(RImg.data,m_RightShowDIBBits,m_RightOriWidth*m_RightOriHeight*(m_nRBit/8));
	g_pBSVDlg->m_CSectionCopyR.Unlock();

	std::stringstream StrStm;
	string imageFileName;
	StrStm.clear();
 	imageFileName.clear();
	string RfilePath=ProjectPath+"Ori_RImage\\chess";
	StrStm<<m_nRImageIndex;
 	StrStm>>imageFileName;
	RfilePath += imageFileName;
	RfilePath+= ".bmp";
	imwrite(RfilePath,RImg);
}


void CBSVDlg::StopLeftCamera()
{
	MVSTATUS_CODES status;
	if (m_pLCamera != NULL)
	{
		status = MVStopGrab(m_pLCamera);		 
        status = MVCloseCam(m_pLCamera);	
		m_pLCamera = NULL;
	}
}


void CBSVDlg::StopRightCamera()
{
	MVSTATUS_CODES status;
	if (m_pRCamera != NULL)
	{
		status = MVStopGrab(m_pRCamera);
		status = MVCloseCam(m_pRCamera);
		m_pRCamera = NULL;
	}	
}


void CBSVDlg::OnBnClickedCornerdetect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_LeftShowDIBBits == NULL)
	{
		MessageBox("无图像数据！","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		if( m_LPixelFormat != PixelFormat_Mono8 )
			return;
		Mat LSrcImg;
		LSrcImg.create(m_LeftOriHeight, m_LeftOriWidth, CV_8UC1);
		g_pBSVDlg->m_CSectionCopyL.Lock();
		memcpy(LSrcImg.data,m_LeftShowDIBBits,m_LeftOriWidth*m_LeftOriHeight*(m_nLBit/8));
		g_pBSVDlg->m_CSectionCopyL.Unlock();

		string LfilePath=ProjectPath+"LCornerDetect.bmp";
		imwrite(LfilePath,LSrcImg);
		Mat LCatchImg = imread(LfilePath);
		CornerDetect(LCatchImg);
	}
}


void CBSVDlg::OnBnClickedCornerdetect2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_RightShowDIBBits == NULL)
	{
		MessageBox("无图像数据！","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		if( m_RPixelFormat != PixelFormat_Mono8 )
			return;
		Mat RSrcImg;
		RSrcImg.create(m_RightOriHeight, m_RightOriWidth, CV_8UC1);
		g_pBSVDlg->m_CSectionCopyR.Lock();
		memcpy(RSrcImg.data,m_RightShowDIBBits,m_RightOriWidth*m_RightOriHeight*(m_nRBit/8));
		g_pBSVDlg->m_CSectionCopyR.Unlock();

		string RfilePath=ProjectPath+"RCornerDetect.bmp";
		imwrite(RfilePath,RSrcImg);
		Mat RCatchImg = imread(RfilePath);
		CornerDetect(RCatchImg);
	}
}


/*该段代码中变量“1”代表左相机，变量“2”代表右相机*/
void CBSVDlg::OnBnClickedStereocalibration()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString a, b, c, d, SCaliFile;
	//a.Format(_T("%s\\LCamera_calibdata.txt"),ProjectPath);
	//b.Format(_T("%s\\LCamera_caliberation_result.txt"),ProjectPath);
	//string A((LPCTSTR)a);
	//string B((LPCTSTR)b);
	//c.Format(_T("%s\\RCamera_calibdata.txt"),ProjectPath);
	//d.Format(_T("%s\\RCamera_caliberation_result.txt"),ProjectPath);
	//string C((LPCTSTR)c);
	//string D((LPCTSTR)d);
	//SCaliFile.Format(_T("%s\\Stereo_caliberation_result.txt"),ProjectPath);

	//double err = 0;
	//row_corner_num = 0;
	//column_corner_num = 0;
	//grid_width = 0;
	//grid_height = 0;
	//PicNum==0;
	//CCali_ParaDlg dlg;
	//dlg.DoModal();
	//if(row_corner_num==0|column_corner_num==0|grid_width==0|grid_height==0|PicNum==0)
	//{
	//	AfxMessageBox(_T("请将参数填写完整！"));
	//}
	//else
	//{
	//	cout<<"***开始左相机标定***"<<endl;
	//	cout<<endl;
	//	WriteTxt(A,PicNum);
	//	m_LImgPath="Ori_LImage\\chess";
	//	Calibration(A,B,row_corner_num,column_corner_num,grid_width,grid_height,m_LImgPath,1);
	//	
	//	cout<<endl;
	//	cout<<"***开始右相机标定，稍候2秒***"<<endl;
	//	cout<<endl;
	//	waitKey(2000);

	//	WriteTxt2(C,PicNum);
	//	m_RImgPath="Ori_RImage\\chess";
	//	Calibration(C,D,row_corner_num,column_corner_num,grid_width,grid_height,m_RImgPath,2);
	//}

	////保存单相机参数
	//CString ymlFile1,ymlFile2;
	//ymlFile1.Format(_T("%s\\data\\cameraParams1.yml"),ProjectPath);
	//ymlFile1.Format(_T("%s\\data\\cameraParams2.yml"),ProjectPath);
	//m_StereoCalib.saveCameraData(m_cameraParams1, (LPCTSTR)ymlFile1);
	//m_StereoCalib.saveCameraData(m_cameraParams2, (LPCTSTR)ymlFile2);

	////开始双目标定
	//m_stereoParams.cameraParams1 = m_cameraParams1;
	//m_stereoParams.cameraParams2 = m_cameraParams2;

	//stereoCalibrate(m_cornersDatas.objectPoints, m_cornersDatas.imagePoints1, m_cornersDatas.imagePoints2,
	//	m_cameraParams1.cameraMatrix, m_cameraParams1.distortionCoefficients, m_cameraParams2.cameraMatrix,
	//	m_cameraParams2.distortionCoefficients, m_cornersDatas.imageSize, m_stereoParams.rotation,
	//	m_stereoParams.translation, m_stereoParams.essential, m_stereoParams.foundational,
	//	TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 1e-5), CV_CALIB_FIX_INTRINSIC
	//	);

	////计算双目标定误差
	//m_StereoCalib.getStereoCalibrateError(m_cornersDatas, m_stereoParams, err);
	//CString error;
	//error.Format(_T("双目标定误差为：%f"),err);
	//AfxMessageBox(error);

	////进行双目矫正
	//m_StereoCalib.mstereoRectify(m_cameraParams1, m_cameraParams2, m_stereoParams, m_remapMatrixs);

	////进行图片矫正
	//ifstream fin11(a);
	//ifstream fin12(c);
	//string str11, str22;
	//Mat img1r, img2r;
	//CString name11,name22;
	//int cnt = 0;
	//while (getline(fin11, str11) && getline(fin12, str22))
	//{
	//	cnt++;
	//	Mat image1 = imread(str11);
	//	Mat image2 = imread(str22);
	//	m_StereoCalib.remapImage(image1, image2, img1r, img2r, m_remapMatrixs);
	//	name11.Format(_T("%s\\Save_LImage\\Left_%d.jpg"),ProjectPath,cnt);
	//	name22.Format(_T("%s\\Save_RImage\\Right_%d.jpg"),ProjectPath,cnt);
	//	string Name11((LPCTSTR)name11);
	//	string Name22((LPCTSTR)name22);
	//	imwrite(Name11, img1r);
	//	imwrite(Name22, img2r);
	//}

	////对校正后的图像进行画线
	//string Name11((LPCTSTR)name11);
	//string Name22((LPCTSTR)name22);
	//Mat imgLr = imread(Name11);
	//Mat imgRr = imread(Name22);
	//Mat img(m_cornersDatas.imageSize.height*0.5, m_cornersDatas.imageSize.width, CV_8UC3);//创建IMG，高度一样，宽度双倍
	//Mat imgPart1 = img(Rect(0, 0, m_cornersDatas.imageSize.width*0.5, m_cornersDatas.imageSize.height*0.5));//浅拷贝
	//Mat imgPart2 = img(Rect(m_cornersDatas.imageSize.width*0.5, 0, m_cornersDatas.imageSize.width*0.5, m_cornersDatas.imageSize.height*0.5));//浅拷贝
	//resize(imgLr, imgPart1, imgPart1.size(), 0, 0, CV_INTER_AREA);
	//resize(imgRr, imgPart2, imgPart2.size(), 0, 0, CV_INTER_AREA);//改变图像尺寸，调节0,0；
	//for (int i = 0; i < img.rows; i += 16) //画横线
	//	line(img, Point(0, i), Point(img.cols, i), Scalar(0, 255, 0), 1, 8);
	//namedWindow("校正后的图像", WINDOW_NORMAL);
	//imshow("校正后的图像", img);

	////保存双目标定数据
	///*保存在txt文件中*/
	//ofstream StereoCaliResult(SCaliFile);
	//StereoCaliResult<<"左相机内参数矩阵：\n";	
	//StereoCaliResult<<m_cameraParams1.cameraMatrix<<'\n';	
	//StereoCaliResult<<"左相机畸变系数：\n";
	//StereoCaliResult<<m_cameraParams1.distortionCoefficients<<'\n';
	//StereoCaliResult<<"右相机内参数矩阵：\n";	
	//StereoCaliResult<<m_cameraParams2.cameraMatrix<<'\n';	
	//StereoCaliResult<<"右相机畸变系数：\n";
	//StereoCaliResult<<m_cameraParams2.distortionCoefficients<<'\n';
	//StereoCaliResult<<"左右相机旋转矩阵：\n";	
	//StereoCaliResult<<m_stereoParams.rotation<<'\n';	
	//StereoCaliResult<<"左右相机平移矩阵：\n";
	//StereoCaliResult<<m_stereoParams.translation<<'\n';
	//StereoCaliResult<<"本征矩阵：\n";
	//StereoCaliResult<<m_stereoParams.essential<<'\n';
	//StereoCaliResult<<"基础矩阵：\n";
	//StereoCaliResult<<m_stereoParams.foundational<<'\n';

	///*保存在yml文件中*/
	////CString ymlFile;
	////ymlFile.Format(_T("%s\\data\\CalibrationDatas.yml"),ProjectPath);
	////m_StereoCalib.saveCalibrationDatas((LPCTSTR)ymlFile, m_cornersDatas, m_stereoParams, m_remapMatrixs);
	//
	///*保存在xml文件中*/
	//CString xmlFile;
	//xmlFile.Format(_T("%s\\data\\CalibrationDatas.xml"),ProjectPath);
	//m_StereoCalib.saveCalibrationDatas((LPCTSTR)xmlFile, m_cornersDatas, m_stereoParams, m_remapMatrixs);
}


void CBSVDlg::OnBnClickedStereomatch()
{
	// TODO: 在此添加控件通知处理程序代码
	cv::Mat img1, img2, img1p, img2p, disp, disp8u,pointCloud;
	//挑选采集的第一组图片进行双目匹配
	CString LImg, RImg;
	LImg.Format(_T("%s\\Ori_LImage\\chess1.bmp"),ProjectPath);
	RImg.Format(_T("%s\\Ori_RImage\\chess1.bmp"),ProjectPath);
	img1 = imread((LPCTSTR)LImg);
	img2 = imread((LPCTSTR)RImg);

	
	int stereoMethod = 0;//双目匹配方法标志位,0为BM，其他为SGBM
	if(stereoMethod==0)
	{
		//使用BM算法进行双目匹配
		m_stereoMatcher.bmMatch(img1, img2, disp, img1p, img2p);
	}
	else
	{
		//使用SGBM算法进行双目匹配
		m_stereoMatcher.sgbmMatch(img1, img2, disp, img1p, img2p);
	}

	// 画出等距的若干条横线，以比对行对准情况
	int j = 0;
	for( j = 0; j < img1p.rows; j += 32 )		
	{
		line( img1p, cvPoint(0,j),	cvPoint(img1p.cols,j), CV_RGB(0,255,0)); 
		line( img2p, cvPoint(0,j),	cvPoint(img2p.cols,j), CV_RGB(0,255,0)); 
	}

	imshow("左视图",img1p);
	imshow("右视图",img2p);

	bool showColorDisparity = false;//是否显示彩色视差图,该处默认不显示
	if(showColorDisparity)
	{
		m_stereoMatcher.getDisparityImage(disp, disp8u, true);
		imshow("伪彩色视差图",disp8u);
	}
	else
	{
		m_stereoMatcher.getDisparityImage(disp, disp8u, false);
		imshow("视差图",disp8u);
	}

	//检测距离摄像头最近的物体（lq：这里检测到的最近物体都显示为(0,0,0)）
	bool generatePointCloud = true;	//是否生成三维点云标志位
	PointCloudAnalyzer pointCloudAnalyzer;
	clock_t tStart = clock();
	if (generatePointCloud)
	{
		m_stereoMatcher.getPointClouds(disp, pointCloud);

		vector<PointCloudAnalyzer::ObjectInfo> objectInfos;
		pointCloudAnalyzer.detectNearObject(img1p, pointCloud, objectInfos);

		if (!objectInfos.empty() && (clock()-tStart) > 500)
		{
			tStart = clock();
			double fl = m_stereoMatcher.m_FL;
			m_ObjectDistance = objectInfos[0].distance; m_ObjectDistance = (int)(m_ObjectDistance * 10000) / 10000.;
			m_ObjectHeight = objectInfos[0].boundRect.height * objectInfos[0].distance / fl; m_ObjectHeight = (int)(m_ObjectHeight * 10000) / 10000.;
			m_ObjectWidth = objectInfos[0].boundRect.width * objectInfos[0].distance / fl; m_ObjectWidth = (int)(m_ObjectWidth * 10000) / 10000.;
			UpdateData(FALSE);
		}
	}
	// 保存景深图像的点云数据
	if ( generatePointCloud )
	{
		CString PtsFile;
		PtsFile.Format(_T("%s\\data\\points.txt"),ProjectPath);
		m_stereoMatcher.savePointClouds(pointCloud, PtsFile);
	}
	//输出图像中所有像素点对应的三维坐标
	//const double max_z = 1.0e4;
	//for(int y = 0; y < pointCloud.rows; y++)
	//{
	//	for(int x = 0; x < pointCloud.cols; x++)
	//	{
	//		cv::Vec3f point = pointCloud.at<cv::Vec3f>(y, x);
	//		if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z)
	//			printf("%d行%d列:%d %d %d\n", y,x,0, 0, 0);
	//		else
	//			printf("%d行%d列:%f %f %f \n", y,x,point[0], point[1], point[2]);
	//	}
	//}

	//输出像素点（46，271）对应的空间三维坐标
	int x = 271;
	int y = 46;
	const double max_z = 1.0e4;
	cv::Vec3f point = pointCloud.at<cv::Vec3f>(y, x);
	if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z)
		printf("%d行%d列:%d %d %d\n", y,x,0, 0, 0);
	else
		printf("%d行%d列:%f %f %f \n", y,x,point[0], point[1], point[2]);
}

/*
 *函数功能：通过检测图像圆心的坐标值来确定圆心的深度信息
 *问题：目前位置空间坐标系原点位置，未进行试验验证是否可行
*/
void CBSVDlg::OnBnClickedStereomeasure()
{
	// TODO: 在此添加控件通知处理程序代码
	cv::Mat img1, img2, img1p, img2p, disp, disp8u,pointCloud;
	//图片输入：挑选采集的第一组图片进行双目匹配（可改成采集的当前图片）
	CString LImg, RImg;
	LImg.Format(_T("%s\\Ori_LImage\\Left0003.bmp"),ProjectPath);
	RImg.Format(_T("%s\\Ori_RImage\\Right0003.bmp"),ProjectPath);
	img1 = imread((LPCTSTR)LImg);
	img2 = imread((LPCTSTR)RImg);
	//使用BM算法进行双目匹配
	m_stereoMatcher.bmMatch(img1, img2, disp, img1p, img2p);
	//获取视差图
	m_stereoMatcher.getDisparityImage(disp, disp8u, false);
	//下一步：对视差图进行圆形检测获取其圆心像素坐标
	//高斯滤波
	Mat disp8uFilImg;
	GaussianBlur(disp8u, disp8uFilImg, Size(7,7), 0, 0);
	//固定阈值分割图像
	Mat disp8uBinImg;
	threshold(disp8uFilImg, disp8uBinImg, 100, 255, CV_THRESH_BINARY);

	//以左相机的变量为例来进行处理
	Blob_ReleaseLeftBlobSeq();
	Detect_LeftCircleDetect(disp8uBinImg, 0, 255, 0.6, 0.6);
	Blob_DenoisingLeftArea(1000, 8000);


	//下一步：将单个圆心像素坐标映射为空间三维坐标
	double x = LBlob.BlobX;//想办法提取出m_LeftBlobSeq中表示圆心坐标的BlobX和BlobY
	double y = LBlob.BlobY;//保存空间点时，像素坐标是以int类型存储的，这里使用double
	                       //不一定可行
	//下一步：空间三维坐标的显示或保存,目前以将坐标值输出到控制台为例
	const double max_z = 1.0e4;
	cv::Vec3f point = pointCloud.at<cv::Vec3f>(y, x);
	if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z)
		printf("%d行%d列:%d %d %d\n", y,x,0, 0, 0);
	else
		printf("%d行%d列:%f %f %f \n", y,x,point[0], point[1], point[2]);
}


void CBSVDlg::OnBnClickedStopcamera()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_pBSVDlg->m_pLCamera != NULL)
	{
		g_pBSVDlg->StopLeftCamera();
	}
}


void CBSVDlg::OnBnClickedStopcamera2()
{
	// TODO: 在此添加控件通知处理程序代码
		if(g_pBSVDlg->m_pRCamera != NULL)
	{
		g_pBSVDlg->StopRightCamera();
	}
}


void CBSVDlg::OnBnClickedStop2camera()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedStopcamera();
	OnBnClickedStopcamera2();
}


void CBSVDlg::OnBnClickedSave2pic()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedSavepic();
	OnBnClickedSavepic2();
}


void CBSVDlg::OnBnClickedTabbutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_childdlg1.ShowWindow(SW_SHOW);
	m_Childdlg1.ShowWindow(SW_HIDE);
	m_childdlg3.ShowWindow(SW_HIDE);
}


void CBSVDlg::OnBnClickedTabbutton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_childdlg1.ShowWindow(SW_HIDE);
	m_Childdlg1.ShowWindow(SW_SHOW);
	m_childdlg3.ShowWindow(SW_HIDE);
}


void CBSVDlg::OnBnClickedTabbutton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_childdlg1.ShowWindow(SW_HIDE);
	m_Childdlg1.ShowWindow(SW_HIDE);
	m_childdlg3.ShowWindow(SW_SHOW);
}


void CBSVDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC buttonDC;
	CBitmap bitmapTrans;
	BITMAP bmp;
	CDC mem;
	CRect rc;
	buttonDC.Attach(lpDrawItemStruct->hDC);//得到用于绘制按钮的DC
	mem.CreateCompatibleDC(&buttonDC);//准备向按钮区域传输图形    

	if (lpDrawItemStruct->CtlID == IDC_TABBUTTON1)
	{
		rc = lpDrawItemStruct->rcItem;//获取按钮所占的矩形大小
		UINT state  = lpDrawItemStruct->itemState;//获取按钮当前的状态，不同状态绘制不同的按钮    
		if (state & ODS_FOCUS)//如果按钮已经取得焦点，绘制选中状态下的按钮
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP1);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap * old = mem.SelectObject(&bitmapTrans);
			//向按钮传输位图，使用stretcnblt可以使图片随按钮大小而改变
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old);
			bitmapTrans.DeleteObject();

			//更改按钮标题 
			CString btnCaption = _T("双目操作");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem)); //得到原Button的矩形大小  
			CRect textRect;//定义一个CRect用于绘制文本
			textRect.CopyRect(&drawRect);//拷贝矩形区域 
			CSize sz = pDC->GetTextExtent(btnCaption);//获得字符串尺寸
			textRect.top += (textRect.Height()- sz.cy)/2;//调整文本位置居中
			pDC->SetBkMode(TRANSPARENT);//设置文本背景透明 
			pDC->SetTextColor(RGB(100,100,100)); //设置文本颜色
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);//绘制文本内容
		}
		else //如果按钮未取得焦点，绘制选中状态下的按钮
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP2);
			CBitmap *old2 = mem.SelectObject(&bitmapTrans);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap *old=mem.SelectObject(&bitmapTrans);
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old2);
			bitmapTrans.DeleteObject();

			//获得button标题 
			CString btnCaption = _T("双目操作");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));   		
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0,0,0)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}
	}

		if (lpDrawItemStruct->CtlID == IDC_TABBUTTON2)
	{
		rc = lpDrawItemStruct->rcItem;//获取按钮所占的矩形大小
		UINT state  = lpDrawItemStruct->itemState;//获取按钮当前的状态，不同状态绘制不同的按钮    
		if (state & ODS_FOCUS)//如果按钮已经取得焦点，绘制选中状态下的按钮
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP1);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap * old = mem.SelectObject(&bitmapTrans);
			//向按钮传输位图，使用stretcnblt可以使图片随按钮大小而改变
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old);
			bitmapTrans.DeleteObject();

			//更改按钮标题
			CString btnCaption = _T("左相机操作");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));   		
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(100,100,100)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}
		else //如果按钮已经取得焦点，绘制选中状态下的按钮
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP2);
			CBitmap *old2 = mem.SelectObject(&bitmapTrans);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap *old=mem.SelectObject(&bitmapTrans);
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old2);
			bitmapTrans.DeleteObject();

			//获得button标题 
			CString btnCaption = _T("左相机操作");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));   		
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0,0,0)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}
	}

	if (lpDrawItemStruct->CtlID == IDC_TABBUTTON3)
	{
		rc = lpDrawItemStruct->rcItem;//获取按钮所占的矩形大小
		UINT state  = lpDrawItemStruct->itemState;//获取按钮当前的状态，不同状态绘制不同的按钮    
		if (state & ODS_FOCUS)//如果按钮已经取得焦点，绘制选中状态下的按钮
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP1);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap * old = mem.SelectObject(&bitmapTrans);
			//向按钮传输位图，使用stretcnblt可以使图片随按钮大小而改变
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old);
			bitmapTrans.DeleteObject();

			//更改按钮标题
			CString btnCaption = _T("右相机操作");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(100,100,100)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}
		else //如果按钮已经取得焦点，绘制选中状态下的按钮
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP2);
			CBitmap *old2 = mem.SelectObject(&bitmapTrans);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap *old=mem.SelectObject(&bitmapTrans);
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old2);
			bitmapTrans.DeleteObject();

			//更改按钮标题
			CString btnCaption = _T("右相机操作");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));   		
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2; 
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0,0,0)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}



void CBSVDlg::ShowLeftPic(void)
{
	MVSTATUS_CODES hr;
	if (m_pLCamera != NULL)
	{
		MessageBox("相机已打开！");
		return;
	}
	 
	hr = MVOpenCamByIndex( 0, &m_pLCamera ); //打开相机
	if (hr != MVST_SUCCESS)
	{
		MessageBox("左相机打开失败！");
		return;
	}
	else if (hr == MVST_SUCCESS)
	{
		m_nLBit = 24;
		MVGetWidth(m_pLCamera, &m_LeftOriWidth);  //得到图像长宽
		MVGetHeight(m_pLCamera,&m_LeftOriHeight);
		
		MVGetPixelFormat(m_pLCamera,&m_LPixelFormat);     //得到数据格式
		if( m_LPixelFormat == PixelFormat_Mono8 )
		{
			m_nLBit = 8;
		}
		else
		{
			m_nLBit = 24;
		}
	//	MVSetStrobeSource(m_pLCamera,LineSource_ExposureActive);
		
		
		hr =  MVStartGrab(m_pLCamera, LeftCallbackFunction, (long)this); //设置回调函数
		if (hr == MVST_SUCCESS)
		{	
			//原始图像
			m_pLBmpInfo->bmiHeader.biBitCount = m_nLBit;
			m_pLBmpInfo->bmiHeader.biWidth = m_LeftOriWidth;
			m_pLBmpInfo->bmiHeader.biHeight = m_LeftOriHeight;
			
			//为图像显示的图像数据分配内存空间
		    delete[] m_LeftShowDIBBits;	
		    m_LeftShowDIBBits = NULL;	
			m_LeftShowDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];

			delete[] m_LeftOriDIBBits;	
			m_LeftOriDIBBits = NULL;						
			m_LeftOriDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];
			
			//为进行处理的图像赋值
			m_LeftRectLeft=0;  
			m_LeftRectBottom=0;
			m_LeftWidth=m_LeftOriWidth; 
			m_LeftHeight=m_LeftOriHeight;

			delete[] m_LeftDIBBits;	
			m_LeftDIBBits = NULL;					
			m_LeftDIBBits = new char[m_LeftWidth * m_LeftHeight * (m_nLBit/8)];
		}
	}
}


Mat CBSVDlg::FaceDetect(Mat frame)
{
	std::vector<Rect> face;  
	Mat frame_gray(frame.size(),CV_8U);  
    cvtColor( frame, frame_gray, CV_BGR2GRAY );  
    equalizeHist( frame_gray, frame_gray );  
  
    Mycascade.detectMultiScale( frame_gray, face, 1.1, 2, 0, Size(64, 128) );  
  
    for( int i = 0; i < face.size(); i++ )  
	{    
        rectangle(frame,                   //图像.  
			      face[i],
                  Scalar(0, 255, 0),     //线条颜色 (RGB) 或亮度（灰度图像 ）(grayscale image）  
                  1);                   //组成矩形的线条的粗细程度。取负值时（如 CV_FILLED）函数绘制填充了色彩的矩形 
    }  
 
	imshow( window_name, frame );  
	return frame;
}


void CBSVDlg::OnBnClickedFacedetect()
{
	// TODO: 在此添加控件通知处理程序代码
	Mat image,ROI;  
	string Cascade_mode = "..\\BSV\\cascade.xml";
    if(!Mycascade.load( Cascade_mode ))  
	{ 
		printf("[error] 无法加载级联分类器文件！\n");   
		/*return -1;*/    
	} 
	CString ImgPath;
	ImgPath.Format (_T("%s\\face.jpg"),ProjectPath);
    image= imread((LPCTSTR)ImgPath);//读取图片  
    if(!image.data)  
	{  
		printf("[error] 没有图片\n");   
		/*return -1;*/  
	}  
    ROI= FaceDetect(image);  
    waitKey(0);   
    //return 4;  
}

/*
 *函数功能：窗口全屏控件按相应比例放大
 *问题：会造成控件排布混乱，目前只对其中一部分控件执行了该功能
 *原文：https://blog.csdn.net/chw1989/article/details/7488711 
*/
void CBSVDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//空间大小随窗体变化
	//原文：https://blog.csdn.net/chw1989/article/details/7488711 
	if(nType==1) return;//最小化则什么都不做
	CWnd *pWnd; 
	pWnd = GetDlgItem(IDC_STATIC);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_LeftPic);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_RightPic);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_TABBUTTON1);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_TABBUTTON2);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_TABBUTTON3);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_MENUFORM);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Open2Camera);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Cap2Video);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_StereoCalibration);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Circle2Detect);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_StereoMatch);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_StereoMeasure);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_BUTTON8);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Stop2Camera);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Save2Pic);
	ChangeSize(pWnd, cx, cy);
	GetClientRect(&m_rect);// 将变化后的对话框大小设为旧大小
}

/*
 *函数功能：改变控件大小
 *问题：暂无
 *原文：https://blog.csdn.net/chw1989/article/details/7488711 
*/
void CBSVDlg::ChangeSize(CWnd *pWnd, int cx, int cy)
{
    if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建	
	{
		CRect rect;   //获取控件变化前的大小  
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
 
		//    cx/m_rect.Width()为对话框在横向的变化比例
		rect.left=rect.left*cx/m_rect.Width();//调整控件大小
		rect.right=rect.right*cx/m_rect.Width();
		rect.top=rect.top*cy/m_rect.Height();
		rect.bottom=rect.bottom*cy/m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
	}
}

