#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "../screen_registers.h"

//Credits:
// Setup based on Shawn Hyam ili9341 driver (https://github.com/shawnhyam/pico)
// Base code from pimoroni

class ili9341 {

    // screen properties
    uint16_t width;
    uint16_t height;
    
    // set some default values...
    spi_inst_t *spi = spi0;
    int8_t pin_cs     = 1;
    int8_t pin_dc     = 0;
    int8_t pin_sck    = 2;
    int8_t pin_mosi   = 3;
    int8_t pin_miso   = -1;
    int8_t pin_bl     = -1;
    int8_t pin_reset  = 4;

    uint16_t offset_x = 0;
    uint16_t offset_y = 0;
    uint16_t rotation = 0;
    bool invert = false;
    bool rgb = false;
    
  public:
    uint16_t *buffer;

  public:
    ili9341(uint16_t width, uint16_t height, uint16_t *frame_buffer) : buffer(frame_buffer), width(width), height(height) {}

    ili9341(uint16_t width, uint16_t height, uint16_t *frame_buffer,
           spi_inst_t * spi,
           int8_t cs, int8_t dc, int8_t sck, int8_t mosi, int8_t reset, int8_t bl, bool invert) : buffer(frame_buffer), width(width), height(height),
                   spi(spi), pin_cs(cs), pin_dc(dc), pin_sck(sck), pin_mosi(mosi), pin_reset(reset), pin_bl(bl) { }

    void setup(uint16_t width, uint16_t height, uint16_t *frame_buffer, spi_inst_t *spi,
           int8_t cs, int8_t dc, int8_t sck, int8_t mosi, int8_t rst, int8_t bl, bool invert, bool rgb);
           
    void setOffsets(uint16_t rotate, uint16_t x=0, uint16_t y=0) { rotation = rotate; offset_x = x; offset_y = y; }
    
    void init();
    void update();
    void set_backlight(uint8_t brightness);
    void command(uint8_t command, size_t len=0, const char *data = NULL);

};
  


