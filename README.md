# MPUX Driver

Simple MPU6050 driver written in C for with using I2C communication

## How to Use

- Clone repository.
- Configure Include and source files into the project.
- Create wrappers functions for transmission and reception functions using third-party I2C driver (bare metal or from vendor's HAL).
- See `Examples` folder for further info on how to create and use wrappers functions for transmission e reception.

## Unit Tests

The driver provides an Unit test routine for testing the API. To run it, change directory to `Tests` folder and run command

```
$make test
```

## Disclaimer
Still under development for including another features
