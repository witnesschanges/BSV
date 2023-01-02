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
	HANDLE CameraHandle;//������
	CameraType type;
	CCriticalSection Section;//�����ͼ�ٽ���
	CCriticalSection SectionCopy;//�����ͼ�ٽ���
	int ImageIndex;//����ɼ�ͼƬ��ű�־
	//CPtrArray BlobSeq;
	CArray<Blob> BlobSeq;//��¼���Բ��������������
	MV_PixelFormatEnums PixelFormat;//���ͼ���ʽ
};