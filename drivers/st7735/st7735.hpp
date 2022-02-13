#pragma once

#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "../screen_registers.h"

//Credits: pimoroni
  
class ST7735 {

  private:
    // screen properties
    uint16_t width;
    uint16_t height;
    uint16_t row_stride;

  public:
    // frame buffer where pixel data is stored
    uint16_t *frame_buffer;

  private:

    //uint32_t dma_channel;

    // set some default values...
    spi_inst_t *spi = spi0;
    int8_t cs     = 17;
    int8_t dc     = 20;
    int8_t sck    = 18;
    int8_t mosi   = 19;
    int8_t miso   = -1;
    int8_t bl     = -1;
    int8_t vsync  = -1; // only available on some pimoroni products
    int8_t rst    = 16;

    uint32_t spi_baud = 30 * 1024 * 1024;

    uint16_t offset_x = 0;
    uint16_t offset_y = 0;
    uint16_t rotation = 0;
    bool invert = false;
    bool rgb = false;

  public:

    ST7735(uint16_t width, uint16_t height, uint16_t *frame_buffer) :
      width(width), height(height), frame_buffer(frame_buffer) {}

    ST7735(uint16_t width, uint16_t height, uint16_t *frame_buffer,
           spi_inst_t *spi,
           int8_t cs, int8_t dc, int8_t sck, int8_t mosi, int8_t rst, int8_t bl = -1) :
      width(width), height(height), frame_buffer(frame_buffer),
      spi(spi), cs(cs), dc(dc), sck(sck), mosi(mosi), rst(rst), bl(bl) {} //, miso(miso)

  public:
    void init();
    void setup(uint16_t width, uint16_t height, uint16_t *frame_buffer, spi_inst_t *spi,
           int8_t cs, int8_t dc, int8_t sck, int8_t mosi, int8_t rst, int8_t bl, bool invert, bool rgb);
    void setOffsets(uint16_t rotate, uint16_t x=0, uint16_t y=0) { rotation = rotate; offset_x = x; offset_y = y; }
    
    void command(uint8_t command, size_t len = 0, const char *data = NULL);
    void update(bool dont_block = false);
    void set_backlight(uint8_t brightness);
    void reset();
};


