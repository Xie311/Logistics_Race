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
    Upper_StateMachine_Init_02();
    Upper_Motor_init();

    /*开启线程*/
    Upper_StateMachine_TaskStart_01(3);
    Upper_StateMachine_TaskStart_02(4);
    Upper_Servo_TaskStart();

    /*串口使能*/
    Gantry_usart_init();

    for (;;)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        osDelay(10);
    }
}