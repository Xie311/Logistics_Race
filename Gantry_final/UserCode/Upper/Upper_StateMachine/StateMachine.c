/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-08-03 21:16:00
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

uint16_t tick_flag_01 = 0;
uint16_t tick_flag_02 = 0;
uint16_t tick_flag_03 = 0;
uint16_t tick_flag_04 = 0;
uint16_t tick_flag_05 = 0;

uint16_t tick_flag[4] = {0};

float start_tick_05   = 0;
float cur_tick_05     = 0;
float start_tick_01   = 0;
float cur_tick_01     = 0;
float start_tick_02   = 0;
float cur_tick_02     = 0;
float start_tick_03   = 0;
float cur_tick_03     = 0;
float start_tick_04   = 0;
float cur_tick_04     = 0;

float start_tick[4]   = {0};
float cur_tick[4] = {0};
float pro_tick[4] = {0};

float delay_tick = 2000;
/****************线程相关函数********************/

/**
 * @brief board_02 状态机线程
 * @param argument
 * @note 气缸0在地，1抬起
 * @note 雷达0、1、2、3反馈斜线距离
 *       雷达4反馈直线距离
 * @note 启动时直线雷达距离为1227
 */
void Upper_State_Task(void *argument)
{
    osDelay(100);
    int index = *((int *)argument);

    for (;;){
        if(stake_flag == 0)
        {
            KP = 28;
            /***** 上电 定爪前进 *****/
            if (weight_placement[4] == 1) { // 砝码在内圈
                Upper[index].gantry_t.position.y = 1984.0;
             } else { // 砝码在外圈
                 Upper[index].gantry_t.position.y = 1634.0; 
             }

            /***** 四个爪子前往砝码 *****/
            if (weight_placement[index] == 1) { // 砝码在内圈
                Upper[index].gantry_t.position.x = 340.0;
            } else if (weight_placement[index] == 0) { // 砝码在外圈
                Upper[index].gantry_t.position.x = 700.0;
            }
            /************************/


            /**** y轴到达预设位置 ****/
            if ((fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 5)) {
                stake_flag = 1;
                //stateflag[index] = 1;
            }

            // if ((stateflag[0] == 1) && (stateflag[1] == 1) && (stateflag[2] == 1) && (stateflag[3] == 1)){
                
            // }
        }

        else if (stake_flag == 1) {
            /***** 中央砝码夹取 *****/
            /****** 定爪改成步进电机驱动！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！视情况调整延时！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！ ！！！！！ ******/
            // osDelay(200);
            HAL_GPIO_WritePin(cylinder_05_GPIO_Port, cylinder_05_Pin, GPIO_PIN_RESET); // 气缸下落
            osDelay(400);

            KP = 68;
            /***** 定爪前进（参数给的要比实际更前一点点，因为爪子能推着砝码往前走，有超时保护不用担心走不动了卡着不动） *****/
            if (weight_placement[4] == 1) { // 砝码在内圈
                 Upper[index].gantry_t.position.y = 2074.0;  //2041.5
             } else { // 砝码在外圈
                 Upper[index].gantry_t.position.y = 1688.0; //1670
             }

            if(tick_flag_05==0){
                start_tick_05  = xTaskGetTickCount();
                tick_flag_05 = 1;
            }
            cur_tick_05 = xTaskGetTickCount();
            

            float pro_tick_05 = cur_tick_05 - start_tick_05;

            /******* 当机构到达预设位置或爪子落下后的时间大于2500ms（可能出现吸上砝码后负载大走不动）爪子上升 *******/
            if (((fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3))||(pro_tick_05>=2500)) {
            //if (pro_tick_05 >= delay_tick) {
                 stake_flag = 2;
             }
        }

        else if(stake_flag == 2){
            /****** 定爪改成步进电机驱动！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！视情况调整延时！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！ ！！！！！ ******/
            osDelay(650);
            HAL_GPIO_WritePin(cylinder_05_GPIO_Port, cylinder_05_Pin, GPIO_PIN_SET); // 气缸向上
            osDelay(400);
            stake_flag = 3;
        }

        else if (stake_flag == 3) {
            /***** 预备动爪抓取砝码 *****/
            KP                               = 30;
            Upper[index].gantry_t.position.y = 2426.416748;

            if (weight_placement[index] == 1) { // 砝码在内圈
                Upper[index].gantry_t.position.x = 340.0; 
            } else { // 砝码在外圈
                Upper[index].gantry_t.position.x = 700.0;  
            }

            /********************************************************************************/

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 4) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 4)&& (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 4)) {
                stateflag[index] = 2;
            }

            if ((stateflag[0] == 2) && (stateflag[1] == 2) && (stateflag[2] == 2) && (stateflag[3] == 2)) {
                stake_flag = 4;
            }
        }

        else if (stake_flag == 4) {

            /**** 定爪放置中间砝码，四个动爪气缸下落并吸取砝码 ****/
            /****** 定爪改成步进电机驱动！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！视情况调整延时！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！ ！！！！！ ******/
            // 定爪下落（不知道用不用）
            HAL_GPIO_WritePin(electromagnet_05_GPIO_Port, electromagnet_05_Pin, GPIO_PIN_RESET);
            // 定爪上升（防止撞到钩子）  钩子！

            HAL_GPIO_WritePin(cylinder_01_GPIO_Port, cylinder_01_Pin, GPIO_PIN_RESET); // 气缸向下
            HAL_GPIO_WritePin(cylinder_02_GPIO_Port, cylinder_02_Pin, GPIO_PIN_RESET); // 气缸向下
            HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_RESET); // 气缸向下
            HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_RESET); // 气缸向下
            
            osDelay(500);
            
            /***** 前往砝码 *****/
            KP = 68;

            /***** 砝码位置（参数给的要比实际更前一点点，因为爪子能推着砝码往前走，有超时保护不用担心走不动了卡着不动） *****/
            if (index == 0) {
                if (weight_placement[index] == 1) { // 砝码在内圈
                    Upper[index].gantry_t.position.x = 200.0;
                } else if (weight_placement[index] == 0) { // 砝码在外圈
                    Upper[index].gantry_t.position.x = 568.0;
                }
            } else if(index == 1) {
                if (weight_placement[index] == 1) { // 砝码在内圈
                    Upper[index].gantry_t.position.x = 200.0;
                } else if (weight_placement[index] == 0) { // 砝码在外圈
                    Upper[index].gantry_t.position.x = 568.0;
                }
            }else if(index == 2){
                if (weight_placement[index] == 1) { // 砝码在内圈
                    Upper[index].gantry_t.position.x = 200.0;
                } else if (weight_placement[index] == 0) { // 砝码在外圈
                    Upper[index].gantry_t.position.x = 574.0; 
                }
            }else if(index == 3){
                if (weight_placement[index] == 1) { // 砝码在内圈
                    Upper[index].gantry_t.position.x = 202.0;
                } else if (weight_placement[index] == 0) { // 砝码在外圈
                    Upper[index].gantry_t.position.x = 564.0; 
                }
            }

            
            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 8) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 4) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 4)) {
                     stateflag[index] = 3;
            }

            /******** 超时保护，防止卡死 ********/
            if(index==0){
                if (tick_flag_01 == 0) {
                    start_tick_01 = xTaskGetTickCount();
                    tick_flag_01  = 1;
                }
                cur_tick_01 = xTaskGetTickCount();

                float pro_tick_01 = cur_tick_01 - start_tick_01;

                if(pro_tick_01>=2500){
                    stateflag[0] = 3;
                }
            }

            else if (index == 1) {
                if (tick_flag_02 == 0) {
                    start_tick_02 = xTaskGetTickCount();
                    tick_flag_02  = 1;
                }
                cur_tick_02 = xTaskGetTickCount();

                float pro_tick_02 = cur_tick_02 - start_tick_02;

                if (pro_tick_02 >= 2500) {
                    stateflag[1] = 3;
                }
            }

            else if (index == 2) {
                if (tick_flag_03 == 0) {
                    start_tick_03 = xTaskGetTickCount();
                    tick_flag_03  = 1;
                }
                cur_tick_03 = xTaskGetTickCount();

                float pro_tick_03 = cur_tick_03 - start_tick_03;

                if (pro_tick_03 >= 2500) {
                    stateflag[2] = 3;
                }
            }

            else if (index == 3) {
                if (tick_flag_04 == 0) {
                    start_tick_04 = xTaskGetTickCount();
                    tick_flag_04  = 1;
                }
                cur_tick_04 = xTaskGetTickCount();

                float pro_tick_04 = cur_tick_04 - start_tick_04;

                if (pro_tick_04 >= 2500) {
                    stateflag[3] = 3;
                }
            }
            /**********************************************************/

            // 原本想建一个数组来存标志位不知为何寄了。改成四个独立变量就好了罢了罢了不管了
            // if (tick_flag[index] == 0) {
            //     start_tick[index] = xTaskGetTickCount();
            //     tick_flag[index]  = 1;
            // }
            // cur_tick_05 = xTaskGetTickCount();
            // pro_tick[index] = cur_tick[index] - start_tick[index];


            // 如果四侧均完成砝码夹取则前往木桩
            if ((stateflag[0] == 3) && (stateflag[1] == 3) && (stateflag[2] == 3) && (stateflag[3] == 3) ) {
                osDelay(500);
                HAL_GPIO_WritePin(cylinder_01_GPIO_Port, cylinder_01_Pin, GPIO_PIN_SET); // 气缸向上
                HAL_GPIO_WritePin(cylinder_02_GPIO_Port, cylinder_02_Pin, GPIO_PIN_SET); // 气缸向上
                HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 气缸向上
                HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_SET); // 气缸向上
                osDelay(500);

                stake_flag = 5;
            }
        }

        else if(stake_flag == 5){
            KP = 30;
            /***** 木桩位置 *****/

            Upper[0].gantry_t.position.x = 708.0;
            Upper[1].gantry_t.position.x = 696.0;
            Upper[2].gantry_t.position.x = 711.0;
            Upper[3].gantry_t.position.x = 700.0;

            /***********************************************/
            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 3) ) {
                stateflag[index] = 4;
            }

            if ((stateflag[0] == 4) && (stateflag[1] == 4) ) {
                stake_flag = 6;
            }
        }

        else if (stake_flag == 6) {
            /***** 前往B区木桩 *****/
            //KP                               = 38;
            Upper[index].gantry_t.position.y = 2986.0;

            if ((fabs(Upper[index].gantry_t.position.x - distance_aver[index]) < 3) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)) {
                stateflag[index] = 5;
            }

            if ((stateflag[0] == 5) && (stateflag[1] == 5)) {
                stake_flag = 7;
            }
        }

        else if (stake_flag == 7) {
            /***** 放置B区砝码 *****/
            osDelay(1000);

            HAL_GPIO_WritePin(electromagnet_01_GPIO_Port, electromagnet_01_Pin, GPIO_PIN_RESET); // 砝码下电
            HAL_GPIO_WritePin(electromagnet_02_GPIO_Port, electromagnet_02_Pin, GPIO_PIN_RESET); // 砝码下电

            stake_flag = 8;
        }

        else if (stake_flag == 8) {
            osDelay(800);

            KP = 30;
            Upper[0].gantry_t.position.x = 760.0;
            Upper[1].gantry_t.position.x = 760.0;

            if ((fabs(Upper[0].gantry_t.position.x - distance_aver[0]) < 3) && (fabs(Upper[1].gantry_t.position.x - distance_aver[1]) < 3)) {
                 stake_flag = 9;
            }
        }

        // else if (stake_flag == 8) {
        //     /***** 5区木桩位置 *****/
        //     Upper[0].gantry_t.position.y = 2422.0;
        //     Upper[1].gantry_t.position.y = 2422.0;
            
        //     if((fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)){
        //         stake_flag = 9;
        //     }
        // }

        // else if(stake_flag==9){
        //     osDelay(400);
        //     // 定爪砝码下电
        //     HAL_GPIO_WritePin(electromagnet_05_GPIO_Port, electromagnet_05_Pin, RESET);
        //     osDelay(300);
        //     stake_flag = 10;
        // }

        else if (stake_flag == 9) {
            /***** 前往A区木桩 *****/
            KP = 30;
            Upper[0].gantry_t.position.y = 345;
            Upper[1].gantry_t.position.y = 345;

            /***** 木桩位置 *****/
            Upper[2].gantry_t.position.x = 711.0;
            Upper[3].gantry_t.position.x = 702.0;
            /*****************/

            if ((fabs(Upper[2].gantry_t.position.x - distance_aver[2]) < 3) && (fabs(Upper[3].gantry_t.position.x - distance_aver[3]) < 3) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)) {
                stake_flag = 10;
            }
        }

        else if (stake_flag == 10) {
            /***** 放下砝码 *****/
            osDelay(1000);

            HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET); // 砝码下电
            HAL_GPIO_WritePin(electromagnet_04_GPIO_Port, electromagnet_04_Pin, GPIO_PIN_RESET); // 砝码下电
            
            /***** 此刻比赛完成，停止计时  后续动作只是为了避免靠爪子扶住砝码的嫌疑 *****/
            stake_flag = 11;
        }

        else if (stake_flag == 11) {
            osDelay(1000);

            Upper[2].gantry_t.position.x = 740.0;
            Upper[3].gantry_t.position.x = 740.0;

            // if ((fabs(Upper[2].gantry_t.position.x - distance_aver[2]) < 3) && (fabs(Upper[3].gantry_t.position.x - distance_aver[3]) < 3) && (fabs(Upper[0].gantry_t.position.y - distance_aver[4]) < 3) && (fabs(Upper[1].gantry_t.position.y - distance_aver[4]) < 3)) {
            //     stake_flag = 13;
            // }
        }

        // else if(stake_flag==13){

        //     osDelay(2000);
        //     KP = 20;

        //     Upper[0].gantry_t.position.y = 1227.0;
        //     Upper[1].gantry_t.position.y = 1227.0;
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


