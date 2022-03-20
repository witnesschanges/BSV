
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

enum CameraType
{
	Left,
	Right
};

class Camera
{
public:
	void Initial();
public:
	HANDLE CameraHandle;//������
	CameraType type; // todo: ��enum�����滻flag
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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();

	// ���ɵ���Ϣӳ�亯��
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOpencamera();
	afx_msg void OnBnClickedOpencamera2();
	afx_msg void OnBnClickedCapvideo();
	afx_msg void OnBnClickedCapvideo2();
	afx_msg void OnBnClickedSetcamera();
	afx_msg void OnBnClickedSetcamera2();
	afx_msg void OnBnClickedCircledetect();
	afx_msg void OnBnClickedCircledetect2();
	afx_msg void OnBnClickedStopdetect();
	afx_msg void OnBnClickedStopdetect2();
	afx_msg void OnBnClickedSavepic();
	afx_msg void OnBnClickedSavepic2();
	afx_msg void OnBnClickedCalibration();
	afx_msg void OnBnClickedCalibration2();

	afx_msg void OnBnClickedCircle2detect();
	afx_msg void OnBnClickedOpen2camera();
	afx_msg void OnBnClickedCap2video();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

public:
	void ShowLeftCircles();
	void ShowRightCircles();
	void DrawLeftCamera();
	void DrawRightCamera();
	BOOL CalibFinishLeftCamera();
	BOOL CalibFinishRightCamera();
	void StopLeftCamera();

public:
	//todo��business logic seperate from prentation logic
	void OpenCamera(Camera &camera, UINT32 openCameraId, UINT32 capVideoId, UINT32 setCameraId);
	void CapVideo(Camera &camera, Image &image, UINT32 capVideoId, int (*StreamCB)(MV_IMAGE_INFO*, long nUserVal));
	void SetCamera(Camera& camera);
	void Circledetect(Image &image, UINT_PTR nIDEvent, UINT nElapse, bool timerflag, UINT32 circleDetectId, UINT32 stopDetectId);
	void Stopdetect(bool timerFlag, UINT_PTR nIDEvent, UINT32 circleDetectId, UINT32 stopDetectId);
	void Savepic(Camera& camera, Image& image, bool isLeft);
	void Calibrate(bool isLeft);

	void ReleaseBlobSeq(CArray<Blob>& BlobSeq);
	void DetectCircle(Camera& camera, InputArray srcImg, double lowthresh,
		double highthresh, double aspectRatio, double circularity);
	void DenoisingBlobArea(CArray<Blob> &blob, int minArea, int maxArea);

public:
	Camera m_LeftCamera;
	Camera m_RightCamera;
	Image m_LeftImage;
	Image m_RightImage;
	SingleCalibration m_LeftCalibration;
	SingleCalibration m_RightCalibration;
	Calibration m_Calibration;

	CRichEditCtrl m_static_FinishText;
	CListCtrl m_list_Finish;
	bool m_TimerFlag;//��ʱ����־
	bool m_TimerFlag2;//��ʱ��2��־
	CWinThread* m_IAAThread;
	CEvent m_EVKillIAA;//ֹͣ���������߳�
	CEvent m_EVDeadIAA;//���������߳̽���
	bool m_ThreadFlag;//�궨�߳����б��,TRUE��ʾ����,FALSE��ʾֹͣ
};
