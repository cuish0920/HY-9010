#include "UVCCamery.h"
#include <windows.h>
cv::VideoCapture UVCCamery::UVC;
static CvCapture* pCapture;
bool UVCCamery::frameAcquisition;
bool UVCCamery::waitStopAcquisition;
bool UVCCamery::openStatus;
/**************************************************
***@author:cui_sh
***@brief:构造函数
***@time:
***@version:
**************************************************/

UVCCamery::UVCCamery()
{
	
}


/**************************************************
***@author:cui_sh
***@brief:析构函数
***@time:
***@version:
**************************************************/

UVCCamery::~UVCCamery()
{
	
}


bool UVCCamery::OpenCamera(uint8_t uiIdx)
{
	if (!UVC.open(0))
	{
		return false;
	}
	if (!UVC.isOpened())    // 判断是否打开成功
	{
		return false;
	}
	openStatus = true;
	return true;
}
bool UVCCamery::CloseCamera() 
{
	if (!openStatus)
	{
		return false;
	}
	if (frameAcquisition)
	{
		waitStopAcquisition = true;
		frameAcquisition = false;
		while (waitStopAcquisition)
		{
			Sleep(200);
		}
		return true;
	}
	
	UVC.release();
	UVC.~VideoCapture();
	UVC = NULL;
	openStatus = false;
	return true;
}
void UVCCamery::StartAcquisition(UVCCallBack callback)
{
	frameAcquisition = true;
	cv::Mat frame;
	while (frameAcquisition)
	{
		UVC >> frame;
		if (!frame.empty())	// 判断是否为空
		{
			callback(frame);
		}
		cv::waitKey(5);
	}
	//openStatus = false;
	waitStopAcquisition = false;
}

bool UVCCamery::StopAcquisition()
{
	waitStopAcquisition = true;
	frameAcquisition = false;
	while (waitStopAcquisition)
	{
		Sleep(200);
	}
	return true;
}

bool UVCCamery::getCameryStatus()
{
	return openStatus;
}
bool UVCCamery::getAcquistionStatus()
{
	return frameAcquisition;
}