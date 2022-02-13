#include "ship.h"


///// Ship Class ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ship::create(Point startPos, int32_t startRot) {
	center = startPos;
	pos = Point(startPos.x << 16, startPos.y <<16);
	rot = startRot;
	dir = Point(0, 0);
	burnDir = Point(0, 0);
	speed = 0;
	decay = 0.97f;
	scale = 2.0f;
	points[0] = Point(0 << 16, 10 << 16);
	points[1] = Point(7 << 16, -4 << 16);
	points[2] = Point(0 << 16, -1 << 16);
	points[3] = Point(-7 << 16, -4 << 16);
	points[4] = Point(0, 10 << 16);
	thrustPoints[0] = Point(3 << 16, -4 << 16);
	thrustPoints[1] = Point(0 << 16, -7 << 16);
	thrustPoints[2] = Point(-3 << 16, -4 << 16);
	showThrust = false;
	fieldStrength = 1.f;
	radius = 10.f * scale;
}

void ship::update(PicoDisplay& pico_display)
{
	if (visible) {
		pico_display.set_pen(255,255,0);
		Point sc = pos; 
		for (uint32_t i = 0; i < shipPoints - 1; i++) {
			Point a = sc + points[i].rotate(rot) * scale;
			Point b = sc + points[i + 1].rotate(rot) * scale;
			pico_display.line(Point(a.x / 65536, a.y  / 65536), Point(b.x  / 65536, b.y  / 65536));
		}
		if (showThrust) {
			pico_display.set_pen(255,128,0);
			for (uint32_t i = 0; i < 2; i++) {
				Point a = sc + thrustPoints[i].rotate(rot) * scale;
				Point b = sc + thrustPoints[i + 1].rotate(rot) * scale;
				pico_display.line(Point(a.x / 65536, a.y  / 65536), Point(b.x  / 65536, b.y  / 65536));
			}
			showThrust = false;
		}

		pos += burnDir;
		
		//check if ship goes off screen and make it reappear on the other side...
		int32_t sw = PicoDisplay::WIDTH<<16, sh = PicoDisplay::HEIGHT<<16, sr = radius * 65536;
		if (pos.x < -sr) pos.x = sw + sr;
		if (pos.y < -sr) pos.y = sh + sr;
		if (pos.x > sw + sr) pos.x = -sr;
		if (pos.y > sh + sr) pos.y = -sr;

		if (speed != 0.f) {
			speed = speed * decay;
			burnDir = burnDir * decay;
		}
	}
}

void ship::jet(float acc)
{
	if (visible) {
		//trx.SetRot(rot);
		if (speed > -3.f && speed < 3.f) {
			//playSound(sndThrust);
			burnDir += Point(0, acc*65536).rotate(rot);
			speed += acc;
		}
		showThrust = true;
	}
}

