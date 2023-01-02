
// BSVDlg.cpp : ʵ���ļ�
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
	//����ͼ���ڴ�
	camera.SectionCopy.Lock();

	//Ϊԭʼͼ��ֵ�������24λRGB��ʽ����ֱ��ת��
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

	image.BufHandle = true;//��ͼ����Խ��д����ʶ
}

/*
 *����˵��������ص�����,����������������봥��ģʽ��ʱ��ÿ���һ֡ͼ��ͻ���øú���˵����
 *�����������
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
 *����˵��������ص�����,����������������봥��ģʽ��ʱ��ÿ���һ֡ͼ��ͻ���øú���˵����
 *�����������
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// CBSVDlg �Ի���
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

// CBSVDlg ��Ϣ�������
BOOL CBSVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//�򿪿���̨
	AllocConsole();
	freopen("CONOUT$", "a+", stdout);

	//����Picture Control�ؼ��Ĵ�С
	SetPictureSize(IDC_LeftPic);
	SetPictureSize(IDC_RightPic);

	GetDlgItem(IDC_CapVideo)->EnableWindow(false);
	GetDlgItem(IDC_SetCamera)->EnableWindow(false);
	GetDlgItem(IDC_StopDetect)->EnableWindow(false);

	GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
	GetDlgItem(IDC_SetCamera2)->EnableWindow(false);
	GetDlgItem(IDC_StopDetect2)->EnableWindow(false);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CBSVDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�ͷ��ڴ���Դ
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

	//�����ʱ��
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CBSVDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBSVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
*����˵������ʾ���ͼ��
*�����������
*          
*����������ޡ�
*�� �� ֵ���ޡ�
*/
void CBSVDlg::DrawCamera(Camera &camera, Image &image, UINT32 picId)
{
	camera.Section.Lock();
	CDC* pdc = GetDlgItem(picId)->GetDC();//���DC
	CRect rc;
	GetDlgItem(picId)->GetClientRect(&rc);//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR);//����ģʽ

	//��ʾͼ��
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(), rc.Height(),
		0, 0, image.OriWidth, image.OriHeight,
		image.ShowDIBBits, (LPBITMAPINFO)image.BmpInfo, DIB_RGB_COLORS, SRCCOPY);

	//��ʾʮ�ֲο���
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
	if (camera.CameraHandle == NULL)	//�������
	{
		hr = MVOpenCamByIndex(0, &camera.CameraHandle);
		if (hr != MVST_SUCCESS)
		{
			MessageBox("δ�ҵ��������ȷ���豸���Ӻ�IP���ã�", "BSV", MB_ICONWARNING);
			return;
		}
		else if (hr == MVST_SUCCESS)
		{
			GetDlgItem(openCameraId)->SetWindowText("�ر����");
			GetDlgItem(capVideoId)->EnableWindow(true);
			GetDlgItem(setCameraId)->EnableWindow(true);
		}
	}
	else	//�ر����
	{
		hr = MVStopGrab(camera.CameraHandle);
		hr = MVCloseCam(camera.CameraHandle);
		camera.CameraHandle = NULL;

		GetDlgItem(openCameraId)->SetWindowText("�������");
		GetDlgItem(capVideoId)->EnableWindow(false);
		GetDlgItem(setCameraId)->EnableWindow(false);
	}
}

void CBSVDlg::OnBnClickedOpencamera()
{
	//���������
	OpenCamera(m_LeftCamera, IDC_OpenCamera, IDC_CapVideo, IDC_SetCamera);

}

void CBSVDlg::CapVideo(Camera& camera, Image &image, UINT32 capVideoId, int (*CallBackFunc)(MV_IMAGE_INFO*, long nUserVal))
{
	if (camera.CameraHandle != NULL)
	{
		MVGetWidth(camera.CameraHandle, &image.OriWidth);  //���ͼ�񳤿�
		MVGetHeight(camera.CameraHandle, &image.OriHeight);
		MVGetPixelFormat(camera.CameraHandle, &camera.PixelFormat);  //������ݸ�ʽ
		if (camera.PixelFormat == PixelFormat_Mono8)
		{
			image.Bit = 8;
		}
		else
		{
			image.Bit = 24;
		}

		MVSTATUS_CODES hr;
		hr = MVStartGrab(camera.CameraHandle, CallBackFunc, (long)this); //���ûص�����
		if (hr == MVST_SUCCESS)
		{
			//ԭʼͼ��
			image.BmpInfo->bmiHeader.biBitCount = image.Bit;
			image.BmpInfo->bmiHeader.biWidth = image.OriWidth;
			image.BmpInfo->bmiHeader.biHeight = image.OriHeight;

			//Ϊͼ����ʾ��ͼ�����ݷ����ڴ�ռ�
			delete[] image.ShowDIBBits;
			image.ShowDIBBits = NULL;
			image.ShowDIBBits = new char[image.OriWidth * image.OriHeight * (image.Bit / 8)];

			delete[] image.OriDIBBits;
			image.OriDIBBits = NULL;
			image.OriDIBBits = new char[image.OriWidth * image.OriHeight * (image.Bit / 8)];

			//Ϊ���д����ͼ��ֵ
			image.Width = image.OriWidth;
			image.Height = image.OriHeight;

			delete[] image.DIBBits;
			image.DIBBits = NULL;
			image.DIBBits = new char[image.Width * image.Height * (image.Bit / 8)];

			//��ʼ�ɼ�ͼ��󣬲��ٽ��ܸ��µġ������ɼ���ָ��
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
		MessageBox("��δ����������޷����ã�", "BSV", MB_ICONWARNING);
	}
	else
	{
		MessageBox("���޴˹���");
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
		MessageBox("��ͼ�����ݣ�", "BSV", MB_ICONWARNING);
		return;
	}
	else
	{
		SetTimer(nIDEvent, nElapse, NULL);//���ö�ʱ��
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
*�������ܣ���Ӧ��ʱ����Ϣ������
*����˵����
nIDEvent ��ʱ����ʶ������һ�������ڿ���ʹ�ö����ʱ��
         ����ͬ�Ķ�ʱ������nIDEvent�����֡�
*/
void CBSVDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)//nIDEvent Ϊ��ʱ���¼�ID��1��2
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
	//TODO: ȥ�������������е�ħ����
	//TODO: ����ʽ��̱�����������
	if (camera.PixelFormat != PixelFormat_Mono8)
		return;

	Mat SrcImg;
	SrcImg.create(image.OriHeight, image.OriWidth, CV_8UC1);
	camera.Section.Lock();
	memcpy(SrcImg.data, image.ShowDIBBits, image.OriWidth * image.OriHeight * (image.Bit / 8));
	camera.Section.Unlock();

	Mat FilImg;
	//˫���˲�������ȥ��
	//bilateralFilter(SrcImg, FilImg, 25, 25*2, 25/2);
	//��˹�˲�
	GaussianBlur(SrcImg, FilImg, Size(7, 7), 0, 0);

	Mat BinImg;
	//����Ӧ��ֵ�ָ�ͼ��
	//adaptiveThreshold(FilImg, BinImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7, 5);
	//�̶���ֵ��ֵ��ͼ��ָ�ͼ��
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
	//���������
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
*�������ܣ��ͷ�BlobSeq�е�Blob�����ڴ棬���BlobSeq�е�����
*����˵��
CPtrArray* BlobSeq	   ���ͷ��ڴ���ſ���С�
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
*�������ܣ����Բ������
*����˵��
const CvArr* srcImage         ������ĻҶ�ͼ����ֵ��ͼ��
double lowthresh, highthresh  ����ӦCanny��Ե���ĸߵ���ֵ
double aspectRatio            �ݺ��
double circularity            Բ�ζ�
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
			area = contourArea(contours[i]);		//��i�����������
			length = arcLength(contours[i], true);	//��i���������ܳ�
			cir = (12.56 * area) / (length * length);		//��i��������Բ�ζ�
			if (cir > circularity)
			{
				rect = fitEllipse(contours[i]);
				width = rect.size.width;	//��Ӿ��εĿ��
				height = rect.size.height;	//��Ӿ��εĸ߶�
				asp = height / width;			//�ݺ��
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
*�������ܣ�����ſ�������������ָ����Χ�ڵ��ſ�����
*����˵��
	CPtrArray* blobSeq     ��������ſ����
	int minArea, maxArea    ָ�����ſ������Χ
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
	CDC* pdc = GetDlgItem(picId)->GetDC();//��ãģ�
	CRect rc;
	GetDlgItem(picId)->GetClientRect(&rc);//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR);//����ģʽ

	CBrush brushRed(RGB(255, 0, 0)); 	// ������ɫ���ʶ���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	CString result;     //��ʽ��֮���������Чλ��Ϊ6
	
	//�ݴ��������ݲ���Ϊ����
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
	//����һ��Ӧ�ü�һ���ж���䣬�������û��������ȫ�Ͳ�ִ������ı궨���
	if (m_Calibration.RowCornerNum == 0 || m_Calibration.ColumCornerNum == 0 
		|| m_Calibration.GridWidth == 0 || m_Calibration.GridHeight == 0)
	{
		AfxMessageBox(_T("δ�����������"));
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
		AfxMessageBox("û�д����");
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
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];//����ͼ����Ϣͷ
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
