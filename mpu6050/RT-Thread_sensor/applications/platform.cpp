#include <sensor.h>
#include <mpu6500_sensor.h>

MPU6500_Accelerometer acc = MPU6500_Accelerometer("i2c1", MPU6500_ADDRESS_AD0_LOW);
MPU6500_Gyroscope gryo = MPU6500_Gyroscope("i2c1", MPU6500_ADDRESS_AD0_LOW);
