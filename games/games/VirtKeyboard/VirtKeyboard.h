/*

MIT No Attribution

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
#include "hardware/adc.h"

using namespace pimoroni;

class VirtKeyboard {
public:

  VirtKeyboard() {}
  
  VirtKeyboard(PicoDisplay &pico_display) {
    init(pico_display);
  }
  
  void init(PicoDisplay &pico_display);
  void update(PicoDisplay &pico_display);

private:
  int hw = PicoDisplay::WIDTH / 2;
  int hh = PicoDisplay::HEIGHT / 2;
  int tx = 0, ty = 0;
  Pen green;
  string[4] kbd = { "A B C D E F G H I J", 
                    "K L M N O P Q R S T", 
                    "U V W X Y Z _ @ . /", 
                    "0 1 2 3 4 5 6 7 8 9" };
  
};
