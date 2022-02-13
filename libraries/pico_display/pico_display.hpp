#pragma once

#if defined(DISPLAY_DRIVER_ST7735_160_128)
#define DISPLAY_DRIVER_ST7735
#define DISPLAY_WIDTH 161
#define DISPLAY_HEIGHT 130
#endif

#if defined(DISPLAY_DRIVER_ST7735_128_128)
#define DISPLAY_DRIVER_ST7735
#define DISPLAY_WIDTH 131
#define DISPLAY_HEIGHT 130
#define DISPLAY_RGB
#endif

#if defined(DISPLAY_DRIVER_ST7735_160_80)
#define DISPLAY_DRIVER_ST7735
#define DISPLAY_WIDTH 161
#define DISPLAY_HEIGHT 80
#define DISPLAY_INVERT
#define DISPLAY_OFFSET_Y 26
#define DISPLAY_RGB
#endif

#if defined(DISPLAY_DRIVER_ST7789_240_240)
#define DISPLAY_DRIVER_ST7789
#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 240
#define DISPLAY_INVERT
#define DISPLAY_OFFSET_X 80
#endif

#if defined(DISPLAY_DRIVER_ILI9341_320_240)
#define DISPLAY_DRIVER_ILI9341
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_RGB
#endif

#if !defined(DISPLAY_ROTATION)
#define DISPLAY_ROTATION 0
#endif

#if !defined(DISPLAY_OFFSET_X)
#define DISPLAY_OFFSET_X 0
#endif

#if !defined(DISPLAY_OFFSET_Y)
#define DISPLAY_OFFSET_Y 0
#endif

#if defined(DISPLAY_DRIVER_ST7789)
#include "../../drivers/st7789/st7789.hpp"
#elif defined(DISPLAY_DRIVER_ST7735)
#include "../../drivers/st7735/st7735.hpp"
#elif defined(DISPLAY_DRIVER_ILI9341)
#include "../../drivers/ili9341/ili9341.hpp"
#endif

#include "../../libraries/pico_graphics/pico_graphics.hpp"
#include "hardware/spi.h"

namespace pimoroni {

  class PicoDisplay : public PicoGraphics {
  public:

#if (DISPLAY_ROTATION == 90) || (DISPLAY_ROTATION == 270)
    static const int WIDTH = DISPLAY_HEIGHT;
    static const int HEIGHT = DISPLAY_WIDTH;
    static const int OFFSET_X = DISPLAY_OFFSET_Y;
    static const int OFFSET_Y = DISPLAY_OFFSET_X;
#else
    static const int WIDTH = DISPLAY_WIDTH;
    static const int HEIGHT = DISPLAY_HEIGHT;
    static const int OFFSET_X = DISPLAY_OFFSET_X;
    static const int OFFSET_Y = DISPLAY_OFFSET_Y;
#endif
    static const int ROTATION = DISPLAY_ROTATION;
    
    static const int CS = DISPLAY_CS;
    static const int DC = DISPLAY_DC;
    static const int SCK = DISPLAY_SCK;
    static const int MOSI = DISPLAY_MOSI;
    static const int BL = DISPLAY_BL;
    static const int RST = DISPLAY_RST;
    
#ifdef DISPLAY_INVERT
    static const int INVERT = 1;
#else
    static const int INVERT = 0;
#endif

#ifdef DISPLAY_RGB
    static const int RGB = 1;
#else
    static const int RGB = 0;
#endif
    
    static const uint8_t A = BUTTON_A; //
    static const uint8_t B = BUTTON_B; //
    static const uint8_t X = BUTTON_X; //
    static const uint8_t Y = BUTTON_Y; //

    static const uint8_t L = BUTTON_L; //left
    static const uint8_t R = BUTTON_R; //right
    static const uint8_t U = BUTTON_U; //up
    static const uint8_t D = BUTTON_D; //down
    static const uint8_t F = BUTTON_F; //fire
    
    static const uint8_t USBIN = 24; //usb plugged in or not?
    
    uint16_t *__fb;
    spi_inst_t *spi = spi0;
    
  private:
#if defined(DISPLAY_DRIVER_ST7789)
    ST7789 screen;
#elif defined(DISPLAY_DRIVER_ST7735)
    ST7735 screen;
#elif defined(DISPLAY_DRIVER_ILI9341)
    ili9341 screen;
#endif

  public:
    PicoDisplay(uint16_t *buf);
    PicoDisplay(uint16_t *buf, int width, int height);

    void init();
    void update();
    void set_backlight(uint8_t brightness);
    bool is_pressed(uint8_t button);
    void showBattery();
  };

}
