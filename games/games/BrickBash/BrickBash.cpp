#include "BrickBash.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"   // stdlib 
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm 
#include "hardware/sync.h" // wait for interrupt 


// Audio PIN is to match some of the design guide shields. 
#define AUDIO_PIN 28  // you can change this to whatever you like

/* 
 * This include brings in static arrays which contain audio samples. 
 * if you want to know how to make these please see the python code
 * for converting audio samples into static arrays. 
 */
#include "thats_cool.h"
int wav_position = 0;

/*
 * PWM Interrupt Handler which outputs PWM level and advances the 
 * current sample. 
 * 
 * We repeat the same value for 8 cycles this means sample rate etc
 * adjust by factor of 8   (this is what bitshifting <<3 is doing)
 * 
 */
void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));    
    if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
        // set pwm level 
        // allow the pwm value to repeat for 8 cycles this is >>3 
        pwm_set_gpio_level(AUDIO_PIN, WAV_DATA[wav_position>>3]);  
        wav_position++;
    } else {
        // reset to start
        wav_position = 0;
    }
}

void BrickBash::init(PicoDisplay &pico_display)
{

  ball.x = (rand() % 200)<<16;
  ball.y = (rand() % 120)<<16;
  ball.dx = (1<<15);
  ball.dy = (int32_t)(-1.0f * 65536.f);
  ball.r = 5;
  
  bat.r = 30;
  bat.x = hw - bat.r/2;
  bat.y = h - 22;
  btx = (float)bat.x;
  
  bricks[0][0] = 0b0011111101111110;
  bricks[0][1] = 0b0111111010111111;
  bricks[0][2] = 0b0110110101011011;
  bricks[0][3] = 0b0111001010100111;
  bricks[0][4] = 0b0111110101011111;
  bricks[0][5] = 0b0111111010111111;
  bricks[0][6] = 0b0000000000000000; //0b0011110101011110;
  bricks[0][7] = 0b0000000000000000;
  
  bricks[1][0] = 0b0011111111111110;
  bricks[1][1] = 0b0111001001001111;
  bricks[1][2] = 0b0111110000111111;
  bricks[1][3] = 0b0111001111001111;
  bricks[1][4] = 0b0110010110100111;
  bricks[1][5] = 0b0111001111001111;
  bricks[1][6] = 0b0111110000111111;
  bricks[1][7] = 0b0011111111111110;
  
  bricks[2][0] = 0b0011111111111110;
  bricks[2][1] = 0b0111001111100111;
  bricks[2][2] = 0b0110110111011011;
  bricks[2][3] = 0b0111001111100111;
  bricks[2][4] = 0b0111111111111111;
  bricks[2][5] = 0b0111001111100111;
  bricks[2][6] = 0b0111110000011111;
  bricks[2][7] = 0b0011111111111110;
  
  bricks[3][0] = 0b0111111111111111;
  bricks[3][1] = 0b0111111111111111;
  bricks[3][2] = 0b0111111111111111;
  bricks[3][3] = 0b0111111111111111;
  bricks[3][4] = 0b0111111111111111;
  bricks[3][5] = 0b0111111111111111;
  bricks[3][6] = 0b0111111111111111;
  bricks[3][7] = 0b0111111111111111;
  
  bricks[4][0] = 0b0111111000111111;
  bricks[4][1] = 0b0111111000111111;
  bricks[4][2] = 0b0111000000000111;
  bricks[4][3] = 0b0111000000000111;
  bricks[4][4] = 0b0111111000111111;
  bricks[4][5] = 0b0111111000111111;
  bricks[4][6] = 0b0111111000111111;
  bricks[4][7] = 0b0111111111111111;

  bricks[5][0] = 0b0011101111111000;
  bricks[5][1] = 0b0101011111100111;
  bricks[5][2] = 0b0110111111011111;
  bricks[5][3] = 0b0111001110111111;
  bricks[5][4] = 0b0111110001111111;
  bricks[5][5] = 0b0111101110011111;
  bricks[5][6] = 0b0111011111100111;
  bricks[5][7] = 0b0111101111111000;

  bricks[6][0] = 0b0001111111111100;
  bricks[6][1] = 0b0011001111100110;
  bricks[6][2] = 0b0110110111011011;
  bricks[6][3] = 0b0111001111100111;
  bricks[6][4] = 0b0111110000011111;
  bricks[6][5] = 0b0111001111100111;
  bricks[6][6] = 0b0011110000011110;
  bricks[6][7] = 0b0001111111111100;
  
  bricks[7][0] = 0b0111111111111111;
  bricks[7][1] = 0b0101010101010101;
  bricks[7][2] = 0b0110101010101011;
  bricks[7][3] = 0b0101010101010101;
  bricks[7][4] = 0b0110101010101011;
  bricks[7][5] = 0b0101010101010101;
  bricks[7][6] = 0b0110101010101011;
  bricks[7][7] = 0b0111111111111111;
  
  brick_cols[0] = pico_display.create_pen(255, 0, 0);
  brick_cols[1] = pico_display.create_pen(255, 128, 0);
  brick_cols[2] = pico_display.create_pen(255, 255, 0);
  brick_cols[3] = pico_display.create_pen(0, 255, 0);
  brick_cols[4] = pico_display.create_pen(0, 128, 255);
  brick_cols[5] = pico_display.create_pen(0, 0, 255);
  brick_cols[6] = pico_display.create_pen(128, 0, 255);
  brick_cols[7] = pico_display.create_pen(255, 0, 128);
  
  bat_cols[0] = pico_display.create_pen(0, 0, 255);
  bat_cols[1] = pico_display.create_pen(0, 255, 255);
  bat_cols[2] = pico_display.create_pen(255, 255, 255);
  bat_cols[3] = pico_display.create_pen(0, 255, 255);
  bat_cols[4] = pico_display.create_pen(0, 0, 255);
  
  lives = 3;
  score = 0;
  level = 0;
  start = true;
  
  skycols.clear();
  skycols.push_back(Colour(0,0,128));
	skycols.push_back(Colour(0,0,0));
  
/*
    stdio_init_all();
    set_sys_clock_khz(176000, true); 
    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);

    int audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);

    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);
    // set the handle function above
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Setup PWM for audio output
    pwm_config config = pwm_get_default_config();
    /* Base clock 176,000,000 Hz divide by wrap 250 then the clock divider further divides
     * to set the interrupt rate. 
     * 
     * 11 KHz is fine for speech. Phone lines generally sample at 8 KHz
     * 
     * 
     * So clkdiv should be as follows for given sample rate
     *  8.0f for 11 KHz
     *  4.0f for 22 KHz
     *  2.0f for 44 KHz etc
     */
/*
    pwm_config_set_clkdiv(&config, 8.0f); 
    pwm_config_set_wrap(&config, 250); 
    pwm_init(audio_pin_slice, &config, true);

    pwm_set_gpio_level(AUDIO_PIN, 0);

    __wfi();
    */
}

void BrickBash::update(PicoDisplay &pico_display)
{

    draw_game(pico_display);
    
    game_status(pico_display);

    collisions(pico_display);
    
    controllers(pico_display);
    
    //Display score
    pico_display.set_pen(255, 255, 255);
    pico_display.text("SCORE:"+std::to_string(score),Point(hw-50,h-15),0,fontsize);
    pico_display.set_pen(0, 255, 0);
    pico_display.text("HIGHSCORE:"+std::to_string(highscore),Point(0,0),0,fontsize);
     
    //pico_display.text("ACD0:"+std::to_string(adc),Point(hw-80,h-50),0,2);
    //adc_select_input(1);
    //adc = adc_read();
    //pico_display.text("ACD1:"+std::to_string(adc),Point(hw-80,h-30),0,2);
      
}

void BrickBash::draw_game(PicoDisplay &pico_display)
{
  
    	//pico_display.set_pen(0, 0, 0);
	  //pico_display.clear();
  
    pico_display.gradientRect(Rect(0,0,w,h),skycols);
    
    //Draw bat
    for (int i=0; i<5; i++) {
        pico_display.set_pen(bat_cols[i]);
        pico_display.line(Point(bat.x, bat.y + i*2 - 1), Point(bat.x + bat.r, bat.y + i*2 - 1));
    }
    
    //Draw ball
    pico_display.set_pen(128, 255, 128);
    pico_display.circle(Point(ball.x>>16, ball.y>>16), ball.r);
    
    //Draw bricks
    for (int i=0; i<8; i++)
    {
      if (bricks[level][i] != 0) {
        for (int32_t xb=0; xb<16; xb++) {
          if (bricks[level][i] & (1<<xb)) {
            pico_display.set_pen(brick_cols[i]);
            pico_display.rectangle(Rect(brickx + xb*brickWidth, bricky+i*brickHeight, brickWidth-2, brickHeight-1));
          }
        }
      }
    }
}

void BrickBash::game_status(PicoDisplay &pico_display)
{
    //Check game status
    if (lives==0) {
        pico_display.set_pen(255, 255, 255);
        pico_display.text("GAME OVER!",Point(hw-65,hh-30),500,fontsize+1);
        if (highscore < score) {
          pico_display.text("YOU GOT THE",Point(hw-50,hh),500,fontsize);
          pico_display.text("HIGH SCORE!",Point(hw-50,hh+30),500,fontsize);
          pico_display.text("WELL DONE!!",Point(hw-50,hh+60),500,fontsize);
        } 
        else
        {
          pico_display.text("YOU STILL NEED TO",Point(hw-75,hh),500,fontsize);
          pico_display.text("BEAT THE HIGH SCORE",Point(hw-85,hh+30),500,fontsize);
          pico_display.text("OF "+std::to_string(highscore),Point(hw-40,hh+60),500,fontsize);
        }
        
        if (pico_display.is_pressed(pico_display.X)) {
          while (!pico_display.is_pressed(pico_display.X));
          if (highscore < score) highscore = score;
          init(pico_display);
        }
    }
    else
    {
        if (start) {
            if (lives==3 && level==0) {
              pico_display.set_pen(255,255,255);
              pico_display.text("BREAKOUT!",Point(hw-60,hh-25),500,fontsize+1);
              pico_display.text("Press A to START",Point(hw-70,hh),500,fontsize);
              if (pico_display.is_pressed(pico_display.X)) {
                while (!pico_display.is_pressed(pico_display.X));
                start = false;
              }
            } 
            else 
            {
              if (pico_display.is_pressed(pico_display.X)) {
                while (!pico_display.is_pressed(pico_display.X));
                start = false;
              }
            }
            ball.x = (bat.x + 10)<<16;
            ball.y = (bat.y - 4)<<16;
        }
        else 
        {
            ball.x += ball.dx * speed;
            ball.y += ball.dy * speed;
        }
        pico_display.set_pen(128, 255, 128);
        for (int i=0; i<lives-1; i++) {
            pico_display.circle(Point(w-i*10-5, h-10), ball.r);
        }
    }
}

void BrickBash::collisions(PicoDisplay &pico_display)
{
    //Ball hit walls?
    int32_t bx = ball.x >> 16;
    int32_t by = ball.y >> 16;
    if(bx > pico_display.bounds.w || ball.x < 0) ball.dx *= -1;
    if(ball.y < 0) ball.dy *= -1;
    
    //Ball hit ground?
    if (by >= pico_display.bounds.h) {
        if (lives>0) lives--;
        ball.dy *= -1;
        start = true;
    }

    //Ball hit bat?
    if((by >= (bat.y-3) && by <= bat.y) && (bx > bat.x && bx<(bat.x+bat.r))) {
      ball.dy *= -1;
      ball.dx = ((bx-(bat.x+bat.r/2))<<16) / 16;
    }
    
    //Ball hit bricks? ...
    bool finished = true;
    for (int i=0; i<8; i++)
    {
      if (bricks[level][i] != 0) {
        if (by >= bricky+brickHeight*i && by < bricky+brickHeight*(i+1)) {
          int32_t xb = (bx-brickx) / brickWidth; //16
          if (xb>=0 && xb<16) {
            if (bricks[level][i] & (1<<xb)) {
              bricks[level][i] = bricks[level][i] ^ (1<<xb);
              ball.dy *= -1;
              score += (8-i)*20;
            }
          }
        }
        finished = false;
      }
    }
    
    if (finished) {
      level = (level+1) % maxlevels;
      lives++;
      start = true;
    }
}

void BrickBash::controllers(PicoDisplay &pico_display)
{
      //Check controllers
    /*
    adc_select_input(1);
    float adc = (float)adc_read() / 4096.f - 0.5f;
    
    float nx = btx - adc*8.f;
    if (nx > 0 && nx < (PicoDisplay::WIDTH-bat.r)) { btx = nx; bat.x = btx; }
    //bat.x += (adc*2.f); //(PicoDisplay::WIDTH-bat.r) * adc;
    */
    
    
    if(pico_display.is_pressed(pico_display.L) && bat.x < (PicoDisplay::WIDTH-bat.r)) {
      bat.x += speed;
    }
    
    if(pico_display.is_pressed(pico_display.R) && bat.x>0) {
      bat.x -= speed;
    }
}
