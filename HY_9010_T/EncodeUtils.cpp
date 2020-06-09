#include "EncodeUtils.h"
#include "LogUtils.h"
#include <windows.h>
#include <io.h>
#include <direct.h> 
#define MAX_PATH_LEN 256
#define ACCESS(fileName,accessMode) _access(fileName,accessMode)
#define MKDIR(path) _mkdir(path)

//////////////////外设检测串口///////////////////
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")
#include <devguid.h>
//////////////////外设检测串口///////////////////
//////////////////引入json///////////////////
#include "CJsonObject.h"
//////////////////引入json///////////////////
std::wstring EncodeUtils::Utf8ToUnicode(std::string &strUTF8)
{
	try {
		int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
		if (len == 0)
		{
			return L"";
		}

		wchar_t *pRes = new wchar_t[len];
		if (pRes == NULL)
		{
			return L"";
		}

		MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, pRes, len);
		pRes[len - 1] = L'\0';
		std::wstring result = pRes;
		delete[] pRes;
		pRes = NULL;

		return result;
	}
	catch (...) {
		LogUtils::debugger->error("error[Utf8ToUnicode]");
		//printf("error[Utf8ToUnicode]\r\n");
	}
	
}

std::string EncodeUtils::UnicodeToUtf8(std::wstring &strUnicode)
{
	try {
		int len = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
		if (len == 0)
		{
			return "";
		}

		char *pRes = new char[len];
		if (pRes == NULL)
		{
			return "";
		}

		WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, pRes, len, NULL, NULL);
		pRes[len - 1] = '\0';
		std::string result = pRes;
		delete[] pRes;
		pRes = NULL;

		return result;
	}
	catch (...) {
		LogUtils::debugger->error("error[UnicodeToUtf8]");
		//printf("error[UnicodeToUtf8]\r\n");
	}
	
}

std::wstring EncodeUtils::StringToWString(std::string &str)
{
	try {
		int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		if (len == 0)
		{
			return L"";
		}

		wchar_t *pRes = new wchar_t[len];
		if (pRes == NULL)
		{
			return L"";
		}

		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pRes, len);
		pRes[len - 1] = L'\0';
		std::wstring result = pRes;
		delete[] pRes;
		pRes = NULL;

		return result;
	}
	catch (...) {
		LogUtils::debugger->error("error[StringToWString]");
		//printf("error[StringToWString]\r\n");
	}
	
}

std::string EncodeUtils::WStringToString(std::wstring &wstr)
{
	try {
		int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
		if (len == 0)
		{
			return "";
		}

		char *pRes = new char[len];
		if (pRes == NULL)
		{
			return "";
		}

		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pRes, len, NULL, NULL);
		pRes[len - 1] = '\0';
		std::string result = pRes;
		delete[] pRes;
		pRes = NULL;
		return result;
	}
	catch (...) {
		LogUtils::debugger->error("error[WStringToString]");
		//printf("error[WStringToString]\r\n");
	}
	
}
std::string EncodeUtils::toUtf8String(const wchar_t * const str)
{
	try {
		auto length = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
		std::unique_ptr<char[]> buffer{ new char[length]() };
		WideCharToMultiByte(CP_UTF8, 0, str, -1, buffer.get(), length, NULL, NULL);

		return std::string(buffer.get(), length);
	}
	catch (...) {
		LogUtils::debugger->error("error[toUtf8String]");
		//printf("error[toUtf8String]");
	}
	
}

int32_t EncodeUtils::createDirectory(const std::string &directoryPath)
{
	uint32_t dirPathLen = directoryPath.length();
	if (dirPathLen > MAX_PATH_LEN)
	{
		return -1;
	}
	char tmpDirPath[MAX_PATH_LEN] = { 0 };
	for (uint32_t i = 0; i < dirPathLen; ++i)
	{
		tmpDirPath[i] = directoryPath[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
		{
			if (ACCESS(tmpDirPath, 0) != 0)
			{
				int32_t ret = MKDIR(tmpDirPath);
				if (ret != 0)
				{
					return ret;
				}
			}
		}
	}
	return 0;
}

std::string EncodeUtils::getComMonDevices()
{
	//CStringArray   *usartList = new CStringArray();

	neb::CJsonObject jqSerialInfo;
	jqSerialInfo.AddEmptySubArray("SerialInfo");

	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD DeviceIndex;
	// Create a HDEVINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs((LPGUID)&GUID_DEVCLASS_PORTS, 0, 0, DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		DWORD ErrorCode = GetLastError();
		return "";
	}
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (DeviceIndex = 0; SetupDiEnumDeviceInfo(hDevInfo, DeviceIndex, &DeviceInfoData); DeviceIndex++)
	{
		DWORD DataPropertyType;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;
		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_FRIENDLYNAME,
			&DataPropertyType,
			(PBYTE)buffer,
			buffersize,
			&buffersize))

		{
			DWORD ErrorCode = GetLastError();
			if (ErrorCode == ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer) LocalFree(buffer);
				buffer = (LPTSTR)LocalAlloc(LPTR, buffersize);
			}
			else
			{
				break;
			}

		}
		//CString forMatcom = CString(buffer);
		//usartList->Add(forMatcom);
		neb::CJsonObject jqGridItem;
		jqGridItem.Add("info", UnicodeToUtf8(StringToWString(std::string(buffer))));//汉字乱码
		jqSerialInfo["SerialInfo"].Add(jqGridItem);


		if (buffer)
		{
			LocalFree(buffer);
		}
	}
	if (GetLastError() != NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS)
	{
		return "";
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);
	


	return jqSerialInfo.ToString();
}


std::string EncodeUtils::getUVCDevices()
{
	//CStringArray   *usartList = new CStringArray();

	neb::CJsonObject jqSerialInfo;
	jqSerialInfo.AddEmptySubArray("SerialInfo");

	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD DeviceIndex;
	// Create a HDEVINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs((LPGUID)&GUID_DEVCLASS_PORTS, 0, 0, DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		DWORD ErrorCode = GetLastError();
		return "";
	}
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (DeviceIndex = 0; SetupDiEnumDeviceInfo(hDevInfo, DeviceIndex, &DeviceInfoData); DeviceIndex++)
	{
		DWORD DataPropertyType;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;
		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_FRIENDLYNAME,
			&DataPropertyType,
			(PBYTE)buffer,
			buffersize,
			&buffersize))

		{
			DWORD ErrorCode = GetLastError();
			if (ErrorCode == ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer) LocalFree(buffer);
				buffer = (LPTSTR)LocalAlloc(LPTR, buffersize);
			}
			else
			{
				break;
			}

		}
		//CString forMatcom = CString(buffer);
		//usartList->Add(forMatcom);
		neb::CJsonObject jqGridItem;
		jqGridItem.Add("info", UnicodeToUtf8(StringToWString(std::string(buffer))));//汉字乱码
		jqSerialInfo["SerialInfo"].Add(jqGridItem);


		if (buffer)
		{
			LocalFree(buffer);
		}
	}
	if (GetLastError() != NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS)
	{
		return "";
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);



	return jqSerialInfo.ToString();
}

int EncodeUtils::getUsartCom(std::string data) 
{
	int pos_s = data.find('(');
	pos_s += 4;
	int pos_e = data.find(')');
	data = data.substr(pos_s, pos_e-pos_s);
	//printf("---------------getUsartCom:%s-------------\r\n", data.c_str());
	if (data.find(">") != -1)
	{
		return -1;
	}
	return atoi(data.c_str());
}

std::string EncodeUtils::getSend_ConnectCmd(int addr)
{
	//
	char buf[64];
	memset(buf,'\0',64);
	int sum = 0;
	sprintf(buf,"%s%02X%s","A5", addr,"4900000000");
	BYTE buf_b[64];
	int length =Str2Hex(buf, buf_b);
	for (int index = 0; index < length; index++) {
		sum += buf_b[index];
	}
	sum &= 0x00FF;
	sprintf(buf, "%s%02X%s%02X", "A5", addr, "4900000000", sum);
	return std::string(buf);
}
std::string EncodeUtils::getCheck_ConnectCmd(int addr)
{
	char buf[64];
	memset(buf, '\0', 64);
	int sum = 0;
	sprintf(buf, "%s%02X%s", "A57A", addr, "00000000");
	BYTE buf_b[64];
	int length = Str2Hex(buf, buf_b);
	for (int index = 0; index < length; index++) {
		sum += buf_b[index];
	}
	sum &= 0x00FF;
	sprintf(buf, "%s%02X%s%02X", "A57A", addr, "00000000", sum);
	return std::string(buf);
}

bool EncodeUtils::getCheck_ConnectCmd(char *data, int datalength)
{
	try {
		int sum = 0;
		char buf[2];
		BYTE *buf_b = new BYTE[datalength];
		int length = Str2Hex(data, buf_b);
		for (int index = 1; index < length - 2; index++) {
			sum += buf_b[index];
			//printf("%02X ",buf_b[index]);
		}
		sum &= 0x00FF;

		if (sum == buf_b[length - 2])
		{
			delete[] buf_b;
			buf_b = NULL;
			return true;
		}
		delete[] buf_b;
		buf_b = NULL;
		return false;
	}
	catch (...) {
		LogUtils::debugger->error("getCheck_ConnectCmd[error]");
		//printf("getCheck_ConnectCmd[error]\r\n");
	}
	
}

/****************************************************************************************/

int EncodeUtils::Hex2Char(char c)
{
	try {
		if ((c >= '0') && (c <= '9'))

			return c - 0x30;//

		else if ((c >= 'A') && (c <= 'F'))

			return c - 'A' + 10;//

		else if ((c >= 'a') && (c <= 'f'))

			return c - 'a' + 10;//
		else

			return 0x10;
	}
	catch (...) {
		LogUtils::debugger->error("Hex2Char[error]");
		//printf("Hex2Char[error]\r\n");
	}
	
}

int EncodeUtils::Str2Hex(char* str, unsigned char *data)
{
	try {
		int t, t1;

		int rlen = 0, len = strlen(str);//str.GetLength();

		if (len == 1)

		{

			char h = str[0];

			t = Hex2Char(h);

			data[0] = (BYTE)t;

			rlen++;

		}


		for (int i = 0; i < len;)

		{

			char l, h = str[i];

			if (h == ' ')

			{

				i++;

				continue;

			}

			i++;

			if (i >= len)

				break;

			l = str[i];

			t = Hex2Char(h);

			t1 = Hex2Char(l);

			if ((t == 16) || (t1 == 16))//

				break;

			else

				t = t * 16 + t1;

			i++;

			data[rlen] = (BYTE)t;

			rlen++;

		}

		return rlen;

	}
	catch (...) {
		LogUtils::debugger->error("Str2Hex[error]");
		//printf("Str2Hex[error]\r\n");
	}
	
}

int EncodeUtils::calcLSB(std::string data)
{
	try {
		char buf[2];
		BYTE *buf_b = new BYTE[data.length()];
		int length = Str2Hex((char*)data.c_str(), buf_b);
		//for (int index = 2; index < length - 2; index++) {
		//	sum += buf_b[index];
		//	//printf("%02X ",buf_b[index]);
		//}
		int sum = buf_b[2] << 32 | buf_b[3] << 16 | buf_b[4] << 8 | buf_b[5];
		return sum;
		delete[] buf_b;
		buf_b = NULL;
		return false;
	}
	catch (...) {
		LogUtils::debugger->error("calcLSB[error]");
		//printf("calcLSB[error]");
	}
	
}
int EncodeUtils::calcMSB(std::string data)
{
	try {
		char buf[2];
		BYTE *buf_b = new BYTE[data.length()];
		int length = Str2Hex((char*)data.c_str(), buf_b);
		int sum = buf_b[2] | buf_b[3] << 8 | buf_b[4] << 16 | buf_b[5] << 32;
		return sum;
		delete[] buf_b;
		buf_b = NULL;
		return false;
	}
	catch (...) {
		LogUtils::debugger->error("calcMSB[error]");
		//printf("calcMSB[error]");
	}
	
}
std::string EncodeUtils::format(std::string data)
{
	try {
		std::string for_str = "";
		for (int index = data.length(); index > 0; index -= 2)
		{
			for_str += data.substr(index - 2, 2);

			//for_str += " ";
		}
		//for_str = for_str.substr(0, for_str.length()-1);
		return for_str;
	}
	catch (...) {
		LogUtils::debugger->error("format[error]");
	}
	
}
int EncodeUtils::formatHexVale2Int(std::string data)
{
	try {
		std::string f_str = format(data);
		const char * s = f_str.c_str();
		int temp = 0;
		int i, m, n;
		m = strlen(s);//十六进制是按字符串传进来的，所以要获得他的长度
		for (i = 0; i < m; i++)
		{
			if (s[i] >= 'A'&&s[i] <= 'F')//十六进制还要判断他是不是在A-F或者a-f之间a=10。。
				n = s[i] - 'A' + 10;
			else if (s[i] >= 'a'&&s[i] <= 'f')
				n = s[i] - 'a' + 10;
			else n = s[i] - '0';
			temp = temp * 16 + n;
		}
		return temp;
	}
	catch (...) {
		LogUtils::debugger->error("formatHexVale2Int[error]");
	}
	
}


std::string EncodeUtils::getFormatStringForMon(int num, char*cmdhead)
{
	try {
		char buff[128];// = new char[128];
		char output[128];// = new char[128];
		memset(output, '\0', 128);
		//sprintf(buff, "%I64X", num);
		sprintf(buff, "%02X", num);
		int length = strlen(buff);
		if (length % 2 != 0)
		{
			buff[length + 1] = '\0';
			for (int index = length; index > 0; index--)
			{

				buff[index] = buff[index - 1];
			}

			buff[0] = (char)48;
			length++;
		}



		for (int index = 0; index < 4 - length / 2; index++)
		{
			output[index * 2] = (char)48;
			output[index * 2 + 1] = (char)48;
		}
		for (int index = 4 - length / 2, index_c = 0; index < 4; index++, index_c++)
		{
			output[index * 2] = buff[index_c * 2];
			output[index * 2 + 1] = buff[index_c * 2 + 1];
		}
		char *result = new char[128];
		memset(result, '\0', 128);
		sprintf(result, "%s%s", cmdhead, output);

		//printf("result : %s\r\n", result);
		//计算校验和
		BYTE *buf_b = new BYTE[std::string(result).length()];
		length = Str2Hex(result, buf_b);
		//printf("length : %d\r\n", length);
		int checkSum = 0;
		for (int index = 1; index < length; index++) {
			checkSum += buf_b[index];
			//printf("%02X ",buf_b[index]);
		}
		delete[] buf_b;
		buf_b = NULL;
		//sum&=0x00ff;//取低两位
		checkSum &= 0x00ff;
		memset(buff, '\0', 128);
		sprintf(buff, "%02X", checkSum);


		sprintf(result, "%s%s", result, buff);
		sprintf(result, "%s%s", result, "55");
		std::string result_return = std::string(result);
		delete[] result;
		result = NULL;
		return result_return;
	}
	catch (...) {
		LogUtils::debugger->error("getFormatStringForMon[error]");
		//printf("getFormatStringForMon[error]\r\n");
	}
	
}

/****************************************************************************************/

std::vector<std::string> EncodeUtils::split(const std::string& str, const std::string& delim)
{
	try {
		std::vector<std::string> res;
		if ("" == str) return res;
		//先将要切割的字符串从string类型转换为char*类型
		char * strs = new char[str.length() + 1]; //不要忘了
		if (NULL == strs) {
			return res;
		}
		strcpy(strs, str.c_str());

		char * d = new char[delim.length() + 1];
		if (NULL == d) {
			return res;
		}
		strcpy(d, delim.c_str());

		char *p = strtok(strs, d);
		while (p) {
			std::string s = p; //分割得到的字符串转换为string类型
			res.push_back(s); //存入结果数组
			p = strtok(NULL, d);
		}
		delete[] strs;
		strs = NULL;
		delete[] d;
		d = NULL;
		return res;
	}
	catch (...) {
		LogUtils::debugger->error("split[error]");
		//printf("split[error]\r\n");
	}
	

}
bool EncodeUtils::ReSetWindows(unsigned long dwFlags, bool bForce) {
	if (dwFlags != EWX_LOGOFF && dwFlags != EWX_REBOOT && dwFlags != EWX_SHUTDOWN)
		return FALSE;
	OSVERSIONINFO osvi = { 0 };
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!GetVersionEx(&osvi))
	{
		return FALSE;
	}
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		EnableShutDownPriv();
	}
	
	dwFlags |= (bForce != FALSE) ? EWX_FORCE : EWX_FORCEIFHUNG;
	return ExitWindowsEx(dwFlags, 0);
}
bool EncodeUtils::EnableShutDownPriv()
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp = { 0 };
	
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}
	
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}