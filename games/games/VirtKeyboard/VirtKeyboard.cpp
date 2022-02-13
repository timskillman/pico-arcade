#include "VirtKeyboard.h"

#include <stdlib.h>
#include <math.h>

sing namespace pimoroni;


#define SPEED 1
#define PIXEL_SIZE  2


#if DEMO == 0

static int32_t sinlut[360];
static int32_t coslut[360];

void VirtKeyboard::init(PicoDisplay& pico_display)
{
	green = pico_display.create_pen(0, 255, 0);
}


void VirtKeyboard::update(PicoDisplay& pico_display)
{
	pico_display.set_pen(green)
	for (int i=0; i<4; i++) {
		pico_display.text(kbd[i], Point(hw-100, hh-(60 - i*20), 500, 2);
	}

    //Check controllers
    adc_select_input(1);
    int16_t adx = -((adc_read() >> 9) - 4);
    adc_select_input(0);
    int16_t ady = -((adc_read() >> 9) - 4);
	
    if (abs(adx)>1 || abs(ady)>1) {
	  if ((tx+adx)>0 && (tx+adx)<9) tx += adx;
	  if ((ty+ady)>0 && (ty+ady)<3) ty += ady;
    }
}
