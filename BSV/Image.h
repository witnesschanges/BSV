#pragma once
#include <afxwin.h>

class Image
{
public:
	void Initial();
	void InitialBmpInfo();
public:
	BITMAPINFO* BmpInfo;//ͼ����Ϣͷָ��
	LPSTR OriDIBBits;//ԭʼͼ�񣬴�ͼ���ļ�����������ֱ�ӻ�ȡ���Ҷ�ͼ�������RGBֱ��ת��Ϊ8λ�Ҷ�ͼ
	LPSTR DIBBits;//Ҫ���д����ͼ���п���������ͼ��Ҳ�п�����ԭʼͼ���һ������
	LPSTR ShowDIBBits;//Ҫ������ʾ��ͼ��
	LONG Width;//Ҫ���д����ͼ��Ŀ�ȣ������ͼ��������д����ÿ����ԭʼͼ��Ŀ���ǲ�ͬ��
	LONG Height;//Ҫ���д����ͼ��ĸ߶ȣ������ͼ��������д����ÿ����ԭʼͼ��Ŀ���ǲ�ͬ��
	LONG RectLeft;//ͼ��ѡ���������߽�
	LONG RectBottom;//ͼ��ѡ��������±߽�
	int Bit;//ͼ��λ��
	int OriWidth;//ͼ��ԭʼ���
	int OriHeight;//ͼ��ԭʼ�߶�
	BOOL BufHandle;//ͼ����Խ��д����ʶ,��ÿ�ε���������ص�������,�ñ����true,����ͼ����д���ʱ,�ñ����false
};