#pragma once


typedef struct  Blob		//�ſ�ṹ�����ڼ�¼�����ſ�ļ�������ֵ
{
	double BlobWidth;      //����Ŀ��
	double BlobHeight;     //����ĸ߶�
	double BlobX;          //�������������X
	double BlobY;          //�������������Y
	double AspectRatio;    //�ݺ��
	double Circularity;    //Բ�ζ�
	long   Area;           //�����������������ص�����
	long   Perimeter;      //��������ܳ�

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
