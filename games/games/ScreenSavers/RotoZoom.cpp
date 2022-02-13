/*

MIT No Attribution

Copyright (c) 2020-2021 Mika Tuupola

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

-cut-

SPDX-License-Identifier: MIT-0

Modified by Tim Skillman - fixed point calculations to increase speed

*/
#include "RotoZoom.h"
#include <stdlib.h>
#include <math.h>

#include "becka.h"

using namespace pimoroni;


#define SPEED 1
#define PIXEL_SIZE  2

static int32_t sinlut[360];
static int32_t coslut[360];

void RotoZoom::init(PicoDisplay& pico_display)
{
    /* Generate look up tables - 16 fixed point. */
    for (uint16_t i = 0; i < 360; i++) {
        sinlut[i] = sin(i * M_PI / 180)*65536;
        coslut[i] = cos(i * M_PI / 180)*65536;
    }
}

void RotoZoom::update(PicoDisplay& pico_display)
{
    int32_t s = sinlut[angle];
    int32_t c = coslut[angle];
    int32_t z = s * 1.2;

    uint16_t w1 = PicoDisplay::WIDTH;
    uint16_t w2 = w1*2;
    
    for (uint16_t x = 0; x < PicoDisplay::WIDTH; x += PIXEL_SIZE ) {
        for (uint16_t y = 0; y < PicoDisplay::HEIGHT; y += PIXEL_SIZE ) {

            /* Get a rotated pixel from the head image. */
            
            int16_t u = abs((int16_t)((((x * c - y * s)>>16) * z)>>16) % IMAGE_WIDTH);
            int16_t v = abs((int16_t)((((x * s + y * c)>>16) * z)>>16) % IMAGE_HEIGHT);
            
            uint16_t *color = (uint16_t*) (image + (IMAGE_WIDTH * v + u)*2);
#if PIXEL_SIZE == 1
            *pico_display.ptr(x, y) = *color;
#else
            //2 pixels ...
            uint16_t *pp = (uint16_t*)pico_display.ptr(x, y);
            pp[0] = *color;
            pp[1] = *color;
            
            pp[w1+0] = *color;
            pp[w1+1] = *color;
  #if PIXEL_SIZE == 3 
            //3 pixels ...
            pp[2] = *color;
            pp[w1+2] = *color;
            pp[w2+0] = *color;
            pp[w2+1] = *color;
            pp[w2+2] = *color;
  #endif
#endif
        }
    }
    
    angle = (angle + SPEED) % 360;
    
}
