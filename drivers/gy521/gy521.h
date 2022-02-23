#pragma once

#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Code based on several sources:
// Credits:
// https://github.com/Robocraze/MPU6050
// https://github.com/adafruit/Adafruit_CircuitPython_MPU6050
// https://github.com/emlid/Navio/blob/master/C%2B%2B/Navio/MPU9250.cpp
// https://github.com/KleistRobotics/Pico-MPU-9250/
// Shawn Hymel https://www.digikey.co.uk/en/maker/projects/raspberry-pi-pico-rp2040-i2c-example-with-micropython-and-cc/47d0c922b79342779cdbd4b37b7eb7e2

class GY521 {
	public:
	
	GY521() {}
	
	// I2C address
	const int GY521_ADDR = 0x68;
	
	// Registers
	const uint8_t REG_DEVID = 0x68;
	const uint8_t REG_PWR_MGMT_1 = 0x6B; //0x2D;
	const uint8_t REG_ACCEL_XOUT = 0x3B; //read 0x3B,0x3C,0x3D,0x3E,0x3F,0x40 - x,y,z acceleration (hi/lo bytes)
    const uint8_t REG_TEMP_OUT = 0x41;   //read 0x41,042 - temp (hi/lo bytes)
    const uint8_t REG_GYRO_XOUT = 0x43;  //read 0x43,0x44,0x45,0x46,0x47,0x48 - x,y,z gyro (hi/lo bytes)
    const uint8_t REG_ACCEL_CONFIG = 0x1C;
    const uint8_t REG_GYRO_CONFIG = 0x1B;
    
	// Other constants
	const float SENSITIVITY_2G = 1.0 / 256;
	const float EARTH_GRAVITY = 9.80665;     // Earth's gravity

	//Scale Modifiers
	const float ACC_SCLR_2G = 16384.0;
	const float ACC_SCLR_4G = 8192.0;
	const float ACC_SCLR_8G = 4096.0;
	const float ACC_SCLR_16G = 2048.0;

	const float GYR_SCLR_250DEG = 131.0;
	const float GYR_SCLR_500DEG = 65.5;
	const float GYR_SCLR_1000DEG = 32.8;
	const float GYR_SCLR_2000DEG = 16.4;

	//Pre-defined ranges
	enum { ACC_RNG_2G = 0x00, 
		   ACC_RNG_4G = 0x08, 
		   ACC_RNG_8G = 0x10, 
		   ACC_RNG_16G = 0x18};

	enum { GYR_RNG_250DEG = 0x00,
		   GYR_RNG_500DEG = 0x08,
		   GYR_RNG_1000DEG = 0x10,
		   GYR_RNG_2000DEG = 0x18};
    
	// I2C setup
	i2c_inst_t* i2c = nullptr;
    uint8_t sda_pin = -1;
    uint8_t scl_pin = -1;
    
	int reg_read(i2c_inst_t* i2c,
		const uint address,
		const uint8_t reg,
		uint8_t* buf,
		const uint8_t bufsz);
					
	void reg_write(i2c_inst_t* i2c,
		const uint address,
		const uint8_t reg,
		uint8_t* buf,
		const uint8_t bufsz);

	void init(i2c_inst_t* i2cport, uint8_t sda, uint8_t scl);
	
	float read_temperature();
	void read_acceleration(int16_t acceleration[3]);
	void read_gyro(int16_t gyro[3]);
	void read_acc_gyro_smooth(int32_t acceleration[3], int32_t gyro[3], uint8_t samples);
	void calculate_angles_from_accel(int16_t eulerAngles[2], int16_t accel[3]);
	void calculate_angles(int16_t eulerAngles[2], int16_t accel[3], int16_t gyro[3], uint64_t usSinceLastReading);
		
	int8_t accel_range = -1;
	int8_t gyro_range = -1;
	float acc_scaler = ACC_SCLR_2G;
	float gyr_scaler = GYR_SCLR_250DEG;
	    
};
