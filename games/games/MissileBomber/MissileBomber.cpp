#include "MissileBomber.h"
#include <math.h>
#include "../../Common/Common.h"

#define fireRate 300000

void MissileBomber::init(PicoDisplay &pico_display)
{
    cross = Point(PicoDisplay::WIDTH / 2, PicoDisplay::HEIGHT / 2);
    int32_t firetime = GetTicks() + fireRate;
    missile_pen = pico_display.create_pen(0,0,128);
    bomb_pen = pico_display.create_pen(128,0,0);
    bombLevel = 5;
    //initBombs(bombLevel);
    
    base_dist = PicoDisplay::WIDTH / 11;
    for (int i=0; i<9; i++) {
	base_lives[i]=3;
	total_lives+=base_lives[i];
    }
    score = 0;
    wave = 1;
    wave_time = GetTicks() + 3000000;
    
    skycols.clear();
    skycols.push_back(Colour(128,128,255));
    skycols.push_back(Colour(0,0,255));
    skycols.push_back(Colour(0,0,128));
    skycols.push_back(Colour(255,128,0));
    skycols.push_back(Colour(255,0,0));
}

void MissileBomber::drawCross(PicoDisplay &pico_display, Point cross)
{
    pico_display.set_pen(0, 255, 255);
    pico_display.line(Point(cross.x-5, cross.y), Point(cross.x+5, cross.y));
    pico_display.line(Point(cross.x, cross.y-5), Point(cross.x, cross.y+5));
}

void MissileBomber::update(PicoDisplay &pico_display)
{
    
    //pico_display.set_pen(0, 0, 80);
    //pico_display.clear();

    pico_display.gradientRect(Rect(0,0,w,h),skycols);
    
    //Check controllers
    adc_select_input(1);
    int16_t adx = -((adc_read() >> 9) - 4);
    adc_select_input(0);
    int16_t ady = -((adc_read() >> 9) - 4);
	
    if (abs(adx)>1 || abs(ady)>1) {
	if ((cross.x+adx)>0 && (cross.x+adx)<w) cross.x += adx;
	if ((cross.y+ady)>0 && (cross.y+ady)<(h-50)) cross.y += ady;
    }
    
    drawCross(pico_display, cross);
    
    if (total_lives>0 && pico_display.is_pressed(pico_display.X) && GetTicks()>firetime) {
	if (!missiles[currentMissile].Visible()) {
	    int32_t base_fire_from = rand() % 3;
	    int32_t bases_alive = 3;
	    while (bases_alive > 0 && base_lives[base_fire_from*4]<1) { 
		base_fire_from=(base_fire_from + 1) % 3;
		bases_alive--; 
	    }
	    if (bases_alive>0) {
		base_fire_from = (base_fire_from*base_dist*4)+base_dist;
		missiles[currentMissile].fire(Point(base_fire_from,h - 40), cross, 5, missile_pen, col3(255,255,255));
	    }
	}
	currentMissile = (currentMissile + 1) % max_Missiles;
	firetime = GetTicks() + fireRate;
    }
    
    for (auto &missile : missiles)
    {
	if (missile.Visible()) {
	    missile.update(pico_display);
	    if (bombCount>0) {
		for (int b=0; b<bombLevel; b++)
		{
		    auto &bomb = bombs[b];
		    if (bomb.Visible() && missile.hit(bomb.getPosition())) {
			score=score+bomb.maxrad * 10;
			bomb.kill();
			bombCount--;
			missile.kill();
		    }
		}
	    }
	}
    }
    
    
    for (int32_t i =0; i<9; i++) {
	if (i==0 || i == 4 || i == 8) pico_display.set_pen(200,220,255); else pico_display.set_pen(255,255,0);
	pico_display.rectangle(Rect((i+1)*base_dist,h-20-base_lives[i]*8,base_dist / 2,base_lives[i]*8));
    }

    
    if (bombCount > 0) {
	for (int i=0; i<bombLevel; i++)
	{
	    auto &bomb = bombs[i];
	    if (bomb.Visible()) {
		bomb.update(pico_display);
		if (!bomb.Visible()) {
		    if (base_lives[bomb.getBombBase()] > 0) {
			total_lives--;
			base_lives[bomb.getBombBase()]--;
		    }
		    bomb.kill();
		    bombCount--;
		}
	    }
	}
	wave_time = GetTicks() + 3000000;
    } 
    else 
    {
	if (total_lives <= 0)
	{
	    pico_display.set_pen(255, 255, 255);
	    pico_display.text("GAME OVER!",Point(hw-65,hh-30),500,3);
	    pico_display.text("Press B to continue",Point(hw-80,hh),500,2);
	    if (highscore < score) {
	      pico_display.set_pen(255, 255, 0);
	      pico_display.text("YOU GOT THE",Point(hw-45,hh+30),500,2);
	      pico_display.text("HIGH SCORE!",Point(hw-43,hh+50),500,2);
	      pico_display.text("WELL DONE!!",Point(hw-41,hh+70),500,2);
	    } 
	    if (pico_display.is_pressed(pico_display.B)) {
		while (!(pico_display.is_pressed(pico_display.B)));
		if (highscore < score) highscore = score;
		init(pico_display);
	    }
	}
	else
	{
	    
	    if (wave>1) { 
		pico_display.set_pen(rand() % 255, rand() % 255, rand() % 255);
		std::string wstr = std::to_string(wave);
		pico_display.text("WAVE "+wstr,Point(hw-32-wstr.length()*5,hh-25),500,3);
		pico_display.text("GET READY!",Point(hw-40,hh),500,2);
	    }
	    else
	    {
		pico_display.set_pen(255,255,255);
		pico_display.text("MISSILE DEFENDER",Point(hw-110,hh-25),500,3);
		pico_display.text("Press A to START",Point(hw-70,hh),500,2);
	    }
	    if ((wave>1 && GetTicks() > wave_time) || (pico_display.is_pressed(pico_display.A)))
	    {
		if (pico_display.is_pressed(pico_display.A)) {
		    while (!(pico_display.is_pressed(pico_display.A)));
		}
		bombLevel++;
		wave++;
		if (total_lives>0) initBombs(bombLevel);
	    }
	}
    }
    
    //Display land 
    pico_display.set_pen(0,120,0);
    pico_display.rectangle(Rect(0,h-20,w,20));
    
    //Display score
    pico_display.set_pen(255, 255, 255);
    pico_display.text("SCORE:"+std::to_string(score),Point(hw-50,h-15),500,2);
    pico_display.text("HIGH: "+std::to_string(highscore),Point(0,0),500,2);
}

void MissileBomber::initBombs(int32_t bombs)
{
    for (int32_t i=0; i<bombs; i++)
    {
	initBomb(i);
    }
    bombCount = bombs;
}

void MissileBomber::initBomb(int32_t n)
{
    int32_t base_bomb = rand() % 9;
    while (total_lives>0 && base_lives[base_bomb]==0) base_bomb=(base_bomb+1) % 9;
    
    Point start_pos(rand() % PicoDisplay::WIDTH, -(rand() % 80));
    Point attack_base_pos(base_bomb * base_dist + base_dist * 1.25,PicoDisplay::HEIGHT - 30);
    float attack_speed = (float)((rand() % 20)+10) / 20.f;
    bombs[n].drop(start_pos, attack_base_pos, attack_speed, base_bomb, bomb_pen, col3(255,0,0));
}

