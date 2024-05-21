/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-17 13:27:29
 * @FilePath: \Gantry\UserCode\Upper\Upper_Start\UpperStart.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by ChenYiTong, All Rights Reserved. 
 */
#include "UpperStart.h"

/**
 * @brief 默认任务，启动其他线程
 *
 * @param argument
 */
void StartDefaultTask(void *argument)
{
    /*初始化函数*/
    Upper_StateMachine_Init_01();
   //Upper_StateMachine_Init_02();
    Upper_Motor_init();
   
    /*串口使能*/
    Gantry_usart_init();

    osDelay(1000);
    /*开启线程*/
    Upper_Decode_TaskStart();
    Upper_StateMachine_TaskStart_01(0);
    // Upper_StateMachine_TaskStart_02(1);
    Upper_Servo_TaskStart();
    Upper_Debug_TaskStart();

    for (;;) {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        osDelay(800);
    }
}