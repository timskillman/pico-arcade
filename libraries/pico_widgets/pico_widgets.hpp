#pragma once

#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>

#include "pico_display.hpp"

namespace pimoroni {

	void draw_sevensegs(PicoDisplay &pico_display, const Point& pos, int num, int digits, int size, Pen col1, Pen col2);
	void draw_measurements(PicoDisplay &pico_display, const Rect& rect, int value, int ticks, int tick_dist, int tick_length, int tick_thick, int tick_major, int tick_major_length, Pen col, std::vector<Colour>& cols);
	void draw_meter(PicoDisplay &pico_display, const Rect& rect, const Point& center, int radius, int value, int start_angle, int end_angle, int tick_dist, int tick_length, int tick_thick, int tick_major, int tick_major_length, Pen col, std::vector<Colour>& cols);
}
