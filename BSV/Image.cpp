#include "StdAfx.h"
#include "Image.h"

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
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];//设置图像信息头
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