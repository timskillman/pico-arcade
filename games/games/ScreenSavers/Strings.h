/*

MIT License

Copyright (c) 2020-2022, Tim Skillman

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

using namespace pimoroni;

static const int MAX_BEZIERS = 100;

class Strings {
public:

  Strings() {}
  
  Strings(PicoDisplay &pico_display) {
    init(pico_display);
  }
  
  void init(PicoDisplay &pico_display);
  void update(PicoDisplay &pico_display);

private:

  struct Bezier
  {
    
    Bezier() {}
  
    Bezier(const Point p1, const Point p2, const Point p3, const Point p4, int32_t col) : p1(Point(p1.x<<16, p1.y<<16)), p2(Point(p2.x<<16, p2.y<<16)), p3(Point(p3.x<<16, p3.y<<16)), p4(Point(p4.x<<16, p4.y<<16)), col(col)  {}
    
    void draw(PicoDisplay &pico_display) 
    {
      pico_display.set_pen(col);
      pico_display.draw_bezier(Point(p1.x>>16, p1.y>>16),Point(p2.x>>16, p2.y>>16),Point(p3.x>>16, p3.y>>16),Point(p4.x>>16, p4.y>>16));
    }
    
    void movePoint(Point& p, Point& pm, const int w, const int h)
    {
      p.x+=pm.x; if (p.x>(w<<16) || p.x<0) { pm.x=-pm.x; p.x+=pm.x; }
      p.y+=pm.y; if (p.y>(h<<16) || p.y<0) { pm.y=-pm.y; p.y+=pm.y; }
    }
    
    void animate()
    {
      movePoint(p1,pm1,PicoDisplay::WIDTH,PicoDisplay::HEIGHT);
      movePoint(p2,pm2,PicoDisplay::WIDTH,PicoDisplay::HEIGHT);
      movePoint(p3,pm3,PicoDisplay::WIDTH,PicoDisplay::HEIGHT);
      movePoint(p4,pm4,PicoDisplay::WIDTH,PicoDisplay::HEIGHT);
    }
    
    Point p1, p2, p3, p4, pm1, pm2, pm3, pm4;
    int32_t col;
  };

  Bezier beziers[MAX_BEZIERS];
};

