#include "wtr_ops.h"
#include <stdio.h>

OPS_t OPS_Data = {0};   // OPS_Data 结构体，用于存储解码后的数据，初始值为 0
uint8_t ops_buffer[28]; // 接收缓冲区，用于存储接收到的数据，大小为 28 字节

/**
 * @brief 码盘解码函数
 *
 * @return uint8_t
 */
uint8_t OPS_Decode(void)
{
    static union {
        uint8_t data[24]; // 用于存储解码后的姿态数据的数据缓冲区
        float ActVal[6]; // 用于存储解码后的值的数组
    } posture;

    // 检查起始和结束字符以确保数据完整性
    if ((ops_buffer[0] == 0x0D) && (ops_buffer[1] == 0x0A) && (ops_buffer[26] == 0x0A) && (ops_buffer[27] == 0x0D)) {
        // 从缓冲区提取数据并分配到姿态数据缓冲区
        for (int i = 0; i < 24; i++) {
            posture.data[i] = ops_buffer[i + 2];
        }

        // 将解码后的值分配给 OPS_Data 结构体的字段
        OPS_Data.z_angle = posture.ActVal[0];
        OPS_Data.x_angle = posture.ActVal[1];
        OPS_Data.y_angle = posture.ActVal[2];
        OPS_Data.pos_x   = posture.ActVal[3];
        OPS_Data.pos_y   = posture.ActVal[4];
        OPS_Data.w_z     = posture.ActVal[5];
    }

    return 0; // 返回成功代码
}

// /**
//  * @brief: 码盘线程启动
//  * @return {*}
//  */
// void OPS_Decode_TaskStart()
// {
//     osThreadAttr_t OPS_Decode_attr =
//         {
//             .name       = "OPS_Decode",
//             .priority   = osPriorityAboveNormal,
//             .stack_size = 128 * 10,
//         };
//     osThreadNew(OPS_Decode, NULL, &OPS_Decode_attr);
// }
