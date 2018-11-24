#include "stdafx.h"
#include "CircleCalibration.h"

using namespace std;
using namespace cv;

//团块参数声明
CArray<Blob> BlobSeq;
//双目相机标定相关参数声明
StereoCalib stereoCalib;
StereoCalib::CornerDatas cornersDatas;
StereoCalib::CameraParams cameraParams1;
StereoCalib::CameraParams cameraParams2;
StereoCalib::StereoParams stereoParams;
StereoCalib::RemapMatrixs remapMatrixs;

void CircleCalibration(string PicPath,string Cali_Result,int row_corner_num, 
	int column_corner_num,double grid_width,double grid_height,string ImgPath,int Flag)
{
	//1、首先读入图片并对圆形进行检测
	ifstream fin(PicPath);//存在空白行会报错
	ofstream fout(Cali_Result);
	string filename;/* 文件夹名称*/
	int image_count=0;  /* 图像数量*/
	int success_count=0;	/* 识别成功图像数量*/
	Size image_size;
	Size board_size=Size(column_corner_num,row_corner_num);/* 标定板上每行、列的圆点数 */
	vector<Point2f> image_points_buf;  /* 缓存每幅图像上检测到的圆心 */
	vector<Point2f> new_points_buf; /* 缓存每幅图像上排序后的大圆圆心 */
	vector<Point2f> image_bigpoints_buf; /* 缓存每幅图像上检测到的大圆圆心 */
	vector<vector<Point2f>> image_points_seq; /* 保存检测到的所有圆心 */
	vector<vector<Point2f>> image_bigpoints_seq; /* 保存检测到的所有大圆圆心 */
	while (getline(fin,filename))
	{
		//2、圆形检测并排序
		//读入图像
		Mat imageInput=imread(filename);
		if (image_count == 0)  //以读入的第一张图片宽高代表图片组的宽高
			{
				image_size.width = imageInput.cols;
				image_size.height =imageInput.rows;			
				cout<<"图片宽度为："<<image_size.width<<endl;
				cout<<"图片高度为："<<image_size.height<<endl;
			}
		Mat LSrcImg = imageInput;
		Mat LFilImg;
		//双边滤波，保边去噪
		//bilateralFilter(LSrcImg, LFilImg, 25, 25*2, 25/2);
		//高斯滤波
		GaussianBlur(LSrcImg, LFilImg, Size(7,7), 0, 0);
			
		Mat LBinImg;
		//自适应阈值分割图像	
		//adaptiveThreshold(LFilImg, LBinImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, 
		//CV_THRESH_BINARY, 7, 5);	
		//固定阈值分割图像，二值化图像
		threshold(LFilImg, LBinImg, 100, 255, CV_THRESH_BINARY);
			
		//圆形初始检测，未排序
		Blob_ReleaseBlobSeq();
		Detect_CircleDetect(LBinImg, 0, 255, 0.8, 0.8);
		Blob_DenoisingArea(600, 8000);
		int ln = BlobSeq.GetSize();
		for (int i=0;i<ln;i+=1)
		{
			Blob pB = BlobSeq.GetAt(i);
			Point2f center;
			center.x = pB.BlobX;
			center.y = pB.BlobY;
			image_points_buf.push_back(center);
		}
		if (image_points_buf.size()!=99)
		{
			cout<<"第"<<image_count<<"张图片提取标定点数量不匹配！"<<endl;
			image_count++;	
			drawChessboardCorners(LBinImg,board_size,image_points_buf,false); 
			namedWindow("相机标定",WINDOW_NORMAL);
			imshow("相机标定",LBinImg);//显示图片
			waitKey(500);
			image_points_buf.clear();
		}
		else
		{
			//圆形标志点排序
			cout<<"第"<<image_count+1<<"张图片提取标定点成功！"<<endl;
			image_count++;
			success_count++;
			//在此处可添加显示未排序前的圆点标记图片的代码
			
			//筛选出大圆圆心
			Blob_DenoisingArea(1500, 8000);
			int bn = BlobSeq.GetSize();
			for (int i=0;i<bn;i+=1)
			{
				Blob pB = BlobSeq.GetAt(i);
				Point2f center;
				center.x = pB.BlobX;
				center.y = pB.BlobY;
				image_bigpoints_buf.push_back(center);	
			}
			new_points_buf = CircleSort(image_points_buf,image_bigpoints_buf); //image_points_buf可能存在被清空的问题
			//将所有检测到的圆形按顺序标识出来
			drawChessboardCorners(LBinImg,board_size,new_points_buf,true); 
			namedWindow("圆形检测并排序",WINDOW_NORMAL);
			imshow("圆形检测并排序",LBinImg);
			waitKey(500);
			image_bigpoints_seq.push_back(image_bigpoints_buf);
		}
		image_points_seq.push_back(new_points_buf);
		//每张图片检测完之后清除图片数据
		image_bigpoints_buf.clear();
		image_points_buf.clear();
		new_points_buf.clear();
	}

	if(success_count<image_count)
	{
		AfxMessageBox(_T("图片中圆点识别不完整，程序强制退出！"));
		exit(1);//程序退出
	}

	//3、摄像机标定
	cout<<"\n***开始标定***"<<endl;
	Size square_size = Size(grid_width,grid_height);  //实际测量得到的标定板上每个棋盘格的大小
	vector<vector<Point3f>> object_points; //保存标定板上角点的三维坐标
	Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); //摄像机内参数矩阵 
	vector<int> point_counts;  // 每幅图像中圆点的数量
	Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); //摄像机的5个畸变系数：k1,k2,p1,p2,k3 
	vector<Mat> tvecsMat;  //每幅图像的旋转向量
	vector<Mat> rvecsMat; //每幅图像的平移向量
	//初始化标定板上角点的三维坐标
	int i,j,t;
	for (t=0;t<image_count;t++) 
	{
		vector<Point3f> tempPointSet;
		for (i=0;i<board_size.height;i++) 
		{
			for (j=0;j<board_size.width;j++) 
			{
				Point3f realPoint;
				// 假设标定板放在世界坐标系中z=0的平面上
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
	calibrateCamera(object_points,image_points_seq,image_size,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
	cout<<"标定完成！"<<endl;
	//4、对标定结果进行评价
	cout<<"***开始评价标定结果***\n";
	double total_err = 0.0; //所有图像的平均误差的总和
	double err = 0.0; // 每幅图像的平均误差
	vector<Point2f> image_points2; //保存重新计算得到的投影点
	cout<<"->每幅图像的标定误差：\n";
	fout<<"->每幅图像的标定误差：\n";
	for (i=0;i<image_count;i++)
	{
		vector<Point3f> tempPointSet=object_points[i];
		//通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点
		projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],cameraMatrix,distCoeffs,image_points2);
		//计算新的投影点和旧的投影点之间的误差
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

	//5、保存标定结果  	
	std::cout<<"***开始保存标定结果***"<<endl;       
	Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0));//保存每幅图像的旋转矩阵
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

	//6、图片矫正并显示
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
		waitKey();	//用户按键触发;如果用户没有按下键,则接续等待
 		StrStm.clear();
 		filePath.clear();
 		StrStm<<i+1;
 		StrStm>>imageFileName;
 		imageFileName += "_d.jpg";
		string A((LPCTSTR)ProjectPath);
		string SavefilePath=A+"Save_LImage\\"+imageFileName;//保存图像路径
 		imwrite(SavefilePath,newimage);
 	}
 	std::cout<<"***保存结束***"<<endl;
	//以Txt文件打开标定结果 
	ShellExecute(NULL, _T("open"), Cali_Result.c_str(), NULL, NULL, SW_SHOW);

	//7、向BSVDlg.cpp传递变量以及完成的单相机标定数据
 	cornersDatas.nImages = image_count;
	cornersDatas.boardSize = square_size;
	cornersDatas.imageSize = image_size;
	cornersDatas.objectPoints = object_points;
	if(Flag==1)
	{
		cameraParams1.imageSize = image_size;
		cameraParams1.cameraMatrix = cameraMatrix;
		cameraParams1.distortionCoefficients = distCoeffs;
		cameraParams1.rotations = rvecsMat;
		cameraParams1.translations = tvecsMat;
		cornersDatas.imagePoints1 = image_points_seq;
	}
	else 
	{
		cameraParams2.imageSize = image_size;
		cameraParams2.cameraMatrix = cameraMatrix;
		cameraParams2.distortionCoefficients = distCoeffs;
		cameraParams2.rotations = rvecsMat;
		cameraParams2.translations = tvecsMat;
		cornersDatas.imagePoints2 = image_points_seq;
	}

	return ;
}

void StereoCircleCalibration(CString LCalibdataPath,CString RCalibdataPath,CString SCaliFile)
{
	//1、将单相机标定参数cameraParams1、cameraParams2保存至.yml文件中
	CString ymlFile1,ymlFile2;
	ymlFile1.Format(_T("%s\\data\\cameraParams1.yml"),ProjectPath);
	ymlFile2.Format(_T("%s\\data\\cameraParams2.yml"),ProjectPath);
	stereoCalib.saveCameraData(cameraParams1, (LPCTSTR)ymlFile1);
	stereoCalib.saveCameraData(cameraParams2, (LPCTSTR)ymlFile2);

	//2、开始双目标定
	stereoParams.cameraParams1 = cameraParams1;
	stereoParams.cameraParams2 = cameraParams2;
	stereoCalibrate(cornersDatas.objectPoints, cornersDatas.imagePoints1, 
		cornersDatas.imagePoints2,cameraParams1.cameraMatrix, 
		cameraParams1.distortionCoefficients, cameraParams2.cameraMatrix,
		cameraParams2.distortionCoefficients, cornersDatas.imageSize, 
		stereoParams.rotation,stereoParams.translation, 
		stereoParams.essential, stereoParams.foundational,
		TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 1e-5), 
		CV_CALIB_FIX_INTRINSIC);

	//3、双目矫正
	stereoCalib.mstereoRectify(cameraParams1, cameraParams2,
		stereoParams, remapMatrixs);
	//图片矫正并显示
	ifstream fin11(LCalibdataPath);
	ifstream fin12(RCalibdataPath);
	string str11, str22;
	Mat img1r, img2r;
	CString name11,name22;
	int cnt = 0;
	while (getline(fin11, str11) && getline(fin12, str22))
	{
		cnt++;
		Mat image1 = imread(str11);
		Mat image2 = imread(str22);
		stereoCalib.remapImage(image1, image2, img1r, img2r, remapMatrixs);
		name11.Format(_T("%s\\Save_LImage\\Left_%d.jpg"),ProjectPath,cnt);
		name22.Format(_T("%s\\Save_RImage\\Right_%d.jpg"),ProjectPath,cnt);
		string Name11((LPCTSTR)name11);
		string Name22((LPCTSTR)name22);
		imwrite(Name11, img1r);
		imwrite(Name22, img2r);
	}
	//在校正后的图像画出极线
	string Name11((LPCTSTR)name11);
	string Name22((LPCTSTR)name22);
	Mat imgLr = imread(Name11);
	Mat imgRr = imread(Name22);
	Mat img(cornersDatas.imageSize.height*0.5, cornersDatas.imageSize.width, 
		CV_8UC3);//创建IMG，高度一样，宽度双倍
	Mat imgPart1 = img(Rect(0, 0, cornersDatas.imageSize.width*0.5, 
		cornersDatas.imageSize.height*0.5));//浅拷贝
	Mat imgPart2 = img(Rect(cornersDatas.imageSize.width*0.5, 0, 
		cornersDatas.imageSize.width*0.5, cornersDatas.imageSize.height*0.5));//浅拷贝
	resize(imgLr, imgPart1, imgPart1.size(), 0, 0, CV_INTER_AREA);
	resize(imgRr, imgPart2, imgPart2.size(), 0, 0, CV_INTER_AREA);//改变图像尺寸，调节0,0；
	for (int i = 0; i < img.rows; i += 16) //画横线
		line(img, Point(0, i), Point(img.cols, i), Scalar(0, 255, 0), 1, 8);
	namedWindow("校正后的图像", WINDOW_NORMAL);
	imshow("校正后的图像", img);

	//4、利用极线约束计算双目标定误差
	double err;//所有像素点对极线误差的平均值
	stereoCalib.getStereoCalibrateError(cornersDatas, stereoParams, err);
	CString error;
	error.Format(_T("像素点对极线误差的平均值为：%f"),err);
	AfxMessageBox(error);

	//5、保存双目标定数据
	//a)保存在txt文件中
	ofstream StereoCaliResult(SCaliFile);
	StereoCaliResult<<"左相机内参数矩阵：\n";	
	StereoCaliResult<<cameraParams1.cameraMatrix<<'\n';	
	StereoCaliResult<<"左相机畸变系数：\n";
	StereoCaliResult<<cameraParams1.distortionCoefficients<<'\n';
	StereoCaliResult<<"右相机内参数矩阵：\n";	
	StereoCaliResult<<cameraParams2.cameraMatrix<<'\n';	
	StereoCaliResult<<"右相机畸变系数：\n";
	StereoCaliResult<<cameraParams2.distortionCoefficients<<'\n';
	StereoCaliResult<<"左右相机旋转矩阵：\n";	
	StereoCaliResult<<stereoParams.rotation<<'\n';	
	StereoCaliResult<<"左右相机平移矩阵：\n";
	StereoCaliResult<<stereoParams.translation<<'\n';
	StereoCaliResult<<"本征矩阵：\n";
	StereoCaliResult<<stereoParams.essential<<'\n';
	StereoCaliResult<<"基础矩阵：\n";
	StereoCaliResult<<stereoParams.foundational<<'\n';

	//b)保存在yml文件中
	CString ymlFile;
	ymlFile.Format(_T("%s\\data\\CalibrationDatas.yml"),ProjectPath);
	stereoCalib.saveCalibrationDatas((LPCTSTR)ymlFile, cornersDatas, stereoParams, 
		remapMatrixs);
	
	//c)保存在xml文件中
	CString xmlFile;
	xmlFile.Format(_T("%s\\data\\CalibrationDatas.xml"),ProjectPath);
	stereoCalib.saveCalibrationDatas((LPCTSTR)xmlFile, cornersDatas, stereoParams, remapMatrixs);
}

//对特征圆进行排序
vector<Point2f> CircleSort(vector<Point2f> buff,vector<Point2f> Bigbuff)
{
	//识别五个大圆的位置
	int d, min_d,max_d,count=0,n;
	n = Bigbuff.size();
	vector<int> distance; //五个大圆之间的所有距离
	vector<Point2f> BigCircle; //排序后的大圆，参考标定板大圆序号
	for(int i=0;i<n;i++)
	{
		for (int j=0;j<n,j!=i;j++)
		{
			d = sqrt((Bigbuff[i].x-Bigbuff[j].x)*(Bigbuff[i].x-Bigbuff[j].x)
				+(Bigbuff[i].y-Bigbuff[j].y)*(Bigbuff[i].y-Bigbuff[j].y));
			distance.push_back(d);
			count++;
		}
	}
	sort(distance.begin(),distance.end());
	max_d = distance[count-1];
	min_d = distance[0];
	int aa,bb,cc,dd,ee;
	for(int i=0;i<n;i++)
	{
		for (int j=0;j<n,j!=i;j++)
		{
			d = CalDistance(Bigbuff,i,j);
			if(d == max_d) //距离最大的两大圆
			{
				aa=i;
				bb=j;
			}
			if(d == min_d)//距离最小的两大圆
			{
				cc=i;
				dd=j;
			}
		}
	}
	for (int i=0;i<5;i++)
	{
		if(i!=aa&&i!=bb&&i!=cc&&i!=dd)
		{
			ee=i; //剩下的大圆
		}
	}
	//Bigbuff[ee]; //b4圆心坐标
	//分辨b1、b2圆
	int judge;
	if(CalDistance(Bigbuff,ee,cc)>CalDistance(Bigbuff,ee,dd))
	{
		BigCircle.push_back(Bigbuff[dd]);//b1圆
		BigCircle.push_back(Bigbuff[cc]);//b2圆
		judge = cc;//以b2圆做参考，分辨b3,b5圆
	}
	else
	{
		BigCircle.push_back(Bigbuff[cc]);//b1圆
		BigCircle.push_back(Bigbuff[dd]);//b2圆
		judge = dd;//以b2圆做参考，分辨b3,b5圆
	}
	//分辨b3、b5圆
	if(CalDistance(Bigbuff,judge,aa)>CalDistance(Bigbuff,judge,bb))
	{
		BigCircle.push_back(Bigbuff[bb]);//b3圆
		BigCircle.push_back(Bigbuff[ee]);//b4圆
		BigCircle.push_back(Bigbuff[aa]);//b5圆
	}
	else
	{
		BigCircle.push_back(Bigbuff[aa]);//b3圆
		BigCircle.push_back(Bigbuff[ee]);//b4圆
		BigCircle.push_back(Bigbuff[bb]);//b5圆
	}
	//大圆直线拟合
	std::vector<cv::Point> Lpoint1,Lpoint2;//cv::point里面存储的是整形数据，可能会影响精度
	cv::Vec4f Line1,Line2;
	Lpoint1.push_back (BigCircle[0]);//数组从0开始：0代表b1圆，……，4代表b5圆
	Lpoint1.push_back (BigCircle[3]);
	fitLine(Lpoint1,Line1,CV_DIST_L2,0,0.01,0.01);//对大圆1，4直线拟合,作为纵向参考线
	Lpoint2.push_back (BigCircle[2]);
	Lpoint2.push_back (BigCircle[4]);
	fitLine(Lpoint2,Line2,CV_DIST_L2,0,0.01,0.01);//对大圆3，5直线拟合,作为横向参考线
	
	//确定每个点的位置并进行排序
	vector<Point2f> NewPointSeq;//可能要放在参数列表里进行输出
	vector<int> distance14,distance35; 
	int d1,d2,an,distance14_2,distance35_2;
	an = buff.size();
	distance14_2 = 
		(Line1[1]*BigCircle[1].x-Line1[0]*BigCircle[1].y+Line1[0]*Line1[3]-Line1[1]*Line1[2])
		/sqrt(Line1[1]*Line1[1]+Line1[0]*Line1[0]); //参考点2到1、4连线距离，确定左右
	distance35_2 = 
		(Line2[1]*BigCircle[1].x-Line2[0]*BigCircle[1].y+Line2[0]*Line2[3]-Line2[1]*Line2[2])
		/sqrt(Line2[1]*Line2[1]+Line2[0]*Line2[0]); //参考点2到3、5连线距离，确定上下
	
	for(int i=0;i<an;i++)//所有点到直线1，4的距离，保留正负
	{
		d1 = 
			(Line1[1]*buff[i].x-Line1[0]*buff[i].y+Line1[0]*Line1[3]-Line1[1]*Line1[2])
			/sqrt(Line1[1]*Line1[1]+Line1[0]*Line1[0]); 
		distance14.push_back(d1);
	}

	for(int j=0;j<an;j++)//所有点到直线3，5的距离，保留正负
	{
		d2 = 
			(Line2[1]*buff[j].x-Line2[0]*buff[j].y+Line2[0]*Line2[3]-Line2[1]*Line2[2])
			/sqrt(Line2[1]*Line2[1]+Line2[0]*Line2[0]); 
		distance35.push_back(d2);
	}

	if (distance14_2 >0)
	{
		sort(distance14.begin(),distance14.end());//升序排列
	}
	else
	{
		sort(distance14.rbegin(),distance14.rend());//倒序排列
	}
	if (distance35_2 >0)
	{
		sort(distance35.begin(),distance35.end());//升序排列
	}
	else
	{
		sort(distance35.rbegin(),distance35.rend());//倒序排列
	}

	for(int i=0;i<an;i++)
	{
		int distance14_i,distance35_i,row,column,NewPosition;
		distance14_i = 
		(Line1[1]*buff[i].x-Line1[0]*buff[i].y+Line1[0]*Line1[3]-Line1[1]*Line1[2])
		/sqrt(Line1[1]*Line1[1]+Line1[0]*Line1[0]); //参考点i到1、4连线距离，确定列数
		vector<int>::iterator result1 = 
			find(distance14.begin(),distance14.end(),distance14_i);
		if (result1!=distance14.end())//该语句代表distance14中找到了distance14_i
		{ 
			column = (result1 - distance14.begin())/9+1; //确定列数
		}
		distance35_i = 
		(Line2[1]*buff[i].x-Line2[0]*buff[i].y+Line2[0]*Line2[3]-Line2[1]*Line2[2])
			/sqrt(Line2[1]*Line2[1]+Line2[0]*Line2[0]);  //参考点i到3、5连线距离，确定列数
		vector<int>::iterator result2 = 
			find(distance35.begin(),distance35.end(),distance35_i);
		if (result2!=distance35.end())//该语句代表distance35中找到了distance35_i
		{ 
			row = (result2 - distance35.begin())/11+1; //确定行数,iterator - vector.begin()
		}											   //返回iterator指向的位置
		NewPosition = (row-1)*11+column;
		NewPointSeq.resize(an); //将NewPointSeq的大小设置为an
		NewPointSeq[NewPosition-1] = buff[i];//对相应的每个位置进行赋值
	}
	return NewPointSeq;
}

int CalDistance(vector<Point2f> buff,int a,int b)
{
	int dis;
	dis = sqrt((buff[a].x-buff[b].x)*(buff[a].x-buff[b].x)
		+(buff[a].y-buff[b].y)*(buff[a].y-buff[b].y));
	return dis;
}

void Blob_ReleaseBlobSeq()
{
	if (!BlobSeq.IsEmpty())
	{
		BlobSeq.RemoveAll();
		BlobSeq.FreeExtra();
	}
}

void Blob_DenoisingArea(int MinArea, int MaxArea)
{
	if (!BlobSeq.IsEmpty())
	{
		int ln = BlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = BlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				BlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		BlobSeq.FreeExtra();
	}
}

void Detect_CircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity)
{
	Mat EdgeImg;
	Canny(SrcImg, EdgeImg, lowthresh, highthresh);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(EdgeImg, contours, hierarchy, CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE, cvPoint(0,0));
	
	//画出检测到的图形轮廓
	//Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
	//drawContours ( EdgeImg, contours,-1,color);
	//namedWindow("检测轮廓",WINDOW_NORMAL);
	//imshow("检测轮廓",EdgeImg);
	//waitKey(500);
	//EdgeImg.release();

	double area, length, cir, width, height, asp;
	RotatedRect rect;
	int cnt = 0;
	int ln = contours.size();
	BlobSeq.SetSize(ln,10);
	for (int i=0; i<ln; i++)
	{
		if (contours[i].size() > 10)
		{
			area = contourArea(contours[i]);		//第i个轮廓的面积
			length = arcLength(contours[i],true);	//第i个轮廓的周长
			cir = (12.56*area)/(length*length);		//第i个轮廓的圆形度
			if (cir > Circularity)
			{
				rect = fitEllipse(contours[i]);    
				width = rect.size.width;	//外接矩形的宽度
				height = rect.size.height;	//外接矩形的高度
				asp = height/width;			//纵横比
				if (asp>AspectRatio && asp<(1.0/AspectRatio))
				{
					Blob pB;
					pB.BlobWidth = width;
					pB.BlobHeight = height;
					pB.Area = area;
					pB.Perimeter = length;
					pB.AspectRatio = asp;
					pB.Circularity = cir;
					pB.BlobX = rect.center.x;
					pB.BlobY = rect.center.y;

					BlobSeq.SetAtGrow(cnt,pB);
					cnt++;
				}
			}
		}
	}
	BlobSeq.FreeExtra();
}
