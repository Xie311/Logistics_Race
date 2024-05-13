
#include "UpperStart.h"



void StartDefaultTask(void *argument)
{
/*初始化函数*/
    Core_XY_StateMachine_Init();
//    Core_xy_Motor_init();
/*开启线程*/
    Core_xy_StateMachine_Start();
    //Upper_Servo_Start();
/*串口使能*/
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart1, Rxbuffer, sizeof(Rxbuffer));
    __HAL_UART_ENABLE(&huart2);
    
    //HAL_UART_Receive_IT(&huart7, Rxbuffer, sizeof(Rxbuffer));

    for (;;) {

        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        
        //printf("%f,%f,%f\n", Core_xy.Motor_Y->AxisData.AxisAngle_inDegree,current_angle[1], Core_xy.Motor_Y->speedPID.output);
        //float distance_sum = 0;
        // for (uint16_t i = 0; i < 12; i++) 
        // {
        //     distance_sum += LidarData[i].distance;
        // }
        // float distance_aver = distance_sum / 12;
        //printf("%d\n", LidarData[0].distance);
        //printf("%d\n", LidarData[0].distance);
        osDelay(10);
    }
}







