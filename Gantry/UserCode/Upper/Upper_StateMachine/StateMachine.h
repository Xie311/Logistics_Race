/*
 * @Author: X311
 * @Date: 2024-05-13 09:00:14
 * @LastEditors: X311 
 * @LastEditTime: 2024-05-21 10:47:38
 * @FilePath: \Gantry\UserCode\Upper\Upper_StateMachine\StateMachine.h
 * @Brief: 
 * 
 * Copyright (c) 2024 by X311, All Rights Reserved. 
 */
#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "UpperStart.h"

void Upper_StateMachine_TaskStart_01(void);
void Upper_StateMachine_Init_01(void);
void Upper_StateMachine_TaskStart_02(void);
void Upper_StateMachine_Init_02(void);

extern Upper_COMPONENT Upper[2];
extern float weight_placement[5];
extern float initial_pos[3];
extern float current_pos[3];

#endif 