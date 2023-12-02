/********************************************************************************
* @author: Meteor
* @date: 2023/11/2
* @version: 1.0
* @description: This is source file of debug uart.It is using to parse ppm signal.
********************************************************************************/
#include "ppm.h"
#include "tim.h"


// PPM parse data
ppm_data_t  ppm_data = {
        ._ch_cnt = 0,
        ._ch1_start = 0,
        ._duration = 0U
};


/********************************************************************************
* @author: Meteor
* @date: 2023/11/2
* @brief: PPM signal parse Initialization.
* @params: None
* @return: Hal status
********************************************************************************/
HAL_StatusTypeDef PPM_ParseInit() {
    // Start tim2 counter
    return HAL_TIM_Base_Start(&htim2);
}

/********************************************************************************
* @author: Meteor
* @date: 2023/11/2
* @brief: PPM_PIN external interrupt callback function,it will be called
*         in EXTI0_IRQHandler().
* @params: GPIO_Pin: EXTI line
* @return: None
********************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == PPM_PIN_Pin) {
        // Get high level time
        ppm_data._duration = __HAL_TIM_GET_COUNTER(&htim2);
        HAL_TIM_Base_Stop(&htim2);  // Stop timer 2
        __HAL_TIM_SET_COUNTER(&htim2, 0UL); // Clear counter

        /*
         * Because the period of PPM data is 20ms,and low level time of all 9 channels
         * is 0.4ms always.Besides,high level time of every channel between 0.6ms and 1.6ms.
         * So that means max_idle_time = 20 - (9 * 0.4 + 8 * 0.6) = 11.6ms,
         * and min_idle_time = 20 - (9 * 0.4 + 8 * 1.6) = 3.6ms,which will help to
         * recognize the start of channel 1.
         */
        // If duration between 3.6ms and 11.6ms,which means the start of channel 1
        if ((ppm_data._duration >= 3600) && (ppm_data._duration <= 11600)) {
            ppm_data._ch1_start = 1; // Mark start of ch1 data
            ppm_data._ch_cnt = 0;   // Start channel counting
        }

        if (ppm_data._ch1_start) {
            /* Get current channel's data,which ranges from about 1000 to 2000
             * (including 400us low level time).And the _ppm_data_buf[0] store
             * the idle time,channel x is stored in _ppm_data_buf[x] where x between 1~6.
             */
            ppm_data._ppm_data_buf[ppm_data._ch_cnt] = ppm_data._duration;
            // Switch to the next channel and note the upper limit of the channel value
            if (++ppm_data._ch_cnt >= 8) {
                // The ninth channel is idle channel
                ppm_data._ch_cnt = 0;
            }
        }

        HAL_TIM_Base_Start(&htim2); // Start timer 2 again
    }
}
