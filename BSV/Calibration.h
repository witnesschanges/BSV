#pragma once
#include <afxwin.h>

//双目相机参数
class Calibration
{
public:
	void Initial();
public:
	BOOL MeasureDraw;//双相机显示测量结果标记,true表示显示,false表示不显示(Not used yet)
	int ImageIndex;//采集图像编号(Not used yet)
	int RowCornerNum;//标定图像内角点（不和边缘接触的角点）行数
	int ColumCornerNum;//标定图像内角点（不和边缘接触的角点）列数
	int GridWidth;//实际测量得到的标定板上每个棋盘格的长
	int GridHeight;//实际测量得到的标定板上每个棋盘格的宽
};

//单相机标定参数
class SingleCalibration
{
public:
	BOOL CalibDraw;//相机显示标定位置标记,true表示显示,false表示不显示
	CPtrArray ImgCoordSeq;//记录各个图像的标靶坐标
};