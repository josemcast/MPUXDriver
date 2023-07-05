#include <stdint.h>
#include "mpu_driver.h"
#include <stdio.h>

#define TRUE    1
#define FALSE   0

typedef enum {MPUNotInit, MPUIdle, MPUBusy, MPUError, MPUCommError} MPUStateMachine;
static MPUStateMachine currentState = MPUNotInit;

/**
 * @brief Internal handler for callbacks and configuration
 * 
 * Should be used for transmission and reception of data using callback functions and for calculate and scale
 * measurements after reading using init configuration 
 */
typedef struct 
{
    mpuFSConfig_t gyroFS;
    mpuFSConfig_t accelFS;
    callbackSignature callbackTx;
    callbackSignature callbackRx;

}mpuConfig_t;

static mpuConfig_t internalConfigHandler;

static int8_t errorCount = 0;

static uint16_t fsConfigToScaleAccel[4] = {16384, 8192, 4096, 2048};
static uint16_t fsConfigToScaleGyro[4] = {250, 500, 1000, 2000};


static uint8_t isScaleConfigsInRange(mpuFSConfig_t config1, mpuFSConfig_t config2)
{
    if((config1 < 0) || (config1 > 3))
        return FALSE;
 
    if((config2 < 0) || (config2 > 3))
        return FALSE;

    return TRUE;
}

static mpuRetStatus_t initMPUUnit(void)
{
    
    uint8_t data[2];
    data[0] = MPU_PWR_MGMT_1;
    data[1] = 0x00;
    
    mpuRetStatus_t ret = internalConfigHandler.callbackTx(MPU_ADDRESS, data, 2);

    if(ret == MPU_STATUS_OK)
    {
        if(internalConfigHandler.gyroFS != 0)
        {
            data[0] = MPU_GYRO_CONFIG;
            data[1] = (internalConfigHandler.gyroFS << 3);
            ret = internalConfigHandler.callbackTx(MPU_ADDRESS, data, 2);
            if(ret == MPU_STATUS_COMM_ERROR)
                return ret;
        }

        if(internalConfigHandler.accelFS != 0)
        {
            data[0] = MPU_ACCEL_CONFIG;
            data[1] = (internalConfigHandler.accelFS << 3);
            ret = internalConfigHandler.callbackTx(MPU_ADDRESS, data, 2);
            if(ret == MPU_STATUS_COMM_ERROR)
                return ret;
        }

    }
    
    return ret;
}


mpuRetStatus_t mpuInit(mpuFSConfig_t accelFS, mpuFSConfig_t gyroFS, callbackSignature tx, callbackSignature rx)
{

    if( (tx == 0 || rx == 0) || (isScaleConfigsInRange(gyroFS, accelFS) == FALSE))
        return MPU_STATUS_WRONG_CONFIGS;

    internalConfigHandler.gyroFS = gyroFS;
    internalConfigHandler.accelFS = accelFS;
    internalConfigHandler.callbackTx = tx;
    internalConfigHandler.callbackRx = rx;

    switch(currentState)
    {
        case MPUIdle:
            return MPU_STATUS_OK;
        
        case MPUNotInit: {

            currentState = MPUBusy;
            
            mpuRetStatus_t ret = initMPUUnit();
            currentState = (ret == MPU_STATUS_OK ? MPUIdle : MPUCommError);
            return ret;
            
        }

        case MPUBusy:
            return MPU_STATUS_ERROR;

        default: {
            //Try init device again
    
            mpuRetStatus_t ret = initMPUUnit();
            
            if(ret != MPU_STATUS_OK)
            {
                errorCount++;
                if(errorCount >= 5)
                {
                    currentState = MPUError;
                    return MPU_STATUS_ERROR;
                }

                return ret;
            }

            currentState = MPUIdle;
            return MPU_STATUS_OK;
        }
    }

}

mpuRetStatus_t isMPUDriverReady()
{
    return currentState == MPUIdle ? MPU_STATUS_OK: MPU_STATUS_ERROR;
}

static mpuRetStatus_t getMeasurements(uint8_t regAddress, uint16_t *outBuffer, uint8_t size)
{
    
    uint8_t data[size*2]; //2 unit8 for each axis of the accelerometer
    data[0] = regAddress;

    mpuRetStatus_t ret;

    ret = internalConfigHandler.callbackTx(MPU_ADDRESS, data, 1);
    if(ret == MPU_STATUS_OK)
    {
        ret = internalConfigHandler.callbackRx(MPU_ADDRESS, data, size*2);
        if(ret == MPU_STATUS_OK)
        {
            for(uint8_t i = 0; i < size; i++)
            {
                outBuffer[i] = concatenateBytesIntoHalfWord(data[i*2], data[i*2+1]);  
            }

            return MPU_STATUS_OK;

        } else {
            return MPU_STATUS_COMM_ERROR;
        }
    }
    
    return MPU_STATUS_COMM_ERROR;
}

mpuRetStatus_t mpuReadAccelerometerRaw(uint16_t *buffer)
{

    switch(currentState)
    {
        case MPUNotInit:
            return MPU_STATUS_NOT_INIT;

        case MPUIdle: {

            currentState = MPUBusy;

            mpuRetStatus_t ret = getMeasurements(MPU_ACCEL_XOUT_H, buffer, 3);

            currentState = (ret == MPU_STATUS_OK ? MPUIdle : MPUCommError);  
            return ret;

        }

        default:
            return MPU_STATUS_ERROR;

    }
}

mpuRetStatus_t mpuReadGyroRaw(uint16_t *buffer)
{
    switch(currentState)
    {
        case MPUNotInit:
            return MPU_STATUS_NOT_INIT;

        case MPUIdle: {

            currentState = MPUBusy;

            mpuRetStatus_t ret = getMeasurements(MPU_GYRO_XOUT_H, buffer, 3);

            currentState = (ret == MPU_STATUS_OK ? MPUIdle : MPUCommError);

            return ret;

        }

        default:
            return MPU_STATUS_ERROR;

    }
}

mpuRetStatus_t mpuReadTemperatureRaw(uint16_t *buffer)
{
    switch(currentState)
    {
        case MPUNotInit:
            return MPU_STATUS_NOT_INIT;

        case MPUIdle: {

            currentState = MPUBusy;

            mpuRetStatus_t ret = getMeasurements(MPU_TEMP_OUT_H, buffer, 1);

            currentState = (ret == MPU_STATUS_OK ? MPUIdle : MPUCommError);

            return ret;

        }

        default:
            return MPU_STATUS_ERROR;

    }
}

mpuRetStatus_t mpuReadAllRaw(uint16_t *buffer)
{
    switch(currentState)
    {
        case MPUNotInit:
            return MPU_STATUS_NOT_INIT;

        case MPUIdle: {

            currentState = MPUBusy;

            mpuRetStatus_t ret = getMeasurements(MPU_ACCEL_XOUT_H, buffer, 7);

            currentState = (ret == MPU_STATUS_OK ? MPUIdle : MPUCommError);

            return ret;
        }

        default:
            return MPU_STATUS_ERROR;

    }
}

static void scaleMeasurement(float * buffer, uint16_t *measurements, uint16_t factor)
{
    for(int i = 0; i < 3; i++)
    {
        buffer[i] = ((int16_t) measurements[i] / (float) factor);
    }
}

mpuRetStatus_t mpuReadAccelerometer(float *buffer)
{
    uint16_t internalBuffer[3];
    mpuRetStatus_t ret = mpuReadAccelerometerRaw(internalBuffer);

    if(ret != MPU_STATUS_OK)
        return ret;
    

    scaleMeasurement(buffer, internalBuffer, fsConfigToScaleAccel[internalConfigHandler.accelFS]);

    return MPU_STATUS_OK;
}

mpuRetStatus_t mpuReadGyro(float *buffer)
{
    uint16_t internalBuffer[3];
    mpuRetStatus_t ret = mpuReadGyroRaw(internalBuffer);

    if(ret != MPU_STATUS_OK)
        return ret;
    
    scaleMeasurement(buffer, internalBuffer, fsConfigToScaleGyro[internalConfigHandler.gyroFS]);

    return MPU_STATUS_OK;

}

mpuRetStatus_t mpuReadTemperature(float *buffer)
{
    uint16_t internalBuffer[1];
    mpuRetStatus_t ret = mpuReadTemperatureRaw(internalBuffer);

    if(ret != MPU_STATUS_OK)
        return ret;
        
    *buffer = ((int16_t)internalBuffer[0] / 340.0) + 36.53;

    return MPU_STATUS_OK;
    
}


/* Utils */
uint16_t concatenateBytesIntoHalfWord(uint8_t upperByte, uint8_t lowerByte)
{              
    return ((((uint16_t)upperByte) << 8 ) | lowerByte);
}