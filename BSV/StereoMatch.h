#ifndef _STEREO_MATCH_H_
#define _STEREO_MATCH_H_

#pragma once

#include <vector>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;

class StereoMatch
{
public:
	StereoMatch(void);
	virtual ~StereoMatch(void);

	int StereoMatch::bmMatch(cv::Mat& frameLeft, cv::Mat& frameRight, cv::Mat& disparity, cv::Mat& imageLeft, cv::Mat& imageRight);
	int StereoMatch::sgbmMatch(cv::Mat& frameLeft, cv::Mat& frameRight, cv::Mat& disparity, cv::Mat& imageLeft, cv::Mat& imageRight);
	int StereoMatch::init(int imgWidth, int imgHeight, const char* xmlFilePath);
	int StereoMatch::loadCalibData(const char* xmlFilePath);
	int StereoMatch::getDisparityImage(cv::Mat& disparity, cv::Mat& disparityImage, bool isColor);
	int StereoMatch::getPointClouds(cv::Mat& disparity, cv::Mat& pointClouds);
	void StereoMatch::savePointClouds(cv::Mat& pointClouds, const char* filename);

	cv::StereoBM	m_BM;				// 立体匹配 BM 方法
	cv::StereoSGBM	m_SGBM;				// 立体匹配 SGBM 方法
	double			m_FL;				// 左摄像机校正后的焦距值

private:
	bool	m_Calib_Data_Loaded;		// 是否成功载入定标参数
	cv::Mat m_Calib_Mat_Q;				// Q 矩阵
	cv::Mat m_Calib_Mat_Remap_X_L;		// 左视图畸变校正像素坐标映射矩阵 X
	cv::Mat m_Calib_Mat_Remap_Y_L;		// 左视图畸变校正像素坐标映射矩阵 Y
	cv::Mat m_Calib_Mat_Remap_X_R;		// 右视图畸变校正像素坐标映射矩阵 X
	cv::Mat m_Calib_Mat_Remap_Y_R;		// 右视图畸变校正像素坐标映射矩阵 Y
	cv::Mat m_Calib_Mat_Mask_Roi;		// 左视图校正后的有效区域
	cv::Rect m_Calib_Roi_L;				// 左视图校正后的有效区域矩形
	cv::Rect m_Calib_Roi_R;				// 右视图校正后的有效区域矩形

	int m_frameWidth;					// 帧宽
	int m_frameHeight;					// 帧高
	int m_numberOfDisparies;			// 视差变化范围
};

#endif