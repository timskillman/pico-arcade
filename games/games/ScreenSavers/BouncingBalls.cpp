
#include "BouncingBalls.h"
#include "../../Common/Common.h"

const int MAX_BALLS = 20;
static int32_t w = PicoDisplay::WIDTH;
static int32_t h = PicoDisplay::HEIGHT;
static int32_t hw = w / 2;
static int32_t hh = h / 2;


Ball::Ball(int32_t radius) : radius(radius)
{
	
}

void Ball::set(PicoDisplay& pico_display, int32_t _radius, Point _pos, Point _velocity, uint32_t _col)
{
	radius = _radius;
	pos = Point(_pos.x*65536, _pos.y*65536);
	velocity = _velocity;
	uint8_t r = _col & 255, g = (_col>>8) & 255,b = (_col>>16) & 255;
	col = pico_display.create_pen(55+r,55+g,55+b);
	dcol = pico_display.create_pen(55+r/2,55+g/2,55+b/2);
	gravity = 0;
	fixed = true;
}

void Ball::draw(PicoDisplay& pico_display)
{
	int32_t rr = radius / 3; //, r5 = radius / 2;
	pico_display.set_pen(dcol);
	pico_display.circle(Point(pos.x >> 16, pos.y >> 16),radius);
	
	pico_display.set_pen(255,255,255);
	pico_display.circle(Point((pos.x >> 16) - rr, (pos.y >> 16) - rr),rr);
}

bool BouncingBalls::ballOverlap(Ball& ball1, Ball& ball2) {
	int32_t distance = distanceSquared(ball1.pos, ball2.pos);
	int32_t radiusSum = ball1.radius + ball2.radius;
	return distance <= radiusSum * radiusSum;
}

int32_t BouncingBalls::distanceSquared(Point &a, Point &b) {
	int32_t distX = (a.x - b.x)>>16;
	int32_t distY = (a.y - b.y)>>16;
	return distX * distX + distY * distY;
}

void BouncingBalls::init(PicoDisplay &pico_display)
{
	fast_rand_seed(45343);
	balls.resize(MAX_BALLS);
	
	balls[0].set(pico_display, 20,Point(hw,hh),Point(0,0),0x0000ff);
	balls[1].set(pico_display, 10,Point(20,20),Point(0,0),0xff0000);
	balls[2].set(pico_display, 10,Point(w-20,hh),Point(0,0),0xff00ff);
	balls[3].set(pico_display, 15,Point(40,h-20),Point(0,0),0xff00ff);
	balls[4].set(pico_display, 15,Point(w-40,h-20),Point(0,0),0xff00ff);
	
	for (int i=5; i<MAX_BALLS; i++) {
		balls[i].set(pico_display, fast_rand(8)+5, Point(fast_rand(w), fast_rand(h)), Point(0, 0), 0x00ffff);
		/*balls[i] = Ball(fast_rand(8)+5);
		balls[i].setPosition(fast_rand(w)*65536, fast_rand(h)*65536);
		balls[i].setVelocity((fast_rand(256)-128)*1024, (fast_rand(256)-128)*1024);
		balls[i].setVelocity(0, 0);
		uint8_t r = fast_rand(200), g = fast_rand(200),b = fast_rand(200);
		balls[i].col = pico_display.create_pen(55+r,55+g,55+b);
		balls[i].dcol = pico_display.create_pen(55+r/2,55+g/2,55+b/2);
		balls[i].gravity = 0;
		balls[i].fixed = true;
		*/
		balls[i].fixed = false;
	}
	balls[MAX_BALLS-1].fixed = false;
	balls[MAX_BALLS-1].setVelocity((fast_rand(256)-128)*1024, (fast_rand(256)-128)*1024);
}

void BouncingBalls::update(PicoDisplay &pico_display)
{
	
	pico_display.set_pen(0, 20, 0);
	pico_display.clear();

	bool hit = false;
	balls[0].draw(pico_display);
    for (int a=1; a<balls.size(); a++) {
	  
	 Ball& ball1 = balls[a];
	 
	 ball1.draw(pico_display);
	 
	 //Check ball collisions ...
	 for (int b=0; b<a; b++) {
		 
		if (b!=a) {
			
		Ball& ball2 = balls[b];
		
		if (ballOverlap(ball1, ball2)) {
			
			int32_t distance = distanceSquared(ball1.pos, ball2.pos); //16fp
			float distanceSquared = sqrtf((float)distance);
						
			float overlap = (distanceSquared - (float)ball1.radius - (float)ball2.radius) / 2.f;

			distanceSquared = (distanceSquared==0) ? 0.0001f : 1.f / distanceSquared;

			int32_t ballDistX = ball1.pos.x - ball2.pos.x; //16fp
			int32_t ballDistY = ball1.pos.y - ball2.pos.y; //16fp

			int32_t moveX = (int32_t)(((overlap * (float)ballDistX) * distanceSquared))*2;
			int32_t moveY = (int32_t)(((overlap * (float)ballDistY) * distanceSquared))*2;
			
			if (!ball1.fixed) ball1.pos = Point(ball1.pos.x - moveX, ball1.pos.y - moveY);
			if (!ball2.fixed) ball2.pos = Point(ball2.pos.x + moveX, ball2.pos.y + moveY);

			float normalx = (float)(ballDistX * distanceSquared) / 65000.f; //65536.f;
			float normaly = (float)(ballDistY * distanceSquared) / 65000.f; //65536.f;
			float tangentx = -normaly;
			float tangenty = normalx;

			int32_t dotProductTangent1 = ball1.velocity.x * tangentx + ball1.velocity.y * tangenty;
			int32_t dotProductTangent2 = ball2.velocity.x * tangentx + ball2.velocity.y * tangenty;

			int32_t dotProductNormal1 = ball1.velocity.x * normalx + ball1.velocity.y * normaly;
			int32_t dotProductNormal2 = ball2.velocity.x * normalx + ball2.velocity.y * normaly;

			//float m1 = (dotProductNormal1 * (ball1.mass - ball2.mass) + 2.0f * ball1.mass * dotProductNormal2) / (ball1.mass + ball2.mass);
			//float m2 = (dotProductNormal2 * (ball2.mass - ball1.mass) + 2.0f * ball1.mass * dotProductNormal1) / (ball1.mass + ball2.mass);
			
			if (!ball1.fixed) {
				ball1.setVelocity(tangentx * dotProductTangent1 + normalx*dotProductNormal2, tangenty * dotProductTangent1 + normaly*dotProductNormal2);
				ball1.gravity = 0;
			}
			if (!ball2.fixed) {
				ball2.setVelocity(tangentx * dotProductTangent2 + normalx*dotProductNormal1, tangenty * dotProductTangent2 + normaly*dotProductNormal1);
				ball2.gravity = 0;	
			}	
		}
	  }
	}
  }
  
  //Check borders ...
  for (int i=0; i<balls.size(); i++) {
	  if (!balls[i].fixed) {
		  balls[i].pos = balls[i].pos + balls[i].velocity;
		  balls[i].pos.y = balls[i].pos.y + balls[i].gravity;
		  balls[i].gravity += 3000;
		  balls[i].velocity = balls[i].velocity * 0.999f;
		  if (balls[i].pos.x<0 || balls[i].pos.x>(w<<16)) { balls[i].velocity.x = -balls[i].velocity.x; balls[i].pos.x += balls[i].velocity.x; }
		  if (balls[i].pos.y<0 || balls[i].pos.y>(h<<16)) { 
			  balls[i].velocity.y = -balls[i].velocity.y;
			  balls[i].velocity.x = (fast_rand(256)-128)*1024;
			  if (balls[i].pos.y>(h<<16)) balls[i].velocity.y -= 400000;
			  if (balls[i].pos.y<0) balls[i].velocity.y = 4000;
			  balls[i].pos.y += balls[i].velocity.y; 
			  balls[i].gravity = 0;
		  }
	  }
  }
  
}
