/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-07-08 23:11:17
 * @FilePath: \Gantry_board_02\UserCode\Upper\Callback\Gantry_callback.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
#include "UpperStart.h"
#include "target.h"

int flag[4] = {0}; // 激光1\2\3;接收上位机数据
//int flag1   = 0; 
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        flag[0] = 1;
        HAL_UART_Receive_IT(&huart1, Rxbuffer[0], sizeof(Rxbuffer[0]));
        //printf("1\n");
    }

    if (huart->Instance == USART2) {
        //printf("2\n");
        flag[1] = 1;
        HAL_UART_Receive_IT(&huart2, Rxbuffer[1], sizeof(Rxbuffer[1]));
    }

    if (huart->Instance == USART3) {
        flag[2] = 1;
        HAL_UART_Receive_IT(&huart3, Rxbuffer[2], sizeof(Rxbuffer[2]));
    }

    if (huart->Instance == UART5) {
        // 接收上位机数据并解码
        flag[3] = 1;
        HAL_UART_Receive_IT(&huart5, (uint8_t *)receive_buffer, sizeof(receive_buffer));
        //osDelay(100);
    }

}
    






