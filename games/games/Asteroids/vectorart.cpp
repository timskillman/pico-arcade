#include "vectorart.h"

void vecart::drawString(PicoDisplay &pico_display, std::string text, Point pos, Point size)
{
	float dx = size.x/3;
	float dy = size.y/2;
	for (size_t i = 0; i < text.size(); i++) {
		int l = text[i]-33;
		if (l >= 0) {
			std::string vecChars = letterPaths[l];
			float x = 0; float ox = x;
			float y = 0; float oy = y;
			bool mv = true; bool nom = false;
			for (size_t v = 0; v < vecChars.size(); v++) {
				uint8_t c = vecChars[v] - 46;
				if (c > 0) {
					x = (float)((uint8_t)((c - 2) / 3)) * dx;
					y = (float)((c - 2) % 3) * dy;
					if (!mv) {
						pico_display.line(Point(pos.x + ox, pos.y + oy), Point(pos.x + x, pos.y + y)); nom = false;
					}
					else nom = true;
					ox = x; oy = y;
					mv = false;
				}
				else { // .
					if (mv || nom) pico_display.circle(pos + Point(ox, oy), dx / 3);
					mv = true;
				}
			}
			if (mv || nom) pico_display.circle(pos + Point(ox, oy), dx / 3);
		}
		pos.x += dx * 3.f;
	}
}
