#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <iostream>
#include <fstream> 
#include <conio.h>
#include "global.h"
#include "Stereo.h"

using namespace std;
using namespace cv;

//双目相机标定相关参数
StereoCalib m_StereoCalib;
StereoCalib::CornerDatas m_cornersDatas;
StereoCalib::CameraParams m_cameraParams1;
StereoCalib::CameraParams m_cameraParams2;
StereoCalib::StereoParams m_stereoParams;
StereoCalib::RemapMatrixs m_remapMatrixs;

/*
 *函数说明：根据标定图片改写左相机LCamera_calibdata.txt内容
 *输入参数：
 PicPath:标定所用图像文件的路径
 PicNum:标定图片的数量
 *输出参数: 无
 *返 回 值：无
*/
void WriteTxt(string PicPath,int PicNum)
{
	fstream file(PicPath, ios::out);
	ofstream fout(PicPath);
	int k;
	for(k=1;k<=PicNum;k++)
	{
		fout<<ProjectPath<<"Ori_LImage\\chess"<<k<<".bmp"<<endl;
	}
}
/*
 *函数说明：改写右相机LCamera_calibdata.txt内容
*/
void WriteTxt2(string PicPath,int PicNum)
{
	fstream file(PicPath, ios::out);
	ofstream fout(PicPath);
	int k;
	for(k=1;k<=PicNum;k++)
	{
		fout<<ProjectPath<<"Ori_RImage\\chess"<<k<<".bmp"<<endl;
	}
}

/*
 *函数说明：阈值条回调函数,与下面Harris角点检测函数同时使用
*/
//Mat image;
//Mat imageGray;
//int thresh=200;
//int MaxThresh=255;
//void Trackbar(int,void*)
//{
//	Mat dst,dst8u,dstshow,imageSource;
//	dst=Mat::zeros(image.size(),CV_32FC1);  
//	imageSource=image.clone();
//	cornerHarris(imageGray,dst,3,3,0.04,BORDER_DEFAULT);//角点检测
//	normalize(dst,dst8u,0,255,CV_MINMAX);  //归一化
//	convertScaleAbs(dst8u,dstshow);//使用线性变换转换输入数组元素成8位无符号整型
//	/*
//	dst（目标图像）显示，经过归一化转化的Harris角点检测图像，大小与原图像一致，该
//	 ，值越大，越有可能是角点。
//	image变量经过颜色转换、高斯滤波、归一化、以及线性变换即为dstshow。
//	*/
//	namedWindow("dst",WINDOW_NORMAL);
//	imshow("dst",dstshow);
//	for(int i=0;i<image.rows;i++)
//	{
//		for(int j=0;j<image.cols;j++)
//		{
//			if(dstshow.at<uchar>(i,j)>thresh)  //阈值判断
//			{
//				circle(imageSource,Point(j,i),2,Scalar(0,0,255),2); //标注角点
//			}
//		}
//	}
//	imshow("Corner Detected",imageSource);
//}
/*
 *函数说明：Harris角点检测函数
*/
//void CornerDetect(Mat Img)
//{
//	image = Img;
//	cvtColor(image,imageGray,CV_RGB2GRAY);//颜色空间转换函数，该处转换成灰度图
//	GaussianBlur(imageGray,imageGray,Size(5,5),1); // 滤波
//	namedWindow("Corner Detected",WINDOW_NORMAL);
//	createTrackbar("threshold：","Corner Detected",&thresh,MaxThresh,Trackbar);
//	imshow("Corner Detected",image);
//	Trackbar(0,0);
//	waitKey();
//}

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
void Calibration(string PicPath,string Cali_Result,int row_corner_num, 
	int column_corner_num,double grid_width,double grid_height,string ImgPath,int Flag)
{
	ifstream fin(PicPath);//存在空白行会报错
	ofstream fout(Cali_Result);
	Size image_size;
	Size board_size=Size(row_corner_num,column_corner_num);/* 标定板上每行、列的角点数 */
	vector<Point2f> image_points_buf;  /* 缓存每幅图像上检测到的角点 */
	vector<vector<Point2f>> image_points_seq; /* 保存检测到的所有角点 */
	string filename;
	int image_count=0;  //图像数量
	int success_count=0;	//识别成功图像数量
	cout<<"***开始识别角点***"<<endl;
	while (getline(fin,filename))
	{
		Mat imageInput=imread(filename);
		if (image_count == 0)  //以读入的第一张图片宽高代表图片组的宽高
			{
				image_size.width = imageInput.cols;
				image_size.height =imageInput.rows;			
				cout<<"图片宽度为："<<image_size.width<<endl;
				cout<<"图片高度为："<<image_size.height<<endl;
			}
		int a = findChessboardCorners(imageInput,board_size,image_points_buf,
			CALIB_CB_ADAPTIVE_THRESH|CALIB_CB_NORMALIZE_IMAGE);
		if (a == 0)
		{
			cout<<"第"<<image_count+1<<"张图片角点识别失败！"<<endl;
			image_count++;
			drawChessboardCorners(imageInput,board_size,image_points_buf,false); //用于在图片中标记角点
			namedWindow("相机标定",WINDOW_NORMAL);
			imshow("相机标定",imageInput);//显示图片
			waitKey(500);
		}
		else
		{
			cout<<"第"<<image_count+1<<"张图片角点识别成功！"<<endl;
			image_count++;
			success_count++;
			image_points_seq.push_back(image_points_buf);
			/* 在图像上显示角点位置 */
			drawChessboardCorners(imageInput,board_size,image_points_buf,true); //用于在图片中标记角点
			namedWindow("相机标定",WINDOW_NORMAL);
			imshow("相机标定",imageInput);//显示图片
			/*lq:不知道是干什么用的*/
			/*if (image_count == 1)
			{
				imwrite("lq：一张图片的路径",imageInput);
			}*/
			waitKey(500);//暂停0.5S	
		}
	}
	if(success_count<image_count)
	{
		cout<<"角点识别成功的图片数量："<<success_count<<endl;
		cout<<"待标定图片中有角点不能全部识别，程序强制退出！"<<endl;
		waitKey(3000);
		exit(1);
	}
	/*打印角点坐标*/
	int CornerNum=board_size.width*board_size.height;  //每张图片上总的角点数
	int total = image_points_seq.size();
	for (int ii=0 ; ii<total ;ii++)
	{
		/*用于输出每张图片检测到第一个角点的坐标*/
		if (0 == ii%CornerNum)	//CornerNum是每幅图片的角点个数。此判断语句是为了输出图片号，便于控制台观看 
		{	
			int i = -1;
			i = ii/CornerNum;
			int j=i+1;
			cout<<"--> 第 "<<j <<"图片的数据 --> : "<<endl;
		}
		if (0 == ii%3)	// 此判断语句，格式化输出，便于控制台查看
		{
			cout<<endl;
		}
		else
		{
			cout.width(10);//调整字段的宽度
		}
			cout<<" -->"<<image_points_seq[ii][0].x;
			cout<<" -->"<<image_points_seq[ii][0].y;
		}
		/*用于输出每张图片检测到角点的坐标*/
		//cout<<"<--第 "<<ii+1 <<"图片的数据 -->"<<endl;
		//for (int jj=0 ; jj<CornerNum ;jj++)
		//{
		//	cout<<" -->"<<image_points_seq[ii][jj].x<<"\t";
		//	cout<<" -->"<<image_points_seq[ii][jj].y<<"\t";
		//	cout<<endl;
		//}
	//}

	//以下是摄像机标定
	cout<<"\n***开始标定***"<<endl;
	/*棋盘三维信息*/
	Size square_size = Size(grid_width,grid_height);  /* 实际测量得到的标定板上每个棋盘格的大小 */
	vector<vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */
	/*内外参数*/
	Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* 摄像机内参数矩阵 */
	vector<int> point_counts;  // 每幅图像中角点的数量
	Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
	vector<Mat> tvecsMat;  /* 每幅图像的旋转向量 */
	vector<Mat> rvecsMat; /* 每幅图像的平移向量 */
	/* 初始化标定板上角点的三维坐标 */
	int i,j,t;
	for (t=0;t<image_count;t++) 
	{
		vector<Point3f> tempPointSet;
		for (i=0;i<board_size.height;i++) 
		{
			for (j=0;j<board_size.width;j++) 
			{
				Point3f realPoint;
				/* 假设标定板放在世界坐标系中z=0的平面上 */
				realPoint.x = i*square_size.width;
				realPoint.y = j*square_size.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		object_points.push_back(tempPointSet);
	}
	for (i=0;i<image_count;i++)
	{
		point_counts.push_back(board_size.width*board_size.height);
	}
	/* 开始标定 */
	calibrateCamera(object_points,image_points_seq,image_size,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
	cout<<"标定完成！"<<endl;
	//对标定结果进行评价
	cout<<"***开始评价标定结果***\n";
	double total_err = 0.0; /* 所有图像的平均误差的总和 */
	double err = 0.0; /* 每幅图像的平均误差 */
	vector<Point2f> image_points2; /* 保存重新计算得到的投影点 */
	cout<<"->每幅图像的标定误差：\n";
	fout<<"->每幅图像的标定误差：\n";
	for (i=0;i<image_count;i++)
	{
		vector<Point3f> tempPointSet=object_points[i];
		/* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
		projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],cameraMatrix,distCoeffs,image_points2);
		/* 计算新的投影点和旧的投影点之间的误差*/
		vector<Point2f> tempImagePoint = image_points_seq[i];
		Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);
		Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);
		for (int j = 0 ; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		//norm函数其实把两个通道分别分开来计算的(X1-X2)^2的值，然后统一求和，最后进行根号
		total_err += err/=  point_counts[i];   
		std::cout<<"第"<<i+1<<"幅图像的平均误差："<<err<<"像素"<<endl;   
		fout<<"第"<<i+1<<"幅图像的平均误差："<<err<<"像素"<<endl;   
	}   
	std::cout<<"总体平均误差："<<total_err/image_count<<"像素"<<endl;   
	fout<<"总体平均误差："<<total_err/image_count<<"像素"<<endl<<endl;   
	std::cout<<"***评价完成！***"<<endl;
	//保存标定结果  	
	std::cout<<"***开始保存标定结果***"<<endl;       
	Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */
	fout<<"相机内参数矩阵："<<endl;   
	fout<<cameraMatrix<<endl<<endl;   
	fout<<"畸变系数：\n";   
	fout<<distCoeffs<<endl<<endl<<endl;   
	for (int i=0; i<image_count; i++) 
	{ 
		fout<<"第"<<i+1<<"幅图像的旋转向量："<<endl;   
		fout<<rvecsMat[i]<<endl;   
		/* 将旋转向量转换为相对应的旋转矩阵 */   
		Rodrigues(rvecsMat[i],rotation_matrix);   
		fout<<"第"<<i+1<<"幅图像的旋转矩阵："<<endl;   
		fout<<rotation_matrix<<endl;   
		fout<<"第"<<i+1<<"幅图像的平移向量："<<endl;   
		fout<<tvecsMat[i]<<endl<<endl;   
	}   
	std::cout<<"***完成保存***"<<endl; 
	fout<<endl;

	//向BSVDlg.cpp传递变量以及完成标定数据的保存
 	m_cornersDatas.nImages = image_count;
	m_cornersDatas.boardSize = square_size;
	m_cornersDatas.imageSize = image_size;
	m_cornersDatas.objectPoints = object_points;
	if(Flag==1)
	{
		m_cameraParams1.imageSize = image_size;
		m_cameraParams1.cameraMatrix = cameraMatrix;
		m_cameraParams1.distortionCoefficients = distCoeffs;
		m_cameraParams1.rotations = rvecsMat;
		m_cameraParams1.translations = tvecsMat;
		m_cornersDatas.imagePoints1 = image_points_seq;
	}
	else 
	{
		m_cameraParams2.imageSize = image_size;
		m_cameraParams2.cameraMatrix = cameraMatrix;
		m_cameraParams2.distortionCoefficients = distCoeffs;
		m_cameraParams2.rotations = rvecsMat;
		m_cameraParams2.translations = tvecsMat;
		m_cornersDatas.imagePoints2 = image_points_seq;
	}
	/* 显示标定结果  */
 	Mat mapx = Mat(image_size,CV_32FC1);
 	Mat mapy = Mat(image_size,CV_32FC1);
 	Mat R = Mat::eye(3,3,CV_32F);
 	std::cout<<"***保存矫正图像***"<<endl;
 	string imageFileName;
 	std::stringstream StrStm;
 	for (int i = 0 ; i != image_count ; i++)
 	{
 		std::cout<<"图片"<<i+1<<"保存成功！"<<endl;
		initUndistortRectifyMap(cameraMatrix,distCoeffs,R,cameraMatrix,image_size,CV_32FC1,mapx,mapy);		
 		StrStm.clear();
 		imageFileName.clear();
		string filePath=(LPCTSTR)ProjectPath+ImgPath;
 		StrStm<<i+1;
 		StrStm>>imageFileName;
		filePath+=imageFileName;
		filePath+=".bmp";	//原始图像路径
 		Mat imageSource = imread(filePath);
 		Mat newimage = imageSource.clone();
		//另一种不需要转换矩阵的方式
		//undistort(imageSource,newimage,cameraMatrix,distCoeffs);
 		remap(imageSource,newimage,mapx, mapy, INTER_LINEAR);
		namedWindow("原始图像", WINDOW_NORMAL);//将显示图像可放大缩小
		imshow("原始图像",imageSource);
		namedWindow("矫正后图像", WINDOW_NORMAL);
		imshow("矫正后图像",newimage);
		waitKey();		//用户按键触发;如果用户没有按下键,则接续等待
 		StrStm.clear();
 		filePath.clear();
 		StrStm<<i+1;
 		StrStm>>imageFileName;
 		imageFileName += "_d.jpg";
		string A((LPCTSTR)ProjectPath);
		string SavefilePath=A+"Save_LImage\\"+imageFileName;	//保存图像路径
 		imwrite(SavefilePath,newimage);
 	}
 	std::cout<<"***保存结束***"<<endl;
	//以Txt文件打开标定结果 
	ShellExecute(NULL, _T("open"), Cali_Result.c_str(), NULL, NULL, SW_SHOW);
	return ;
}
