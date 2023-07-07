# MPUX Driver

Simple MPU6050 driver written in C for using with I2C communication

## How to Use

- Clone repository ( and submodules in cause one wants to run Unit Tests).
- Configure Include and source files into the project.
- Create wrappers functions for transmission and reception functions using third-party I2C driver (bare metal or from vendor's HAL).
- See `Examples` folder for further info on how to create and use wrappers functions for transmission e reception.

## Current features

- Get raw accelerometer, gyro and temperature measurements from MPU unit.
- Get and scale accelerometer, gyro and temperature according to full scale configs and sensitivity defined in datasheet.
- MPU reset function.

## Unit Tests

The driver provides an Unit Test routine for the API. To run it, change directory to `Tests` and run command

```
$make test
```

## Disclaimer
Still under development to include another features
