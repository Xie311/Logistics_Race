/**
 * @file wtr_target.c
 * @author X311
 * @brief 接受上位机数据
 * @version 0.0
 * @date 2024-05-12
 * @eitor
 * @copyright Copyright (c) 2024
 *
 */
#include "target.h"

uint8_t receive_buffer[24];
Tar_t Tar_Data = {0};
float weight_placement[5] = {0};
int tar_count = 0;
/**
 * @brief 上位机数据接收
 *
 */
void Upper_Target_Init()
{
    __HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart5, receive_buffer, sizeof(receive_buffer));
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

/**
 * @brief 启动上位机数据解码线程
 *
 */
void Target_Decode_TaskStart(void)
{
    osThreadAttr_t target_decode_Task_attributes = {
        .name       = "target_decode_Task",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityNormal,
    };
    osThreadNew(Target_Decode_Task, NULL, &target_decode_Task_attributes);
}

/**
 * @brief   上位机数据解码线程
 */
void Target_Decode_Task(void)
{
    float weight_placement_tmp[5] = {0};
    int switch_flag = 0;
    //osDelay(100);
    if(flag[3]==1){
        Upper_Target_Decode();
        for (int i = 0; i < 5;i++){
            weight_placement_tmp[i] = weight_placement[i];
        }
        flag[3] = 0;

        for (;;) {
            if (flag[3] == 1) {
                Upper_Target_Decode();
                flag[3] = 0;

                for (int i = 0; i < 5;i++){
                    if(weight_placement_tmp[i] == weight_placement[i]){
                        switch_flag = 1;
                    }
                }
                
                //收到的数组与基准数组相等
                if (switch_flag == 0){
                    count++;
                }
                else{
                    
                }

                if(count)
            }
            osDelay(4);
        }
    }


}