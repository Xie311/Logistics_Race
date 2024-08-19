/*** 
 * @Author: doge60 3020118317@qq.com
 * @Date: 2024-08-17 11:00:50
 * @LastEditors: doge60 3020118317@qq.com
 * @LastEditTime: 2024-08-17 17:22:51
 * @FilePath: \servo_test\User\Stepper_motors\Stepper_motors.h
 * @Description: 
 * @
 * @Copyright (c) 2024 by doge60 3020118317@qq.com, All Rights Reserved. 
 */
#ifndef STEPPER_MOTORS_H
#define STEPPER_MOTORS_H

#include "UpperStart.h"

#define delay_time 500 // 修改延时可以调速

// void Stepper_init(void);
void Stepper_Servo(int Stepper_state,int step_num); // 步进电机转step_num步的封装函数（一圈200步，4细分下一圈800脉冲，一脉冲0.45°）；Stepper_state为1时正转，Stepper_state为0时反转

#endif // STEPPER_MOTORS_H