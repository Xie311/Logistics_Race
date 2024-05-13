#ifndef __UPPERSERVO_H__
#define __UPPERSERVO_H__

#include "UpperStart.h"
void Upper_Servo_Start(void);

void Core_xy_Motor_init();



void VelocityPlanning(float initialAngle, float maxAngularVelocity, float AngularAcceleration, float targetAngle, float currentTime, volatile float *currentAngle);

#endif // __UPPERSERVO_H__