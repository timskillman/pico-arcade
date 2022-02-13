#pragma once

#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>

#include "pico_display.hpp"
#include "../../drivers/gy521/gy521.h"

using namespace pimoroni;

class RollerBall {
public:

  RollerBall() {}
  
  RollerBall(PicoDisplay &pico_display) {
    init(pico_display);
  }
  
  void init(PicoDisplay &pico_display);
  void update(PicoDisplay &pico_display);

private:
  
  struct pt {
    int32_t      x;
    int32_t      y;
    int8_t      r;
    int32_t     dx;
    int32_t     dy;
    uint16_t pen;
  };

  int32_t w = PicoDisplay::WIDTH;
  int32_t h = PicoDisplay::HEIGHT;
  int32_t hw = w / 2;
  int32_t hh = h / 2;
  int32_t sx = 10;
  int32_t sy = 20;
  uint32_t score = 0;
  uint32_t lives = 3;
  uint32_t level = 0;
  int32_t speed = 3;
  int32_t rs = 9;
  
  //int32_t sampleCount=0;
  GY521 mpu;
  //uint8_t id = 0;
  //int16_t acceleration[3], gyro[3], gyroCal[3], eulerAngles[2], fullAngles[2]; //Declares the variables required for calculations
  absolute_time_t timeOfLastCheck;
  //std::string accstr = "",gyrstr = "",tempstr = "";
  //int32_t acc[3], gyr[3];
  //float tc=0;
  
  bool start = true;
  int maxlevels = 5;

  float btx;
  
  pt ball;
  pt bat;
  
  int highscore = 0;
    
  std::vector<Colour> skycols;
};


