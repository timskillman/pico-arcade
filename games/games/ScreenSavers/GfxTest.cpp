#include "GfxTest.h"
#include "../../Common/Common.h"

static int32_t w = PicoDisplay::WIDTH;
static int32_t h = PicoDisplay::HEIGHT;

void GfxTest::init(PicoDisplay& pico_display)
{
	skycols.clear();
	skycols.push_back(Colour(255,255,0));
	skycols.push_back(Colour(255,128,0));
	skycols.push_back(Colour(255,0,0));
	
	greycols.clear();
	greycols.push_back(Colour(255,255,255));
	greycols.push_back(Colour(40,40,40));
	
	greycols2.clear();
	greycols2.push_back(Colour(40,40,40));
	greycols2.push_back(Colour(255,255,255));
	
	meas_cols.push_back(Colour(255,255,255));
	meas_cols.push_back(Colour(200,200,150));
	meas_cols.push_back(Colour(150,150,130));
	meas_cols.push_back(Colour(150,150,130));
	meas_cols.push_back(Colour(130,130,120));
	
	blue_cols.push_back(Colour(80,255,255));
	blue_cols.push_back(Colour(40,128,200));
	blue_cols.push_back(Colour(20,40,120));
	blue_cols.push_back(Colour(10,30,100));
	blue_cols.push_back(Colour(80,255,255));
}

void GfxTest::update(PicoDisplay& pico_display)
{
    pico_display.set_pen(20, 20, 20);
    pico_display.clear();
    
    //pico_display.gradientRect(Rect(0,0,w,h),skycols);
    
    //pico_display.gradientCircle(Point(w/2,h/2),h/2,skycols);

    int32_t v=fastcos(r);
    draw_sevensegs(pico_display, Point(20,0),v,5,3,pico_display.create_pen(255,0,0),pico_display.create_pen(20,0,0));
    
    draw_meter(pico_display, Rect(10,40,w-20,190), Point(w/2-10,180), 170, (v>>9)+128, -50, 51, 2, 5, 3, 10, 10, pico_display.create_pen(180,0,0), meas_cols);
    pico_display.gradientCircle(Point(w/2,220),20,greycols,2<<16);
    pico_display.gradientCircle(Point(w/2,220),15,greycols2);
    
    pico_display.triangleCol(Point(100,0),Point(0,50),Point(50,150),0xff0000,0xff00,0xff);
    //draw_measurements(pico_display, Rect(0,170,w,70), (v>>11)+32, 100, 5, 20, 2, 10, 35, pico_display.create_pen(255,255,0), blue_cols);
    r+=2;
}
