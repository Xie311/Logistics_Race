/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-26 21:44:53
 * @FilePath: \Gantry_board_02\UserCode\Upper\Upper_StateMachine\StateMachine.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
#include "StateMachine.h"
#include <stdlib.h>
// 雷达单位为毫米
#define Inner_ring_weights 212.41224        // 雷达平面到内圈砝码半径的距离
#define Outer_ring_weights 587.41224        // 雷达平面到外圈砝码半径的距离
#define weight_straight_distance  627.5      // 雷达平面到砝码中心的直距离

#define Stake_straight_distance  245.0       // 雷达平面到木桩中心的直距离
#define Stake_skew 627.5                     // 雷达平面到木桩中心的斜距离

#define offset 730
#define claw_offset 138.03 //?

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
        if(stateflag[index]==0)
        {
            /***** 上电 定爪猪突猛进 *****/
            Upper[index].gantry_t.position.y = 877.0;

            if (weight_placement[index] == 1) {  //砝码在内圈
                Upper[index].gantry_t.position.x = 615.0; 
            }
            else if(weight_placement[index] == 0) {                                              // 砝码在外圈
                Upper[index].gantry_t.position.x = 650.0;  //630
            }

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 2) && (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 2))
            {
                stateflag[index] = 1;
            }

            if ((stateflag[0] == 1) && (stateflag[1] == 1)){
                stake_flag = 1;
            }
        }

        else if (stake_flag==1) {
            /***** 前往中央木桩 *****/
            osDelay(500);
            //KP = 50;
            Upper[index].gantry_t.position.y = 877.0;
            if (weight_placement[index] == 1) { // 砝码在内圈
                Upper[index].gantry_t.position.x = 570.0;
            } 
            else if (weight_placement[index] == 0) { // 砝码在外圈
                Upper[index].gantry_t.position.x = 570.0;
            }

            /***放下第一个砝码***/
            osDelay(500);
            HAL_GPIO_WritePin(cylinder_05_GPIO_Port, cylinder_05_Pin, GPIO_PIN_RESET); // 气缸下落
            osDelay(1000);
            HAL_GPIO_WritePin(electromagnet_05_GPIO_Port, electromagnet_05_Pin, GPIO_PIN_RESET);
            osDelay(1000);

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 5) && (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 5)) {
                stateflag[index] = 2;
            }

            //夹取第二组砝码
            if ((stateflag[0] == 2) && (stateflag[1] == 2)) {
                stake_flag = 2;
            }
        }

        else if (stake_flag == 2) {
            /***** 前往砝码 *****/
            Upper[index].gantry_t.position.y = 877.0;
            if (weight_placement[index] == 1) { // 砝码在内圈
                Upper[index].gantry_t.position.x = 570.0;
            } else if (weight_placement[index] == 0) { // 砝码在外圈
                Upper[index].gantry_t.position.x = 570.0;
            }

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 3) || (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 3)) {
                stateflag[index] = 3;
            }
        } 
        
        else if (stateflag[index] == 1) {
            /***** 前往砝码 *****/
            Upper[index].gantry_t.position.y = 877.0;
            if (weight_placement[index] == 1) { // 砝码在内圈
                Upper[index].gantry_t.position.x = 570.0;
            }
             else if (weight_placement[index] == 0) { // 砝码在外圈
                Upper[index].gantry_t.position.x = 570.0;
            }

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 5) && (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 5)) {
                stateflag[index] = 2;
            }

            // 如果两边均完成砝码夹取则前往木桩
            if ((stateflag[0] == 2) && (stateflag[1] == 2)) {
                stake_flag = 3;
            }
        }


        else if (stake_flag == 3) {
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

