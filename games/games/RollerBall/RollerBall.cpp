#include "RollerBall.h"

#include "hardware/adc.h"
#include "pico/stdlib.h"   // stdlib 
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm 
#include "hardware/sync.h" // wait for interrupt 

void RollerBall::init(PicoDisplay &pico_display)
{
  skycols.clear();
  skycols.push_back(Colour(0,0,128));
  skycols.push_back(Colour(0,0,0));
  
  ball.x=hw<<rs;
  ball.y=hh<<rs;
  
  mpu.init(i2c1, 14, 15);
  
  timeOfLastCheck = get_absolute_time();
}

void RollerBall::update(PicoDisplay &pico_display)
{
  	//pico_display.set_pen(0, 0, 0);
	//pico_display.clear();
  
    pico_display.gradientRect(Rect(0,0,w,h),skycols);
    
    //smooth out readings by averaging samples ...
    int samples = 10;
    float temp = 0;
    int32_t acceleration[3],gyro[3];
	for (int32_t i=0; i<3; i++) {
		acceleration[i]=0;
		gyro[i]=0;
	}
    for (int16_t s=0; s<samples; s++) {
		temp+= mpu.read_temperature();
		int16_t acc[3];
		mpu.read_acceleration(acc);
		int16_t gyr[3];
		mpu.read_gyro(gyr);
		for (int32_t i=0; i<3; i++) {
			acceleration[i]+=acc[i];
			gyro[i]+=gyr[i];
		}
	}
	temp = temp / samples;
	for (int32_t i=0; i<3; i++) {
		acceleration[i]=acceleration[i]/samples;
		gyro[i]=gyro[i]/samples;
	}
	std::string tempstr = "Temp = "+ std::to_string(temp);
	std::string accstr = "Acc: X=" + std::to_string(acceleration[0])+", Y="+ std::to_string(acceleration[1])+", Z="+ std::to_string(acceleration[2]);
	std::string gyrstr = "Gyro: X=" + std::to_string(gyro[0])+", Y="+ std::to_string(gyro[1])+", Z="+ std::to_string(gyro[2]);
	
	int32_t sw = w<<rs, sh = h<<rs;
	ball.x-=acceleration[0];
	if (ball.x>sw) ball.x=sw; else if (ball.x<0) ball.x=0;

	ball.y+=acceleration[1];
	if (ball.y>sh) ball.y=sh; else if (ball.y<0) ball.y=0;
		

    pico_display.set_pen(255, 255, 255);
    pico_display.text(tempstr,Point(hw-65,hh-30),500,1);
    pico_display.text(accstr,Point(hw-65,hh-20),500,1);
    pico_display.text(gyrstr,Point(hw-65,hh-10),500,1);
    
    pico_display.set_pen(255, 128, 128);
    pico_display.rectangle(Rect(0,hh,10,abs((float)acceleration[0]/16384.f)*hh));
    pico_display.rectangle(Rect(15,hh,10,abs((float)acceleration[1]/16384.f)*hh));
    pico_display.rectangle(Rect(30,hh,10,abs((float)acceleration[2]/16384.f)*hh));
    
    pico_display.set_pen(128, 255, 128);
    pico_display.rectangle(Rect(45,hh,10,abs((float)gyro[0]/16384.f)*hh));
    pico_display.rectangle(Rect(60,hh,10,abs((float)gyro[1]/16384.f)*hh));
    pico_display.rectangle(Rect(75,hh,10,abs((float)gyro[2]/16384.f)*hh));

    pico_display.set_pen(255, 255, 128);
    pico_display.circle(Point(ball.x>>rs,ball.y>>rs),10);
      
    /*
    mpu.read_raw_accel(acceleration);  //Reads the accel and gyro 
    mpu.read_raw_gyro(gyro);
    gyro[0] -= gyroCal[0];  //Applies the calibration
    gyro[1] -= gyroCal[1];
    gyro[2] -= gyroCal[2];
    mpu.calculate_angles(eulerAngles, acceleration, gyro, absolute_time_diff_us(timeOfLastCheck, get_absolute_time()));  //Calculates the angles
    timeOfLastCheck = get_absolute_time();

    mpu.convert_to_full(eulerAngles, acceleration, fullAngles);
    
    pico_display.set_pen(255, 255, 255);
    std::string roll = "Roll = " + std::to_string(fullAngles[0]);
    std::string pitch = "Pitch = " + std::to_string(fullAngles[1]);
    std::string pid = "ID = " + std::to_string(id);
    pico_display.text(roll,Point(hw-65,hh-30),500,1);
    pico_display.text(pitch,Point(hw-65,hh-40),500,1);
    pico_display.text(pid,Point(hw-65,hh-50),500,1);
*/
}
