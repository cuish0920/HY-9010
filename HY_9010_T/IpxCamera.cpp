#include "IpxCamera.h"
#include <iostream>

#include <algorithm>


//////////////////引入json///////////////////
#include "CJsonObject.h"
//////////////////引入json///////////////////
#include "LogUtils.h"
/**************************************************
***@author:cui_sh
***@brief:构造函数
***@time:
***@version:
**************************************************/

IpxCamera::IpxCamera() :lDevice(nullptr), arrays(nullptr), imperx_status(false),
startAcquisition(false)
{
	//lDevice = nullptr;
	//startAcquisition = false;
	InitializeCriticalSection(&m_cs);//
	
	InitializeCriticalSection(&m_cs_stram);
	//初使化相机的参数
	ProgFrameTimeAbs = 50;//帧频设置
	ExposureTimeRaw = 20;//积分时间设置
	BinningHorizontal = 4;
	BinningVertical = 4;
	Width = 480;
	Height = 270;
	ReverseX = 0;
	//imperx->ReverseY = this->ReverseY;
	PixelFormat = 17825797;
	TriggerMode = 0;
	TriggerActivation = 0;
	TriggerSource = 27;
	ExposureMode = 1;
	ProgFrameTimeEnable = 1;
	TriggerType = 1;

	//arrays = nullptr;
}


/**************************************************
***@author:cui_sh
***@brief:析构函数
***@time:
***@version:
**************************************************/

IpxCamera::~IpxCamera()
{
	
	DeleteCriticalSection(&m_cs_stram);
	DeleteCriticalSection(&m_cs);//
}

/**************************************************
***@author:cui_sh
***@brief:获取相机列表
***@time:
***@version:
**************************************************/

std::string IpxCamera::queryIpxCam()
{
	try {
		/** 进入临界段 m_cs  */
		EnterCriticalSection(&m_cs);
		System = IpxCam::IpxCam_GetSystem();
		std::vector<IpxCam::Interface*>ifaceVector = SelectDevice(System);
		neb::CJsonObject imperxList;
		IpxCam::Interface *iface = NULL;
		IpxCam::DeviceInfo *lDeviceInfo = NULL;
		imperxList.AddEmptySubArray("imperx");
		for (; !ifaceVector.empty();)
		{
			neb::CJsonObject imperxItem;

			iface = ifaceVector.back();

			imperxItem.Add("name", iface->GetFirstDeviceInfo()->GetModel());
			imperxItem.Add("value", iface->GetDescription());
			imperxItem.Add("id", ifaceVector.size() - 1);

			imperxList["imperx"].Add(imperxItem);
			ifaceVector.pop_back();
		}
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
		return imperxList.ToString();
	}
	catch (...) {

		LogUtils::error->error("至命错误-queryIpxCam");
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
	}

}

std::vector<IpxCam::Interface*> IpxCamera::SelectDevice(IpxCam::System *aSystem)
{
	try {
		/** 进入临界段 m_cs  */
		EnterCriticalSection(&m_cs);
		if (aSystem != nullptr)
		{
			IpxCam::Interface *iface = NULL;

			uint32_t idx = 0;

			ifaceVector.clear();
			auto list = aSystem->GetInterfaceList();
			for (auto iface = list->GetFirst(); iface; iface = list->GetNext())
			{
				if (iface->GetFirstDeviceInfo() != nullptr)
				{
					ifaceVector.push_back(iface);

					idx++;
				}


			}

			return ifaceVector;
		}
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
		return {};
	}
	catch (...) {
		LogUtils::error->error("至命错误-SelectDevice");
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
	}

}

/**************************************************
***@author:cui_sh
***@brief:打开相机
***@time:
***@version:
**************************************************/

bool IpxCamera::openImperx(int ImperxID)
{
	try {
		/** 进入临界段 m_cs  */
		EnterCriticalSection(&m_cs);
		if (ifaceVector.empty()) {
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}

		IpxCam::DeviceInfo *lDeviceInfo = ifaceVector.at(ImperxID)->GetFirstDeviceInfo();
		if (lDeviceInfo != nullptr)
		{
			lDevice = ConnectToU3VDevice(lDeviceInfo);
			if (lDevice == nullptr)
			{
				/** 离开临界段 m_cs */
				LeaveCriticalSection(&m_cs);
				return false;
			}


			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return true;
		}
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
		return false;
	}
	catch (...) {
		LogUtils::error->error("至命错误-openImperx");
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
	}

}

/**************************************************
***@author:cui_sh
***@brief:关闭设备
***@time:
***@version:
**************************************************/

bool IpxCamera::closeImperx()
{
	try {
		/**************************************************
		***@brief:检查是否打开视频流
		**************************************************/
		/** 进入临界段 m_cs  */
		EnterCriticalSection(&m_cs);
		if (startAcquisition)
		{
			//退出视频流
			this->StopAcquisition();
		}


		if (imperx_status)
		{
			imperx_status = false;
			//Disconnecting device
			lDevice->Release();
			lDevice = nullptr;
			arrays = nullptr;
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return true;
		}
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
		return false;
	}
	catch (...) {
		LogUtils::error->error("至命错误-closeImperx");
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
	}


}


/**************************************************
***@author:cui_sh
***@brief:连接到指定的设备
***@time:
***@version:
**************************************************/
IpxCam::Device *IpxCamera::ConnectToU3VDevice(IpxCam::DeviceInfo *aDeviceInfo)
{
	try {
		// Create the camera
		/** 进入临界段 m_cs  */
		EnterCriticalSection(&m_cs);
		IpxCam::Device *lDevice = IpxCam_CreateDevice(aDeviceInfo);
		if (lDevice == nullptr)
		{
			//printf("ConnectToU3VDevice：error\r\n");

			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return nullptr;
		}
		//printf("ConnectToU3VDevice：success\r\n");
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
		return lDevice;
	}
	catch (...) {
		LogUtils::error->error("至命错误-ConnectToU3VDevice");
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
	}

}

IpxGenParam::Array *IpxCamera::imperxParamGet()
{
	try {
		/** 进入临界段 m_cs  */
		//EnterCriticalSection(&m_cs);
		if (arrays == nullptr)
		{

			arrays = lDevice->GetCameraParameters();


		}
		/** 离开临界段 m_cs */
		//LeaveCriticalSection(&m_cs);
		return arrays;
	}
	catch (...) {
		LogUtils::error->error("至命错误-imperxParamGet");
		/** 离开临界段 m_cs */
		//LeaveCriticalSection(&m_cs);
	}


}

/*获取本地初使化参数*/
bool IpxCamera::setIniParamValue(std::string param)
{
	try {
		/** 进入临界段 m_cs  */
		EnterCriticalSection(&m_cs);
		double value;
		neb::CJsonObject sys_iniparam(param);

		if (!sys_iniparam.Get("ProgFrameTimeAbs", ProgFrameTimeAbs)) {
			sys_iniparam.Get("ProgFrameTimeAbs", value);
			ProgFrameTimeAbs = value;
		}
		//printf("system_ini=%d \r\n", system_ini);
		//sys_iniparam.Get("ExposureTimeRaw", ExposureTimeRaw);
		if (!sys_iniparam.Get("ExposureTimeRaw", ExposureTimeRaw)) {
			sys_iniparam.Get("ExposureTimeRaw", value);
			ExposureTimeRaw = value;
		}

		//printf(" ExposureTimeRaw = %d\r\n",ExposureTimeRaw);
		//printf("ExposureTimeRaw=%d \r\n", ExposureTimeRaw);
		//sys_iniparam.Get("BinningHorizontal", BinningHorizontal);
		if (!sys_iniparam.Get("BinningHorizontal", BinningHorizontal)) {
			sys_iniparam.Get("BinningHorizontal", value);
			BinningHorizontal = value;
		}
		//printf("BinningHorizontal=%d \r\n", BinningHorizontal);
		//sys_iniparam.Get("BinningVertical", BinningVertical);
		if (!sys_iniparam.Get("BinningVertical", BinningVertical)) {
			sys_iniparam.Get("BinningVertical", value);
			BinningVertical = value;
		}
		//printf("BinningVertical=%d \r\n", BinningVertical);
		sys_iniparam.Get("Width", Width);
		if (!sys_iniparam.Get("Width", Width)) {
			sys_iniparam.Get("Width", Width);
			Width = value;
		}

	
		if (!sys_iniparam.Get("Height", Height)) {
			sys_iniparam.Get("Height", Height);
			Height = value;
		}
		//sys_iniparam.Get("ReverseX", ReverseX);

		if (!sys_iniparam.Get("ReverseX", ReverseX)) {
			sys_iniparam.Get("ReverseX", ReverseX);
			ReverseX = value;
		}

		//sys_iniparam.Get("PixelFormat", PixelFormat);
		if (!sys_iniparam.Get("PixelFormat", PixelFormat)) {
			sys_iniparam.Get("PixelFormat", PixelFormat);
			PixelFormat = value;
		}
		//sys_iniparam.Get("TriggerMode", TriggerMode);
		if (!sys_iniparam.Get("TriggerMode", TriggerMode)) {
			sys_iniparam.Get("TriggerMode", TriggerMode);
			TriggerMode = value;
		}
		//sys_iniparam.Get("TriggerActivation", TriggerActivation);
		if (!sys_iniparam.Get("TriggerActivation", TriggerActivation)) {
			sys_iniparam.Get("TriggerActivation", TriggerActivation);
			TriggerActivation = value;
		}

		//sys_iniparam.Get("TriggerSource", TriggerSource);
		if (!sys_iniparam.Get("TriggerSource", TriggerSource)) {
			sys_iniparam.Get("TriggerSource", TriggerSource);
			TriggerSource = value;
		}

		//sys_iniparam.Get("ExposureMode", ExposureMode);
		if (!sys_iniparam.Get("ExposureMode", ExposureMode)) {
			sys_iniparam.Get("ExposureMode", ExposureMode);
			ExposureMode = value;
		}
		//sys_iniparam.Get("ProgFrameTimeEnable", ProgFrameTimeEnable);
		if (!sys_iniparam.Get("ProgFrameTimeEnable", ProgFrameTimeEnable)) {
			sys_iniparam.Get("ProgFrameTimeEnable", ProgFrameTimeEnable);
			ProgFrameTimeEnable = value;
		}
		//sys_iniparam.Get("TriggerType", TriggerType);
		if (!sys_iniparam.Get("TriggerType", TriggerType)) {
			sys_iniparam.Get("TriggerType", TriggerType);
			TriggerType = value;
		}
		//加上相元合并
		//sys_iniparam.Get("BinningHorizontal", BinningHorizontal);
		if (!sys_iniparam.Get("BinningHorizontal", BinningHorizontal)) {
			sys_iniparam.Get("BinningHorizontal", BinningHorizontal);
			BinningHorizontal = value;
		}
		Width = 1920 / BinningHorizontal;
		//sys_iniparam.Get("BinningVertical", BinningVertical);
		if (!sys_iniparam.Get("BinningVertical", BinningVertical)) {
			sys_iniparam.Get("BinningVertical", BinningVertical);
			BinningVertical = value;
		}
		//宽高计算
		Height = 1080 / BinningVertical;

		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
		return true;
	}
	catch (...) {
		LogUtils::error->error("至命错误-setIniParamValue");
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
	}

}
bool IpxCamera::iniCameryParam()
{
	try {
		/** 进入临界段 m_cs  */
		EnterCriticalSection(&m_cs);
		IpxCamErr err;
		arrays = imperxParamGet();

		if (arrays == nullptr)
		{
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}
		//为了使系统参数据能设置进去
		err = imperxParamSet("ProgFrameTimeEnable", std::to_string(0));
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			printf("%s\r\n", "ProgFrameTimeEnable");
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}

		err = imperxParamSet("BinningHorizontal", std::to_string(BinningHorizontal));
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			printf("%s\r\n", "BinningHorizontal");
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}

		err = imperxParamSet("BinningVertical", std::to_string(BinningVertical));
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			printf("%s\r\n", "BinningVertical");
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}

		err = imperxParamSet("Width", std::to_string(Width));
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			printf("%s\r\n", "Width");
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}

		err = imperxParamSet("Height", std::to_string(Height));
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			printf("%s\r\n", "Height");
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}

		///*
		//"Mono8": 17301505 (0x01080001)
		//"Mono10": 17825795 (0x01100003)
		//"Mono10Packed": 17563652 (0x010c0004)
		//"Mono12": 17825797 (0x01100005)
		//"Mono12Packed": 17563654 (0x010c0006)
		//"Mono16": 17825799 (0x01100007)
		//"BayerGR8": 17301512 (0x01080008)
		//"BayerRG8": 17301513 (0x01080009)
		//"BayerGB8": 17301514 (0x0108000a)
		//"BayerBG8": 17301515 (0x0108000b)
		//"BayerGR10": 17825804 (0x0110000c)
		//"BayerRG10": 17825805 (0x0110000d)
		//"BayerGB10": 17825806 (0x0110000e)
		//"BayerBG10": 17825807 (0x0110000f)
		//"BayerGR12": 17825808 (0x01100010)
		//"BayerRG12": 17825809 (0x01100011)
		//"BayerGB12": 17825810 (0x01100012)
		//"BayerBG12": 17825811 (0x01100013)
		//"RGB8Packed": 35127316 (0x02180014)
		//*/
		//err = m_this->imperxParamSet("PixelFormat", std::to_string(II_PIX_MONO12));
		err = imperxParamSet("PixelFormat", std::to_string(PixelFormat));
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}

		//printf("TriggerMode = %d\r\n", TriggerMode);
		err = imperxParamSet("TriggerMode", std::to_string(TriggerMode));//0-off 1-on
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}
		err = imperxParamSet("TriggerActivation", std::to_string(TriggerActivation));//0-RisingEdge entries: 1-FallingEdge
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}

		err = imperxParamSet("TriggerSource", std::to_string(TriggerSource));//
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}
		err = imperxParamSet("ExposureMode", std::to_string(ExposureMode));
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}




		err = imperxParamSet("ReverseX", std::to_string(ReverseX));
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}
		/*err =imperxParamSet("ReverseY", std::to_string(m_this->ReverseY));
		if (err != ((IpxCamErr)0)) throw AcquireImages_error();*/

		err = imperxParamSet("TriggerType", std::to_string(TriggerType));
		if (err != IpxCamErr::IPX_CAM_ERR_OK) {
			/** 离开临界段 m_cs */
			LeaveCriticalSection(&m_cs);
			return false;
		}
		/**************************************************
		***@brief:用于设置帧频 ProgFrameTimeAbs= (1/帧频)*1000000
		**************************************************/
		if (TriggerMode == 0)
		{
			//取得当前帧
			int currentFps = 80;//getCurrentFrameRate();
			(currentFps > ProgFrameTimeAbs) ? ProgFrameTimeAbs = ProgFrameTimeAbs : ProgFrameTimeAbs = currentFps;
			//LogUtils::debugger->debug("ProgFrameTimeAbs=%d %s", ProgFrameTimeAbs, std::to_string((1.0 / ProgFrameTimeAbs * 1000000)).c_str());
			err = imperxParamSet("ProgFrameTimeAbs", std::to_string((1.0 / ProgFrameTimeAbs * 1000000)));
			if (err != ((IpxCamErr)0)) {
				/** 离开临界段 m_cs */
				LeaveCriticalSection(&m_cs);
				return false;
			}
			err = imperxParamSet("ProgFrameTimeEnable", std::to_string(ProgFrameTimeEnable));
			if (err != ((IpxCamErr)0)) {
				/** 离开临界段 m_cs */
				LeaveCriticalSection(&m_cs);
				return false;
			}

			Sleep(200);
			int maxExposure = getMaxExposure();
			//LogUtils::debugger->debug("maxExposure=%d ExposureTimeRaw=%d", maxExposure,ExposureTimeRaw); 
			(maxExposure > ExposureTimeRaw) ? ExposureTimeRaw = ExposureTimeRaw : ExposureTimeRaw = maxExposure;
			//LogUtils::debugger->debug("maxExposure=%d ExposureTimeRaw=%d", maxExposure, ExposureTimeRaw);

			err = imperxParamSet("ExposureTimeRaw", std::to_string(ExposureTimeRaw));
			if (err != ((IpxCamErr)0)) {
				/** 离开临界段 m_cs */
				LeaveCriticalSection(&m_cs);
				return false;
			}

			

		}

		imperx_status = true;
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
		return imperx_status;//打开设备成功
	}
	catch (...) {
		LogUtils::error->error("至命错误-iniCameryParam");
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&m_cs);
	}

}

IpxCamErr IpxCamera::imperxParamSet(std::string name, std::string value)
{
	try {
		/** 进入临界段 m_cs  */
		//EnterCriticalSection(&m_cs);
		//参数据设置
		IpxCamErr err;
		//IpxGenParam::Array *arrays = lDevice->GetCameraParameters(&err);
		if (arrays == NULL)
		{

			/** 离开临界段 m_cs */
			//LeaveCriticalSection(&m_cs);
			return IPX_CAM_ERR_UNKNOWN;
		}
		IpxGenParam::Param *m_param = arrays->GetParam(name.c_str(), &err);

		IpxGenParam::ParamType type = m_param->GetType();

		//std::cout << m_param->GetName() << "		" << value << std::endl;

		switch (type)
		{
		case IpxGenParam::ParamType::ParamInt:

			/** 离开临界段 m_cs */
			//LeaveCriticalSection(&m_cs);
			return arrays->SetIntegerValue(m_param->GetName(), atoll(value.c_str()));
			break;
		case IpxGenParam::ParamType::ParamFloat:
			/** 离开临界段 m_cs */
			//LeaveCriticalSection(&m_cs);
			return arrays->SetFloatValue(m_param->GetName(), atof(value.c_str()));
			break;
		case IpxGenParam::ParamType::ParamString:
			/** 离开临界段 m_cs */
			//LeaveCriticalSection(&m_cs);
			return arrays->SetStringValue(m_param->GetName(), value.c_str());
			break;
		case IpxGenParam::ParamType::ParamEnum:
			/** 离开临界段 m_cs */
			//LeaveCriticalSection(&m_cs);
			return arrays->SetEnumValue(m_param->GetName(), atoll(value.c_str()));
			//err = arrays->SetEnumValueStr(m_param->GetName(), "Timed");

			//return arrays->SetEnumValue("ExposureMode", 1);
			break;
			/*case IpxGenParam::ParamType::ParamEnumEntry:
				std::cout << "ParamEnumEntry ";
				break;*/
		case IpxGenParam::ParamType::ParamBoolean:
			//LeaveCriticalSection(&m_cs);
			return arrays->SetBooleanValue(m_param->GetName(), atoi(value.c_str()));
			break;
		case IpxGenParam::ParamType::ParamCommand:
			/** 离开临界段 m_cs */
			//LeaveCriticalSection(&m_cs);
			return arrays->ExecuteCommand(value.c_str());
			break;
			/*case IpxGenParam::ParamType::ParamCategory:
				std::cout << "ParamCategory ";
				break;*/
		case IpxGenParam::ParamType::ParamUnknown:
			/** 离开临界段 m_cs */
			//LeaveCriticalSection(&m_cs);
			return IPX_CAM_ERR_UNKNOWN;
			break;

		default:
			/** 离开临界段 m_cs */
			//LeaveCriticalSection(&m_cs);
			return IPX_CAM_ERR_UNKNOWN;
			break;
		}
		/** 离开临界段 m_cs */
		//LeaveCriticalSection(&m_cs);
		return IPX_CAM_ERR_UNKNOWN;
	}
	catch (...) {
		LogUtils::error->error("至命错误-imperxParamSet");
		/** 离开临界段 m_cs */
		//LeaveCriticalSection(&m_cs);
	}

}
int64_t IpxCamera::TESTFUNC()
{
	//EnterCriticalSection(&m_cs);
	printf("TESTFUNC\r\n");
	//LeaveCriticalSection(&m_cs);
	return 0;
}

int64_t IpxCamera::getCurrentFrameRate()
{
	try {
		//printf("getCurrentFrameRate 进入临界段\r\n");
		///** 进入临界段 m_cs  */
		//EnterCriticalSection(&m_cs);
		///** 离开临界段 m_cs */
		//LeaveCriticalSection(&m_cs);

		
		IpxCamErr err;
		IpxGenParam::Array *m_param = imperxParamGet();
		if (m_param == nullptr)
		{

			/** 离开临界段 m_cs */
			//LeaveCriticalSection(&m_cs);
			return 0;
		}

		/** 离开临界段 m_cs */
		//LeaveCriticalSection(&m_cs);
		return m_param->GetIntegerValue("CurrentFrameRate", &err);

	}
	catch (...) {
		LogUtils::error->error("至命错误-getCurrentFrameRate");
		printf("error 进入临界段\r\n");
		/** 离开临界段 m_cs */
		//LeaveCriticalSection(&m_cs);
	}

}
int64_t IpxCamera::getCurrentTemperature()
{
	try {

		/** 进入临界段 m_cs  */
		//EnterCriticalSection(&m_cs);
		IpxCamErr err;
		IpxGenParam::Array *m_param = imperxParamGet();
		if (m_param == nullptr)
		{
			/** 离开临界段 m_cs */

			return 0;
		}
		/** 离开临界段 m_cs */
		//LeaveCriticalSection(&m_cs);
		return m_param->GetIntegerValue("CurrentTemperature", &err);
	}
	catch (...) {
		LogUtils::error->error("至命错误-getCurrentTemperature");
		/** 离开临界段 m_cs */
		//LeaveCriticalSection(&m_cs);
	}

}
int64_t IpxCamera::getProgFrameTimeAbs()
{
	try {
		/** 进入临界段 m_cs  */

		IpxCamErr err;
		IpxGenParam::Array *m_param = imperxParamGet();
		if (m_param == nullptr)
		{
			/** 离开临界段 m_cs */

			return 0;
		}
		/** 离开临界段 m_cs */

		return m_param->GetIntegerValue("ProgFrameTimeAbs", &err);
	}
	catch (...) {
		LogUtils::error->error("至命错误-getProgFrameTimeAbs");
		/** 离开临界段 m_cs */

	}

}

int64_t IpxCamera::getExposureTimeRaw()
{
	try {
		/** 进入临界段 m_cs  */

		IpxCamErr err;
		IpxGenParam::Array *m_param = imperxParamGet();
		if (m_param == nullptr)
		{
			/** 离开临界段 m_cs */

			return 0;
		}
		/** 离开临界段 m_cs */

		return m_param->GetIntegerValue("ExposureTimeRaw", &err);
	}
	catch (...) {
		LogUtils::error->error("至命错误-getExposureTimeRaw");
		/** 离开临界段 m_cs */

	}

}

int64_t IpxCamera::getMaxExposure()
{
	try {
		/** 进入临界段 m_cs  */

		IpxCamErr err;
		IpxGenParam::Array *m_param = imperxParamGet();
		if (m_param == nullptr)
		{
			/** 离开临界段 m_cs */

			return 0;
		}
		/** 离开临界段 m_cs */

		return m_param->GetIntegerValue("MaxExposure", &err);
	}
	catch (...) {
		LogUtils::error->error("至命错误-getMaxExposure");
		/** 离开临界段 m_cs */

	}

}

int64_t IpxCamera::getGainRaw()
{
	try {
		/** 进入临界段 m_cs  */

		IpxCamErr err;
		IpxGenParam::Array *m_param = imperxParamGet();
		if (m_param == nullptr)
		{
			/** 进入临界段 m_cs  */

			return 0;
		}
		/*Limits: Min: 0, Max: 1023*/
		/** 进入临界段 m_cs  */

		return m_param->GetIntegerValue("GainRaw", &err);
	}
	catch (...) {
		LogUtils::error->error("至命错误-getGainRaw");
		/** 进入临界段 m_cs  */

	}

}
int64_t IpxCamera::getTriggerActivation()
{
	try {
		/** 进入临界段 m_cs  */

		IpxCamErr err;
		IpxGenParam::Array *m_param = imperxParamGet();
		if (m_param == nullptr)
		{
			/** 离开临界段 m_cs */

			return 0;
		}
		//"RisingEdge": 0 (0x00000000)
		//"FallingEdge" : 1 (0x00000001)
		/** 离开临界段 m_cs */

		return m_param->GetIntegerValue("TriggerActivation", &err);
	}
	catch (...) {
		LogUtils::error->error("至命错误-getTriggerActivation");
		/** 离开临界段 m_cs */

	}

}

int64_t IpxCamera::getTriggerMode()
{
	try {
		/** 进入临界段 m_cs  */

		IpxCamErr err;
		IpxGenParam::Array *m_param = imperxParamGet();
		//printf("");
		if (m_param == nullptr)
		{
			printf("m_param == nullptr\r\n");

			/** 离开临界段 m_cs */

			return 0;
		}
		//"Off": 0 (0x00000000) "On": 1 (0x00000001)
		//"Off": 0 (0x00000000) "On": 1 (0x00000001)

		/** 离开临界段 m_cs */

		return m_param->GetEnumValue("TriggerMode", &err);
		/*int64_t TriggerMode = m_param->GetEnumValue("TriggerMode", &err);
		if (err != IpxCamErr::IPX_CAM_ERR_OK)
		{
			printf("IPX_CAM_ERR_OK == nullptr %d\r\n", err);
			return 0;
		}
		else
		{
			printf("TriggerMode = %d\r\n", TriggerMode);
			return TriggerMode;
		}*/
	}
	catch (...) {
		/** 离开临界段 m_cs */

	}


}

IpxCam::Stream *IpxCamera::OpenStreamByIndx(IpxCam::Device *aDevice, uint32_t idx)
{
	try {
		/** 进入临界段 m_cs  */

		IpxCam::Stream *lStream = aDevice->GetStreamByIndex(idx);
		if (lStream == NULL)
		{
			/** 离开临界段 m_cs */

			return nullptr;
		}
		/** 离开临界段 m_cs */

		return lStream;
	}
	catch (...) {
		/** 离开临界段 m_cs */

	}


}

void IpxCamera::CreateDataStreamBuffers(IpxCam::Stream *aDataStream, std::list<IpxCam::Buffer*> *aBufferList)
{
	try {
		/** 进入临界段 m_cs  */

		//Get the Payload Size
		auto bufSize = aDataStream->GetBufferSize();

		auto minNumBuffers = aDataStream->GetMinNumBuffers();

		for (size_t i = 0; i < minNumBuffers + 1; ++i) {
			// Create new buffer object
			IpxCam::Buffer* hBuffer;
			IpxCamErr err;
			//Allocate the payload memory of the new buffer object
			hBuffer = aDataStream->CreateBuffer(bufSize, nullptr, &err);

			aBufferList->push_back(hBuffer);
		}
		/** 离开临界段 m_cs */

	}
	catch (...) {
		/** 离开临界段 m_cs */

	}

}
void IpxCamera::FreeDataStreamBuffers(IpxCam::Stream *aDataStream, std::list<IpxCam::Buffer*> *aBufferList)
{
	try {
		/** 进入临界段 m_cs  */

		// Go through the buffer list
		std::for_each(std::begin(*aBufferList), std::end(*aBufferList),
			[&aDataStream](IpxCam::Buffer* buffer) {
			aDataStream->RevokeBuffer(buffer);
		});
		//Clear the buffer List
		aBufferList->clear();

		/** 离开临界段 m_cs */

	}
	catch (...) {

		/** 离开临界段 m_cs */

	}

}

int IpxCamera::AcquisitionThread(void *org) {

	IpxCamera *obj = reinterpret_cast<IpxCamera*>(org);
	IpxCamErr err;
	while (obj->startAcquisition)
	{
		/** 进入临界段 m_cs  */
		EnterCriticalSection(&obj->m_cs_stram);
		//IpxCam::Buffer* buffer = lStream->GetBuffer(UINT64_MAX, &err);
		IpxCam::Buffer* buffer = obj->lStream->GetBuffer(2000, &err);

		if ((IPX_CAM_ERR_OK == err) && buffer && buffer->GetHeight() && buffer->GetWidth())
		{
			//回调帧数据
			obj->_callback(buffer);
			//printf("index = %d\r\n", index);
			obj->lStream->QueueBuffer(buffer);
			//index++;
		}
		else if (GC_ERR_TIMEOUT == err)
		{
			if (!obj->startAcquisition) {
				//printf("error%d\r\n", err);
				/** 离开临界段 m_cs */
				LeaveCriticalSection(&obj->m_cs_stram);
				break;
			}
			//index_error++;
			//printf("--------------------------------------index_error = %d \r\n", index_error);
		}
		/** 离开临界段 m_cs */
		LeaveCriticalSection(&obj->m_cs_stram);
		//Sleep(1);
	}
	return 0;
}

/**************************************************
***@author:cui_sh
***@brief:打开视频流
***@time:2019-06-17
***@version:
**************************************************/

bool IpxCamera::StartAcquisition(Imperxcallback m_callback)
{
	try
	{

		if (startAcquisition) {

			return false;
		}

		if (lDevice->GetNumStreams() < 1) {

			return false;
		}
		lStream = OpenStreamByIndx(lDevice, 0);
		if (!lStream) {

			return false;
		}
		CreateDataStreamBuffers(lStream, &lBufferList);

		if (m_callback == nullptr) {

			return false;
		}
		_callback = m_callback;
		//开始输出图像
		IpxCamErr err;
		arrays = imperxParamGet();
		startAcquisition = true;
		if (arrays)
		{
			err = imperxParamSet("TLParamsLocked", "1");
			if (err != IpxCamErr::IPX_CAM_ERR_OK) {

				return false;
			}
			err = lStream->StartAcquisition();
			if (err != IpxCamErr::IPX_CAM_ERR_OK)
			{

				return false;
			}
			err = imperxParamSet("AcquisitionStart", "AcquisitionStart");
			if (err != IpxCamErr::IPX_CAM_ERR_OK)
			{

				return false;
			}
			int index = 0;
			int index_error = 0;
			//获取帧数据
			//printf("获取帧数据 \r\n");

			//std::thread t1(AcquisitionThread,this);
			//std::thread t2(AcquisitionThread, this);
			/*std::thread t3(AcquisitionThread, this);
			std::thread t4(AcquisitionThread, this);
			std::thread t5(AcquisitionThread, this)*/;


			//t1.join();
			//t2.join();
			/*t3.join();
			t4.join();
			t5.join();
*/

			while (startAcquisition)
			{
				//IpxCam::Buffer* buffer = lStream->GetBuffer(UINT64_MAX, &err);
				IpxCam::Buffer* buffer = lStream->GetBuffer(2000, &err);
				
				if ((IPX_CAM_ERR_OK == err) && buffer && buffer->GetHeight() && buffer->GetWidth())
				{
					//回调帧数据
					m_callback(buffer);
					//printf("index = %d\r\n", index);
					lStream->QueueBuffer(buffer);
					//index++;
				}
				else if(GC_ERR_TIMEOUT== err)
				{
					if (!startAcquisition) {
						//printf("error%d\r\n", err);
						break;
					}
					//index_error++;
					//printf("--------------------------------------index_error = %d \r\n", index_error);
				}

				//printf("m_callback\r\n");
				//Sleep(1);
			}





			//printf("AcquisitionStop m_callback\r\n");
			IpxCamErr err = arrays->ExecuteCommand("AcquisitionStop");
			if (err != IPX_CAM_ERR_OK)
			{


				arrays->ExecuteCommand("AcquisitionAbort");
			}
			lStream->StopAcquisition(1);
			arrays->SetIntegerValue("TLParamsLocked", 0);


			lStream->FlushBuffers(IpxCam::Flush_AllDiscard);
			FreeDataStreamBuffers(lStream, &lBufferList);
			lStream->Release();
		}

	}
	catch (...)
	{

	}


	//printf("结束\r\n");
	return false;
}

/**************************************************
***@author:关闭视频流
***@brief:
***@time:
***@version:
**************************************************/

bool IpxCamera::StopAcquisition()
{
	try {
		/** 进入临界段 m_cs  */

		startAcquisition = false;
		//printf("startAcquisition = false \r\n");
		/** 离开临界段 m_cs */

		return true;
	}
	catch (...) {
		/** 离开临界段 m_cs */

	}

}

/**************************************************
***@author:设置帧频
***@brief:
***@time:
***@version:
**************************************************/

bool IpxCamera::setProgFrameTimeAbs(int64_t value)
{
	try {
		/** 进入临界段 m_cs  */

		int currentFps = 80;//getCurrentFrameRate();
		(currentFps > value) ? value = value : value = currentFps;


		IpxCamErr err = imperxParamSet("ProgFrameTimeAbs", std::to_string((1.0 / value * 1000000)));
		if (err == IPX_CAM_ERR_OK)
		{
			return true;
		}
		/** 离开临界段 m_cs */

		return false;
	}
	catch (...) {
		/** 离开临界段 m_cs */

	}

}

/**************************************************
***@author:设置积分时间
***@brief:
***@time:
***@version:
**************************************************/
bool IpxCamera::setExposureTimeRaw(int64_t value)
{
	try {
		/** 进入临界段 m_cs  */

		int maxExposure = getMaxExposure();
		LogUtils::debugger->debug("maxExposure %d value=%d", maxExposure, value);
		(maxExposure > value) ? value = value : value = maxExposure;
		LogUtils::debugger->debug("maxExposurevalue=%d", value);
		IpxCamErr err = imperxParamSet("ExposureTimeRaw", std::to_string(value));
		if (err == IPX_CAM_ERR_OK)
		{

			/** 离开临界段 m_cs */

			return true;
		}

		/** 离开临界段 m_cs */

		return false;
	}
	catch (...) {

		/** 离开临界段 m_cs */

	}


}

/**************************************************
***@author:增益设置
***@brief:
***@time:
***@version:
**************************************************/
bool IpxCamera::setGainRaw(int64_t value)
{
	try {
		/** 进入临界段 m_cs  */

		/*Limits: Min: 0, Max: 1023*/
		int GainRaw = 1023;
		(GainRaw > value) ? value = value : value = GainRaw;
		IpxCamErr err = imperxParamSet("GainRaw", std::to_string(value));
		if (err == IPX_CAM_ERR_OK)
		{
			/** 离开临界段 m_cs */

			return true;
		}
		/** 离开临界段 m_cs */

		return false;
	}
	catch (...) {
		/** 离开临界段 m_cs */

	}

}


/**************************************************
***@author:打开半闭触发 0:off 1:on
***@brief:
***@time:
***@version:
**************************************************/
bool IpxCamera::setTriggerMode(int64_t value)
{
	try {
		/** 进入临界段 m_cs  */

		IpxCamErr err = imperxParamSet("TriggerMode", std::to_string(value));
		if (err == IPX_CAM_ERR_OK)
		{
			/** 离开临界段 m_cs */

			return true;
		}
		/** 离开临界段 m_cs */

		return false;
	}
	catch (...) {
		/** 离开临界段 m_cs */

	}

}

/**************************************************
***@author:设置触发沿
***@brief: "RisingEdge": 0 (0x00000000) "FallingEdge": 1 (0x00000001)
***@time:
***@version:
**************************************************/
bool IpxCamera::setTriggerActivation(int64_t value)
{
	try {
		/** 进入临界段 m_cs  */

		IpxCamErr err = imperxParamSet("TriggerActivation", std::to_string(value));
		if (err == IPX_CAM_ERR_OK)
		{

			/** 离开临界段 m_cs */

			return true;
		}

		/** 离开临界段 m_cs */

		return false;
	}
	catch (...) {

		/** 离开临界段 m_cs */

	}

}