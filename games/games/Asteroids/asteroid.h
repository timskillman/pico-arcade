#pragma once
#include "pico_display.hpp"

//#include "sound.h"

using namespace pimoroni;

const int maxAsteroids = 50;
const int asteroidPoints = 10;
const int aspeed = 2;
const int levelUp = 5;

class asteroid {
public:
	asteroid();
	asteroid(int32_t radius, Point posn, Point adir);
	~asteroid() {};

	void create(int32_t rad, Point posn, Point adir);

	void update(PicoDisplay& pico_display);

	bool hit(Point hitpoint, int32_t radius2);

	void destroy() { visible = false; }

	int32_t getRadius() { return radius; }
	Point getPos() { return pos; }

	bool visible;
	int32_t radius;
	Point pos;

private:
	int32_t rot;
	int32_t rotspeed;
	Point points[asteroidPoints];
	Point dir;
	uint32_t hitLevel;
};

class asteroidField
{
public:
	asteroidField() {};
	~asteroidField() {};

	void create(uint32_t number);
	void spawnCluster(uint32_t number, Point pos, int32_t radius);
	void update(PicoDisplay& pico_display);
	bool hit(Point pos, int32_t radius);

	asteroid asteroids[maxAsteroids];
	uint32_t asteroidLevel;
	uint32_t asteroidMax;
	uint32_t asteroidCount;
};
