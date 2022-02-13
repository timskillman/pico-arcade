#pragma once

#include "pico_display.hpp"
#include "missile.h"

using namespace pimoroni;

const int shipPoints = 5;

class ship {
public:

	ship() {}
	void create(Point startPos, int32_t startRot);
	void update(PicoDisplay& pico_display);
	void jet(float acc);

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
	float decay;
	float scale;
	Point points[shipPoints];
	Point thrustPoints[3];
	bool showThrust;
};
