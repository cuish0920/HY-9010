#pragma once
#ifndef __ENCODE_UTILS_H__
#define __ENCODE_UTILS_H_

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/OstreamAppender.hh>

#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>

#include <log4cpp/PropertyConfigurator.hh>
//#include <log4cpp/Layout.hh>
//#include <log4cpp/BasicLayout.hh>
//#include <log4cpp/Priority.hh>
//#include <log4cpp/NDC.hh>


class LogUtils {
public:
	//全局访问接口  
	LogUtils();
	~LogUtils();

public:
	static void init_log(std::string log_name);
	static void shutdown();
public:
	static log4cpp::Category* debugger;
	static log4cpp::Category* info;
	static log4cpp::Category* error;
private:

	//static LogUtils *instance_; //引用性声明

};




#endif 