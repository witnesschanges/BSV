
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

	delete[] m_RightImage.ShowDIBBits;
	m_LeftImage.ShowDIBBits = NULL;
	delete[] m_LeftImage.DIBBits;
	m_LeftImage.DIBBits = NULL;
	delete[] m_LeftImage.OriDIBBits;
	m_LeftImage.OriDIBBits = NULL;

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
	if(g_pBSVDlg->m_LeftImage.OriDIBBits != NULL)
	{
		//����ͼ���ڴ�
		g_pBSVDlg->m_LeftCamera.SectionCopy.Lock();

		//Ϊԭʼͼ��ֵ�������24λRGB��ʽ����ֱ��ת��
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

		g_pBSVDlg->m_LeftImage.BufHandle = true;//��ͼ����Խ��д����ʶ
		g_pBSVDlg->DrawLeftCamera();
	}
	return 1;
}

//*****���º���������������д*****//
int RightCallbackFunction(MV_IMAGE_INFO *pInfo,long nUserVal)
	// int index, LPSTR pData, int Length, void* pUserData )
{
	if(g_pBSVDlg->m_RightImage.OriDIBBits != NULL)//������
	{
		//����ͼ���ڴ�
		g_pBSVDlg->m_RightCamera.SectionCopy.Lock();

		//Ϊԭʼͼ��ֵ�������24λRGB��ʽ����ֱ��ת��
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

		g_pBSVDlg->m_RightImage.BufHandle = true;//��ͼ����Խ��д����ʶ
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
		0,0,m_LeftImage.OriWidth,m_LeftImage.OriHeight,
		m_LeftImage.ShowDIBBits,(LPBITMAPINFO) m_LeftImage.BmpInfo, DIB_RGB_COLORS, SRCCOPY); 
	
	//��ʾʮ�ֲο���
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
*����˵������ʾ�����ͼ��
*�����������
*          
*����������ޡ�
*�� �� ֵ���ޡ�
*/
void CBSVDlg::DrawRightCamera()
{
	m_RightCamera.Section.Lock();
	CDC *pdc = GetDlgItem(IDC_RightPic)->GetDC();//���DC
	CRect rc;
	GetDlgItem(IDC_RightPic)->GetClientRect(&rc);//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;//����ģʽ

	//��ʾͼ��
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(),rc.Height(),
		0,0,m_RightImage.OriWidth,m_RightImage.OriHeight,
		m_RightImage.ShowDIBBits,(LPBITMAPINFO) m_RightImage.BmpInfo, DIB_RGB_COLORS, SRCCOPY); 
	
	//��ʾʮ�ֲο���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������
	OpenCamera(m_LeftCamera, IDC_OpenCamera, IDC_CapVideo, IDC_SetCamera);

}

void CBSVDlg::OnBnClickedCapvideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_LeftCamera.CameraHandle != NULL)
	{
		MVGetWidth(m_LeftCamera.CameraHandle, &m_LeftImage.OriWidth);  //���ͼ�񳤿�
		MVGetHeight(m_LeftCamera.CameraHandle,&m_LeftImage.OriHeight);
		MVGetPixelFormat(m_LeftCamera.CameraHandle,&m_LeftCamera.PixelFormat);  //������ݸ�ʽ
		if( m_LeftCamera.PixelFormat == PixelFormat_Mono8 )
		{
			m_LeftImage.Bit = 8;
		}
		else
		{
			m_LeftImage.Bit = 24;
		}
		
		MVSTATUS_CODES hr;
		hr =  MVStartGrab(m_LeftCamera.CameraHandle, LeftCallbackFunction, (long)this); //���ûص�����
		if (hr == MVST_SUCCESS)
		{	
			//ԭʼͼ��
			m_LeftImage.BmpInfo->bmiHeader.biBitCount = m_LeftImage.Bit;
			m_LeftImage.BmpInfo->bmiHeader.biWidth = m_LeftImage.OriWidth;
			m_LeftImage.BmpInfo->bmiHeader.biHeight = m_LeftImage.OriHeight;

			//Ϊͼ����ʾ��ͼ�����ݷ����ڴ�ռ�
			delete[] m_LeftImage.ShowDIBBits;
			m_LeftImage.ShowDIBBits = NULL;
			m_LeftImage.ShowDIBBits = new char[m_LeftImage.OriWidth * m_LeftImage.OriHeight * (m_LeftImage.Bit/8)];

			delete[] m_LeftImage.OriDIBBits;
			m_LeftImage.OriDIBBits = NULL;
			m_LeftImage.OriDIBBits = new char[m_LeftImage.OriWidth * m_LeftImage.OriHeight * (m_LeftImage.Bit/8)];

			//Ϊ���д����ͼ��ֵ
			m_LeftImage.Width=m_LeftImage.OriWidth;
			m_LeftImage.Height=m_LeftImage.OriHeight;

			delete[] m_LeftImage.DIBBits;
			m_LeftImage.DIBBits = NULL;
			m_LeftImage.DIBBits = new char[m_LeftImage.Width * m_LeftImage.Height * (m_LeftImage.Bit/8)];

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
	if (m_LeftImage.ShowDIBBits == NULL)
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
			LSrcImg.create(m_LeftImage.OriHeight, m_LeftImage.OriWidth, CV_8UC1);

			g_pBSVDlg->m_LeftCamera.SectionCopy.Lock();
			memcpy(LSrcImg.data,m_LeftImage.ShowDIBBits,m_LeftImage.OriWidth*m_LeftImage.OriHeight*(m_LeftImage.Bit/8));
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
			if( m_RightCamera.PixelFormat != PixelFormat_Mono8 )
				return;

			Mat RSrcImg;
			RSrcImg.create(m_RightImage.OriHeight, m_RightImage.OriWidth, CV_8UC1);
			g_pBSVDlg->m_RightCamera.SectionCopy.Lock();
			memcpy(RSrcImg.data,m_RightImage.ShowDIBBits,m_RightImage.OriWidth*m_RightImage.OriHeight*(m_RightImage.Bit/8));
			g_pBSVDlg->m_RightCamera.SectionCopy.Unlock();
	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������	
	OpenCamera(m_RightCamera, IDC_OpenCamera2, IDC_CapVideo2, IDC_SetCamera2);
}

void CBSVDlg::OnBnClickedCapvideo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_RightCamera.CameraHandle != NULL)
	{
		MVGetWidth(m_RightCamera.CameraHandle, &m_RightImage.OriWidth);  //���ͼ�񳤿�
		MVGetHeight(m_RightCamera.CameraHandle,&m_RightImage.OriHeight);
		MVGetPixelFormat(m_RightCamera.CameraHandle,&m_RightCamera.PixelFormat);  //������ݸ�ʽ
		if( m_RightCamera.PixelFormat == PixelFormat_Mono8 )
		{
			m_RightImage.Bit = 8;
		}
		else
		{
			m_RightImage.Bit = 24;
		}
		
		MVSTATUS_CODES hr2;
		hr2 =  MVStartGrab(m_RightCamera.CameraHandle, RightCallbackFunction, (long)this); //���ûص�����
		if (hr2 == MVST_SUCCESS)
		{	
			//ԭʼͼ��
			m_RightImage.BmpInfo->bmiHeader.biBitCount = m_RightImage.Bit;
			m_RightImage.BmpInfo->bmiHeader.biWidth = m_RightImage.OriWidth;
			m_RightImage.BmpInfo->bmiHeader.biHeight = m_RightImage.OriHeight;

			//Ϊͼ����ʾ��ͼ�����ݷ����ڴ�ռ�
			delete[] m_RightImage.ShowDIBBits;
			m_RightImage.ShowDIBBits = NULL;
			m_RightImage.ShowDIBBits = new char[m_RightImage.OriWidth * m_RightImage.OriHeight * (m_RightImage.Bit/8)];

			delete[] m_RightImage.OriDIBBits;
			m_RightImage.OriDIBBits = NULL;
			m_RightImage.OriDIBBits = new char[m_RightImage.OriWidth * m_RightImage.OriHeight * (m_RightImage.Bit/8)];

			//Ϊ���д����ͼ��ֵ
			m_RightImage.Width=m_RightImage.OriWidth;
			m_RightImage.Height=m_RightImage.OriHeight;

			delete[] m_RightImage.DIBBits;
			m_RightImage.DIBBits = NULL;
			m_RightImage.DIBBits = new char[m_RightImage.Width * m_RightImage.Height * (m_RightImage.Bit/8)];

			//��ʼ�ɼ�ͼ��󣬲��ٽ��ܸ��µġ������ɼ���ָ��
			GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
			//GetDlgItem(IDC_Btn_SetCamera)->EnableWindow(false);
		}
	}
}

void CBSVDlg::OnBnClickedSetcamera2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_RightCamera.CameraHandle == NULL )
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
	if (!m_RightCamera.BlobSeq.IsEmpty())
	{
		m_RightCamera.BlobSeq.RemoveAll();
		m_RightCamera.BlobSeq.FreeExtra();
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
	m_RightCamera.BlobSeq.SetSize(ln,10);
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

					m_RightCamera.BlobSeq.SetAtGrow(cnt,pB);
					cnt++;
				}
			}
		}
	}
	m_RightCamera.BlobSeq.FreeExtra();
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
	CDC *pdc = GetDlgItem(IDC_LeftPic)->GetDC();		//��ãģ�
	CRect rc;
	GetDlgItem(IDC_LeftPic)->GetClientRect(&rc);		//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;	//����ģʽ

	// ������ɫ���ʶ���
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
	CDC *pdc = GetDlgItem(IDC_RightPic)->GetDC();		//��ãģ�
	CRect rc;
	GetDlgItem(IDC_RightPic)->GetClientRect(&rc);		//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;	//����ģʽ

	// ������ɫ���ʶ���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_RightImage.ShowDIBBits == NULL)
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
//	m_LeftCalibration.CalibDraw = false;//�û���������궨��־
//	OnPaint();
//
//    int left_num=0;
//	left_num=m_LeftCalibration.CoordSeq.GetSize();
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
//	m_RightCalibration.CalibDraw = false;//�û���������궨��־
//	OnPaint();
//
//    int right_num=0;
//	right_num=m_RightCalibration.CoordSeq.GetSize();
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
	 //TODO: �ڴ���ӿؼ�֪ͨ����������
	CString a, b ;
	a.Format(_T("LCamera_calibdata.txt"));
	b.Format(_T("LCamera_caliberation_result.txt"));
	string A((LPCTSTR)a);
	string B((LPCTSTR)b);

	m_Calibration.Initial();
	CCali_ParaDlg dlg;
	dlg.DoModal();
	/*����һ��Ӧ�ü�һ���ж���䣬�������û��������ȫ�Ͳ�ִ������ı궨���*/
	if(m_Calibration.RowCornerNum==0|m_Calibration.ColumCornerNum==0|m_Calibration.GridWidth==0|m_Calibration.GridHeight==0)
	{
		AfxMessageBox(_T("δ�����������"));
	}
	else
	{
		CalibrationLeft(A,B,m_Calibration.RowCornerNum,m_Calibration.ColumCornerNum,m_Calibration.GridWidth,m_Calibration.GridHeight);
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
	CalibrationLeft(A,B,m_Calibration.RowCornerNum,m_Calibration.ColumCornerNum,m_Calibration.GridWidth,m_Calibration.GridHeight);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_RightCamera.CameraHandle == NULL)
	{
		AfxMessageBox("û�д������");
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];		//����ͼ����Ϣͷ
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
