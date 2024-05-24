/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @FilePath: \Gantry\UserCode\Upper\Upper_Servo\UpperServo.c
 * @Brief:
 *
 * Copyright (c) 2024 by X311, All Rights Reserved.
 */
#include "UpperServo.h"

Upper_COMPONENT Upper[2]; // 龙门蝴蝶结两侧的数据
float KP = 40;
/********线程相关部分*************/
/**
 * @brief 伺服函数
 * @note  1：斜右 负向末端前进
          3：斜左 正向末端前进
          2：直右 正向前
          4：直左 负向前
*/
void Upper_Servo_Task(void *argument)
{
    osDelay(100);
    for (;;) {
        /***** 调试代码 *****/
        // speedServo(2000, Upper[0].Motor_X);
        // speedServo(-2000, Upper[1].Motor_X);
        // speedServo(2000, Upper[0].Motor_Y);
        // speedServo(-2000, Upper[1].Motor_Y);

        /*************************************************************/
        Upper[0].gantry_t.velocity.x = -KP * (Upper[0].gantry_t.position.x - distance_aver[0]);
        Upper[1].gantry_t.velocity.x =  KP * (Upper[1].gantry_t.position.x - distance_aver[1]);
        Upper[0].gantry_t.velocity.y =  KP * (Upper[0].gantry_t.position.y - distance_aver[2]);
        Upper[1].gantry_t.velocity.y = -KP * (Upper[1].gantry_t.position.y - distance_aver[2]);

        speedServo(Upper[0].gantry_t.velocity.x, Upper[0].Motor_X);
        speedServo(Upper[0].gantry_t.velocity.y, Upper[0].Motor_Y);
        speedServo(Upper[1].gantry_t.velocity.x, Upper[1].Motor_X);
        speedServo(Upper[1].gantry_t.velocity.y, Upper[1].Motor_Y);

        //CanTransmit_DJI_1234(&hcan1, 0, 0, 0, 0);

        CanTransmit_DJI_1234(&hcan1,
                             Upper[0].Motor_X->speedPID.output,   // 负向末端前进
                             Upper[0].Motor_Y->speedPID.output,   // 正向前
                             Upper[1].Motor_X->speedPID.output,   // 正向末端前进
                             Upper[1].Motor_Y->speedPID.output    // 负向前
        );
        osDelay(6);
    }
}

void Upper_Servo_TaskStart(void)
{
    const osThreadAttr_t Upper_Servo_attributes = {
        .name       = "Upper_Servo",
        .stack_size = 128 * 10,
        .priority   = (osPriority_t)osPriorityNormal,
    };
     osThreadNew(Upper_Servo_Task, NULL, &Upper_Servo_attributes);
}

/*******封装函数部分********/
void Upper_Motor_init() // 电机初始化
{

    Upper[0].Motor_X = &hDJI[0];
    Upper[0].Motor_Y = &hDJI[1];
    Upper[1].Motor_X = &hDJI[2];
    Upper[1].Motor_Y = &hDJI[3];
    hDJI[0].motorType  = M2006;
    hDJI[1].motorType  = M2006;
    hDJI[2].motorType  = M2006;
    hDJI[3].motorType  = M2006;
    DJI_Init();
    for (int i = 0; i < 2;i++)
    {
        Upper[i].Motor_Y->speedPID.KP = 1.0;
        Upper[i].Motor_Y->speedPID.KI = 0.4;
        Upper[i].Motor_Y->speedPID.KD = 1.0; // 1.2;
    }

    // speed_PID
    Upper[0].Motor_X->speedPID.KP = 8;
    Upper[0].Motor_X->speedPID.KI = 0.4;
    Upper[0].Motor_X->speedPID.KD = 0.8;

    Upper[1].Motor_X->speedPID.KP = 6;
    Upper[1].Motor_X->speedPID.KI = 0.4;
    Upper[1].Motor_X->speedPID.KD = 10.0;

    // Upper[1].Motor_X->speedPID.KP = 1.0;
    // Upper[1].Motor_X->speedPID.KI = 0.05;
    // Upper[1].Motor_X->speedPID.KD = 8.0;

    // // pos_PID
    // Upper[0].Motor_X->posPID.KP = -800;
    // Upper[0].Motor_X->posPID.KI = 0;
    // Upper[0].Motor_X->posPID.KD = 0;

    // Upper[0].Motor_Y->posPID.KP = -800;
    // Upper[0].Motor_Y->posPID.KI = 0;
    // Upper[0].Motor_Y->posPID.KD = 0;

    // Upper[1].Motor_X->posPID.KP = 800;
    // Upper[1].Motor_X->posPID.KI = 0;
    // Upper[1].Motor_X->posPID.KD = 0;

    // Upper[1].Motor_Y->posPID.KP = 800;
    // Upper[1].Motor_Y->posPID.KI = 0;
    // Upper[1].Motor_Y->posPID.KD = 0;

    CANFilterInit(&hcan1);
}

/**
 * @brief T型速度规划函数
 * @param initialAngle 初始角度
 * @param maxAngularVelocity 最大角速度
 * @param AngularAcceleration 角加速度
 * @param targetAngle 目标角度
 * @param currentTime 当前时间
 * @param currentAngle 当前角度
 * @todo 转换为国际单位制
 */
void VelocityPlanning(float initialAngle, float maxAngularVelocity, float AngularAcceleration, float targetAngle, float currentTime, volatile float *currentAngle)
{

    float angleDifference = targetAngle - initialAngle;     // 计算到目标位置的角度差
    float sign            = (angleDifference > 0) ? 1 : -1; // 判断角度差的正负(方向)

    float accelerationTime = maxAngularVelocity / AngularAcceleration;                                                      // 加速(减速)总时间
    float constTime        = (fabs(angleDifference) - AngularAcceleration * pow(accelerationTime, 2)) / maxAngularVelocity; // 匀速总时间
    float totalTime        = constTime + accelerationTime * 2;                                                              // 计算到达目标位置所需的总时间

    // 判断能否达到最大速度
    if (constTime > 0) {
        // 根据当前时间判断处于哪个阶段
        if (currentTime <= accelerationTime) {
            // 加速阶段
            *currentAngle = initialAngle + sign * 0.5 * AngularAcceleration * pow(currentTime, 2);
        } else if (currentTime <= accelerationTime + constTime) {
            // 匀速阶段
            *currentAngle = initialAngle + sign * maxAngularVelocity * (currentTime - accelerationTime) + 0.5 * sign * AngularAcceleration * pow(accelerationTime, 2);
        } else if (currentTime <= totalTime) {
            // 减速阶段
            float decelerationTime = currentTime - accelerationTime - constTime;
            *currentAngle          = initialAngle + sign * maxAngularVelocity * constTime + 0.5 * sign * AngularAcceleration * pow(accelerationTime, 2) + sign * (maxAngularVelocity * decelerationTime - 0.5 * AngularAcceleration * pow(decelerationTime, 2));
        } else {
            // 达到目标位置
            *currentAngle = targetAngle;
        }
    } else {
        maxAngularVelocity = sqrt(fabs(angleDifference) * AngularAcceleration);
        accelerationTime   = maxAngularVelocity / AngularAcceleration;
        totalTime          = 2 * accelerationTime;
        constTime          = 0;
        // 根据当前时间判断处于哪个阶段
        if (currentTime <= accelerationTime) {
            // 加速阶段
            *currentAngle = initialAngle + sign * 0.5 * AngularAcceleration * pow(currentTime, 2);
        } else if (currentTime <= totalTime) {
            // 减速阶段
            float decelerationTime = currentTime - accelerationTime; // 减速时间
            *currentAngle          = initialAngle + sign * 0.5 * AngularAcceleration * pow(accelerationTime, 2) + sign * (maxAngularVelocity * decelerationTime - 0.5 * AngularAcceleration * pow(decelerationTime, 2));
        } else {
            // 达到目标位置
            *currentAngle = targetAngle;
        }
    }
}