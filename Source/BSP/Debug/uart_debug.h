/********************************************************************************
* @author: Meteor
* @date: 2023/11/2
* @version: 1.0
* @description: This is header file of debug uart.
********************************************************************************/
#ifndef UAV_V1_0_UART_DEBUG_H
#define UAV_V1_0_UART_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "main.h"


    // Macro definition
    #define DEBUG_MODE  0
    #define RX_BUFFER_SIZE  255U
    #define SIZE(x)     (sizeof (x) / sizeof (x[0]))


    // Type definition
    #if DEBUG_MODE
    // Uart rx data
    typedef struct {
        volatile uint8_t _len;  // Length of rx data
        volatile uint8_t _rx_end_flag;  // Flag the end of rx data
        uint8_t *_buffer;   // Buffer of rx data
    }uart_rx_data_t;
    // Uart debug mode
    typedef enum {
        NONE,
        PPM,
        GYRO    // Choose mode of sending data by uart
    }debug_mode_e;


    // External functions
    HAL_StatusTypeDef Debug_UartIdleITInit();
    void Debug_UartIdleITHandler(DMA_HandleTypeDef *uart_dma_rx_stream);
    HAL_StatusTypeDef Debug_UartDataProcess();

    #endif

#ifdef __cplusplus
}
#endif

#endif //UAV_V1_0_UART_DEBUG_H
