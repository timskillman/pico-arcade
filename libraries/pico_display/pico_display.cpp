#include <math.h>
#include <string.h>

#include "hardware/gpio.h"  // Workaround SDK bug - https://github.com/raspberrypi/pico-sdk/issues/3
#include "hardware/pwm.h"
#include "hardware/adc.h"

#include "pico_display.hpp"

const uint8_t BATT_PIN = 29;
#define BATT_LOW 3.0f  //Battery low needs to take into account the screen power as well
#define BATT_HIGH 4.2f
#define BATT_WIDTH 20

namespace pimoroni {

  PicoDisplay::PicoDisplay(uint16_t *buf)
    : PicoGraphics(WIDTH, HEIGHT, buf), screen(WIDTH, HEIGHT, buf)  {
      __fb = buf;
  }

  PicoDisplay::PicoDisplay(uint16_t *buf, int width, int height)
    : PicoGraphics(width, height, buf), screen(width, height, buf)  {
      __fb = buf;
  }

  void PicoDisplay::init() {

    // setup button inputs
    gpio_set_function(A, GPIO_FUNC_SIO); gpio_set_dir(A, GPIO_IN); gpio_pull_up(A);
    gpio_set_function(B, GPIO_FUNC_SIO); gpio_set_dir(B, GPIO_IN); gpio_pull_up(B);
    gpio_set_function(X, GPIO_FUNC_SIO); gpio_set_dir(X, GPIO_IN); gpio_pull_up(X);
    gpio_set_function(Y, GPIO_FUNC_SIO); gpio_set_dir(Y, GPIO_IN); gpio_pull_up(Y);

    // setup 5 way mini joystick
    gpio_set_function(L, GPIO_FUNC_SIO); gpio_set_dir(L, GPIO_IN); gpio_pull_up(L);
    gpio_set_function(R, GPIO_FUNC_SIO); gpio_set_dir(R, GPIO_IN); gpio_pull_up(R);
    gpio_set_function(U, GPIO_FUNC_SIO); gpio_set_dir(U, GPIO_IN); gpio_pull_up(U);
    gpio_set_function(D, GPIO_FUNC_SIO); gpio_set_dir(D, GPIO_IN); gpio_pull_up(D);
    gpio_set_function(F, GPIO_FUNC_SIO); gpio_set_dir(F, GPIO_IN); gpio_pull_up(F);
    
    //Detect battery level (Pimoroni Lipo Pico)
    adc_init();
    adc_gpio_init(BATT_PIN);
    adc_gpio_init(26);
    adc_gpio_init(27);
    
    // initialise the screen
    screen.setup(WIDTH, HEIGHT, __fb, spi, CS, DC, SCK, MOSI, RST, BL, INVERT==1, RGB==1); 
    screen.setOffsets(ROTATION, OFFSET_X, OFFSET_Y);
    screen.init();
  }

  void PicoDisplay::update() {
    screen.update();
  }

  void PicoDisplay::showBattery()
  {
      if (gpio_get(USBIN)) {
          set_pen(255,255,10);
          rectangle(Rect(WIDTH-BATT_WIDTH-8,1,3,7));
          rectangle(Rect(WIDTH-BATT_WIDTH-10,3,2,3));
          rectangle(Rect(WIDTH-BATT_WIDTH-5,1,2,2));
          rectangle(Rect(WIDTH-BATT_WIDTH-5,6,2,2));
      }

      float conversion_factor = 3 * 3.3f / (1 << 12);
      adc_select_input(3);
      int32_t bb = BATT_WIDTH - 4;
      
      float adc_voltage = adc_read() * conversion_factor;
      int32_t batbar = bb * (adc_voltage - BATT_LOW) / (BATT_HIGH - BATT_LOW);
      if (batbar>bb) batbar=bb;
      bool lowbatt = batbar < (bb*0.2f);
      
      if (lowbatt) set_pen(255,0,0); else set_pen(255,255,255);
      rectangle(Rect(WIDTH-BATT_WIDTH,0,BATT_WIDTH,9));
      rectangle(Rect(WIDTH-BATT_WIDTH-2,3,2,3));
      
      if (batbar<bb) {
        set_pen(10,80,10);
        rectangle(Rect(WIDTH-BATT_WIDTH+2,2,bb-batbar,5));
      }
      if (lowbatt) set_pen(255,0,0); else set_pen(10,255,10);
      rectangle(Rect(WIDTH-BATT_WIDTH+2+bb-batbar,2,batbar,5));
      //std::string st = std::to_string(adc_voltage);
      //text("BATT:"+st,Point(15,0),0,2);
  }
  
  void PicoDisplay::set_backlight(uint8_t brightness) {
    screen.set_backlight(brightness);
  }

  bool PicoDisplay::is_pressed(uint8_t button) {
    return !gpio_get(button);
  }
  
}
