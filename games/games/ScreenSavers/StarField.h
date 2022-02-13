/*

MIT License

Copyright (c) 2020-2021 Tim Skillman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "pico_display.hpp"
#include "pico/stdlib.h"
#include "hardware/adc.h"

using namespace pimoroni;

static int32_t w = PicoDisplay::WIDTH;
static int32_t h = PicoDisplay::HEIGHT;
static int32_t hw = w / 2;
static int32_t hh = h / 2;
static const int MAX_STARS = 128;

class StarField {
public:

  StarField() {}
  
  StarField(PicoDisplay &pico_display) {
    init(pico_display);
  }
  
  void init(PicoDisplay &pico_display);
  void update(PicoDisplay &pico_display);

private:

  struct star
  {
    
    star() {}
  
    star(int32_t _x, int32_t _y, int32_t _z, int32_t _zspeed) {
      x=_x; y=_y; z=_z; zspeed = _zspeed;
      oldpos = calcPoint();
    }
    
    Point calcPoint() {
      return Point((x*w) / z + hw, (y*w) / z + hh);
    }
    
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t zspeed;
    Point oldpos;
  };

  star stars[MAX_STARS];
};

