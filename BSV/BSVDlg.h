
// BSVDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "MVGigE.h"
#include "MVImage.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "FeatureDetect.h"
#include "mscomm.h"
#include "Stereo.h"
#include "StereoMatch.h"
#include "PointCloudAnalyzer.h"
#include "ChildDlg1.h"
#include "ChildDlg2.h"
#include "ChildDlg3.h"
#include "afxcmn.h"
#include "ChildDialog1.h"

using namespace std;
using namespace cv;

#define WaitOutTime 3000
#define pi 3.141592653

// CBSVDlg 对话框
class CBSVDlg : public CDialogEx
{
// 构造
public:
	CBSVDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CBSVDlg();	//析构函数

// 对话框数据
	enum { IDD = IDD_BSV_DIALOG };

// 声明嵌入式子窗口
public:
	//CChildDlg1* m_childdlg1;
	//CChildDlg2* m_childdlg2;
	//CChildDlg3* m_childdlg3;
	
	CChildDlg1 m_childdlg1;
	CChildDlg2 m_childdlg2;
	CChildDlg3 m_childdlg3;
	CChildDialog1 m_Childdlg1;
	//窗口大小变量
	CRect m_rect;

public:
	/*左相机变量*/
	HANDLE m_pLCamera;			//左相机句柄
	BITMAPINFO  *m_pLBmpInfo;	//左图像信息头指针
	MV_PixelFormatEnums m_LPixelFormat;	//左相机图像格式
	LPSTR  m_LeftOriDIBBits;    //原始左图像，从图像文件读入或相机中直接获取。灰度图像，如果是RGB直接转化为8位灰度图
	LPSTR  m_LeftDIBBits;		//要进行处理的左图像，有可能是整幅图像也有可能是原始图像的一个区域
	LPSTR  m_LeftShowDIBBits;   //要进行显示的左图像
	LONG   m_LeftWidth;         //要进行处理的左图像的宽度，如果对图像区域进行处理，该宽度与原始图像的宽度是不同的
	LONG   m_LeftHeight;        //要进行处理的左图像的高度，如果对图像区域进行处理，该宽度与原始图像的宽度是不同的
	LONG   m_LeftRectLeft;      //左图像选择区域的左边界
	LONG   m_LeftRectBottom;    //左图像选择区域的下边界
	int    m_nLBit;				//左图像位数
	int    m_LeftOriWidth;      //左图像原始宽度
	int    m_LeftOriHeight;     //左图像原始高度
	BOOL   m_bLBufHandle;	    //左图像可以进行处理标识,当每次调用左相机回调函数是,该标记置true,
							    //当对左图像进行处理时,该标记置false
	CCriticalSection  m_CSectionL;	    //左相机采图临界区
	CCriticalSection  m_CSectionCopyL;	//左相机采图临界区
	int m_nLImageIndex;			//左相机采集图片序号标志
	string m_LImgPath;			//左相机图片存储位置
	CArray<Blob> m_LeftBlobSeq;	//记录左相机圆形特征中心坐标
	Blob LBlob;					//左相机团块数据（圆形识别时使用）
	/*标定相关变量*/
	BOOL   m_bLCalibDraw;		//左相机显示标定位置标记,true表示显示,false表示不显示
	CPtrArray m_LeftImgCoordSeq;//记录各个图像的标靶坐标
	
	/*右相机变量*/
	HANDLE m_pRCamera;			//右相机句柄
	BITMAPINFO  *m_pRBmpInfo;	//右图像信息头指针
	MV_PixelFormatEnums m_RPixelFormat;	//右相机图像格式
	LPSTR  m_RightOriDIBBits;   //原始右图像，从图像文件读入或相机中直接获取。灰度图像，如果是RGB直接转化为8位灰度图
	LPSTR  m_RightDIBBits;		//要进行处理的右图像，有可能是整幅图像也有可能是原始图像的一个区域
	LPSTR  m_RightShowDIBBits;	//要进行显示的右图像
	LONG   m_RightWidth;		//要进行处理的右图像的宽度，如果对图像区域进行处理，该宽度与原始图像的宽度是不同的
	LONG   m_RightHeight;		//要进行处理的右图像的高度，如果对图像区域进行处理，该宽度与原始图像的宽度是不同的
	LONG   m_RightRectLeft;		//右图像选择区域的左边界
	LONG   m_RightRectBottom;	//右图像选择区域的下边界
	int    m_nRBit;				//右图像位数
	int    m_RightOriWidth;		//右图像原始宽度
	int    m_RightOriHeight;	//右图像原始高度
	BOOL   m_bRBufHandle;	    //右图像可以进行处理标识,当每次调用右相机回调函数是,该标记置true,
							    //当对右图像进行处理时,该标记置false
	CCriticalSection  m_CSectionR;	    //右相机采图临界区
	CCriticalSection  m_CSectionCopyR;	//右相机采图临界区
	int m_nRImageIndex;			//右相机采集图片序号标志
	string m_RImgPath;			//右相机图片存储位置
	CArray<Blob> m_RightBlobSeq;//记录右相机圆形特征中心坐标
	/*标定相关变量*/
	BOOL   m_bRCalibDraw;		//右相机显示标定位置标记,true表示显示,false表示不显示
	CPtrArray m_RightImgCoordSeq;//记录各个图像的标靶坐标

	/*系统参数*/
	bool    m_TimerFlag;	    //定时器标志
	bool    m_TimerFlag2;       //定时器2标志
	CWinThread *m_IAAThread;
	CEvent m_EVKillIAA;			//停止连续调整线程
	CEvent m_EVDeadIAA;			//连续调整线程结束
	bool   m_ThreadFlag;		//标定线程运行标记,TRUE表示运行,FALSE表示停止

	//相机标定参数
	BOOL m_bDMeasureDraw;		//双相机显示测量结果标记,true表示显示,false表示不显示
	int m_n2ImageIndex;			//采集图像编号
	int row_corner_num;			//标定图像内角点（不和边缘接触的角点）行数
	int column_corner_num;		//标定图像内角点（不和边缘接触的角点）列数
	int grid_width;				//实际测量得到的标定板上每个棋盘格的长
	int grid_height;			//实际测量得到的标定板上每个棋盘格的宽
	int PicNum;					//需要标定图片的数量

	//双目匹配参数
	StereoMatch m_stereoMatcher;

	//人脸识别参数
	string Cascade_mode;
	CascadeClassifier Mycascade;
	string window_name;

	protected:	
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public://lq:以afx_msg打头的是消息处理函数
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
	void StopRightCamera();
	afx_msg void OnBnClickedCalibration();
	afx_msg void OnBnClickedSavepic();
	afx_msg void OnBnClickedSavepic2();
	afx_msg void OnBnClickedCalibration2();
	afx_msg void OnBnClickedCornerdetect();
	afx_msg void OnBnClickedCornerdetect2();
	afx_msg void OnBnClickedStereocalibration();
	afx_msg void OnBnClickedStereomatch();
	double m_ObjectDistance;
	double m_ObjectHeight;
	double m_ObjectWidth;
	afx_msg void OnBnClickedStereomeasure();
	afx_msg void OnBnClickedStopcamera();
	afx_msg void OnBnClickedStopcamera2();
	afx_msg void OnBnClickedStop2camera();
	afx_msg void OnBnClickedSave2pic();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedTabbutton1();
	afx_msg void OnBnClickedTabbutton2();
	afx_msg void OnBnClickedTabbutton3();
	void ShowLeftPic(void);
	Mat FaceDetect(Mat frame);
	afx_msg void OnBnClickedFacedetect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(CWnd *pWnd, int cx, int cy);
//	afx_msg void OnBnClickedButton20();
//	afx_msg void OnBnClickedCircleStereocalibration();
};
