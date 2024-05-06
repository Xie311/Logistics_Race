/*
 * @Author Chen Yitong
 * @Date 2023-09-22 22:19:27
 * @LastEditors: x311 
 * @LastEditTime: 2024-05-03 03:09:04
 * @FilePath: \Chassis_02\UserCode\Chassis\Start\Chassis_Start.c
 * @brief 底盘启动文件
 *
 * Copyright (c) 2023 by ChenYiTong, All Rights Reserved.
 */
#include "Chassis_Start.h"

/**
 * @brief 默认任务，启动其他线程
 * @return {*}
 */
void StartDefaultTask(void const *argument)
{
    //================初始化====================
    Chassis_StateMachine_Init(); // 状态机初始化
    Chassis_Servo_Init();        // 底盘电机初始化
    Chassis_Target_Init();       // 上位机数据接收初始化
    Chassis_Perception_Init();   // 码盘初始化

    //================启动线程==================
    osDelay(15000); // 等待外设初始化结束
    Chassis_StateMachine_TaskStart(); // 状态机进程启动
    Chassis_Servo_TaskStart();        // 底盘伺服进程启动
    OPS_Debug_TaskStart();            // 码盘调试线程
    //Tar_Debug_TaskStart();          // 数据接收调试线程

    for (;;) {
        HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
        osDelay(800);
    }
}