#include "missile.h"

missile::missile()
{
	pos = Point(0, 0);
	dir = Point(0, 0);
	visible = false;
	points[0] = Point(0, 0);
	points[1] = Point(0, 5<<16);
	rot = 0;
	scale = 1.f;
	radius = 5.f*scale;
	speed = 10.f;
}

void missile::fire(Point startPos, int32_t startRot, int32_t radius)
{
	//playSound(sndShipfire);
	pos = startPos;
	rot = startRot;
	dir = Point(0, 1<<16).rotate(startRot);
	pos += dir * radius;
	dir = dir * speed;
	visible = true;
}

void missile::update(PicoDisplay& pico_display, float scale)
{
	if (visible) {
		for (uint32_t i = 0; i < 2 - 1; i++) {
			pico_display.line((pos + points[i].rotate(rot)*scale)/65536.f, (pos + points[i + 1].rotate(rot)*scale)/65536.f);
		}
		pos += dir;

		int32_t sw = PicoDisplay::WIDTH<<16, sh = PicoDisplay::HEIGHT<<16;
		int32_t bord = 5<<16;
		if ((pos.x > sw+bord) || (pos.x < -bord) || (pos.y > sh+bord) || (pos.y < -bord)) visible = false;
	}
}

bool missile::hit(Point hitpoint, int32_t radius2) {
	if (!visible) return false;
	int32_t srad = radius + radius2;
	int32_t dx = (hitpoint.x - pos.x)/65536, dy = (hitpoint.y - pos.y)/65536;
	return (dx*dx + dy*dy) < (srad*srad);
}
