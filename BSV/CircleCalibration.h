#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include <iostream>
#include "FeatureDetect.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "global.h"
#include <iostream>
#include <fstream> 
#include <conio.h>
#include <cmath>

using namespace cv;
using namespace std;

/*
 *函数说明：完成相机标定
 *输入参数：
 PicPath:标定所用图像文件的路径
 Cali_Result：保存标定结果的.txt文件名
 row_corner_num：标定图像内角点（不和边缘接触的角点）行数
 column_corner_num：标定图像内角点（不和边缘接触的角点）列数
 grid_width:实际测量得到的标定板上每个棋盘格的长
 grid_height:实际测量得到的标定板上每个棋盘格的宽
 Flag:相机标志，1为左相机，其他为右相机
 *返 回 值：无
 */
void CircleCalibration(string PicPath,string Cali_Result,int row_corner_num, 
	int column_corner_num,double grid_width,double grid_height,string ImgPath,int Flag);

/*
 *函数功能：释放BlobSeq中的Blob数据内存，清除BlobSeq中的数据
 *参数说明
 CArray<Blob> m_LeftBlobSeq	待释放内存的左相机团块队列。
 */
/*左相机*/
void Blob_ReleaseLeftBlobSeq();

/*
 *函数功能：清除团块队列中面积不在指定范围内的团块数据
 *参数说明
 CArray<Blob> m_LeftBlobSeq     待处理的团块队列
 int MinArea,MaxArea    指定的团块面积范围
*/
/*左相机*/
void Blob_DenoisingLeftArea(int MinArea, int MaxArea);

/*
 *函数功能：检测圆形特征
 *参数说明
 const CvArr* SrcImage         待处理的灰度图像或二值化图像
 double lowthresh, highthresh  自适应Canny边缘检测的高低阈值
 int LeastPixelNum             目标轮廓包含的最小像素点数目
 double AspectRatio            纵横比
 double Circularity            圆形度
*/
/*左相机*/
void Detect_LeftCircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity);

/*
 *函数功能：对圆形标志点进行排序
 *参数说明
 vector<Point2f> buff         待排序的圆心坐标
*/
void CircleSort(vector<Point2f> buff);

/*
 *函数功能：计算两点之间的距离里
 *参数说明
 vector<Point2f> buff         待计算的圆心坐标
 int a					      坐标序号
 int b                        坐标序号
*/
int CalDistance(vector<Point2f> buff,int a,int b);