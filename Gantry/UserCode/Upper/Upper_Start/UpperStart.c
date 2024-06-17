/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-06-17 23:39:26
 * @FilePath: \Gantry\UserCode\Upper\Upper_Start\UpperStart.c
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

    // static union {
    //     uint8_t data[20];
    //     float weight_state[5];
    // } state;

    // if ((receive_buffer[0] == 0xFF) && (receive_buffer[1] == 0xFE) && (receive_buffer[22] == 0xFE) && (receive_buffer[23] == 0xFF)) {
    //      printf("11111111\n");
    //      for (int i = 0; i < 20; i++) {
    //         state.data[i] = receive_buffer[i + 2];
    //     }

    //     for (int t = 0; t < 5; t++) {
    //         weight_placement[t] = state.weight_state[t];
    //     }
    // }

    // weight_placement[0]=1;
    // weight_placement[1]=1;

    /*串口使能*/
    Gantry_usart_init();
    Upper_Decode_TaskStart();

    /*初始化函数*/
    
    Upper_StateMachine_Init_01();
    Upper_StateMachine_Init_02();
    osDelay(2000);
    Upper_Motor_init();

    HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET);   //  SET 气缸抬升
    HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_SET);   //  SET 气缸抬升
    HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(electromagnet_04_GPIO_Port, electromagnet_04_Pin, GPIO_PIN_SET);
    osDelay(600);
    /*开启线程*/
    
    Upper_StateMachine_TaskStart_01();
    Upper_StateMachine_TaskStart_02();
    Upper_Servo_TaskStart();
    Upper_Debug_TaskStart();

    for (;;) {
        //HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        osDelay(800);
    }
}