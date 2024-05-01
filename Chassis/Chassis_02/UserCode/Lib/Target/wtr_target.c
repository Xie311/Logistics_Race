/**
 * @file wtr_target.c
 * @author X311
 * @brief 接受上位机数据
 * @version 0.0
 * @date 2024-04-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "wtr_target.h"
#include <stdio.h>
mavlink_chassis_t Tar_Data;

void Chassis_Target_Init()
{
    // 绑定通道和串口
    wtrMavlink_BindChannel(&huart6, MAVLINK_COMM_0);
    // 启动通道0对应串口的中断接收
    wtrMavlink_StartReceiveIT(MAVLINK_COMM_0);
}

/**
 * @brief 接收到完整消息且校验通过后会调用这个函数。在这个函数里调用解码函数就可以向结构体写入收到的消息
 * @param msg 接收到的消息
 * @return
 */
void wtrMavlink_MsgRxCpltCallback(mavlink_message_t *msg)
{
    switch (msg->msgid) {
        case MAVLINK_MSG_ID_CHASSIS:
            // 将接收到的消息解码
            mavlink_msg_chassis_decode(msg, &Tar_Data);
            break;
        default:
            break;
    }
}
