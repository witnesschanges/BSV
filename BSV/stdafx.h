
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#define Chinesse
//相机参数设置对话框
#ifdef    Chinesse
//相机参数设置对话框
#define   SCP_Static_Shut_STR                   "曝光"
#define   SCP_Static_Gain_STR                   "增益"
#define   SCP_Button_HightSet_STR               "高级设置"
#define   SCP_Button_Reset_STR                  "重置"
#define   SCP_Button_OK_STR                     "确定"
#define   SCP_Radio_Continue_STR                "连续模式"
#define   SCP_Radio_Trigger_STR                 "触发模式"
//结束相机参数设置对话框

//主对话框
#define   BMS_Title_LeftCamParam_STR             "左相机参数"
#define   BMS_Title_RightCamParam_STR            "右相机参数"
////左相机标定完成按钮事件
#define   	STR_LCalibFinishText_1    "左相机标定结果:"
#define   	STR_LCalibFinishText_2    "畸变系数:"
#define   	STR_LCalibFinishText_3    "左相机内参矩阵A:"
#define   	STR_LCalibFinishText_4    "左相机评估结果:"
#define   	STR_LCalibFinishList_1    "序号|残差"
#define   	STR_LCalibFinishList_2    "左图片"
////右相机标定完成按钮事件
#define   	STR_RCalibFinishText_1    "右相机标定结果:"
#define   	STR_RCalibFinishText_2    "畸变系数:"
#define   	STR_RCalibFinishText_3    "右相机内参矩阵A:"
#define   	STR_RCalibFinishText_4    "右相机评估结果:"
#define   	STR_RCalibFinishList_1    "序号|残差"
#define   	STR_RCalibFinishList_2    "右图片"
////双相机标定完成按钮事件	
#define   	STR_DCalibFinishText_1    "左像机标定结果:                   右像机标定结果:                   双目标定结果:\n"
#define   	STR_DCalibFinishText_2    "畸变系数:                               畸变系数:                               平移向量T:\n"
#define   	STR_DCalibFinishText_3    "左像机内参矩阵A:                 右像机内参矩阵A:                 旋转矩阵R:\n"
#define   	STR_DCalibFinishText_4    "左像机评估结果:"
#define   	STR_DCalibFinishText_5    "\n右像机评估结果:"
#define   	STR_DCalibFinishText_6    "\n\n基础矩阵:\n"
#define   	STR_DCalibFinishList_1    "序号|残差"
#define   	STR_DCalibFinishList_2    "左图片"
#define   	STR_DCalibFinishList_3    "右图片"

#define   	STR_CalibMeasureMessageBox       "正在进行标定或测量,请按停止后在进行重新标定","提示"
#define   	STR_CalibFailText         "标定失败"
#define     STR_LUnUse                "左相机不可用","提示"
#define     STR_LNoOpen               "左相机没有工作","提示"
#define     STR_RUnUse                "右相机不可用","提示"
#define     STR_RNoOpen               "右相机没有工作","提示"
#define     STR_CamISOpen            "相机已打开"
#define     STR_CamISClose            "相机已关闭"

extern    CString  g_strMeasure[1] ;//测量提示文本
extern    CString  g_strCalibLeft[5] ;
extern    CString  g_strCalibRight[5]  ;
extern    CString  g_strCalibDouble[5] ;
extern    CString  g_strCalibFinishLeft   ; 
extern    CString  g_strCalibFinishRight  ; //结束主对话框
extern    CString  g_strCalibFinishDouble ;
//当前标定提示
extern    CString  g_strCurCalibTishi[10];
//标定评估结果显示
extern    CString g_strEvaluate[5];
#else

#endif

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


