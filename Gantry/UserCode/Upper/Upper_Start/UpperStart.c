#include "UpperStart.h"

void StartDefaultTask(void *argument)
{
    /*初始化函数*/
    Upper_StateMachine_Init();
    Upper_Motor_init();
    /*开启线程*/
    Upper_StateMachine_Start();
    Upper_Servo_Start();
    /*串口使能*/
    Gantry_usart_init();

        // HAL_UART_Receive_IT(&huart7, Rxbuffer, sizeof(Rxbuffer));

        for (;;)
    {

        osDelay(10);
    }
}