#ifndef MYOBJECT_H
#define MYOBJECT_H
#include <node.h>


#include <node_object_wrap.h>
//////////////////字符编码///////////////////
#include "EncodeUtils.h"
//////////////////字符编码///////////////////


//////////////////引入json///////////////////
#include "CJsonObject.h"
//////////////////引入json///////////////////

//////////////////引入opencv///////////////////
#include "OpencvUtils.h"
//////////////////引入opencv///////////////////

//////////////////XML文件///////////////////
#include "xmlOper.h"
//////////////////XML文件///////////////////

//////////////////串口支持///////////////////
//#include "SerialPort.h"
#include "SerialUtils.h"
//////////////////串口支持///////////////////

//////////////////光谱仪相机支持///////////////////
#include "IpxCamera.h"
//////////////////光谱仪相机支持///////////////////

//////////////////光谱仪相机支持///////////////////
#include "LogUtils.h"
//////////////////光谱仪相机支持///////////////////

//////////////////光谱仪相机支持///////////////////
#include "threadsafe_queue.h"

//#include <thread>

//////////////////光谱仪相机支持///////////////////


#include <uv.h>

#define GPS_ANY_SIZE 1024

#define GPS_ANY_GGA 2048

#define GPS_ANY_SAVE 1024

#define CAMERY_SIZE_R 100
#define CAMERY_RENDER 6
#define RENDER_MAX_FPS 20
#define MB2
struct AsyncData
{
	std::wstring type;  // 类型
	std::wstring message;  // 消息
};
static AsyncData g_asyncData;//光谱仪回调

static AsyncData g_sysInfoAsyncData;//系统信息回调

static AsyncData g_recCmdAsyncData;//

static AsyncData g_recGpsAsyncData;//

typedef struct  specData {
	uint8_t * imagedata;
	uint32_t bufSize;
	int Width;
	int Height;
};

namespace demo {

	class MyObject : public node::ObjectWrap {
	public:
		static void Init(v8::Isolate* isolate);
		static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);

	private:
		explicit MyObject();
		~MyObject();

		static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void callBack(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void sysInfoCallBack(const v8::FunctionCallbackInfo<v8::Value>& args);
		static v8::Persistent<v8::Function> constructor;

		// 获取到 Frame
		static v8::Persistent<v8::Function> g_frameDataCallback;

		static v8::Persistent<v8::Function> g_sysInfoCallback;

		static void RecCmdCallBack(const v8::FunctionCallbackInfo<v8::Value>& args);
		static v8::Persistent<v8::Function> g_recCmdCallback;


		static void RecGpsCallBack(const v8::FunctionCallbackInfo<v8::Value>& args);
		static v8::Persistent<v8::Function> g_recGpsCallback;

		// 是否退出摄像头抓取线程
		static bool g_quit;
		static bool g_sysinfo;
		static bool g_syscmd;
		static bool g_sysgps;

		// 摄像头抓取线程
		static uv_thread_t g_captureThreadId;
		static uv_thread_t g_captureThreadIdPicView;

		static uv_thread_t g_sysInfoThreadId;

		static uv_thread_t g_cmdCallbackThreadId;
		static uv_thread_t g_gpsCallbackThreadId;
		//归零

		// 光谱仪数据存储线程
		static uv_thread_t g_saveThreadId;

		// 异步对象，用来在摄像头抓取线程和主线程之间传递数据
		static uv_async_t g_async;
		//系统状态信息回调
		static uv_async_t g_asyncSysInfo;
		static uv_async_t g_asyncRecCmd;
		static uv_async_t g_asyncRecGps;

		static void start(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void stop(const v8::FunctionCallbackInfo<v8::Value>& args);

		static UINT WINAPI saveData(void *arg);

		static int saveDataThread();
		static int viewThread();
		static UINT WINAPI capture(void *arg);
		static UINT WINAPI captureview(void *arg);

		static UINT WINAPI sysInfoHandle(void *arg);

		static UINT WINAPI sysCmdHandle(void *arg);
		static UINT WINAPI sysGpsHandle(void *arg);
		//static void sysGpsHandle(void *arg);

		static void broadcast();

		static void eventCallback(uv_async_t* handle);

		static void eventCallbackSysInfo(uv_async_t* handle);
		static void eventCallbackRecCmd(uv_async_t* handle);
		static void eventCallbackRecGps(uv_async_t* handle);
		static double get_wall_time();
		static unsigned char* g_frameBuffer;
		static unsigned char* spectrumBuf;
		/*****************日志回调函数******************************/
		static v8::Persistent<v8::Function> g_logCallback;
		static void setLogCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void log(v8::Isolate* isolate, const wchar_t * const message);

		static void createFrameBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void createSpecBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);

	private:
		static void getCameryAQStatus(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void getCameryRecStatus(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void getCameryList(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void getHdrInfo(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void openCamery(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void cameryCallback(IpxCam::Buffer* buffer);
		static void closeCamery(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void setParamImperx(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void getParamImperx(const v8::FunctionCallbackInfo<v8::Value>& args);
		
		/*static std::queue<specData*>specDataT;
		static std::queue<specData*>specDataTView;*/

		static threadsafe_queue<specData> specDataT;
		static threadsafe_queue<specData> specDataTView;

		static bool is_save;
		static void start_rec(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void stop_rec(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void UART_SEND(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void wavelengthSwitch(const v8::FunctionCallbackInfo<v8::Value>& args);
	private:
		static void getSerialPortList(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void usartCallback(BYTE* data, int Length);
		static void usartgpsCallback(BYTE* data, int Length);
	private:

	private:
		static cv::Mat rgb_mat;
		static int r_row;
		static int g_row;
		static int b_row;
		static int spectrum_cols;
		static int rgb_mat_width;//图像的宽是自己定义的，高是原图的宽
		static int current_col;
		static ASYNCSERIAL::AsyncSerial *m_gpsSerial;
		static ASYNCSERIAL::AsyncSerial *m_Serial;
		static char buf2hex[3];
		static std::string recvresult;
		//static std::ofstream gpsFileWrite;//用于存储gps信息
		static std::FILE * gpsFile;
		//static std::ofstream gpsFileWriteTest;//用于存储gps信息
		static bool is_save_gps;
		static std::string prefix;
		static std::queue<std::string> rev_cmd;
		//static std::queue<char> gps_data;
		//static std::queue<char> gps_data_save;
		//
		static char *gps_org;
		static int gps_org_index;
		static char *gps_any;
		static int gps_any_index;

		static std::string gps_ascii;
		static int64_t ExposureTime;
		static double time_stamp;
		static std::vector<std::string> gps_msg;
	private:
		static void ID_06_ANY(std::string data);
		static void ID_0E_ANY(std::string data);
		static std::string gps_buf_06;
		static std::string gps_buf_0E;
		static int ekf_modal;
		static int rtk_modal;
		static std::string EKF_STATUS;
		static std::string RTK_STATUS;
	private:

		static void openSerialPort(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void closeSerialPort(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void openCmdSerialPort(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void closeCmdSerialPort(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void takePhoto(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void startTakephoto(uv_work_t * work);
		static void TakephotoCompleted(uv_work_t * work, int status);


		static void startSendCommand(uv_work_t * work);
		static void SendCommandCompleted(uv_work_t * work, int status);

	private:
		/** 同步互斥,临界区保护 */
		static CRITICAL_SECTION   m_cs_0;//!<gps数据
		static CRITICAL_SECTION   m_cs_1;//!<核心控制板
		
		static CRITICAL_SECTION   m_cs_4;//!<系统信息回调
	private:
		//static void autoFocus();
		static double currentArticulation;
		static cv::Mat save_image;
		static bool m_takePhoto;
		static int64_t FrameRate;

		static DWORD HEXS(char *decString);
		static int recheckSum(std::string data);
		static char *getFormatStringForCan(int num, char*cmdhead);
		static int writeLines;
		static std::FILE * spefilewrite;
		static int writeWidth;
		static int writeHeight;
		static int skip_count;
	private:

		void LoadSystemIniParam();
		static neb::CJsonObject sysInfoJson;
	};

}  // namespace demo

#endif