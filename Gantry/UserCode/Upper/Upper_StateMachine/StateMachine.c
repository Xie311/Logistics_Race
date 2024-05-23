/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-23 23:23:39
 * @FilePath: \Gantry\UserCode\Upper\Upper_StateMachine\StateMachine.c
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
    osDelay(100);
    int index = *((int *)argument);
    uint16_t stateflag = 0;

    //HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_RESET);           // 爪子不抬升
    //HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET);   // 砝码上电

    for (;;){
        if(stateflag==0)
        {
            /***** 前往砝码 *****/
            Upper[index].gantry_t.position.y = 906.50;
            if (weight_placement[index] != 0){  //砝码在内圈
                Upper[index].gantry_t.position.x = 620;
            }
            else{ // 砝码在外圈
                Upper[index].gantry_t.position.x = 620.0;
            }

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 1) && (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 1))
            {
                stateflag = 1;
            }
        } 

        // else if (stateflag == 1)
        // {
        //     KP = 10;
        //     /***** 前往砝码 *****/
        //     Upper[index].gantry_t.position.y = 906.50;
        //     if (weight_placement[index] != 0) { // 砝码在内圈
        //         Upper[index].gantry_t.position.x = 620;
        //     } else { // 砝码在外圈
        //         Upper[index].gantry_t.position.x = 620;
        //     }

        //     if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 1) && (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 1)) {
        //         stateflag = 2;
        //         KP  = 0;
        //     }
        // }

        // else if (stateflag == 2) {
        //     KP = 30;
        //     /***** 前往砝码 *****/
        //     Upper[index].gantry_t.position.y = 916.50;
        //     if (weight_placement[index] != 0) { // 砝码在内圈
        //         Upper[index].gantry_t.position.x = 500;
        //     } else { // 砝码在外圈
        //         Upper[index].gantry_t.position.x = 617.5;
        //     }

        //     if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 1) && (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 1)) {
        //         stateflag = 2;
        //     }
        // }

        // else if (stateflag == 3) {
        //     /***** 抓取砝码 *****/
        //     KP = 0;
        //     osDelay(500);
        //     //HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET);  // 气缸抬升
        //     stateflag = 4;
        // }

        // else if (stateflag == 3)
        // {
        //     /***** 前往木桩 *****/
        //     KP = 60;
        //     Upper[index].gantry_t.position.x   = 700;
        //     Upper[index].gantry_t.position.y   = 300;

        //     if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 1) || (fabs(Upper[index].gantry_t.position.y - distance_aver[2]) < 1)) {
        //         stateflag = 4;
        //     }
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
            //}

            osDelay(8);
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

