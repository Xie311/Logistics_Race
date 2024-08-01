/*
 * @Author: X311
 * @Date: 2024-05-16 22:06:32
 * @LastEditors: X311 
 * @LastEditTime: 2024-07-31 08:03:10
 * @FilePath: \Gantry_final\UserCode\Upper\Upper_Debug\upper_debug.c
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
        // printf("%f,%f,%f,%f,%f,%d\n",
        //        distance_aver[0], distance_aver[1], distance_aver[2], distance_aver[3], distance_aver[4], (int)stake_flag
        //        //    Upper[0].Motor_X->speedPID.output, Upper[1].Motor_X->speedPID.output,
        //        //    Upper[0].Motor_Y->speedPID.output, Upper[1].Motor_Y->speedPID.output,
        //        //   (int)stateflag[0], (int)stateflag[1], (int)stake_flag,
        //        //(int)weight_placement[0], (int)weight_placement[1], (int)weight_placement[2], (int)weight_placement[3], (int)weight_placement[4]);
        // );

        // printf("%d,%d,%d,%d,%d,%d\n",
        //        receive_buffer[0],receive_buffer[22],receive_buffer[23],
        //        (int)weight_placement[0], (int)weight_placement[1], (int)weight_placement[2]);
        // osDelay(100);
    }
}

/**
 * @brief OLED线程开启
 *
 */
void Upper_OLED_TaskStart(void)
{
    const osThreadAttr_t upper_oled_Task_attributes = {
        .name       = "upper_oled_Task",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(Upper_OLED_Task, NULL, &upper_oled_Task_attributes);
}

/**
 * @brief   OLED线程
 */
void Upper_OLED_Task(void *argument)
{
    OLED_Init(); //屏幕初始化
    OLED_Clear(); //先清屏
    osDelay(100);
    for (;;) {
        OLED_ShowNum(5, 1, distance_aver[3], 10, 14);
        // OLED_ShowNum(25, 1, distance_aver[1], 2, 16);
        // OLED_ShowNum(45, 1, distance_aver[2], 2, 16);
        // OLED_ShowNum(65, 1, distance_aver[3], 2, 16);
        // OLED_ShowNum(85, 1, distance_aver[4], 2, 16);

       // OLED_ShowNum(5, 4, (int)Uart_State , 5, 16);
        //OLED_ShowNum(80,4, (int)receive_buffer[23], 5, 16);

        osDelay(20);
    }
}

/**
 * @brief 复位线程开启
 *
 */
void Upper_Reset_TaskStart(void)
{
    const osThreadAttr_t upper_reset_Task_attributes = {
        .name       = "upper_reset_Task",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(Upper_Reset_Task, NULL, &upper_reset_Task_attributes);
}

/**
 * @brief   复位线程
 */
void Upper_Reset_Task(void *argument)
{
    osDelay(100);
    for (;;) {
        //uint16_t reset_flag = HAL_GPIO_ReadPin(Reset_GPIO_Port,Reset_Pin);
        uint16_t reset_flag_01 = HAL_GPIO_ReadPin(Reset_01_GPIO_Port,Reset_01_Pin);
        uint16_t reset_flag_02 = HAL_GPIO_ReadPin(Reset_02_GPIO_Port,Reset_01_Pin);
        uint16_t reset_flag_03 = HAL_GPIO_ReadPin(Reset_03_GPIO_Port,Reset_01_Pin);

        if ((reset_flag_01 == 1) && (reset_flag_02 == 1) && (reset_flag_03 == 1) ) {
            reset_flag_01   = 0;
            reset_flag_02   = 0;
            reset_flag_03   = 0;

            stateflag[0] = 100;
            stateflag[1] = 100;
            stateflag[2] = 100;
            stateflag[3] = 100;
            stateflag[4] = 100;

            stake_flag = 100;

            HAL_GPIO_WritePin(electromagnet_01_GPIO_Port, electromagnet_01_Pin, GPIO_PIN_RESET); // 砝码下电
            HAL_GPIO_WritePin(electromagnet_02_GPIO_Port, electromagnet_02_Pin, GPIO_PIN_RESET); // 砝码下电
            HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET); // 砝码下电
            HAL_GPIO_WritePin(electromagnet_04_GPIO_Port, electromagnet_04_Pin, GPIO_PIN_RESET); // 砝码下电
            HAL_GPIO_WritePin(electromagnet_05_GPIO_Port, electromagnet_05_Pin, GPIO_PIN_RESET); // 砝码下电

            osDelay(200);

            HAL_GPIO_WritePin(cylinder_01_GPIO_Port, cylinder_01_Pin, GPIO_PIN_SET); // 气缸向上
            HAL_GPIO_WritePin(cylinder_02_GPIO_Port, cylinder_02_Pin, GPIO_PIN_SET); // 气缸向上
            HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 气缸向上
            HAL_GPIO_WritePin(cylinder_04_GPIO_Port, cylinder_04_Pin, GPIO_PIN_SET); // 气缸向上
            HAL_GPIO_WritePin(cylinder_05_GPIO_Port, cylinder_05_Pin, GPIO_PIN_SET); // 气缸向上

            osDelay(300);

            KP  = 30;

            Upper[0].gantry_t.position.y = 1227.0;
            Upper[1].gantry_t.position.y = 1227.0;

            Upper[0].gantry_t.position.x = 700.0;
            Upper[1].gantry_t.position.x = 700.0;
            Upper[2].gantry_t.position.x = 700.0;
            Upper[3].gantry_t.position.x = 700.0;
        }
        osDelay(10);
    }
}