#include "Triangles.h"
#include "../../Common/Common.h"

static int32_t w = PicoDisplay::WIDTH;
static int32_t h = PicoDisplay::HEIGHT;

void Triangles::init(PicoDisplay& pico_display)
{
    
    for (int i=0; i<MAX_TRIS; i++) {
	uint8_t r = fast_rand(200), g = fast_rand(200),b = fast_rand(200);
	uint32_t col = pico_display.create_pen(55+r,55+g,55+b);
	tris[i] = Tri(Point(fast_rand(w), fast_rand(h)), Point(fast_rand(w), fast_rand(h)), Point(fast_rand(w), fast_rand(h)), col);
	tris[i].pm1 = Point((fast_rand(1024)-512)<<10, (fast_rand(1024)-512)<<10);
	tris[i].pm2 = Point((fast_rand(1024)-512)<<10, (fast_rand(1024)-512)<<10);
	tris[i].pm3 = Point((fast_rand(1024)-512)<<10, (fast_rand(1024)-512)<<10);
    }
}

void Triangles::update(PicoDisplay& pico_display)
{
    pico_display.set_pen(0, 0, 20);
    pico_display.clear();
    
    for (int i=0; i<MAX_TRIS; i++) {
	tris[i].draw(pico_display);
	tris[i].animate();
    }
}

