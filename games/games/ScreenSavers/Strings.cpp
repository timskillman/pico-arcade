#include "Strings.h"
#include "../../Common/Common.h"

static int32_t w = PicoDisplay::WIDTH;
static int32_t h = PicoDisplay::HEIGHT;

void Strings::init(PicoDisplay& pico_display)
{
    
    for (int i=0; i<MAX_BEZIERS; i++) {
	uint8_t r = fast_rand(200), g = fast_rand(200),b = fast_rand(200);
	uint32_t col = pico_display.create_pen(55+r,55+g,55+b);
	beziers[i] = Bezier(Point(fast_rand(w), fast_rand(h)), Point(fast_rand(w), fast_rand(h)), Point(fast_rand(w), fast_rand(h)), Point(fast_rand(w), fast_rand(h)), col);
	beziers[i].pm1 = Point((fast_rand(1024)-512)<<10, (fast_rand(1024)-512)<<10);
	beziers[i].pm2 = Point((fast_rand(1024)-512)<<10, (fast_rand(1024)-512)<<10);
	beziers[i].pm3 = Point((fast_rand(1024)-512)<<10, (fast_rand(1024)-512)<<10);
	beziers[i].pm4 = Point((fast_rand(1024)-512)<<10, (fast_rand(1024)-512)<<10);
    }
}

void Strings::update(PicoDisplay& pico_display)
{
    pico_display.set_pen(0, 0, 20);
    pico_display.clear();
    
    for (int i=0; i<MAX_BEZIERS; i++) {
	beziers[i].draw(pico_display);
	beziers[i].animate();
    }
}

