/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-16 01:37:44
 * @FilePath: \Gantry\UserCode\Upper\Callback\Gantry_callback.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by ChenYiTong, All Rights Reserved. 
 */
#include "UpperStart.h"
#include "target.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        STP_23L_Decode(0);
        HAL_UART_Receive_IT(&huart1, Rxbuffer[0], sizeof(Rxbuffer[0]));
        //printf("2\n");
    }
    if (huart->Instance == USART2) {
        STP_23L_Decode(1);
        HAL_UART_Receive_IT(&huart2, Rxbuffer[1], sizeof(Rxbuffer[1]));
    }
    if (huart->Instance == USART3) {
        STP_23L_Decode(2);
        HAL_UART_Receive_IT(&huart3, Rxbuffer[2], sizeof(Rxbuffer[2]));
    }
    if (huart->Instance == USART6) {
        STP_23L_Decode(3);
        HAL_UART_Receive_IT(&huart6, Rxbuffer[3], sizeof(Rxbuffer[3]));
    }
    if(huart->Instance == UART5){
        // 接收上位机数据并解码
        Upper_Target_Decode();
        HAL_UART_Receive_IT(&huart5, (uint8_t *)receive_buffer, sizeof(receive_buffer));
    }
}