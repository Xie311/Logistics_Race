#include "UpperStart.h"


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        STP_23L_Decode(0);
        HAL_UART_Receive_IT(&huart1, Rxbuffer[0], sizeof(Rxbuffer[0]));
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
}