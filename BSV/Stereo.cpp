#include "stdafx.h"
#include "stereo.h"


StereoCalib::StereoCalib(void)
{

}
StereoCalib::~StereoCalib(void)
{

}

void StereoCalib::initCorners(Size boardSize, Size imageSize, int nImages, float squareWidth, CornerDatas& cornerDatas)
{
	cornerDatas.boardSize = boardSize;
	cornerDatas.imageSize = imageSize;
	cornerDatas.nImages = nImages;

	int i, j, t;
	for (t = 0; t<nImages; t++)
	{
		vector<Point3f> tempPointSet;
		for (i = 0; i<boardSize.height; i++)
		{
			for (j = 0; j<boardSize.width; j++){
				Point3f realPoint;
				//假定标定板放在世界坐标系中的z=0的平面上
				realPoint.x = i*squareWidth;
				realPoint.y = j*squareWidth;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		cornerDatas.objectPoints.push_back(tempPointSet);
	}
}

void StereoCalib::detectCorners(string filename1, string filename2, CornerDatas& cornerDatas)
{
	//cout << "=========提取角点=========" << endl;
	int image_count = 0;
	string str1, str2;
	vector<cv::Point2f> img1_point_buf; //缓存每幅图像上检测到的角点
	vector<cv::Point2f> img2_point_buf;
	ifstream fin1(filename1);
	ifstream fin2(filename2);
	while (getline(fin1, str1) && getline(fin2, str2))
	{
		image_count++;
		Mat img1 = imread(str1);
		Mat img2 = imread(str2);

		if (!img1.data || !img2.data){
			//cout << "读取图片错误" << endl;
			exit(1);
		}

		if (0 == findChessboardCorners(img1, cornerDatas.boardSize, img1_point_buf))
		{
			//cout << "can not find chessboard corners!\n";  //没找到角点
			exit(1);
		}
		else
		{
			//--------亚像素精确化------------
			/*角点亚像素精确化计算速度偏慢*/
			//Mat view_gray;
			//cvtColor(img1, view_gray, CV_RGB2GRAY);
			//find4QuadCornerSubpix(view_gray, img1_point_buf, Size(5, 5));
			//保存角点
			cornerDatas.imagePoints1.push_back(img1_point_buf);
			//在图像上显示角点位置
			//char name[100];
			//sprintf_s(name, "..\\pic_corners_l\\%d.jpg", image_count);
			//imwrite(name, view_gray);

		}

		if (0 == findChessboardCorners(img2, cornerDatas.boardSize, img2_point_buf))
		{
			//cout << "can not find chessboard corners!\n";  //没找到角点
			exit(1);
		}
		else
		{
			//--------亚像素精确化------------
			//Mat view_gray1;
			//cvtColor(img2, view_gray1, CV_RGB2GRAY);
			//find4QuadCornerSubpix(view_gray1, img2_point_buf, Size(5, 5));
			//保存角点
			cornerDatas.imagePoints2.push_back(img2_point_buf);
			//在图像上显示角点位置
			//drawChessboardCorners(view_gray1, cornerDatas.boardSize, img2_point_buf, false); //用于在图片中标记角点

			//char name1[100];
			//sprintf_s(name1, "..\\pic_corners_r\\%d.jpg", image_count);
			//imwrite(name1, view_gray1);
		}
	}
	//cout << "提取角点完成" << endl;
}

int StereoCalib::saveCameraData(CameraParams& CameraParams1, string filename)
{
	std::string filename_ = filename;

	//写入数据
	cv::FileStorage fs(filename_.c_str(), cv::FileStorage::WRITE);
	if (fs.isOpened())
	{
		//time_t rawtime;
		//time(&rawtime);
		//fs << "calibrationDate" << asctime(localtime(&rawtime));

		fs << "imageSize" << "[" << CameraParams1.imageSize.width << CameraParams1.imageSize.height << "]";

		fs << "cameraMatrix" << CameraParams1.cameraMatrix;
		fs << "distortionCoefficients" << CameraParams1.distortionCoefficients;

		int nImages = CameraParams1.rotations.size();
		fs << "nImages" << nImages;
		for (int i = 0; i < nImages; i++)
		{
			char matName[50];
			sprintf_s(matName, "rotaionMatrix_%d", i);
			fs << matName << CameraParams1.rotations[i];
		}
		for (int i = 0; i < nImages; i++)
		{
			char matName[50];
			sprintf_s(matName, "translationMatrix_%d", i);
			fs << matName << CameraParams1.translations[i];
		}
		fs.release();
		return 1;
	}
	else
	{
		return 0;
	}
}

int StereoCalib::getStereoCalibrateError(CornerDatas& cornerDatas, StereoParams& sterepParams, double& err)
{
	// 利用对极线约束检查立体校正效果
	// because the output fundamental matrix implicitly
	// includes all the output information,
	// we can check the quality of calibration using the
	// epipolar geometry constraint: m2^t*F*m1=0

	vector<cv::Vec3f> epilines[2];
	vector<vector<cv::Point2f> > imagePoints[2];
	cv::Mat cameraMatrix[2], distCoeffs[2];
	int npoints = 0;
	int i, j, k;

	imagePoints[0] = cornerDatas.imagePoints1;
	imagePoints[1] = cornerDatas.imagePoints2;
	cameraMatrix[0] = sterepParams.cameraParams1.cameraMatrix;
	cameraMatrix[1] = sterepParams.cameraParams2.cameraMatrix;
	distCoeffs[0] = sterepParams.cameraParams1.distortionCoefficients;
	distCoeffs[1] = sterepParams.cameraParams2.distortionCoefficients;

	for (i = 0; i < cornerDatas.nImages; i++)
	{
		int npt = (int)imagePoints[0][i].size();
		cv::Mat imgpt[2];

		for (k = 0; k < 2; k++)
		{
			imgpt[k] = cv::Mat(imagePoints[k][i]);
			// 计算校正后的棋盘角点坐标
			undistortPoints(imgpt[k], imgpt[k], cameraMatrix[k], distCoeffs[k], cv::Mat(), cameraMatrix[k]);
			// 计算对极线
			computeCorrespondEpilines(imgpt[k], k + 1, sterepParams.foundational, epilines[k]);
		}

		// 计算对极线误差
		for (j = 0; j < npt; j++)
		{
			double errij =
				fabs(imagePoints[0][i][j].x * epilines[1][j][0] +
				imagePoints[0][i][j].y * epilines[1][j][1] + epilines[1][j][2]) +
				fabs(imagePoints[1][i][j].x * epilines[0][j][0] +
				imagePoints[1][i][j].y * epilines[0][j][1] + epilines[0][j][2]);
			err += errij;
		}
		npoints += npt;
	}
	err /= npoints;

	return 1;
}

void StereoCalib::mstereoRectify(CameraParams& CameraParams1, CameraParams& CameraParams2, StereoParams& stereoParams, RemapMatrixs& remapMatrixs)
{
	//双目矫正
	Mat R1, R2, P1, P2, Q;
	Rect roi1, roi2;
	double alpha = -1;

	stereoRectify(CameraParams1.cameraMatrix, CameraParams1.distortionCoefficients,
		CameraParams2.cameraMatrix, CameraParams2.distortionCoefficients, CameraParams1.imageSize,
		stereoParams.rotation, stereoParams.translation, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY,
		alpha,
		CameraParams1.imageSize,
		&roi1, &roi2);


	//输出数据
	Q.copyTo(remapMatrixs.Q);
	remapMatrixs.roi1 = roi1;
	remapMatrixs.roi2 = roi2;

	//初始化
	remapMatrixs.mX1 = cv::Mat(CameraParams1.imageSize, CV_32FC1);
	remapMatrixs.mY1 = cv::Mat(CameraParams1.imageSize, CV_32FC1);
	remapMatrixs.mX2 = cv::Mat(CameraParams1.imageSize, CV_32FC1);
	remapMatrixs.mY2 = cv::Mat(CameraParams1.imageSize, CV_32FC1);

	//生成图像校正所需的像素映射矩阵
	initUndistortRectifyMap(
		CameraParams1.cameraMatrix,
		CameraParams1.distortionCoefficients,
		R1, P1,
		CameraParams1.imageSize,
		CV_16SC2,
		remapMatrixs.mX1, remapMatrixs.mY1);

	initUndistortRectifyMap(
		CameraParams2.cameraMatrix,
		CameraParams2.distortionCoefficients,
		R2, P2,
		CameraParams1.imageSize,
		CV_16SC2,
		remapMatrixs.mX2, remapMatrixs.mY2);
}

int StereoCalib::saveCalibrationDatas(string filename, CornerDatas& cornerDatas, StereoParams& stereoParams, RemapMatrixs& remapMatrixs)
{
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);

	if (fs.isOpened())
	{
		//time_t rawtime;
		//time(&rawtime);
		//fs << "calibrationDate" << asctime(localtime(&rawtime));

		fs << "num_boards" << cornerDatas.nImages;
		fs << "imageSize" << "[" << cornerDatas.imageSize.width << cornerDatas.imageSize.height << "]";

		fs << "leftCameraMatrix" << stereoParams.cameraParams1.cameraMatrix;
		fs << "leftDistortCoefficients" << stereoParams.cameraParams1.distortionCoefficients;
		fs << "rightCameraMatrix" << stereoParams.cameraParams2.cameraMatrix;
		fs << "rightDistortCoefficients" << stereoParams.cameraParams2.distortionCoefficients;
		fs << "rotationMatrix" << stereoParams.rotation;
		fs << "translationVector" << stereoParams.translation;
		fs << "foundationalMatrix" << stereoParams.foundational;

		//fs << "rectifyMethod" << "BOUGUET";
		fs << "leftValidArea" << "[:"
			<< remapMatrixs.roi1.x << remapMatrixs.roi1.y
			<< remapMatrixs.roi1.width << remapMatrixs.roi1.height << "]";
		fs << "rightValidArea" << "[:"
			<< remapMatrixs.roi2.x << remapMatrixs.roi2.y
			<< remapMatrixs.roi2.width << remapMatrixs.roi2.height << "]";
		fs << "QMatrix" << remapMatrixs.Q;
		fs << "remapX1" << remapMatrixs.mX1;
		fs << "remapY1" << remapMatrixs.mY1;
		fs << "remapX2" << remapMatrixs.mX2;
		fs << "remapY2" << remapMatrixs.mY2;

		fs.release();
		return 1;
	}
	else
	{
		return 0;
	}
}

int StereoCalib::remapImage(cv::Mat& img1, cv::Mat& img2, cv::Mat& img1r, cv::Mat& img2r, RemapMatrixs& remapMatrixs)
{
	if (!remapMatrixs.mX1.empty() && !remapMatrixs.mY1.empty())
	{
		cv::remap(img1, img1r, remapMatrixs.mX1, remapMatrixs.mY1, cv::INTER_LINEAR);

	}
	if (!remapMatrixs.mX2.empty() && !remapMatrixs.mY2.empty())
	{
		cv::remap(img2, img2r, remapMatrixs.mX2, remapMatrixs.mY2, cv::INTER_LINEAR);

	}
	return 1;
}
