#pragma once

/**************************************************
***@author:cui_sh
***@brief:imperx相机引入
***@time:2019-06-14
***@version:
**************************************************/

#ifndef __PUSI_H__
#define __PUSI_H__

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            //  从 Windows 头文件中排除极少使用的信息
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <iostream>
#include "PMC006xxLib.h"
#include "PCS0801Lib.h"
#include "PIO002xxLib.h"


//////////////////外设检测串口///////////////////
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")
#include <devguid.h>
//////////////////外设检测串口///////////////////

//////////////////引入json///////////////////
#include "CJsonObject.h"
//////////////////引入json///////////////////
#include <uv.h>
#include <node.h>
#define MOVE_DIRECTION_1 0
#define MOVE_DIRECTION_2 1
namespace PUSI {
	class PUSI_FUN
	{
		 private:
			 PUSI_FUN();
			 ~PUSI_FUN();
		public:
			static int cout_zero_equp;
			static void go_to_home(neb::CJsonObject jsondata);
			static void pusiReset(uv_work_t* req);
			static void afterPusiReset(uv_work_t* req);
			static neb::CJsonObject gohome_info;
			static bool OPEN(int port);
			static bool CLOSE();
			static bool GET_CONNECT_STATUS();
			static bool GET_WORK_STATUS(int addr);

			static bool MOVE_CHECK(int addr, int dir, int rate, neb::CJsonObject controlData);
			static bool MOVE_CTRL_FLG(bool value);
			static void MOVE_CONTROL(int addr, int dir, int rate,int axisSpeed, int axisLength);
			static void SCAN_CONTROL(int addr, int length);
			static void SCAN_READY(int addr,int direction, int speed, int zeroSpeed, int offset, int length);
			static void SCAN_GO(int addr, int maxLength);

		private:
			static void MOVE_TO_L(int addr);
			static void MOVE_TO_R(int addr,int maxLength);
			

		private:
				static bool is_connection;
				static bool is_move;

	};
}

#endif 