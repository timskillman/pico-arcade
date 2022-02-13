#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ili9341.hpp"

void ili9341::setup(uint16_t _width, uint16_t _height, uint16_t *frame_buffer, spi_inst_t *_spi,
         int8_t _cs, int8_t _dc, int8_t _sck, int8_t _mosi, int8_t _rst, int8_t _bl, bool _invert, bool _rgb)
{
  width = _width;
  height = _height;
  spi = _spi;
  buffer = frame_buffer;
  pin_cs = _cs;
  pin_dc = _dc;
  pin_sck = _sck;
  pin_mosi = _mosi;
  pin_reset = _rst;
  pin_bl = _bl;
  invert = _invert;
  rgb = _rgb;
}

void ili9341::init() 
{
    spi_init(spi, 500 * 1000);
    int baudrate = spi_set_baudrate(spi, 80000 * 1000);

    gpio_set_function(pin_miso, GPIO_FUNC_SPI);
    gpio_set_function(pin_sck, GPIO_FUNC_SPI);
    gpio_set_function(pin_mosi, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(pin_cs);
    gpio_set_dir(pin_cs, GPIO_OUT);
    gpio_put(pin_cs, 0);

    // Reset is active-low
    gpio_init(pin_reset);
    gpio_set_dir(pin_reset, GPIO_OUT);
    gpio_put(pin_reset, 1);

    // high = command, low = data
    gpio_init(pin_dc);
    gpio_set_dir(pin_dc, GPIO_OUT);
    gpio_put(pin_dc, 0);

    // setup PWM for screen brightness
    if(pin_bl != -1) {
      pwm_config cfg = pwm_get_default_config();
      pwm_set_wrap(pwm_gpio_to_slice_num(pin_bl), 65535);
      pwm_init(pwm_gpio_to_slice_num(pin_bl), &cfg, true);
      gpio_set_function(pin_bl, GPIO_FUNC_PWM);
    }
    
    sleep_ms(10);
    gpio_put(pin_reset, 0);
    sleep_ms(10);
    gpio_put(pin_reset, 1);

    command(reg::SWRESET);
    sleep_ms(100);

    command(reg::GAMSET, 1, "\x01");
    command(reg::GMCTRP1, 15, "\x0f\x31\x2b\x0c\x0e\x08\x4e\xf1\x37\x07\x10\x03\x0e\x09\x00");
    command(reg::GMCTRN1, 15, "\x00\x0e\x14\x03\x11\x07\x31\xc1\x48\x08\x0f\x0c\x31\x36\x0f");
    command(reg::COLMOD, 1, "\x55"); //colour format 16bit
    command(reg::FRMCTR1, 2, "\x00\x1e");  //frame rate
    command(reg::SLPOUT);
    command(reg::DISPON);

    if (invert) 
      command(reg::INVON);
    else
      command(reg::INVOFF);
    
    uint8_t madctl = (rgb) ? MADCTL::RGB : 0;
    switch (rotation) {
      case 0: madctl = madctl | MADCTL::SWAP_XY; break;
      case 90: madctl = madctl | MADCTL::ROW_ORDER | MADCTL::COL_ORDER | MADCTL::SWAP_XY; break;
      case 180: madctl = madctl | MADCTL::COL_ORDER; break;
      case 270: madctl = madctl | MADCTL::ROW_ORDER; break;
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

    command(reg::RAMWR);

}

void ili9341::update()
{
    gpio_put(pin_cs, 0); // cs select
    spi_write_blocking(spi, (uint8_t*)buffer, width * height * 2);
    gpio_put(pin_cs, 1); // cs deselect
}

void ili9341::set_backlight(uint8_t brightness)
{
    // gamma correct the provided 0-255 brightness value onto a
    // 0-65535 range for the pwm counter
    float gamma = 2.8;
    uint16_t value = (uint16_t)(pow((float)(brightness) / 255.0f, gamma) * 65535.0f + 0.5f);
    pwm_set_gpio_level(pin_bl, value);
}
    
void ili9341::command(uint8_t command, size_t len, const char *data) 
{
  gpio_put(pin_cs, 0); // cs select
  gpio_put(pin_dc, 0); // command mode
  spi_write_blocking(spi, &command, 1);
  gpio_put(pin_dc, 1); // data mode
  
  if(data) {
    spi_write_blocking(spi, (const uint8_t*)data, len);
  }
  
  gpio_put(pin_cs, 1); // cs deselect
}


