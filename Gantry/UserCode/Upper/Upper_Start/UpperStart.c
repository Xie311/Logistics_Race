#include "UpperStart.h"

/**
 * @brief 默认任务，启动其他线程
 *
 * @param argument
 */
void StartDefaultTask(void *argument)
{
    /*初始化函数*/
    Upper_StateMachine_Init_01();
   //Upper_StateMachine_Init_02();
    Upper_Motor_init();
   
    /*串口使能*/
    Gantry_usart_init();

    /*开启线程*/
    osDelay(1000);
    Upper_Decode_TaskStart();
    Upper_StateMachine_TaskStart_01(3);
    // Upper_StateMachine_TaskStart_02(4);
    Upper_Servo_TaskStart();
    Upper_Debug_TaskStart();

    for (;;) {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        osDelay(800);
    }
}