#include "objectFactory.h"
namespace demo {

	using v8::Context;
	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::FunctionTemplate;
	using v8::Isolate;
	using v8::Local;
	using v8::NewStringType;
	using v8::Number;
	using v8::Object;
	using v8::Persistent;
	using v8::String;
	using v8::Value;
	using v8::Exception;

	//CSerialPort *m_SerialPort=NULL;

	xmlOPer *m_xml;
	CComPtr<IXMLDOMElement> spRootEle;
	Persistent<Function> MyObject::constructor;

	v8::Persistent<v8::Function> MyObject::g_frameDataCallback;

	v8::Persistent<v8::Function> MyObject::g_sysInfoCallback;

	v8::Persistent<v8::Function> MyObject::g_recCmdCallback;
	v8::Persistent<v8::Function> MyObject::g_recGpsCallback;

	bool MyObject::g_sysinfo;

	bool MyObject::g_syscmd;
	bool MyObject::g_sysgps;

	bool MyObject::g_quit=true;
	uv_thread_t MyObject::g_captureThreadId;
	uv_thread_t MyObject::g_captureThreadIdPicView;
	uv_thread_t MyObject::g_sysInfoThreadId;
	uv_thread_t MyObject::g_cmdCallbackThreadId;
	uv_thread_t MyObject::g_gpsCallbackThreadId;
	
	uv_thread_t MyObject::g_saveThreadId;
	uv_async_t MyObject::g_async={0};
	uv_async_t MyObject::g_asyncSysInfo = { 0 };
	uv_async_t MyObject::g_asyncRecCmd = { 0 };
	uv_async_t MyObject::g_asyncRecGps = { 0 };

	CSerialPort *MyObject::m_Serial=NULL;
	CSerialPort *MyObject::m_gpsSerial = NULL;
	int64_t MyObject::FrameRate;
	std::vector<std::string> MyObject::gps_msg;

	v8::Persistent<v8::Function> MyObject::g_logCallback;

	unsigned char* MyObject::g_frameBuffer;
	unsigned char*  MyObject::spectrumBuf;

	cv::Mat MyObject::save_image;
	bool MyObject::m_takePhoto;


	double MyObject::currentArticulation;

	int64_t MyObject::ExposureTime;
	


	IpxCamera *m_IpxCamera;

	#define BUFFER_SIZE 640*480*4 
	#define GPS_BUF_SIZE 5*1024

	#define BUFFER_SPEC 270*2 

	#define BUFFER_QUEUE_SIZE 100
	//��AddOn�У�����һ���ṹ�����첽�����д�������

	//�ռ�ά��ػ���
	cv::Mat MyObject::rgb_mat;
	int  MyObject::r_row;
	int  MyObject::g_row;
	int  MyObject::b_row;
	int  MyObject::spectrum_cols = 10;
	int  MyObject::rgb_mat_width=640;//ͼ��Ŀ����Լ�����ģ�����ԭͼ�Ŀ�
	int  MyObject::current_col=0;
	bool MyObject::is_save=false;
	bool MyObject::is_save_gps = false;
	char MyObject::buf2hex[3];
	std::string MyObject::recvresult="";
	std::string MyObject::prefix="";
	std::queue<std::string> MyObject::rev_cmd;
	//std::queue<char> MyObject::gps_data;
	//std::queue<char> MyObject::gps_data_save;
	char *MyObject::gps_org;
	int MyObject::gps_org_index=0;

	char *MyObject::gps_any;
	int MyObject::gps_any_index = 0;

    std::string MyObject::gps_ascii="";
	int MyObject::ekf_modal;
	int MyObject::rtk_modal;
	std::string MyObject::EKF_STATUS;
	std::string MyObject::RTK_STATUS;

	std::string MyObject::gps_buf_06;
	std::string MyObject::gps_buf_0E;
	//�������ݶ���
	std::queue<specData*>MyObject::specDataT;
	std::queue<specData*>MyObject::specDataTView;
	neb::CJsonObject MyObject::sysInfoJson;
	struct FocusElement {
		uv_work_t work;              //libuv
		Persistent<Function> callback;    //javascript callback  <heap>
		int type;
		int starTimeout;
		int endTimeout;
		std::string cmd;
	};
	double MyObject::time_stamp;
	std::FILE * MyObject::gpsFile;
	//std::ofstream MyObject::gpsFileWrite;
	//std::ofstream MyObject::gpsFileWriteTest;


	MyObject::MyObject(){
	

		//m_xml = new xmlOPer();
		//spRootEle = m_xml->openXmlDoc();
		m_IpxCamera = new IpxCamera();
		//LoadSystemIniParam();
		g_frameBuffer = new unsigned char[BUFFER_SIZE];
		spectrumBuf = new unsigned char[BUFFER_SPEC];
		gps_org = new char[GPS_BUF_SIZE];
		gps_org_index = 0;

		gps_any = new char[GPS_BUF_SIZE];
		gps_any_index = 0;

		try {
			LogUtils::init_log("");
			LogUtils::debugger->debug("��־��ʹ���ɹ�");
		}
		catch (...) {

		}
		//�������Թ���
		//AllocConsole();
		//freopen("CONOUT$", "w", stdout);
	}

	MyObject::~MyObject() {
		m_IpxCamera = NULL;
		try {
			LogUtils::debugger->debug("�ͷ��ڴ�");
			delete[]g_frameBuffer;
			delete[]spectrumBuf;
			delete[] gps_org;
			gps_org_index = 0;
			delete[] gps_any;
			gps_any_index = 0;
			LogUtils::debugger->debug("�������� shutdown");
			LogUtils::shutdown();
		}
		catch (...) {

		}
		//m_xml = NULL;
	
	}
	double MyObject::get_wall_time()
	{
		#ifdef _WIN32
				return static_cast<double>(clock()) / static_cast<double>(CLOCKS_PER_SEC);
		#else
				struct timeval time;
				if (gettimeofday(&time, NULL))
				{
					return 0;
				}
				return (double)time.tv_sec + (double)time.tv_usec*0.000001;
		#endif
	}

	

	/**************************************************
	***@author:cui_sh
	***@brief:�����û�Ĭ�ϲ���
	***@time:
	***@version:
	**************************************************/
	void MyObject::LoadSystemIniParam()
	{
		//��ȡ�����ļ�
	
		//CComPtr<IXMLDOMNode>SingleNode = m_xml->selectSingleNode(spRootEle, _T("/System/ProgFrameTimeAbs"));
		//key_value m_kv = m_xml->selectSingleNodeKeyValue(SingleNode);
		//std::string value = CT2A(m_kv.xml_value.GetBuffer());
		//std::atoi(value.c_str());//<!--�Զ����豸-->

	

	}


	void MyObject::Init(Isolate* isolate) {
	  //m_SerialPort = NULL;
	  m_takePhoto = false;
	  // ׼�����캯��ģ��
	  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	  tpl->SetClassName(String::NewFromUtf8(isolate, "MyObject", NewStringType::kNormal).ToLocalChecked());
	  tpl->InstanceTemplate()->SetInternalFieldCount(1);

	 
	  
	  NODE_SET_PROTOTYPE_METHOD(tpl, "setLogCallback", setLogCallback);

	  NODE_SET_PROTOTYPE_METHOD(tpl, "callBack", callBack);

	  NODE_SET_PROTOTYPE_METHOD(tpl, "sysInfoCallBack", sysInfoCallBack);//ϵͳ��Ϣ�ص�
      
	  NODE_SET_PROTOTYPE_METHOD(tpl, "RecCmdCallBack", RecCmdCallBack);//STM���ư�ص�

	  NODE_SET_PROTOTYPE_METHOD(tpl, "RecGpsCallBack", RecGpsCallBack);//
	  ///
	  /*��ȡ�豸�б��е����*/
	  NODE_SET_PROTOTYPE_METHOD(tpl, "getCameryList", getCameryList);
	  NODE_SET_PROTOTYPE_METHOD(tpl, "getHdrInfo", getHdrInfo);
	  /*�����*/
	  ///
	  NODE_SET_PROTOTYPE_METHOD(tpl, "openCamery", openCamery);
	  NODE_SET_PROTOTYPE_METHOD(tpl, "closeCamery", closeCamery);
	  //����Ƶ��
	  NODE_SET_PROTOTYPE_METHOD(tpl, "start", start);

	  NODE_SET_PROTOTYPE_METHOD(tpl, "stop", stop);
	  //�л��ռ�ά
	  NODE_SET_PROTOTYPE_METHOD(tpl, "wavelengthSwitch", wavelengthSwitch);
	  //
	  NODE_SET_PROTOTYPE_METHOD(tpl, "start_rec", start_rec);

	  NODE_SET_PROTOTYPE_METHOD(tpl, "stop_rec", stop_rec);
	  //��������
	  NODE_SET_PROTOTYPE_METHOD(tpl, "setParamImperx", setParamImperx);
	  //������ȡ
	  NODE_SET_PROTOTYPE_METHOD(tpl, "getParamImperx", getParamImperx);

	  ///���ڲ���
	  NODE_SET_PROTOTYPE_METHOD(tpl, "getSerialPortList", getSerialPortList);
	  NODE_SET_PROTOTYPE_METHOD(tpl, "openSerialPort", openSerialPort);
	  NODE_SET_PROTOTYPE_METHOD(tpl, "closeSerialPort", closeSerialPort);
	  NODE_SET_PROTOTYPE_METHOD(tpl, "openCmdSerialPort", openCmdSerialPort);
	  NODE_SET_PROTOTYPE_METHOD(tpl, "closeCmdSerialPort", closeCmdSerialPort);
	  NODE_SET_PROTOTYPE_METHOD(tpl, "UART_SEND", UART_SEND);

	  NODE_SET_PROTOTYPE_METHOD(tpl, "getCameryRecStatus", getCameryRecStatus);
	  NODE_SET_PROTOTYPE_METHOD(tpl, "getCameryAQStatus", getCameryAQStatus);



	  //NODE_SET_PROTOTYPE_METHOD(tpl, "createFrameBuffer", createFrameBuffer);
	  //NODE_SET_PROTOTYPE_METHOD(tpl, "createSpecBuffer", createSpecBuffer);


	  NODE_SET_PROTOTYPE_METHOD(tpl, "takePhoto", takePhoto);
	  
	  
	  ///

	  Local<Context> context = isolate->GetCurrentContext();
	  constructor.Reset(isolate, tpl->GetFunction(context).ToLocalChecked());
	  
	  uv_async_init(uv_default_loop(), &g_async, eventCallback);
	  //ϵͳ״̬��Ϣ�ص�����
	  uv_async_init(uv_default_loop(), &g_asyncSysInfo, eventCallbackSysInfo);
	  //����ָ����ջص����� // ���ѽ��֪ͨ��ǰ��
	  uv_async_init(uv_default_loop(), &g_asyncRecCmd, eventCallbackRecCmd);

	  uv_async_init(uv_default_loop(), &g_asyncRecGps, eventCallbackRecGps);
	}

	void MyObject::New(const FunctionCallbackInfo<Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  Local<Context> context = isolate->GetCurrentContext();

	  if (args.IsConstructCall()) {
		// ���캯��һ�����ã�`new MyObject(...)`
		double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue(context).FromMaybe(0);
		MyObject* obj = new MyObject();
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	  } else {
		// ����ͨ���� `MyObject(...)` һ�����ã�תΪ������á�
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();
		args.GetReturnValue().Set(instance);
	  }
	}

	void MyObject::NewInstance(const FunctionCallbackInfo<Value>& args) {
	  Isolate* isolate = args.GetIsolate();

	  const unsigned argc = 1;
	  Local<Value> argv[argc] = { args[0] };
	  Local<Function> cons = Local<Function>::New(isolate, constructor);
	  Local<Context> context = isolate->GetCurrentContext();
	  Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();

	  args.GetReturnValue().Set(instance);
	}

	
	void MyObject::callBack(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		/*Isolate* isolate = args.GetIsolate();
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cb = Local<Function>::Cast(args[0]);

		const unsigned argc = 1;
		int index = 0;
	
		std::string imperx_info = "test"+std::to_string(index++);
		Local<Value> argv[argc] = {
				String::NewFromUtf8(isolate,
				imperx_info.c_str(),
				NewStringType::kNormal).ToLocalChecked()
		};
		cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();*/

		Isolate* isolate = args.GetIsolate();

		// �����һ���������Ǻ��������׳�����
		if (!args[0]->IsFunction())
		{
			v8::MaybeLocal<String> message = v8::String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"The first argument must be a function!").c_str());
			
			
			/*isolate->ThrowException(v8::Exception::TypeError(
				v8::String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"The first argument must be a function!").c_str())));*/
			isolate->ThrowException(v8::Exception::TypeError(message.ToLocalChecked()));
			return;
		}
		
		// ��ȡ�ص���������������ȫ�ֱ���
		Local<Function> frameDataCallback = v8::Local<v8::Function>::Cast(args[0]);
		g_frameDataCallback.Reset(isolate, frameDataCallback);

	}
	
	void MyObject::sysInfoCallBack(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		if (!args[0]->IsFunction())
		{
			v8::MaybeLocal<String> message = v8::String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"The first argument must be a function!").c_str());
			isolate->ThrowException(v8::Exception::TypeError(
				message.ToLocalChecked()));
			return;
		}

		// ��ȡ�ص���������������ȫ�ֱ���
		Local<Function> Callback = v8::Local<v8::Function>::Cast(args[0]);
		g_sysInfoCallback.Reset(isolate, Callback);
	}


	void MyObject::RecCmdCallBack(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		if (!args[0]->IsFunction())
		{
			v8::MaybeLocal<String> message = v8::String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"The first argument must be a function!").c_str());
			isolate->ThrowException(v8::Exception::TypeError(
				message.ToLocalChecked()));
			return;
		}

		// ��ȡ�ص���������������ȫ�ֱ���
		Local<Function> Callback = v8::Local<v8::Function>::Cast(args[0]);
		g_recCmdCallback.Reset(isolate, Callback);
		//double time_stamp = get_wall_time();
		//
		//while (true)
		//{
		//	double _time = get_wall_time();
		//	double microsecond = _time - time_stamp;//ms
		//	//printf("microsecond = %f\r\n", microsecond);
		//	if (microsecond > 2)
		//	{
		//		printf("microsecond = %f\r\n", microsecond);
		//		time_stamp = _time;
		//		//break;
		//	}
		//	Sleep(1);
		//}
		//double m_fps = 1.f / (_time - time_stamp);
		



		//����һ�����������Ϣִ���߳�
		/*g_syscmd = false;
		uv_thread_create(&g_cmdCallbackThreadId, sysCmdHandle, nullptr);*/
	}

	void MyObject::RecGpsCallBack(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		if (!args[0]->IsFunction())
		{
			v8::MaybeLocal<String> message = v8::String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"The first argument must be a function!").c_str());
			isolate->ThrowException(v8::Exception::TypeError(
				message.ToLocalChecked()));
			return;
		}

		// ��ȡ�ص���������������ȫ�ֱ���
		Local<Function> Callback = v8::Local<v8::Function>::Cast(args[0]);
		g_recGpsCallback.Reset(isolate, Callback);

		//����һ�����������Ϣִ���߳�
		//g_syscmd = false;
		//uv_thread_create(&g_gpsCallbackThreadId, sysGpsHandle, nullptr);
	}

	void MyObject::saveData(void *arg)
	{

		try {
			int lines = 0;

			std::string spe_path = prefix + ".spe";
			EncodeUtils::createDirectory(spe_path.c_str());
			//д��spe�ļ����ļ�
			std::FILE * spefile;
			spefile = std::fopen(spe_path.c_str(), "wb");
			//std::ofstream spefile(spe_path.c_str(), std::fstream::out | std::fstream::binary);
			//printf("�ȴ����ݵ���\r\n");
			int Width;
			int Height;
			while (is_save) {

				if (specDataT.size() > BUFFER_QUEUE_SIZE)
				{
					for (int index = 0; index < BUFFER_QUEUE_SIZE; index++)
					{
						specData *m_data = specDataT.front();
						Width = m_data->Width;
						Height = m_data->Height;
						std::fwrite(m_data->imagedata, m_data->bufSize, 1, spefile);
						//spefile.write((char*)m_data->imagedata, m_data->bufSize);
						specDataT.pop();
						delete[] m_data->imagedata;
						delete m_data;
						m_data = NULL;


						lines++;
					}
					//spefile.flush();
				}
				//printf("saveData:01  %d\r\n", specDataT.size());

				Sleep(1);
			}

			while (!specDataT.empty()) {

				specData *m_data = specDataT.front();
				Width = m_data->Width;
				Height = m_data->Height;
				std::fwrite(m_data->imagedata, m_data->bufSize, 1, spefile);
				//spefile.write((char*)m_data->imagedata, m_data->bufSize);
				specDataT.pop();
				delete[] m_data->imagedata;
				delete m_data;
				m_data = NULL;

				lines++;
				//printf("saveData:02  %d\r\n", specDataT.size());
				Sleep(1);
			}
			std::fflush(spefile);
			std::fclose(spefile);
			spefile = NULL;
			is_save_gps = false;


			//д��hdr�ļ�
			//�ӵ�ǰ�ļ�Ŀ¼�¶�ȡhdr�ļ�
			std::string hdr_path = prefix + ".hdr";
			EncodeUtils::createDirectory(hdr_path.c_str());
			std::FILE * hdrfile;
			hdrfile = std::fopen(hdr_path.c_str(), "wb");
			//std::ofstream hdrfile(hdr_path, std::fstream::out);


			std::FILE * red_hdrfile;
			red_hdrfile = fopen("./template.hdr", "rb");

			//std::ifstream red_hdrfile("./template.hdr", std::fstream::in);
			char buffer[1024];


			while (!std::feof(red_hdrfile)) {
				std::fgets(buffer, 1024, red_hdrfile);
				if (strstr(buffer, "lines"))
				{
					std::fprintf(hdrfile, "lines = %d\r\n", lines);
					//hdrfile << "lines = " << std::to_string(lines) << std::endl;
				}
				else if (strstr(buffer, "samples"))
				{
					std::fprintf(hdrfile, "samples = %d\r\n", Width);
					//hdrfile << "samples = " << std::to_string(Width) << std::endl;
				}
				else if (strstr(buffer, "bands") && !strstr(buffer, "default"))
				{
					std::fprintf(hdrfile, "bands = %d\r\n", Height);
					//hdrfile << "bands = " << std::to_string(Height) << std::endl;
				}
				else
				{
					std::fwrite(buffer, strlen(buffer), 1, hdrfile);
					//fprintf(hdrfile, "bands = %d\r\n", Height);
					//hdrfile << buffer << std::endl;
				}

				Sleep(1);
			}
			std::fflush(hdrfile);
			std::fclose(hdrfile);
			hdrfile = NULL;
			std::fclose(red_hdrfile);
			red_hdrfile = NULL;
			//�ر�gps�ļ�д����
			if (m_gpsSerial != NULL)
			{
				if (gps_org_index > 0)
				{
					std::fwrite(gps_org, gps_org_index + 1, 1, gpsFile);
					//gpsFileWrite.write(gps_org, gps_org_index + 1);
				}

				/*while (gps_data_save.size())
				{
					gpsFileWrite.put(gps_data_save.front());
					gps_data_save.pop();
					Sleep(1);
				}*/
				std::fflush(gpsFile);
				std::fclose(gpsFile);
				gpsFile = NULL;
				//printf("gpsFileWrite close\r\n");
			}
		}
		catch (...) {
			LogUtils::error->error("��������saveData");
		}

		
	}

	void MyObject::captureview(void *arg)
	{
		g_asyncData.type = L"Frame";
		g_asyncData.message = L"Frame captured!";
		g_async.data = &g_asyncData;
		double start_time = get_wall_time();
		double end_time = 0;
		int skip = 1;
		int fps = 0;
		int skip_index = 0;
		
		
		//LogUtils::debugger->debug("���ݵ������ܼ���ó� fps = %d skip=%d FrameRate=%d", fps, skip, FrameRate);
		while (!g_quit) {
			
			///////////////////////////////////////////////////////////////////////////////////////
			try {
				//  �����ǰ֡Ƶ����25fps ���֡һ����ʾ��֡Ƶ������25֡
				if (!specDataTView.empty() && specDataTView.size() > CAMERY_SIZE_R)
				{
					LogUtils::debugger->error("�ڴ�ռ�ù��� �ͷ�%d", CAMERY_SIZE_R);
					//��⵽�ڴ�ռ�ù��ߣ��ͷ�
					for (int index = 0; index < CAMERY_SIZE_R; index++)
					{
						specData *m_data = specDataTView.front();
						delete[] m_data->imagedata;
						delete m_data;
						m_data = NULL;
						specDataTView.pop();
					}
				}

				if (!specDataTView.empty())
				{
					
					//LogUtils::debugger->debug("for (int index = 0; index < skip; index++)");
					specData *m_data = specDataTView.front();
					//Width = m_data.Width;
					//Height = m_data.Height;
					cv::Mat image(m_data->Height, m_data->Width, CV_16UC1, m_data->imagedata);

					//
					cv::Mat spectrum = image.colRange(spectrum_cols, spectrum_cols + 1).clone();

					//cv::imshow("image", image);
					if (current_col == 0)
					{
						//����һ����ɫͨ��ͼ��
						rgb_mat = cv::Mat(m_data->Width, rgb_mat_width, CV_16UC3, cv::Scalar(4095, 4095, 4095));
						current_col++;
					}
					for (int index = 0; index < image.cols; index++)
					{
						/*if (index < image.rows)
						{
							spectrumBuf[index] =image.at<ushort>(index, spectrum_cols);
						}*/
						//b g r 
						//rgb_mat.at<cv::Vec3s>(rows,cols)
						rgb_mat.at<cv::Vec3s>(index, current_col - 1)[0] = image.at<ushort>(b_row, index);//B
						rgb_mat.at<cv::Vec3s>(index, current_col - 1)[1] = image.at<ushort>(g_row, index);//G
						rgb_mat.at<cv::Vec3s>(index, current_col - 1)[2] = image.at<ushort>(r_row, index);//R
					}


					if (current_col < rgb_mat.cols)
					{
						current_col++;
					}
					else
					{
						//current_col = 1;
						//converted
						cv::Mat t_mat = cv::Mat::zeros(2, 3, CV_32FC1);

						t_mat.at<float>(0, 0) = 1;
						t_mat.at<float>(0, 2) = -1; //ˮƽƽ����
						t_mat.at<float>(1, 1) = 1;
						t_mat.at<float>(1, 2) = 0; //��ֱƽ����

						//����ƽ�ƾ�����з���任
						cv::warpAffine(rgb_mat, rgb_mat, t_mat, rgb_mat.size());

						t_mat.release();
					}
					//

					cv::Mat converted = rgb_mat.clone();

					cv::resize(converted, converted, cv::Size(640, 480));
					line(converted, cv::Point(630, spectrum_cols), cv::Point(640, spectrum_cols), cv::Scalar(80, 144, 255), 1);
					double g_MaxValue;
					cv::minMaxLoc(converted, 0, &g_MaxValue, 0, 0);
					(g_MaxValue > 4095) ? g_MaxValue = 4095 : g_MaxValue = g_MaxValue;
					converted.convertTo(converted, CV_8U, 255.0 / g_MaxValue);

					//printf("%f    %f\r\n", 255.0 / g_MaxValue,g_MaxValue);
					//cv::imshow("color_tobgra", converted);
					//cv::waitKey(5);
					cv::cvtColor(converted, converted, cv::COLOR_RGB2BGRA);

					//cv::imshow("color_tobgra", converted);
					//cv::waitKey(5);
					memcpy(g_frameBuffer, converted.data, BUFFER_SIZE);

					memcpy(spectrumBuf, spectrum.data, BUFFER_SPEC);
					//cv::imshow("spectrum", spectrum);
					//cv::waitKey(5);
					//uv_async_send(&g_async);

					spectrum.release();
					converted.release();
					image.release();

					delete[] m_data->imagedata;
					delete m_data;
					m_data = NULL;
					specDataTView.pop();

					end_time = get_wall_time();
					fps = 1 / (end_time - start_time);
					start_time = end_time;
					if (fps > RENDER_MAX_FPS) {

						skip = fps / RENDER_MAX_FPS;
						skip > 0 ? skip : 1;

						if (skip_index%skip == 0) {
							uv_async_send(&g_async);
							skip_index = 0;
						}
						skip_index++;
					}
					else {
						uv_async_send(&g_async);
					}
					
					
					
					
				}
					
				//FrameRate = m_IpxCamera->getCurrentFrameRate();
				//skip = FrameRate / fps;
				//skip > 0 ? skip : 1;
				//LogUtils::debugger->debug("��ʾ��������С %d", specDataTView.size());
				//LogUtils::debugger->debug("���ݵ������ܼ���ó� fps = %d skip=%d FrameRate=%d", fps, skip, FrameRate);
				
			}
			catch (...) {
				LogUtils::debugger->error("captureview��������");
			}
			Sleep(1);
		}
		//δ��ʾ������
		while (!specDataTView.empty()) {
			specData *m_data = specDataTView.front();
			delete[] m_data->imagedata;
			delete m_data;
			m_data = NULL;
			specDataTView.pop();
		}


	}

	void MyObject::capture(void *arg)
	{
		////// ��ȡ Frame ��ֱ���˳�ϵͳ
		//cv::Mat img = cv::imread("C:\\Users\\cui_sh\\Pictures\\Screenshots\\test.png", 1);

		////cv::resize(img, img, cv::Size(640, 480));
		////cv::Mat converted;
		////cv::cvtColor(img, converted, cv::COLOR_BGR2RGBA);

		//while (!g_quit)
		//{
		//	g_asyncData.type = L"Frame";
		//	g_asyncData.message = L"Frame captured!";
		//	g_async.data = &g_asyncData;
		//	img_base64 = OpencvUtils::Mat2Base64(img,"png");
		//	//memcpy(g_frameBuffer, converted.data, BUFFER_SIZE);
		//	uv_async_send(&g_async);
		//	Sleep(12);
		//}
		//img.release();
		////converted.release();

		////cvNamedWindow("show", 0);

		//������Ƶ��
		bool result = m_IpxCamera->StartAcquisition(cameryCallback);
	}
	/*
	*ϵͳ���յ�ָ�����ִ�й���
	*/
	void MyObject::sysCmdHandle(void *arg)
	{
		/*std::string cmd = "";
		int value = 0;*/
		int count_0 = 0;
		int count_1 = 1;
		while (!g_syscmd)
		{
			if (!rev_cmd.empty()) {
				g_recCmdAsyncData.type = L"SysCmd";
				g_recCmdAsyncData.message = L"SysCmd Callback!";
				g_asyncRecCmd.data = &g_recCmdAsyncData;
				/*cmd = rev_cmd.front();
				rev_cmd.pop();
				value = EncodeUtils::calcLSB(cmd);*/
				//printf("����ָ�%s result��%d %d\r\n", cmd.c_str(), value, rev_cmd.size());
				//����ָ��
				/*char * data = EncodeUtils::getFormatStringForMon(100, "AA12");
				printf("����ָ�%s", data);
				delete data;*/
				//printf("����ָ�\r\n");
				uv_async_send(&g_asyncRecCmd);
				Sleep(10);
				if (count_0 < 120)
				{
					count_0++;
				}
				//printf("count_0=%d\r\n", count_0);
			}
			else
			{

				Sleep(400);
				if (count_1 < 3)
				{
					count_1++;
				}
				//printf("count_1=%d\r\n", count_1);
			}

			if (count_0 >= 120)
			{
				count_1 = 0;
				count_0 = 0;
				//�㲥
				//printf("count_0�㲥\r\n");
				//����״̬
				broadcast();
			}
			if (count_1 >= 3)
			{
				count_1 = 0;
				count_0 = 0;
				broadcast();
				//printf("count_1�㲥\r\n");
			}
		}
	}
	/*
	*ϵͳ���յ�ָ�����ִ�й���
	*/
	void MyObject::sysGpsHandle(void *arg)
	{
		time_stamp = get_wall_time();
		while (!g_sysgps)
		{
			//if (!gps_data.empty() && (gps_data.size()> GPS_ANY_GGA)) {
			if(gps_any_index> GPS_ANY_GGA){
				g_recGpsAsyncData.type = L"SysGps";
				g_recGpsAsyncData.message = L"SysGps Callback!";
				g_asyncRecGps.data = &g_recGpsAsyncData;
				
				uv_async_send(&g_asyncRecGps);
				Sleep(50);
			}
			else
			{
				Sleep(200);
			}
			
		}
	}

	void MyObject::broadcast()
	{
		/*if (m_Serial == NULL)
			return;*/
		try {
			std::string data = EncodeUtils::getFormatStringForMon(1, "AA23");
			m_Serial->WriteDataHex((char*)data.c_str(), data.length());

			//������״̬ 
			if (m_IpxCamera->lDevice != nullptr)
			{
				data = EncodeUtils::getFormatStringForMon(0, "AA10");
				m_Serial->WriteDataHex((char*)data.c_str(), data.length());
				
			}
			else {
				data = EncodeUtils::getFormatStringForMon(1, "AA10");
				m_Serial->WriteDataHex((char*)data.c_str(), data.length());
				
			}
			//����ʱ�䣨ms��
			if (m_IpxCamera->lDevice != nullptr) {
				int m_ExposureTime = double(ExposureTime / 10);
				data = EncodeUtils::getFormatStringForMon(m_ExposureTime, "AA11");
				m_Serial->WriteDataHex((char*)data.c_str(), data.length());
			}
			//ͼ��ֹͣ g_quit = false Ԥ��  is_save = true ����
			if (g_quit && !is_save) {
				//g_quit ֹͣԤ�� û�б��� 
				data = EncodeUtils::getFormatStringForMon(2, "AA12");//ֹͣ�ɼ�
				m_Serial->WriteDataHex((char*)data.c_str(), data.length());
			}
			else if (!g_quit && !is_save) {
				//ͼ��Ԥ�� û�б���
				data = EncodeUtils::getFormatStringForMon(1, "AA12");//ͼ��Ԥ��
				m_Serial->WriteDataHex((char*)data.c_str(), data.length());
			}
			if (!g_quit && is_save) {
				//ͼ��ɼ�
				data = EncodeUtils::getFormatStringForMon(0, "AA12");//
				m_Serial->WriteDataHex((char*)data.c_str(), data.length());
			}
			//����
			data = EncodeUtils::getFormatStringForMon(r_row, "AA13");
			m_Serial->WriteDataHex((char*)data.c_str(), data.length());
			//�ռ�ά
			data = EncodeUtils::getFormatStringForMon(spectrum_cols, "AA14");
			m_Serial->WriteDataHex((char*)data.c_str(), data.length());
			//RTK״̬ RTK fixed
			if ((RTK_STATUS.compare("RTK float") == 0) || (RTK_STATUS.compare("RTK fixed") == 0)) {
				data = EncodeUtils::getFormatStringForMon(1, "AA25");//RTK
				m_Serial->WriteDataHex((char*)data.c_str(), data.length());
			}
			else
			{
				data = EncodeUtils::getFormatStringForMon(0, "AA25");//��RTK
				m_Serial->WriteDataHex((char*)data.c_str(), data.length());
			}
			//GPSλ������
			if ((EKF_STATUS.compare("") != 0) && (EKF_STATUS.size() > 0)) {
				data = EncodeUtils::getFormatStringForMon(atoi(EKF_STATUS.c_str()), "AA24");//gps������
				m_Serial->WriteDataHex((char*)data.c_str(), data.length());
			}
		}
		catch (...) {

		}
		

	}
	void MyObject::sysInfoHandle(void *arg)
	{
		try {
			while (!g_sysinfo)
			{
				g_sysInfoAsyncData.type = L"SysInfo";
				g_sysInfoAsyncData.message = L"SysInfo Callback!";
				g_asyncSysInfo.data = &g_sysInfoAsyncData;
				//��ȡ�������Ϣ����json�У�֪ͨ�ص���������������ʾ
				/*double ExposureTime = PHY::camery2500::getExposureTime();
				double FrameRate = PHY::camery2500::getFrameRate();
				double Temperature = PHY::camery2500::getTemperature();*/
				sysInfoJson.Clear();
				if (m_IpxCamera->lDevice != nullptr)
				{
					FrameRate = m_IpxCamera->getCurrentFrameRate();
					sysInfoJson.Add("FrameRate", FrameRate);
					ExposureTime = m_IpxCamera->getExposureTimeRaw();
					sysInfoJson.Add("ExposureTime", double(ExposureTime / 1000000.0));
					int64_t Temperature = m_IpxCamera->getCurrentTemperature();
					sysInfoJson.Add("Temperature", Temperature);
				}
				//LogUtils::debugger->debug("д�뻺������ǰ��С%d", specDataT.size());
				//LogUtils::debugger->debug("��ʾ��������С %d", specDataTView.size());

				uv_async_send(&g_asyncSysInfo);
				Sleep(1800);
			}
		}
		catch (...) {
			LogUtils::debugger->error("�������� sysInfoHandle");
		}
		

	}

	/**************************************************
	***@author:cui_sh
	***@brief:��ʼ���ݲɼ�
	***@time:
	***@version:
	**************************************************/
	void MyObject::start_rec(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		//printf("start_rec\r\n");
		Isolate*  isolate = v8::Isolate::GetCurrent();
		if (args.Length() < 2) {
			// �׳�һ�����󲢴��ص� JavaScript��
			std::string error = "invalid parameter";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		if (!args[0]->IsString())
		{
			std::string error = "Wrong parameter type";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		if (!args[1]->IsFunction()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid parameter[fun]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}

		if (m_IpxCamera->lDevice == nullptr)
		{
			std::string error = "Device is not open";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}
		//if (!m_IpxCamera->startAcquisition)
		//{
		//	isolate->ThrowException(Exception::TypeError(
		//		String::NewFromUtf8(isolate,
		//			EncodeUtils::toUtf8String(L"error! stream is open").c_str(),
		//			NewStringType::kNormal).ToLocalChecked()));
		//	return;
		//	//����Ƶ��
		//}
		v8::String::Utf8Value param_type(args[0]->ToString());
		std::string data_d = std::string(*param_type);//ϵͳ��ʹ��������Ϣ
		//��������
		data_d = EncodeUtils::WStringToString(EncodeUtils::Utf8ToUnicode(data_d));

		//data_d = EncodeUtils::StringToWString(data_d);
		//std::wstring ww = EncodeUtils::Utf8ToUnicode(data_d);;
		CTime t = CTime::GetCurrentTime();
		char buf[128];
		
		if (data_d.compare("") == 0 || data_d.length() == 0)
		{
			sprintf(buf, "./data/%04d%02d%02d%02d%02d%02d/%04d%02d%02d%02d%02d%02d", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		}
		else
		{
			sprintf(buf, "%s/%04d%02d%02d%02d%02d%02d/%04d%02d%02d%02d%02d%02d", data_d.c_str(), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		}
		
		//��ʼ���ݴ洢
		prefix = buf;
		//printf("prefix = %s", prefix.c_str());
		
		//��gps�ļ���
		//printf("m_gpsSerial \r\n");
		if (m_gpsSerial != NULL)
		{
			
			std::string path = prefix + ".gps";
			EncodeUtils::createDirectory(path);
			gpsFile = std::fopen(path.c_str(), "wb");
			//gpsFileWrite.open(path.c_str(), std::fstream::out | std::fstream::binary);
			/*if (gpsFileWrite.is_open())
			{
				printf("success: \r\n", path.c_str());
			}
			printf("%s\r\n", path.c_str());*/
			
		}
		
		is_save = true;
		
		is_save_gps = true;
		gps_org_index = 0;
		//�����ⴥ��ָ��
		/*if (m_Serial != NULL) {
			m_Serial->WriteDataHex("AA36000000003655",strlen("AA36000000003655"));
		}*/
		//printf("uv_thread_create start_rec\r\n");
		uv_thread_create(&g_saveThreadId, saveData, nullptr);

		log(isolate, L"start_rec");
		
		v8::Local<v8::Function>::Cast(args[1])->Call(isolate->GetCurrentContext(), v8::Null(isolate), 0, NULL);
		//�ص�����
	}
	/**************************************************
	***@author:cui_sh
	***@brief:ֹͣ���ݲɼ�
	***@time:
	***@version:
	**************************************************/
	void MyObject::stop_rec(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate*  isolate = v8::Isolate::GetCurrent();
		
		is_save = false;
		printf("stop_rec\r\n");
		uv_thread_join(&g_saveThreadId);

		log(isolate, L"stop_rec");
		//�ص�����

	}
	/**************************************************
	***@author:cui_sh
	***@brief:��ָ���Ĵ����Ϸ�����
	***@time:
	***@version:
	**************************************************/
	void MyObject::UART_SEND(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		if (args.Length() < 4)
		{
			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! need input camery index").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		if (!args[0]->IsString()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		if (!args[1]->IsNumber()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[number]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		if (!args[2]->IsNumber()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[number]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		if (!args[3]->IsFunction()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		v8::String::Utf8Value param1(args[0]->ToString());
		std::string data_d = std::string(*param1);
		/*int param_timeout = args[1].As<Number>()->Value();
		double _times = get_wall_time();
		while (true && param_timeout!=0) {
			double _timee = get_wall_time();
			if ((_timee - _times) >= param_timeout)
			{
				break;
			}
			Sleep(100);
		}

		if (m_Serial != NULL) {
			m_Serial->WriteDataHex((char*)data_d.c_str(), data_d.length());
		}

		param_timeout = args[2].As<Number>()->Value();
		_times = get_wall_time();
		while (true && param_timeout != 0) {
			double _timee = get_wall_time();
			if ((_timee - _times) >= param_timeout)
			{
				break;
			}
			Sleep(100);
		}


		v8::Local<v8::Function>::Cast(args[3])->Call(isolate->GetCurrentContext(), v8::Null(isolate), 0, NULL);
	*/
		///////////////////////////////////////////
		//Isolate* isolate = args.GetIsolate();

		//����
		//cv::write("", matimg);
		FocusElement * element = new FocusElement();
		element->work.data = element;
		element->type = -1;
		element->starTimeout = args[1].As<Number>()->Value();
		element->endTimeout = args[2].As<Number>()->Value();
		element->cmd = data_d;
		element->callback.Reset(isolate, Local<Function>::Cast(args[3]));
		uv_queue_work(uv_default_loop(), &element->work, startSendCommand, SendCommandCompleted);
	
	
	
	}
	void MyObject::start(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate*  isolate = v8::Isolate::GetCurrent();
		if (m_IpxCamera->lDevice == nullptr)
		{
			std::string error = "Device is not open";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}
		if (m_IpxCamera->startAcquisition)
		{
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					EncodeUtils::toUtf8String(L"error! stream is open").c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}
		/*if (args.Length() < 1)
		{
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					EncodeUtils::toUtf8String(L"error! need input camery index").c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}
		if (!args[0]->IsNumber()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					EncodeUtils::toUtf8String(L"error! invalid params").c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;

		}*/
	
		//�жϵ�ǰ��Ƶ���Ƿ��Ѿ���
		//printf("start\r\n");
		
		r_row= 250;
		g_row= 130;
		b_row= 100;

		current_col = 0;
		g_quit = false;
		
		// �����̣߳����߳��ڴ���ɼ���ʶ�𣬲������ս���ַ���ǰ��չʾ
		uv_thread_create(&g_captureThreadId, capture, nullptr);//��ȡͼ���߳�
		//������ʾͼ���߳�
		uv_thread_create(&g_captureThreadIdPicView, captureview, nullptr);//��ȡͼ���߳�
		//LogUtils::debugger->debug("��ʼ��Ⱦcaptureview");
		log(isolate, L"start");
		
		v8::Local<v8::Function>::Cast(args[0])->Call(isolate->GetCurrentContext(), v8::Null(isolate), 0, NULL);
		
	}
	void MyObject::stop(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate*  isolate = v8::Isolate::GetCurrent();

		if (args.Length() < 2) {
			// �׳�һ�����󲢴��ص� JavaScript��
			std::string error = "invalid parameter";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		if (!args[0]->IsNumber())
		{
			std::string error = "Wrong parameter type";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		if (!args[1]->IsFunction()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[fun]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}

		if (m_IpxCamera->lDevice == nullptr)
		{
			std::string error = "Device is not open";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}
		if (!m_IpxCamera->startAcquisition)
		{
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					EncodeUtils::toUtf8String(L"error! stream is close").c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}
		int param_value = args[0].As<Number>()->Value();
		//printf("param_value=%d\r\n", param_value);
		if (param_value == 1)
		{
			if (is_save)
			{
				isolate->ThrowException(Exception::TypeError(
					String::NewFromUtf8(isolate,
						EncodeUtils::toUtf8String(L"error! Data acquisition").c_str(),
						NewStringType::kNormal).ToLocalChecked()));
				return;
			}
		}
		
		//printf("stop\r\n");
		g_quit = true;
		bool result = m_IpxCamera->StopAcquisition();
		//printf("StopAcquisition\r\n");
		uv_thread_join(&g_captureThreadId);
		uv_thread_join(&g_captureThreadIdPicView);
		//printf("g_captureThreadId-stop\r\n");
		log(isolate, L"stop");

		v8::Local<v8::Function>::Cast(args[1])->Call(isolate->GetCurrentContext(), v8::Null(isolate), 0, NULL);
	}

	void MyObject::eventCallback(uv_async_t* handle)
	{
		Isolate*  isolate = v8::Isolate::GetCurrent();
		AsyncData *asyncData = reinterpret_cast<AsyncData*>(handle->data);
		//printf("%s\r\n", asyncData->type.c_str());
		if (asyncData->type == L"Error")
		{
			log(isolate, asyncData->message.c_str());
		}
		else if (asyncData->type == L"Frame")
		{
			

			v8::Local<v8::SharedArrayBuffer>  buffer = v8::SharedArrayBuffer::New(isolate, g_frameBuffer, BUFFER_SIZE);
			v8::Local<v8::Uint8ClampedArray> buffdata = v8::Uint8ClampedArray::New(buffer, 0, BUFFER_SIZE);
			
			
			v8::Local<v8::SharedArrayBuffer> spbuffer = v8::SharedArrayBuffer::New(isolate, spectrumBuf, BUFFER_SPEC);
			v8::Local<v8::Uint16Array> spbuffdata = v8::Uint16Array::New(spbuffer, 0, 270);
			

			v8::Local<v8::Value> argv[2] = { buffdata, spbuffdata };

			v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, g_frameDataCallback);
			
			cb->Call(isolate->GetCurrentContext(),v8::Null(isolate),2, argv);
			buffer.Clear();
			spbuffer.Clear();
			/*v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, g_frameDataCallback);
			cb->Call(isolate->GetCurrentContext(), v8::Null(isolate), 0, NULL);*/
		}
		
	}

	void MyObject::eventCallbackRecCmd(uv_async_t* handle)
	{
		//g_asyncRecCmd
		Isolate*  isolate = v8::Isolate::GetCurrent();
		AsyncData *asyncData = reinterpret_cast<AsyncData*>(handle->data);

		if (asyncData->type == L"Error")
		{
			log(isolate, asyncData->message.c_str());
		}
		else if (asyncData->type == L"SysCmd")
		{
			try {
				std::string cmd = rev_cmd.front();
				if (cmd.compare("AA23000000002355") == 0) {
					//�رյ���
					EncodeUtils::ReSetWindows(EWX_SHUTDOWN, true);
				}
				int value = EncodeUtils::calcLSB(cmd);
				rev_cmd.pop();
				//���ݻص� json��ʽ����
				//sysInfoJson.ToString();
				v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, g_recCmdCallback);

				const unsigned argc = 2;

				cmd = cmd.substr(0, 4);
				Local<Value> argv[argc] = {
						String::NewFromUtf8(isolate,cmd.c_str(),NewStringType::kNormal).ToLocalChecked(),
						Number::New(isolate,value)
				};


				cb->Call(isolate->GetCurrentContext(), v8::Null(isolate), argc, argv);
			}
			catch (...) {
				
			}

			
			
		}
	}


	void MyObject::eventCallbackRecGps(uv_async_t* handle)
	{
		//g_asyncRecCmd
		Isolate*  isolate = v8::Isolate::GetCurrent();
		AsyncData *asyncData = reinterpret_cast<AsyncData*>(handle->data);

		if (asyncData->type == L"Error")
		{
			log(isolate, asyncData->message.c_str());
		}
		else if (asyncData->type == L"SysGps")
		{
			try {
				//
				//for (int index = 0; index < GPS_ANY_GGA; index++)
				//{
				//	//char data = gps_data.front();
				//	//gps_data.pop();

				//	//printf("gps_data_size %d\r\n", gps_data.size());
				//	gps_ascii += data;
				//}

				gps_ascii = gps_any;

				//printf("gps_data_size %d\r\n", gps_data.size());
				if (gps_ascii.length() >= GPS_ANY_GGA) {

					int gpgga_index = gps_ascii.find("$GPGGA");
					if (gpgga_index != -1)
					{
						gps_ascii = gps_ascii.substr(gpgga_index, gps_ascii.length() - gpgga_index);
						int gpgga_enter = gps_ascii.find("\r\n");
						if (gpgga_enter != -1)
						{
							gps_ascii = gps_ascii.substr(0, gpgga_enter);
							//printf("%s\r\n", gps_ascii.c_str());
							gps_msg = EncodeUtils::split(gps_ascii, "*");
							std::string str = gps_msg.at(0);

							char ch = str.at(1);
							int x = (int)ch;
							int y;
							for (int i = 2; i < str.length(); i++) {
								y = (int)str.at(i);
								x = x ^ y;
								//printf("%d\r\n", i);
							}
							str = gps_msg.at(1);
							char buf[3];
							sprintf(buf, "%02X", x);
							//printf("%02X\r\n",x);
							if (str.compare(buf) == 0)
							{
								//printf("%d\r\n", gps_data.size());
								//printf("%s\r\n", gps_ascii.c_str());
								//gpsFileWriteTest.write(gps_ascii.c_str(), gps_ascii.length());
								//gpsFileWriteTest.write("\r\n", strlen("\r\n"));
								//gpsFileWriteTest.flush();
								//�����ŷֿ�
								//printf("gps_ascii %s %d\r\n", gps_ascii.c_str(), gps_data.size());

								gps_msg = EncodeUtils::split(gps_ascii, ",");
								if (gps_msg.size() >= 14)
								{
									//printf("msg = %s\r\n", msg.at(6));
									if (gps_msg.at(6).compare("0") == 0) {
										RTK_STATUS = "available";
									}
									else if (gps_msg.at(6).compare("1") == 0) {
										RTK_STATUS = "Autonomous";
									}
									else if (gps_msg.at(6).compare("2") == 0) {
										RTK_STATUS = "RTCM";
									}
									else if (gps_msg.at(6).compare("3") == 0) {
										RTK_STATUS = "Not used";
									}
									else if (gps_msg.at(6).compare("4") == 0) {
										RTK_STATUS = "RTK fixed";
									}
									else if (gps_msg.at(6).compare("5") == 0) {
										RTK_STATUS = "RTK float";
									}
									else if (gps_msg.at(6).compare("9") == 0) {
										RTK_STATUS = "SBAS";
									}
									EKF_STATUS = gps_msg.at(7);
									gps_msg.clear();

								}

							}
							/*else
							{
								printf("������---------------------------------------\r\n");
							}*/

							

						}

						//printf("gps_ascii=%s\r\n",gps_ascii.c_str());
					}
					//printf("%s\r\n", gps_ascii.c_str());
					gps_ascii = "";
				}
				//$GPGGA,025941.58,3019.7989422,N,12028.7625517,E,5,9,2.1,24.827,M,9.978,M,1.6,0031*77
				//����$GPGGA-���س�����������һ��$���ţ�

				//sprintf(buffer_tohex, "%02X", (BYTE)data);


				double _time = get_wall_time();
				//double m_fps = 1.f / (_time - time_stamp);
				double microsecond = _time - time_stamp;
				if (microsecond >= 1.5)
				{
					//printf("�㲥����\r\n");
					//�㲥����
					v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, g_recGpsCallback);

					const unsigned argc = 2;

					Local<Value> argv[argc] = {
							String::NewFromUtf8(isolate,EKF_STATUS.c_str(),NewStringType::kNormal).ToLocalChecked(),
							String::NewFromUtf8(isolate,RTK_STATUS.c_str(),NewStringType::kNormal).ToLocalChecked(),
					};


					cb->Call(isolate->GetCurrentContext(), v8::Null(isolate), argc, argv);



					time_stamp = _time;
				}

			}
			catch (...) {

			}
			

			//��Ƶ�ʿ��Ƶ�1500���� Ȼ����лص�
			//cb->Call(isolate->GetCurrentContext(), v8::Null(isolate), argc, argv);

		}
	}
	/*���� IMU״̬*/
	void MyObject::ID_06_ANY(std::string data)
	{
		gps_buf_06.append(data);//�ѵ�ǰgps���ݷ����ڴ�
		if (gps_buf_06.length() >= GPS_ANY_SIZE) {
			int pos = gps_buf_06.find("FF5A06");
			if (pos != -1)
			{
				/**************************************************
				***@brief:�õ�ǰ�ڴ��е�ֵ�� FF 5A 08��ͷ����ʽ����
				**************************************************/
				gps_buf_06 = gps_buf_06.substr(pos, gps_buf_06.length());
				/**************************************************
				***@brief:���� 33 FF 5A
				**************************************************/
				pos = gps_buf_06.find("33FF5A");
				if (pos != -1) {
					//�ҵ���һ������������
					gps_buf_06 = gps_buf_06.substr(0, pos + 2);
					/**************************************************
					***@brief:��ȡ��Ӧ״̬λ����
					************************************************** /
						// ת int ��   formatHexVale2Int()
						// ת float��  ConvertFloat();
						// ת double�� HexToDouble();
						//ȡ��״̬λ�е�

						///////////////////////////////////////////////�ص�֪ͨ///////////////////////////////////////////////
						/*
							0 SBG_ECOM_SOL_MODE_UNINITIALIZED 					The Kalman filter is not initialized and the returned data are all invalid.
							1 SBG_ECOM_SOL_MODE_VERTICAL_GYRO 					The Kalman filter only rely on a vertical reference to compute roll and pitch angles. Heading and navigation data drift freely.
							2 SBG_ECOM_SOL_MODE_AHRS 							A heading reference is available, the Kalman filter provides full orientation but navigation data drift freely.
							3 SBG_ECOM_SOL_MODE_NAV_VELOCITY 					The Kalman filter computes orientation and velocity. Position is freely integrated from velocity estimation.
							4 SBG_ECOM_SOL_MODE_NAV_POSITION 					Nominal mode, the Kalman filter computes all parameters (attitude, velocity, position). Absolute position is provi
						*/

					ekf_modal = EncodeUtils::formatHexVale2Int(gps_buf_06.substr(gps_buf_06.length() - 14, 8));
					//formatHexVale2Int(gps_buf_06.substr(gps_buf_06.length() - 14, 8));
					ekf_modal = ekf_modal & 0x0007;//4
					//printf("ekf_modal=%d\r\n",ekf_modal);
					if (ekf_modal == 0)
					{
						EKF_STATUS = "UNINITIALIZED";
						//RTK_STATUS="";
					}
					else if (ekf_modal == 1)
					{
						EKF_STATUS = "VERTICAL_GYRO";
					}
					else if (ekf_modal == 2)
					{
						EKF_STATUS = "AHRS";
					}
					else if (ekf_modal == 3)
					{
						EKF_STATUS = "NAV_VELOCITY";
					}
					else if (ekf_modal == 4)
					{
						EKF_STATUS = "NAV_POSITION";
					}
					///////////////////////////////////////////////�ص�֪ͨ///////////////////////////////////////////////
					gps_buf_06 = "";
				}
			}
			gps_buf_06 = "";
		}
	}
	/*���� RTK״̬*/
	void MyObject::ID_0E_ANY(std::string data)
	{
		gps_buf_0E.append(data);//�ѵ�ǰgps���ݷ����ڴ�
		if (gps_buf_0E.length() >= GPS_ANY_SIZE)
		{

			//���Ҳ����������Ϣ
			//��ȡ��������
			int pos = gps_buf_0E.find("FF5A0E");
			if (pos != -1)
			{

				/**************************************************
				***@brief:�õ�ǰ�ڴ��е�ֵ�� FF 5A 08��ͷ����ʽ����
				**************************************************/
				gps_buf_0E = gps_buf_0E.substr(pos, gps_buf_0E.length());

				/**************************************************
				***@brief:���� 33 FF 5A
				**************************************************/
				pos = gps_buf_0E.find("33FF5A");
				if (pos != -1)
				{
					//�ҵ���һ������������
					gps_buf_0E = gps_buf_0E.substr(0, pos + 2);
					rtk_modal = EncodeUtils::formatHexVale2Int(gps_buf_0E.substr(20, 8));
					//formatHexVale2Int(gps_buf_0E.substr(20, 8));
					rtk_modal >>= 6;
					rtk_modal &= 0x001F;
					/**************************************************
					***@brief:��ȡ��Ӧ״̬λ����
					**************************************************/
					// ת int ��   formatHexVale2Int()
					// ת float��  ConvertFloat();
					// ת double�� HexToDouble();
					//ȡ��״̬λ�е�

					if (rtk_modal == 0)
					{
						RTK_STATUS = "NO_SOLUTION";

					}
					else if (rtk_modal == 1)
					{
						RTK_STATUS = "UNKNOWN_TYPE";
					}
					else if (rtk_modal == 2)
					{
						RTK_STATUS = "SINGLE";
					}
					else if (rtk_modal == 3)
					{
						RTK_STATUS = "PSRDIFF";
					}
					else if (rtk_modal == 4)
					{
						RTK_STATUS = "SBAS SBAS";
					}
					else if (rtk_modal == 5)
					{
						RTK_STATUS = "OMNISTAR";
					}
					else if (rtk_modal == 6)
					{
						RTK_STATUS = "RTK_FLOAT";
					}
					else if (rtk_modal == 7)
					{
						RTK_STATUS = "RTK_INT";
					}
					else if (rtk_modal == 8)
					{
						RTK_STATUS = "PPP_FLOAT";
					}
					else if (rtk_modal == 9)
					{
						RTK_STATUS = "PPP_INT";
					}
					else if (rtk_modal == 10)
					{
						RTK_STATUS = "FIXED";
					}
					///////////////////////////////////////////////�ص�֪ͨ///////////////////////////////////////////////
					gps_buf_0E = "";
				}
			}
			gps_buf_0E = "";
		}
	}

	void MyObject::eventCallbackSysInfo(uv_async_t* handle)
	{
		Isolate*  isolate = v8::Isolate::GetCurrent();
		AsyncData *asyncData = reinterpret_cast<AsyncData*>(handle->data);
		if (asyncData->type == L"Error")
		{
			log(isolate, asyncData->message.c_str());
		}
		else if (asyncData->type == L"SysInfo")
		{
			//���ݻص� json��ʽ����
			//sysInfoJson.ToString();
			v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, g_sysInfoCallback);

			const unsigned argc = 1;


			Local<Value> argv[argc] = {
					String::NewFromUtf8(isolate,
					sysInfoJson.ToString().c_str(),
					NewStringType::kNormal).ToLocalChecked()
			};


			cb->Call(isolate->GetCurrentContext(),v8::Null(isolate), 1, argv);

		}
	}

	void MyObject::setLogCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();

		// �����һ���������Ǻ��������׳�����
		if (!args[0]->IsFunction())
		{
			v8::MaybeLocal<String> message = v8::String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"The first argument must be a function!").c_str());
			isolate->ThrowException(v8::Exception::TypeError(
				message.ToLocalChecked()));
			return;
		}
		
		// ��ȡ�ص���������������ȫ�ֱ���
		auto logCallback = v8::Local<v8::Function>::Cast(args[0]);
		g_logCallback.Reset(isolate, logCallback);

		log(isolate, L"Set log callback success!");
	}

	void MyObject::log(v8::Isolate* isolate, const wchar_t * const message)
	{
		std::string messageUtf8 = EncodeUtils::toUtf8String(message);
		v8::MaybeLocal<v8::String> argv = v8::String::NewFromUtf8(isolate, messageUtf8.c_str());
		v8::Local<v8::Value> value = argv.ToLocalChecked();
		v8::Local<v8::Function> logCallback = v8::Local<v8::Function>::New(isolate, g_logCallback);
		logCallback->Call(isolate->GetCurrentContext(),v8::Null(isolate), 1, &value);
	}

	/**************************************************
	***@author:cui_sh
	***@brief:���ص�ǰ�ɼ�״̬
	***@time:
	***@version:
	**************************************************/
	void MyObject::getCameryRecStatus(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		args.GetReturnValue().Set(v8::Boolean::New(isolate, is_save));
		//std::string imperx_info = m_IpxCamera->queryIpxCam();
		//args.GetReturnValue().Set(String::NewFromUtf8(isolate, imperx_info.c_str(), NewStringType::kNormal).ToLocalChecked());
	}

	/**************************************************
	***@author:cui_sh
	***@brief:���ص�ǰ��Ƶ��״̬
	***@time:
	***@version:
	**************************************************/
	void MyObject::getCameryAQStatus(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		args.GetReturnValue().Set(v8::Boolean::New(isolate, !g_quit));
	}




	/**************************************************
	***@author:cui_sh
	***@brief:������
	***@time:
	***@version:
	**************************************************/
	void MyObject::getCameryList(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		std::string imperx_info = m_IpxCamera->queryIpxCam();
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, imperx_info.c_str(), NewStringType::kNormal).ToLocalChecked());
	}
	/**************************************************
	***@author:cui_sh
	***@brief:��ȡ�߹���ͷ�ļ�
	***@time:
	***@version:
	**************************************************/
	void MyObject::getHdrInfo(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		//��ȡ����

		if (args.Length() < 1) {
			// �׳�һ�����󲢴��ص� JavaScript��
			std::string error = "invalid parameter";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		if (!args[0]->IsString()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[str]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		v8::String::Utf8Value param_type(args[0]->ToString());
		std::string data_d = std::string(*param_type);//ϵͳ��ʹ��������Ϣ




		//�򿪱���hdr�ļ�
		std::FILE * red_hdrfile;
		red_hdrfile = fopen("./template.hdr", "rb");

		//std::ifstream red_hdrfile("./template.hdr", std::fstream::in);
		if (red_hdrfile==NULL)
		{
			red_hdrfile = fopen("./resources/extraResources/template.hdr", "rb");
			
		}

		char buffer[2048];
		std::string result = "";
		bool isFind = false;
		if (data_d.compare("wavelength") == 0)
		{
			while (!std::feof(red_hdrfile))
			{
				std::fgets(buffer, 2048, red_hdrfile);
				if (strstr(buffer, "wavelength ="))
				{
					
					isFind = true;

				}
				if (isFind)
				{
					result += buffer;
				}
			}

			int indefOx = result.find_first_of("{");
			if (indefOx != -1)
			{
				result = result.substr(indefOx + 1, result.length() - indefOx - 2);
			}
		}
		else if (data_d.compare("default bands") == 0) {
			while (!std::feof(red_hdrfile))
			{
				std::fgets(buffer, 2048, red_hdrfile);
				if (strstr(buffer, "default bands"))
				{
					result += buffer;
					break;

				}
			}
			int indefOx = result.find_first_of("{");
			if (indefOx != -1)
			{
				result = result.substr(indefOx + 1, result.length() - indefOx - 2);
			}

		}
		std::fclose(red_hdrfile);
		red_hdrfile = NULL;

		//printf("indefOx =%d\r\n%s\r\n", indefOx, result.c_str());
		//
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str(), NewStringType::kNormal).ToLocalChecked());
	}

	/**************************************************
	***@author:cui_sh
	***@brief:��ȡ��ǰ�豸�б��еĴ����豸
	***@time:
	***@version:
	**************************************************/
	void MyObject::getSerialPortList(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		std::string serial_info = EncodeUtils::getComMonDevices();
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, serial_info.c_str(), NewStringType::kNormal).ToLocalChecked());
	}

	void MyObject::openCamery(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		/*��Ҫ��У�飬�豸û�򿪲���ִ��*/
		if (m_IpxCamera->lDevice != nullptr)
		{
			std::string error = "Device is already on";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}
		if (args.Length() < 2) {
			// �׳�һ�����󲢴��ص� JavaScript��
			std::string error = "invalid parameter";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		if (!args[0]->IsNumber())
		{
			std::string error = "Wrong parameter type";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		if (!args[1]->IsString()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[str]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		v8::String::Utf8Value param_type(args[1]->ToString());
		std::string data_d = std::string(*param_type);//ϵͳ��ʹ��������Ϣ
		//printf("%s\r\n", data_d.c_str());
		// ִ�в���
		int value = args[0].As<Number>()->Value();

		bool res = m_IpxCamera->openImperx(value);
		if (res)
		{
			/*neb::CJsonObject sys_iniparam(data_d);
			int ExposureTimeRaw;
			sys_iniparam.Get("ExposureTimeRaw", ExposureTimeRaw);
			printf("ExposureTimeRaw = %d\r\n", ExposureTimeRaw);*/
			res = m_IpxCamera->setIniParamValue(data_d);//��ʹ������
			res = m_IpxCamera->iniCameryParam();//������ʹ��
			if (res)
			{
				g_sysinfo = false;
				//����һ��ϵͳ״̬�㲥�߳�
				uv_thread_create(&g_sysInfoThreadId, sysInfoHandle, nullptr);
				
			}
			
		}
		else
		{
			std::string error = "Open camera error";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}
		std::string msg = (res == true ? "success" : "error");
		msg = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(msg));
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, msg.c_str(), NewStringType::kNormal).ToLocalChecked());
	}

	void MyObject::closeCamery(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		if (is_save)
		{
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					EncodeUtils::toUtf8String(L"error! Data acquisition").c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}
		/*��Ҫ��У�飬�豸�򿪲���ִ��*/
		if (m_IpxCamera->lDevice != nullptr)
		{
			bool res = m_IpxCamera->closeImperx();

			
			
			
			g_sysinfo = true;
			uv_thread_join(&g_sysInfoThreadId);

			std::string msg = (res == true ? "success" : "error");
			msg = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(msg));
			args.GetReturnValue().Set(String::NewFromUtf8(isolate, msg.c_str(), NewStringType::kNormal).ToLocalChecked());
		}
		else
		{
			std::string error = "�豸�Ѿ��ر�";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}


	}
	/**************************************************
	***@author:������������������
	***@brief:��������״̬ true�ǳɹ� false��ʧ��
	***@time:
	***@version:
	**************************************************/
	void MyObject::setParamImperx(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		//����Ƿ�򿪣��򿪲��ܰ��в�������

		if (m_IpxCamera->lDevice == nullptr)
		{
			std::string error = "Device is not open";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		//��һ���������ַ��� �������ò���������
		//�ڶ���������number���͵�ֵ 
		
		if (args.Length() < 2)
		{
			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid param").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		if (!args[0]->IsString()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[str]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		if (!args[1]->IsNumber()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[num]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		//ִ�в������� ��Ҫ�����û�����Ĳ�������API����
		v8::String::Utf8Value param_type(args[0]->ToString());
		std::string data_d = std::string(*param_type);
		int param_value = args[1].As<Number>()->Value();
		bool result = false;
		if (data_d.compare("ProgFrameTimeAbs") == 0){
			result = m_IpxCamera->setProgFrameTimeAbs(param_value);
		}
		else if (data_d.compare("ExposureTimeRaw") == 0) {
			//printf("ExposureTimeRaw:%d\r\n", param_value);
			result = m_IpxCamera->setExposureTimeRaw(param_value);
		}
		else if (data_d.compare("GainRaw") == 0) {
			result = m_IpxCamera->setGainRaw(param_value);
		}
		else if (data_d.compare("TriggerMode") == 0) {
			result = m_IpxCamera->setTriggerMode(param_value);
		}
		else if (data_d.compare("TriggerActivation") == 0) {
			result = m_IpxCamera->setTriggerActivation(param_value);
		}
		std::wstring msg_out = (result==true)?L"update success":L"error!";
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(msg_out.c_str()).c_str(), NewStringType::kNormal).ToLocalChecked());
	}
	/**************************************************
	***@author:��ȡ�������
	***@brief:���Ӿ�������ݶ�Ӧ��ֵ
	***@time:
	***@version:
	**************************************************/
	void MyObject::getParamImperx(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		//����Ƿ�򿪣��򿪲��ܰ��в�������

		if (m_IpxCamera->lDevice == nullptr)
		{
			std::string error = "Device is not open";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		//��һ���������ַ��� �������ò���������
		//�ڶ���������number���͵�ֵ 

		if (args.Length() < 1)
		{
			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid param").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		if (!args[0]->IsString()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[str]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}

		v8::String::Utf8Value param_type(args[0]->ToString());
		//����������
		std::string data_d = std::string(*param_type);

		int result = -1;
		if (data_d.compare("ProgFrameTimeAbs") == 0) {
			result = m_IpxCamera->getProgFrameTimeAbs();
		}
		else if (data_d.compare("CurrentFrameRate") == 0) {
			result = m_IpxCamera->getCurrentFrameRate();
		}
		else if (data_d.compare("ExposureTimeRaw") == 0) {
			result = m_IpxCamera->getExposureTimeRaw();
		}
		else if (data_d.compare("GainRaw") == 0) {
			result = m_IpxCamera->getGainRaw();
		}
		else if (data_d.compare("TriggerMode") == 0) {
			result = m_IpxCamera->getTriggerMode();
			//printf("result TriggerMode =%d\r\n", result);
		}
		else if (data_d.compare("TriggerActivation") == 0) {
			result = m_IpxCamera->getTriggerActivation();
		}

		//Local<Number> num = Number::New(isolate, result);
		args.GetReturnValue().Set(Number::New(isolate, result));
		
		
	}
	void MyObject::cameryCallback(IpxCam::Buffer* buffer)
	{
		//���ݷ�������У�Ȼ��Ӷ�����ȡ������ʾ
		try {
			auto imagePtr = static_cast<uchar*>(buffer->GetBufferPtr()) + buffer->GetImageOffset();
			
			specData * data_struct = new specData;
			data_struct->imagedata = new uint8_t[buffer->GetBufferSize()];
			memcpy(data_struct->imagedata, imagePtr, buffer->GetBufferSize());
			data_struct->bufSize = buffer->GetBufferSize();
			data_struct->Height = buffer->GetHeight();
			data_struct->Width = buffer->GetWidth();
			specDataTView.push(data_struct);

			if (is_save)
			{
				specData *data_struct_s = new specData;
				data_struct_s->imagedata = new uint8_t[buffer->GetBufferSize()];
				memcpy(data_struct_s->imagedata, imagePtr, buffer->GetBufferSize());
				data_struct_s->bufSize = buffer->GetBufferSize();
				data_struct_s->Height = buffer->GetHeight();
				data_struct_s->Width = buffer->GetWidth();
				specDataT.push(data_struct_s);
				//cv::imshow("image", image);
				//cv::waitKey(5);
			}
		}
		catch (...)
		{
			LogUtils::debugger->error("cameryCallback��������");
		}
		///////////////////////////////////////////////////////////////////////////////////////

		//try {
		//	g_asyncData.type = L"Frame";
		//	g_asyncData.message = L"Frame captured!";
		//	g_async.data = &g_asyncData;
		//	//����ͼ������
		//	auto imagePtr = static_cast<uchar*>(buffer->GetBufferPtr()) + buffer->GetImageOffset();
		//	cv::Mat image(buffer->GetHeight(), buffer->GetWidth(), CV_16UC1, imagePtr);
		//	//

		//	if (is_save)
		//	{
		//		specData data_struct;
		//		data_struct.imagedata = new uint8_t[buffer->GetBufferSize()];
		//		memcpy(data_struct.imagedata, image.data, buffer->GetBufferSize());
		//		data_struct.bufSize = buffer->GetBufferSize();
		//		data_struct.Height = buffer->GetHeight();
		//		data_struct.Width = buffer->GetWidth();
		//		specDataT.push(data_struct);
		//		//cv::imshow("image", image);
		//		//cv::waitKey(5);
		//	}

		//	//
		//	cv::Mat spectrum = image.colRange(spectrum_cols, spectrum_cols + 1).clone();

		//	//cv::imshow("image", image);
		//	if (current_col == 0)
		//	{
		//		//����һ����ɫͨ��ͼ��
		//		rgb_mat = cv::Mat(buffer->GetWidth(), rgb_mat_width, CV_16UC3, cv::Scalar(4095, 4095, 4095));
		//		current_col++;
		//	}
		//	for (int index = 0; index < image.cols; index++)
		//	{
		//		/*if (index < image.rows)
		//		{
		//			spectrumBuf[index] =image.at<ushort>(index, spectrum_cols);
		//		}*/
		//		//b g r 
		//		//rgb_mat.at<cv::Vec3s>(rows,cols)
		//		rgb_mat.at<cv::Vec3s>(index, current_col - 1)[0] = image.at<ushort>(b_row, index);//B
		//		rgb_mat.at<cv::Vec3s>(index, current_col - 1)[1] = image.at<ushort>(g_row, index);//G
		//		rgb_mat.at<cv::Vec3s>(index, current_col - 1)[2] = image.at<ushort>(r_row, index);//R
		//	}


		//	if (current_col < rgb_mat.cols)
		//	{
		//		current_col++;
		//	}
		//	else
		//	{
		//		//current_col = 1;
		//		//converted
		//		cv::Mat t_mat = cv::Mat::zeros(2, 3, CV_32FC1);

		//		t_mat.at<float>(0, 0) = 1;
		//		t_mat.at<float>(0, 2) = -1; //ˮƽƽ����
		//		t_mat.at<float>(1, 1) = 1;
		//		t_mat.at<float>(1, 2) = 0; //��ֱƽ����

		//		//����ƽ�ƾ�����з���任
		//		cv::warpAffine(rgb_mat, rgb_mat, t_mat, rgb_mat.size());

		//		t_mat.release();
		//	}
		//	//

		//	cv::Mat converted = rgb_mat.clone();

		//	cv::resize(converted, converted, cv::Size(640, 480));
		//	line(converted, cv::Point(630, spectrum_cols), cv::Point(640, spectrum_cols), cv::Scalar(80, 144, 255), 1);
		//	double g_MaxValue;
		//	cv::minMaxLoc(converted, 0, &g_MaxValue, 0, 0);
		//	(g_MaxValue > 4095) ? g_MaxValue = 4095 : g_MaxValue = g_MaxValue;
		//	converted.convertTo(converted, CV_8U, 255.0 / g_MaxValue);

		//	//printf("%f    %f\r\n", 255.0 / g_MaxValue,g_MaxValue);
		//	//cv::imshow("color_tobgra", converted);
		//	//cv::waitKey(5);
		//	cv::cvtColor(converted, converted, cv::COLOR_RGB2BGRA);

		//	//cv::imshow("color_tobgra", converted);
		//	//cv::waitKey(5);


		//	memcpy(g_frameBuffer, converted.data, BUFFER_SIZE);
		//	memcpy(spectrumBuf, spectrum.data, BUFFER_SPEC);
		//	uv_async_send(&g_async);


		//	spectrum.release();
		//	converted.release();
		//	image.release();
		//}
		//catch (...) {

		//}
		

		

	}
	void MyObject::wavelengthSwitch(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		if (args.Length() < 2)
		{
			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid param").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		if (!args[0]->IsString()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[str]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		if (!args[1]->IsNumber()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[num]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}

		v8::String::Utf8Value param_type(args[0]->ToString());
		std::string data_d = std::string(*param_type);
		int param_value = args[1].As<Number>()->Value();

		if (data_d.compare("r") == 0 || data_d.compare("R") == 0)
		{
			r_row = param_value;
		}
		else if (data_d.compare("g") == 0 || data_d.compare("G") == 0)
		{
			g_row = param_value;
		}
		else if (data_d.compare("b") == 0 || data_d.compare("B") == 0)
		{
			b_row = param_value;
		}
		else if (data_d.compare("spectrum") == 0)
		{
			spectrum_cols = param_value;
		}

		args.GetReturnValue().Set(String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"Update success").c_str(), NewStringType::kNormal).ToLocalChecked());


	}
	//�򿪴���
	void MyObject::openCmdSerialPort(const v8::FunctionCallbackInfo<v8::Value>& args)
	{

		Isolate* isolate = args.GetIsolate();
		if (args.Length() < 1) {
			// �׳�һ�����󲢴��ص� JavaScript��
			std::string error = "invalid parameter";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}


		

		//������ģ���Ƿ��Ѿ�����
		if (m_Serial != NULL && m_Serial->connect_status)
		{
			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! STM Device is already on.").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}

		
		
		//��ȡ�����豸�б�
		neb::CJsonObject list(EncodeUtils::getComMonDevices());
		bool isFind = false;
		int tmp_port = 0;
		std::string cmd = "AA38000000326A55";//����ָ��
		for (int index = 0; index < list["SerialInfo"].GetArraySize(); index++)
		{
			
			std::string serial_name;
			list["SerialInfo"][index].Get("info", serial_name);
			tmp_port = EncodeUtils::getUsartCom(serial_name);//ȡ�ô��ں�
			//printf("%s\r\n", serial_name.c_str());
			if (tmp_port == -1)
			{
				continue;
			}
			//�������ڲ���Ŀ���豸
			CSerialPort *m_Serial_q = new CSerialPort();
			if (m_Serial_q->InitPort(tmp_port, CBR_115200/*CBR_115200*/, 'N', 8, 1, EV_RXCHAR)) {
				//for (int repet=0; repet<10 && !isFind; repet++) {
				BYTE buf[512];
				int readLength = 512;
				//��������ָ��
				//printf("tmp_port=%d ����%s %d\r\n", tmp_port, cmd.c_str(), readLength);
				m_Serial_q->WriteDataHex((char*)cmd.c_str(), cmd.length());
				//ȡ������ָ��
				m_Serial_q->readDataAsyncHex(buf, readLength);
				//printf("tmp_port=%d ����%s %d\r\n", tmp_port, buf, readLength);
				if (readLength > 0) {
					//printf("%s %d\r\n", buf, readLength);
					std::string result((char*)buf);
					if (result.compare(cmd.c_str()) == 0) {
						if (m_Serial_q != NULL) {
							m_Serial_q->~CSerialPort();
							m_Serial_q = NULL;
						}
						//printf("�ҵ���\r\n");
						isFind = true;
						break;
					}
				}
				//}
				

			}
			if (m_Serial_q != NULL) {
				m_Serial_q->~CSerialPort();
				m_Serial_q = NULL;
			}
		}
		
		//printf("���ҽ���\r\n");
		if (!isFind) {
			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! STM Exception!").c_str(),					NewStringType::kNormal).ToLocalChecked()));						return;
		}
		//printf("���ҽ���555\r\n");
		//�Ѿ��ҵ�����Ҫ���ӵĴ��� tmp_port
		if (m_Serial == NULL) {
			m_Serial = new CSerialPort();
			if (m_Serial->InitPort(tmp_port, 115200/*CBR_115200*/, 'N', 8, 1, EV_RXCHAR)) {
				g_syscmd = false;
				uv_thread_create(&g_cmdCallbackThreadId, sysCmdHandle, nullptr);
				//�򿪿��ƴ��ڳɹ� ��Ҫ�豸�ص�����
				m_Serial->setCallbackFun(usartCallback);//���ûص�����
				m_Serial->OpenListenThread();//�򿪼���
				
				args.GetReturnValue().Set(String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"success").c_str(), NewStringType::kNormal).ToLocalChecked());
				return;
			}
			else
			{
				m_Serial->~CSerialPort();
				m_Serial = NULL;
				isolate->ThrowException(Exception::TypeError(					String::NewFromUtf8(isolate,						EncodeUtils::toUtf8String(L"error! can't connect to stm").c_str(),						NewStringType::kNormal).ToLocalChecked()));				return;
			}
		}


		//����ʧ��
		isolate->ThrowException(Exception::TypeError(			String::NewFromUtf8(isolate,				EncodeUtils::toUtf8String(L"error! Serial Unknown Exception!").c_str(),				NewStringType::kNormal).ToLocalChecked()));		return;

	}

	//�򿪴���
	void MyObject::openSerialPort(const v8::FunctionCallbackInfo<v8::Value>& args)
	{

		Isolate* isolate = args.GetIsolate();
		if (args.Length() < 1) {
			// �׳�һ�����󲢴��ص� JavaScript��
			std::string error = "invalid parameter";
			error = EncodeUtils::UnicodeToUtf8(EncodeUtils::StringToWString(error));
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,
					error.c_str(),
					NewStringType::kNormal).ToLocalChecked()));
			return;
		}

		
		if (!args[0]->IsNumber()) {			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! invalid params[number]").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		//gps�����Ƿ��Ѿ���
		if (m_gpsSerial != NULL && m_gpsSerial->connect_status)
		{
			isolate->ThrowException(Exception::TypeError(				String::NewFromUtf8(isolate,					EncodeUtils::toUtf8String(L"error! gps Device is already on.").c_str(),					NewStringType::kNormal).ToLocalChecked()));			return;
		}
		
		

		int gpsport = args[0].As<Number>()->Value();
		//printf("gpsport=%d ",gpsport);
		/*
		*1����gps�˿�
		*2���Զ����Ӵ������ƶ˿� ��õ�������źźͿ����ź�
		*/
		//��gps�˿�
		if (m_gpsSerial == NULL)
		{
			m_gpsSerial = new CSerialPort();
			//��gps�˿�
			gps_org_index = 0;
			gps_any_index = 0;
			if (m_gpsSerial->InitPort(gpsport, CBR_115200/*CBR_115200*/, 'N', 8, 1, EV_RXCHAR)) {
				//ע��ص����ڽ���gps��Ϣ
				m_gpsSerial->setCallbackFun(usartgpsCallback);
				//����һ�����������Ϣִ���߳�
				//printf("m_gpsSerial=%d ", gpsport);
				g_sysgps = false;
				//gpsFileWriteTest.open("./gpstest.txt", std::fstream::out | std::fstream::binary);
				
				uv_thread_create(&g_gpsCallbackThreadId, sysGpsHandle, nullptr);
				//�򿪼���
				m_gpsSerial->OpenListenThread();
				return;
			}
			else{
				m_gpsSerial->~CSerialPort();
				m_gpsSerial = NULL;
				isolate->ThrowException(Exception::TypeError(					String::NewFromUtf8(isolate,						EncodeUtils::toUtf8String(L"error! can't connect to gps").c_str(),						NewStringType::kNormal).ToLocalChecked()));				return;
			}
		}
		
		//����ʧ��
		isolate->ThrowException(Exception::TypeError(			String::NewFromUtf8(isolate,				EncodeUtils::toUtf8String(L"error! Serial Unknown Exception!").c_str(),				NewStringType::kNormal).ToLocalChecked()));		return;
	
	}
	
	void MyObject::usartgpsCallback(char data){
		/*��������gps�忨���ư����Ϣ*/
		/*��Ϣ�洢������Ŀ¼*/

		if (is_save_gps) {
			gps_org[gps_org_index] = data;
			if (gps_org_index == GPS_BUF_SIZE - 1)
			{


				//gpsFileWrite.write(gps_org, GPS_BUF_SIZE);
				fwrite(gps_org, GPS_BUF_SIZE, 1, gpsFile);
				gps_org_index = 0;
				return;
			}
			gps_org_index++;
		}
		//
		gps_any[gps_any_index] = data;
		if (gps_any_index == GPS_BUF_SIZE - 1)
		{
			gps_any_index = 0;
		}
		gps_any_index++;

		//if (is_save_gps)
		//{
		//	gps_data_save.push(data);
		//	
		//	if (gps_data_save.size() > GPS_ANY_SAVE)
		//	{
		//		std::string result = "";
		//		for (int index = 0; index < GPS_ANY_SAVE; index++)
		//		{
		//			
		//			result += gps_data_save.front();
		//			gps_data_save.pop();
		//		}
		//		gpsFileWrite.write(result.c_str(), result.length());
		//		//gpsFileWrite.flush();
		//	}
		//	
		//}

		//������Ҫ����  ���IMU����״̬ �� RTKʵ��״̬
		//gps_data.push(data);
		//printf("%c\r\n", data);
	}

	void MyObject::usartCallback(char data)
	{
		/*��������STM32���ư����Ϣ*/
		/*��Ϣ����ϵͳ����*/
		//printf("��Ϣ����ϵͳ���� \r\n");
		try
		{
			sprintf_s(buf2hex, "%02X", (BYTE)data);
			//printf("%s ", buf2hex);
			recvresult.append(buf2hex);
			//printf("%s ", recvresult.c_str());
			//printf("%s %d %d %d\r\n", recvresult.c_str(), recvresult.find("AA"), recvresult.substr(recvresult.length() - 2, 2).compare("55"), recvresult.length());

			if ((recvresult.find("AA") != 0) && (recvresult.length() >= 2))
			{
				//printf("recvresult=""\r\n");
				recvresult = "";
				//printf("recvresult = """);
			}
			/*if (recvresult.length() == 2 && recvresult.compare("AA") != 0)
			{
				recvresult = "";
			}*/
			else if (recvresult.find("AA") == 0 /*��AA��ͷ*/
				&& recvresult.substr(recvresult.length() - 2, 2).compare("55") == 0 /*��55����*/
				&& (recvresult.length() == 16)/*ָ���*/)
			{
				//�õ�ָ�����У����Ƿ���ȷ
				//printf("��ȷָ��\r\n");
				//AA 37 00 00 00 00 37 55
				if (EncodeUtils::getCheck_ConnectCmd((char*)recvresult.c_str(), recvresult.length())) {
					//У�����ȷ
					//printf("��ȷָ��--%s \r\n", recvresult.c_str());
					//ָ����Ҫ������Ӧ����
					//ָ���������� ���̸߳������  ͬʱ����㲥
					rev_cmd.push(recvresult);
				}
				recvresult = "";
			}
			else if (recvresult.length() > 16)
			{
				recvresult = "";
			}
		}
		catch (...) {

		}
		
	}
	void MyObject::closeSerialPort(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		if (m_gpsSerial != NULL)
		{
			g_sysgps = true;
			uv_thread_join(&g_gpsCallbackThreadId);


			m_gpsSerial->~CSerialPort();
			m_gpsSerial = NULL;
			args.GetReturnValue().Set(String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"success!").c_str(), NewStringType::kNormal).ToLocalChecked());
			return;
		}
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"error! Device not open.").c_str(), NewStringType::kNormal).ToLocalChecked());
	}

	void MyObject::closeCmdSerialPort(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		if (m_Serial != NULL)
		{
			g_syscmd = true;
			uv_thread_join(&g_cmdCallbackThreadId);

			m_Serial->~CSerialPort();
			m_Serial = NULL;
			args.GetReturnValue().Set(String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"success!").c_str(), NewStringType::kNormal).ToLocalChecked());
			//printf("closeCmdSerialPort\r\n");
			return;
		}

		args.GetReturnValue().Set(String::NewFromUtf8(isolate, EncodeUtils::toUtf8String(L"error! Device not open.").c_str(), NewStringType::kNormal).ToLocalChecked());
	}
	
	void MyObject::takePhoto(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		Isolate* isolate = args.GetIsolate();
		
		//����
		//cv::write("", matimg);
		FocusElement * element = new FocusElement();
		element->work.data = element;
		element->type = -1;
		element->callback.Reset(isolate, Local<Function>::Cast(args[0]));
		m_takePhoto = true;
		uv_queue_work(uv_default_loop(), &element->work, startTakephoto, TakephotoCompleted);

	}

	void MyObject::startTakephoto(uv_work_t * work)
	{
		FocusElement * element = (FocusElement*)work->data;
		while (m_takePhoto)
		{
			Sleep(100);
		}
		vector <int > compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(0);
		CTime t = CTime::GetCurrentTime();
		char buf[128];
		sprintf(buf,"./qhyy_data/%04d%02d%02d%02d%02d%02d.png", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		std::string path = buf;
		EncodeUtils::createDirectory(path);
		cv::imwrite(path.c_str(), save_image, compression_params);
		save_image.release();
		element->type = 100;
	}
	void MyObject::TakephotoCompleted(uv_work_t * work, int status)
	{
		FocusElement * element = (FocusElement*)work->data;
		Isolate * isolate = Isolate::GetCurrent();
		const unsigned argc = 1;
		Local<Value> argv[argc] = { String::NewFromUtf8(isolate, std::to_string(element->type).c_str()).ToLocalChecked() };
		Local<Function>::New(isolate, element->callback)->Call(isolate->GetCurrentContext(),isolate->GetCurrentContext()->Global(), argc, argv);
		element->callback.Reset();
		delete element;
	}

	void MyObject::createFrameBuffer(const v8::FunctionCallbackInfo<v8::Value>& args) {
		
		Isolate *  isolate = args.GetIsolate();
		v8::Local<v8::SharedArrayBuffer>  buffer = v8::SharedArrayBuffer::New(isolate, g_frameBuffer, BUFFER_SIZE);
		v8::Local<v8::Uint8ClampedArray> array = v8::Uint8ClampedArray::New(buffer, 0, BUFFER_SIZE);
		args.GetReturnValue().Set(array);

	}

	void MyObject::createSpecBuffer(const v8::FunctionCallbackInfo<v8::Value>& args) {

		Isolate *  isolate = args.GetIsolate();
		v8::Local<v8::SharedArrayBuffer> spbuffer = v8::SharedArrayBuffer::New(isolate, spectrumBuf, BUFFER_SPEC);
		v8::Local<v8::Uint16Array> spbuffdata = v8::Uint16Array::New(spbuffer, 0, 270);

		//v8::Local<v8::SharedArrayBuffer>  buffer = v8::SharedArrayBuffer::New(isolate, spectrumBuf, BUFFER_SPEC);
		//v8::Local<v8::Uint8ClampedArray> array = v8::Uint8ClampedArray::New(buffer, 0, 270);
		args.GetReturnValue().Set(spbuffdata);

	}
	

	void MyObject::startSendCommand(uv_work_t * work)
	{
		FocusElement * element = (FocusElement*)work->data;


		/*double _times = get_wall_time();
		while (true && element->starTimeout != 0) {
			double _timee = get_wall_time();
			if ((_timee - _times) >= element->starTimeout)
			{
				break;
			}
			Sleep(100);
		}*/
		printf("A element->starTimeout = %d\r\n", element->starTimeout);
		Sleep(element->starTimeout);
		printf("A element->starTimeout = %d\r\n", element->starTimeout);
		if (m_Serial != NULL) {
			m_Serial->WriteDataHex((char*)element->cmd.c_str(), element->cmd.length());
		}
		printf("B element->endTimeout = %d\r\n", element->endTimeout);
		Sleep(element->endTimeout);
		printf("B element->endTimeout = %d\r\n", element->endTimeout);
		
		
		/*_times = get_wall_time();
		while (true && element->endTimeout != 0) {
			double _timee = get_wall_time();
			if ((_timee - _times) >= element->endTimeout)
			{
				break;
			}
			Sleep(100);
		}*/
		element->type = 200;
	}
	void MyObject::SendCommandCompleted(uv_work_t * work, int status)
	{
		FocusElement * element = (FocusElement*)work->data;
		Isolate * isolate = Isolate::GetCurrent();
		const unsigned argc = 1;
		Local<Value> argv[argc] = { String::NewFromUtf8(isolate, std::to_string(element->type).c_str()).ToLocalChecked() };
		Local<Function>::New(isolate, element->callback)->Call(isolate->GetCurrentContext(), isolate->GetCurrentContext()->Global(), argc, argv);
		element->callback.Reset();
		delete element;
	}
	DWORD MyObject::HEXS(char *decString){
		DWORD hexValue = 0;
		DWORD sl = 0;
		BOOL isWhile = FALSE;
		DWORD idx = 0;
		char str[256];
		BYTE ct = 0;

		sl = strlen(decString);
		if ((sl > 0) && (sl < 256))
			isWhile = TRUE;
		strcpy(str, decString);
		idx = 0;
		while (isWhile)
		{
			ct = (BYTE)(str[idx]);
			switch (ct)
			{
			case 48: // "0"
				hexValue = hexValue << 4;
				hexValue += 0x0;
				break;
			case 49: // "1"
				hexValue = hexValue << 4;
				hexValue += 0x01;
				break;
			case 50: // "2"
				hexValue = hexValue << 4;
				hexValue += 0x02;
				break;
			case 51: // "3"
				hexValue = hexValue << 4;
				hexValue += 0x03;
				break;
			case 52: // "4"
				hexValue = hexValue << 4;
				hexValue += 0x04;
				break;
			case 53: // "5"
				hexValue = hexValue << 4;
				hexValue += 0x05;
				break;
			case 54: // "6"
				hexValue = hexValue << 4;
				hexValue += 0x06;
				break;
			case 55: // "7"
				hexValue = hexValue << 4;
				hexValue += 0x07;
				break;
			case 56: // "8"
				hexValue = hexValue << 4;
				hexValue += 0x08;
				break;
			case 57: // "9"
				hexValue = hexValue << 4;
				hexValue += 0x09;
				break;
			case 65: // "A"
				hexValue = hexValue << 4;
				hexValue += 0x0a;
				break;
			case 97: // "a"
				hexValue = hexValue << 4;
				hexValue += 0x0a;
				break;
			case 66: // "B"
				hexValue = hexValue << 4;
				hexValue += 0x0b;
				break;
			case 98: // "b"
				hexValue = hexValue << 4;
				hexValue += 0x0b;
				break;
			case 67: // "C"
				hexValue = hexValue << 4;
				hexValue += 0x0c;
				break;
			case 99: // "c"
				hexValue = hexValue << 4;
				hexValue += 0x0c;
				break;
			case 68: // "D"
				hexValue = hexValue << 4;
				hexValue += 0x0d;
				break;
			case 100: // "d"
				hexValue = hexValue << 4;
				hexValue += 0x0d;
				break;
			case 69: // "E"
				hexValue = hexValue << 4;
				hexValue += 0x0e;
				break;
			case 101: // "e"
				hexValue = hexValue << 4;
				hexValue += 0x0e;
				break;
			case 70: // "F"
				hexValue = hexValue << 4;
				hexValue += 0x0f;
				break;
			case 102: // "f"
				hexValue = hexValue << 4;
				hexValue += 0x0f;
				break;
			default: //unknown char
				isWhile = FALSE;
				break;
			}
			idx++;
			if (idx >= sl)
				isWhile = FALSE;
		}
		/*if (str)
		{
			delete[] str;
		}*/


		return  hexValue;
	}
	int MyObject::recheckSum(std::string data) {
		//printf("check -001\r\n");
		int sum = 0;
		char tmp[3];



		tmp[0] = data.at(2);
		tmp[1] = data.at(3);
		tmp[2] = '\0';
		DWORD value = HEXS(tmp);
		sum += value;
		tmp[0] = data.at(4);
		tmp[1] = data.at(5);
		tmp[2] = '\0';
		value = HEXS(tmp);
		sum += value;
		tmp[0] = data.at(6);
		tmp[1] = data.at(7);
		tmp[2] = '\0';
		value = HEXS(tmp);
		sum += value;
		tmp[0] = data.at(8);
		tmp[1] = data.at(9);
		tmp[2] = '\0';
		value = HEXS(tmp);
		sum += value;
		tmp[0] = data.at(10);
		tmp[1] = data.at(11);
		tmp[2] = '\0';
		value = HEXS(tmp);
		sum += value;
		sum &= 0x00ff;
		return sum;
	}
	char *MyObject::getFormatStringForCan(int num, char*cmdhead) {
		char *result = new char[128];
		memset(result, '\0', 128);
		sprintf(result, "%s", cmdhead);
		char buff[16];
		memset(buff, '\0', 16);



		int value = (num >> 24) & 0x00FF;
		sprintf(buff, "%02X", value);
		sprintf(result, "%s%s", result, buff);
		memset(buff, '\0', 16);

		value = (num >> 16) & 0x00FF;
		sprintf(buff, "%02X", value);
		sprintf(result, "%s%s", result, buff);
		memset(buff, '\0', 16);

		value = (num >> 8) & 0x00FF;
		sprintf(buff, "%02X", value);
		sprintf(result, "%s%s", result, buff);
		memset(buff, '\0', 16);



		value = num & 0x00FF;
		sprintf(buff, "%02X", value);
		sprintf(result, "%s%s", result, buff);

		memset(buff, '\0', 16);


		//����У���
		value = recheckSum(result);
		sprintf(buff, "%02X", value);
		sprintf(result, "%s%s%s", result, buff, "BA");


		return result;
	}
	
	
	
}  // namespace demo

