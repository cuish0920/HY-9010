#include "LogUtils.h"
log4cpp::Category *LogUtils::debugger;
log4cpp::Category *LogUtils::info;
log4cpp::Category *LogUtils::error;
//LogUtils *LogUtils::instance_;
////全局访问接口  
//LogUtils *LogUtils::GetInstance()
//{
//	if (instance_ == NULL)
//	{
//		instance_ = new LogUtils();
//		//instance_->init_log("");
//		//LogUtils::init_log("");
//
//	}
//	return instance_;
//}
LogUtils::LogUtils()
{

}
LogUtils::~LogUtils()
{

}

void LogUtils::init_log(std::string log_name)
{
	//if (log_name.compare("") == 0)
	//{
	//	log_name = "sys_c.log";
	//}
	//log4cpp::PatternLayout* pLayout = new log4cpp::PatternLayout();
	//pLayout->setConversionPattern("%d: %p %c %x: %m%n");

	//log4cpp::Appender* fileAppender = new log4cpp::FileAppender("fileAppender", log_name);
	//fileAppender->setLayout(pLayout);

	//debugger = &log4cpp::Category::getRoot().getInstance("debugger");
	//debugger->addAppender(fileAppender);
	//debugger->setPriority(log4cpp::Priority::DEBUG);

	//info = &log4cpp::Category::getRoot().getInstance("info");
	//info->addAppender(fileAppender);
	//info->setPriority(log4cpp::Priority::INFO);

	//error = &log4cpp::Category::getRoot().getInstance("error");
	////log4cpp::Category& error = log4cpp::Category::getRoot().getInstance("error");
	//error->addAppender(fileAppender);
	//error->setPriority(log4cpp::Priority::ERROR);



	std::string initFileName = "log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);


	debugger = &log4cpp::Category::getRoot();

	/*log4cpp::Category& sub1 =
		log4cpp::Category::getInstance(std::string("sub1"));*/

		//sub1.info("All hatches closed");
		//log4cpp::Category::shutdown();

}


void LogUtils::shutdown()
{
	log4cpp::Category::shutdown();
}
