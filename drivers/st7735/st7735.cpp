#include "st7735.hpp"

#include <cstdlib>
#include <math.h>

#include "hardware/dma.h"
#include "hardware/pwm.h"

//Credits: pimoroni

  void ST7735::setup(uint16_t _width, uint16_t _height, uint16_t *_frame_buffer, spi_inst_t *_spi,
           int8_t _cs, int8_t _dc, int8_t _sck, int8_t _mosi, int8_t _rst, int8_t _bl, bool _invert, bool _rgb)
  {
    width = _width;
    height = _height;
    frame_buffer = _frame_buffer;
    spi = _spi;
    cs = _cs;
    dc = _dc;
    sck = _sck;
    mosi = _mosi;
    rst = _rst;
    bl = _bl;
    invert = _invert;
    rgb = _rgb;
  }
  
  void ST7735::init() {
    // configure spi interface and pins
    spi_init(spi, spi_baud);

    gpio_set_function(dc, GPIO_FUNC_SIO);
    gpio_set_dir(dc, GPIO_OUT);

    gpio_set_function(cs, GPIO_FUNC_SIO);
    gpio_set_dir(cs, GPIO_OUT);
    
    gpio_set_function(rst, GPIO_FUNC_SIO);
    gpio_set_dir(rst, GPIO_OUT);

    gpio_set_function(sck,  GPIO_FUNC_SPI);
    gpio_set_function(mosi, GPIO_FUNC_SPI);

    if(miso != -1) {
      gpio_set_function(miso, GPIO_FUNC_SPI);
    }

    // if supported by the display then the vsync pin is
    // toggled high during vertical blanking period
    if(vsync != -1) {
      gpio_set_function(vsync, GPIO_FUNC_SIO);
      gpio_set_dir(vsync, GPIO_IN);
      gpio_set_pulls(vsync, false, true);
    }

    // if a backlight pin is provided then set it up for
    // pwm control
    if(bl != -1) {
      pwm_config cfg = pwm_get_default_config();
      pwm_set_wrap(pwm_gpio_to_slice_num(bl), 65535);
      pwm_init(pwm_gpio_to_slice_num(bl), &cfg, true);
      gpio_set_function(bl, GPIO_FUNC_PWM);
    }
      
      reset();
      
      command(reg::SWRESET);

      sleep_ms(150);

      command(reg::SLPOUT);

      sleep_ms(500);

      command(reg::FRMCTR1, 3, "\x01\x2c\x2d");              // Rate = fosc/(1x2+40) * (LINE+2C+2D)
      command(reg::FRMCTR2, 3, "\x01\x2c\x2d");              // Rate = fosc/(1x2+40) * (LINE+2C+2D)
      command(reg::FRMCTR3, 6, "\x01\x2c\x2d\x01\x2c\x2d");  // Rate = fosc/(1x2+40) * (LINE+2C+2D)

      sleep_ms(10);
      
      command(reg::INVCTR, 1, "\x07");

      command(reg::PWCTR1, 3, "\xa2\x02\x84");
      command(reg::PWCTR2, 1, "\xc5");
      command(reg::PWCTR3, 2, "\x0a\x00");
      command(reg::PWCTR4, 2, "\x8a\x2a");
      command(reg::PWCTR5, 2, "\x8a\xee");

      command(reg::VMCTR1, 1, "\x0e");

      if (invert) 
        command(reg::INVON);
      else
        command(reg::INVOFF);
        
      command(reg::COLMOD, 1, "\x05"); //16bpp
      
      uint8_t madctl = (rgb) ? MADCTL::RGB : 0;
      switch (rotation) {
        case 0: madctl = madctl | MADCTL::ROW_ORDER |MADCTL::SWAP_XY; break;
        case 90: madctl = madctl | MADCTL::ROW_ORDER | MADCTL::COL_ORDER; break;
        case 180: madctl = madctl | MADCTL::COL_ORDER | MADCTL::SWAP_XY; break;
        case 270: break; //madctl = madctl | MADCTL::ROW_ORDER; break;
      }
      command(reg::MADCTRL, 1, (char *)&madctl);
      uint16_t w = (width+offset_x) - 1;
      uint16_t h = (height+offset_y) - 1;

      char buf[4];
      buf[0] = 0; 
      buf[1] = offset_x;
      buf[2] = w >> 8;
      buf[3] = w & 0xff;
      command(reg::CASET, 4, buf);

      buf[0] = 0; 
      buf[1] = offset_y;
      buf[2] = h >> 8;
      buf[3] = h & 0xff;
      command(reg::RASET, 4, buf);

      command(reg::GMCTRP1, 16, "\x0f\x1a\x0f\x18\x2f\x28\x20\x22\x1f\x1b\x23\x37\x00\x07\x02\x10");
      command(reg::GMCTRN1, 16, "\x0f\x1b\x0f\x17\x33\x2c\x29\x2e\x30\x30\x39\x3f\x00\x07\x03\x10");
      sleep_ms(10);

      command(reg::DISPON);
      sleep_ms(100);
      
      command(reg::NORON);
      sleep_ms(10);
      
      gpio_put(cs, 1);

  }

  void ST7735::reset()
  {
    gpio_put(dc, 0);
    gpio_put(rst, 1);
    sleep_ms(500);
    gpio_put(rst, 0);
    sleep_ms(500);
    gpio_put(rst, 1);
    sleep_ms(500);
  }
  
  void ST7735::command(uint8_t command, size_t len, const char *data) {
    gpio_put(cs, 0);

    gpio_put(dc, 0); // command mode
    spi_write_blocking(spi, &command, 1);

    if(data) {
      gpio_put(dc, 1); // data mode
      spi_write_blocking(spi, (const uint8_t*)data, len);
    }

    gpio_put(cs, 1);
  }

  void ST7735::update(bool dont_block) {
    ST7735::command(reg::RAMWR, width * height * sizeof(uint16_t), (const char*)frame_buffer);
  }

  void ST7735::set_backlight(uint8_t brightness) {
    // gamma correct the provided 0-255 brightness value onto a
    // 0-65535 range for the pwm counter
    float gamma = 2.8;
    uint16_t value = (uint16_t)(pow((float)(brightness) / 255.0f, gamma) * 65535.0f + 0.5f);
    pwm_set_gpio_level(bl, value);
  }

