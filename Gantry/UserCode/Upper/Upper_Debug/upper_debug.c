/*
 * @Author: X311
 * @Date: 2024-05-16 22:06:32
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-17 02:07:55
 * @FilePath: \Gantry\UserCode\Upper\Upper_Debug\upper_debug.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by ChenYiTong, All Rights Reserved. 
 */
#include "upper_debug.h"

/**
 * @brief 调试线程开启
 * 
 */
void Upper_Debug_TaskStart(void)
{
    const osThreadAttr_t upper_debug_Task_attributes = {
        .name       = "upper_debug_Task",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityNormal1,
    };
    osThreadNew(Upper_Debug_Task, NULL, &upper_debug_Task_attributes);
}

/**
 * @brief   调试线程
 */
void Upper_Debug_Task(void *argument)
{
    for (;;) {
        printf("%f,%f,%f\n", distance_aver[2], Upper[0].Motor_X->speedPID.output, Upper[0].Motor_Y->speedPID.output);
        osDelay(50);
    }
}