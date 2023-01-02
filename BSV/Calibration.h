#pragma once
#include <afxwin.h>

//˫Ŀ�������
class Calibration
{
public:
	void Initial();
public:
	BOOL MeasureDraw;//˫�����ʾ����������,true��ʾ��ʾ,false��ʾ����ʾ(Not used yet)
	int ImageIndex;//�ɼ�ͼ����(Not used yet)
	int RowCornerNum;//�궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
	int ColumCornerNum;//�궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
	int GridWidth;//ʵ�ʲ����õ��ı궨����ÿ�����̸�ĳ�
	int GridHeight;//ʵ�ʲ����õ��ı궨����ÿ�����̸�Ŀ�
};

//������궨����
class SingleCalibration
{
public:
	BOOL CalibDraw;//�����ʾ�궨λ�ñ��,true��ʾ��ʾ,false��ʾ����ʾ
	CPtrArray ImgCoordSeq;//��¼����ͼ��ı������
};