/********************************************************************************
* @author: Meteor
* @date: 2023/10/22
* @version: 1.0
* @description: This is source file of motors.It is using to drive motors.
********************************************************************************/
#include <stdlib.h>
#include "motor.h"
#include "tim.h"


Motor_t *motor_up_right = NULL;
Motor_t *motor_down_right = NULL;
Motor_t *motor_down_left = NULL;
Motor_t *motor_up_left = NULL;


/********************************************************************************
* @author: Meteor
* @date: 2023/10/22
* @brief: Initialize 4 motor variables.
* @params: None
* @return: HAL status
********************************************************************************/
HAL_StatusTypeDef Motor_Init() {
    // Start all PWM channels with 1KHz frequency
    if (HAL_OK != HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1)) {
        return HAL_ERROR;
    }
    if (HAL_OK != HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2)) {
        return HAL_ERROR;
    }
    if (HAL_OK != HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3)) {
        return HAL_ERROR;
    }
    if (HAL_OK != HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4)) {
        return HAL_ERROR;
    }

    // Set motor1 initial params
    motor_up_right = (Motor_t*) malloc(sizeof(Motor_t));
    if (motor_up_right == NULL) {
        return HAL_ERROR;
    } else {
        motor_up_right->_speed = 0;
        motor_up_right->_is_active = IN_ACTIVE;
        motor_up_right->_tim_channel = TIM_CHANNEL_1;
    }
    // Set motor2 initial params
    motor_down_right = (Motor_t*) malloc(sizeof(Motor_t));
    if (motor_down_right == NULL) {
        return HAL_ERROR;
    } else {
        motor_down_right->_speed = 0;
        motor_down_right->_is_active = IN_ACTIVE;
        motor_down_right->_tim_channel = TIM_CHANNEL_2;
    }
    // Set motor3 initial params
    motor_down_left = (Motor_t*) malloc(sizeof(Motor_t));
    if (motor_down_left == NULL) {
        return HAL_ERROR;
    } else {
        motor_down_left->_speed = 0;
        motor_down_left->_is_active = IN_ACTIVE;
        motor_down_left->_tim_channel = TIM_CHANNEL_3;
    }
    // Set motor4 initial params
    motor_up_left = (Motor_t*) malloc(sizeof(Motor_t));
    if (motor_up_left == NULL) {
        return HAL_ERROR;
    } else {
        motor_up_left->_speed = 0;
        motor_up_left->_is_active = IN_ACTIVE;
        motor_up_left->_tim_channel = TIM_CHANNEL_4;
    }

    // Set motors state to ACTIVE
    Motor_SetState(motor_up_right, ACTIVE);
    Motor_SetState(motor_down_right, ACTIVE);
    Motor_SetState(motor_down_left, ACTIVE);
    Motor_SetState(motor_up_left, ACTIVE);

    return HAL_OK;
}


/********************************************************************************
* @author: Meteor
* @date: 2023/10/22
* @brief: Set specific motor's state.
* @params: motor: Choose which motor you want to set state.
*          state: Set state of the chosen motor.
*                 ACTIVE: means motor is active
*                 IN_ACTIVE: means motor is inactive
* @return: HAL status
********************************************************************************/
HAL_StatusTypeDef Motor_SetState(Motor_t* motor, MotorState_e state) {
    if (NULL == motor) {
        return HAL_ERROR;
    }

    motor->_is_active = state;
    return HAL_OK;
}


/********************************************************************************
* @author: Meteor
* @date: 2023/10/22
* @brief: Set specific motor's speed.
* @params: motor: Choose which motor you want to set speed.
*          speed: Set speed value of the chosen motor which range 0~100.
* @return: HAL status
********************************************************************************/
HAL_StatusTypeDef Motor_SetSpeed(Motor_t* motor, uint8_t speed) {
    if (NULL == motor) {
        return HAL_ERROR;
    }
    if (IN_ACTIVE == motor->_is_active) {
        return HAL_ERROR;
    }

    motor->_speed = speed;  //Save speed value
    __HAL_TIM_SET_COMPARE(&htim4, motor->_tim_channel, motor->_speed);  //Set CCR reg

    return HAL_OK;
}


