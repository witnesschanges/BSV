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

CameraCalibration::CameraCalibration(string pic_Path, string cali_Result, int row_corner_num, int column_corner_num, double grid_width, double grid_height) : m_fin(ifstream(pic_Path)), m_fout(ofstream(cali_Result))
{
	m_pic_Path = pic_Path;
	m_cali_Result = cali_Result;

	m_board_size = Size(row_corner_num, column_corner_num);
	m_square_size = Size(grid_width, grid_height);

	m_image_count = 0;
	m_cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0));
	m_distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0));

	m_rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0));

	m_object_points = InitialCornerCorodinates();
	m_point_counts = InitialCornerCounts();

}

void CameraCalibration::DrawCorners(Mat imageInput, bool patternWasFound, string banner)
{
	drawChessboardCorners(imageInput, m_board_size, m_image_points_buf, patternWasFound); //������ͼƬ�б�ǽǵ�
	ShowImageWindow(banner, imageInput);
	waitKey(500);
}

void CameraCalibration::ShowImageWindow(string banner, Mat imageInput)
{
	namedWindow(banner, WINDOW_NORMAL);
	imshow(banner, imageInput);//��ʾͼƬ
}

void CameraCalibration::StartRecoginzeCorner()
{
	string filename;
	int success_count;
	string banner = "����궨";
	while (getline(m_fin, filename))
	{
		Mat imageInput = imread(filename);
		if (m_image_count == 0)  //�Զ���ĵ�һ��ͼƬ��ߴ���ͼƬ��Ŀ��
		{
			m_image_size.width = imageInput.cols;
			m_image_size.height = imageInput.rows;
			cout << "ͼƬ���Ϊ��" << m_image_size.width << endl;
			cout << "ͼƬ�߶�Ϊ��" << m_image_size.height << endl;
		}
		bool isSuccess = findChessboardCorners(imageInput, m_board_size, m_image_points_buf, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
		if (!isSuccess)
		{
			cout << "��" << m_image_count << "��ͼƬ�ǵ�ʶ��ʧ�ܣ�" << endl;
			DrawCorners(imageInput, false, banner);
		}
		else
		{
			cout << "��" << m_image_count << "��ͼƬ�ǵ�ʶ��ɹ���" << endl;
			success_count++;
			m_image_points_seq.push_back(m_image_points_buf);
			DrawCorners(imageInput, true, banner);
		}
		m_image_count++;
	}
	if (success_count < m_image_count)
	{
		cout << "�ǵ�ʶ��ɹ���ͼƬ������" << success_count << endl;
		cout << "���궨ͼƬ���нǵ㲻��ȫ��ʶ�𣬳���ǿ���˳���" << endl;
		waitKey(3000);
		exit(1);
	}
}

void CameraCalibration::PrintCornerCoordinates()
{
	int CornerNum = m_board_size.width * m_board_size.height;  //ÿ��ͼƬ���ܵĽǵ���
	int total = m_image_points_seq.size();
	for (int ii = 0; ii < total; ii++)
	{
		/*�������ÿ��ͼƬ��⵽��һ���ǵ������*/
		if (0 == ii % CornerNum) //CornerNum��ÿ��ͼƬ�Ľǵ���������ж������Ϊ�����ͼƬ�ţ����ڿ���̨�ۿ� 
		{
			int i = -1;
			i = ii / CornerNum;
			int j = i + 1;
			cout << "--> �� " << j << "ͼƬ������ --> : " << endl;
		}
		else if (0 == ii % 3)// ���ж���䣬��ʽ����������ڿ���̨�鿴
		{
			cout << endl;
		}
		else
		{
			cout.width(10);//�����ֶεĿ��
		}
		cout << " -->" << m_image_points_seq[ii][0].x;
		cout << " -->" << m_image_points_seq[ii][0].y;
	}
	/*�������ÿ��ͼƬ��⵽�ǵ������*/
	//cout<<"<--�� "<<ii+1 <<"ͼƬ������ -->"<<endl;
	//for (int jj=0 ; jj<CornerNum ;jj++)
	//{
	//	cout<<" -->"<<m_image_points_seq[ii][jj].x<<"\t";
	//	cout<<" -->"<<m_image_points_seq[ii][jj].y<<"\t";
	//	cout<<endl;
	//}
}

vector<vector<Point3f>> CameraCalibration::InitialCornerCorodinates()
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
				/* ����궨�������������ϵ��z=0��ƽ���� */
				realPoint.x = i * m_square_size.width;
				realPoint.y = j * m_square_size.height;
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

void CameraCalibration::EvaluateCalibrationResults()
{
	double total_err = 0.0; // ����ͼ���ƽ�������ܺ�
	double err = 0.0; // ÿ��ͼ���ƽ�����
	vector<Point2f> image_points2; // �������¼���õ���ͶӰ��

	for (int i = 0; i < m_image_count; i++)
	{
		vector<Point3f> tempPointSet = m_object_points[i];
		/* ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ�� */
		projectPoints(tempPointSet, m_rvecsMat[i], m_tvecsMat[i], m_cameraMatrix, m_distCoeffs, image_points2);
		/* �����µ�ͶӰ��;ɵ�ͶӰ��֮������*/
		vector<Point2f> tempImagePoint = m_image_points_seq[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		//norm������ʵ������ͨ���ֱ�ֿ��������(X1-X2)^2��ֵ��Ȼ��ͳһ��ͣ������и���
		total_err += err /= m_point_counts[i];
		std::cout << "��" << i + 1 << "��ͼ���ƽ����" << err << "����" << endl;
		m_fout << "��" << i + 1 << "��ͼ���ƽ����" << err << "����" << endl;
	}
	std::cout << "����ƽ����" << total_err / m_image_count << "����" << endl;
	m_fout << "����ƽ����" << total_err / m_image_count << "����" << endl << endl;
}

void CameraCalibration::SaveCalibrationResults()
{
	m_fout << "����ڲ�������" << endl;
	m_fout << m_cameraMatrix << endl << endl;
	m_fout << "����ϵ����\n";
	m_fout << m_distCoeffs << endl << endl << endl;
	for (int i = 0; i < m_image_count; i++)
	{
		m_fout << "��" << i + 1 << "��ͼ�����ת������" << endl;
		m_fout << m_tvecsMat[i] << endl;
		/* ����ת����ת��Ϊ���Ӧ����ת���� */
		Rodrigues(m_tvecsMat[i], m_rotation_matrix);
		m_fout << "��" << i + 1 << "��ͼ�����ת����" << endl;
		m_fout << m_rotation_matrix << endl;
		m_fout << "��" << i + 1 << "��ͼ���ƽ��������" << endl;
		m_fout << m_rvecsMat[i] << endl << endl;
	}
}

void CameraCalibration::StoreAndDisplayCalibrationResults()
{
	Mat mapx = Mat(m_image_size, CV_32FC1);
	Mat mapy = Mat(m_image_size, CV_32FC1);
	Mat R = Mat::eye(3, 3, CV_32F);
	string imageFileName;
	std::stringstream StrStm;
	for (int i = 0; i != m_image_count; i++)
	{
		std::cout << "ͼƬ" << i + 1 << "����ɹ���" << endl;
		initUndistortRectifyMap(m_cameraMatrix, m_distCoeffs, R, m_cameraMatrix, m_image_size, CV_32FC1, mapx, mapy);
		StrStm.clear();
		imageFileName.clear();
		string filePath = "Ori_LImage\\chess";
		StrStm << i + 1;
		StrStm >> imageFileName;
		filePath += imageFileName;
		filePath += ".bmp";	//ԭʼͼ��·��
		Mat imageSource = imread(filePath);
		Mat newimage = imageSource.clone();
		//��һ�ֲ���Ҫת������ķ�ʽ
		//undistort(imageSource,newimage,m_cameraMatrix,m_distCoeffs);
		remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);
		ShowImageWindow("ԭʼͼ��", imageSource);
		ShowImageWindow("������ͼ��", newimage);
		waitKey();		//�û���������;����û�û�а��¼�,������ȴ�
		StrStm.clear();
		filePath.clear();
		StrStm << i + 1;
		StrStm >> imageFileName;
		imageFileName += "_d.jpg";
		string SavefilePath = "Save_LImage\\" + imageFileName;	//����ͼ��·��
		imwrite(SavefilePath, newimage);
	}
}

void CameraCalibration::CalibrateCamera()
{
	cout << "*** ��ʼʶ��ǵ� ***" << endl;
	StartRecoginzeCorner();

	PrintCornerCoordinates();

	cout << "\n*** ��ʼ�궨 ***" << endl;
	calibrateCamera(m_object_points, m_image_points_seq, m_image_size, m_cameraMatrix, m_distCoeffs, m_rvecsMat, m_tvecsMat, 0);
	cout << "�궨��ɣ�" << endl;

	cout << "*** ��ʼ���۱궨��� ***\n";
	EvaluateCalibrationResults();
	std::cout << "*** ������ɣ�***" << endl;

	std::cout << "*** ��ʼ����궨��� ***" << endl;
	SaveCalibrationResults();
	m_fout << endl;
	std::cout << "***��ɱ���***" << endl;

	std::cout << "*** �������ͼ�� ***" << endl;
	StoreAndDisplayCalibrationResults();
	std::cout << "*** ������� ***" << endl;
}