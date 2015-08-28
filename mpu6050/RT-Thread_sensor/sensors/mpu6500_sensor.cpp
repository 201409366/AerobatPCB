/*
 * File      : mpu6500_sensor.cpp
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2014, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2014-12-20     Bernard      the first version
 */

#include <string.h>
#include <stdio.h>
#include <rtdevice.h>

#include "mpu6500_sensor.h"

/*
 * NOTE: if you use Keil MDK, please add '--gnu' to C/C++ compiler setting of this file.
 */

const static sensor_t _mpu6500_sensor[] = 
{
    {
    .name = "Accelerometer",
    .vendor = "Invensense",
    .version = sizeof(sensor_t), 
    .handle = 0, 
    .type = SENSOR_TYPE_ACCELEROMETER, 
    .maxRange = SENSOR_ACCEL_RANGE_16G,
    .resolution = 1.0f, 
    .power = 0.5f,
    .minDelay = 10000, 
    .fifoReservedEventCount = 0, 
    .fifoMaxEventCount = 64, 
    },
    {
    .name = "Gyroscope",
    .vendor = "Invensense",
    .version = sizeof(sensor_t), 
    .handle = 0, 
    .type = SENSOR_TYPE_GYROSCOPE, 
    .maxRange = SENSOR_GYRO_RANGE_2000DPS,
    .resolution = 1.0f,
    .power = 0.5f,
    .minDelay = 10000, 
    .fifoReservedEventCount = 0, 
    .fifoMaxEventCount = 64, 
    }
};

MPU6500::MPU6500(int sensor_type, const char* iic_bus, int addr)
	: SensorBase(sensor_type)
{
    this->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(iic_bus);
    if (this->i2c_bus == NULL) 
    {
        printf("MPU6500: No IIC device:%s\n", iic_bus);
        return;
    }

    this->i2c_addr = addr;

    /* register to sensor manager */
    SensorManager::registerSensor(this);
}

int MPU6500::read_reg(rt_uint8_t reg, rt_uint8_t *value)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = this->i2c_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = this->i2c_addr;
    msgs[1].flags = RT_I2C_RD; /* Read from slave */
    msgs[1].buf   = (rt_uint8_t *)value;
    msgs[1].len   = 1;

    if (rt_i2c_transfer(this->i2c_bus, msgs, 2) == 2)
        return RT_EOK;

    return -RT_ERROR;
}

int MPU6500::read_buffer(rt_uint8_t reg, rt_uint8_t* value, rt_size_t size)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = this->i2c_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = this->i2c_addr;
    msgs[1].flags = RT_I2C_RD; /* Read from slave */
    msgs[1].buf   = (rt_uint8_t *)value;
    msgs[1].len   = size;

    if (rt_i2c_transfer(this->i2c_bus, msgs, 2) == 2)
        return RT_EOK;

    return -RT_ERROR;
}

int MPU6500::write_reg(rt_uint8_t reg, rt_uint8_t value)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = this->i2c_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = this->i2c_addr;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf   = (rt_uint8_t *)&value;
    msgs[1].len   = 1;

    if (rt_i2c_transfer(this->i2c_bus, msgs, 2) == 2)
        return RT_EOK;
    
    return -RT_ERROR;
}

MPU6500_Accelerometer::MPU6500_Accelerometer(const char* iic_name, int addr)
    : MPU6500(SENSOR_TYPE_ACCELEROMETER, iic_name, addr)
{
	int index;
	uint8_t id;
	rt_uint8_t value[6];
	rt_int32_t x, y, z;
    SensorConfig config = {SENSOR_MODE_NORMAL, SENSOR_DATARATE_400HZ, SENSOR_ACCEL_RANGE_2G};

    /* initialize MPU6500 */
    write_reg(MPU6500_PWR_MGMT_1,   0x80);
    write_reg(MPU6500_PWR_MGMT_1,   0x81);
    write_reg(MPU6500_PWR_MGMT_2,   0x00);
    write_reg(MPU6500_GYRO_CONFIG,  0x18);
    write_reg(MPU6500_ACCEL_CONFIG, 0x08);
    write_reg(MPU6500_ACCEL_CONFIG_2, 0x09);
    write_reg(MPU6500_INT_PIN_CFG,  0x30);

	x_offset = y_offset = z_offset = 0;
	x = y = z = 0;

	/* read MPU6500 id */
	read_reg(MPU6500_WHOAMI, &id);
	if (id != MPU6500_ID)
	{
		printf("Warning: not found MPU6500 id: %02x\n", id);
	}

	/* get offset */
	for (index = 0; index < 200; index ++)
	{
		read_buffer(MPU6500_ACCEL_XOUT_H, value, 6);

		x += (((rt_int16_t)value[0] << 8)   | value[1]);
		y += (((rt_int16_t)value[2] << 8)   | value[3]);
		z += (((rt_int16_t)value[4] << 8)   | value[5]);		
	}
	x_offset = x / 200;
	y_offset = y / 200;
	z_offset = z / 200;
    
	this->enable = RT_FALSE;
	this->sensitivity = SENSOR_ACCEL_SENSITIVITY_2G;
    this->config = config;
}

int 
MPU6500_Accelerometer::configure(SensorConfig *config)
{
	int range;
	uint8_t value;

	if (config == RT_NULL) return -1;

	/* TODO: set datarate */

	/* get range and calc the sensitivity */
	range = config->range.accel_range;
	switch (range)
	{
	case SENSOR_ACCEL_RANGE_2G:
		this->sensitivity = SENSOR_ACCEL_SENSITIVITY_2G;
		range = 0;
		break;
	case SENSOR_ACCEL_RANGE_4G:
		this->sensitivity = SENSOR_ACCEL_SENSITIVITY_4G;
		range = 0x01 << 2;
		break;
	case SENSOR_ACCEL_RANGE_8G:
		this->sensitivity = SENSOR_ACCEL_SENSITIVITY_8G;
		range = 0x02 << 2;
		break;
	case SENSOR_ACCEL_RANGE_16G:
		this->sensitivity = SENSOR_ACCEL_SENSITIVITY_16G;
		range = 0x03 << 2;
		break;
	default:
		return -1;
	}

	/* set range to sensor */
	read_reg(MPU6500_ACCEL_CONFIG, &value);
	value &= ~(0x3 << 2);
	value |= range;
	write_reg(MPU6500_ACCEL_CONFIG, value);

    return 0;
}

int 
MPU6500_Accelerometer::activate(int enable)
{
	uint8_t value;

    if (enable && this->enable == RT_FALSE)
    {
        /* enable accelerometer */
		read_reg(MPU6500_PWR_MGMT_2, &value); 
		value &= ~(0x07 << 2);
		write_reg(MPU6500_PWR_MGMT_2, value);
    }

	if (!enable && this->enable == RT_TRUE)
    {
        /* disable accelerometer */
		read_reg(MPU6500_PWR_MGMT_2, &value); 
		value |= (0x07 << 2);
		write_reg(MPU6500_PWR_MGMT_2, value);
    }

	if (enable) this->enable = RT_TRUE;
	else this->enable = RT_FALSE;

    return 0;
}

int 
MPU6500_Accelerometer::poll(sensors_event_t *event)
{
	rt_uint8_t value[6];
	rt_int16_t x, y, z;

    /* parameters check */
    if (event == NULL) return -1;

	/* get event data */
	event->version = sizeof(sensors_event_t);
	event->sensor = (int32_t) this;
	event->timestamp = rt_tick_get();
	event->type = SENSOR_TYPE_ACCELEROMETER;

	read_buffer(MPU6500_ACCEL_XOUT_H, value, 6);

	/* get raw data */
	x = (((rt_int16_t)value[0] << 8) | value[1]);
	y = (((rt_int16_t)value[2] << 8) | value[3]);
	z = (((rt_int16_t)value[4] << 8) | value[5]);

	if (config.mode == SENSOR_MODE_RAW)
	{
		event->raw_acceleration.x = x;
		event->raw_acceleration.y = y;
		event->raw_acceleration.z = z;
	}
	else
	{
		x -= x_offset; y -= y_offset; z -= z_offset;
		event->acceleration.x = x * this->sensitivity * SENSORS_GRAVITY_STANDARD;
		event->acceleration.y = y * this->sensitivity * SENSORS_GRAVITY_STANDARD;
		event->acceleration.z = z * this->sensitivity * SENSORS_GRAVITY_STANDARD;
	}
	
	return 0;
}

void 
MPU6500_Accelerometer::getSensor(sensor_t *sensor)
{
    /* get sensor description */
    if (sensor)
    {
        memcpy(sensor, &_mpu6500_sensor[0], sizeof(sensor_t));
    }
}

MPU6500_Gyroscope::MPU6500_Gyroscope(const char* iic_name, int addr)
    : MPU6500(SENSOR_TYPE_GYROSCOPE, iic_name, addr)
{
	int index;
	uint8_t id;
	rt_uint8_t value[6];
	rt_int32_t x, y, z;

    /* initialize MPU6500 */
    write_reg(MPU6500_PWR_MGMT_1,   0x80);
    write_reg(MPU6500_PWR_MGMT_1,   0x81);
    write_reg(MPU6500_PWR_MGMT_2,   0x00);
    write_reg(MPU6500_GYRO_CONFIG,  0x18);
    write_reg(MPU6500_ACCEL_CONFIG, 0x08);
    write_reg(MPU6500_ACCEL_CONFIG_2, 0x09);
    write_reg(MPU6500_INT_PIN_CFG,  0x30);

	x_offset = y_offset = z_offset = 0;
	x = y = z = 0;

	/* read MPU6500 id */
	read_reg(MPU6500_WHOAMI, &id);
	if (id != MPU6500_ID)
	{
		printf("Warning: not found MPU6500 id: %02x\n", id);
	}

	/* get offset */
	for (index = 0; index < 200; index ++)
	{
		read_buffer(MPU6500_GYRO_XOUT_H, value, 6);

		x += (((rt_int16_t)value[0] << 8)   | value[1]);
		y += (((rt_int16_t)value[2] << 8)   | value[3]);
		z += (((rt_int16_t)value[4] << 8)   | value[5]);		
	}
	x_offset = x / 200;
	y_offset = y / 200;
	z_offset = z / 200;

	this->enable = RT_FALSE;
	this->sensitivity = SENSOR_GYRO_SENSITIVITY_250DPS;
}

int 
MPU6500_Gyroscope::configure(SensorConfig *config)
{
	int range;
	uint8_t value;

	if (config == RT_NULL) return -1;

	/* TODO: set datarate */

	/* get range and calc the sensitivity */
	range = config->range.gyro_range;
	switch (range)
	{
	case SENSOR_GYRO_RANGE_250DPS:
		this->sensitivity = SENSOR_GYRO_SENSITIVITY_250DPS;
		range = 0;
		break;
	case SENSOR_GYRO_RANGE_500DPS:
		this->sensitivity = SENSOR_GYRO_SENSITIVITY_500DPS;
		range = 0x01 << 2;
		break;
	case SENSOR_GYRO_RANGE_1000DPS:
		this->sensitivity = SENSOR_GYRO_SENSITIVITY_1000DPS;
		range = 0x02 << 2;
		break;
	case SENSOR_GYRO_RANGE_2000DPS:
		this->sensitivity = SENSOR_GYRO_SENSITIVITY_2000DPS;
		range = 0x03 << 2;
		break;
	default:
		return -1;
	}

	/* set range to sensor */
	read_reg(MPU6500_GYRO_CONFIG, &value);
	value &= ~(0x3 << 2);
	value |= range;
	write_reg(MPU6500_GYRO_CONFIG, value);

    return 0;
}

int 
MPU6500_Gyroscope::activate(int enable)
{
	uint8_t value;
	
    if (enable && this->enable == RT_FALSE)
    {
        /* enable gyroscope */
		read_reg(MPU6500_PWR_MGMT_1, &value);
		value &= ~(0x01 << 4);
		write_reg(MPU6500_PWR_MGMT_1, value);

		read_reg(MPU6500_PWR_MGMT_2, &value); 
		value &= ~(0x07 << 0);
		write_reg(MPU6500_PWR_MGMT_2, value);
    }

	if (!enable && this->enable == RT_TRUE)
    {
        /* disable gyroscope */
		read_reg(MPU6500_PWR_MGMT_2, &value); 
		value |= (0x07 << 0);
		write_reg(MPU6500_PWR_MGMT_2, value);
    }

	if (enable) this->enable = RT_TRUE;
	else this->enable = RT_FALSE;

    return 0;
}

int 
MPU6500_Gyroscope::poll(sensors_event_t *event)
{
	rt_uint8_t value[6];
	rt_int16_t x, y, z;

	/* parameters check */
	if (event == NULL) return -1;

	/* get event data */
	event->version = sizeof(sensors_event_t);
	event->sensor = (int32_t) this;
	event->timestamp = rt_tick_get();
	event->type = SENSOR_TYPE_GYROSCOPE;

	read_buffer(MPU6500_GYRO_XOUT_H, value, 6);

	/* get raw data */
	x = (((rt_int16_t)value[0] << 8) | value[1]);
	y = (((rt_int16_t)value[2] << 8) | value[3]);
	z = (((rt_int16_t)value[4] << 8) | value[5]);

	
	if (config.mode == SENSOR_MODE_RAW)
	{
		event->raw_gyro.x = x;
		event->raw_gyro.y = y;
		event->raw_gyro.z = z;
	}
	else
	{
		x -= x_offset; y -= y_offset; z -= z_offset;
		event->gyro.x = x * this->sensitivity * SENSORS_DPS_TO_RADS;
		event->gyro.y = y * this->sensitivity * SENSORS_DPS_TO_RADS;
		event->gyro.z = z * this->sensitivity * SENSORS_DPS_TO_RADS;
	}
	
	return 0;
}

void 
MPU6500_Gyroscope::getSensor(sensor_t *sensor)
{
    /* get sensor description */
    if (sensor)
    {
        memcpy(sensor, &_mpu6500_sensor[1], sizeof(sensor_t));
    }
}

