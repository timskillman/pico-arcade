#include "gy521.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include <math.h>

int GY521::reg_read(i2c_inst_t *i2c, const uint address, const uint8_t reg, uint8_t *buf, const uint8_t bufsz) {

    if (bufsz < 1) return 0;

    // Read from I2C
    i2c_write_blocking(i2c, address, &reg, 1, true);
    int num_bytes_read = i2c_read_blocking(i2c, address, buf, bufsz, false);

    return num_bytes_read;
}

void GY521::reg_write(i2c_inst_t* i2c, const uint address, const uint8_t reg, uint8_t* buf, const uint8_t bufsz)
{
    if (bufsz < 1) return;

    size_t wbufsz = bufsz + 1;
    uint8_t writebuf[wbufsz];

    writebuf[0] = reg;
    for (int i = 0; i < bufsz; i++) {
        writebuf[i + 1] = buf[i];
    }

    // Write to I2C
    i2c_write_blocking(i2c, address, writebuf, wbufsz, false);
}

void GY521::init(i2c_inst_t* i2cport, uint8_t sda, uint8_t scl)
{

    // Initialize chosen serial port
    stdio_init_all();

    //Initialize I2C port at 400 kHz
    i2c = i2c1; //Remember to note which SPI port number you r SDA/SDL pins are on! (Either i2c0 or i2c1)
    sda_pin = sda;
    scl_pin = scl;
    uint baudrate = i2c_init(i2c, 400 * 1000);

    // Initialize I2C pins
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    
    //Wake up device
    uint8_t buf[] = {REG_PWR_MGMT_1, 0x00};
    i2c_write_blocking(i2c, GY521_ADDR, buf, 2, false);
    
    sleep_ms(5);

    // Buffer to store raw reads
    uint8_t data[2];
    
    //get device acceleration range
    reg_read(i2c, GY521_ADDR, REG_ACCEL_CONFIG, data, 2);
    accel_range = data[0];
    
    switch (accel_range) {
	case ACC_RNG_2G: acc_scaler = ACC_SCLR_2G; break;
	case ACC_RNG_4G: acc_scaler = ACC_SCLR_4G; break;
	case ACC_RNG_8G: acc_scaler = ACC_SCLR_8G; break;
	case ACC_RNG_16G: acc_scaler = ACC_SCLR_16G; break;
    }
	
    //get device gyro range
    reg_read(i2c, GY521_ADDR, REG_GYRO_CONFIG, data, 2);
    gyro_range = data[0];
}

float GY521::read_temperature()
{
	uint8_t data[2];
	reg_read(i2c, GY521_ADDR, REG_TEMP_OUT, data, 2);
	int16_t temp = (data[0]<<8) | data[1];
	return (temp / 340.f) + 36.53f;
}

void GY521::read_acceleration(int16_t acceleration[3])
{
    	uint8_t data[6];
	reg_read(i2c, GY521_ADDR, REG_ACCEL_XOUT, data, 6);
	
	acceleration[0] = ((data[0]<<8) | data[1]);// / acc_scaler;
	acceleration[1] = ((data[2]<<8) | data[3]);// / acc_scaler;
	acceleration[2] = ((data[4]<<8) | data[5]);// / acc_scaler;
}

void GY521::read_gyro(int16_t gyro[3])
{
    uint8_t data[6];
	reg_read(i2c, GY521_ADDR, REG_GYRO_XOUT, data, 6);
	
	gyro[0] = ((data[0]<<8) | data[1]);// / acc_scaler;
	gyro[1] = ((data[2]<<8) | data[3]);// / acc_scaler;
	gyro[2] = ((data[4]<<8) | data[5]);// / acc_scaler;
}

void GY521::calculate_angles(int16_t eulerAngles[2], int16_t accel[3], int16_t gyro[3], uint64_t usSinceLastReading) //Calculates angles based on the accelerometer and gyroscope. Requires usSinceLastReading to use the gyro.
{
    long hertz = 1000000.l/usSinceLastReading;
    
    if (hertz < 200.l)
    {
        calculate_angles_from_accel(eulerAngles, accel);
        return;
    }

    long temp = 1.l/(hertz * 65.5l);  

    eulerAngles[0] += gyro[0] * temp;
    eulerAngles[1] += gyro[1] * temp;

    eulerAngles[0] += eulerAngles[1] * sin(gyro[2] * temp * 0.1f);
    eulerAngles[1] -= eulerAngles[0] * sin(gyro[2] * temp * 0.1f);

    int16_t accelEuler[2];
    calculate_angles_from_accel(accelEuler, accel);

    eulerAngles[0] = eulerAngles[0] * 0.9996 + accelEuler[0] * 0.0004;
    eulerAngles[1] = eulerAngles[1] * 0.9996 + accelEuler[1] * 0.0004;
}

void GY521::calculate_angles_from_accel(int16_t eulerAngles[2], int16_t accel[3]) //Uses just the direction gravity is pulling to calculate angles.
{
    float accTotalVector = sqrt((accel[0] * accel[0]) + (accel[1] * accel[1]) + (accel[2] * accel[2]));

    float anglePitchAcc = asin(accel[1] / accTotalVector) * 57.296;
    float angleRollAcc = asin(accel[0] / accTotalVector) * -57.296;

    eulerAngles[0] = anglePitchAcc;
    eulerAngles[1] = angleRollAcc;
}
