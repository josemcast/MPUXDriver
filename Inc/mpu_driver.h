/** \file mpu_driver.h 
 * 
 * Created on: Jul 05, 2023
 * Author: josemcast
*/

#ifndef MPU_DRIVER_H
#define MPU_DRIVER_H

#include "mpu_registers.h"

/**
 * @brief Return status for all API
 * 
 */
typedef enum {MPU_STATUS_OK, MPU_STATUS_NOT_INIT, MPU_STATUS_ERROR, MPU_STATUS_COMM_ERROR, MPU_STATUS_WRONG_CONFIGS} mpuRetStatus_t;


/**
 * @brief Full Scale configuration for both Gyro and Accelerometer
 * 
 * For Gyro the Full Scale config is:
 * FS_SEL0 = 250 °/s
 * FS_SEL1 = 500 °/s
 * FS_SEL2 = 1000 °/s
 * FS_SEL3 = 2000 °/s
 * 
 * For Accelerometer the Full Scale config is:
 * FS_SEL0 = 2g
 * FS_SEL0 = 4g
 * FS_SEL0 = 8g
 * FS_SEL0 = 16g
 */
typedef enum {FS_SEL0, FS_SEL1, FS_SEL2, FS_SEL3} mpuFSConfig_t;

/**
 * @brief callback signature for TX and RX functions used to communicate with MPU Unit
 * 
 * This functions should be defined by user and used with @mpuInit function
 */
typedef mpuRetStatus_t (*callbackSignature)(uint8_t regAddress, uint8_t *data, uint8_t size);


/********************************************************************************
*************************** API Declaration *************************************
********************************************************************************/

/**
 * @brief MPU init function
 * 
 * @param gyroFS Full scale config for Gyro
 * @param accelFS Full scale config for Accelerometer
 * @param tx    Callback function for transmitting data to MPU Unit
 * @param rx    Callback function for receiving data from MPU Unit
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t mpuInit(mpuFSConfig_t accelFS, mpuFSConfig_t gyroFS, callbackSignature tx, callbackSignature rx);

/**
 * @brief Check if MPU driver is ready to be utilized
 * 
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t isMPUDriverReady();

/**
 * @brief Get Accelerometer measurements without scale and convert it
 * 
 * @param buffer buffer of 3 unit16_t accelerometer measurements
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t mpuReadAccelerometerRaw(uint16_t *buffer);


/**
 * @brief Get Gyro measurementes without scale and convert it 
 * 
 * @param buffer buffer of 3 uint16_t gyro measurements
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t mpuReadGyroRaw(uint16_t *buffer);

/**
 * @brief Get temperature measurements withou scale and convert it
 * 
 * @param buffer buffer of one uint16_t to hold temperature measurement
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t mpuReadTemperatureRaw(uint16_t *buffer);

/**
 * @brief Read accelerometer, temperature and gyroscope raw values from MPU unit
 * 
 * @param buffer buffer of size 7 to hold measurements from accelerometer, temperature and gyro 
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t mpuReadAllRaw(uint16_t *buffer);

/**
 * @brief Read accelerometer values from MPU Unit and scale it according to defined FS config
 * 
 * @param buffer buffer of 3 float accelerometer measurements
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t mpuReadAccelerometer(float *buffer);

/**
 * @brief Read gyro values from MPU Unit and scale it according to defined FS config
 * 
 * @param buffer 
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t mpuReadGyro(float *buffer);


/**
 * @brief Read temperature value from MPU Unit and scale it
 * 
 * @param buffer to hold temperature measurement
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t mpuReadTemperature(float *buffer);


/**
 * @brief Reset all MPU registers to default value. mpuInit(...) should be called again with proper config options
 * 
 * @return mpuRetStatus_t 
 */
mpuRetStatus_t mpuDeviceReset();


/********************************************************************************
*************************** Utils Functions *************************************
********************************************************************************/

/**
 * @brief concatenate two bytes variables into one hald word variable
 * 
 * @param upperByte upper 8 bits for the 16bits variable
 * @param lowerByte lower 8 bits for the 16bits variable
 * @return uint16_t 
 */
uint16_t concatenateBytesIntoHalfWord(uint8_t upperByte, uint8_t lowerByte);
#endif // MPU_REGISTERS_H