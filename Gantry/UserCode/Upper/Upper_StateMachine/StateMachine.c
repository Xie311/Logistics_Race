#include "StateMachine.h"

// UpperState TargetState[2];
// 两个分区的龙门的目标状态,来自上位机的视觉确认和固定的路径规划--->一个巨大的状态机
// 其中的Velocity用来放speedpid的REF，非定量，可以给系数
/*备注：当前状态在全局变量Upper[2]中*/
uint16_t MatchingState;
/****************线程相关函数********************/

void Upper_State_Task(void *argument)
{
    osDelay(100);

    for (;;) {
        /************************砝码在内圈************************/
        if (weight_placement[3] != 0)
        {
            /***前往砝码***/
            /*全速段*/
            do{
                Upper[0].gantry_t.position.x = 100;
                Upper[0].gantry_t.position.y = 0;
            } while ((distance_aver[0] - Upper[0].gantry_t.position.x) < 30); // 距离砝码3cm时电磁铁上电

            /*减速段*/
            // 待加入

            /***抓取砝码***/
            // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET);  //砝码上电
            do {
                Upper[0].gantry_t.position.x = 100;
                Upper[0].gantry_t.position.y = 0;
            } while ((distance_aver[0] - Upper[0].gantry_t.position.x) < 1);

            osDelay(500);
            HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET);  // 爪子抬升

            /***前往木桩***/
            do
            {
                Upper[0].gantry_t.position.x = 100;  // 到达蝴蝶结末端
                Upper[0].gantry_t.position.y = 627.5;
            } while (((distance_aver[0] - Upper[0].gantry_t.position.x) < 1) || (fabs(distance_aver[1] - Upper[0].gantry_t.position.y)) < 1);

            /***放置砝码***/
            HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET);  //爪子放下
            osDelay(1000);
            HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET);  // 电磁铁下电
        }
        /************************砝码在外圈************************/
        else{
            /***前往砝码***/
            /*全速段*/
            do {
                Upper[0].gantry_t.position.x = 100;
                Upper[0].gantry_t.position.y = 0;
            } while ((distance_aver[0] - Upper[0].gantry_t.position.x) < 30); // 距离砝码3cm时电磁铁上电

            /*减速段*/
            // 待加入

            /***抓取砝码***/
            // HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_SET); // 砝码上电
            do {
                Upper[0].gantry_t.position.x = 100;
                Upper[0].gantry_t.position.y = 0;
            } while ((distance_aver[0] - Upper[0].gantry_t.position.x) < 1);

            osDelay(500);
            HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 爪子抬升

            /***前往木桩***/
            do {
                Upper[0].gantry_t.position.x = 100; // 到达蝴蝶结末端
                Upper[0].gantry_t.position.y = 627.5;
            } while (((distance_aver[0] - Upper[0].gantry_t.position.x) < 1) || (fabs(distance_aver[1] - Upper[0].gantry_t.position.y)) < 1);

            /***放置砝码***/
            HAL_GPIO_WritePin(cylinder_03_GPIO_Port, cylinder_03_Pin, GPIO_PIN_SET); // 爪子放下
            osDelay(1000);
            HAL_GPIO_WritePin(electromagnet_03_GPIO_Port, electromagnet_03_Pin, GPIO_PIN_RESET); // 电磁铁下电
        }
        osDelay(10);
    }
}

void Upper_StateMachine_Start(void)
{
    osThreadId_t Upper_StateHandle;
    const osThreadAttr_t Upper_State_attributes = {
        .name       = "Upper_State",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityAboveNormal,
    };

    Upper_StateHandle = osThreadNew(Upper_State_Task, NULL, &Upper_State_attributes);
}

/*******封装函数***********/
void Upper_StateMachine_Init()
{
    Upper[0].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
    Upper[1].gantry_t.xMutex_control = xSemaphoreCreateRecursiveMutex();
}