#pragma once

/**************************************************
***@author:cui_sh
***@brief:imperx相机引入
***@time:2019-06-14
***@version:
**************************************************/

#ifndef __IPXCAMERA_H__
#define __IPXCAMERA_H__
#include <Windows.h>  
#include <string>
#include <vector>
#include <list>
//Imperx Camera API Functions
#include "IpxCameraApi.h"
#include <thread>
typedef void(*Imperxcallback)(IpxCam::Buffer* buffer);
class IpxCamera
{
public:
	IpxCamera();
	~IpxCamera();

public:
	/**************************************************
	***@brief:获取设备列表
	**************************************************/
	std::string queryIpxCam();
	std::vector<IpxCam::Interface*> SelectDevice(IpxCam::System *aSystem);

	/**************************************************
	***@brief:打开设备
	**************************************************/
	bool openImperx(int ImperxID);

	/**************************************************
	***@brief:关闭设备
	**************************************************/
	bool closeImperx();

	/**************************************************
	***@brief:连接到指定设备
	**************************************************/

	IpxCam::Device *ConnectToU3VDevice(IpxCam::DeviceInfo *aDeviceInfo);

	/**************************************************
	***@brief:tre-相机已经打开
	***@brief:false-相机未打开
	**************************************************/
	bool imperx_status;
	IpxGenParam::Array *arrays;
	/**************************************************
	***@brief:图像回调标识
	**************************************************/
	bool startAcquisition;
private:
	/**************************************************
	***@brief:相机参数设置
	**************************************************/
	IpxCamErr imperxParamSet(std::string name, std::string value);
	/**************************************************
	***@brief:相机参数据获取
	**************************************************/
	IpxGenParam::Array *imperxParamGet();
public:
	/**************************************************
	***@brief:打开流
	**************************************************/

	IpxCam::Stream *IpxCamera::OpenStreamByIndx(IpxCam::Device *aDevice, uint32_t idx);

	void CreateDataStreamBuffers(IpxCam::Stream *aDataStream, std::list<IpxCam::Buffer*> *aBufferList);
	void FreeDataStreamBuffers(IpxCam::Stream *aDataStream, std::list<IpxCam::Buffer*> *aBufferList);

	/**************************************************
		***@brief:打开视频流
		**************************************************/
	void * m_instance;
	bool StartAcquisition(Imperxcallback m_callback);
	/**************************************************
	***@brief:关闭视频流
	**************************************************/
	bool StopAcquisition();
public:
	
	bool iniCameryParam();
	bool setIniParamValue(std::string param);
public:
	int64_t TESTFUNC();

	int64_t getCurrentFrameRate();
	int64_t getProgFrameTimeAbs();
	int64_t getCurrentTemperature();
	int64_t getExposureTimeRaw();
	int64_t getMaxExposure();
	int64_t getGainRaw();
	int64_t getTriggerActivation();
	int64_t getTriggerMode();

	bool setProgFrameTimeAbs(int64_t value);
	bool setExposureTimeRaw(int64_t value);
	bool setGainRaw(int64_t value);
	bool setTriggerMode(int64_t value);
	bool setTriggerActivation(int64_t value);
	static int AcquisitionThread(void *org);
	Imperxcallback _callback;
public:
	int ProgFrameTimeAbs;//帧频设置
	int ExposureTimeRaw;//积分时间设置
	//相机参数设置
	int BinningHorizontal;
	int BinningVertical;
	int Width;
	int Height;
	int ReverseX;
	int ReverseY;
	int PixelFormat;
	int TriggerMode;
	int TriggerActivation;
	int TriggerSource;
	int ExposureMode;
	int ProgFrameTimeEnable;
	int TriggerType;
public:
	IpxCam::Device *lDevice;
private:
	IpxCam::System * System;
	IpxCam::Stream *lStream;
	std::vector<IpxCam::Interface*> ifaceVector;
	std::list<IpxCam::Buffer*> lBufferList;
public:
	/** 同步互斥,临界区保护 */
	CRITICAL_SECTION   m_cs;//!<
	CRITICAL_SECTION   m_cs_stram;//!<
};

#endif 