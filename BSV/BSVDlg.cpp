
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
#include "externFunc.h"
#include "Cali_ParaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBSVDlg* g_pBSVDlg = NULL;
CBSVDlg* g_pBSVThread = NULL;

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

	//�����������ʼ��
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	g_pBSVDlg = this;
	 
	m_LeftCamera.CameraHandle = NULL;
	m_LeftCamera.ImageIndex = 0;

	m_bLBufHandle = false;

	m_LeftOriDIBBits = NULL;
	m_LeftDIBBits = NULL;
	m_LeftShowDIBBits = NULL;

	m_LeftWidth = 0;
	m_LeftHeight = 0;
	m_LeftOriWidth = 1600;
	m_LeftOriHeight = 1200;

	m_nLBit = 8;

	m_pLBmpInfo = (LPBITMAPINFO) new 
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];		//����ͼ����Ϣͷ
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
	//�����������ʼ��
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
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];		//����ͼ����Ϣͷ
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

	//�̶�Picture Control�ؼ��Ĵ�С
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

	delete[] m_RightShowDIBBits;
	m_LeftShowDIBBits = NULL;
	delete[] m_LeftDIBBits;
	m_LeftDIBBits = NULL;
	delete[] m_LeftOriDIBBits;
	m_LeftOriDIBBits = NULL;

	//�����ʱ��
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
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBSVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
 *����˵��������ص�����,����������������봥��ģʽ��ʱ��ÿ���һ֡ͼ��ͻ���øú���˵����
 *�����������
 *		
 *���������
 LPBYTE pbuf       ͼ��������
 int nW            ͼ��Ŀ�
 int nH            ͼ��ĸ�
 int nBitCount     ͼ�������λ��
 int nCameraIndex  �����ţ���1��ʼ������
 FILE *pFile       ��������Ϣ���ļ�ָ��
 *����ֵ���ޡ�
*/
int LeftCallbackFunction(MV_IMAGE_INFO *pInfo,long nUserVal)
	// int index, LPSTR pData, int Length, void* pUserData )
{
	if(g_pBSVDlg->m_LeftOriDIBBits != NULL)
	{
		//����ͼ���ڴ�
		g_pBSVDlg->m_LeftCamera.SectionCopy.Lock();

		//Ϊԭʼͼ��ֵ�������24λRGB��ʽ����ֱ��ת��
		if( g_pBSVDlg->m_LeftCamera.PixelFormat == PixelFormat_Mono8 )
		{
			memcpy(g_pBSVDlg->m_LeftOriDIBBits,pInfo->pImageBuffer,
				g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_LeftOriHeight);
		}
		else
		{
			int count = (((g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_nLBit)+31)/32)*4;
			MVBayerToBGR(g_pBSVDlg->m_LeftCamera.CameraHandle,pInfo->pImageBuffer,g_pBSVDlg->m_LeftOriDIBBits,count,
				g_pBSVDlg->m_LeftOriWidth,g_pBSVDlg->m_LeftOriHeight,g_pBSVDlg->m_LeftCamera.PixelFormat);
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
		g_pBSVDlg->m_LeftCamera.SectionCopy.Unlock();

		g_pBSVDlg->m_bLBufHandle = true;//��ͼ����Խ��д����ʶ
		g_pBSVDlg->DrawLeftCamera();
	}
	return 1;
}

//*****���º���������������д*****//
int RightCallbackFunction(MV_IMAGE_INFO *pInfo,long nUserVal)
	// int index, LPSTR pData, int Length, void* pUserData )
{
	if(g_pBSVDlg->m_RightOriDIBBits != NULL)//������
	{
		//����ͼ���ڴ�
		g_pBSVDlg->m_CSectionCopyR.Lock();

		//Ϊԭʼͼ��ֵ�������24λRGB��ʽ����ֱ��ת��
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

		g_pBSVDlg->m_bRBufHandle = true;//��ͼ����Խ��д����ʶ
		g_pBSVDlg->DrawRightCamera();
	}
	return 1;
}

/*
*����˵������ʾ�����ͼ��
*�����������
*          
*����������ޡ�
*�� �� ֵ���ޡ�
*/
void CBSVDlg::DrawLeftCamera()
{
	m_LeftCamera.Section.Lock();
	CDC *pdc = GetDlgItem(IDC_LeftPic)->GetDC();//���DC
	CRect rc;
	GetDlgItem(IDC_LeftPic)->GetClientRect(&rc);//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR );//����ģʽ

	//��ʾͼ��
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(),rc.Height(),
		0,0,m_LeftOriWidth,m_LeftOriHeight,
		m_LeftShowDIBBits,(LPBITMAPINFO) m_pLBmpInfo, DIB_RGB_COLORS, SRCCOPY); 
	
	//��ʾʮ�ֲο���
	//CPen pPenBlue;
	//pPenBlue.CreatePen(PS_SOLID,1,RGB(0,0,128));
	//CPen *pOldPen=pdc->SelectObject(&pPenBlue);
	//double dbRateX = (double)rc.Width() /(double)m_LeftWidth;
	//double dbRateY = (double)rc.Height() /(double)m_LeftHeight;
	//double x1 = m_LeftWidth*dbRateX;
	//double x2 = x1/2;
	//double y1 = m_LeftHeight*dbRateY;
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
*����˵������ʾ�����ͼ��
*�����������
*          
*����������ޡ�
*�� �� ֵ���ޡ�
*/
void CBSVDlg::DrawRightCamera()
{
	m_CSectionR.Lock();
	CDC *pdc = GetDlgItem(IDC_RightPic)->GetDC();//���DC
	CRect rc;
	GetDlgItem(IDC_RightPic)->GetClientRect(&rc);//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;//����ģʽ

	//��ʾͼ��
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(),rc.Height(),
		0,0,m_RightOriWidth,m_RightOriHeight,
		m_RightShowDIBBits,(LPBITMAPINFO) m_pRBmpInfo, DIB_RGB_COLORS, SRCCOPY); 
	
	//��ʾʮ�ֲο���
	//CPen pPenBlue;
	//pPenBlue.CreatePen(PS_SOLID,1,RGB(0,0,128));
	//CPen *pOldPen=pdc->SelectObject(&pPenBlue);
	//double dbRateX = (double)rc.Width() /(double)m_RightWidth;
	//double dbRateY = (double)rc.Height() /(double)m_RightHeight;
	//double x1 = m_RightWidth*dbRateX;
	//double x2 = x1/2;
	//double y1 = m_RightHeight*dbRateY;
	//double y2 = y1/2;
	//pdc->MoveTo (0,y2);
	//pdc->LineTo (x1,y2);
	//pdc->MoveTo (x2,0);
	//pdc->LineTo (x2,y1);
	//pdc->SelectObject(pOldPen);
	//pPenBlue.DeleteObject();

	GetDlgItem(IDC_RightPic)->ReleaseDC(pdc);	
	m_CSectionR.Unlock();
}

void CBSVDlg::OnBnClickedOpencamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������	
	MVSTATUS_CODES hr;
	if (m_LeftCamera.CameraHandle == NULL)	//�������
	{
		hr = MVOpenCamByIndex(0, &m_LeftCamera.CameraHandle );
		if (hr != MVST_SUCCESS)
		{
			MessageBox("δ�ҵ��������ȷ���豸���Ӻ�IP���ã�","BSV",MB_ICONWARNING);
			return;
		}
		else if (hr == MVST_SUCCESS)
		{
			GetDlgItem(IDC_OpenCamera)->SetWindowText("�ر����");
			GetDlgItem(IDC_CapVideo)->EnableWindow(true);
			GetDlgItem(IDC_SetCamera)->EnableWindow(true);
		}
	}
	else	//�ر����
	{
		hr = MVStopGrab(m_LeftCamera.CameraHandle);
		hr = MVCloseCam(m_LeftCamera.CameraHandle);
		m_LeftCamera.CameraHandle = NULL;

		GetDlgItem(IDC_OpenCamera)->SetWindowText("�������");
		GetDlgItem(IDC_CapVideo)->EnableWindow(false);
		GetDlgItem(IDC_SetCamera)->EnableWindow(false);
	}
}

void CBSVDlg::OnBnClickedCapvideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_LeftCamera.CameraHandle != NULL)
	{
		MVGetWidth(m_LeftCamera.CameraHandle, &m_LeftOriWidth);  //���ͼ�񳤿�
		MVGetHeight(m_LeftCamera.CameraHandle,&m_LeftOriHeight);
		MVGetPixelFormat(m_LeftCamera.CameraHandle,&m_LeftCamera.PixelFormat);  //������ݸ�ʽ
		if( m_LeftCamera.PixelFormat == PixelFormat_Mono8 )
		{
			m_nLBit = 8;
		}
		else
		{
			m_nLBit = 24;
		}
		
		MVSTATUS_CODES hr;
		hr =  MVStartGrab(m_LeftCamera.CameraHandle, LeftCallbackFunction, (long)this); //���ûص�����
		if (hr == MVST_SUCCESS)
		{	
			//ԭʼͼ��
			m_pLBmpInfo->bmiHeader.biBitCount = m_nLBit;
			m_pLBmpInfo->bmiHeader.biWidth = m_LeftOriWidth;
			m_pLBmpInfo->bmiHeader.biHeight = m_LeftOriHeight;

			//Ϊͼ����ʾ��ͼ�����ݷ����ڴ�ռ�
			delete[] m_LeftShowDIBBits;
			m_LeftShowDIBBits = NULL;
			m_LeftShowDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];

			delete[] m_LeftOriDIBBits;
			m_LeftOriDIBBits = NULL;
			m_LeftOriDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];

			//Ϊ���д����ͼ��ֵ
			m_LeftRectLeft=0;
			m_LeftRectBottom=0;
			m_LeftWidth=m_LeftOriWidth;
			m_LeftHeight=m_LeftOriHeight;

			delete[] m_LeftDIBBits;
			m_LeftDIBBits = NULL;
			m_LeftDIBBits = new char[m_LeftWidth * m_LeftHeight * (m_nLBit/8)];

			//��ʼ�ɼ�ͼ��󣬲��ٽ��ܸ��µġ������ɼ���ָ��
			GetDlgItem(IDC_CapVideo)->EnableWindow(false);
			//GetDlgItem(IDC_Btn_SetCamera)->EnableWindow(false);
		}
	}
}

void CBSVDlg::OnBnClickedSetcamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_LeftCamera.CameraHandle == NULL )
	{
		MessageBox("��δ����������޷����ã�","BSV",MB_ICONWARNING);
	}
	else
	{
		//SetCameraDlg Dlg;	//�½�������öԻ���
		//if (Dlg.DoModal())
		//{
		//}
		MessageBox("���޴˹���");
	}
}

void CBSVDlg::OnBnClickedCircledetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_LeftShowDIBBits == NULL)
	{
		MessageBox("��ͼ�����ݣ�","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		SetTimer(1, 300, NULL);//���ö�ʱ��1
		m_TimerFlag = true;

		GetDlgItem(IDC_CircleDetect)->EnableWindow(false);
		GetDlgItem(IDC_StopDetect)->EnableWindow(true);
	}
}

void CBSVDlg::OnBnClickedStopdetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_TimerFlag)
	{
		KillTimer(1);
		m_TimerFlag = false;
	}

	GetDlgItem(IDC_CircleDetect)->EnableWindow(true);
	GetDlgItem(IDC_StopDetect)->EnableWindow(false);
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
	switch(nIDEvent)//nIDEvent Ϊ��ʱ���¼�ID��1��2��3
	{
	case 1:
		{
			if( m_LeftCamera.PixelFormat != PixelFormat_Mono8 )
				return;

			Mat LSrcImg;
			LSrcImg.create(m_LeftOriHeight, m_LeftOriWidth, CV_8UC1);

			g_pBSVDlg->m_LeftCamera.SectionCopy.Lock();
			memcpy(LSrcImg.data,m_LeftShowDIBBits,m_LeftOriWidth*m_LeftOriHeight*(m_nLBit/8));
			g_pBSVDlg->m_LeftCamera.SectionCopy.Unlock();
	
			Mat LFilImg;
			//˫���˲�������ȥ��
			//bilateralFilter(LSrcImg, LFilImg, 25, 25*2, 25/2);
			//��˹�˲�
			GaussianBlur(LSrcImg, LFilImg, Size(7,7), 0, 0);

			Mat LBinImg;
			//����Ӧ��ֵ�ָ�ͼ��	
			//adaptiveThreshold(LFilImg, LBinImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, 
			//CV_THRESH_BINARY, 7, 5);	
			//�̶���ֵ�ָ�ͼ�� lq:��ֵ��ͼ��
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
			if( m_RPixelFormat != PixelFormat_Mono8 )
				return;

			Mat RSrcImg;
			RSrcImg.create(m_RightOriHeight, m_RightOriWidth, CV_8UC1);
			g_pBSVDlg->m_CSectionCopyR.Lock();
			memcpy(RSrcImg.data,m_RightShowDIBBits,m_RightOriWidth*m_RightOriHeight*(m_nRBit/8));
			g_pBSVDlg->m_CSectionCopyR.Unlock();
	
			Mat RFilImg;
			//˫���˲�������ȥ��
			//bilateralFilter(RSrcImg, RFilImg, 25, 25*2, 25/2);
			//��˹�˲�
			GaussianBlur(RSrcImg, RFilImg, Size(7,7), 0, 0);

			Mat RBinImg;
			//����Ӧ��ֵ�ָ�ͼ��
			//adaptiveThreshold(RFilImg, RBinImg, 255, CV_ADAPTIVE_THRESH_MEAN_C,
			//CV_THRESH_BINARY, 7, 5);
			//�̶���ֵ�ָ�ͼ�� lq:��ֵ��ͼ��
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

void CBSVDlg::OnBnClickedOpencamera2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������	
	MVSTATUS_CODES hr;
	if (m_pRCamera == NULL)	//�������
	{
		hr = MVOpenCamByIndex(1, &m_pRCamera );
		if (hr != MVST_SUCCESS)
		{
			MessageBox("δ�ҵ��������ȷ���豸���Ӻ�IP���ã�","BSV",MB_ICONWARNING);
			return;
		}
		else if (hr == MVST_SUCCESS)
		{
			GetDlgItem(IDC_OpenCamera2)->SetWindowText("�ر����");
			GetDlgItem(IDC_CapVideo2)->EnableWindow(true);
			GetDlgItem(IDC_SetCamera2)->EnableWindow(true);
		}
	}
	else	//�ر����
	{
		hr = MVStopGrab(m_pRCamera);
		hr = MVCloseCam(m_pRCamera);
		m_pRCamera = NULL;

		GetDlgItem(IDC_OpenCamera2)->SetWindowText("�������");
		GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
		GetDlgItem(IDC_SetCamera2)->EnableWindow(false);
	}
}

void CBSVDlg::OnBnClickedCapvideo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pRCamera != NULL)
	{
		MVGetWidth(m_pRCamera, &m_RightOriWidth);  //���ͼ�񳤿�
		MVGetHeight(m_pRCamera,&m_RightOriHeight);
		MVGetPixelFormat(m_pRCamera,&m_RPixelFormat);  //������ݸ�ʽ
		if( m_RPixelFormat == PixelFormat_Mono8 )
		{
			m_nRBit = 8;
		}
		else
		{
			m_nRBit = 24;
		}
		
		MVSTATUS_CODES hr2;
		hr2 =  MVStartGrab(m_pRCamera, RightCallbackFunction, (long)this); //���ûص�����
		if (hr2 == MVST_SUCCESS)
		{	
			//ԭʼͼ��
			m_pRBmpInfo->bmiHeader.biBitCount = m_nRBit;
			m_pRBmpInfo->bmiHeader.biWidth = m_RightOriWidth;
			m_pRBmpInfo->bmiHeader.biHeight = m_RightOriHeight;

			//Ϊͼ����ʾ��ͼ�����ݷ����ڴ�ռ�
			delete[] m_RightShowDIBBits;
			m_RightShowDIBBits = NULL;
			m_RightShowDIBBits = new char[m_RightOriWidth * m_RightOriHeight * (m_nRBit/8)];

			delete[] m_RightOriDIBBits;
			m_RightOriDIBBits = NULL;
			m_RightOriDIBBits = new char[m_RightOriWidth * m_RightOriHeight * (m_nRBit/8)];

			//Ϊ���д����ͼ��ֵ
			m_RightRectLeft=0;
			m_RightRectBottom=0;
			m_RightWidth=m_RightOriWidth;
			m_RightHeight=m_RightOriHeight;

			delete[] m_RightDIBBits;
			m_RightDIBBits = NULL;
			m_RightDIBBits = new char[m_RightWidth * m_RightHeight * (m_nRBit/8)];

			//��ʼ�ɼ�ͼ��󣬲��ٽ��ܸ��µġ������ɼ���ָ��
			GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
			//GetDlgItem(IDC_Btn_SetCamera)->EnableWindow(false);
		}
	}
}

void CBSVDlg::OnBnClickedSetcamera2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pRCamera == NULL )
	{
		MessageBox("��δ����������޷����ã�","BSV",MB_ICONWARNING);
	}
	else
	{
		//SetCameraDlg Dlg;	//�½�������öԻ���
		//if (Dlg.DoModal())
		//{
		//}
		MessageBox("���޴˹���");
	}
}

void CBSVDlg::OnBnClickedOpen2camera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedOpencamera();
	OnBnClickedOpencamera2();
}

void CBSVDlg::OnBnClickedCap2video()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedCapvideo();
	OnBnClickedCapvideo2();
}

/*
*�������ܣ��ͷ�BlobSeq�е�Blob�����ڴ棬���BlobSeq�е�����
*����˵��
CPtrArray* BlobSeq	   ���ͷ��ڴ���ſ���С�
*/
/*�����*/
void CBSVDlg::Blob_ReleaseLeftBlobSeq()
{
	if (!m_LeftCamera.BlobSeq.IsEmpty())
	{
		m_LeftCamera.BlobSeq.RemoveAll();
		m_LeftCamera.BlobSeq.FreeExtra();
	}
}

/*�����*/
void CBSVDlg::Blob_ReleaseRightBlobSeq()
{
	if (!m_RightBlobSeq.IsEmpty())
	{
		m_RightBlobSeq.RemoveAll();
		m_RightBlobSeq.FreeExtra();
	}
}

/*
*�������ܣ����Բ������
*����˵��
const CvArr* SrcImage         ������ĻҶ�ͼ����ֵ��ͼ��
double lowthresh, highthresh  ����ӦCanny��Ե���ĸߵ���ֵ
int LeastPixelNum             Ŀ��������������С���ص���Ŀ
double AspectRatio            �ݺ��
double Circularity            Բ�ζ�
*/
/*��������Բ��*/
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
			area = contourArea(contours[i]);		//��i�����������
			length = arcLength(contours[i],true);	//��i���������ܳ�
			cir = (12.56*area)/(length*length);		//��i��������Բ�ζ�
			if (cir > Circularity)
			{
				rect = fitEllipse(contours[i]);
				width = rect.size.width;	//��Ӿ��εĿ��
				height = rect.size.height;	//��Ӿ��εĸ߶�
				asp = height/width;			//�ݺ��
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

/*��������Բ��*/
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
			area = contourArea(contours[i]);		//��i�����������
			length = arcLength(contours[i],true);	//��i���������ܳ�
			cir = (12.56*area)/(length*length);		//��i��������Բ�ζ�
			if (cir > Circularity)
			{
				rect = fitEllipse(contours[i]);
				width = rect.size.width;	//��Ӿ��εĿ��
				height = rect.size.height;	//��Ӿ��εĸ߶�
				asp = height/width;			//�ݺ��
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

////*�������ܣ���ʱ��2�ص�����
//void CALLBACK CBSVDlg::Timer2(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime )
//{
//	
//}

/*
*�������ܣ�����ſ�������������ָ����Χ�ڵ��ſ�����
*����˵��
CPtrArray* BlobSeq     ��������ſ����
int MinArea,MaxArea    ָ�����ſ������Χ
*/
/*�����*/
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

/*�����*/
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

void CBSVDlg::ShowLeftCircles()
{
	CDC *pdc = GetDlgItem(IDC_LeftPic)->GetDC();		//��ãģ�
	CRect rc;
	GetDlgItem(IDC_LeftPic)->GetClientRect(&rc);		//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;	//����ģʽ

	// ������ɫ���ʶ���
	CBrush BrushRed(RGB(255, 0, 0));
	CBrush *pOldBrush;
	pOldBrush = pdc->SelectObject(&BrushRed);

	double dbRateX = (double)rc.Width()/(double)m_LeftOriWidth;
	double dbRateY = (double)rc.Height()/(double)m_LeftOriHeight;
	int ln = m_LeftCamera.BlobSeq.GetSize();
	for (int i=0;i<ln;i++)
	{
		Blob pB = m_LeftCamera.BlobSeq.GetAt(i);
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
		pdc->Ellipse(x-5, y-5, x+5, y+5);
	}

	pdc->SelectObject(pOldBrush);
	BrushRed.DeleteObject();
	ReleaseDC(pdc);
}

void CBSVDlg::ShowRightCircles()
{
	CDC *pdc = GetDlgItem(IDC_RightPic)->GetDC();		//��ãģ�
	CRect rc;
	GetDlgItem(IDC_RightPic)->GetClientRect(&rc);		//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;	//����ģʽ

	// ������ɫ���ʶ���
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

void CBSVDlg::OnBnClickedCircledetect2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_RightShowDIBBits == NULL)
	{
		MessageBox("��ͼ�����ݣ�","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		SetTimer(2, 300, NULL);//lq:���ö�ʱ��2
		m_TimerFlag2 = true;

		GetDlgItem(IDC_CircleDetect2)->EnableWindow(false);
		GetDlgItem(IDC_StopDetect2)->EnableWindow(true);
	}
}

void CBSVDlg::OnBnClickedStopdetect2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedCircledetect();
	OnBnClickedCircledetect2();
}

CString format_fraction(double data)
{
	CString result;     //��ʽ��֮���������Чλ��Ϊ6
	
	//�ݴ��������ݲ���Ϊ����
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
//*����˵����������궨��ɺ�����
//*�����������
//*�������: ��
//*�� �� ֵ��TRUE��ʾ�궨�ɹ�,FALSE��ʾʧ�ܡ�
//*/
//BOOL CBSVDlg::CalibFinishLeftCamera()
//{
//	//�����¼��ͼ������Բ�����
//	Blob_ReleaseLeftBlobSeq();	//lq:�иĶ�
//	//�����¼����ͼ��ı������
//	m_bLCalibDraw = false;//�û���������궨��־
//	OnPaint();
//
//    int left_num=0;
//	left_num=m_LeftImgCoordSeq.GetSize();
//	
//	//����궨����
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
//    CPtrArray * leftx = & m_LeftImgCoordSeq;
//	if (!Calibration_LeftCamera(leftx,leftICR,left_re))
//	{
//		for (int i=0;i<m_LeftImgCoordSeq.GetSize();i++)
//		{
//			Image_Coordation*	pIC;
//			pIC=(Image_Coordation*)m_LeftImgCoordSeq.GetAt(i);
//			delete pIC;
//			pIC=NULL;
//		}
//		m_LeftImgCoordSeq.RemoveAll();	
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
//    	GetDlgItem(IDC_LIST_BVS_ListFinish)->ShowWindow(SW_HIDE);//�궨���������ʾList
//	    m_static_FinishText.ShowWindow(SW_SHOW);//�궨�����ʾ��̬��
//
//		m_static_FinishText.SetWindowText(STR_CalibFailText);//STR_CalibFailTextӦ��ָ���ǡ��궨ʧ�ܡ�
//
//		return false; 
//	}
//	else
//	{
//		GetDlgItem(IDC_STATIC_ShouDInfor)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_LIST_BVS_ListFinish)->ShowWindow(SW_SHOW);//�궨���������ʾList
//		m_static_FinishText.ShowWindow(SW_SHOW);//�궨�����ʾ��̬��
//		
//		int left_grade;
//		CString ColumnName;
//		left_grade=Calibration_ComputeEvaluate(left_re,left_num);
//		
//		//��ʾ����궨����
//	
//
//		CString s;
//		s.Format("%s\n%s\nk1=%s, k2=%s\n",STR_LCalibFinishText_1,STR_LCalibFinishText_2,format_fraction(leftICR.k1), format_fraction(leftICR.k2)) ;
//	//	s.Format("������궨���:\n����ϵ��:\nk1=%s, k2=%s\n",format_fraction(leftICR.k1), format_fraction(leftICR.k2)) ;
//	
//		strCalibrate += s;
//		
//		s.Format("%s\n",STR_LCalibFinishText_3);
//	//	s = "������ڲξ���A:\n";
//		strCalibrate += s;
//		
//		s.Format("%s %s  %s\n%s %s  %s\n%s %s  %s\n",format_fraction(leftICR.fx) , format_fraction(leftICR.skewness), 
//			format_fraction(leftICR.u0), format_fraction(zero),  format_fraction(leftICR.fy),
//			format_fraction(leftICR.v0), format_fraction(zero), format_fraction(zero), format_fraction(unit));
//		strCalibrate += s;
//		
//		s.Format("%s%s",STR_LCalibFinishText_4,g_strEvaluate[left_grade]);
//	//	s = "������������:" + strEvaluate[left_grade];
//		strCalibrate += s;
//		m_static_FinishText.SetWindowText(strCalibrate);
//
//		//���������
//		while(m_list_Finish.DeleteColumn(0))
//		{
//			
//		}
//		m_list_Finish.DeleteAllItems();
//		m_list_Finish.InsertColumn(0, STR_LCalibFinishList_1,LVCFMT_IMAGE|LVCFMT_LEFT);  //�����1��
//		
//		//���ֻ�궨�����
//		int i;
//		for (i = 1; i <= left_num; i++)
//		{
//			ColumnName.Format("%s%d",STR_LCalibFinishList_2, i);
//			m_list_Finish.InsertColumn(i, ColumnName);//�����1��
//			
//		}
//		
//		m_list_Finish.SetColumnWidth(0, 70);
//		int j;
//		for(j = 1; j <= left_num; j++)
//		{
//			m_list_Finish.SetColumnWidth(j, 60);//�����еĿ�
//		}
//		
//		for (i = 0; i < 99; i++)
//		{
//			//���ֶε�ֵ
//			ColumnName.Format("%d", i);
//			
//			m_list_Finish.InsertItem(i,0);//����һ��
//			m_list_Finish.SetItemText(i,0,ColumnName);//���е�CounterΪ�кţ�0Ϊ�кż��ֶκ�
//			
//			for(int k = 1; k <= left_num; k++)
//			{
//				ColumnName.Format("%.3f", left_re[k-1][i]);
//				m_list_Finish.SetItemText(i,k,ColumnName);//���е�CounterΪ�кţ�0Ϊ�кż��ֶκ�
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
//		for ( i=0;i<m_LeftImgCoordSeq.GetSize();i++)
//		{
//			Image_Coordation*	pIC;
//			pIC=(Image_Coordation*)m_LeftImgCoordSeq.GetAt(i);
//			delete pIC;
//			pIC=NULL;
//		}
//		m_LeftImgCoordSeq.RemoveAll();
//		
//		return true;
//	}
//	return false;
//}
///*
//*����˵����������궨��ɺ�����
//*�����������
//*�������: ��
//*�� �� ֵ��TRUE��ʾ�궨�ɹ�,FALSE��ʾʧ�ܡ�
//*/
//BOOL CBSVDlg::CalibFinishRightCamera()
//{
//	//�����¼��ͼ������Բ�����
//	Blob_ReleaseLeftBlobSeq();
//	//�����¼����ͼ��ı������
//	m_bRCalibDraw = false;//�û���������궨��־
//	OnPaint();
//
//    int right_num=0;
//	right_num=m_RightImgCoordSeq.GetSize();
//	//����궨����
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
//	if (!Calibration_RightCamera(&m_RightImgCoordSeq,rightICR,right_re))
//	{
//		for (int i=0;i<m_RightImgCoordSeq.GetSize();i++)
//		{
//			Image_Coordation*	pIC;
//			pIC=(Image_Coordation*)m_RightImgCoordSeq.GetAt(i);
//			delete pIC;
//			pIC=NULL;
//		}
//		m_RightImgCoordSeq.RemoveAll();	
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
//    	GetDlgItem(IDC_LIST_BVS_ListFinish)->ShowWindow(SW_HIDE);//�궨���������ʾList
//	    m_static_FinishText.ShowWindow(SW_SHOW);//�궨�����ʾ��̬��
//
//		m_static_FinishText.SetWindowText(STR_CalibFailText);
//
//		return false;
//	}
//	else
//	{
//		GetDlgItem(IDC_STATIC_ShouDInfor)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_LIST_BVS_ListFinish)->ShowWindow(SW_SHOW);//�궨���������ʾList
//	    m_static_FinishText.ShowWindow(SW_SHOW);//�궨�����ʾ��̬��
//
//		
//		int right_grade;
//		CString ColumnName;
//		right_grade=Calibration_ComputeEvaluate(right_re,right_num);
//		
//		//��ʾ����궨����
//	
//
//		CString s;
//		s.Format("%s\n%s\nk1=%s, k2=%s\n",STR_RCalibFinishText_1,STR_RCalibFinishText_2,format_fraction(rightICR.k1), format_fraction(rightICR.k2)) ;
//	//	s.Format("������궨���:\n����ϵ��:\nk1=%s, k2=%s\n",format_fraction(rightICR.k1), format_fraction(rightICR.k2)) ;
//	
//		strCalibrate += s;
//		
//		s.Format("%s\n",STR_RCalibFinishText_3);
//	//	s = "������ڲξ���A:\n";
//		strCalibrate += s;
//		
//		s.Format("%s %s  %s\n%s %s  %s\n%s %s  %s\n",format_fraction(rightICR.fx) , format_fraction(rightICR.skewness), 
//			format_fraction(rightICR.u0), format_fraction(zero),  format_fraction(rightICR.fy),
//			format_fraction(rightICR.v0), format_fraction(zero), format_fraction(zero), format_fraction(unit));
//		strCalibrate += s;
//		
//		s.Format("%s%s",STR_RCalibFinishText_4,g_strEvaluate[right_grade]);
//	//	s = "������������:" + strEvaluate[right_grade];
//		strCalibrate += s;
//		m_static_FinishText.SetWindowText(strCalibrate);
//
//		//���������
//		while(m_list_Finish.DeleteColumn(0))
//		{
//			
//		}
//		m_list_Finish.DeleteAllItems();
//		m_list_Finish.InsertColumn(0, STR_RCalibFinishList_1,LVCFMT_IMAGE|LVCFMT_LEFT);  //�����1��
//		
//		int i;
//		for (i = 1; i <= right_num; i++)
//		{
//			ColumnName.Format("%s%d",STR_RCalibFinishList_2, i);
//			m_list_Finish.InsertColumn(i, ColumnName);//�����1��
//			
//		}
//		
//		m_list_Finish.SetColumnWidth(0, 70);
//		for(int j = 1; j <= right_num; j++)
//		{
//			m_list_Finish.SetColumnWidth(j, 60);//�����еĿ�
//		}
//		
//		for (i = 0; i < 99; i++)
//		{
//			//���ֶε�ֵ
//			ColumnName.Format("%d", i);
//			
//			m_list_Finish.InsertItem(i,0);//����һ��
//			m_list_Finish.SetItemText(i,0,ColumnName);//���е�CounterΪ�кţ�0Ϊ�кż��ֶκ�
//			int k;
//			for(k = 1; k <= right_num; k++)
//			{
//				ColumnName.Format("%.3f", right_re[k-1][i]);
//				m_list_Finish.SetItemText(i,k,ColumnName);//���е�CounterΪ�кţ�0Ϊ�кż��ֶκ�
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
//		for ( i=0;i<m_RightImgCoordSeq.GetSize();i++)
//		{
//			Image_Coordation*	pIC;
//			pIC=(Image_Coordation*)m_RightImgCoordSeq.GetAt(i);
//			delete pIC;
//			pIC=NULL;
//		}
//		m_RightImgCoordSeq.RemoveAll();
//		return true;
//	}
//
//   return false;
//}

void CBSVDlg::OnBnClickedCalibration()
{
	 //TODO: �ڴ���ӿؼ�֪ͨ����������
	CString a, b ;
	a.Format(_T("LCamera_calibdata.txt"));
	b.Format(_T("LCamera_caliberation_result.txt"));
	string A((LPCTSTR)a);
	string B((LPCTSTR)b);

	row_corner_num = 0;
	column_corner_num = 0;
	grid_width = 0;
	grid_height = 0;
	CCali_ParaDlg dlg;
	dlg.DoModal();
	/*����һ��Ӧ�ü�һ���ж���䣬�������û��������ȫ�Ͳ�ִ������ı궨���*/
	if(row_corner_num==0|column_corner_num==0|grid_width==0|grid_height==0)
	{
		AfxMessageBox(_T("δ�����������"));
	}
	else
	{
		CalibrationLeft(A,B,row_corner_num,column_corner_num,grid_width,grid_height);
	}
}

void CBSVDlg::OnBnClickedCalibration2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString a, b ;
	a.Format(_T("RCamera_calibdata.txt"));
	b.Format(_T("RCamera_caliberation_result.txt"));
	string A((LPCTSTR)a);
	string B((LPCTSTR)b);

	CCali_ParaDlg dlg;
	dlg.DoModal();
	CalibrationLeft(A,B,row_corner_num,column_corner_num,grid_width,grid_height);
}

void CBSVDlg::OnBnClickedSavepic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ͼ����Ori_LImage�У���ΪͼƬ����Lchess*.bmp
	if(m_LeftCamera.CameraHandle == NULL)
	{
		AfxMessageBox("û�д������");
		return;
	}
	m_LeftCamera.ImageIndex++;

	if( m_LeftCamera.PixelFormat != PixelFormat_Mono8 )
		return;
	Mat LImg;
	LImg.create(m_LeftOriHeight, m_LeftOriWidth, CV_8UC1);

	g_pBSVDlg->m_LeftCamera.SectionCopy.Lock();
	memcpy(LImg.data,m_LeftShowDIBBits,m_LeftOriWidth*m_LeftOriHeight*(m_nLBit/8));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pRCamera == NULL)
	{
		AfxMessageBox("û�д������");
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
	string RfilePath="Ori_RImage\\chess";
	StrStm<<m_nRImageIndex;
 	StrStm>>imageFileName;
	RfilePath += imageFileName;
	RfilePath+= ".bmp";
	imwrite(RfilePath,RImg);
}

//void CBSVDlg::OnBnClickedButton2()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	
//}
