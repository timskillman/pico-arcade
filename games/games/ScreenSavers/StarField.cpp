#include "StarField.h"
#include "../../Common/Common.h"

using namespace pimoroni;

void StarField::init(PicoDisplay& pico_display)
{
    /* Generate look up tables - 16 fixed point. 
    for (uint16_t i = 0; i < 360; i++) {
        sinlut[i] = sin(i * M_PI / 180)*65536;
        coslut[i] = cos(i * M_PI / 180)*65536;
    }
    */
    
    for (int i=0; i<MAX_STARS; i++) {
	stars[i] = star(fast_rand(100)-50, fast_rand(100)-50, fast_rand(255)+1, -fast_rand(10)-3);
    }
}

void StarField::update(PicoDisplay& pico_display)
{
    pico_display.set_pen(0, 0, 20);
    pico_display.clear();
    
    pico_display.set_pen(255,255,255);
    for (int i=0; i<MAX_STARS; i++) {
	Point sp = stars[i].calcPoint();
	if (sp.x>0 && sp.x<w && sp.y>0 && sp.y<h) {
	    pico_display.line(stars[i].oldpos,sp);
	    stars[i].oldpos = sp;
	}
	
	stars[i].z += stars[i].zspeed;
	if (stars[i].z < 1) {
	    stars[i] = star(fast_rand(100)-50, fast_rand(100)-50, fast_rand(255)+1, -fast_rand(10)-3);
	}
    }
}

