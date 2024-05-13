/*
 * @Author: szf
 * @Date: 2023-02-22 12:04:21
 * @LastEditTime: 2024-04-28 16:01:49
 * @LastEditors: labbbbbbbbb 
 * @brief 运动学逆解算及PID计算函数
 * @FilePath: \Upper_trial01\UserCode\Lib\Calculate\wtr_calculate.c
 */

#include "wtr_calculate.h"
#include <math.h>
//#include "Chassis_UserConfig.h"

double moter_speed[4];

// 麦轮底盘参数
#define rotate_ratio    0.3615   // (Width + Length)/2
#define wheel_rpm_ratio 2387.324 // 换算线速度到rpm


/**
 * @brief麦克纳姆轮底盘逆解算
 * @author: szf
 * @date:
 * @return {void}
 */
void CalculateFourMecanumWheels(double *moter_speed, double vx, double vy, double vw)
{
    moter_speed[0] = (vx - vy - vw * rotate_ratio) * wheel_rpm_ratio;
    moter_speed[1] = (vx + vy - vw * rotate_ratio) * wheel_rpm_ratio;
    moter_speed[2] = (-vx + vy - vw * rotate_ratio) * wheel_rpm_ratio;
    moter_speed[3] = (-vx - vy - vw * rotate_ratio) * wheel_rpm_ratio;
}


/**
 * @brief: PID控制-增量式PID
 * @auther: Chen YiTong 3083697520
 * @param {__IO PID_t} *pid
 * @return {*}
 */

void PID_Calc(__IO PID_t *pid)
{
    pid->cur_error = pid->ref - pid->fdb;
    pid->output += pid->KP * (pid->cur_error - pid->error[1]) + pid->KI * pid->cur_error + pid->KD * (pid->cur_error - 2 * pid->error[1] + pid->error[0]);
    pid->error[0] = pid->error[1];
    pid->error[1] = pid->ref - pid->fdb;
    /*设定输出上限*/
    if (pid->output > pid->outputMax) pid->output = pid->outputMax;
    if (pid->output < -pid->outputMax) pid->output = -pid->outputMax;
}


/**
 * @brief: PID控制-位置式PID
 * @auther: zyt
 * @param {__IO PID_t} *pid
 * @return {*}
 */

void PID_Calc_P(__IO PID_t *pid)
{
    pid->cur_error = pid->ref - pid->fdb;
    pid->integral += pid->cur_error;
    pid->output = pid->KP * pid->cur_error + pid->KI * pid->integral + pid->KD * (pid->cur_error - pid->error[1]);
    pid->error[0] = pid->error[1];  //这句已经没有用了
    pid->error[1] = pid->ref - pid->fdb;
    /*设定输出上限*/
    if (pid->output > pid->outputMax) pid->output = pid->outputMax;
    if (pid->output < -pid->outputMax) pid->output = -pid->outputMax;
}


/**
 * @brief: PID算法-P控制
 * @auther: Chen YiTong 3083697520
 * @param {__IO PID_t} *pid
 * @return {*}
 */
void P_Calc(__IO PID_t *pid)
{
    pid->cur_error = pid->ref - pid->fdb;
    pid->output    = pid->KP * pid->cur_error;
    /*设定输出上限*/
    if (pid->output > pid->outputMax) pid->output = pid->outputMax;
    if (pid->output < -pid->outputMax) pid->output = -pid->outputMax;

    if (fabs(pid->output) < pid->outputMin)
        pid->output = 0;
}

/**
 * @brief: 位置伺服
 * @auther: Chen YiTong 3083697520
 * @param {float} ref 目标位置
 * @param {DJI_t} *motor 电机结构体
 * @return {*}
 */
void positionServo(float ref, DJI_t *motor)
{

    motor->posPID.ref = ref;
    motor->posPID.fdb = motor->AxisData.AxisAngle_inDegree;
    P_Calc(&motor->posPID);
    
    motor->speedPID.ref = motor->posPID.output;
    motor->speedPID.fdb = motor->FdbData.rpm;
    PID_Calc_P(&(motor->speedPID));
}

/**
 * @brief: 速度伺服函数
 * @auther: Chen YiTong 3083697520
 * @param {float} ref 目标值
 * @param {DJI_t} *motor 电机结构体
 * @return {*}
 */
void speedServo(float ref, DJI_t *motor)
{
    motor->speedPID.ref = ref;
    motor->speedPID.fdb = motor->FdbData.rpm;
    PID_Calc(&(motor->speedPID));
}

/**
 * @brief: 圆周死区控制
 * @auther: szf
 * @param {double} x
 * @param {double} y
 * @param {double} *new_x
 * @param {double} *new_y
 * @param {double} threshould
 * @return {*}
 */
void DeadBand(double x, double y, double *new_x, double *new_y, double threshould)
{
    double length     = sqrt(x * x + y * y);
    double new_length = length - threshould;

    if (new_length <= 0) {
        *new_x = 0;
        *new_y = 0;
        return;
    }

    double k = new_length / length;

    *new_x = x * k;
    *new_y = y * k;
}

/**
 * @brief: 单轴死区控制
 * @auther: szf
 * @param {double} x
 * @param {double} *new_x
 * @param {double} threshould
 * @return {*}
 */
// void DeadBandOneDimensional(double x, double *new_x, double threshould)
// {

//     double difference_x = fabs(x) - threshould;

//     if (difference_x < 0) {
//         *new_x = 0;
//         return;
//     }

//     double k = difference_x / fabs(x);
//     *new_x   = k * x;
// }

void DeadBandOneDimensional(double x, double *new_x, double threshould)
{

    double difference_x = fabs(x) - threshould;

    if (difference_x < 0) {
        *new_x = 0;
        return;
    }

    return;
}
