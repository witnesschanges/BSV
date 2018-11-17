#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace std;
using namespace cv;


/*
 *函数说明：阈值条回调函数,与下面Harris角点检测函数同时使用
*/
Mat image;
Mat imageGray;
int thresh=200;
int MaxThresh=255;
void Trackbar(int,void*)
{
	Mat dst,dst8u,dstshow,imageSource;
	dst=Mat::zeros(image.size(),CV_32FC1);  
	imageSource=image.clone();
	cornerHarris(imageGray,dst,3,3,0.04,BORDER_DEFAULT);//角点检测
	normalize(dst,dst8u,0,255,CV_MINMAX);  //归一化
	convertScaleAbs(dst8u,dstshow);//使用线性变换转换输入数组元素成8位无符号整型
	/*
	dst（目标图像）显示，经过归一化转化的Harris角点检测图像，大小与原图像一致，该
	 ，值越大，越有可能是角点。
	image变量经过颜色转换、高斯滤波、归一化、以及线性变换即为dstshow。
	*/
	namedWindow("dst",WINDOW_NORMAL);
	imshow("dst",dstshow);
	for(int i=0;i<image.rows;i++)
	{
		for(int j=0;j<image.cols;j++)
		{
			if(dstshow.at<uchar>(i,j)>thresh)  //阈值判断
			{
				circle(imageSource,Point(j,i),2,Scalar(0,0,255),2); //标注角点
			}
		}
	}
	imshow("Corner Detected",imageSource);
}
/*
 *函数说明：Harris角点检测函数
*/
void CornerDetect(Mat Img)
{
	image = Img;
	cvtColor(image,imageGray,CV_RGB2GRAY);//颜色空间转换函数，该处转换成灰度图
	GaussianBlur(imageGray,imageGray,Size(5,5),1); // 滤波
	namedWindow("Corner Detected",WINDOW_NORMAL);
	createTrackbar("threshold：","Corner Detected",&thresh,MaxThresh,Trackbar);
	imshow("Corner Detected",image);
	Trackbar(0,0);
	waitKey();
}
