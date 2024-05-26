/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-26 22:17:05
 * @FilePath: \Gantry_board_02\UserCode\Upper\Upper_Start\UpperStart.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
#include "UpperStart.h"

/**
 * @brief 默认任务，启动其他线程
 *
 * @param argument
 */
void StartDefaultTask(void *argument)
{
    /*** 接受树莓派串口初始化 ***/
    Upper_Target_Init();

    Target_Decode_TaskStart();
    while (Uart_State != 2) {
        ; // 若未收到上位机数据则一直循环
    }

    /*串口使能*/
    Gantry_usart_init();

    /*初始化函数*/
    Upper_StateMachine_Init_01();
    Upper_StateMachine_Init_02();
    Upper_Motor_init();
   
    HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(electromagnet_04_GPIO_Port, electromagnet_04_Pin, GPIO_PIN_SET);
    //osDelay(500);
    /*开启线程*/
    Upper_Decode_TaskStart();
    Upper_StateMachine_TaskStart_01();
    Upper_StateMachine_TaskStart_02();
    Upper_Servo_TaskStart();
    Upper_Debug_TaskStart();

    for (;;) {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        osDelay(800);
    }
}