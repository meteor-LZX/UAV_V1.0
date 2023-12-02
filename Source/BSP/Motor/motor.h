/********************************************************************************
* @author: Meteor
* @date: 2023/10/22
* @version: 1.0
* @description: This is header file of motors.
********************************************************************************/
#ifndef UAV_V1_0_MOTOR_H
#define UAV_V1_0_MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "main.h"


    // Types definition
    // Motor State
    typedef enum {
        ACTIVE = 0,
        IN_ACTIVE
    }MotorState_e;
    // Motor Struct
    typedef struct {
        MotorState_e _is_active;     // Mark state of motor
        uint8_t _speed;         // Speed range is 0~100
        uint32_t _tim_channel;  // TIM Channels
    }Motor_t;


    // External variables
    extern Motor_t *motor_up_right;
    extern Motor_t *motor_down_right;
    extern Motor_t *motor_down_left;
    extern Motor_t *motor_up_left;


    // External functions
    HAL_StatusTypeDef Motor_Init();
    HAL_StatusTypeDef Motor_SetState(Motor_t* motor, MotorState_e state);
    HAL_StatusTypeDef Motor_SetSpeed(Motor_t* motor, uint8_t speed);

#ifdef __cplusplus
}
#endif

#endif //UAV_V1_0_MOTOR_H
