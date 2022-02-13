#include "MetaBalls.h"

struct vector2 {
    int16_t x;
    int16_t y;
};

struct ball {
    struct vector2 position;
    struct vector2 velocity;
    uint32_t radius;
    uint16_t color;
};

struct ball balls[16];

static const uint8_t NUM_BALLS = 5;
static const uint8_t MIN_VELOCITY = 3;
static const uint8_t MAX_VELOCITY = 5;
static const uint8_t MIN_RADIUS = 10;
static const uint8_t MAX_RADIUS = 40;
    
void MetaBalls::init(PicoDisplay& pico_display)
{
    /* Set up imaginary balls inside screen coordinates. */
    for (int16_t i = 0; i < NUM_BALLS; i++) {
        balls[i].radius = (rand() % MAX_RADIUS) + MIN_RADIUS;
        balls[i].radius = balls[i].radius * balls[i].radius * 65536; //16 fixed point
        balls[i].color = 0xffff;
        balls[i].position.x = rand() % PicoDisplay::WIDTH;
        balls[i].position.y = rand() % PicoDisplay::HEIGHT;
        balls[i].velocity.x = (rand() % MAX_VELOCITY) + MIN_VELOCITY;
        balls[i].velocity.y = (rand() % MAX_VELOCITY) + MIN_VELOCITY;
    }
}

void MetaBalls::update(PicoDisplay& pico_display)
{
    const Pen background = pico_display.create_pen(255, 0, 0);
    const Pen black = pico_display.create_pen(0, 0, 0);
    const Pen white = pico_display.create_pen(255, 255, 255);
    const Pen green = pico_display.create_pen(0, 255, 0);
    Pen color;

    for (uint16_t y = 0; y < PicoDisplay::HEIGHT; y += 2) {
        for (uint16_t x = 0; x < PicoDisplay::WIDTH; x += 2) {
            int32_t sum = 0;
            for (uint8_t i = 0; i < NUM_BALLS; i++) {
                const int32_t dx = x - balls[i].position.x;
                const int32_t dy = y - balls[i].position.y;
                const int32_t l = dx * dx + dy * dy;
                sum += balls[i].radius / l;
            }

            color = (sum > 0xffff) ? 0xffff: ( ((sum>>3) & 0b1111110000000000) | ((sum>>13) & 0b111111) );  //red | ((sum>>8) & 0b11111100000)
            uint16_t *pp = (uint16_t*)pico_display.ptr(x, y);
            pp[0] = color;
            pp[1] = color;
            pp[PicoDisplay::WIDTH] = color;
            pp[PicoDisplay::WIDTH+1] = color;
        }
    }
    
    for (int16_t i = 0; i < NUM_BALLS; i++) {
        balls[i].position.x += balls[i].velocity.x;
        balls[i].position.y += balls[i].velocity.y;

        /* Touch left or right edge, change direction. */
        if ((balls[i].position.x < 0) | (balls[i].position.x > PicoDisplay::WIDTH)) {
            balls[i].velocity.x = -balls[i].velocity.x;
        }

        /* Touch top or bottom edge, change direction. */
        if ((balls[i].position.y < 0) | (balls[i].position.y > PicoDisplay::HEIGHT)) {
            balls[i].velocity.y = -balls[i].velocity.y;
        }
    }
    
    int hw = PicoDisplay::WIDTH / 2;
    int hh = PicoDisplay::HEIGHT / 2;
    /*
    pico_display.text("Giuseppe,",Point(hw-80,hh-60),500,2);
    pico_display.text("Don't forget to",Point(hw-80,hh-40),500,2);
    pico_display.text("Keep in touch!",Point(hw-80,hh-20),500,2);
    pico_display.text("timskillman@gmail.com",Point(hw-80,hh),500,2);
    pico_display.text("+44 7922 800336",Point(hw-80,hh+20),500,2);
    */

}

