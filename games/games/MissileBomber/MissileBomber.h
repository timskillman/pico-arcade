#pragma once

#include "pico_display.hpp"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "Missiles.h"
#include "col3.h"

using namespace pimoroni;

const int max_Missiles = 20;
const int maxBombs = 50;

class MissileBomber {
public:

  MissileBomber() {}
	
  MissileBomber(PicoDisplay &pico_display) {
    init(pico_display);
  }
  
  void init(PicoDisplay &pico_display);
  void update(PicoDisplay &pico_display);

private:

  void drawCross(PicoDisplay &pico_display, Point cross);
  void initBombs(int32_t bombs);
  void initBomb(int32_t no);
  
  Point cross;
  Missile missiles[max_Missiles];
  Bomb bombs[maxBombs];
  
  Pen missile_pen;
  Pen bomb_pen;
  
  int32_t w = PicoDisplay::WIDTH;
  int32_t h = PicoDisplay::HEIGHT;
  int32_t hw = w / 2;
  int32_t hh = h / 2;
  
  std::vector<Colour> skycols;
  
  uint32_t currentMissile = 0;
	int32_t firetime = 0;
  int32_t bombLevel = 0;
  int32_t bombCount = 0;
  int32_t score = 0;
  int32_t highscore = 0;
  int32_t base_lives[9];
  int32_t base_dist = 10;
  int32_t total_lives = 0;
  int32_t wave = 1;
  int32_t wave_time;
};

