#ifndef __PCS0801_LIB_H
#define __PCS0801_LIB_H

#include "PUSIControlLib.h"

/**************************************宏定义**************************************/
#define WORKMODE_CONSECUTIVE	0
#define WORKMODE_CALIBRATION	1

/**************************************类型定义**************************************/

/**************************************导出函数声明**************************************/
PUSIResult __stdcall SetAutoLoadCalibData(DWORD dwAddress,BOOL bAutoLoad); //设置自动加载校准数据
PUSIResult __stdcall SetWorkMode(DWORD dwAddress,DWORD dwWorkMode); //设置工作模式
PUSIResult __stdcall SetMeasureMode(DWORD dwAddress,DWORD dwMeasureMode); //设置测量模式
PUSIResult __stdcall GetSensorWorkMode(DWORD dwAddress,DWORD *pWorkMode); //读传感器的工作模式
PUSIResult __stdcall GetSensorWorkStatus(DWORD dwAddress,DWORD *pWorkStatus); //读传感器的工作状态
PUSIResult __stdcall GetCapValue(DWORD dwAddress,DWORD *pCapValue); //读电容值
PUSIResult __stdcall GetCapCalibData(DWORD dwAddress,DWORD *pCapCalibData); //读电容校准数据
PUSIResult __stdcall SetCapCalibData(DWORD dwAddress,DWORD dwCapCalibData); //设置电容校准值
PUSIResult __stdcall SetCapOffset(DWORD dwAddress,DWORD dwCapOffset); //设置电容偏移量
PUSIResult __stdcall SetCapTriger1(DWORD dwAddress,DWORD dwCapTriger1); //设置电容触发值1
PUSIResult __stdcall GetCapTriger1(DWORD dwAddress,DWORD *pCapTriger1); //获取电容触发值1
PUSIResult __stdcall SetCapTriger2(DWORD dwAddress,DWORD dwCapTriger2); //设置电容触发值2
PUSIResult __stdcall GetCapTriger2(DWORD dwAddress,DWORD *pCapTriger2); //获取电容触发值2
PUSIResult __stdcall SetCapTriger3(DWORD dwAddress,DWORD dwCapTriger3); //设置电容触发值3
PUSIResult __stdcall GetCapTriger3(DWORD dwAddress,DWORD *pCapTriger3); //设置电容触发值3
PUSIResult __stdcall SetCapTriger4(DWORD dwAddress,DWORD dwCapTriger4); //设置电容触发值4
PUSIResult __stdcall GetCapTriger4(DWORD dwAddress,DWORD *pCapTriger4); //设置电容触发值4
PUSIResult __stdcall SetMeasureRangeAdjust(DWORD dwAddress,BOOL bEnable); //测量范围调整使能
PUSIResult __stdcall SetMeasureAdjustA(DWORD dwAddress,DWORD dwAdjustA); //设置测量调整值A
PUSIResult __stdcall SetMeasureAdjustB(DWORD dwAddress,DWORD dwAdjustB); //设置测量调整值B
PUSIResult __stdcall SetVoltChannel(DWORD dwAddress,BOOL bEnable); //设置电压通道使能
PUSIResult __stdcall GetVoltValue(DWORD dwAddress,DWORD *pVoltValue); //读取电压值
PUSIResult __stdcall ResetSensor(DWORD dwAddress); //复位传感器

#endif
