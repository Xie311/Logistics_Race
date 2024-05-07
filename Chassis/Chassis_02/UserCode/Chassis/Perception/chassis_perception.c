#include "chassis_perception.h"

/**
 * @brief   码盘定位系统初始化
 */
void Chassis_Perception_Init(void)
{
    //HAL_UART_Receive_DMA(&OPS_UART_HANDLE, ops_buffer, sizeof(ops_buffer));
    // 串口中断使能
    __HAL_UART_ENABLE_IT(&huart8, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart8, ops_buffer, sizeof(ops_buffer));

    // 码盘PID初始化
    OPS_Data.opsPID_x.KP = -0.4; 
    OPS_Data.opsPID_x.KI = 0;
    OPS_Data.opsPID_x.KD = 0;
    OPS_Data.opsPID_x.outputMax = 8000;

    OPS_Data.opsPID_y.KP = 1;
    OPS_Data.opsPID_y.KI = 0;
    OPS_Data.opsPID_y.KD = 0;
    OPS_Data.opsPID_y.outputMax = 8000;
}
