/*
 * @Author: X311
 * @Date: 2024-05-16 22:06:32
 * @LastEditors: X311 
 * @LastEditTime: 2024-07-11 17:57:25
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
    osDelay(100);
    for (;;) {
        printf("%f,%f,%f,%d,%d\n",
               distance_aver[0], distance_aver[1], distance_aver[2],
            //    Upper[0].Motor_X->speedPID.output, Upper[1].Motor_X->speedPID.output,
            //    Upper[0].Motor_Y->speedPID.output, Upper[1].Motor_Y->speedPID.output,
            //    (int)stateflag[0], (int)stateflag[1], (int)stake_flag,
               (int)weight_placement[0], (int)weight_placement[1]);

        // printf("%d,%d,%d,%d,%d\n",
        //        receive_buffer[0],receive_buffer[22],receive_buffer[23],
        //        (int)weight_placement[0], (int)weight_placement[1]);
        osDelay(100);
    }
}