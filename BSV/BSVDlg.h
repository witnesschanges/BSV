
// BSVDlg.h : ͷ�ļ�
//

#pragma once
#include <afxwin.h>
#include "MVGigE.h"
#include "MVImage.h"
#include "FeatureDetect.h"
#include "mscomm.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

#define WaitOutTime 3000
#define pi 3.141592653

class Camera
{
public:
	void Initial();
public:
	HANDLE CameraHandle;//������
	CCriticalSection Section;//�����ͼ�ٽ���
	CCriticalSection SectionCopy;//�����ͼ�ٽ���
	int ImageIndex;//����ɼ�ͼƬ��ű�־
	//CPtrArray BlobSeq;
	CArray<Blob> BlobSeq;//��¼���Բ��������������
	MV_PixelFormatEnums PixelFormat;//���ͼ���ʽ
};

class Image
{
public:
	void Initial();
	void InitialBmpInfo();
public:
	BITMAPINFO* BmpInfo;//ͼ����Ϣͷָ��
	LPSTR OriDIBBits;//ԭʼͼ�񣬴�ͼ���ļ�����������ֱ�ӻ�ȡ���Ҷ�ͼ�������RGBֱ��ת��Ϊ8λ�Ҷ�ͼ
	LPSTR DIBBits;//Ҫ���д����ͼ���п���������ͼ��Ҳ�п�����ԭʼͼ���һ������
	LPSTR ShowDIBBits;//Ҫ������ʾ��ͼ��
	LONG Width;//Ҫ���д����ͼ��Ŀ�ȣ������ͼ��������д����ÿ����ԭʼͼ��Ŀ���ǲ�ͬ��
	LONG Height;//Ҫ���д����ͼ��ĸ߶ȣ������ͼ��������д����ÿ����ԭʼͼ��Ŀ���ǲ�ͬ��
	LONG RectLeft;//ͼ��ѡ���������߽�
	LONG RectBottom;//ͼ��ѡ��������±߽�
	int Bit;//ͼ��λ��
	int OriWidth;//ͼ��ԭʼ���
	int OriHeight;//ͼ��ԭʼ�߶�
	BOOL BufHandle;//ͼ����Խ��д����ʶ,��ÿ�ε���������ص�������,�ñ����true,����ͼ����д���ʱ,�ñ����false
};

//˫Ŀ�������
class Calibration
{
public:
	void Initial();
public:
	BOOL MeasureDraw;//˫�����ʾ����������,true��ʾ��ʾ,false��ʾ����ʾ(Not used yet)
	int ImageIndex;//�ɼ�ͼ����(Not used yet)
	int RowCornerNum;//�궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
	int ColumCornerNum;//�궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
	int GridWidth;//ʵ�ʲ����õ��ı궨����ÿ�����̸�ĳ�
	int GridHeight;//ʵ�ʲ����õ��ı궨����ÿ�����̸�Ŀ�
};

//������궨����
class SingleCalibration
{
public:
	BOOL CalibDraw;//�����ʾ�궨λ�ñ��,true��ʾ��ʾ,false��ʾ����ʾ
	CPtrArray ImgCoordSeq;//��¼����ͼ��ı������
};

// CBSVDlg �Ի���
class CBSVDlg : public CDialogEx
{
// ����
public:
	CBSVDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BSV_DIALOG };

public:
	Camera m_LeftCamera;
	Camera m_RightCamera;
	Image m_LeftImage;
	Image m_RightImage;
	SingleCalibration m_LeftCalibration;
	SingleCalibration m_RightCalibration;
	Calibration m_Calibration;

	bool    m_TimerFlag;	    //��ʱ����־
	bool    m_TimerFlag2;       //��ʱ��2��־
	CWinThread *m_IAAThread;

	CEvent m_EVKillIAA;		//ֹͣ���������߳�
	CEvent m_EVDeadIAA;		//���������߳̽���
	bool   m_ThreadFlag;	//�궨�߳����б��,TRUE��ʾ����,FALSE��ʾֹͣ

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public://lq:��afx_msg��ͷ������Ϣ������
	afx_msg void OnBnClickedOpencamera();
	afx_msg void OnBnClickedCapvideo();
	afx_msg void OnBnClickedSetcamera();
	afx_msg void OnBnClickedCircledetect();
	void DrawLeftCamera();
	void DrawRightCamera();
	afx_msg void OnBnClickedOpencamera2();
	afx_msg void OnBnClickedCapvideo2();
	afx_msg void OnBnClickedSetcamera2();
	afx_msg void OnBnClickedStopdetect();
	afx_msg void OnBnClickedOpen2camera();
	afx_msg void OnBnClickedCap2video();
	void Blob_ReleaseLeftBlobSeq();
	void Blob_ReleaseRightBlobSeq();
	void Detect_LeftCircleDetect(InputArray SrcImg, double lowthresh, 
		double highthresh, double AspectRatio, double Circularity);
	void Detect_RightCircleDetect(InputArray SrcImg, double lowthresh, 
		double highthresh, double AspectRatio, double Circularity);
	void Blob_DenoisingLeftArea(int MinArea, int MaxArea);
	void Blob_DenoisingRightArea(int MinArea, int MaxArea);
	void ShowLeftCircles();
	void ShowRightCircles();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedCircledetect2();
	afx_msg void OnBnClickedStopdetect2();
	afx_msg void OnBnClickedCircle2detect();
	BOOL CalibFinishLeftCamera();
	BOOL CalibFinishRightCamera();

	public:
	CRichEditCtrl	m_static_FinishText;
	CListCtrl	m_list_Finish;
	void StopLeftCamera();
	afx_msg void OnBnClickedCalibration();
	afx_msg void OnBnClickedSavepic();
	afx_msg void OnBnClickedSavepic2();
	afx_msg void OnBnClickedCalibration2();
};
