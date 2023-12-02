/********************************************************************************
* @author: Meteor
* @date: 2023/11/4
* @version: 1.0
* @description: This is source file of controlling UAV.
********************************************************************************/
#include "control.h"
#include "ppm.h"


// Drone's status
drone_status_t drone_sta = {
    ._status = LOCK
};

/********************************************************************************
* @author: Meteor
* @date: 2023/11/4
* @brief: Wait for unlocking signal to unlock the UAV.
* @params: None
* @return: None
********************************************************************************/
void UAV_UnLock() {
    // If channel 4 greater than 1950(i.e. Push the throttle rocker to the far right)
    while (ppm_data._ppm_data_buf[4] <= 1950) {
        LED1_TOGGLE();
        LED2_TOGGLE();
        HAL_Delay(100);      // Blink 2 LEDs in every 100ms
    }
    drone_sta._status = UNLOCK;
    LED1_ON();
    LED2_ON();  // Keep LEDs on
}


