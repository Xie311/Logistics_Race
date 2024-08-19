/*
 * @Author: X311
 * @Date: 2024-08-18 15:28:04
 * @LastEditors: X311 
 * @LastEditTime: 2024-08-19 16:47:20
 * @FilePath: \Gantry_final\UserCode\Upper\Upper_Stepper\Stepper_motors.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
/*
 * @Author: X311
 * @Date: 2024-08-18 15:28:04
 * @LastEditors: X311 
 * @LastEditTime: 2024-08-19 15:21:36
 * @FilePath: \Gantry_final\UserCode\Upper\Upper_Stepper\Stepper_motors.c
 * @note: 0为逆时针转动向上
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
#include "Stepper_motors.h"

void Stepper_Servo(int Stepper_state,int step_num) // 一步1.8°
{
    HAL_GPIO_WritePin(ENA_GPIO_Port,ENA_Pin,0); // 共阴接法低电平使能

    if(Stepper_state==1) // dir为1时正转，dir为0时反转
    {
        HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_RESET);
    }

    for(int i=0;i<step_num;i++)
    {
        HAL_GPIO_WritePin(PUL_GPIO_Port,PUL_Pin,GPIO_PIN_SET); //两个PUL一步（一个脉冲）
        delay_us(delay_time);
        HAL_GPIO_WritePin(PUL_GPIO_Port,PUL_Pin,GPIO_PIN_RESET);
        delay_us(delay_time);
    }
}

// /**
//  * @brief 步进电机伺服线程开启
//  */
// void Stepper_TaskStart(void)
// {
//     const osThreadAttr_t upper_stepper_Task_attributes = {
//         .name       = "upper_stepperTask",
//         .stack_size = 128 * 10,
//         .priority   = (osPriority_t)osPriorityNormal,
//     };
//     osThreadNew(Upper_Stepper_Task, NULL, &upper_stepper_Task_attributes);
// }

// /**
//  * @brief   步进电机伺服线程
//  */
// void Upper_Stepper_Task(void *argument)
// {
//     osDelay(100);
//     for (;;) {
//         if(stepper_flag==1){
//             Stepper_Servo(0, 60);
//         }
        

//         osDelay(2);
//     }
// }
