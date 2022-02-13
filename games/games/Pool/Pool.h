#include "pico_display.hpp"
#include "pico/stdlib.h"
#include "hardware/adc.h"


// Code based on C++ Ball Physics tutorial:
// https://writeach.com/posts/-LzU_6lcwPFl4wMit5sQ/C%2B%2B-Ball-Physics-Tutorial-4---Balls-Collision

using namespace pimoroni;

class Ball {

public:
	Ball() {}
	Ball(int32_t radius);

	void draw(PicoDisplay& pico_display);

	void setPosition(int32_t x, int32_t y) { pos = Point(x,y); }  //16fp
	void setVelocity(int32_t x, int32_t y) { velocity = Point(x*0.98f,y*0.98f); }  //16fp
	void setDragged(bool dragged);

	Point velocity;
	Point pos;
	Pen col;
	Pen dcol;
	
	bool dragged;
	//int32_t mass;
	int32_t radius = 10;
};

class BouncingBalls {
public:

  Pool() {}
  
  Pool(PicoDisplay &pico_display) {
    init(pico_display);
  }
  
  void init(PicoDisplay &pico_display);
  void update(PicoDisplay &pico_display);
 
private:

  bool ballOverlap(Ball& ball1, Ball& ball2);
  int32_t distanceSquared(Point &a, Point &b);
  
  std::vector<Ball> balls;
  
};
