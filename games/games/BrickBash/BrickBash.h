#pragma once

#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>

#include "pico_display.hpp"

using namespace pimoroni;

class BrickBash {
public:

  BrickBash() {}
  
  BrickBash(PicoDisplay &pico_display) {
    init(pico_display);
  }
  
  void init(PicoDisplay &pico_display);
  void update(PicoDisplay &pico_display);

private:
  
  void draw_game(PicoDisplay &pico_display);
  void game_status(PicoDisplay &pico_display);
  void collisions(PicoDisplay &pico_display);
  void controllers(PicoDisplay &pico_display);
  
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
  int8_t fontsize = 2;
  
  bool start = true;
  int maxlevels = 5;
  
  uint16_t bricks[8][8];
  Pen brick_cols[8];
  Pen bat_cols[5];
  int32_t brickWidth = w / 16;
  int32_t brickHeight = h / 16;
  int32_t brickx = brickWidth /2;
  int16_t bricky = 4;

  float btx;
  
  pt ball;
  pt bat;
  
  int highscore = 0;
    
  std::vector<Colour> skycols;
};

