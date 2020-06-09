#ifndef __PIO002XX_LIB_H
#define __PIO002XX_LIB_H

#include "PUSIControlLib.h"

/**************************************宏定义**************************************/
//PWM通道
#define PWM0	0
#define PWM1	1
#define PWM2	2
#define PWM3	3
//ADC工作配置
//输入端口
#define ADM_IN_AN0	0	
#define ADM_IN_AN1	1	
//工作模式
#define ADM_SINGLE			0  //单次转换	
#define ADM_CONSECUTIVE		2  //连续转换

//编码器配置
//编码器使能
#define ENC_DISABLE				0
#define ENC_ENABLE				1
//触发沿配置
#define ENC_POSEDGE_TRIGER		0
#define ENC_NEGEDGE_TRIGER		16

//外部停止
#ifndef EXTSTOP1
#define EXTSTOP1	0x1
#endif

#ifndef EXTSTOP2
#define EXTSTOP2	0x2
#endif

/**************************************类型定义**************************************/

/**************************************导出函数声明**************************************/
//大电流驱动端口
PUSIResult __stdcall SetPWMDutyCycle(DWORD dwAddress,DWORD dwPWMChannel,DWORD dwDutyCycle); //设置PWM占空比，一次设置一个通道
PUSIResult __stdcall GetPWMDutyCycle(DWORD dwAddress,DWORD *pDutyCycle); //获取PWM占空比，一次获取四个通道

PUSIResult __stdcall SetDrvOutput(DWORD dwAddress,DWORD dwDrvOutput); //设置大电流驱动输出端口
PUSIResult __stdcall GetDrvOutput(DWORD dwAddress,DWORD *pDrvOutput); //获取大电流驱动输出端口
//ADC
PUSIResult __stdcall StartADC(DWORD dwAddress,DWORD dwADCCfg); //启动ADC，example: ADM_IN_AN1|ADM_CONSECUTIVE
PUSIResult __stdcall StopADC(DWORD dwAddress); //停止ADC
PUSIResult __stdcall GetADCResult(DWORD dwAddress,DWORD *pADCResult); //获取ADC转换结果
//GPIO
PUSIResult __stdcall SetGPIODirection(DWORD dwAddress,DWORD dwDir); //设置GPIO方向
PUSIResult __stdcall GetGPIODirection(DWORD dwAddress,DWORD *pDir); //获取GPIO方向
//LED
PUSIResult __stdcall SetLEDEnable(DWORD dwAddress,BOOL bEnable); //设置LED显示使能
PUSIResult __stdcall SetLEDDisplay(DWORD dwAddress,DWORD dwDispData); //设置LED显示数据
//encoder
PUSIResult __stdcall SetEncoderEnable(DWORD dwAddress,DWORD dwEncoderCfg); //设置编码器使能,ENC_ENABLE|ENC_NEGEDGE_TRIGER
PUSIResult __stdcall SetEncoderPos(DWORD dwAddress,int nEncoderPos); //设置编码器位置
PUSIResult __stdcall GetEncoderPos(DWORD dwAddress,int *pEncoderPos); //获取编码器位置
//user data
PUSIResult __stdcall SetUserDataAddr(DWORD dwAddress,DWORD dwUserDataAddr); //设置用户数据地址
PUSIResult __stdcall GetUserDataAddr(DWORD dwAddress,DWORD *pUserDataAddr); //获取用户数据地址
PUSIResult __stdcall WriteUserData(DWORD dwAddress,BYTE bUserData); //写一个字节用户数据
PUSIResult __stdcall ReadUserData(DWORD dwAddress,BYTE *pUserData); //读一个字节用户数据
//motor drive
PUSIResult __stdcall SetMotorDriveEnable(DWORD dwAddress,BOOL bEnable); //设置马达驱动使能
PUSIResult __stdcall SetVelocityMode(DWORD dwAddress,BOOL bVelocityMode); //设置速度模式
PUSIResult __stdcall StartStep(DWORD dwAddress,DWORD dwStepNum); //启动步进
PUSIResult __stdcall AbortStep(DWORD dwAddress); //当前步进命令中止	
PUSIResult __stdcall SetMotorPosition(DWORD dwAddress,int dwMotorPos); //设置马达位置
PUSIResult __stdcall GetMotorPosition(DWORD dwAddress,int* pMotorPos); //获取马达位置
PUSIResult __stdcall SetDirection(DWORD dwAddress,BOOL bReverse); //设置转动方向
PUSIResult __stdcall SetVelocity(DWORD dwAddress,DWORD dwVelocity); //设置转动速度
PUSIResult __stdcall GetVelocity(DWORD dwAddress,DWORD *pVelocity); //获取转动速度
PUSIResult __stdcall SetStartVelocity(DWORD dwAddress,DWORD dwStartVelocity); //设置启动速度
PUSIResult __stdcall GetStartVelocity(DWORD dwAddress,DWORD *pStartVelocity); //获取启动速度
PUSIResult __stdcall SetStopVelocity(DWORD dwAddress,DWORD dwStopVelocity); //设置停止速度
PUSIResult __stdcall GetStopVelocity(DWORD dwAddress,DWORD *pStopVelocity); //获取停止速度
PUSIResult __stdcall SetAccCoff(DWORD dwAddress,DWORD dwCoff); //设置加速系数
PUSIResult __stdcall GetAccCoff(DWORD dwAddress,DWORD *pCoff);//获取加速系数
PUSIResult __stdcall SetDecCoff(DWORD dwAddress,DWORD dwCoff); //设置减速系数
PUSIResult __stdcall GetDecCoff(DWORD dwAddress,DWORD *pCoff); //获取减速系数
PUSIResult __stdcall SetExtStopEnable(DWORD dwAddress,DWORD dwStopEnable); //设置外部紧急停止使能
PUSIResult __stdcall SetExtStopTrigerMode(DWORD dwAddress,DWORD dwTrigerMode); //设置外部紧急停止触发方式
PUSIResult __stdcall GetExtStopTrigerMode(DWORD dwAddress,DWORD *pTrigerMode); //获取外部紧急停止触发方式
PUSIResult __stdcall ClearExtStop(DWORD dwAddress,DWORD dwExtStop); //清除外部停止
PUSIResult __stdcall GetControllerStatus1(DWORD dwAddress,DWORD *pStatus); //获取控制器状态1
PUSIResult __stdcall GetControllerStatus2(DWORD dwAddress,DWORD *pStatus); //获取控制器状态2
#endif
