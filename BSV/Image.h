#pragma once
#include <afxwin.h>

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