#include "Missiles.h"

void Missile::update(PicoDisplay &pico_display)
{
    if (exploding) {
	if (exprad < 180) {
	    currad = sin(exprad * M_PI / 180)*maxrad;
	    pico_display.set_pen((rand() % explode_col.red), (rand() % explode_col.green), (rand() % explode_col.blue));
	    pico_display.circle(endPoint,currad);
	    pico_display.set_pen(0,0,0);
	    pico_display.circle(endPoint,currad*0.7);
	    exprad+=2;
	}
	else
	{
	    kill();
	}
    }
    else
    {
	pico_display.set_pen(trail_col);
	pico_display.line(startPoint, Point(x,y));
	float ox = x, oy = y;
	x=x+dx;
	y=y+dy;
	pico_display.set_pen(255, 255, 128);
	pico_display.line(Point(ox,oy), Point(x,y));
	
	steps--;
	if (steps<0 && !exploding) {
	    exploding = true;
	    exprad = 0;
	}
    }
}

void Missile::fire(Point sp, Point ep, float speed, Pen trailcol, col3 explodecol)
{
    startPoint = sp;
    endPoint = ep;
    
    
    x = startPoint.x;
    y = startPoint.y;
    dx = endPoint.x - startPoint.x;
    dy = endPoint.y - startPoint.y;
    float d = sqrtf(dx*dx + dy*dy);
    
    steps = d / speed;
    dx = dx / steps;
    dy = dy / steps;
    
    exploding = false;
    visible = true;
    maxrad = 5 + rand() % 30;
    exprad = 180;
    
    trail_col = trailcol;
    explode_col = explodecol;
}

bool Missile::hit(Point pos)
{
    if (!exploding) return false;
    Point dist = (pos - endPoint);
    return (dist.x*dist.x + dist.y*dist.y) <= currad*currad;
}

void Missile::kill()
{
    exploding = false;
    visible = false;
    exprad = 180;
}



void Bomb::drop(Point sp, Point ep, float speed, int32_t basebomb, Pen trailcol, col3 explodecol)
{
    fire(sp,ep,speed,trailcol,explodecol);
    base_bomb = basebomb;
}

