#include "stdafx.h"
#include "CircleCalibration.h"

using namespace std;
using namespace cv;

CArray<Blob> LeftBlobSeq;

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
	Size board_size=Size(row_corner_num,column_corner_num);/* 标定板上每行、列的圆点数 */
	vector<Point2f> image_points_buf;  /* 缓存每幅图像上检测到的圆心 */
	vector<Point2f> image_bigpoints_buf; /* 缓存每幅图像上检测到的大圆圆心 */
	vector<vector<Point2f>> image_points_seq; /* 保存检测到的所有圆心 */
	vector<vector<Point2f>> image_bigpoints_seq; /* 保存检测到的所有大圆圆心 */
	while (getline(fin,filename))
	{
		Mat imageInput=imread(filename);
		if (image_count == 0)  /* 以读入的第一张图片宽高代表图片组的宽高*/
			{
				image_size.width = imageInput.cols;
				image_size.height =imageInput.rows;			
				cout<<"图片宽度为："<<image_size.width<<endl;
				cout<<"图片高度为："<<image_size.height<<endl;
			}
		
		//圆形检测
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
		//固定阈值分割图像 lq:二值化图像
		threshold(LFilImg, LBinImg, 100, 255, CV_THRESH_BINARY);
			
		Blob_ReleaseLeftBlobSeq();
		Detect_LeftCircleDetect(LBinImg, 0, 255, 0.8, 0.8);
		Blob_DenoisingLeftArea(600, 8000);
		int ln = LeftBlobSeq.GetSize();
		for (int i=0;i<ln;i+=1)
		{
			Blob pB = LeftBlobSeq.GetAt(i);
			Point2f center;
			center.x = pB.BlobX;
			center.y = pB.BlobY;
			image_points_buf.push_back(center);
		}

		if (image_points_buf.size()!=99)
		{
			cout<<"第%d张图片提取标定点数量不匹配！"<<image_count<<endl;
			image_count++;	
			drawChessboardCorners(LBinImg,board_size,image_points_buf,false); 
			namedWindow("相机标定",WINDOW_NORMAL);
			imshow("相机标定",LBinImg);//显示图片
			waitKey(500);
			image_points_buf.clear();
		}
		else
		{
			//用于在图片中标记圆心
			cout<<"第%d张图片提取标定点成功！"<<image_count<<endl;
			image_count++;
			success_count++;
			drawChessboardCorners(LBinImg,board_size,image_points_buf,true); 
			namedWindow("相机标定",WINDOW_NORMAL);
			imshow("相机标定",LBinImg);//显示图片
			waitKey(500);
			/*对圆点进行排序后再赋值*/
			image_points_seq.push_back(image_points_buf);
			image_points_buf.clear();
		}

		//筛选出大圆圆心
		Blob_DenoisingLeftArea(1500, 8000);
		int bn = LeftBlobSeq.GetSize();
		for (int i=0;i<bn;i+=1)
		{
			Blob pB = LeftBlobSeq.GetAt(i);
			Point2f center;
			center.x = pB.BlobX;
			center.y = pB.BlobY;
			image_bigpoints_buf.push_back(center);	
		}
		CircleSort(image_bigpoints_buf);
		image_bigpoints_seq.push_back(image_bigpoints_buf);
		image_bigpoints_buf.clear();
	}
	if(success_count<image_count)
	{
		AfxMessageBox(_T("图片中圆点识别不完整，程序强制退出！"));
		exit(1);//程序退出
	}
}

//对特征圆进行排序
void CircleSort(vector<Point2f> buff)
{
	int d, min_d,max_d,count=0,n;
	n = buff.size();
	vector<int> distance; //五个大圆之间的所有距离
	for(int i=0;i<n;i++)
	{
		for (int j=0;j<n,j!=i;j++)
		{
			d = sqrt((buff[i].x-buff[j].x)*(buff[i].x-buff[j].x)
				+(buff[i].y-buff[j].y)*(buff[i].y-buff[j].y));
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
			d = CalDistance(buff,i,j);
			//d = sqrt((buff[i].x-buff[j].x)*(buff[i].x-buff[j].x)
			//	+(buff[i].y-buff[j].y)*(buff[i].y-buff[j].y));
			if(d == max_d)
			{
				aa=i;
				bb=j;
			}
			if(d == min_d)
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
			ee=i;
		}
	}
	buff[ee]; //b4圆
	//分辨b1、b2圆
	int judge;
	if(CalDistance(buff,ee,cc)>CalDistance(buff,ee,dd))
	{
		buff[ee];
		judge = ee;
	}
	else
	{
		buff[dd];
		judge = dd;
	}
	//分辨b3、b5圆
	if(CalDistance(buff,judge,aa)>CalDistance(buff,judge,bb))
	{
		buff[aa];
	}
	else
	{
		buff[bb];
	}
}

int CalDistance(vector<Point2f> buff,int a,int b)
{
	int dis;
	dis = sqrt((buff[a].x-buff[b].x)*(buff[a].x-buff[b].x)
		+(buff[a].y-buff[b].y)*(buff[a].y-buff[b].y));
	return dis;
}
//
////3、设置圆心世界坐标系
//	/*棋盘三维信息*/
//	Size square_size = Size(grid_width,grid_height);  /* 实际测量得到的标定板上圆心之间的距离*/
//	vector<vector<Point3f>> object_points; /* 保存标定板上理论的三维坐标 */
//	int i,j,t;
//	for (t=0;t<image_count;t++) 
//	{
//		vector<Point3f> tempPointSet;
//		for (i=0;i<board_size.height;i++) 
//		{
//			for (j=0;j<board_size.width;j++) 
//			{
//				Point3f realPoint;
//				/* 假设标定板放在世界坐标系中z=0的平面上 */
//				realPoint.x = i*square_size.width;
//				realPoint.y = j*square_size.height;
//				realPoint.z = 0;
//				tempPointSet.push_back(realPoint);
//			}
//		}
//		object_points.push_back(tempPointSet);
//	}
//
////4、最后对摄像机标定
//	cout<<"\n***开始标定***"<<endl;
//	/*内外参数*/
//	Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* 摄像机内参数矩阵 */
//	vector<int> point_counts;  /* 每幅图像中圆点的数量*/
//	Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
//	vector<Mat> tvecsMat;  /* 每幅图像的旋转向量 */
//	vector<Mat> rvecsMat; /* 每幅图像的平移向量 */
//	/* 初始化标定板上角点的三维坐标 */
//	for (i=0;i<image_count;i++)
//	{
//		point_counts.push_back(board_size.width*board_size.height);
//	}
//	/* 开始标定 */
//	calibrateCamera(object_points,image_points_seq,image_size,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
//	cout<<"标定完成！"<<endl;

void Blob_ReleaseLeftBlobSeq()
{
	if (!LeftBlobSeq.IsEmpty())
	{
		LeftBlobSeq.RemoveAll();
		LeftBlobSeq.FreeExtra();
	}
}

void Blob_DenoisingLeftArea(int MinArea, int MaxArea)
{
	if (!LeftBlobSeq.IsEmpty())
	{
		int ln = LeftBlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = LeftBlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				LeftBlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		LeftBlobSeq.FreeExtra();
	}
}

void Detect_LeftCircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity)
{
	Mat EdgeImg;
	Canny(SrcImg, EdgeImg, lowthresh, highthresh);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(EdgeImg, contours, hierarchy, CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE, cvPoint(0,0));
	
	//画出检测到的轮廓轮廓
	Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
	drawContours ( EdgeImg, contours,-1,color);
	namedWindow("检测轮廓",WINDOW_NORMAL);
	imshow("检测轮廓",EdgeImg);
	waitKey(500);
	EdgeImg.release();

	double area, length, cir, width, height, asp;
	RotatedRect rect;
	int cnt = 0;
	int ln = contours.size();
	LeftBlobSeq.SetSize(ln,10);
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

					LeftBlobSeq.SetAtGrow(cnt,pB);
					cnt++;
				}
			}
		}
	}
	LeftBlobSeq.FreeExtra();
}
