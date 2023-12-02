/********************************************************************************
* @author: Meteor
* @date: 2023/11/2
* @version: 1.0
* @description: This is source file of debug uart.It is using to debug.
********************************************************************************/
#include "uart_debug.h"
#if DEBUG_MODE
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "ppm.h"
#include "IMU.h"


// Rx data buffer,and the max size was setting 255
static uint8_t rx_data_buf[RX_BUFFER_SIZE] = {0};
static uart_rx_data_t rx_data = {
        ._len = 0,
        ._rx_end_flag = 0,
        ._buffer = rx_data_buf
};


/********************************************************************************
* @author: Meteor
* @date: 2032/11/2
* @brief: Uart idle interrupt initialization.
* @params: None
* @return: HAL status
********************************************************************************/
HAL_StatusTypeDef Debug_UartIdleITInit() {
    // Enable uart idle interrupt
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    // Start uart rx with dma
    return HAL_UART_Receive_DMA(&huart1, rx_data._buffer, RX_BUFFER_SIZE);
}

/********************************************************************************
* @author: Meteor
* @date: 2032/11/2
* @brief: Uart idle IT handling function,it will be called in USART1_IRQHandler().
* @params: uart_dam_rx_handler: Uart rx dma stream handler.
* @return: None
********************************************************************************/
void Debug_UartIdleITHandler(DMA_HandleTypeDef *uart_dma_rx_stream) {
    // Idle IT be triggered
    if (RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        // Stop uart rx dma
        HAL_UART_DMAStop(&huart1);
        // Get length of rx data
        rx_data._len = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(uart_dma_rx_stream);
        rx_data._rx_end_flag = 1;   // Mark the end of rx
    }
}

/********************************************************************************
* @author: Meteor
* @date: 2023/11/2
* @brief: Processing uart debug data.
* @params: None
* @return: HAL status
********************************************************************************/
HAL_StatusTypeDef Debug_UartDataProcess() {
    static debug_mode_e debug_mode = NONE;
    // End of rx
    if (rx_data._rx_end_flag) {
        rx_data._rx_end_flag = 0; // Clear flag

        // Send ppm data
        if (rx_data._buffer[0] == 'P' && rx_data._buffer[1] == 'P' && rx_data._buffer[2] == 'M') {
            debug_mode = PPM;
        }
        // Send adc data
        else if (rx_data._buffer[0] == 'G' && rx_data._buffer[1] == 'Y' && rx_data._buffer[2] == 'R' && rx_data._buffer[3] == 'O') {
            debug_mode = GYRO;
        }
        // Stop send data
        else if (rx_data._buffer[0] == 'I' && rx_data._buffer[1] == 'D' && rx_data._buffer[2] == 'L' && rx_data._buffer[3] == 'E') {
            debug_mode = NONE;
        }

        memset(rx_data._buffer, 0x00, rx_data._len);
        rx_data._len = 0;   // Clear len
    }

    // Send specific data
    if (debug_mode == PPM) {
        char uart_temp[50];
        sprintf(uart_temp, "CH1:%u,CH2:%u,CH3:%u,CH4:%u,CH5:%u,CH6:%u", \
                  ppm_data._ppm_data_buf[1], ppm_data._ppm_data_buf[2], \
                  ppm_data._ppm_data_buf[3], ppm_data._ppm_data_buf[4], \
                  ppm_data._ppm_data_buf[5], ppm_data._ppm_data_buf[6]);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *) uart_temp, SIZE(uart_temp));
        HAL_Delay(10);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *) "\r\n", 3);
        HAL_Delay(1);
    } else if (debug_mode == GYRO) {
        // Get gyroscope data
        MPU6050_GetData();

        char uart_temp[80];
        sprintf(uart_temp, "a_x:%.2f,a_y:%.2f,a_z:%.2f---g_x:%.2f,g_y:%.2f,g_z:%.2f---",
                mpu6050_data.Accel_X, mpu6050_data.Accel_Y, mpu6050_data.Accel_Z,
                mpu6050_data.Gyro_X, mpu6050_data.Gyro_Y, mpu6050_data.Gyro_Z);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)uart_temp, SIZE(uart_temp));
        LED2_TOGGLE();
        LED1_TOGGLE();
        HAL_Delay(100);
    } else {
        char uart_temp[] = "Please send the following string to select debug mode:\r\n1.PPM\r\n2.GYRO\r\n3.IDLE\r\n";
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)uart_temp, SIZE(uart_temp));
        HAL_Delay(1000);
    }

    // Start rx again
    return HAL_UART_Receive_DMA(&huart1, rx_data._buffer, RX_BUFFER_SIZE);
}

#endif