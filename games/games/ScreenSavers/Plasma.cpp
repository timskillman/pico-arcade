#include "Plasma.h"

//uint16_t *palette;
//uint8_t *plasma;

static const uint16_t hw = PicoDisplay::WIDTH/2, hh = PicoDisplay::HEIGHT/2;
static std::vector<uint8_t> plasma(hw * hh);
static std::vector<uint16_t> palette(256);

static const uint8_t PSPEED = 1;
//static const uint8_t PIXEL_SIZE = 2;

void Plasma::init(PicoDisplay& pico_display)
{
    uint8_t *ptr = &plasma[0];

    /* Generate nice continous palette. */
    for(uint16_t i = 0; i < 256; i++) {
        const uint8_t r = 128.0f + 128.0f * sin((M_PI * i / 128.0f) + 1);
        const uint8_t g = 128.0f + 128.0f * sin((M_PI * i / 64.0f) + 1);
        const uint8_t b = 128.0f + 128.0f * sin((M_PI * i / 32.0f) + 1);
        palette[i] = pico_display.create_pen(r, g, b);
    }

    for (uint16_t y = 0; y < hh; y++) {
        for (uint16_t x = 0; x < hw; x++) {
                /* Generate three different sinusoids. */
                const float v1 = 128.0f + (128.0f * sin(x / 32.0f));
                const float v2 = 128.0f + (128.0f * sin(y / 24.0f));
                const float v3 = 128.0f + (128.0f * sin(sqrt(x * x + y * y) / 24.0f));
                /* Calculate average of the three sinusoids */
                /* and use it as color index. */
                const uint8_t color = (v1 + v2 + v3) / 3;
                *(ptr++) = color;
        }
    }
}

void Plasma::update(PicoDisplay& pico_display)
{
    uint8_t *ptr = &plasma[0];

    for (uint16_t y = 0; y < hh; y++) {
        for (uint16_t x = 0; x < hw; x++) {
            /* Get a color for pixel from the plasma buffer. */
            const uint8_t index = *(ptr++);
            const Pen color = palette[index];
            /* Put a pixel to the display. */
            uint16_t *pp = (uint16_t*)pico_display.ptr(x*2, y*2);
            pp[0] = color;
            pp[1] = color;
            pp[PicoDisplay::WIDTH] = color;
            pp[PicoDisplay::WIDTH+1] = color;
        }
    }

    ptr = &plasma[0];
    for (uint16_t y = 0; y < hh; y++) {
        for (uint16_t x = 0; x < hw; x++) {
                /* Get a color from plasma and choose the next color. */
                /* Unsigned integers wrap automatically. */
                const uint8_t index = *ptr + PSPEED;
                /* Put the new color back to the plasma buffer. */
                *(ptr++) = index;
        }
    }
}
