/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-18 00:12:23
 * @FilePath: \Gantry\UserCode\Upper\Upper_StateMachine\StateMachine.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
#include "StateMachine.h"
#include <stdlib.h>
#define Inner_ring_weights 212.41224
#define Outer_ring_weights 587.41224
#define Slash_Length 600
#define Stake_location  627.5
/****************线程相关函数********************/

/**
 * @brief 状态机线程
 * 
 * @param argument 
 */
void Upper_State_Task(void *argument)
{
    int index = *((int *)argument);
    for (;;){
        /************************砝码在内圈************************/
        if (weight_placement[index] != 0)  
        {
            /***前往砝码***/
            /*全速段*/
            do{
                Upper[0].gantry_t.position.x = Inner_ring_weights;
                Upper[0].gantry_t.position.y = 0;
            } while ((distance_aver[0] - Upper[0].gantry_t.position.x) < 30); // 距离砝码3cm时电磁铁上电

            /*减速段*/
            // 待加入

            /***抓取砝码***/
            // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET);  //砝码上电
            do {
                Upper[0].gantry_t.position.x = Inner_ring_weights;
                Upper[0].gantry_t.position.y = 0;
            } while ((distance_aver[0] - Upper[0].gantry_t.position.x) < 1);

            osDelay(500);
            HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET);  // 爪子抬升

            /***前往木桩***/
            do
            {
                Upper[0].gantry_t.position.x = Slash_Length; // 到达蝴蝶结末端
                Upper[0].gantry_t.position.y = Stake_location;
            } while (((distance_aver[0] - Upper[0].gantry_t.position.x) < 1) || (fabs(distance_aver[1] - Upper[0].gantry_t.position.y)) < 1);

            /***放置砝码***/
            osDelay(200);
            HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 爪子放下
            osDelay(1000);
            HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET);  // 电磁铁下电
        }
        /************************砝码在外圈************************/
        else{
            // /***前往砝码***/
            // /*全速段*/
            // do {
                Upper[0].gantry_t.position.x = Outer_ring_weights;
                Upper[0].gantry_t.position.y = 0;
            // } while ((distance_aver[0] - Upper[0].gantry_t.position.x) < 30); // 距离砝码3cm时电磁铁上电

            // /*减速段*/
            // // 待加入

            // /***抓取砝码***/
            // // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin,GPIO_PIN_SET);
            // HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET); // 砝码上电
            // do {
            //     Upper[0].gantry_t.position.x = Outer_ring_weights;
            //     Upper[0].gantry_t.position.y = 0;
            // } while ((distance_aver[0] - Upper[0].gantry_t.position.x) < 1);  // 爪子前行，吸砝码

            // osDelay(500);
            // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 爪子抬升

            // /***前往木桩***/
            // do {
            //     Upper[0].gantry_t.position.x = Slash_Length; // 到达蝴蝶结末端
            //     Upper[0].gantry_t.position.y = Stake_location;
            // } while (((distance_aver[0] - Upper[0].gantry_t.position.x) < 1) || (fabs(distance_aver[1] - Upper[0].gantry_t.position.y)) < 1);

            // /***放置砝码***/
            // osDelay(200);
            // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 爪子放下
            // osDelay(500);
            // HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET); // 电磁铁下电
        }
        osDelay(10);
    }
}

/********************* 区域1 *********************/
void Upper_StateMachine_TaskStart_01(int index)
{
    int *parameter = malloc(sizeof(int));
    *parameter = 3;

    osThreadAttr_t Upper_State_attributes = {
        .name       = "Upper_State",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityAboveNormal,
    };
    osThreadNew(Upper_State_Task, parameter, &Upper_State_attributes);
}

void Upper_StateMachine_Init_01(void)
{
    Upper[0].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
    Upper[1].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
}

/********************* 区域2 *********************/
void Upper_StateMachine_TaskStart_02(int index)
{
    int *parameter  = malloc(sizeof(int));
    *parameter      = 4;
    osThreadAttr_t Upper_State_attributes = {
        .name       = "Upper_State",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityAboveNormal,
    };
    osThreadNew(Upper_State_Task, parameter, &Upper_State_attributes);
}

void Upper_StateMachine_Init_02()
{
    Upper[0].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
    Upper[1].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
}
