#pragma once

//Code based on https://github.com/andreban/pico-fire
//Copyright (c) Andreban, Feb 21

#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>

#include "pico_display.hpp"
#include "../../Common/Common.h"

using namespace pimoroni;
    
class Fire {
public:

    Fire() {}

    Fire(PicoDisplay& pico_display) {
        init(pico_display);
    }

    void init(PicoDisplay& pico_display);
    void update(PicoDisplay& pico_display);

private:

    const uint32_t w = PicoDisplay::WIDTH;
    const uint32_t h = PicoDisplay::HEIGHT;
    
    uint8_t fire[PicoDisplay::WIDTH * PicoDisplay::HEIGHT];
    uint16_t pallete[36];
    
    int posAt(int x, int y);

    int8_t wind = 0;

    bool x_pressed = false;
    bool y_pressed = false;
    bool b_pressed = false;
    bool a_pressed = false;
    bool enabled = true;
};
