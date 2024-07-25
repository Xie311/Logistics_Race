/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-07-25 22:06:28
 * @FilePath: \Gantry_final\UserCode\Upper\Upper_Start\UpperStart.c
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
    // /*** 接受树莓派串口初始化 ***/
    // Upper_Target_Init();
    // Upper_Target_Decode();

    /**** 测试代码 ****/
//    weight_placement[0]=1;
//    weight_placement[1]=1;
//    weight_placement[2]=1;
   

    /*串口使能*/
    Gantry_usart_init();

    /*初始化函数*/
    Upper_StateMachine_Init_01();
    Upper_StateMachine_Init_02();
    Upper_StateMachine_Init_03();
    Upper_StateMachine_Init_04();
    osDelay(2000);
    Upper_Motor_init();

    HAL_GPIO_WritePin(cylinder_01_GPIO_Port, cylinder_01_Pin, GPIO_PIN_SET);       // SET气缸抬升
    HAL_GPIO_WritePin(cylinder_02_GPIO_Port, cylinder_02_Pin, GPIO_PIN_SET);       // SET气缸抬升
    HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET);       // SET气缸抬升
    HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_SET);       // SET气缸抬升
    HAL_GPIO_WritePin(cylinder_05_GPIO_Port, cylinder_05_Pin, GPIO_PIN_SET);       // SET气缸抬升

    HAL_GPIO_WritePin(electromagnet_01_GPIO_Port, electromagnet_01_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(electromagnet_02_GPIO_Port, electromagnet_02_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(electromagnet_04_GPIO_Port, electromagnet_04_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(electromagnet_05_GPIO_Port, electromagnet_05_Pin, GPIO_PIN_SET);
    osDelay(600);

    /*开启线程*/
    Upper_Decode_TaskStart();
    Upper_StateMachine_TaskStart_01();
    Upper_StateMachine_TaskStart_02();
    Upper_StateMachine_TaskStart_03();
    Upper_StateMachine_TaskStart_04();
    Upper_Servo_TaskStart();
    Upper_Debug_TaskStart();

    for (;;) {
        osDelay(800);
    }
}