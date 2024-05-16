/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-17 00:55:28
 * @FilePath: \Gantry\UserCode\Upper\Upper_Start\UpperStart.h
 * @Brief: 
 * 
 * Copyright (c) 2024 by ChenYiTong, All Rights Reserved. 
 */
#ifndef __UPPERSTART_H__
#define __UPPERSTART_H__

#include "main.h"
#include "can.h"
#include "gpio.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "wtr_calculate.h"
#include "cmsis_os.h"
#include "wtr_can.h"
#include "wtr_dji.h"
#include "ParamDef.h"
#include "UpperServo.h"
#include "StateMachine.h"
#include "string.h"
#include "math.h"
#include "STP_Decode.h" 
#include "USART_Init.h"
#include "target.h"
#include "upper_debug.h"
extern int flag[4];
extern int flag1;
#endif // __UPPERSTART_H__