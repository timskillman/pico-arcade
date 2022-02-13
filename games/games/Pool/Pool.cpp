
#include "Pool.h"
#include "../../Common/Common.h"

const int MAX_BALLS = 100;
static int32_t w = PicoDisplay::WIDTH;
static int32_t h = PicoDisplay::HEIGHT;
static int32_t hw = w / 2;
static int32_t hh = h / 2;


Ball::Ball(int32_t radius) : radius(radius)
{
	
}

void Ball::draw(PicoDisplay& pico_display)
{
	int32_t rr = radius / 3; //, r5 = radius / 2;
	pico_display.set_pen(dcol);
	pico_display.circle(Point(pos.x >> 16, pos.y >> 16),radius);
	
	//pico_display.set_pen(col);
	//pico_display.circle(Point((pos.x >> 16) - rr, (pos.y >> 16) - rr),r5);
	
	pico_display.set_pen(255,255,255);
	pico_display.circle(Point((pos.x >> 16) - rr, (pos.y >> 16) - rr),rr);
}

bool Pool::ballOverlap(Ball& ball1, Ball& ball2) {
	int32_t distance = distanceSquared(ball1.pos, ball2.pos);
	int32_t radiusSum = ball1.radius + ball2.radius;
	return distance <= radiusSum * radiusSum;
}

int32_t Pool::distanceSquared(Point &a, Point &b) {
	int32_t distX = (a.x - b.x)>>16;
	int32_t distY = (a.y - b.y)>>16;
	return distX * distX + distY * distY;
}

void Pool::init(PicoDisplay &pico_display)
{
	fast_srand(45343);
	balls.resize(MAX_BALLS);
	for (int i=0; i<MAX_BALLS; i++) {
		balls[i] = Ball(fast_rand(8)+5);
		balls[i].setPosition(fast_rand(w)*65536, fast_rand(h)*65536);
		balls[i].setVelocity((fast_rand(256)-128)*1024, (fast_rand(256)-128)*1024);
		uint8_t r = fast_rand(200), g = fast_rand(200),b = fast_rand(200);
		balls[i].col = pico_display.create_pen(55+r,55+g,55+b);
		balls[i].dcol = pico_display.create_pen(55+r/2,55+g/2,55+b/2);
	}
}

void Pool::update(PicoDisplay &pico_display)
{
	
	pico_display.set_pen(0, 20, 0);
	pico_display.clear();

	bool hit = false;
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

			int32_t moveX = (int32_t)(((overlap * (float)ballDistX) * distanceSquared));
			int32_t moveY = (int32_t)(((overlap * (float)ballDistY) * distanceSquared));
			
			ball1.pos = Point(ball1.pos.x - moveX, ball1.pos.y - moveY);
			ball2.pos = Point(ball2.pos.x + moveX, ball2.pos.y + moveY);

			float normalx = (float)(ballDistX * distanceSquared) / 65536.f;
			float normaly = (float)(ballDistY * distanceSquared) / 65536.f;
			float tangentx = -normaly;
			float tangenty = normalx;

			int32_t dotProductTangent1 = ball1.velocity.x * tangentx + ball1.velocity.y * tangenty;
			int32_t dotProductTangent2 = ball2.velocity.x * tangentx + ball2.velocity.y * tangenty;

			int32_t dotProductNormal1 = ball1.velocity.x * normalx + ball1.velocity.y * normaly;
			int32_t dotProductNormal2 = ball2.velocity.x * normalx + ball2.velocity.y * normaly;

			//float m1 = (dotProductNormal1 * (ball1.mass - ball2.mass) + 2.0f * ball1.mass * dotProductNormal2) / (ball1.mass + ball2.mass);
			//float m2 = (dotProductNormal2 * (ball2.mass - ball1.mass) + 2.0f * ball1.mass * dotProductNormal1) / (ball1.mass + ball2.mass);
			
			ball1.setVelocity(tangentx * dotProductTangent1 + normalx*dotProductNormal2, tangenty * dotProductTangent1 + normaly*dotProductNormal2);
			ball2.setVelocity(tangentx * dotProductTangent2 + normalx*dotProductNormal1, tangenty * dotProductTangent2 + normaly*dotProductNormal1);		
		}
	  }
	}
  }
  
  //Check borders ...
  for (int i=0; i<balls.size(); i++) {
	  balls[i].pos = balls[i].pos + balls[i].velocity;
	  //balls[i].velocity = balls[i].velocity * 0.999f;
	  if (balls[i].pos.x<0 || balls[i].pos.x>(w<<16)) { balls[i].velocity.x = -balls[i].velocity.x; balls[i].pos.x += balls[i].velocity.x; }
	  if (balls[i].pos.y<0 || balls[i].pos.y>(h<<16)) { balls[i].velocity.y = -balls[i].velocity.y; balls[i].pos.y += balls[i].velocity.y; }
  }
  
}
