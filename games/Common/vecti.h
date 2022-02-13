#pragma once

class vec2i {
public:

  vec2i() : x(0), y(0) {}
  vec2i(const int32_t x, const int32_t y) : x(x), y(y) {}
  vec2i operator - () const { return vec2i(-x,-y); }
  vec2i operator - (vec2i rhs) const { return vec2i(x - rhs.x, y - rhs.y); }
  vec2i operator + (vec2i rhs) const { return vec2i(x + rhs.x, y + rhs.y); }
  vec2i operator * (vec2i rhs) const { return vec2i(x * rhs.x, y * rhs.y); }
  vec2i operator * (float v) const { return vec2i(x * v, y * v); }
  vec2i operator / (int32_t v) const { return vec2i(x / v, y / v); }
  bool operator == (vec2i rhs) const { return (x == rhs.x && y == rhs.y); }
  
  int32_t x;
  int32_t y;
    
  
};
