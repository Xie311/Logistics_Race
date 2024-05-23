/*
 * @Author: X311
 * @Date: 2024-05-16 22:06:32
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-23 14:52:01
 * @FilePath: \Gantry\UserCode\Upper\Upper_Debug\upper_debug.c
 * 
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
        .priority   = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(Upper_Debug_Task, NULL, &upper_debug_Task_attributes);
}

/**
 * @brief   调试线程
 */
void Upper_Debug_Task(void *argument)
{
    for (;;) {
        printf("%f,%f,%f,%f,%f\n", distance_aver[0], distance_aver[1], distance_aver[2],current_pos[0], Upper[0].Motor_X->speedPID.output);
        osDelay(80);
    }
}