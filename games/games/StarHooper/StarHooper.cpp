#include "StarHooper.h"
#include "../../Common/Common.h"

using namespace pimoroni;

void StarHooper::init(PicoDisplay& pico_display)
{
    /* Generate look up tables - 16 fixed point. 
    for (uint16_t i = 0; i < 360; i++) {
        sinlut[i] = sin(i * M_PI / 180)*65536;
        coslut[i] = cos(i * M_PI / 180)*65536;
    }
    */
    
    for (int i=0; i<MAX_STARS; i++) {
	stars[i] = star(fast_rand(100)-50, fast_rand(100)-50, fast_rand(255)+1, -fast_rand(10)-3, 1, Point(0,0));
    }
    
    for (int i=0; i<MAX_ASTEROIDS; i++) {
	asteroids[i] = star(fast_rand(100)-50, fast_rand(100)-50, 200+fast_rand(500), -fast_rand(7)-6, fast_rand(5)+2, Point(0,0));
    }
    
    mpu.init(i2c1, 14, 15);
    
    skycols.clear();
    skycols.push_back(Colour(0,0,0));
    skycols.push_back(Colour(0,0,0));
    skycols.push_back(Colour(0,0,80));
    skycols.push_back(Colour(80,80,80));
    skycols.push_back(Colour(0,0,80));
    skycols.push_back(Colour(0,0,0));
    skycols.push_back(Colour(0,0,0));

}

void StarHooper::update(PicoDisplay& pico_display)
{
    pico_display.gradientRect(Rect(0,0,w,h),skycols);
    //pico_display.set_pen(0, 0, 20);
    //pico_display.clear();
    
    int32_t acceleration[3],gyro[3];
    mpu.read_acc_gyro_smooth(acceleration, gyro, 10);
    
    cx-=acceleration[1];
    if (cx>sw) cx=sw; else if (cx<0) cx=0;

    cy+=acceleration[0];
    if (cy>sh) cy=sh; else if (cy<0) cy=0;
    
    Point xypos = Point(cx>>rs,cy>>rs);
    Point offset = Point((xypos.x-hw)/20,(xypos.y-hh)/20);
    
    pico_display.set_pen(20, 30, 20);
    pico_display.circle(Point(hw+((xypos.x-hw)>>4),hh+((xypos.y-hh)>>4)),50);
    pico_display.set_pen(80, 128, 80);
    pico_display.circle(Point(hw+((xypos.x-hw)>>4),hh+((xypos.y-hh)>>4)),50,12000);

    
    for (int i=0; i<MAX_ASTEROIDS; i++) {
	
	Point sp = asteroids[i].calcPoint(offset);
	
	if (sp.x>0 && sp.x<w && sp.y>0 && sp.y<h && stars[i].z>1) {
	    pico_display.set_pen(40, 40, 40);
	    pico_display.circle(sp,(asteroids[i].size *w) / asteroids[i].z);
	    pico_display.set_pen(120, 120, 120);
	    pico_display.circle(sp,(asteroids[i].size *w) / asteroids[i].z, 12000);
	}
	
	asteroids[i].z += asteroids[i].zspeed;
	if (asteroids[i].z < 1) {
	    asteroids[i] = star(fast_rand(100)-50, fast_rand(100)-50, 200+fast_rand(500), -fast_rand(10)-3, fast_rand(5)+2, offset);
	}
    }
    
    //Draw Stars
    pico_display.set_pen(128,128,128);
    for (int i=0; i<MAX_STARS; i++) {
	Point sp = stars[i].calcPoint(offset);
	
	if (sp.x>0 && sp.x<w && sp.y>0 && sp.y<h && stars[i].z>1) {
	    pico_display.line(stars[i].oldpos,sp);
	    stars[i].oldpos = sp;
	}
	
	stars[i].z += stars[i].zspeed;
	if (stars[i].z < 1) {
	    stars[i] = star(fast_rand(100)-50, fast_rand(100)-50, fast_rand(255)+1, -fast_rand(10)-3, 1, offset);
	}
    }
    
    
    //Draw spaceship
    pico_display.set_pen(128, 128, 128);
    pico_display.triangle(xypos+Point(0,-8),xypos+Point(20,5),xypos+Point(-20,5));
    pico_display.set_pen(255, 255, 128);
    pico_display.triangle(xypos+Point(-9,5),xypos+Point(-12,-1),xypos+Point(-6,-1));
    pico_display.triangle(xypos+Point(9,5),xypos+Point(12,-1),xypos+Point(6,-1));
    
}

