/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-21 14:06:04
 * @FilePath: \Gantry\UserCode\Upper\Upper_StateMachine\StateMachine.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
#include "StateMachine.h"
#include <stdlib.h>
// 雷达单位为毫米
#define Inner_ring_weights 2124.1224        // 雷达平面到内圈砝码半径的距离
#define Outer_ring_weights 5874.1224        // 雷达平面到外圈砝码半径的距离
#define weight_straight_distance  6275      // 雷达平面到砝码中心的直距离

#define Stake_straight_distance  2450       // 雷达平面到木桩中心的直距离
#define Stake_skew 6275                     // 雷达平面到木桩中心的斜距离

#define offset 730
#define claw_offset 1380.3 //?

float initial_pos[3];
float current_pos[3];
/****************线程相关函数********************/

/**
 * @brief 状态机线程
 * @param argument 
 * @note 气缸0在地，1抬起
 * @note board_01 夹取放置2、3区的砝码
 * @note 雷达0、1反馈斜线距离
 *       雷达2反馈直线距离
 */
void Upper_State_Task(void *argument)
{
    int index = *((int *)argument);
    uint16_t stateflag = 0;

    HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_RESET);           // 爪子不抬升
    HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET);   // 砝码上电

    for (;;){
        if(stateflag==0)
        {
            /***** 前往砝码 *****/
            Upper[index].gantry_t.position.y = weight_straight_distance;
            if (weight_placement[index + 2] != 0){  //砝码在内圈
                Upper[index].gantry_t.position.x = Inner_ring_weights - claw_offset; 
            }
            else{                                   // 砝码在外圈
                Upper[index].gantry_t.position.x = Outer_ring_weights - claw_offset;
            }

        //     TickType_t StartTick = xTaskGetTickCount();
        //     initial_pos[index]   = distance_aver[index];
        //     initial_pos[2]       = distance_aver[2];
        //     _Bool isArray1       = 0;
        //     float diff[2]        = {0};
        //     do {
        //         TickType_t CurrentTick = xTaskGetTickCount();
        //         float current_time     = (CurrentTick - StartTick) * 1.0 / 1000.0;
        //         VelocityPlanning(initial_pos[0], 2000, 50, Upper[index].gantry_t.position.x, current_time, &(current_pos[index]));
        //         VelocityPlanning(initial_pos[1], 1000, 50, Upper[index].gantry_t.position.y, current_time, &(current_pos[2]));
        //         diff[0] = fabs(Upper[index].gantry_t.position.x - current_pos[index]);
        //         diff[1] = fabs(Upper[index].gantry_t.position.y - current_pos[2]);
        //         if ((diff[0] < 0.01) && (diff[1] < 0.01)) { isArray1 = 1; }

        //     } while (!isArray1);
        //     stateflag = 1;
        // }

        // else if (stateflag == 1) 
        // {
        //     /***** 抓取砝码 *****/    // 测完再看看是否保留
        //     Upper[index].Motor_X->speedPID.KP = 0;
        //     Upper[index].Motor_X->speedPID.KI = 0;
        //     Upper[index].Motor_X->speedPID.KD = 0;
        //     Upper[index].Motor_Y->speedPID.KP = 0;
        //     Upper[index].Motor_Y->speedPID.KI = 0;
        //     Upper[index].Motor_Y->speedPID.KD = 0;
        //     HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET); // 砝码上电
        //     osDelay(800);
        //     HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET);  // 气缸抬升
        //     Upper[index].Motor_X->speedPID.integral = 0;
        //     Upper[index].Motor_Y->speedPID.integral = 0;
        //     Upper[index].Motor_X->speedPID.KP       = 15;
        //     Upper[index].Motor_X->speedPID.KI       = 2;
        //     Upper[index].Motor_X->speedPID.KD       = 5;
        //     Upper[index].Motor_Y->speedPID.KP       = 0;
        //     Upper[index].Motor_Y->speedPID.KI       = 0;
        //     Upper[index].Motor_Y->speedPID.KD       = 0;
        //     stateflag = 2;
        // }

        // else if (stateflag == 2) 
        // {
        //     /***** 前往木桩 *****/
        //     Upper[index].gantry_t.position.x   = Stake_skew - claw_offset;
        //     Upper[index].gantry_t.position.y   = Stake_straight_distance;
        //     TickType_t StartTick           = xTaskGetTickCount();
        //     initial_pos[0]                 = distance_aver[index];
        //     initial_pos[1]                 = distance_aver[2];
        //     _Bool isArray2                 = 0;
        //     float diff[2]                  = {0};
        //     do {
        //         TickType_t CurrentTick = xTaskGetTickCount();
        //         float current_time     = (CurrentTick - StartTick) * 1.0 / 1000.0;
        //         VelocityPlanning(initial_pos[0], 2000, 50, Upper[index].gantry_t.position.x, current_time, &(current_pos[index]));
        //         VelocityPlanning(initial_pos[1], 1000, 50, Upper[index].gantry_t.position.y, current_time, &(current_pos[2]));
        //         diff[0] = fabs(Upper[index].gantry_t.position.x - current_pos[index]);
        //         diff[1] = fabs(Upper[index].gantry_t.position.y - current_pos[2]);
        //         if ((diff[0] < 0.01) && (diff[1] < 0.01)) { isArray2 = 1; }

        //     } while (!isArray2);
        //     stateflag = 3;
        // } 
        
        // else if (stateflag == 3) 
        // {
        //     /***** 放下砝码 *****/
        //     Upper[index].Motor_X->speedPID.KP = 0;
        //     Upper[index].Motor_X->speedPID.KI = 0;
        //     Upper[index].Motor_X->speedPID.KD = 0;
        //     Upper[index].Motor_Y->speedPID.KP = 0;
        //     Upper[index].Motor_Y->speedPID.KI = 0;
        //     Upper[index].Motor_Y->speedPID.KD = 0;
        //     HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_RESET);  //气缸向下
        //     osDelay(1000);
        //     HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET); // 砝码下电
        }

        osDelay(10);
    }
}
            // /************************砝码在内圈************************/

            // {
            //     /***前往砝码***/
            //     do {
            //         Upper[index].gantry_t.position.x = Inner_ring_weights - claw_offset; // 从后往前推拿砝码，这个值应该得再小点
            //         Upper[index].gantry_t.position.y = weight_straight_distance;
            //     } while (((distance_aver[index] - Upper[index].gantry_t.position.x) < 50) || ((Upper[index].gantry_t.position.y - distance_aver[2]) < 50)); // 距离砝码5cm时电磁铁上电

            //     /***抓取砝码***/
            //     // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin,GPIO_PIN_SET);
            //     HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET); // 砝码上电
            //     do {
            //         Upper[index].gantry_t.position.x = Inner_ring_weights - claw_offset;
            //         Upper[index].gantry_t.position.y = weight_straight_distance;
            //     } while (((distance_aver[index] - Upper[index].gantry_t.position.x) < 1) || ((Upper[index].gantry_t.position.y - distance_aver[2]) < 1));

            //     osDelay(500);
            //     HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 爪子抬升

            //     /***前往木桩***/
            //     do {
            //         Upper[index].gantry_t.position.x = Stake_skew - claw_offset;
            //         Upper[index].gantry_t.position.y = Stake_straight_distance;
            //     } while (((distance_aver[index] - Upper[index].gantry_t.position.x) < 1) || (fabs(distance_aver[2] - Upper[0].gantry_t.position.y)) < 1);

            //     /***放置砝码***/
            //     osDelay(200);
            //     HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 爪子放下
            //     osDelay(500);
            //     HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET); // 电磁铁下电
            // }
            // /************************砝码在外圈************************/
            // else{
            // /***前往砝码***/
            // do {
            //     Upper[index].gantry_t.position.x = Outer_ring_weights - claw_offset; // 从后往前推拿砝码，这个值应该得再小点
            //     Upper[index].gantry_t.position.y = weight_straight_distance;
            // } while (((distance_aver[index] - Upper[index].gantry_t.position.x) < 50) || ((Upper[index].gantry_t.position.y - distance_aver[2]) < 50)); // 距离砝码5cm时电磁铁上电

            // /***抓取砝码***/
            // // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin,GPIO_PIN_SET);
            // HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET); // 砝码上电
            // do {
            //     Upper[index].gantry_t.position.x = Outer_ring_weights - claw_offset;
            //     Upper[index].gantry_t.position.y = weight_straight_distance;
            // } while (((distance_aver[index] - Upper[index].gantry_t.position.x) < 1) || ((Upper[index].gantry_t.position.y - distance_aver[2]) < 1));

            // osDelay(500);
            // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 爪子抬升

            // /***前往木桩***/
            // do {
            //     Upper[index].gantry_t.position.x = Stake_skew - claw_offset;
            //     Upper[index].gantry_t.position.y = Stake_straight_distance;
            // } while (((distance_aver[index] - Upper[index].gantry_t.position.x) < 1) || (fabs(distance_aver[2] - Upper[0].gantry_t.position.y)) < 1);

            // /***放置砝码***/
            // osDelay(200);
            // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 爪子放下
            // osDelay(500);
            // HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET); // 电磁铁下电


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

void Upper_StateMachine_Init_02(void)
{
    Upper[0].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
    Upper[1].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
}
