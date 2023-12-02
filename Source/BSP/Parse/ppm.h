/********************************************************************************
* @author: Meteor
* @date: 2023/11/2
* @version: 1.0
* @description: This is header file of ppm signal parsing.
********************************************************************************/
#ifndef UAV_V1_0_PPM_H
#define UAV_V1_0_PPM_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "main.h"


    // Type definition
    typedef struct {
        volatile uint8_t _ch1_start; // Mark start of channel 1
        volatile uint16_t _duration;   // Duration between 2 falling edge,whose unit is microsecond
        volatile uint8_t _ch_cnt;   // Mark current channel
        // My PPM receiver have 6 channel useful data and 9 channel data actually,
        // (Use logic analyzer to get the result).
        // But I only need 4 channel at present.
        volatile uint16_t _ppm_data_buf[9];
    }ppm_data_t;


    // External variables
    extern ppm_data_t  ppm_data;


    // External functions
    HAL_StatusTypeDef PPM_ParseInit();


#ifdef __cplusplus
}
#endif

#endif //UAV_V1_0_PPM_H
