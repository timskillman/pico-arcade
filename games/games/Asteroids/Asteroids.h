/* Classic Asteroids
   By Tim Skillman
   2016
 */
#pragma once

#include "pico_display.hpp"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "ship.h"
#include "asteroid.h"
#include "ufo.h"

using namespace pimoroni;

const int maxMissiles = 20;
const int fireRate = 100000;

class Asteroids {
public:

  Asteroids() {}
  
  Asteroids(PicoDisplay &pico_display) {
    init(pico_display);
  }
  
  void init(PicoDisplay &pico_display);
  void update(PicoDisplay &pico_display);

private:

  int32_t sw = PicoDisplay::WIDTH;
  int32_t sh = PicoDisplay::HEIGHT;
  
  ship myship;
  missile missiles[maxMissiles];
  missile ufomissiles[maxMissiles];
  asteroidField astroField;
  UFO ufo;
  
  uint32_t currentMissile = 0;
	int32_t firetime = 0;
  
  float ctsize = 1;
  bool gameOver = false;
  bool start = true;
  
	int score = 0;
  int highscore = 0;
	int32_t gocol = 0;
	int32_t goinc = 16;
  std::vector<Colour> skycols;
};
