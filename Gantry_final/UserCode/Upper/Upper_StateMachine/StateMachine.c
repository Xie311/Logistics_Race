/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-07-25 20:14:56
 * @FilePath: \Gantry_final\UserCode\Upper\Upper_StateMachine\StateMachine.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
#include "StateMachine.h"
#include <stdlib.h>
// 雷达单位为毫米
// float initial_pos[3];
// float current_pos[3];
uint16_t stateflag[4] = {0};
uint16_t stake_flag   = 0;
/****************线程相关函数********************/

/**
 * @brief board_02 状态机线程
 * @param argument 
 * @note 气缸0在地，1抬起
 * @note 雷达0、1、2、3反馈斜线距离
 *       雷达4反馈直线距离
 */
void Upper_State_Task(void *argument)
{
    osDelay(100);
    int index = *((int *)argument);

    for (;;){
        if(stake_flag == 0)
        {
            /***** 上电 定爪前进 *****/
             if (weight_placement[4] == 1) { // 砝码在内圈
                 Upper[index].gantry_t.position.y = 2220.0;
             } else { // 砝码在外圈
                 Upper[index].gantry_t.position.y = 1800.0;
             }

            /***** 四个爪子前往砝码 *****/
            if (weight_placement[index] == 1) { // 砝码在内圈
                Upper[index].gantry_t.position.x = 270.0;
            } else if (weight_placement[index] == 0) { // 砝码在外圈
                Upper[index].gantry_t.position.x = 640.0;
            }
            /************************/

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 3) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)) {
                stateflag[index] = 1;
            }

            if ((stateflag[0] == 1) && (stateflag[1] == 1) && (stateflag[2] == 1) && (stateflag[3] == 1)){
                stake_flag = 1;
            }
        }

        else if (stake_flag == 1) {
            /***** 中央砝码夹取 *****/
            osDelay(200);
            HAL_GPIO_WritePin(cylinder_05_GPIO_Port, cylinder_05_Pin, GPIO_PIN_RESET); // 气缸下落
            osDelay(400);
            KP                               = 40;
            /***** 定爪前进 *****/
             if (weight_placement[4] == 1) { // 砝码在内圈
                 Upper[index].gantry_t.position.y = 2286.0;  
             } else { // 砝码在外圈
                 Upper[index].gantry_t.position.y = 1836.0;
             }

             if ((fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)) {
                 stake_flag = 2;
             }
        }

        else if(stake_flag == 2){/.,><M
            /****** 定爪上升 ******/
            osDelay(300);
            HAL_GPIO_WritePin(cylinder_05_GPIO_Port, cylinder_05_Pin, GPIO_PIN_SET); // 气缸向上
            osDelay(400);
            stake_flag = 3;
        }

        // else if (stake_flag == 3) {
        //     /***** 预备定爪放置砝码动爪抓取砝码 *****/
        //     Upper[index].gantry_t.position.y = 2548.5;

        //     if (weight_placement[index] == 1) { // 砝码在内圈
        //         Upper[index].gantry_t.position.x = 270.0; 
        //     } else { // 砝码在外圈
        //         Upper[index].gantry_t.position.x = 640.0;  
        //     }

        //     /********************************************************************************/

        //     if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 2) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3)&& (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)) {
        //         stateflag[index] = 2;
        //     }

        //     if ((stateflag[0] == 2) && (stateflag[1] == 2) && (stateflag[2] == 2) && (stateflag[3] == 2)) {
        //         stake_flag = 15;
        //     }
        // }

        // else if (stateflag[index] == 2) {
        //     osDelay(300);
        //     // 定爪砝码下电
        //     HAL_GPIO_WritePin(electromagnet_05_GPIO_Port, electromagnet_05_Pin, 0);
        //     osDelay(600);

        //     if (index == 0) {
        //         HAL_GPIO_WritePin(cylinder_01_GPIO_Port, cylinder_01_Pin, GPIO_PIN_RESET); // 气缸向下
        //     } 
        //     else if (index == 1) {
        //         HAL_GPIO_WritePin(cylinder_02_GPIO_Port, cylinder_02_Pin, GPIO_PIN_RESET); // 气缸向下
        //     } 
        //     else if (index == 2) {
        //         HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_RESET); // 气缸向下
        //     } 
        //     else if (index == 3) {
        //         HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_RESET); // 气缸向下
        //     } 
        //     osDelay(500);
            
        //     /***** 前往砝码 *****/
        //     KP = 60;

        //     /***** 砝码位置 *****/
        //     if (index == 0) {
        //         if (weight_placement[index] == 1) { // 砝码在内圈
        //             Upper[index].gantry_t.position.x = 183.0;
        //         } else if (weight_placement[index] == 0) { // 砝码在外圈
        //             Upper[index].gantry_t.position.x = 565.0;
        //         }
        //     } else if(index == 1) {
        //         if (weight_placement[index] == 1) { // 砝码在内圈
        //             Upper[index].gantry_t.position.x = 190.0;
        //         } else if (weight_placement[index] == 0) { // 砝码在外圈
        //             Upper[index].gantry_t.position.x = 550.0;
        //         }
        //     }else if(index == 2){
        //         if (weight_placement[index] == 1) { // 砝码在内圈
        //             Upper[index].gantry_t.position.x = 190.0;
        //         } else if (weight_placement[index] == 0) { // 砝码在外圈
        //             Upper[index].gantry_t.position.x = 550.0;
        //         }
        //     }else if(index == 3){
        //         if (weight_placement[index] == 1) { // 砝码在内圈
        //             Upper[index].gantry_t.position.x = 190.0;
        //         } else if (weight_placement[index] == 0) { // 砝码在外圈
        //             Upper[index].gantry_t.position.x = 550.0;
        //         }
        //     }
        //     /*******************************************************************/

        //     if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 3) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 4) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 4)) {
        //         stateflag[index] = 3;
        //     }

        //     // 如果四侧均完成砝码夹取则前往木桩
        //     if ((stateflag[0] == 3) && (stateflag[1] == 3) && (stateflag[2] == 3) && (stateflag[3] == 3) ) {
        //         osDelay(600);
        //         HAL_GPIO_WritePin(cylinder_01_GPIO_Port, cylinder_01_Pin, GPIO_PIN_SET); // 气缸向上
        //         HAL_GPIO_WritePin(cylinder_02_GPIO_Port, cylinder_02_Pin, GPIO_PIN_SET); // 气缸向上
        //         HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 气缸向上
        //         HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_SET); // 气缸向上
        //         osDelay(500);

        //         stake_flag = 4;
        //     }
        // }

        // else if(stake_flag == 4){
        //     KP = 40;
        //     /***** 木桩位置 *****/
        //     if (index == 0) {
        //         Upper[index].gantry_t.position.x = 700.0;
        //     } else if (index == 1) {
        //         Upper[index].gantry_t.position.x = 690.0;
        //     } else if (index == 2) {
        //         Upper[index].gantry_t.position.x = 690.0;
        //     } else if (index == 3) {
        //         Upper[index].gantry_t.position.x = 690.0;
        //     }

        //     /***********************************************/
        //     if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 3) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 4) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 4)) {
        //         stateflag[index] = 4;
        //     }

        //     if ((stateflag[0] == 4) && (stateflag[1] == 4) && (stateflag[2] == 4) && (stateflag[3] == 4)) {
        //         stake_flag = 5;
        //     }
        // }

        // else if (stake_flag == 5) {
        //     /***** 前往B区木桩 *****/
        //     KP                               = 30;
        //     Upper[index].gantry_t.position.y = 3500.0;

        //     if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 3) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)) {
        //         stateflag[index] = 4;
        //     }

        //     if ((stateflag[0] == 4) && (stateflag[1] == 4)) {
        //         stake_flag = 6;
        //     }
        // }

        // else if (stake_flag == 6) {
        //     /***** 放置B区砝码 *****/
        //     osDelay(500);

        //     HAL_GPIO_WritePin(electromagnet_01_GPIO_Port, electromagnet_01_Pin, GPIO_PIN_RESET); // 砝码下电
        //     HAL_GPIO_WritePin(electromagnet_02_GPIO_Port, electromagnet_02_Pin, GPIO_PIN_RESET); // 砝码下电

        //     stake_flag = 7;
        // }

        // else if (stake_flag == 7) {
        //     osDelay(800);
        //     KP = 20;

        //     Upper[0].gantry_t.position.x = 750.0;
        //     Upper[1].gantry_t.position.x = 750.0;

        //     if ((fabs(Upper[0].gantry_t.position.x - distance_aver[0]) < 3) && (fabs(Upper[1].gantry_t.position.x - distance_aver[1]) < 3) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)) {
        //          stake_flag = 8;
        //     }
        // }

        // else if (stake_flag == 8) {
        //     osDelay(300);
        //     KP = 45;
        //     /***** 前往A区木桩 *****/
        //     Upper[index].gantry_t.position.y = 408.0;
        //     /***** 木桩位置 *****/
        //     if (index == 3) {
        //         Upper[index].gantry_t.position.x = 698.0;
        //     } else if (index == 4) {
        //         Upper[index].gantry_t.position.x = 698.0;
        //     }
        //     /*****************/

        //     if ((fabs(Upper[2].gantry_t.position.x - distance_aver[2]) < 3) && (fabs(Upper[3].gantry_t.position.x - distance_aver[3]) < 3) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)) {
        //         stake_flag = 9;
        //     }
        // }

        // else if (stake_flag == 9) {
        //     /***** 放下砝码 *****/
        //     osDelay(600);
        //     //KP = 0;
            
        //     HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET); // 砝码下电
        //     HAL_GPIO_WritePin(electromagnet_04_GPIO_Port, electromagnet_04_Pin, GPIO_PIN_RESET); // 砝码下电

        //     stake_flag = 10;
        // }

        // else if (stake_flag == 10) {
        //     osDelay(800);
        //     KP=20;

        //     if (index == 0) {
        //         Upper[index].gantry_t.position.x = 740.0;
        //     } else {
        //         Upper[index].gantry_t.position.x = 740.0;
        //     }
        // }

        osDelay(6);
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

/********************* 区域3 *********************/
void Upper_StateMachine_TaskStart_03(void)
{
    int *parameter  = malloc(sizeof(int));
    *parameter      = 2;
    osThreadAttr_t Upper_State_attributes = {
        .name       = "Upper_State",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityAboveNormal,
    };
    osThreadNew(Upper_State_Task, parameter, &Upper_State_attributes);
}

void Upper_StateMachine_Init_03(void)
{
    Upper[0].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
    Upper[1].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
}

/********************* 区域4 *********************/
void Upper_StateMachine_TaskStart_04(void)
{
    int *parameter  = malloc(sizeof(int));
    *parameter      = 3;
    osThreadAttr_t Upper_State_attributes = {
        .name       = "Upper_State",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityAboveNormal,
    };
    osThreadNew(Upper_State_Task, parameter, &Upper_State_attributes);
}

void Upper_StateMachine_Init_04(void)
{
    Upper[0].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
    Upper[1].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
}


