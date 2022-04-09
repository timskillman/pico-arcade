#pragma once

#include "pico_display.hpp"
#include "MetaBalls.h"
#include "Plasma.h"
#include "RotoZoom.h"
#include "StarField.h"
#include "BouncingBalls.h"
#include "Strings.h"
#include "Triangles.h"
#include "GfxTest.h"
#include "../../Common/Common.h"

using namespace pimoroni;

class ScreenSaver {
public:

  ScreenSaver() {}
  
  ScreenSaver(PicoDisplay &pico_display) {
    init(pico_display);
  }
  
  void init(PicoDisplay &pico_display);
  void update(PicoDisplay &pico_display);

private:

  MetaBalls metaballs;
  Plasma plasma;
  RotoZoom rotozoom;
  StarField starfield;
  BouncingBalls bouncingballs;
  Strings strings;
  Triangles triangles;
  GfxTest gfx;
  
  int currentScreen = 0;
  int maxScreens = 8;
  int nextScreenTime;
};

