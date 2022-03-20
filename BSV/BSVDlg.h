
// BSVDlg.h : 头文件
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
	HANDLE CameraHandle;//相机句柄
	CameraType type; // todo: 用enum类型替换flag
	CCriticalSection Section;//相机采图临界区
	CCriticalSection SectionCopy;//相机采图临界区
	int ImageIndex;//相机采集图片序号标志
	//CPtrArray BlobSeq;
	CArray<Blob> BlobSeq;//记录相机圆形特征中心坐标
	MV_PixelFormatEnums PixelFormat;//相机图像格式
};

class Image
{
public:
	void Initial();
	void InitialBmpInfo();
public:
	BITMAPINFO* BmpInfo;//图像信息头指针
	LPSTR OriDIBBits;//原始图像，从图像文件读入或相机中直接获取。灰度图像，如果是RGB直接转化为8位灰度图
	LPSTR DIBBits;//要进行处理的图像，有可能是整幅图像也有可能是原始图像的一个区域
	LPSTR ShowDIBBits;//要进行显示的图像
	LONG Width;//要进行处理的图像的宽度，如果对图像区域进行处理，该宽度与原始图像的宽度是不同的
	LONG Height;//要进行处理的图像的高度，如果对图像区域进行处理，该宽度与原始图像的宽度是不同的
	LONG RectLeft;//图像选择区域的左边界
	LONG RectBottom;//图像选择区域的下边界
	int Bit;//图像位数
	int OriWidth;//图像原始宽度
	int OriHeight;//图像原始高度
	BOOL BufHandle;//图像可以进行处理标识,当每次调用左相机回调函数是,该标记置true,当对图像进行处理时,该标记置false
};

//双目相机参数
class Calibration
{
public:
	void Initial();
public:
	BOOL MeasureDraw;//双相机显示测量结果标记,true表示显示,false表示不显示(Not used yet)
	int ImageIndex;//采集图像编号(Not used yet)
	int RowCornerNum;//标定图像内角点（不和边缘接触的角点）行数
	int ColumCornerNum;//标定图像内角点（不和边缘接触的角点）列数
	int GridWidth;//实际测量得到的标定板上每个棋盘格的长
	int GridHeight;//实际测量得到的标定板上每个棋盘格的宽
};

//单相机标定参数
class SingleCalibration
{
public:
	BOOL CalibDraw;//相机显示标定位置标记,true表示显示,false表示不显示
	CPtrArray ImgCoordSeq;//记录各个图像的标靶坐标
};

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
	void ShowLeftCircles();
	void ShowRightCircles();
	void DrawLeftCamera();
	void DrawRightCamera();
	BOOL CalibFinishLeftCamera();
	BOOL CalibFinishRightCamera();
	void StopLeftCamera();

public:
	//todo：business logic seperate from prentation logic
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
	bool m_TimerFlag;//定时器标志
	bool m_TimerFlag2;//定时器2标志
	CWinThread* m_IAAThread;
	CEvent m_EVKillIAA;//停止连续调整线程
	CEvent m_EVDeadIAA;//连续调整线程结束
	bool m_ThreadFlag;//标定线程运行标记,TRUE表示运行,FALSE表示停止
};
