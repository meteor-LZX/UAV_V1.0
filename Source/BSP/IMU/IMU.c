/********************************************************************************
* @author: Meteor
* @date: 2023/11/15
* @version: 1.0
* @description: This is source file for getting data of MPU6050.
********************************************************************************/
#include "IMU.h"
#include "ppm.h"


static int16_t Mpu6050Addr = 0x68;
mpu_data_t mpu6050_data;


int8_t Sensor_I2C2_Read(uint16_t DevAddr, uint16_t MemAddr, uint8_t *oData, uint8_t DataLen)
{
    return HAL_I2C_Mem_Read(&hi2c2,DevAddr,MemAddr,1,oData,DataLen,1000);
}

int8_t Sensor_I2C2_Write(uint16_t DevAddr, uint16_t MemAddr, uint8_t *iData, uint8_t DataLen)
{
    return HAL_I2C_Mem_Write(&hi2c2,DevAddr,MemAddr,1,iData,DataLen,1000);
}

int16_t Sensor_I2C2_Serch(void)
{
    for(uint8_t i = 1; i < 255; i++)
    {
        if(HAL_I2C_IsDeviceReady(&hi2c2, i, 1, 1000) == HAL_OK)
        {
            Mpu6050Addr = i;
            return i;
        }
    }
    return 0xD1;
}

int8_t MPU6050_Init(int16_t Addr)
{
    uint8_t check;
    HAL_I2C_Mem_Read(&hi2c2,Addr,WHO_AM_I,1,&check,1,1000);
    if(check == 0x68) // 确认设备用 地址寄存器
    {
        check = 0x00;
        Sensor_I2C2_Write(Addr,PWR_MGMT_1,&check, 1); 	    // 唤醒
        check = 0x07;
        Sensor_I2C2_Write(Addr,SMPLRT_DIV,&check, 1);	    // 1Khz的速率
        check = 0x00;
        Sensor_I2C2_Write(Addr,ACCEL_CONFIG,&check, 1);	 	// 加速度配置
        check = 0x00;
        Sensor_I2C2_Write(Addr,GYRO_CONFIG,&check, 1);		// 陀螺配置
        return 0;
    }
    return -1;
}

void MPU6050_Read_Accel(void)
{
    uint8_t Read_Buf[6];

    // 寄存器依次是加速度X高 - 加速度X低 - 加速度Y高位 - 加速度Y低位 - 加速度Z高位 - 加速度Z低位
    Sensor_I2C2_Read(Mpu6050Addr, ACCEL_XOUT_H, Read_Buf, 6);

    mpu6050_data.Accel_X = (int16_t)(Read_Buf[0] << 8 | Read_Buf[1]);
    mpu6050_data.Accel_Y = (int16_t)(Read_Buf[2] << 8 | Read_Buf[3]);
    mpu6050_data.Accel_Z = (int16_t)(Read_Buf[4] << 8 | Read_Buf[5]);

//    Mpu6050_Data.Accel_X = Mpu6050_Data.Accel_X / 16384.0f;
//    Mpu6050_Data.Accel_Y = Mpu6050_Data.Accel_Y / 16384.0f;
//    Mpu6050_Data.Accel_Z = Mpu6050_Data.Accel_Z / 16384.0f;

}
void MPU6050_Read_Gyro(void)
{
    uint8_t Read_Buf[6];

    // 寄存器依次是角度X高 - 角度X低 - 角度Y高位 - 角度Y低位 - 角度Z高位 - 角度Z低位
    Sensor_I2C2_Read(Mpu6050Addr, GYRO_XOUT_H, Read_Buf, 6);

    mpu6050_data.Gyro_X = (int16_t)(Read_Buf[0] << 8 | Read_Buf[1]);
    mpu6050_data.Gyro_Y = (int16_t)(Read_Buf[2] << 8 | Read_Buf[3]);
    mpu6050_data.Gyro_Z = (int16_t)(Read_Buf[4] << 8 | Read_Buf[5]);

//    Mpu6050_Data.Gyro_X = Mpu6050_Data.Gyro_X / 131.0f;
//    Mpu6050_Data.Gyro_Y = Mpu6050_Data.Gyro_Y / 131.0f;
//    Mpu6050_Data.Gyro_Z = Mpu6050_Data.Gyro_Z / 131.0f;

}
void MPU6050_Read_Temp(void)
{
    uint8_t Read_Buf[2];

    Sensor_I2C2_Read(Mpu6050Addr, TEMP_OUT_H, Read_Buf, 2);

    mpu6050_data.Temp = (int16_t)(Read_Buf[0] << 8 | Read_Buf[1]);

}

/********************************************************************************
* @author: Meteor
* @date: 2023-11-30
* @brief: Get the data of MPU6050.
* @params: None
* @return: None
********************************************************************************/
void MPU6050_GetData() {
    uint8_t rx_buf[14];

    // Data sequence(continuous): ACC_X,Y,Z; Tem; Gyro_X,Y,Z;
    Sensor_I2C2_Read(Mpu6050Addr, ACCEL_XOUT_H, rx_buf, 14);

    mpu6050_data.Accel_X = (int16_t)(rx_buf[0] << 8 | rx_buf[1]);
    mpu6050_data.Accel_Y = (int16_t)(rx_buf[2] << 8 | rx_buf[3]);
    mpu6050_data.Accel_Z = (int16_t)(rx_buf[4] << 8 | rx_buf[5]);
    mpu6050_data.Temp = (int16_t)(rx_buf[6] << 8 | rx_buf[7]);
    mpu6050_data.Gyro_X = (int16_t)(rx_buf[8] << 8 | rx_buf[9]);
    mpu6050_data.Gyro_Y = (int16_t)(rx_buf[10] << 8 | rx_buf[11]);
    mpu6050_data.Gyro_Z = (int16_t)(rx_buf[12] << 8 | rx_buf[13]);

    // Normalization
    mpu6050_data.Accel_X = mpu6050_data.Accel_X / 16384.0f;
    mpu6050_data.Accel_Y = mpu6050_data.Accel_Y / 16384.0f;
    mpu6050_data.Accel_Z = mpu6050_data.Accel_Z / 16384.0f;
    mpu6050_data.Gyro_X = mpu6050_data.Gyro_X / 131.0f;
    mpu6050_data.Gyro_Y = mpu6050_data.Gyro_Y / 131.0f;
    mpu6050_data.Gyro_Z = mpu6050_data.Gyro_Z / 131.0f;
}
