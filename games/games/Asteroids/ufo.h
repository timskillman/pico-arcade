#pragma once

#include "pico_display.hpp"

using namespace pimoroni;

const int ufoPoints = 9;

class UFO {
public:

	UFO() {}
	void create(Point startPos);
	bool update(PicoDisplay& pico_display);
	
	Point pos;
	float rot;
	bool visible;
	float fieldStrength;
	float radius;
	
private:
	Point center;
	Point dir;
	Point burnDir;
	float speed;
	float scale;
	Point points[ufoPoints];
	uint32_t appearTime = 0;
};

