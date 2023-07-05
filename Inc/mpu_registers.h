/** \file mpu_registers.h 
 *  
 *  Created on: Jul 05, 2023
 *  Author: josemcast
*/

#ifndef MPU_REGISTERS_H
#define MPU_REGISTERS_H

#define MPU_ADDRESS                 0x68

#define MPU_SMPLRT_DIV              0x19
#define MPU_CONFIG                  0x1A
#define MPU_GYRO_CONFIG             0x1B
#define MPU_ACCEL_CONFIG            0x1C
#define MPU_FIFO_EN                 0x23

// Accel, Temp and Gyro Reading registers
#define MPU_ACCEL_XOUT_H            0x3B
#define MPU_ACCEL_XOUT_L            0x3C
#define MPU_ACCEL_YOUT_H            0x3D
#define MPU_ACCEL_YOUT_L            0x3E
#define MPU_ACCEL_ZOUT_H            0x3F
#define MPU_ACCEL_ZOUT_L            0x40

#define MPU_TEMP_OUT_H              0x41
#define MPU_TEMP_OUT_L              0x42

#define MPU_GYRO_XOUT_H             0x43
#define MPU_GYRO_XOUT_L             0x44
#define MPU_GYRO_YOUT_H             0x45
#define MPU_GYRO_YOUT_L             0x46
#define MPU_GYRO_ZOUT_H             0x47
#define MPU_GYRO_ZOUT_L             0x48

// Control
#define MPU_SIGNAL_PATH_RESET       0x68
#define MPU_USER_CTRL               0x6A
#define MPU_PWR_MGMT_1              0x6B
#define MPU_PWR_MGMT_2              0x6C
#define MPU_FIFO_COUNTH             0x72
#define MPU_FIFO_COUNTL             0x73
#define MPU_FIFO_R_W                0x74
#define MPU_WHO_AM_I                0x75

#endif  //MPU_REGISTERS_H