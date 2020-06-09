#include "SerialUtils.h"
#include <stdio.h>
#include <process.h>

namespace ASYNCSERIAL {
	const UINT SLEEP_TIME_INTERVAL = 5;
	AsyncSerial::AsyncSerial():m_hCom(INVALID_HANDLE_VALUE)
		, m_IsOpen(false), m_Thread(INVALID_HANDLE_VALUE), m_IsRead(false)
	{
		memset(&m_ovWait, 0, sizeof(m_ovWait));
		memset(&m_ovWrite, 0, sizeof(m_ovWrite));
		memset(&m_ovRead, 0, sizeof(m_ovRead));

	}

	AsyncSerial::~AsyncSerial()
	{
	}
	bool AsyncSerial::Isopen() {
		return m_IsOpen;
	}
	bool AsyncSerial::InitCOM(int Port,int buad) {
		if (!openSerialPort(Port)) {
			return false;
		}
		SetupComm(m_hCom, 4096, 4096);//设置发送接收缓存
		
		DCB dcb;
		GetCommState(m_hCom, &dcb);
		dcb.DCBlength = sizeof(dcb);
		dcb.BaudRate = buad;
		dcb.Parity = 0;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;

		SetCommState(m_hCom, &dcb);//配置串口
		
		PurgeComm(m_hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);

		COMMTIMEOUTS ct;
		ct.ReadIntervalTimeout = MAXDWORD;//读取无延时，因为有WaitCommEvent等待数据
		ct.ReadTotalTimeoutConstant = 0;  //
		ct.ReadTotalTimeoutMultiplier = 0;//

		ct.WriteTotalTimeoutMultiplier = 500;
		ct.WriteTotalTimeoutConstant = 5000;

		SetCommTimeouts(m_hCom, &ct);


		//创建事件对象
		m_ovRead.hEvent = CreateEvent(NULL, false, false, NULL);
		m_ovWrite.hEvent = CreateEvent(NULL, false, false, NULL);
		m_ovWait.hEvent = CreateEvent(NULL, false, false, NULL);

		SetCommMask(m_hCom, EV_ERR | EV_RXCHAR);//设置接受事件
		//创建读取线程
		//m_Thread = (HANDLE)_beginthreadex(NULL, 0, &AsyncSerial::OnRecv, this, 0, NULL);
		m_IsOpen = true;
		return true;
	}
	
	bool AsyncSerial::openSerialPort(int portNo) {

		char szPort[50];
		sprintf_s(szPort, "\\\\.\\COM%d", portNo);
		m_hCom = CreateFileA(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_NORMAL,//设置异步标识
			NULL);
		if (INVALID_HANDLE_VALUE == m_hCom)
		{
			return false;
		}
		return true;
	}
	
	bool AsyncSerial::OpenListenThread(listenCallback m_Callback) {
		/** 串口是否已经打开 */
		if (!m_IsOpen) {
			return false;
		}
		/** 检测线程是否已经开启了 */
		if (m_Thread != INVALID_HANDLE_VALUE)
		{
			/** 线程已经开启 */
			return false;
		}
		m_Thread = (HANDLE)_beginthreadex(NULL, 0, &AsyncSerial::OnRecv, this, 0, NULL);
		if (!m_Thread)
		{
			return false;
		}
		/** 设置线程的优先级,高于普通线程 */
		if (!SetThreadPriority(m_Thread, THREAD_PRIORITY_HIGHEST))
		{
			return false;
		}


		this->m_Callback = m_Callback;
		m_IsRead = true;
		return true;
	}
	bool AsyncSerial::CloseListenTread()
	{
		
		/** 检测线程是否已经开启了 */
		if (m_Thread == INVALID_HANDLE_VALUE)
		{
			/** 线程已经开启 */
			return false;
		}
		CloseListenTread();
		m_IsRead = false;
		/** 等待线程退出 */
		WaitForSingleObject(m_Thread, 5000);//等待线程结束
		/** 置线程句柄无效 */
		CloseHandle(m_Thread);
		m_Thread = INVALID_HANDLE_VALUE;
		m_Callback = NULL;
	}
	void AsyncSerial::UninitCOM()
	{
		m_IsOpen = false;
		if (INVALID_HANDLE_VALUE != m_hCom)
		{
			CloseHandle(m_hCom);
			m_hCom = INVALID_HANDLE_VALUE;
		}
		if (NULL != m_ovRead.hEvent)
		{
			CloseHandle(m_ovRead.hEvent);
			m_ovRead.hEvent = NULL;
		}
		if (NULL != m_ovWrite.hEvent)
		{
			CloseHandle(m_ovWrite.hEvent);
			m_ovWrite.hEvent = NULL;
		}
		if (NULL != m_ovWait.hEvent)
		{
			CloseHandle(m_ovWait.hEvent);
			m_ovWait.hEvent = NULL;
		}
		
	}
	int AsyncSerial::ComWriteHex(const char * buf) {
		BOOL rtn = FALSE;
		DWORD WriteSize = 0;
		PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_TXABORT);
		m_ovWait.Offset = 0;
		//修改为十六进制数据
		//空格去除
		BYTE *inputData =  hexStr2Bytes((char*)buf);
		if (inputData == NULL) {
			return FALSE;
		}
		rtn = WriteFile(m_hCom, inputData, strlen(buf)/2, &WriteSize, &m_ovWrite);
		delete[] inputData;
		inputData = NULL;
		//rtn = WriteFile(m_hCom, buf, len, &WriteSize, &m_ovWrite);
		int len = 0;
		if (FALSE == rtn && GetLastError() == ERROR_IO_PENDING)//后台读取
		{
			//等待数据写入完成
			if (FALSE == ::GetOverlappedResult(m_hCom, &m_ovWrite, &WriteSize, TRUE))
			{
				return false;
			}
		}

		len = WriteSize;
		return rtn != FALSE;
	}

	bool AsyncSerial::ComWrite(LPBYTE buf, int &len) {
		BOOL rtn = FALSE;
		DWORD WriteSize = 0;
		PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_TXABORT);
		m_ovWait.Offset = 0;
		rtn = WriteFile(m_hCom, buf, len, &WriteSize, &m_ovWrite);
		len = 0;
		if (FALSE == rtn && GetLastError() == ERROR_IO_PENDING)//后台读取
		{
			//等待数据写入完成
			if (FALSE == ::GetOverlappedResult(m_hCom, &m_ovWrite, &WriteSize, TRUE))
			{
				return false;
			}
		}

		len = WriteSize;
		return rtn != FALSE;
	}
	/*接收回调函数*/
	unsigned int __stdcall AsyncSerial::OnRecv(void* LPParam) 
	{
		/** 得到本类的指针 */
		AsyncSerial *obj = reinterpret_cast<AsyncSerial*>(LPParam);
		DWORD WaitEvent = 0, Bytes = 0;
		BOOL Status = FALSE;
		//BYTE ReadBuf[4096];
		DWORD Error;
		COMSTAT cs = { 0 };

		while (obj->m_IsRead) {
			WaitEvent = 0;
			obj->m_ovWait.Offset = 0;
			Status = WaitCommEvent(obj->m_hCom, &WaitEvent, &obj->m_ovWait);
			//BYTE ReadBuf[4096];
			//WaitCommEvent也是一个异步命令，所以需要等待
			if (FALSE == Status && GetLastError() == ERROR_IO_PENDING)//
			{
				//如果缓存中无数据线程会停在此，如果hCom关闭会立即返回False
				Status = GetOverlappedResult(obj->m_hCom, &obj->m_ovWait, &Bytes, TRUE);
			}
			ClearCommError(obj->m_hCom, &Error, &cs);
			if (TRUE == Status //等待事件成功
				&& WaitEvent&EV_RXCHAR//缓存中有数据到达
				&& cs.cbInQue > 0)//有数据
			{
				BYTE *ReadBuf = new BYTE[cs.cbInQue];
				if (ReadBuf == NULL) {
					continue;
				}
				Bytes = 0;
				obj->m_ovRead.Offset = 0;
				memset(ReadBuf, 0, cs.cbInQue);
				//printf("%d\r\n", cs.cbInQue);
				//数据已经到达缓存区，ReadFile不会当成异步命令，而是立即读取并返回True
				Status = ReadFile(obj->m_hCom, ReadBuf, cs.cbInQue, &Bytes, &obj->m_ovRead);
				if (Status != FALSE)
				{
					/*printf("Bytes:%d", Bytes);
					ReadBuf[Bytes] = '\0';
					printf("%s", ReadBuf);*/
					/*for (int index = 0; index < cs.cbInQue; index++) {
						printf("%c", ReadBuf[index]);
					}*/
					if (obj->m_Callback != NULL) {
						obj->m_Callback(ReadBuf, Bytes);
					}
					//std::cout << "Read:" << ReadBuf << "   Len:" << Bytes << std::endl;
				}
				else {
					PurgeComm(obj->m_hCom, PURGE_RXCLEAR | PURGE_RXABORT);
				}
				
				delete []ReadBuf;
				ReadBuf = NULL;
			}
			else
			{
				
				Sleep(SLEEP_TIME_INTERVAL);
				continue;
			}
		}
		return 0;
	}
	
	BYTE* AsyncSerial::hexStr2Bytes(std::string src)
	{
		char *strEnd;
		int m = 0;
		int len = src.length() / 2;
		BYTE* ret = new BYTE[len];
		if (NULL == ret) {
			return NULL;
		}
		for (int i = 0; i < len; i++)
		{
			m = i * 2;
			std::string subs = src.substr(m, 2);
			ret[i] = strtol(subs.c_str(), &strEnd, 16);
		}
		return ret;
	}
	
	void AsyncSerial::Bytes2HexStr(BYTE *src, int srcLen, BYTE *des)
	{
		BYTE *res;
		int i = 0;

		res = des;
		while (srcLen > 0)
		{
			sprintf((char*)(res + i * 2), "%02X", *(src + i));
			i++;
			srcLen--;
		}
	}
	
};

namespace SYNCSERIAL {

	SyncSerial::SyncSerial():m_hCom(INVALID_HANDLE_VALUE)
		, m_IsOpen(false), m_IsRead(false)
	{
		InitializeCriticalSection(&m_cs);
	}

	SyncSerial::~SyncSerial()
	{
		DeleteCriticalSection(&m_cs);
	}
	bool SyncSerial::openSerialPort(int portNo) {

		char szPort[50];
		sprintf_s(szPort, "\\\\.\\COM%d", portNo);
		m_hCom = CreateFileA(szPort,
			GENERIC_READ | GENERIC_WRITE, 
			0, 
			NULL,
			OPEN_EXISTING,
			0/*同步方式打开串口*/,
			NULL);
		
		if (INVALID_HANDLE_VALUE == m_hCom)
		{
			return false;
		}
		return true;
	}


	bool SyncSerial::InitCOM(int Port, int buad) {
		if (!openSerialPort(Port)) {
			return false;
		}
		SetupComm(m_hCom, 4096, 4096);//设置发送接收缓存

		
		DCB dcb;

		GetCommState(m_hCom, &dcb);//设置串口
		//BuildCommDCBA(szDCBparam, &dcb);

		dcb.DCBlength = sizeof(dcb);
		dcb.BaudRate = buad;
		dcb.Parity = 0;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		//dcb.fRtsControl = RTS_CONTROL_ENABLE;
		SetCommState(m_hCom, &dcb);

		PurgeComm(m_hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);//清空缓存

		COMMTIMEOUTS ct;
		//设置读取超时时间，及ReadFlie最长等待时间
		ct.ReadIntervalTimeout = 0;
		ct.ReadTotalTimeoutConstant = 100;
		ct.ReadTotalTimeoutMultiplier = 200;

		ct.WriteTotalTimeoutMultiplier = 500;
		ct.WriteTotalTimeoutConstant = 5000;

		SetCommTimeouts(m_hCom, &ct);//设置超时
		
		m_IsOpen = true;
		return true;
	}
	void SyncSerial::UninitCOM()
	{
		m_IsOpen = false;
		if (INVALID_HANDLE_VALUE != m_hCom)
		{
			CloseHandle(m_hCom);
			m_hCom = INVALID_HANDLE_VALUE;
		}
	}


	bool SyncSerial::ComRead(LPBYTE buf, int &len) {
		DWORD ReadSize = 0;
		BOOL rtn = FALSE;

		//设置读取1个字节数据，当缓存中有数据到达时则会立即返回，否则直到超时
		rtn = ReadFile(m_hCom, buf, 1, &ReadSize, NULL);

		//如果是超时rtn=true但是ReadSize=0，如果有数据到达，会读取一个字节ReadSize=1
		if (rtn == TRUE && 1 == ReadSize)
		{
			DWORD Error;
			COMSTAT cs = { 0 };
			int ReadLen = 0;
			//查询剩余多少字节未读取，存储于cs.cbInQue中
			ClearCommError(m_hCom, &Error, &cs);
			ReadLen = (cs.cbInQue > len) ? len : cs.cbInQue;
			if (ReadLen > 0)
			{
				//由于之前等待时以读取一个字节，所欲buf+1
				rtn = ReadFile(m_hCom, buf + 1, ReadLen, &ReadSize, NULL);
				len = 0;
				if (rtn)
				{
					len = ReadLen + 1;
				}
			}
		}
		PurgeComm(m_hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);
		return (rtn != FALSE)&& ReadSize;
	}

	bool SyncSerial::ComWrite(LPBYTE buf, int &len) {
		PurgeComm(m_hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);
		BOOL rtn = FALSE;
		DWORD WriteSize = 0;
		rtn = WriteFile(m_hCom, buf, len, &WriteSize, NULL);
		len = WriteSize;
		return rtn != FALSE;
	}

	bool SyncSerial::ComWriteHex(const char* buf) {
		PurgeComm(m_hCom, PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT);
		BOOL rtn = FALSE;
		DWORD WriteSize = 0;
		BYTE *inputData = hexStr2Bytes((char*)buf);
		if (inputData == NULL) {
			return FALSE;
		}
		rtn = WriteFile(m_hCom, inputData, strlen(buf)/2, &WriteSize, NULL);
		delete[] inputData;
		inputData = NULL;
		//int len = WriteSize;
		return rtn != FALSE;
	}
	void SyncSerial::Lock() 
	{
		EnterCriticalSection(&m_cs);
	}
	void SyncSerial::Unlock() 
	{
		LeaveCriticalSection(&m_cs);
	}

	BYTE* SyncSerial::hexStr2Bytes(std::string src)
	{
		char *strEnd;
		int m = 0;
		int len = src.length() / 2;
		BYTE* ret = new BYTE[len];
		if (NULL == ret) {
			return NULL;
		}
		for (int i = 0; i < len; i++)
		{
			m = i * 2;
			std::string subs = src.substr(m, 2);
			ret[i] = strtol(subs.c_str(), &strEnd, 16);
		}
		return ret;
	}

	void SyncSerial::Bytes2HexStr(BYTE *src, int srcLen, BYTE *des)
	{
		BYTE *res;
		int i = 0;

		res = des;
		while (srcLen > 0)
		{
			sprintf((char*)(res + i * 2), "%02X", *(src + i));
			i++;
			srcLen--;
		}
	}

};
