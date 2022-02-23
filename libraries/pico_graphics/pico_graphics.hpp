#pragma once

#include <string>
#include <cstdint>
#include <algorithm>
#include <vector>
#include "font8_data.hpp"
#include <math.h>

#define DEGRAD 0.01745329f

// a tiny little graphics library for our Pico products
// supports only 16-bit (565) RGB framebuffers
namespace pimoroni {

  typedef uint16_t Pen;

  struct Rect;

  struct Point {
    int32_t x = 0, y = 0;

    Point() = default;
    Point(int32_t x, int32_t y) : x(x), y(y) {}

    inline Point& operator-= (const Point &a) { x -= a.x; y -= a.y; return *this; }
    inline Point& operator+= (const Point &a) { x += a.x; y += a.y; return *this; }
    inline Point& operator*= (const float &a) { x *= a; y *= a; return *this; }
    inline Point operator + (const Point &a) { return Point(x+a.x,y+a.y); }
    inline Point operator - (const Point &a) { return Point(x-a.x,y-a.y); }
    inline Point operator * (const float &a) { return Point(x*a,y*a); }
    inline Point operator - () { return Point(-x,-y); }
    inline Point operator / (const float &a) { return Point(x/a,y/a); }
    
    //angle in degrees ...
    Point rotate(const float &angle) {
        float ca = cos(angle*DEGRAD), sa = sin(angle*DEGRAD);
        return Point(x*ca - y*sa, x*sa + y*ca);
    }
    
    Point clamp(const Rect &r) const;
  };

  struct Rect {
    int32_t x = 0, y = 0, w = 0, h = 0;

    Rect() = default;
    Rect(int32_t x, int32_t y, int32_t w, int32_t h) : x(x), y(y), w(w), h(h) {}
    Rect(const Point &tl, const Point &br) : x(tl.x), y(tl.y), w(br.x - tl.x), h(br.y - tl.y) {}

    bool empty() const;
    bool contains(const Point &p) const;
    bool contains(const Rect &p) const;
    bool intersects(const Rect &r) const;
    Rect intersection(const Rect &r) const;

    void inflate(int32_t v);
    void deflate(int32_t v);
  };

  struct Colour {
      
      Colour() = default;
      Colour(uint8_t red,uint8_t green,uint8_t blue) : red(red), green(green), blue(blue) {}
      
      uint8_t red = 0, green = 0, blue = 0;
  };
  
  class PicoGraphics {
  public:
    uint16_t *frame_buffer;

    Rect      bounds;
    Rect      clip;

    Pen       pen;

    const Font *font;

  public:
    PicoGraphics(uint16_t width, uint16_t height, uint16_t *frame_buffer);
    void set_font(const Font *font);
    void set_pen(uint8_t r, uint8_t g, uint8_t b);
    void set_pen(Pen p);

    constexpr Pen create_pen(uint8_t r, uint8_t g, uint8_t b) {
      uint16_t p = ((r & 0b11111000) << 8) |
                  ((g & 0b11111100) << 3) |
                  ((b & 0b11111000) >> 3);

      return __builtin_bswap16(p);
    };

    void set_clip(const Rect &r);
    void remove_clip();

    Pen* ptr(const Point &p);
    Pen* ptr(const Rect &r);
    Pen* ptr(int32_t x, int32_t y);
    Pen* ptr();
    
    void clear();
    void pixel(const Point &p);
    void pixel_span(const Point &p, int32_t l);
    void rectangle(const Rect &r);
    void gradientRect(const Rect &r, std::vector<Colour> cols);
    void circle(const Point &p, int32_t r, uint32_t crescent = 2<<16);
    void character(const char c, const Point &p, uint8_t scale = 2);
    void text(const std::string &t, const Point &p, int32_t wrap, uint8_t scale = 2);
    void polygon(const std::vector<Point> &points);
    void triangle(Point p1, Point p2, Point p3);
    void trilist(const std::vector<int8_t>& tripoints, Point pos, float scale);
    void line(Point p1, Point p2);
    void fill(const Point &p);
    void triangleTexture(Point p1, Point p2, Point p3, 
        uint16_t* texture, int32_t tw, int32_t th,
        Point tp1, Point tp2, Point tp3);
    void draw_bezier(const Point& p1, const Point& p2, const Point& p3, const Point& p4, int steps = 20);
    Point calc_bezier_point(const Point& p1, const Point& p2, const Point& p3, const Point& p4,float p);
    
  private:
  
    void triTexSection(int32_t &xa, int32_t &xb, int32_t dx, int32_t dx2, int32_t ya, int32_t yb,
    uint16_t* texture, int32_t tw,
    int32_t &txa, int32_t &tya, int32_t &txb, int32_t &tyb,
    int32_t tdxa, int32_t tdya, int32_t tdxb, int32_t tdyb,
    int32_t sx, int32_t bx);
  };

}
