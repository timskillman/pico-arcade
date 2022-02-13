#pragma once

class col3 {
public:

  col3() : red(0), green(0), blue(0) {}
  
  col3(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
  
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};
