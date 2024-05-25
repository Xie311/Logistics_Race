/*
 * @Author: X311
 * @Date: 2024-05-14 00:50:30
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-26 01:26:41
 * @FilePath: \Gantry\UserCode\Lib\Target\target.c
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
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
float final_weight_placement[5] = {2};
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
    int switch_flag = 0;  // 判断每次接收到的数组与基准数组是否相等
    int tar_count = 0;    // 计数连续相同数组的次数
    //osDelay(100);
    if(flag[3]==1){
        Upper_Target_Decode();
        for (int i = 0; i < 5;i++){
            weight_placement_tmp[i] = weight_placement[i];
        }
        flag[3] = 0;
        count   = 1; // 首次接收时，计数器初始化为1

        for (;;) {
            if (flag[3] == 1) {
                Upper_Target_Decode();
                flag[3] = 0;
                switch_flag = 0;

                for (int i = 0; i < 5;i++){
                    if(weight_placement_tmp[i] == weight_placement[i]){
                        switch_flag = 1;
                        break;
                    }
                }
                
                //收到的数组与基准数组相等
                if (switch_flag == 0){
                    count++;
                }
                //收到的数组与基准数组不相等
                else{
                    count = 1;  //重新计数
                    for (int i = 0; i < 5; i++) {
                        weight_placement_tmp[i] = weight_placement[i];
                    }
                }

                //如果连续十次接收到同样的数组，则把这个数组设置为最终值
                if(count>=10){
                    for (int i = 0; i < 5; i++) {
                        final_weight_placement[i] = weight_placement[i];
                    }
                }
            }
            osDelay(3);
        }
    }


}