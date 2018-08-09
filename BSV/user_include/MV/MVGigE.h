#ifndef MVGIGE_H_INCLUDE_
#define MVGIGE_H_INCLUDE_

#ifdef MVGIGE_EXPORTS
#define MVGIGE_API __declspec(dllexport)
#else
#define MVGIGE_API __declspec(dllimport)
#endif

#include "GigECamera_Types.h"
#include "MVimage.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \brief 初始化函数库。在调用函数所有函数之前调用。
  *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVInitLib();

/*!
 *  \brief 退出函数库。在程序退出前调用，以释放资源。
  *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVTerminateLib();

/*! 
 *  \brief	查找连接到计算机上的相机
 *  \retval 
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVUpdateCameraList();

/*! 
 *  \brief	获取连接到计算机上的相机的数量
 *  \param [out]	pNumCams	相机数量
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetNumOfCameras(int *pNumCams);

/*! 
 *  \brief	得到第idx个相机的信息。
 *  \param [in]	idx idx从0开始，按照相机的IP地址排序，地址小的排在前面。
 *  \param [out]	pCamInfo  相机的信息 (IP,MAC,SN,型号...) 
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetCameraInfo(unsigned char idx,MVCamInfo *pCamInfo);

/*! 
 *  \brief	打开第idx个相机
 *  \param [in]	idx	idx从0开始，按照相机的IP地址排序，地址小的排在前面。
 *  \param [out]	hCam 如果成功,返回的相机句柄
 *  \retval MVST_INVALID_PARAMETER : idx取值不对
 *			MVST_ACCESS_DENIED		: 相机无法访问，可能正被别的软件控制
 *			MVST_ERROR				: 其他错误
 *			MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVOpenCamByIndex(unsigned char idx,HANDLE *hCam);

/*! 
 *  \brief	打开指定UserDefinedName的相机
 *  \param [in]	name UserDefinedName。
 *  \param [out] hCam 如果成功,返回的相机句柄。如果失败，为NULL。
 *  \retval 
 *			MVST_ACCESS_DENIED		: 相机无法访问，可能正被别的软件控制
 *			MVST_ERROR				: 其他错误
 *			MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVOpenCamByUserDefinedName( char *name,HANDLE *hCam );

/*! 
 *  \brief	关闭相机。断开和相机的连接。
 *  \param [in]	hCam 相机的句柄
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVCloseCam(HANDLE hCam);

/*!
 * \brief	读取图像宽度
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	int * pWidth	图像宽度[像素]
 * \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetWidth(HANDLE hCam,int *pWidth);

/*!
 *  \brief 读取图像宽度可设置的范围
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [out]	int * pWidthMin	图像宽度可设置的最小值
 *  \param [out]	int * pWidthMax	图像宽度可设置的最大值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetWidthRange(HANDLE hCam,int *pWidthMin,int *pWidthMax);

/*!
 *  \brief 读取图像宽度调整的步长
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [out] int * pWidthInc	图像宽度的调整的步长，即图像的宽度 = 最小宽度 + 步长 x 整数
  *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetWidthInc(HANDLE hCam,int *pWidthInc);

/*!
 *  \brief 设置图像的宽度
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	int nWidth	图像宽度，应该在宽度可设置范围之内，并且 = 最小宽度 + 步长 x 整数
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetWidth(HANDLE hCam,int nWidth);

/*!
 * \brief	读取图像高度
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	int * pHeight	图像高度[像素]
 * \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetHeight(HANDLE hCam,int *pHeight);

/*!
 *  \brief 读取图像高度可设置的范围
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [out]	int * pHeightMin	图像高度可设置的最小值
 *  \param [out]	int * pHeightMax	图像高度可设置的最大值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetHeightRange(HANDLE hCam,int *pHeightMin,int *pHeightMax);

/*!
 *  \brief 设置图像的高度
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	int nHeight	图像高度，应该在高度可设置范围之内
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetHeight(HANDLE hCam,int nHeight);

/*!
 *  \brief 读取水平方向偏移量。图像宽度设置到小于最大宽度时，可以调整水平偏移量，设置采集窗口的水平起始位置。
 *  \param [in]	HANDLE hCam
 *  \param [out]	int * pOffsetX	水平偏移量
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetOffsetX(HANDLE hCam,int *pOffsetX);

/*!
 *  \brief 读取水平方向偏移量取值范围。
 *  \param [in]	HANDLE hCam
 *  \param [out]	int * pOffsetXMin	水平偏移量最小值
 *  \param [out]	int * pOffsetXMax	水平偏移量最大值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetOffsetXRange(HANDLE hCam,int *pOffsetXMin,int *pOffsetXMax);

/*!
 *  \brief 设置水平方向偏移量。图像宽度设置到小于最大宽度时，可以调整水平偏移量，设置采集窗口的水平起始位置。
 *  \param [in]	HANDLE hCam
 *  \param [in]	int nOffsetX 水平偏移量。应该在水平偏移量允许的范围之内。
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetOffsetX(HANDLE hCam,int nOffsetX);

/*!
 *  \brief 读取垂直方向偏移量。图像宽度设置到小于最大宽度时，可以调整垂直偏移量，设置采集窗口的垂直起始位置。
 *  \param [in]	HANDLE hCam
 *  \param [out]	int * pOffsetY	垂直偏移量
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetOffsetY(HANDLE hCam,int *pOffsetY);

/*!
 *  \brief 读取垂直方向偏移量取值范围。
 *  \param [in]	HANDLE hCam
 *  \param [out]	int * pOffsetYMin	垂直偏移量最小值
 *  \param [out]	int * pOffsetYMax	垂直偏移量最大值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetOffsetYRange(HANDLE hCam,int *pOffsetYMin,int *pOffsetYMax);

/*!
 *  \brief 设置垂直方向偏移量。图像宽度设置到小于最大宽度时，可以调整垂直偏移量，设置采集窗口的垂直起始位置。
 *  \param [in]	HANDLE hCam
 *  \param [in]	int nOffsetY 垂直偏移量。应该在垂直偏移量允许的范围之内。
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetOffsetY(HANDLE hCam,int nOffsetY);	

/*!
 * \brief    读取图像的像素格式
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	MV_PixelFormatEnums * pPixelFormat
 * \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetPixelFormat(HANDLE hCam, MV_PixelFormatEnums *pPixelFormat);

/*!
 *  \brief	读取传感器的通道数
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [out] SensorTapsEnums* pSensorTaps
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetSensorTaps(HANDLE hCam, SensorTapsEnums *pSensorTaps );

/*!
 * \brief    读取当前增益值
 * \param [in]	HANDLE hCam	相机句柄
 * \param [out]	double * pGain
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetGain(HANDLE hCam,double *pGain);

/*!
 * \brief    读取增益可以设置的范围
 * \param [in]	HANDLE hCam	相机句柄
 * \param [out]	double * pGainMin	最小值
 * \param [out]	double * pGainMax	最大值
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainRange(HANDLE hCam,double *pGainMin,double *pGainMax);	

/*!
 * \brief    设置增益
 * \param [in]	HANDLE hCam	相机句柄
 * \param [in]	double fGain	增益
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetGain(HANDLE hCam,double fGain);

/*!
 * \brief    当相机传感器为多通道时，设置某个通道的增益
 * \param [in]	HANDLE hCam		相机句柄
 * \param [in]	double fGain	增益
 * \param [in]	int nTap		通道。双通道[0,1],四通道[0,1,2,3]
 *  \retval MVC_ST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetGainTaps(HANDLE hCam, double fGain, int nTap );

/*!
 * \brief    当相机传感器为多通道时，读取某个通道的增益
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	double* pGain
 * \param [in]	int nTap		通道。双通道[0,1],四通道[0,1,2,3]
 *  \retval MVC_ST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainTaps(HANDLE hCam, double *pGain,int nTap );

/*!
 * \brief    当相机传感器为多通道时，读取某个通道的增益可设置的范围
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	double * pGainMin	增益最小值
 * \param [out]	double * pGainMax	增益最大值
 * \param [in]	int nTap		通道。双通道[0,1],四通道[0,1,2,3]
 *  \retval MVC_ST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainRangeTaps(HANDLE hCam, double *pGainMin,double *pGainMax, int nTap );

/*!
 * \brief    读取当前白平衡系数
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	double * pRed	红色平衡系数
 * \param [out]	double * pGreen 绿色平衡系数
 * \param [out]	double * pBlue	蓝色平衡系数
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetWhiteBalance( HANDLE hCam,double *pRed,double *pGreen, double *pBlue );

/*!
 * \brief	读取白平衡设置的范围
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	double * pMin	系数最小值
 * \param [out]	double * pMax	系数最大值
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetWhiteBalanceRange(HANDLE hCam, double *pMin,double *pMax );

/*!
 * \brief    设置白平衡系数
 * \param [in]	HANDLE hCam		相机句柄
 * \param [in]	double fRed		红色平衡系数
 * \param [in]	double fGreen	绿色平衡系数
 * \param [in]	double fBlue	蓝色平衡系数
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetWhiteBalance(HANDLE hCam,double fRed,double fGreen, double fBlue );

/*!
 * \brief    读取是否通道自动平衡
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	int* pBalance	是否自动平衡
 *  \retval MVST_SUCCESS			: 成功
 */	
MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainBalance(HANDLE hCam, int *pBalance );

/*!
 * \brief    设置是否自动通道平衡
 * \param [in]	HANDLE hCam		相机句柄
 * \param [in]	int nBalance	是否自动通道平衡
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetGainBalance(HANDLE hCam,int nBalance );

/*! 
 *  \brief	读取当前曝光时间
 *  \param [in]	hCam
 *  \param [in]	pExposuretime	单位us
 *  \retval 
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetExposureTime(HANDLE hCam,double *pExposuretime);

/*!
 * \brief  读取曝光时间的设置范围  
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	double * pExpMin	最短曝光时间 单位为us
 * \param [out]	double * pExpMax	最长曝光时间 单位为us
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetExposureTimeRange(HANDLE hCam,double *pExpMin,double *pExpMax);

/*!
 * \brief	设置曝光时间
 * \param [in]	HANDLE hCam		相机句柄
 * \param [in]	unsigned long nExp_us 曝光时间 单位为us
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetExposureTime(HANDLE hCam,unsigned long nExp_us);

/*!
 * \brief	读取帧率可设置的范围
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	double* pFpsMin 最低帧率
 * \param [out]	double* pFpsMax 最高帧率	
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetFrameRateRange(HANDLE hCam,double *pFpsMin,double *pFpsMax);

/*!
 * \brief	读取当前帧率   
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	double * fFPS	帧率 帧/秒
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetFrameRate(HANDLE hCam,double *fFPS);

/*!
 * \brief    设置帧率
 * \param [in]	HANDLE hCam		相机句柄
 * \param [in]	double fps	帧率 帧/秒
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetFrameRate(HANDLE hCam,double fps);

/*!
 *  \brief 开始采集图像
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in] StreamCB	回调函数指针
 *  \param [in]	long nUserVal	用户数据，传递到回调函数的形参
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVStartGrab(HANDLE hCam,int (*StreamCB)(MV_IMAGE_INFO*,long nUserVal),long nUserVal);


/*!
 *  \brief 停止采集图像
 *  \param [in]	HANDLE hCam	相机句柄
 *  \retval  MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVStopGrab(HANDLE hCam);

/*!
 * \brief    读取触发模式
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	TriggerModeEnums * pMode	触发模式  TriggerMode_Off,TriggerMode_On
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerMode(HANDLE hCam,TriggerModeEnums *pMode);

/*! 
 *  \brief	设置触发模式
 * \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	mode	触发模式
	TriggerMode_Off：相机工作在连续采集模式，
	TriggerMode_On:相机工作在触发模式，需要有外触发信号或软触发指令才拍摄
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetTriggerMode(HANDLE hCam,TriggerModeEnums mode);

/*!
 * \brief    读取触发源
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	TriggerSourceEnums * pSource	触发源，软触发或外触发
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerSource(HANDLE hCam,TriggerSourceEnums *pSource);

/*! 
 *  \brief	设置触发源
 * \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	TriggerSourceEnums	source 触发源
				TriggerSource_Software：通过\c MVTriggerSoftware()函数触发。
				TriggerSource_Line1：通过连接的触发线触发。
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetTriggerSource(HANDLE hCam,TriggerSourceEnums source);

/*!
 * \brief    读取触发极性
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	TriggerActivationEnums * pAct
				TriggerActivation_RisingEdge: 上升沿触发
				TriggerActivation_FallingEdge: 下降沿触发
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerActivation(HANDLE hCam,TriggerActivationEnums *pAct);

/*! 
 *  \brief	当使用触发线触发时,设置是上升沿触发还是下降沿触发
 *  \param [in]	hCam
 *  \param [in]	act 上升沿或下降沿
				TriggerActivation_RisingEdge: 上升沿触发
				TriggerActivation_FallingEdge: 下降沿触发
 *  \retval 
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetTriggerActivation(HANDLE hCam,TriggerActivationEnums act);

/*!
 * \brief	读取触发延时
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	uint32_t * pDelay_us	触发延时,单位us
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerDelay(HANDLE hCam,uint32_t *pDelay_us);

/*!
 * \brief    读取触发延时范围
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	uint32_t * pMin	触发延时最小值,单位us	
 * \param [out]	uint32_t * pMax 触发延时最大值,单位us	
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerDelayRange(HANDLE hCam,uint32_t *pMin,uint32_t *pMax);

/*! 
 *  \brief	设置相机接到触发信号后延迟多少微秒后再开始曝光。
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	nDelay_us
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetTriggerDelay(HANDLE hCam,uint32_t nDelay_us);

/*! 
 *  \brief	发出软件触发指令
 *  \param [in]	HANDLE hCam		相机句柄
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVTriggerSoftware(HANDLE hCam);

/*!
 * \brief	读取闪光同步信号源
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	LineSourceEnums * pSource	闪光同步信号源
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetStrobeSource(HANDLE hCam,LineSourceEnums *pSource);

/*! 
 *  \brief	闪光同步信号源
 *  \param [in]	hCam
 *  \param [in]	source
				LineSource_Off：关闭闪光同步
				LineSource_ExposureActive：曝光的同时闪光
				LineSource_Timer1Active：由定时器控制
				LineSource_UserOutput0：由用户通过指令控制
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetStrobeSource(HANDLE hCam,LineSourceEnums source);

/*!
 * \brief	读取闪光同步是否反转
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	BOOL * pInvert
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetStrobeInvert(HANDLE hCam,BOOL *pInvert);

/*! 
 *  \brief	闪光同步是否反转，即闪光同步有效时输出高电平还是低电平。
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	bInvert
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetStrobeInvert(HANDLE hCam,BOOL bInvert);

/*!
 * \brief	读取用户设置的闪光同步
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	BOOL * pSet
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetUserOutputValue0(HANDLE hCam,BOOL *pSet);

/*! 
 *  \brief	当闪光同步源选为UserOutput时
			主机可以通过MVSetUserOutputValue0来控制闪光同步输出高电平或低电平。
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	bSet 设置电平
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetUserOutputValue0(HANDLE hCam,BOOL bSet);

/*!
 *  \brief 设置心跳超时时间
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	unsigned long nTimeOut	心跳超时时间 单位ms
 *  \retval  	MVST_SUCCESS			: 成功
 *  \note 应用程序打开相机后，正常情况下会去关闭相机。但有时程序会意外中断，没有正常关闭相机。
 *			这种情况下相机就无法再次打开了（相机认为还在被原来软件控制着）。 
 *			因此设计了heartbeat，应用程序无论是否在采集图像，每隔一定时间(一般是1000ms)都要去访问一下相机。
 *			当超过HeartbeatTimeout时间没有来自应用程序的访问，相机就认为应用程序中断了，会自动关闭原有连接，
 *			以迎接新的连接。
 *			定时访问相机的操作在SDK内部已经实现。编程时无需再次实现。
 *			在设置断点调试程序时，可以将nTimeOut设置长一些，否则相机会自动关闭连接。
 *			Release状态下设置短一些(3000ms),否则应用程序意外退出时，相机会长时间无法重新打开。
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetHeartbeatTimeout(HANDLE hCam,unsigned long nTimeOut);//unit ms

/*!
 * \brief	读取网络数据包大小
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	int *pPacketSize 数据包大小
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetPacketSize(HANDLE hCam,unsigned int *pPacketSize);

/*!
 * \brief    读取网络数据包大小的范围。
 * \param [in]	HANDLE hCam			相机句柄
 * \param [out]	unsigned int * pMin	网络数据包最小值	
 * \param [out]	unsigned int * pMax 网络数据包最大值	
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetPacketSizeRange(HANDLE hCam,unsigned int *pMin,unsigned int *pMax);

/*! 
 *  \brief	设置网络数据包的大小。
 *  \param [in]	HANDLE hCam			相机句柄
 *  \param [in]	nPacketSize 网络数据包大小(单位:字节)。该大小必须小于网卡能够支持的最大巨型帧(Jumbo Frame)。
 *  \retval 
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetPacketSize(HANDLE hCam,unsigned int nPacketSize);

/*!
 * \brief	读取网络数据包间隔。
 * \param [in]	HANDLE hCam				相机句柄
 * \param [out]	unsigned int *pDelay_us 数据包间隔时间，单位us
 *  \retval MVST_SUCCESS				: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetPacketDelay(HANDLE hCam,unsigned int *pDelay_us);

/*!
 * \brief    读取网络数据包间隔范围
 * \param [in]	HANDLE hCam			相机句柄
 * \param [out]	unsigned int * pMin	数据包间隔时间最小值，单位us	
 * \param [out]	unsigned int * pMax 数据包间隔时间最大值，单位us	
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetPacketDelayRange(HANDLE hCam,unsigned int *pMin,unsigned int *pMax);


/*! 
 *  \brief	设置网络数据包之间的时间间隔。如果网卡或电脑的性能欠佳，无法处理高速到达的数据包，会导致丢失数据包，
			从而使图像不完整。可以通过增加数据包之间的时间间隔以保证图像传输。但是增加该值将增加图像的时间延迟，
			并有可能影像到帧率。
 *  \param [in]	hCam
 *  \param [in]	nDelay_us 时间间隔(单位:微秒)
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetPacketDelay(HANDLE hCam,unsigned int nDelay_us);

/*!
 * \brief    读取定时器延时
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	uint32_t * pDelay	定时器延时
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetTimerDelay(HANDLE hCam,uint32_t *pDelay);

/*!
 * \brief	读取定时器延时的范围
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	uint32_t * pMin	定时器延时的最小值
 * \param [out]	uint32_t * pMax 定时器延时的最大值
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetTimerDelayRange(HANDLE hCam,uint32_t *pMin,uint32_t *pMax);

/*! 
 *  \brief	当闪光同步源选为Timer1时MVSetStobeSource(hCam,LineSource_Timer1Active)
			设置Timer1在接到触发信号后延迟多少us开始计时
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	uint32_t nDelay 接到触发信号后延迟多少us开始计时
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetTimerDelay(HANDLE hCam,uint32_t nDelay);

/*!
 * \brief    读取定时器计时时长
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	uint32_t * pDuration	定时器计时时长
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetTimerDuration(HANDLE hCam,uint32_t *pDuration);

/*!
 * \brief    读取定时器计时时长取值范围
 * \param [in]	HANDLE hCam		相机句柄
 * \param [out]	uint32_t * pMin	定时器计时时长最小值
 * \param [out]	uint32_t * pMax	定时器计时时长最大值
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetTimerDurationRange(HANDLE hCam,uint32_t *pMin,uint32_t *pMax);

/*! 
*  \brief	当闪光同步源选为Timer1时MVSetStobeSource(hCam,LineSource_Timer1Active)
			设置Timer1在开始计时后，计时多长时间。
 *  \param [in]	HANDLE hCam
 *  \param [in]	uint32_t nDuration 设置Timer1在开始计时后，计时多长时间(us)。即输出高/低电平的脉冲宽度。
 *  \retval MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetTimerDuration(HANDLE hCam,uint32_t nDuration);


/*!
 *  \brief 将Bayer格式的8bit单通道图转换为BGR格式的8Bit三通道图
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	void * psrc	单通道图像的指针
 *  \param [out]	void * pdst	三通道图像指针
 *  \param [in]	unsigned int dststep	三通道图像一行图像的字节数。通常为图像宽度*3，但是会为了4字节对齐会补几个字节。
 *  \param [in]	unsigned int width	图像宽度
 *  \param [in]	unsigned int height	图像高度
 *  \param [in]	MV_PixelFormatEnums pixelformat	像素格式，由MVGetPixelFormat取得
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVBayerToBGR(HANDLE hCam, void *psrc,void *pdst,unsigned int dststep,unsigned int width,unsigned int height,MV_PixelFormatEnums pixelformat);

/*!
 *  \brief 将Bayer格式的8bit单通道图转换为BGR格式的8Bit三通道图
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	MV_IMAGE_INFO * pInfo	采集Callback函数中传来的图像信息指针
 *  \param [out]	MVImage * pImage	转换结果图像的指针
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVImageBayerToBGR(HANDLE hCam,MV_IMAGE_INFO *pInfo,MVImage *pImage);


/*!
 *  \brief 将Bayer格式的8bit单通道图转换为BGR格式的8Bit三通道图,同时调整图像的GAMMA,颜色和反差
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	MV_IMAGE_INFO * pInfo	采集Callback函数中传来的图像信息指针
 *  \param [out]	MVImage * pImage	转换结果图像的指针
 *  \param [in]	double fGamma			Gamma校正值，1为不校正，<1时，将暗部提升。
 *  \param [in]	BOOL bColorCorrect		是否进行颜色校正，进行颜色校正后，图像会变得更鲜艳。
 *  \param [in]	int nContrast			是否调整反差，范围为0－50，当该值大于0时，图像反差会更强。
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVImageBayerToBGREx( HANDLE hCam,MV_IMAGE_INFO *pInfo,MVImage *pImage,double fGamma,BOOL bColorCorrect,int nContrast );

/*!
 *  \brief BGR格式三通道图像缩放
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	unsigned char * pSrc 源图像指针
 *  \param [in]	int srcWidth	源图像宽度
 *  \param [in]	int srcHeight	源图像高度
 *  \param [out]	unsigned char * pDst	缩放后图像指针
 *  \param [in]	double fFactorX	缩放比例
 *  \param [in]	double fFactorY	缩放比例
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVZoomImageBGR(HANDLE hCam,unsigned char *pSrc,int srcWidth,int srcHeight, unsigned char *pDst,double fFactorX,double fFactorY);

/*!
 *  \brief	获取数据传输的统计信息
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	MVStreamStatistic * pStatistic	统计信息
 *  \retval  	JMVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetStreamStatistic(HANDLE hCam, MVStreamStatistic *pStatistic );



/*!
 *  \brief	读取并应用某组用户预设的参数
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	UserSetSelectorEnums userset
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVLoadUserSet(HANDLE hCam,UserSetSelectorEnums userset);

/*!
 *  \brief	将当前相机的参数保存到用户设置中。
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	UserSetSelectorEnums userset
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSaveUserSet(HANDLE hCam,UserSetSelectorEnums userset);

/*!
 *  \brief	设置相机上电开机时默认读取并应用哪一组用户设置
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [in]	UserSetSelectorEnums userset
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetDefaultUserSet(HANDLE hCam,UserSetSelectorEnums userset);

/*!
 *  \brief	读取相机上电开机时默认读取并应用哪一组用户设置
 *  \param [in]	HANDLE hCam		相机句柄
 *  \param [out]	UserSetSelectorEnums* pUserset	用户设置
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetDefaultUserSet(HANDLE hCam, UserSetSelectorEnums *pUserset);

/*!
 *  \brief 图像翻转
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	MVImage * pSrcImage	源图像指针
 *  \param [out] MVImage * pDstImage	结果图像指针。如果为NULL，则翻转的结果还在源图像内。
 *  \param [in]	ImageFlipType flipType	翻转类型。FlipHorizontal:左右翻转,FlipVertical:上下翻转,FlipBoth:旋转180度
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVImageFlip(HANDLE hCam,MVImage *pSrcImage,MVImage *pDstImage,ImageFlipType flipType);


/*!
 *  \brief 图像旋转
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	MVImage * pSrcImage	源图像指针
 *  \param [out] MVImage * pDstImage	结果图像指针,不能为NULL。结果图像的宽度和高度应该和源图像的宽度和高度互换。
 *  \param [in]	ImageRotateType roateType	旋转类型：Rotate90DegCw, Rotate90DegCcw
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVImageRotate(HANDLE hCam,MVImage *pSrcImage,MVImage *pDstImage, ImageRotateType roateType);

/*!
 *  \brief 将彩色BGR三通道24bit图像转换为灰度单通道8bit图像
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	unsigned char * psrc	彩色BGR三通道24bit图像指针
 *  \param [out]	unsigned char * pdst	灰度单通道8bit图像指针
 *  \param [in]	unsigned int width	图像宽度
 *  \param [in]	unsigned int height	图像高度
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVBGRToGray(HANDLE hCam,unsigned char *psrc,unsigned char *pdst,unsigned int width,unsigned int height);

/*!
 *  \brief 将彩色BGR三通道24bit图像转换为灰度单通道8bit图像
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	MVImage * pSrcImage	彩色BGR三通道24bit图像指针
 *  \param [out]	MVImage * pDstImage	灰度单通道8bit图像指针。宽度高度必须和源图相同
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVImageBGRToGray(HANDLE hCam,MVImage *pSrcImage,MVImage *pDstImage);


/*!
 *  \brief	将彩色BGR三通道24bit图像转换为YUV图像 
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	MVImage * pSrcImage	彩色BGR三通道24bit图像指针
 *  \param [out]	unsigned char * pDst	YUV图像指针 (YUV422)
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVImageBGRToYUV(HANDLE hCam,MVImage *pSrcImage,unsigned char *pDst);

/*!
 *  \brief 将灰度单通道8bit图像转换为彩色BGR三通道24bit图像。转换后三个通道的值是相同的。
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	unsigned char * psrc	灰度单通道8bit图像指针
 *  \param [out]	unsigned char * pdst	彩色BGR三通道24bit图像指针
 *  \param [in]	unsigned int width	图像宽度
 *  \param [in]	unsigned int height	图像高度
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGrayToBGR(HANDLE hCam,unsigned char *pSrc,unsigned char *pDst,int width,int height);

/*!
 *  \brief 获取当前自动曝光模式
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	ExposureAutoEnums * pExposureAuto	当前自动曝光模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetExposureAuto( HANDLE hCam,ExposureAutoEnums *pExposureAuto);
/*!
 *  \brief 设置自动曝光模式
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	ExposureAutoEnums ExposureAuto	自动曝光模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetExposureAuto( HANDLE hCam,ExposureAutoEnums ExposureAuto);

/*!	
 *  \brief 获取当前自动增益模式
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	GainAutoEnums * pGainAuto	当前自动增益模式的
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainAuto( HANDLE hCam,GainAutoEnums *pGainAuto);
/*!
 *  \brief 设置当前自动增益模式
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	GainAutoEnums GainAuto	自动增益模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetGainAuto( HANDLE hCam,GainAutoEnums GainAuto);

/*!
 *  \brief 获取当前自动白平衡模式
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	BalanceWhiteAutoEnums * pBalanceWhiteAuto	当前自动白平衡模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetBalanceWhiteAuto( HANDLE hCam,BalanceWhiteAutoEnums *pBalanceWhiteAuto);
/*!
 *  \brief 设置自动白平衡模式
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	BalanceWhiteAutoEnums BalanceWhiteAuto	自动白平衡模式
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetBalanceWhiteAuto( HANDLE hCam,BalanceWhiteAutoEnums BalanceWhiteAuto);

/*!
 *  \brief 获取自动调整增益时，增益调整范围的最小值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	double * pAutoGainLowerLimit	增益调整范围的最小值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoGainLowerLimit( HANDLE hCam,double *pAutoGainLowerLimit);
/*!
 *  \brief 设置自动调整增益时，增益调整范围的最小值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	double fAutoGainLowerLimit	增益调整范围的最小值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoGainLowerLimit( HANDLE hCam,double fAutoGainLowerLimit);

/*!
 *  \brief 获取自动调整增益时，增益调整范围的最大值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	double * pAutoGainUpperLimit	增益调整范围的最大值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoGainUpperLimit( HANDLE hCam,double *pAutoGainUpperLimit);
/*!
 *  \brief 设置自动调整增益时，增益调整范围的最大值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	double fAutoGainUpperLimit	曝光时间调整范围的最小值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoGainUpperLimit( HANDLE hCam,double fAutoGainUpperLimit);

/*!
 *  \brief 获取自动调整曝光时间时，曝光时间调整范围的最小值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	double * pAutoExposureTimeLowerLimit	曝光时间调整范围的最小值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoExposureTimeLowerLimit( HANDLE hCam,double *pAutoExposureTimeLowerLimit);
/*!
 *  \brief 设置自动调整曝光时间时，曝光时间调整范围的最小值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	double fAutoExposureTimeLowerLimit	曝光时间调整范围的最大值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoExposureTimeLowerLimit( HANDLE hCam,double fAutoExposureTimeLowerLimit);

/*!
 *  \brief 获取自动调整曝光时间时，曝光时间调整范围的最大值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	double * pAutoExposureTimeUpperLimit	曝光时间调整范围的最大值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoExposureTimeUpperLimit( HANDLE hCam,double *pAutoExposureTimeUpperLimit);
/*!
 *  \brief 设置自动调整曝光时间时，曝光时间调整范围的最大值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	double fAutoExposureTimeUpperLimit
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoExposureTimeUpperLimit( HANDLE hCam,double fAutoExposureTimeUpperLimit);

/*!
 *  \brief 获取自动调整亮度(曝光、增益)时，期望调整到的图像亮度
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	int * pAutoTargetValue	期望调整到的图像亮度
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoTargetValue( HANDLE hCam,int *pAutoTargetValue);
/*!
 *  \brief 设置自动调整亮度(曝光、增益)时，期望调整到的图像亮度
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	int nAutoTargetValue	期望调整到的图像亮度
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoTargetValue( HANDLE hCam,int nAutoTargetValue);

/*!
 *  \brief 当自动增益和自动曝光时间都打开时，获取哪一个值优先调整
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	AutoFunctionProfileEnums * pAutoFunctionProfile	增益优先或曝光时间优先
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoFunctionProfile( HANDLE hCam,AutoFunctionProfileEnums *pAutoFunctionProfile);
/*!
 *  \brief 当自动增益和自动曝光时间都打开时，设置哪一个值优先调整
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	AutoFunctionProfileEnums AutoFunctionProfile	增益优先或曝光时间优先
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoFunctionProfile( HANDLE hCam,AutoFunctionProfileEnums AutoFunctionProfile);

/*!
 *  \brief	自动增益或自动曝光时，图像亮度与目标亮度差异的容差。
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	int * pAutoThreshold	图像亮度与目标亮度差异的容差
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoThreshold( HANDLE hCam,int *pAutoThreshold);
/*!
 *  \brief 自动增益或自动曝光时，图像亮度与目标亮度差异的容差。
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	int nAutoThreshold	图像亮度与目标亮度差异的容差
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoThreshold( HANDLE hCam,int nAutoThreshold);

/*!
 *  \brief 获取当前伽马值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	double * pGamma	当前伽马值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetGamma( HANDLE hCam, double *pGamma );
/*!
 *  \brief 获取伽马值可设置的范围
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [out]	double * pGammaMin	伽马最小值
 *  \param [out]	double * pGammaMax	伽马最大值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVGetGammaRange( HANDLE hCam, double *pGammaMin, double *pGammaMax);
/*!
 *  \brief 设置伽马值
 *  \param [in]	HANDLE hCam	相机句柄
 *  \param [in]	double fGamma	伽马值
 *  \retval  	MVST_SUCCESS			: 成功
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVSetGamma( HANDLE hCam,double fGamma);

#ifdef __cplusplus
}
#endif

#endif //MVGIGE_H_INCLUDE_