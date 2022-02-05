#pragma once


typedef struct  Blob		//团块结构，用于记录各个团块的几何特征值
{
	double BlobWidth;      //区域的宽度
	double BlobHeight;     //区域的高度
	double BlobX;          //区域的质心坐标X
	double BlobY;          //区域的质心坐标Y
	double AspectRatio;    //纵横比
	double Circularity;    //圆形度
	long   Area;           //该区域的面积，即像素点数量
	long   Perimeter;      //该区域的周长

	Blob(void)
	{
		BlobWidth = 0;
		BlobHeight = 0;
		BlobX = 0;
		BlobY = 0;
		AspectRatio = 0;
		Circularity = 0;
		Area = 0;
		Perimeter = 0;
	}

	virtual ~Blob(void)
	{
		BlobWidth = NULL;
		BlobHeight = NULL;
		BlobX = NULL;
		BlobY = NULL;
		AspectRatio = NULL;
		Circularity = NULL;
		Area = NULL;
		Perimeter = NULL;
	}

	Blob(const Blob& pB)
		:BlobWidth(pB.BlobWidth)
		,BlobHeight(pB.BlobHeight)
		,BlobX(pB.BlobX)
		,BlobY(pB.BlobY)
		,AspectRatio(pB.AspectRatio)
		,Circularity(pB.Circularity)
		,Area(pB.Area)
		,Perimeter(pB.Perimeter)
	{
	}

	Blob& operator=(const Blob& pB)
	{
		if (this != &pB)
		{
			BlobWidth = pB.BlobWidth;
			BlobHeight = pB.BlobHeight;
			BlobX = pB.BlobX;
			BlobY = pB.BlobY;
			AspectRatio = pB.AspectRatio;
			Circularity = pB.Circularity;
			Area = pB.Area;
			Perimeter = pB.Perimeter;
		}
		return *this;
	}
};
