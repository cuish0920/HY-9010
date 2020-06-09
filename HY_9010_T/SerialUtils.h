#pragma once
#ifndef ASYNC_SERIAL
#define ASYNC_SERIAL
#include <Windows.h>
#include <string>
/*异常串口*/
namespace ASYNCSERIAL {
	typedef void(*listenCallback)(BYTE* data, int Length);
	class AsyncSerial
	{
	public:
		AsyncSerial();
		~AsyncSerial();
	public:
		bool InitCOM(int Port,int buad);//打开窗口
		bool ComWrite(LPBYTE buf, int &len);
		int ComWriteHex(const char * buf);
		void UninitCOM();
		bool OpenListenThread(listenCallback m_Callback);
		bool CloseListenTread();
		bool Isopen();
	public:
		BYTE* hexStr2Bytes(std::string src);
		void Bytes2HexStr(BYTE *src, int srcLen, BYTE *des);
	private:
		listenCallback m_Callback;
		bool openSerialPort(int Port);
		static unsigned int __stdcall OnRecv(void* LPParam);
	private:
		HANDLE m_hCom;
		OVERLAPPED m_ovWrite;//用于写入数据
		OVERLAPPED m_ovRead;//用于读取数据
		OVERLAPPED m_ovWait;//用于等待数据
		volatile bool m_IsOpen;//串口是否打开
		volatile bool m_IsRead;//串口是否打开
		HANDLE m_Thread;//读取线程句柄
	private:

	};

};

namespace SYNCSERIAL {
	class SyncSerial
	{
		public:
			SyncSerial();
			~SyncSerial();
		public:
			bool InitCOM(int Port, int buad);//打开窗口
			void UninitCOM();
			bool ComRead(LPBYTE buf, int &len);

			bool ComWrite(LPBYTE buf, int &len);
			bool ComWriteHex(const char* buf);
			void Lock();
			void Unlock();
			BYTE* hexStr2Bytes(std::string src);
			void Bytes2HexStr(BYTE *src, int srcLen, BYTE *des);
		private:
			bool openSerialPort(int Port);
			
			
		private:
			volatile bool m_IsOpen;//串口是否打开
			volatile bool m_IsRead;//串口是否打开
			HANDLE m_hCom;
			CRITICAL_SECTION m_cs;//
	};

	
};

#endif
