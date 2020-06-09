#pragma once

/**************************************************
***@author:cui_sh
***@brief:imperx相机引入
***@time:2019-06-14
***@version:
**************************************************/

#ifndef __MICHROME20__
#define __MICHROME20__


#include "opencv2\opencv.hpp"
typedef void(*UVCCallBack)(cv::Mat m_frame);
class UVCCamery
{
	public:
		UVCCamery();
		~UVCCamery();

	public:
		
		/**************************************************
		***@brief:打开相机
		**************************************************/
		static bool OpenCamera(uint8_t uiIdx);
		/**************************************************
		***@brief:关闭相机
		**************************************************/
		static bool CloseCamera();
		
		/**************************************************
		***@brief:获取图像
		**************************************************/
		static void StartAcquisition(UVCCallBack callback);
		/**************************************************
		***@brief:关闭视频流
		**************************************************/
		static bool StopAcquisition();
		/**************************************************
		***@brief:获取视频打开状态
		**************************************************/
		static bool getCameryStatus();
		static bool getAcquistionStatus();
	private:
		static cv::VideoCapture UVC;
		//static CvCapture* pCapture;
		
		static bool frameAcquisition;
		static bool waitStopAcquisition;
		static bool openStatus;
};

#endif 