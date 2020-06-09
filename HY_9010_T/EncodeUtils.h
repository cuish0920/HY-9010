#pragma once
#ifndef __ENCODE_UTILS_H__
#define __ENCODE_UTILS_H_
#include <string>
#include <vector>

class EncodeUtils {
	 public:
	  static std::wstring Utf8ToUnicode(std::string &strUTF8);
	  static std::string UnicodeToUtf8(std::wstring &strUnicode);
	  static std::wstring StringToWString(std::string &str);
	  static std::string WStringToString(std::wstring &wstr);
	  static std::string toUtf8String(const wchar_t * const str);
	  static int32_t createDirectory(const std::string &directoryPath);
	  static std::string getComMonDevices();
	  static std::string getUVCDevices();
	  static int getUsartCom(std::string data);
	 
	  static std::string getSend_ConnectCmd(int addr);
	  static std::string getCheck_ConnectCmd(int addr);
	  static bool getCheck_ConnectCmd(char *data, int length);
	  static int Str2Hex(char* str, unsigned char *data);
	  static int calcLSB(std::string data);
	  static int calcMSB(std::string data);
	  static int formatHexVale2Int(std::string data);
	  static std::string getFormatStringForMon(int num, char*cmdhead);
	  static std::vector<std::string> split(const std::string& str, const std::string& delim);
	  static bool ReSetWindows(unsigned long dwFlags, bool bForce);
	  
private:
		static int Hex2Char(char c);
		static std::string format(std::string data);
		static bool EnableShutDownPriv();
		
};




#endif 