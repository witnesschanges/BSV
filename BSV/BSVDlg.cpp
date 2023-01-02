
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
#include "CalibrateCamera.h"
#include "Cali_ParaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//TODO: these code blocks that is not belonged to BSVDlg class should be moved into a dedicated file.
CBSVDlg* g_pBSVDlg = NULL;
CBSVDlg* g_pBSVThread = NULL;

void MVGrabInternal(Camera& camera, Image& image, MV_IMAGE_INFO* pInfo)
{
	//拷贝图像内存
	camera.SectionCopy.Lock();

	//为原始图像赋值，如果是24位RGB格式，则直接转换
	if (camera.PixelFormat == PixelFormat_Mono8)
	{
		memcpy(image.OriDIBBits, pInfo->pImageBuffer,
			image.OriWidth * image.OriHeight);
	}
	else
	{
		int count = (((image.OriWidth * image.Bit) + 31) / 32) * 4;
		MVBayerToBGR(camera.CameraHandle, pInfo->pImageBuffer, image.OriDIBBits, count,
			image.OriWidth, image.OriHeight, camera.PixelFormat);
	}

	LPSTR itemBits;
	itemBits = new char[image.OriWidth * image.OriHeight * (image.Bit / 8)];
	memcpy(itemBits, image.OriDIBBits, image.OriWidth * image.OriHeight * (image.Bit / 8));
	for (int i = 0; i < image.OriHeight; i++)
	{
		for (int j = 0; j < image.OriWidth; j++)
		{
			if (image.Bit == 8)
			{
				image.ShowDIBBits[i * image.OriWidth + j] =
					itemBits[(image.OriHeight - i - 1) * image.OriWidth + j];
			}
			else if (image.Bit == 24)
			{
				image.ShowDIBBits[(i * image.OriWidth + j) * 3] =
					itemBits[((image.OriHeight - i - 1) * image.OriWidth + j) * 3];
				image.ShowDIBBits[(i * image.OriWidth + j) * 3 + 1] =
					itemBits[((image.OriHeight - i - 1) * image.OriWidth + j) * 3 + 1];
				image.ShowDIBBits[(i * image.OriWidth + j) * 3 + 2] =
					itemBits[((image.OriHeight - i - 1) * image.OriWidth + j) * 3 + 2];
			}
		}
	}
	delete[]itemBits;
	camera.SectionCopy.Unlock();

	image.BufHandle = true;//左图像可以进行处理标识
}

/*
 *函数说明：相机回调函数,在相机工作在连续与触发模式下时，每获得一帧图像就会调用该函数说明。
 *输入参数：无
 *
*/
int LeftCallbackFunction(MV_IMAGE_INFO* pInfo, long nUserVal)
{
	Camera& leftCamera = g_pBSVDlg->m_LeftCamera;
	Image& leftImage = g_pBSVDlg->m_LeftImage;

	if (leftImage.OriDIBBits != NULL)
	{
		MVGrabInternal(leftCamera, leftImage, pInfo);
		g_pBSVDlg->DrawCamera(leftCamera, leftImage, IDC_LeftPic);
	}

	return 1;
}

/*
 *函数说明：相机回调函数,在相机工作在连续与触发模式下时，每获得一帧图像就会调用该函数说明。
 *输入参数：无
 *
*/
int RightCallbackFunction(MV_IMAGE_INFO* pInfo, long nUserVal)
{
	Camera& rightCamera = g_pBSVDlg->m_RightCamera;
	Image& rightImage = g_pBSVDlg->m_RightImage;

	if (rightImage.OriDIBBits != NULL)
	{
		MVGrabInternal(rightCamera, rightImage, pInfo);
		g_pBSVDlg->DrawCamera(rightCamera, rightImage, IDC_RightPic);
	}

	return 1;
}

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
	ON_BN_CLICKED(IDC_SavePic, &CBSVDlg::OnBnClickedSavepic)
	ON_BN_CLICKED(IDC_SavePic2, &CBSVDlg::OnBnClickedSavepic2)
	ON_BN_CLICKED(IDC_Calibration2, &CBSVDlg::OnBnClickedCalibration2)
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

	//设置Picture Control控件的大小
	SetPictureSize(IDC_LeftPic);
	SetPictureSize(IDC_RightPic);

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
		KillTimer(FirstTimerId);
		m_TimerFlag = NULL;
	}

	if (m_TimerFlag2)
	{
		KillTimer(SecondTimerId);
		m_TimerFlag2 = NULL;
	}

	ReleaseBlobSeq(m_LeftCamera.BlobSeq);
	ReleaseBlobSeq(m_RightCamera.BlobSeq);

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
		LocateCursorMiddle(dc);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CBSVDlg::LocateCursorMiddle(CDC& dc)
{
	int cxIcon = GetSystemMetrics(SM_CXICON);
	int cyIcon = GetSystemMetrics(SM_CYICON);
	
	CRect rect;
	GetClientRect(&rect);
	int x = (rect.Width() - cxIcon + 1) / 2;
	int y = (rect.Height() - cyIcon + 1) / 2;

	dc.DrawIcon(x, y, m_hIcon);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBSVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
*函数说明：显示相机图像
*输入参数：无
*          
*输出参数：无。
*返 回 值：无。
*/
void CBSVDlg::DrawCamera(Camera &camera, Image &image, UINT32 picId)
{
	camera.Section.Lock();
	CDC* pdc = GetDlgItem(picId)->GetDC();//获得DC
	CRect rc;
	GetDlgItem(picId)->GetClientRect(&rc);//获得图像显示区域
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR);//设置模式

	//显示图像
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(), rc.Height(),
		0, 0, image.OriWidth, image.OriHeight,
		image.ShowDIBBits, (LPBITMAPINFO)image.BmpInfo, DIB_RGB_COLORS, SRCCOPY);

	//显示十字参考线
	//CPen pPenBlue;
	//pPenBlue.CreatePen(PS_SOLID,1,RGB(0,0,128));
	//CPen *pOldPen=pdc->SelectObject(&pPenBlue);
	//double dbRateX = (double)rc.Width() /(double)image.Width;
	//double dbRateY = (double)rc.Height() /(double)image.Height;
	//double x1 = image.Width*dbRateX;
	//double x2 = x1/2;
	//double y1 = image.Height*dbRateY;
	//double y2 = y1/2;
	//pdc->MoveTo (0,y2);
	//pdc->LineTo (x1,y2);
	//pdc->MoveTo (x2,0);
	//pdc->LineTo (x2,y1);
	//pdc->SelectObject(pOldPen);
	//pPenBlue.DeleteObject();

	GetDlgItem(picId)->ReleaseDC(pdc);
	camera.Section.Unlock();
}

void CBSVDlg::SetPictureSize(UINT32 picId)
{
	CRect rect;
	GetDlgItem(picId)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(picId)->MoveWindow(rect.left, rect.top, PictureControlWidth, PictureControlHight, true);
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
	//启动左相机
	OpenCamera(m_LeftCamera, IDC_OpenCamera, IDC_CapVideo, IDC_SetCamera);

}

void CBSVDlg::CapVideo(Camera& camera, Image &image, UINT32 capVideoId, int (*CallBackFunc)(MV_IMAGE_INFO*, long nUserVal))
{
	if (camera.CameraHandle != NULL)
	{
		MVGetWidth(camera.CameraHandle, &image.OriWidth);  //获得图像长宽
		MVGetHeight(camera.CameraHandle, &image.OriHeight);
		MVGetPixelFormat(camera.CameraHandle, &camera.PixelFormat);  //获得数据格式
		if (camera.PixelFormat == PixelFormat_Mono8)
		{
			image.Bit = 8;
		}
		else
		{
			image.Bit = 24;
		}

		MVSTATUS_CODES hr;
		hr = MVStartGrab(camera.CameraHandle, CallBackFunc, (long)this); //设置回调函数
		if (hr == MVST_SUCCESS)
		{
			//原始图像
			image.BmpInfo->bmiHeader.biBitCount = image.Bit;
			image.BmpInfo->bmiHeader.biWidth = image.OriWidth;
			image.BmpInfo->bmiHeader.biHeight = image.OriHeight;

			//为图像显示的图像数据分配内存空间
			delete[] image.ShowDIBBits;
			image.ShowDIBBits = NULL;
			image.ShowDIBBits = new char[image.OriWidth * image.OriHeight * (image.Bit / 8)];

			delete[] image.OriDIBBits;
			image.OriDIBBits = NULL;
			image.OriDIBBits = new char[image.OriWidth * image.OriHeight * (image.Bit / 8)];

			//为进行处理的图像赋值
			image.Width = image.OriWidth;
			image.Height = image.OriHeight;

			delete[] image.DIBBits;
			image.DIBBits = NULL;
			image.DIBBits = new char[image.Width * image.Height * (image.Bit / 8)];

			//开始采集图像后，不再接受更新的“连续采集”指令
			GetDlgItem(capVideoId)->EnableWindow(false);
		}
	}
}

void CBSVDlg::OnBnClickedCapvideo()
{
	CapVideo(m_LeftCamera, m_LeftImage, IDC_CapVideo, LeftCallbackFunction);
}

void CBSVDlg::SetCamera(Camera& camera)
{
	if (camera.CameraHandle == NULL)
	{
		MessageBox("尚未启动相机，无法配置！", "BSV", MB_ICONWARNING);
	}
	else
	{
		MessageBox("暂无此功能");
	}
}

void CBSVDlg::OnBnClickedSetcamera()
{
	SetCamera(m_LeftCamera);
}

void CBSVDlg::Circledetect(Image &image, UINT_PTR nIDEvent, UINT nElapse, bool timerflag, UINT32 circleDetectId, UINT32 stopDetectId)
{
	if (image.ShowDIBBits == NULL)
	{
		MessageBox("无图像数据！", "BSV", MB_ICONWARNING);
		return;
	}
	else
	{
		SetTimer(nIDEvent, nElapse, NULL);//设置定时器
		timerflag = true;

		GetDlgItem(circleDetectId)->EnableWindow(false);
		GetDlgItem(stopDetectId)->EnableWindow(true);
	}
}

void CBSVDlg::OnBnClickedCircledetect()
{
	Circledetect(m_LeftImage, FirstTimerId, CircleDetectTimerElapse, m_TimerFlag, IDC_CircleDetect, IDC_StopDetect);
}

void CBSVDlg::Stopdetect(bool timerFlag, UINT_PTR nIDEvent, UINT32 circleDetectId, UINT32 stopDetectId)
{
	if (timerFlag)
	{
		KillTimer(nIDEvent);
		timerFlag = false;
	}

	//todo: use a general method and id array to do this kind of work
	GetDlgItem(circleDetectId)->EnableWindow(true);
	GetDlgItem(stopDetectId)->EnableWindow(false);
}


void CBSVDlg::OnBnClickedStopdetect()
{
	Stopdetect(m_TimerFlag, FirstTimerId, IDC_CircleDetect, IDC_StopDetect);
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
	switch(nIDEvent)//nIDEvent 为定时器事件ID，1，2
	{
		case FirstTimerId:
		{
			RecognizeCircle(g_pBSVDlg->m_LeftCamera, m_LeftImage);
			break;
		}
		case SecondTimerId:
		{
			RecognizeCircle(g_pBSVDlg->m_RightCamera, m_RightImage);
			break;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CBSVDlg::RecognizeCircle(Camera& camera, Image image)
{
	//TODO: 去除函数里面所有的魔法数
	//TODO: 过程式编程编程面向对象编程
	if (camera.PixelFormat != PixelFormat_Mono8)
		return;

	Mat SrcImg;
	SrcImg.create(image.OriHeight, image.OriWidth, CV_8UC1);
	camera.Section.Lock();
	memcpy(SrcImg.data, image.ShowDIBBits, image.OriWidth * image.OriHeight * (image.Bit / 8));
	camera.Section.Unlock();

	Mat FilImg;
	//双边滤波，保边去噪
	//bilateralFilter(SrcImg, FilImg, 25, 25*2, 25/2);
	//高斯滤波
	GaussianBlur(SrcImg, FilImg, Size(7, 7), 0, 0);

	Mat BinImg;
	//自适应阈值分割图像
	//adaptiveThreshold(FilImg, BinImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7, 5);
	//固定阈值二值化图像分割图像
	threshold(FilImg, BinImg, 100, 255, CV_THRESH_BINARY);

	ReleaseBlobSeq(camera.BlobSeq);
	DetectCircle(camera, BinImg, 0, 255, 0.6, 0.6);
	DenoisingBlobArea(camera.BlobSeq, 1000, 8000);

	if (!camera.BlobSeq.IsEmpty())
	{
		ShowCircles(camera, image, IDC_RightPic);
	}
	SrcImg.release();
	FilImg.release();
	BinImg.release();
}

void CBSVDlg::OnBnClickedOpencamera2()
{
	//启动右相机
	OpenCamera(m_RightCamera, IDC_OpenCamera2, IDC_CapVideo2, IDC_SetCamera2);
}

void CBSVDlg::OnBnClickedCapvideo2()
{
	CapVideo(m_RightCamera, m_RightImage, IDC_CapVideo2, RightCallbackFunction);
}

void CBSVDlg::OnBnClickedSetcamera2()
{
	SetCamera(m_RightCamera);
}

void CBSVDlg::OnBnClickedOpen2camera()
{
	OpenCamera(m_LeftCamera, IDC_OpenCamera, IDC_CapVideo, IDC_SetCamera);
	OpenCamera(m_RightCamera, IDC_OpenCamera2, IDC_CapVideo2, IDC_SetCamera2);
}

void CBSVDlg::OnBnClickedCap2video()
{
	CapVideo(m_LeftCamera, m_LeftImage, IDC_CapVideo, LeftCallbackFunction);
	CapVideo(m_RightCamera, m_RightImage, IDC_CapVideo2, RightCallbackFunction);
}

/*
*函数功能：释放BlobSeq中的Blob数据内存，清除BlobSeq中的数据
*参数说明
CPtrArray* BlobSeq	   待释放内存的团块队列。
*/
void CBSVDlg::ReleaseBlobSeq(CArray<Blob>& blobSeq)
{
	if (!blobSeq.IsEmpty())
	{
		blobSeq.RemoveAll();
		blobSeq.FreeExtra();
	}
}

/*
*函数功能：检测圆形特征
*参数说明
const CvArr* srcImage         待处理的灰度图像或二值化图像
double lowthresh, highthresh  自适应Canny边缘检测的高低阈值
double aspectRatio            纵横比
double circularity            圆形度
*/
void CBSVDlg::DetectCircle(Camera &camera, InputArray srcImg, double lowthresh,
	double highthresh, double aspectRatio, double circularity)
{
	Mat edgeImg;
	Canny(srcImg, edgeImg, lowthresh, highthresh);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edgeImg, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	edgeImg.release();

	double area, length, cir, width, height, asp;
	RotatedRect rect;
	int cnt = 0;
	int ln = contours.size();
	camera.BlobSeq.SetSize(ln, 10);
	for (int i = 0; i < ln; i++)
	{
		if (contours[i].size() > 10)
		{
			area = contourArea(contours[i]);		//第i个轮廓的面积
			length = arcLength(contours[i], true);	//第i个轮廓的周长
			cir = (12.56 * area) / (length * length);		//第i个轮廓的圆形度
			if (cir > circularity)
			{
				rect = fitEllipse(contours[i]);
				width = rect.size.width;	//外接矩形的宽度
				height = rect.size.height;	//外接矩形的高度
				asp = height / width;			//纵横比
				if (asp > aspectRatio && asp < (1.0 / aspectRatio))
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

					camera.BlobSeq.SetAtGrow(cnt, pB);
					cnt++;
				}
			}
		}
	}
	camera.BlobSeq.FreeExtra();
}

/*
*函数功能：清除团块队列中面积不在指定范围内的团块数据
*参数说明
	CPtrArray* blobSeq     待处理的团块队列
	int minArea, maxArea    指定的团块面积范围
*/
void CBSVDlg::DenoisingBlobArea(CArray<Blob> &blob, int minArea, int maxArea)
{
	if (!blob.IsEmpty())
	{
		int ln = blob.GetSize();
		for (int i = 0; i < ln; i++)
		{
			Blob pB = blob.GetAt(i);
			if (pB.Area < minArea || pB.Area > maxArea)
			{
				blob.RemoveAt(i);
				i--;
				ln--;
			}
		}
		blob.FreeExtra();
	}
}

void CBSVDlg::ShowCircles(Camera &camera, Image &image, UINT32 picId)
{
	CDC* pdc = GetDlgItem(picId)->GetDC();//获得ＤＣ
	CRect rc;
	GetDlgItem(picId)->GetClientRect(&rc);//获得图像显示区域
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR);//设置模式

	CBrush brushRed(RGB(255, 0, 0)); 	// 创建红色画笔对象
	CBrush* pOldBrush = pdc->SelectObject(&brushRed);

	double dbRateX = (double)rc.Width() / (double)image.OriWidth;
	double dbRateY = (double)rc.Height() / (double)image.OriHeight;
	int ln = camera.BlobSeq.GetSize();
	for (int i = 0; i < ln; i++)
	{
		Blob pB = camera.BlobSeq.GetAt(i);
		int x = (int)(pB.BlobX * dbRateX);
		int y = (int)((image.OriHeight - pB.BlobY) * dbRateY);
		if (x < 6)//Magic number
		{
			x = 6;
		}
		else if (x > rc.Width() - 6)
		{
			x = rc.Width() - 6;
		}
		if (y < 6)
		{
			y = 6;
		}
		else if (y > rc.Height() - 6)
		{
			y = rc.Height() - 6;
		}
		pdc->Ellipse(x - 5, y - 5, x + 5, y + 5);
	}

	pdc->SelectObject(pOldBrush);
	brushRed.DeleteObject();
	ReleaseDC(pdc);
}

void CBSVDlg::OnBnClickedCircledetect2()
{
	Circledetect(m_RightImage, SecondTimerId, CircleDetectTimerElapse, m_TimerFlag2, IDC_CircleDetect2, IDC_StopDetect2);
}

void CBSVDlg::OnBnClickedStopdetect2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_TimerFlag2)
	{
		KillTimer(SecondTimerId);
		m_TimerFlag2 = false;
	}

	GetDlgItem(IDC_CircleDetect2)->EnableWindow(true);
	GetDlgItem(IDC_StopDetect2)->EnableWindow(false);
}

void CBSVDlg::OnBnClickedCircle2detect()
{
	Circledetect(m_LeftImage, FirstTimerId, CircleDetectTimerElapse, m_TimerFlag, IDC_CircleDetect, IDC_StopDetect);
	Circledetect(m_RightImage, SecondTimerId, CircleDetectTimerElapse, m_TimerFlag2, IDC_CircleDetect2, IDC_StopDetect2);
}

CString format_fraction(double data)
{
	CString result;     //格式化之后的数据有效位数为6
	
	//暂存输入数据并存为整形
	int tmp = abs((int)data);

	if (tmp/10000>=1)
	{
		result.Format("%.0f", data);
	}
	else if (tmp/1000>=1)
	{
		result.Format("%.1f", data);
	}
	else if (tmp/100>=1)
	{
		result.Format("%.2f", data);
	}
	else if (tmp/10>=1)
	{
		result.Format("%.3f", data);
	}
	else
	{
		result.Format("%.4f", data);
	}
	return result;
}

void CBSVDlg::Calibrate(bool isLeft)
{
	string dataPath = isLeft ? LeftCameraCalibdataText : RightCameraCalibdataText;
	string resultPath = isLeft ? LeftCameraCalibrationResultText : RightCameraCalibrationResultText;

	m_Calibration.Initial();
	CCali_ParaDlg dlg;
	dlg.DoModal();
	//在这一段应该加一段判断语句，如果数据没有输入完全就不执行下面的标定语句
	if (m_Calibration.RowCornerNum == 0 || m_Calibration.ColumCornerNum == 0 
		|| m_Calibration.GridWidth == 0 || m_Calibration.GridHeight == 0)
	{
		AfxMessageBox(_T("未完整输入参数"));
	}
	else
	{
		CameraCalibration* cameraCalibration = new CameraCalibration(dataPath, resultPath, m_Calibration.RowCornerNum, m_Calibration.ColumCornerNum, m_Calibration.GridWidth, m_Calibration.GridHeight);
		cameraCalibration->CalibrateCamera();
	}
}

void CBSVDlg::OnBnClickedCalibration()
{
	Calibrate(true);
}

void CBSVDlg::OnBnClickedCalibration2()
{
	Calibrate(false);
}

void CBSVDlg::Savepic(Camera &camera, Image &image, bool isLeft)
{
	if (camera.CameraHandle == NULL)
	{
		AfxMessageBox("没有打开相机");
		return;
	}
	camera.ImageIndex++;

	if (camera.PixelFormat != PixelFormat_Mono8)
		return;

	Mat Img;
	Img.create(image.OriHeight, image.OriWidth, CV_8UC1);
	camera.SectionCopy.Lock();
	memcpy(Img.data, image.ShowDIBBits, image.OriWidth * image.OriHeight * (image.Bit / 8));
	camera.SectionCopy.Unlock();

	std::stringstream StrStm;
	string imageFileName;
	StrStm.clear();
	imageFileName.clear();
	string filePath = isLeft ? "Ori_LImage\\chess" : "Ori_RImage\\chess";
	StrStm << camera.ImageIndex;
	StrStm >> imageFileName;
	filePath += imageFileName;
	filePath += ".bmp";
	imwrite(filePath, Img);
}

void CBSVDlg::OnBnClickedSavepic()
{
	Savepic(m_LeftCamera, m_LeftImage, true);
}

void CBSVDlg::OnBnClickedSavepic2()
{
	Savepic(m_RightCamera, m_RightImage, true);
}

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
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];//设置图像信息头
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

void Calibration::Initial()
{
	RowCornerNum = 0;
	ColumCornerNum = 0;
	GridWidth = 0;
	GridHeight = 0;
}
