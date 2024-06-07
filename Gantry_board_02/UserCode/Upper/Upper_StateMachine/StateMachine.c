/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-06-08 01:37:47
 * @FilePath: \Gantry_board_02\UserCode\Upper\Upper_StateMachine\StateMachine.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
#include "StateMachine.h"
#include <stdlib.h>
// 雷达单位为毫米
float initial_pos[3];
float current_pos[3];
uint16_t stateflag[2] = {0};
uint16_t stake_flag   = 0;
/****************线程相关函数********************/

/**
 * @brief board_02 状态机线程
 * @param argument 
 * @note 气缸0在地，1抬起
 * @note board_01 夹取放置2、3区的砝码
 * @note 雷达0、1反馈斜线距离
 *       雷达2反馈直线距离
 */
void Upper_State_Task(void *argument)
{
    osDelay(100);
    int index = *((int *)argument);

    for (;;){
        if(stake_flag == 0)
        {
             /***调试代码***/
            // Upper[index].gantry_t.position.y = 300.0;
            // Upper[index].gantry_t.position.x = 800.0;

            /***** 上电 定爪前进 *****/
            Upper[index].gantry_t.position.y = 1600.0;

            /***** 两个爪子前往砝码 *****/
            if (weight_placement[index] == 1) { // 砝码在内圈
                Upper[index].gantry_t.position.x = 250.0;
            } else if (weight_placement[index] == 0) { // 砝码在外圈
                Upper[index].gantry_t.position.x = 660.0;
            }
            /************************/

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 3) && (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 3)) {
                stake_flag = 1;
            }
        }

        else if (stake_flag == 1) {
            /***** 中央砝码夹取 *****/
            HAL_GPIO_WritePin(cylinder_05_GPIO_Port, cylinder_05_Pin, GPIO_PIN_RESET); // 气缸下落
            osDelay(500);
            KP                               = 60;
            Upper[index].gantry_t.position.y = 877.0;

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 6) && (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 6)) {
                osDelay(600);
                HAL_GPIO_WritePin(cylinder_05_GPIO_Port, cylinder_05_Pin, GPIO_PIN_SET); // 气缸向上
                osDelay(800);
                stake_flag = 2;
            }
        }

        else if(stake_flag == 2){
            /****** 前往木桩并放置第一个砝码 ******/
            KP = 20;
            Upper[index].gantry_t.position.y = 1600.0;

            if (fabs(Upper[index].gantry_t.position.y - distance_aver[2])< 4){
                KP = 0;
                osDelay(500);
                HAL_GPIO_WritePin(electromagnet_05_GPIO_Port, electromagnet_05_Pin, 0); // 砝码下电

                stake_flag  = 3;  
            }
        }

        else if (stake_flag == 3) {
            KP = 20;
            /***** 前往砝码 *****/
            Upper[index].gantry_t.position.y = 2532.5;

            if (weight_placement[index] == 1) { // 砝码在内圈
                Upper[index].gantry_t.position.x = 250.0;
            } else { // 砝码在外圈
                Upper[index].gantry_t.position.x = 660.0;
            }
            /************************/

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 3) || (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 3)) {
                stake_flag = 9;
                stateflag[index] = 1;
            }
            
        }

        else if (stateflag[index] == 1) {
            if (index == 0) {
                HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_RESET); // 气缸向下
            } else {
                HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_RESET); // 气缸向下
            }
            osDelay(500);
            
            /***** 前往砝码 *****/
            KP = 60;
            Upper[index].gantry_t.position.y = 2529.5;

            /***** 砝码位置 *****/
            if (index == 0) {
                if (weight_placement[index] == 1) { // 砝码在内圈
                    Upper[index].gantry_t.position.x = 190.0;
                } else if (weight_placement[index] == 0) { // 砝码在外圈
                    Upper[index].gantry_t.position.x = 558.0;
                }
            } else {
                if (weight_placement[index] == 1) { // 砝码在内圈
                    Upper[index].gantry_t.position.x = 192.0;
                } else if (weight_placement[index] == 0) { // 砝码在外圈
                    Upper[index].gantry_t.position.x = 564.0;
                }
            }
            /*****************/

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 6) && (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 6)) {
                stateflag[index] = 2;
            }

            // 如果两边均完成砝码夹取则前往木桩
            if ((stateflag[0] == 2) && (stateflag[1] == 2)) {
                osDelay(600);
                HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 气缸向上
                HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_SET); // 气缸向上
                osDelay(800);

                stake_flag = 4;
            }
        }

        else if (stake_flag == 4) {
            /***** 前往木桩 *****/
            KP                               = 20;
            Upper[index].gantry_t.position.y = 400.0;
            /***** 砝码位置 *****/
            if (index == 0) {
                Upper[index].gantry_t.position.x = 735.0;
            } else {
                Upper[index].gantry_t.position.x = 688.5;
            }
            /*****************/

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 4) || (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 3)) {
                stateflag[index] = 4;
            }

            if ((stateflag[0] == 4) && (stateflag[1] == 4)) {
                stake_flag = 5;
            }
        }

        else if (stake_flag == 5) {
            /***** 放下砝码 *****/
            KP = 0;
            osDelay(500);
            if (index == 0) {
                HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_RESET); // 气缸向下
                osDelay(1000);
                HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET); // 砝码下电
            }

            if (index == 1) {
                HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_RESET); // 气缸向下
                osDelay(1000);
                HAL_GPIO_WritePin(electromagnet_04_GPIO_Port, electromagnet_04_Pin, GPIO_PIN_RESET); // 砝码下电
            }
        }

        osDelay(8);
    }
}
           
/********************* 区域1 *********************/
void Upper_StateMachine_TaskStart_01(void)
{
    int *parameter = malloc(sizeof(int));
    *parameter = 0;

    osThreadAttr_t Upper_State_attributes = {
        .name       = "Upper_State",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(Upper_State_Task, parameter, &Upper_State_attributes);
}

void Upper_StateMachine_Init_01(void)
{
    Upper[0].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
    Upper[1].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
}

/********************* 区域2 *********************/
void Upper_StateMachine_TaskStart_02(void)
{
    int *parameter  = malloc(sizeof(int));
    *parameter      = 1;
    osThreadAttr_t Upper_State_attributes = {
        .name       = "Upper_State",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityAboveNormal,
    };
    osThreadNew(Upper_State_Task, parameter, &Upper_State_attributes);
}

void Upper_StateMachine_Init_02(void)
{
    Upper[0].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
    Upper[1].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
}

