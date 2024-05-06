#include "chassis_debug.h"

/************************一次只能启动一个调试线程！！！否则printf左吐一点右吐一点****************************************/

/************************ 码盘传数据 调试代码 ***************************/

/**
 * @brief  码盘调试线程开启
 */
void OPS_Debug_TaskStart(void)
{
    const osThreadAttr_t ops_debug_Task_attributes = {
        .name       = "ops_debug_Task",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityNormal1,
    };
    osThreadNew(OPS_Debug_Task, NULL, &ops_debug_Task_attributes);
}

/**
 * @brief   码盘调试线程
 */
void OPS_Debug_Task(void *argument)
{
    for (;;) {
        //printf("%d,%d,%f,%f\r\n", (int)(OPS_Data.pos_x), (int)(OPS_Data.pos_y), ChassisControl.velocity.x,ChassisControl.velocity.y);
        osDelay(50);
    }
}

/************************ 上位机传数据 调试代码 ***************************/



// /**
//  * @brief   数据接收线程开启
//  */
// void Tar_Debug_TaskStart(void)
// {

    // osThreadId_t Tar_debug_TaskHandle;
// const osThreadAttr_t Tar_debug_Task_attributes = {
//     .name       = "Tar_debug_Task",
//     .stack_size = 128 * 4,
//     .priority   = (osPriority_t)osPriorityNormal,
// };

//     Tar_debug_TaskHandle = osThreadNew(Tar_Debug_Task, NULL, &Tar_debug_Task_attributes);
// }

// /**
//  * @brief   数据接收线程
//  */
// void Tar_Debug_Task(void *argument)
// {
//     for (;;) {
//         printf("%d\r\n", (int)(Tar_Data.pos_x));
//         osDelay(1000);
//     }
// }