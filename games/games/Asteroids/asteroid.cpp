#include "asteroid.h"

///// Asteroid Class ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

asteroid::asteroid() :visible(true), rot(0), rotspeed(0) {}

#define PINT2 411566  //6.28*65536  (16fp)

asteroid::asteroid(int32_t rad, Point posn, Point adir)
{
	create(rad, posn, adir);
}

void asteroid::create(int32_t rad, Point posn, Point adir) {
	radius = rad;
	float dv = 360 / (asteroidPoints-1);
	uint32_t c = 0;
	for (int32_t i = 0; i < asteroidPoints; i++) {
		int32_t rr = ((float)rad + ((float)(rand() % 100) / 10))*65536;
		points[c++] = Point(0, rr).rotate((float)i * dv);
	}
	rot = (rand() % 360);
	rotspeed = (rand() % 10) - 5;
	points[c - 1] = points[0];
	pos = Point(posn.x * 65536,posn.y * 65536);
	dir = adir;
	hitLevel = (uint32_t)rad;
	visible = true;
};

void asteroid::update(PicoDisplay& pico_display) {
	//return;
	if (visible) {
		for (uint32_t i = 0; i < (asteroidPoints-1); i++) {
			Point a = pos + points[i].rotate(rot);
			Point b = pos + points[i + 1].rotate(rot);
			pico_display.line(Point(a.x / 65536, a.y / 65536), Point(b.x / 65536, b.y / 65536));
		}
		rot += rotspeed; //rotate asteroid
		pos += dir*aspeed; //move asteroid
		Point sp = pos;

		int32_t r2 = (radius*2)<<16;
		if (sp.x < -r2) pos.x = (PicoDisplay::WIDTH<<16) + r2;
		if (sp.y < -r2) pos.y = (PicoDisplay::HEIGHT<<16) + r2;
		if (sp.x > (PicoDisplay::WIDTH<<16) + r2) pos.x = -r2;
		if (sp.y > (PicoDisplay::HEIGHT<<16) + r2) pos.y = -r2;
	}
}

bool asteroid::hit(Point hitpoint, int32_t radius2) {
	if (!visible) return false;
	int32_t srad = radius + radius2;
	int32_t dx = (hitpoint.x - pos.x)/65536, dy = (hitpoint.y - pos.y)/65536;
	return (dx*dx + dy*dy) < (srad*srad);
}

///// Asteroid Field Class ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void asteroidField::create(uint32_t number)
{
	if (number > maxAsteroids) number = maxAsteroids;
	asteroidLevel = number;
	asteroidMax = number;
	asteroidCount = number;
	for (uint32_t i = 0; i < number; i++) {
		asteroids[i].create((rand() % 20) + 5, 
		Point(rand() % PicoDisplay::WIDTH, rand() % PicoDisplay::HEIGHT),
		Point(
		(((float)(rand() % 100) / 100) - 0.5f)*65536, 
		(((float)(rand() % 100) / 100) - 0.5f)*65536));
	}
}

void asteroidField::spawnCluster(uint32_t number, Point pos, int32_t radius)
{
	uint32_t i = 0;
	while (number>0 && asteroidCount<asteroidMax && i<asteroidMax) {
		//resuse destroyed asteroids...
		if (!asteroids[i].visible) {
			asteroids[i].create((float)(rand() % radius) + 5.f, pos / 65536,
				Point((((float)(rand() % 100) / 100) - 0.5f)*65536, (((float)(rand() % 100) / 100) - 0.5f)*65536));
			asteroidCount++;
			number--;
		}
		i++;
	}
	if (number > 0) {
		if ((asteroidMax + number) < maxAsteroids) {
			//Create new asteroids if there's not enough deestroyed asteroids to recreate ...
			for (uint32_t i = 0; i < number; i++) {
				asteroids[asteroidMax + i].create((float)(rand() % radius) + 5.f, pos,
					Point((((float)(rand() % 100) / 100) - 0.5f)*65536, (((float)(rand() % 100) / 100) - 0.5f)*65536));
			}
			asteroidMax += number;
			asteroidCount += number;
		}
	}
}

void asteroidField::update(PicoDisplay& pico_display) {
	pico_display.set_pen(255,255,255);
	for (uint32_t i = 0; i < asteroidMax; i++) {
		asteroids[i].update(pico_display);
	}
}

bool asteroidField::hit(Point pos, int32_t rad) {
	for (uint32_t m = 0; m < asteroidMax; m++) {
		if (asteroids[m].hit(pos, rad)) {
			asteroids[m].destroy();
			uint32_t r = asteroids[m].getRadius() / 5;
			if (r > 2) {
				//playSound(sndExp1);
				spawnCluster(r, asteroids[m].getPos(), r);
			}
			//else playSound(sndExp2);
			asteroidCount--;
			return true;
		}
	}
	return false;
}
