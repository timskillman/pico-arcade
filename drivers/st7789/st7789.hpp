#pragma once

#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "../screen_registers.h"

//Credits: pimoroni

  class ST7789 {

    //uint32_t dma_channel;

    // screen properties
    uint16_t width;
    uint16_t height;

    // set some default values...
    spi_inst_t *spi = spi0;
    int8_t cs     = 17;
    int8_t dc     = 20;
    int8_t sck    = 18;
    int8_t mosi   = 19;
    int8_t miso   = -1;
    int8_t bl     = -1;
    int8_t rst    = 16;
    int8_t vsync  = -1; // only available on some pimoroni products

    uint16_t offset_x = 0;
    uint16_t offset_y = 0;
    uint16_t rotation = 0;
    bool invert = false;
    bool rgb = false;
    
    uint32_t spi_baud = 50 * 1024 * 1024;

  public:
    uint16_t *frame_buffer;

  public:
    ST7789(uint16_t width, uint16_t height, uint16_t *frame_buffer) :
      width(width), height(height), frame_buffer(frame_buffer) {}

    ST7789(uint16_t width, uint16_t height, uint16_t *frame_buffer,
           spi_inst_t *spi,
           int8_t cs, int8_t dc, int8_t sck, int8_t mosi, int8_t rst, int8_t bl = -1) :
      spi(spi),
      width(width), height(height),      
      cs(cs), dc(dc), sck(sck), mosi(mosi), rst(rst), bl(bl), frame_buffer(frame_buffer) {} //miso(miso), 

    void init(bool auto_init_sequence = true, bool round = false);
    void setup(uint16_t width, uint16_t height, uint16_t *frame_buffer, spi_inst_t *spi,
           int8_t cs, int8_t dc, int8_t sck, int8_t mosi, int8_t rst, int8_t bl, bool invert, bool rgb);
    void setOffsets(uint16_t rotate, uint16_t x=0, uint16_t y=0) { rotation = rotate; offset_x = x; offset_y = y; }
    void command(uint8_t command, size_t len = 0, const char *data = NULL);
    void vsync_callback(gpio_irq_callback_t callback);
    void update(bool dont_block = false);
    void set_backlight(uint8_t brightness);
    void reset();
  };

