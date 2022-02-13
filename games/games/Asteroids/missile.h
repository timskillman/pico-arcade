#pragma once

#include "pico_display.hpp"

using namespace pimoroni;


class missile {
public:
	missile();

	void update(PicoDisplay& pico_display, float scale);
	void fire(Point startPos, int32_t startRot, int32_t radius);
	bool hit(Point hitpoint,  int32_t radius2);
	
	bool visible = false;
	Point pos;
	int32_t radius=1;

private:
	Point dir;
	Point points[2];
	float rot = 0;
	float scale = 1.f;
	float speed = 1.f;
};
