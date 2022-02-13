#include "ufo.h"
#include "../../Common/Common.h"

///// UFO Class ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void UFO::create(Point startPos) {
	pos = Point(startPos.x << 16, startPos.y <<16);
	dir = Point(0, 0);
	burnDir = Point((fast_rand(256)-128)*1024, (fast_rand(256)-128)*512);
	speed = 0;
	scale = 3.0f;
	points[0] = Point(-7 << 16, 0 << 16);
	points[1] = Point(-3 << 16, -2 << 16);
	points[2] = Point(-1 << 16, -4 << 16);
	points[3] = Point(1 << 16, -4 << 16);
	points[4] = Point(3 << 16, -2 << 16);
	points[5] = Point(7 << 16, 0 << 16);
	points[6] = Point(4 << 16, 2 << 16);
	points[7] = Point(-4 << 16, 2 << 16);
	points[8] = Point(-7 << 16, 0 << 16);

	visible = false;
	fieldStrength = 1.f;
	radius = 10.f * scale;
	appearTime = GetTicks() + 1000000 * (fast_rand(20) + 3);
}

bool UFO::update(PicoDisplay& pico_display)
{
	if (GetTicks() > appearTime && !visible) {
		visible = true;
		burnDir = Point((fast_rand(256)-128)*1024, (fast_rand(256)-128)*512);
		int xx = (burnDir.x<0) ? PicoDisplay::WIDTH : 0;
		int yy = fast_rand(PicoDisplay::HEIGHT);
		pos = Point(xx<<16, yy<<16);
	}
	
	if (visible) {
		pico_display.set_pen(255,255,0);
		for (uint32_t i = 0; i < ufoPoints - 1; i++) {
			Point a = pos + points[i] * scale;
			Point b = pos + points[i + 1] * scale;
			pico_display.line(Point(a.x / 65536, a.y  / 65536), Point(b.x  / 65536, b.y  / 65536));
		}

		pos += burnDir;
		
		//check if ufo goes off screen - then disappear
		int32_t sw = PicoDisplay::WIDTH<<16, sh = PicoDisplay::HEIGHT<<16, sr = radius * 65536;
		
		if (pos.x < 0 || pos.y < 0 || pos.x > sw || pos.y > sh) {
			visible = false;
			appearTime = GetTicks() + 1000000 * (fast_rand(20) + 3);
		}

	}
	return visible;
}


