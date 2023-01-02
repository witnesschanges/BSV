
// BSVDlg.h : 头文件
//

#pragma once
#include <afxwin.h>
#include "MVGigE.h"
#include "MVImage.h"
#include "FeatureDetect.h"
#include "Camera.h"
#include "Image.h"
#include "Calibration.h"
#include "mscomm.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

const int WaitOutTime = 3000;
const float pi = 3.141592653;
const int PictureControlWidth = 520;
const int PictureControlHight = 390;
const string LeftCameraCalibdataText = "LCamera_calibdata.txt";
const string RightCameraCalibdataText = "RCamera_calibdata.txt";
const string LeftCameraCalibrationResultText = "LCamera_caliberation_result.txt";
const string RightCameraCalibrationResultText = "RCamera_caliberation_result.txt";
const UINT_PTR FirstTimerId = 1;
const UINT_PTR SecondTimerId = 2;
const UINT CircleDetectTimerElapse = 300;

// CBSVDlg 对话框
class CBSVDlg : public CDialogEx
{
// 构造
public:
	CBSVDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BSV_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();

	// 生成的消息映射函数
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
	//todo：business logic seperate from prensentation logic
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
	void ShowCircles(Camera& camera, Image& image, UINT32 openCameraId);
	void RecognizeCircle(Camera& camera, Image image);
	void DrawCamera(Camera& camera, Image& image, UINT32 picId);
	void SetPictureSize(UINT32 picId);
	void LocateCursorMiddle(CDC& dc);

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
	bool m_TimerFlag;//定时器标志
	bool m_TimerFlag2;//定时器2标志
	CWinThread* m_IAAThread;
	CEvent m_EVKillIAA;//停止连续调整线程
	CEvent m_EVDeadIAA;//连续调整线程结束
	bool m_ThreadFlag;//标定线程运行标记,TRUE表示运行,FALSE表示停止
};
