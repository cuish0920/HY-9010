#include "pusi.h"
#define MOTOR_FORWARD	0 //1
#define MOTOR_REVERSE	1 //0
#define MOTOR_READY_DIRECTIONFORWARD 0
#define MOTOR_READY_DIRECTIONREVERSE 1
#define ERROR_HANDLE(closePort,errInfo)	if(RET_SUCCESS != result){\
	std::printf(errInfo);\
	if(TRUE == closePort){\
		ClosePort();\
	}\
	return;\
}

//#include <vector>
namespace PUSI {
	using v8::Persistent;
	using v8::Function;
	struct Data {
		int addr;//需要归零的电机地址
		int offset;//设置相对零位
		int zeroSpeed;//归零速度
		int speed;//运行速度
	};

	//记录执行归零的电机有哪些
	int PUSI_FUN::cout_zero_equp = 0;
	bool PUSI_FUN::is_connection = false;
	bool PUSI_FUN::is_move = false;
	neb::CJsonObject PUSI_FUN::gohome_info;//记录当前归零的电机有哪些

	PUSI_FUN::PUSI_FUN() {

	}

	PUSI_FUN::~PUSI_FUN() {
	}

	/**************************************************
	***@author:cui_sh
	***@brief:跟据传入的参数让电机执行归零
	***@time:
	***@version:
	**************************************************/
	void PUSI_FUN::go_to_home(neb::CJsonObject list) 
	{
		//printf("go_to_home %d\r\n",list["addrList"].GetArraySize());
		//判断当前传入和电机地址，然后对传入的电机执行零操作
		
		//cout_zero_equp = 0;
		for (int index = 0; index < list["addrList"].GetArraySize(); index++) 
		{
			int addr;
			int zeroSpeed;//归零速度
			int speed;//工作速度
			int offset;
			list["addrList"][index].Get("addr", addr);
			list["addrList"][index].Get("zeroSpeed", zeroSpeed);
			list["addrList"][index].Get("speed", speed);
			list["addrList"][index].Get("offset", offset);
			//printf("%d %d %d %d\r\n", addr, zeroSpeed, speed, offset);
			std::string isExist = "";
			gohome_info.Get(std::to_string(addr), isExist);
			if (isExist.compare("1")==0) {
				//printf("addrList addr:%d 正在归零\r\n", addr);
				
				continue;
			}
			gohome_info.Add(std::to_string(addr), std::to_string(true));
			/*gohome_info.Get(std::to_string(addr), isExist);
			printf("入参前%s\r\n", isExist.c_str());

			gohome_info.Add(std::to_string(addr), std::to_string(true));
			printf("状态: %s\r\n", gohome_info.ToString().c_str());

			gohome_info.Get(std::to_string(addr), isExist);
			printf("入参前后%s\r\n", isExist.c_str());
			
			printf("addrList addr:%d \r\n", addr);*/
			
			cout_zero_equp++;

			//取得需要归零的电机
			//创建归零线程，负责各电机执行归零操作
			Data* data = new Data();
			data->addr = addr;
			data->offset = offset;
			data->zeroSpeed = zeroSpeed;
			data->speed = speed;
			uv_work_t *req = new uv_work_t();
			req->data = data;
			uv_queue_work(
				uv_default_loop(),
				req,
				(uv_work_cb)pusiReset,
				(uv_after_work_cb)afterPusiReset
			);


		}
		//printf("gohome_info: %s\r\n",gohome_info.ToString().c_str());
		//等待所有电机都归零完成 后才返回
		while (cout_zero_equp > 0) {

			Sleep(50);
		}

	}

	/**************************************************
	***@author:cui_sh
	***@brief:执行归零操作
	***@time:
	***@version:
	**************************************************/
	void PUSI_FUN::pusiReset(uv_work_t* req) 
	{
		Data* data = static_cast<Data*>(req->data);
		//归零代码
		DWORD dwVersion = 0;
		DWORD dwStatus = 0;
		PUSIResult result = AbortStep(data->addr);//中止当前步进
		ERROR_HANDLE(FALSE, "AbortStep");

		ClearExtStop(data->addr, 1); //清除中断
		ERROR_HANDLE(FALSE, "ClearExtStop");

		result = SetMaxCurrent(data->addr, 2000); //设置最大电流
		ERROR_HANDLE(TRUE, "");
		result = SetMicroStep(data->addr, 16); //设置细分数
		ERROR_HANDLE(TRUE, "");
		result = SetAccCoff(data->addr, 5); //设置加速度细数5
		ERROR_HANDLE(TRUE, "");
		result = SetDecCoff(data->addr, 5); //设置减速度细数5
		ERROR_HANDLE(TRUE, "");

		result = SetExtStopEnable(data->addr, 1);//使能外部触发中断1
		ERROR_HANDLE(FALSE, "SetExtStopEnable");

		result = SetExtStopTrigerMode(data->addr, 0);//使能外部触发中断1
		ERROR_HANDLE(FALSE, "SetExtStopTrigerMode");

		result = GetIOValue(data->addr, &dwStatus); //获取控制器状态
		ERROR_HANDLE(FALSE, "GetControllerStatus1");
		if ((dwStatus & 0x0400))
		{
			//清除中断
			result = ClearExtStop(data->addr, 1); //清除中断
			ERROR_HANDLE(FALSE, "ClearExtStop");
			//归零使用
			result = SetVelocity(data->addr, data->zeroSpeed);//设置转动速度
			ERROR_HANDLE(FALSE, "SetVelocity");

			result = SetStopVelocity(data->addr, data->zeroSpeed);//设置停止速度
			ERROR_HANDLE(FALSE, "SetStopVelocity");

			result = SetStartVelocity(data->addr, data->zeroSpeed);//设置停止速度
			ERROR_HANDLE(FALSE, "SetStartVelocity");
			//正向转动
			result = SetDirection(data->addr, MOTOR_FORWARD/* MOTOR_REVERSE*/); //设置转动方向
			ERROR_HANDLE(FALSE, "SetDirection");

			result = StartStep(data->addr, 0x7FFFFFFF); //启动步进
			ERROR_HANDLE(FALSE, "StartStep");

			result = GetControllerStatus1(data->addr, &dwStatus); //获取控制器状态
			ERROR_HANDLE(FALSE, "GetControllerStatus1");
			while (dwStatus & 0x1)   //判断忙状态，忙则等待转动完成
			{
				Sleep(50);
				result = GetControllerStatus1(data->addr, &dwStatus); //获取控制器状态
				ERROR_HANDLE(FALSE, "GetControllerStatus1");
			}

			//清除中断
			result = ClearExtStop(data->addr, 1); //清除中断
			ERROR_HANDLE(FALSE, "ClearExtStop");

			result = ClearExtStop(data->addr, 1); //清除中断
			ERROR_HANDLE(FALSE, "ClearExtStop");
			//反向转动
			result = SetDirection(data->addr, /*MOTOR_FORWARD*/ MOTOR_REVERSE); //设置转动方向
			ERROR_HANDLE(FALSE, "SetDirection");

			result = StartStep(data->addr, data->offset); //启动步进
			ERROR_HANDLE(FALSE, "StartStep");

			result = GetControllerStatus1(data->addr, &dwStatus); //获取控制器状态
			ERROR_HANDLE(FALSE, "GetControllerStatus1");
			while ((dwStatus & 0x1) && data->addr)   //判断忙状态，忙则等待转动完成
			{
				Sleep(50);
				result = GetControllerStatus1(data->addr, &dwStatus); //获取控制器状态
				ERROR_HANDLE(FALSE, "GetControllerStatus1");
			}
		}
		else 
		{
			//在零位，需要去到相对零位
			//清除中断
			result = ClearExtStop(data->addr, 1); //清除中断
			ERROR_HANDLE(FALSE, "ClearExtStop");

			result = ClearExtStop(data->addr, 1); //清除中断
			ERROR_HANDLE(FALSE, "ClearExtStop");
			//反向转动
			result = SetDirection(data->addr, /*MOTOR_FORWARD*/ MOTOR_REVERSE); //设置转动方向
			ERROR_HANDLE(FALSE, "SetDirection");

			result = StartStep(data->addr, data->offset); //启动步进
			ERROR_HANDLE(FALSE, "StartStep");

			result = GetControllerStatus1(data->addr, &dwStatus); //获取控制器状态
			ERROR_HANDLE(FALSE, "GetControllerStatus1");
			while (dwStatus & 0x1)   //判断忙状态，忙则等待转动完成
			{
				Sleep(50);
				result = GetControllerStatus1(data->addr, &dwStatus); //获取控制器状态
				ERROR_HANDLE(FALSE, "GetControllerStatus1");
			}
		}

		result = SetMotorPosition(data->addr, 0);//
		ERROR_HANDLE(FALSE, "SetMotorPosition");

		result = SetMicroStep(data->addr, 128); //设置细分数
		ERROR_HANDLE(TRUE, "");

		//归零使用
		result = SetVelocity(data->addr, data->speed);//设置转动速度                    单位改成度/s  换算成脉冲数 
		ERROR_HANDLE(FALSE, "SetVelocity");

		result = SetStopVelocity(data->addr, data->speed);//设置停止速度                单位改成度/s  换算成脉冲数
		ERROR_HANDLE(FALSE, "SetStopVelocity");

		result = SetStartVelocity(data->addr, data->speed);//设置停止速度				  单位改成度/s  换算成脉冲数
		ERROR_HANDLE(FALSE, "SetStartVelocity");
		//printf("pusiReset: %d \r\n", cout_zero_equp);
		gohome_info.Delete(std::to_string(data->addr));
		cout_zero_equp--;
		
	}

	/**************************************************
	***@author:cui_sh
	***@brief:执行归零后会调用紫方法
	***@time:
	***@version:
	**************************************************/
	void PUSI_FUN::afterPusiReset(uv_work_t* req)
	{
		//printf("afterPusiReset: %d \r\n", cout_zero_equp);
		Data* data = static_cast<Data*>(req->data);
		
		/* 回调完成后清除资源 */
		delete data;
		data = NULL;
		delete req;
		req = NULL;

	}
	/**************************************************
	***@author:cui_sh
	***@brief:打开电机连接
	***@time:
	***@version:
	**************************************************/
	bool PUSI_FUN::OPEN(int port)
	{
		//printf("OPEN::opend:%d\r\n", port);
		if (RET_SUCCESS == OpenPort(port, CBR_9600))
		{
			//printf("RET_SUCCESS%d\r\n", port);
			is_connection = true;
			return true;
		}
		return false;
	}

	/**************************************************
	***@author:cui_sh
	***@brief:关闭电机连接
	***@time:
	***@version:
	**************************************************/
	bool PUSI_FUN::CLOSE()
	{
		is_connection = false;
		return ClosePort();
	}

	bool PUSI_FUN::GET_CONNECT_STATUS() 
	{
		return is_connection;
	}

	bool PUSI_FUN::GET_WORK_STATUS(int addr)
	{
		if (GET_CONNECT_STATUS())
		{
			DWORD dwStatus = 0;
			GetControllerStatus1(addr, &dwStatus);
			if (dwStatus & 0x1) {
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
		return is_connection;
	}


	/**************************************************
	***@author:cui_sh
	***@brief:运动前状态检查
	***@time:
	***@version:
	**************************************************/
	bool PUSI_FUN::MOVE_CHECK(int addr, int dir, int rate, neb::CJsonObject controlData)
	{
		//获取当前需要控制动电机对应应的是什么轴
		std::string axis;
		controlData.Get(std::to_string(addr), axis);//获取需要控制的轴

		//获取对应轴的长度
		int axisLength;
		controlData.Get(axis, axisLength);//取得对应轴的长度

		//根据方向判断当前需要运动的方向是否可行

		if (dir == MOVE_DIRECTION_1)
		{
			//向归零方向运动
			//当前电机是否碰到限位开关，若没有则可以运动
			DWORD dwStatus = 0;
			PUSIResult result = GetIOValue(addr, &dwStatus); //获取控制器状态
			//ERROR_HANDLE(FALSE, "GetControllerStatus1");
			if (result != RET_SUCCESS)
			{
				return false;
			}
			if ((dwStatus & 0x0400))
			{
				return true;
			}
			return false;
		}
		else if (dir == MOVE_DIRECTION_2)
		{
			//向归零方反向运动
			int position = 0;
			PUSIResult result = GetMotorPosition(addr, &position); //启动步进
			//ERROR_HANDLE(FALSE, "GetMotorPosition");
			if (result != RET_SUCCESS)
			{
				return false;
			}
			//printf("position:%d  axisLength:%d \r\n", position, axisLength);
			if (abs(position) < axisLength) 
			{
				return true;
			}
			return false;
		}
		return false;
	}

	/**************************************************
	***@author:cui_sh
	***@brief:打开电机连接
	***@time:
	***@version:
	**************************************************/
	void PUSI_FUN::MOVE_CONTROL(int addr, int dir, int rate,int axisSpeed,int axisLength)
	{
		//电机运动控制
		//求出电机最多可以运动的步长
		//设置电机运动方向
		//开始运动（直达用户停止或者，到达极限）

		//获取当前需要控制动电机对应应的是什么轴
		//std::string axis;
		//controlData.Get(std::to_string(addr), axis);//获取需要控制的轴

		//获取对应轴的长度
		//int axisLength;
		//controlData.Get(axis, axisLength);//取得对应轴的长度

		//char buf[24];
		//sprintf(buf,"%s%s", axis.c_str(),"_Speed");
		//int speed;
		//controlData.Get(buf, speed);//取得对应轴的速度

		//设置当前轴的速度
		axisSpeed *= rate;
		//printf("axisSpeed=%d axisLength=%d\r\n", axisSpeed, axisLength);

		PUSIResult result = AbortStep(addr);//中止当前步进
		ERROR_HANDLE(FALSE, "AbortStep");

		ClearExtStop(addr, 1); //清除中断
		ERROR_HANDLE(FALSE, "ClearExtStop");

		result = SetMaxCurrent(addr, 2000); //设置最大电流
		ERROR_HANDLE(TRUE, "");
		result = SetMicroStep(addr, 128); //设置细分数
		ERROR_HANDLE(TRUE, "");
		result = SetAccCoff(addr, 5); //设置加速度细数5
		ERROR_HANDLE(TRUE, "");
		result = SetDecCoff(addr, 5); //设置减速度细数5
		ERROR_HANDLE(TRUE, "");

		result = SetExtStopEnable(addr, 1);//使能外部触发中断1
		ERROR_HANDLE(FALSE, "SetExtStopEnable");

		result = SetExtStopTrigerMode(addr, 0);//使能外部触发中断1
		ERROR_HANDLE(FALSE, "SetExtStopTrigerMode");

		//归零使用
		result = SetVelocity(addr, axisSpeed);//设置转动速度
		ERROR_HANDLE(FALSE, "SetVelocity");

		result = SetStopVelocity(addr, axisSpeed);//设置停止速度
		ERROR_HANDLE(FALSE, "SetStopVelocity");

		result = SetStartVelocity(addr, axisSpeed);//设置停止速度
		ERROR_HANDLE(FALSE, "SetStartVelocity");
		printf("//执行参数设置\r\n");
		//执行参数设置
		if (dir == MOVE_DIRECTION_1)
		{
			//向归零方向运动
			result = SetDirection(addr, MOTOR_FORWARD/* MOTOR_REVERSE*/); //设置转动方向
			ERROR_HANDLE(FALSE, "SetDirection");
			MOVE_TO_L(addr);
		}
		else if (dir == MOVE_DIRECTION_2)
		{
			//向归零方反向运动

			result = SetDirection(addr, MOTOR_REVERSE /* MOTOR_FORWARD*/); //设置转动方向
			ERROR_HANDLE(FALSE, "SetDirection");
			MOVE_TO_R(addr, axisLength);

		}
		
		

	}
	/**************************************************
	***@author:cui_sh
	***@brief:运动控制
	***@time:
	***@version:
	**************************************************/
	bool PUSI_FUN::MOVE_CTRL_FLG(bool value) 
	{
		is_move = value;
		return true;
	}

	/**************************************************
	***@author:cui_sh
	***@brief:向物理零方向移动
	***@time:
	***@version:
	**************************************************/
	void PUSI_FUN::MOVE_TO_L(int addr)
	{
		DWORD dwStatus = 0;
		PUSIResult result = StartStep(addr, 0x7FFFFFFF); //启动步进
		ERROR_HANDLE(FALSE, "StartStep");

		/*while (is_move)
		{
			Sleep(80);
		}*/

		result = GetControllerStatus1(addr, &dwStatus); //获取控制器状态
		ERROR_HANDLE(FALSE, "GetControllerStatus1");
		while ((dwStatus & 0x1)&&(is_move))   //判断忙状态，忙则等待转动完成
		{
			
			result = GetControllerStatus1(addr, &dwStatus); //获取控制器状态
			ERROR_HANDLE(FALSE, "GetControllerStatus1");
			Sleep(50);
		}

		//清除中断
		result = ClearExtStop(addr, 1); //清除中断
		ERROR_HANDLE(FALSE, "ClearExtStop");

		result = ClearExtStop(addr, 1); //清除中断
		ERROR_HANDLE(FALSE, "ClearExtStop");

		//
		AbortStep(addr);//中止当前步进
		ERROR_HANDLE(FALSE, "AbortStep");
	}
	/**************************************************
	***@author:cui_sh
	***@brief:向物理零反方向移动
	***@time:
	***@version:
	**************************************************/
	void PUSI_FUN::MOVE_TO_R(int addr, int maxLength)
	{
		DWORD dwStatus = 0;
		PUSIResult result = StartStep(addr, maxLength); //启动步进
		ERROR_HANDLE(FALSE, "StartStep");
		//printf("MOVE_TO_R maxLength=%d\r\n", maxLength);

		result = GetControllerStatus1(addr, &dwStatus); //获取控制器状态
		ERROR_HANDLE(FALSE, "GetControllerStatus1");
		while ((dwStatus & 0x1) && (is_move))   //判断忙状态，忙则等待转动完成
		{

			result = GetControllerStatus1(addr, &dwStatus); //获取控制器状态
			ERROR_HANDLE(FALSE, "GetControllerStatus1");
			Sleep(50);
		}

		//
		AbortStep(addr);//中止当前步进
		ERROR_HANDLE(FALSE, "AbortStep");
	}


	/**************************************************
	***@author:cui_sh
	***@brief:向物理零反方向移动
	***@time:
	***@version:
	**************************************************/
	void PUSI_FUN::SCAN_GO(int addr, int maxLength)
	{
		DWORD dwStatus = 0;
		PUSIResult result = StartStep(addr, maxLength); //启动步进
		ERROR_HANDLE(FALSE, "StartStep");
		//printf("SCAN_TO_R: maxLength %d\r\n", maxLength);
		result = GetControllerStatus1(addr, &dwStatus); //获取控制器状态
		ERROR_HANDLE(FALSE, "GetControllerStatus1");
		while ((dwStatus & 0x1))   //判断忙状态，忙则等待转动完成
		{

			result = GetControllerStatus1(addr, &dwStatus); //获取控制器状态
			ERROR_HANDLE(FALSE, "GetControllerStatus1");
			Sleep(50);
		}

		//
		AbortStep(addr);//中止当前步进
		ERROR_HANDLE(FALSE, "AbortStep");
	}

	/**************************************************
	***@author:cui_sh
	***@brief:启动摆扫
	***@time:
	***@version:
	**************************************************/
	void PUSI_FUN::SCAN_CONTROL(int addr,int length)
	{
		//首选得到摆扫维度（X,Y,Z）
		
		//获得起始位置和停止位置
		//获得扫描速度

		//电机归零
		
		//运动到指定的起始位置
		

		//启动摆扫
		//到达停止位置
		//SCAN_TO_R(addr, length);
		//关闭摆扫
	}

	/**************************************************
	***@author:cui_sh
	***@brief:启动摆扫前准备
	***@time:
	***@version:
	**************************************************/
	void PUSI_FUN::SCAN_READY(int addr, int direction,int speed,int zeroSpeed,int offset, int length)
	{
		//电机归零
		DWORD dwVersion = 0;
		DWORD dwStatus = 0;
		PUSIResult result = AbortStep(addr);//中止当前步进
		ERROR_HANDLE(FALSE, "AbortStep");
		ClearExtStop(addr, 1); //清除中断
		ERROR_HANDLE(FALSE, "ClearExtStop");
		result = SetMaxCurrent(addr, 2000); //设置最大电流
		ERROR_HANDLE(TRUE, "");
		result = SetMicroStep(addr, 16); //设置细分数
		ERROR_HANDLE(TRUE, "");
		result = SetAccCoff(addr, 5); //设置加速度细数5
		ERROR_HANDLE(TRUE, "");
		result = SetDecCoff(addr, 5); //设置减速度细数5
		ERROR_HANDLE(TRUE, "");
		result = SetExtStopEnable(addr, 1);//使能外部触发中断1
		ERROR_HANDLE(FALSE, "SetExtStopEnable");

		result = SetExtStopTrigerMode(addr, 0);//使能外部触发中断1
		ERROR_HANDLE(FALSE, "SetExtStopTrigerMode");
		//归零使用
		result = SetVelocity(addr, zeroSpeed);//设置转动速度
		ERROR_HANDLE(FALSE, "SetVelocity");

		result = SetStopVelocity(addr, zeroSpeed);//设置停止速度
		ERROR_HANDLE(FALSE, "SetStopVelocity");

		result = SetStartVelocity(addr, zeroSpeed);//设置停止速度
		ERROR_HANDLE(FALSE, "SetStartVelocity");
		//MOTOR_READY_DIRECTIONFORWARD
		//MOTOR_READY_DIRECTIONREVERSE
		//归零转动方向
		if (direction == MOTOR_READY_DIRECTIONFORWARD)
		{
			result = SetDirection(addr, MOTOR_FORWARD/* MOTOR_REVERSE*/); //设置转动方向
			ERROR_HANDLE(FALSE, "SetDirection");
		}
		else if(direction == MOTOR_READY_DIRECTIONREVERSE)
		{
			result = SetDirection(addr, MOTOR_REVERSE /* MOTOR_FORWARD*/); //设置转动方向
			ERROR_HANDLE(FALSE, "SetDirection");
		}
		result = StartStep(addr, offset+length); //启动步进
		ERROR_HANDLE(FALSE, "StartStep");
		result = GetControllerStatus1(addr, &dwStatus); //获取控制器状态
		ERROR_HANDLE(FALSE, "GetControllerStatus1");
		while (dwStatus & 0x1)   //判断忙状态，忙则等待转动完成
		{
			
			result = GetControllerStatus1(addr, &dwStatus); //获取控制器状态
			ERROR_HANDLE(FALSE, "GetControllerStatus1");
			Sleep(50);
		}
		//设置转动方向
		if (direction == MOTOR_READY_DIRECTIONFORWARD)
		{
			result = SetDirection(addr, MOTOR_REVERSE /* MOTOR_FORWARD*/);
			ERROR_HANDLE(FALSE, "SetDirection");
		}
		//else
		//{
		//	result = SetDirection(addr, MOTOR_FORWARD  /* MOTOR_REVERSE*/);
		//	ERROR_HANDLE(FALSE, "SetDirection");
		//}
		result = SetMicroStep(addr, 128); //设置细分数
		ERROR_HANDLE(TRUE, "");

		//工作速度
		result = SetVelocity(addr, speed);//设置转动速度                    单位改成度/s  换算成脉冲数 
		ERROR_HANDLE(FALSE, "SetVelocity");

		result = SetStopVelocity(addr, speed);//设置停止速度                单位改成度/s  换算成脉冲数
		ERROR_HANDLE(FALSE, "SetStopVelocity");

		result = SetStartVelocity(addr, speed);//设置停止速度				  单位改成度/s  换算成脉冲数
		ERROR_HANDLE(FALSE, "SetStartVelocity");

		////电机归零
		//neb::CJsonObject addrList;
		//addrList.AddEmptySubArray("addrList");
		//neb::CJsonObject item;
		//item.Add("addr", addr);
		//item.Add("speed", speed);
		//item.Add("zeroSpeed", zeroSpeed);
		//item.Add("offset", offset+ length);
		//addrList["addrList"].Add(item);

		////printf("SCAN_READY %d\r\n", addrList["addrList"].GetArraySize());
		//go_to_home(addrList);

		////归零完成后到达指定的起始位置
		////SCAN_TO_R(addr, length);
	}
}