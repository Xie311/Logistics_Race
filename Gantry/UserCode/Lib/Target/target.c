/**
 * @file wtr_target.c
 * @author X311
 * @brief 接受上位机数据
 * @version 0.0
 * @date 2024-04-28
 * @eitor
 * @copyright Copyright (c) 2024
 *
 */
#include "target.h"

uint8_t receive_buffer[24];
Tar_t Tar_Data = {0};
float weight_placement[5];
/**
 * @brief 上位机数据接收
 *
 */
void Upper_Target_Init()
{
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart6, receive_buffer, sizeof(receive_buffer));
}

void Upper_Target_Decode()
{
    static union {
        uint8_t data[20];
        float weight_state[5];
    } state;

    if ((receive_buffer[0] == 0xFF) && (receive_buffer[1] == 0xFE) && (receive_buffer[22] == 0xFE) && (receive_buffer[23] == 0xFF)) {
        for (int i = 0; i < 20; i++) {
            state.data[i] = receive_buffer[i + 2];
        }

        for (int t = 0; t < 5; t++) {
            weight_placement[t] = state.weight_state[t];
        }
    }
}