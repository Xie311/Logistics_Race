/*
 * @Author: X311
 * @Date: 2024-05-16 22:06:32
 * @LastEditors: X311 
 * @LastEditTime: 2024-06-06 23:37:16
 * @FilePath: \Gantry_board_02\UserCode\Upper\Upper_Debug\upper_debug.c
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
    osDelay(100);
    for (;;) {
        printf("%f,%f,%f,%2f,%2f,%2f,%2f,%d,%d,%d\n",
               distance_aver[0], distance_aver[1], distance_aver[2],
               Upper[0].Motor_X->speedPID.output, Upper[1].Motor_X->speedPID.output,
               Upper[0].Motor_Y->speedPID.output, Upper[1].Motor_Y->speedPID.output,
               (int)stateflag[0], (int)stateflag[1], (int)stake_flag);
        osDelay(100);
    }
}