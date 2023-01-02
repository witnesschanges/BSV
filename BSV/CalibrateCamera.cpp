#pragma once
#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include "BSV.h"
#include "BSVDlg.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CalibrateCamera.h"

CameraCalibration::CameraCalibration()
{
	m_image_count = 0;  //图像数量
}

void CameraCalibration::DrawCorners(Mat imageInput, bool patternWasFound, string banner)
{
	drawChessboardCorners(imageInput, m_board_size, m_image_points_buf, patternWasFound); //用于在图片中标记角点
	namedWindow(banner, WINDOW_NORMAL);
	imshow(banner, imageInput);//显示图片
	waitKey(500);
}

void CameraCalibration::StartRecoginzeCorner(ifstream& fin, ofstream& fout)
{
	string filename;
	int success_count;
	string banner = "相机标定";
	while (getline(fin, filename))
	{
		Mat imageInput = imread(filename);
		if (m_image_count == 0)  //以读入的第一张图片宽高代表图片组的宽高
		{
			m_image_size.width = imageInput.cols;
			m_image_size.height = imageInput.rows;
			cout << "图片宽度为：" << m_image_size.width << endl;
			cout << "图片高度为：" << m_image_size.height << endl;
		}
		bool isSuccess = findChessboardCorners(imageInput, m_board_size, m_image_points_buf, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
		if (!isSuccess)
		{
			cout << "第" << m_image_count << "张图片角点识别失败！" << endl;
			DrawCorners(imageInput, false, banner);
		}
		else
		{
			cout << "第" << m_image_count << "张图片角点识别成功！" << endl;
			success_count++;
			m_image_points_seq.push_back(m_image_points_buf);
			DrawCorners(imageInput, true, banner);
		}
		m_image_count++;
	}
	if (success_count < m_image_count)
	{
		cout << "角点识别成功的图片数量：" << success_count << endl;
		cout << "待标定图片中有角点不能全部识别，程序强制退出！" << endl;
		waitKey(3000);
		exit(1);
	}
}

void CameraCalibration::PrintCornerCoordinates()
{
	int CornerNum = m_board_size.width * m_board_size.height;  //每张图片上总的角点数
	int total = m_image_points_seq.size();
	for (int ii = 0; ii < total; ii++)
	{
		/*用于输出每张图片检测到第一个角点的坐标*/
		if (0 == ii % CornerNum) //CornerNum是每幅图片的角点个数。此判断语句是为了输出图片号，便于控制台观看 
		{
			int i = -1;
			i = ii / CornerNum;
			int j = i + 1;
			cout << "--> 第 " << j << "图片的数据 --> : " << endl;
		}
		else if (0 == ii % 3)// 此判断语句，格式化输出，便于控制台查看
		{
			cout << endl;
		}
		else
		{
			cout.width(10);//调整字段的宽度
		}
		cout << " -->" << m_image_points_seq[ii][0].x;
		cout << " -->" << m_image_points_seq[ii][0].y;
	}
	/*用于输出每张图片检测到角点的坐标*/
	//cout<<"<--第 "<<ii+1 <<"图片的数据 -->"<<endl;
	//for (int jj=0 ; jj<CornerNum ;jj++)
	//{
	//	cout<<" -->"<<m_image_points_seq[ii][jj].x<<"\t";
	//	cout<<" -->"<<m_image_points_seq[ii][jj].y<<"\t";
	//	cout<<endl;
	//}
}

vector<vector<Point3f>> CameraCalibration::InitialCornerCorodinates(Size square_size)
{
	vector<vector<Point3f>> object_points;
	int i, j, t;
	for (t = 0; t < m_image_count; t++)
	{
		vector<Point3f> tempPointSet;
		for (i = 0; i < m_board_size.height; i++)
		{
			for (j = 0; j < m_board_size.width; j++)
			{
				Point3f realPoint;
				/* 假设标定板放在世界坐标系中z=0的平面上 */
				realPoint.x = i * square_size.width;
				realPoint.y = j * square_size.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		object_points.push_back(tempPointSet);
	}
	return object_points;
}

vector<int> CameraCalibration::InitialCornerCounts()
{
	vector<int> point_counts;
	for (int i = 0; i < m_image_count; i++)
	{
		point_counts.push_back(m_board_size.width * m_board_size.height);
	}
	return point_counts;
}

void CameraCalibration::EvaluateCalibrationResults(vector<int> point_counts, vector<vector<Point3f>> object_points, vector<Mat> tvecsMat,
	vector<Mat> rvecsMat, Mat cameraMatrix, Mat distCoeffs, ofstream& fout)
{
	double total_err = 0.0; // 所有图像的平均误差的总和
	double err = 0.0; // 每幅图像的平均误差
	vector<Point2f> image_points2; // 保存重新计算得到的投影点

	for (int i = 0; i < m_image_count; i++)
	{
		vector<Point3f> tempPointSet = object_points[i];
		/* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
		projectPoints(tempPointSet, rvecsMat[i], tvecsMat[i], cameraMatrix, distCoeffs, image_points2);
		/* 计算新的投影点和旧的投影点之间的误差*/
		vector<Point2f> tempImagePoint = m_image_points_seq[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		//norm函数其实把两个通道分别分开来计算的(X1-X2)^2的值，然后统一求和，最后进行根号
		total_err += err /= point_counts[i];
		std::cout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
		fout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
	}
	std::cout << "总体平均误差：" << total_err / m_image_count << "像素" << endl;
	fout << "总体平均误差：" << total_err / m_image_count << "像素" << endl << endl;
}

void CameraCalibration::SaveCalibrationResults()
{

}

void CameraCalibration::CalibrateCamera(string pic_Path, string cali_Result, int row_corner_num, int column_corner_num, double grid_width, double grid_height)
{
	ifstream fin(pic_Path);//存在空白行会报错
	ofstream fout(cali_Result);
	m_board_size = Size(row_corner_num, column_corner_num);// 标定板上每行、列的角点数

	cout << "*** 开始识别角点 ***" << endl;
	StartRecoginzeCorner(fin, fout);

	PrintCornerCoordinates();

	//以下是摄像机标定
	cout << "\n*** 开始标定 ***" << endl;
	vector<vector<Point3f>> object_points = InitialCornerCorodinates(Size(grid_width, grid_height)); // 每幅图像中角点的三维坐标
	vector<int> point_counts = InitialCornerCounts();// 每幅图像中角点的数量
	/*内外参数*/
	Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); // 摄像机内参数矩阵
	Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0)); // 摄像机的5个畸变系数：k1, k2, p1, p2, k3
	vector<Mat> tvecsMat;  // 每幅图像的旋转向量
	vector<Mat> rvecsMat; // 每幅图像的平移向量
	calibrateCamera(object_points, m_image_points_seq, m_image_size, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
	cout << "标定完成！" << endl;

	//对标定结果进行评价
	cout << "*** 开始评价标定结果 ***\n";
	EvaluateCalibrationResults(point_counts, object_points, tvecsMat, rvecsMat, cameraMatrix, distCoeffs, fout);
	std::cout << "*** 评价完成！***" << endl;

	//保存标定结果  	
	std::cout << "*** 开始保存标定结果 ***" << endl;
	Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); // 保存每幅图像的旋转矩阵
	fout << "相机内参数矩阵：" << endl;
	fout << cameraMatrix << endl << endl;
	fout << "畸变系数：\n";
	fout << distCoeffs << endl << endl << endl;
	for (int i = 0; i < m_image_count; i++)
	{
		fout << "第" << i + 1 << "幅图像的旋转向量：" << endl;
		fout << tvecsMat[i] << endl;
		/* 将旋转向量转换为相对应的旋转矩阵 */
		Rodrigues(tvecsMat[i], rotation_matrix);
		fout << "第" << i + 1 << "幅图像的旋转矩阵：" << endl;
		fout << rotation_matrix << endl;
		fout << "第" << i + 1 << "幅图像的平移向量：" << endl;
		fout << rvecsMat[i] << endl << endl;
	}
	std::cout << "***完成保存***" << endl;
	fout << endl;
	/* 显示标定结果  */
	Mat mapx = Mat(m_image_size, CV_32FC1);
	Mat mapy = Mat(m_image_size, CV_32FC1);
	Mat R = Mat::eye(3, 3, CV_32F);
	std::cout << "***保存矫正图像***" << endl;
	string imageFileName;
	std::stringstream StrStm;
	for (int i = 0; i != m_image_count; i++)
	{
		std::cout << "图片" << i + 1 << "保存成功！" << endl;
		initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, m_image_size, CV_32FC1, mapx, mapy);
		StrStm.clear();
		imageFileName.clear();
		string filePath = "Ori_LImage\\chess";
		StrStm << i + 1;
		StrStm >> imageFileName;
		filePath += imageFileName;
		filePath += ".bmp";	//原始图像路径
		Mat imageSource = imread(filePath);
		Mat newimage = imageSource.clone();
		//另一种不需要转换矩阵的方式
		//undistort(imageSource,newimage,cameraMatrix,distCoeffs);
		remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);
		namedWindow("原始图像", WINDOW_NORMAL);//将显示图像可放大缩小
		imshow("原始图像", imageSource);
		namedWindow("矫正后图像", WINDOW_NORMAL);
		imshow("矫正后图像", newimage);
		waitKey();		//用户按键触发;如果用户没有按下键,则接续等待
		StrStm.clear();
		filePath.clear();
		StrStm << i + 1;
		StrStm >> imageFileName;
		imageFileName += "_d.jpg";
		string SavefilePath = "Save_LImage\\" + imageFileName;	//保存图像路径
		imwrite(SavefilePath, newimage);
	}
	std::cout << "*** 保存结束 ***" << endl;
	return;
}