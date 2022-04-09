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


  static uint16_t cos90[90] = { 
    0x0000, 0xFFF6, 0xFFD8, 0xFFA6, 0xFF60, 0xFF06, 0xFE98, 0xFE17, 0xFD82, 0xFCD9, 0xFC1C, 0xFB4B, 0xFA67, 0xF970, 0xF865, 
    0xF746, 0xF615, 0xF4D0, 0xF378, 0xF20D, 0xF08F, 0xEEFF, 0xED5B, 0xEBA6, 0xE9DE, 0xE803, 0xE617, 0xE418, 0xE208, 0xDFE7, 
    0xDDB3, 0xDB6F, 0xD919, 0xD6B3, 0xD43B, 0xD1B3, 0xCF1B, 0xCC73, 0xC9BB, 0xC6F3, 0xC41B, 0xC134, 0xBE3E, 0xBB39, 0xB826, 
    0xB504, 0xB1D5, 0xAE97, 0xAB4C, 0xA7F3, 0xA48D, 0xA11B, 0x9D9B, 0x9A10, 0x9679, 0x92D5, 0x8F27, 0x8B6D, 0x87A8, 0x83D9, 
    0x7FFF, 0x7C1C, 0x782F, 0x7438, 0x7039, 0x6C30, 0x681F, 0x6406, 0x5FE6, 0x5BBD, 0x578E, 0x5358, 0x4F1B, 0x4AD8, 0x4690, 
    0x4241, 0x3DEE, 0x3996, 0x3539, 0x30D8, 0x2C74, 0x280C, 0x23A0, 0x1F32, 0x1AC2, 0x164F, 0x11DB, 0x0D65, 0x08EF, 0x0477 };

    inline int32_t fastcos(int16_t degree) {
      int32_t d=(degree>=90) ? degree % 90 : degree; 
      switch (((degree>=360) ? degree % 360 : degree) / 90) {
        case 0: return (d==0) ? 0x10000 : (int32_t)cos90[d];
        case 1: return (d==0) ? 0 : -(int32_t)cos90[90 - d];
        case 2: return (d==0) ? -0x10000 : -(int32_t)cos90[d];
        case 3: return (d==0) ? 0 : (int32_t)cos90[90 - d];
      }
    }

    inline int32_t fastsin(int16_t degree) {
      int32_t d=(degree>=90) ? degree % 90 : degree; 
      switch (((degree>=360) ? degree % 360 : degree) / 90) {
        case 0: return (d==0) ? 0 :(int32_t)cos90[90 - d];
        case 1: return (d==0) ? 0x10000 : (int32_t)cos90[d];
        case 2: return (d==0) ? 0 : -(int32_t)cos90[90 - d];
        case 3: return (d==0) ? -0x10000 : -(int32_t)cos90[d];
      }
    }

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
    Point rotate(const int16_t angle) {
        float ca = cos(angle*DEGRAD), sa = sin(angle*DEGRAD);
        return Point(x*ca - y*sa, x*sa + y*ca);
    }
    
    //fast_rotate - x,y cannot be fixed point values (e.g. 16fp)
    Point fast_rotate(const uint16_t angle) {
        int32_t ca = fastcos(angle), sa = fastsin(angle);
        return Point((x*ca - y*sa)>>16, (x*sa + y*ca)>>16);
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
    void gradientRect(const Rect &r, std::vector<Colour>& cols, bool vert = false);
    void gradientCircle(const Point& pos, int16_t radius, std::vector<Colour>& cols, uint32_t crescent = 2<<16);
    void gradientRectRounded(const Rect& rect, int16_t radius, std::vector<Colour>& cols, uint32_t crescent = 2<<16);
    void circle(const Point &p, int32_t r, uint32_t crescent = 2<<16);
    void character(const char c, const Point &p, uint8_t scale = 2);
    void text(const std::string &t, const Point &p, int32_t wrap, uint8_t scale = 2);
    void polygon(const std::vector<Point> &points);
    void triangle(Point p1, Point p2, Point p3);
    void triangleCol(Point p0, Point p1, Point p2, uint32_t c0, uint32_t c1, uint32_t c2);
    void trilist(const std::vector<int8_t>& tripoints, Point pos, int32_t scale, int32_t angle); //16fp scale and rotate
    void line(Point p1, Point p2);
    void fill(const Point &p);
    void triangleTexture(Point p1, Point p2, Point p3, 
        uint16_t* texture, int32_t tw, int32_t th,
        Point tp1, Point tp2, Point tp3);
    void draw_bezier(const Point& p1, const Point& p2, const Point& p3, const Point& p4, int steps = 20);
    Point calc_bezier_point(const Point& p1, const Point& p2, const Point& p3, const Point& p4,float p);
    
    void initRots();
    //int32_t fastcos(uint16_t angle) { return coslut[angle % 360]; }
    //int32_t fastsin(uint16_t angle) { return sinlut[angle % 360]; }
    
  private:
  
    void setColourByHeight(uint32_t c, std::vector<Colour>& cols);
    void triTexSection(int32_t &xa, int32_t &xb, int32_t dx, int32_t dx2, int32_t ya, int32_t yb,
      uint16_t* texture, int32_t tw,
      int32_t &txa, int32_t &tya, int32_t &txb, int32_t &tyb,
      int32_t tdxa, int32_t tdya, int32_t tdxb, int32_t tdyb,
      int32_t sx, int32_t bx);
    
    int32_t sinlut[360];
    int32_t coslut[360];
  };

}
