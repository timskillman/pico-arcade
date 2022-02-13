#pragma once

#include "pico_display.hpp"
#include "col3.h"

using namespace pimoroni;

class Missile {
public:
  void update(PicoDisplay& pico_display);
	void fire(Point startPos, Point endPos, float speed, Pen trailcol, col3 explodecol);
  bool hit(Point pos);
  bool Visible() { return visible; }
  Point getPosition() { return Point(x,y); }
  void kill();
  
  int32_t maxrad = 40;
  
private:
  Point startPoint = { 0, 0 };
	Point endPoint = { 0, 0 };
	bool visible = false;
  float x = 0, y = 0;
	float dx = 0, dy = 0;
  int16_t steps = 1;
  bool exploding = false;
  int32_t exprad = 0;
  int32_t currad = 0;

  Pen trail_col;
  col3 explode_col;

};

class Bomb : public Missile
{
public:

  void drop(Point startPos, Point endPos, float speed, int32_t bombbase, Pen trailcol, col3 explodecol);

  int32_t getBombBase() { return base_bomb; }
  
private:
  int32_t base_bomb = 0;
  
};
