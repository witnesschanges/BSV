#pragma once
#include <afxwin.h>
#include <afxmt.h>
#include "MVGigE.h"
#include "FeatureDetect.h"

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
	CameraType type;
	CCriticalSection Section;//相机采图临界区
	CCriticalSection SectionCopy;//相机采图临界区
	int ImageIndex;//相机采集图片序号标志
	//CPtrArray BlobSeq;
	CArray<Blob> BlobSeq;//记录相机圆形特征中心坐标
	MV_PixelFormatEnums PixelFormat;//相机图像格式
};