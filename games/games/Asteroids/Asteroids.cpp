

#include "Asteroids.h"
#include <math.h>
#include "../../Common/Common.h"

//#include "sound.h"
#include "vectorart.h"
//#include <sstream>


//static int32_t sinlut[360];
//static int32_t coslut[360];


void Asteroids::init(PicoDisplay &pico_display) {

    /* Generate look up tables - 16 fixed point. */
    //for (uint16_t i = 0; i < 360; i++) {
    //    sinlut[i] = sin(i * M_PI / 180)*65536;
    //    coslut[i] = cos(i * M_PI / 180)*65536;
    //}
    
    // Create initial asteroid field
	astroField.create(levelUp);
	
	// Create ship
	myship.create(Point(sw / 2, sh / 2), 3.1415926);
    myship.visible = true;
    
    ufo.create(Point(0,sh/2));
    
	// Create missiles
	for (int i=0; i<maxMissiles; i++) {
		missiles[i] = missile();
		ufomissiles[i] = missile();
	}
	
	int32_t firetime = GetTicks() + fireRate;
    score = 0;
    gameOver = false;
    start = true;
    
	// Load sound effects
	//const char* soundFiles[MAX_SOUNDS] = { "shipfire.raw", "explosion1.raw", "explosion2.raw", "thrust.raw", "explosion3.raw" };
	//loadSoundFiles(soundFiles);

	skycols.clear();
	skycols.push_back(Colour(0,0,0));
	skycols.push_back(Colour(0,0,0));
    skycols.push_back(Colour(0,0,80));
    skycols.push_back(Colour(80,80,80));
    skycols.push_back(Colour(0,0,80));
    skycols.push_back(Colour(0,0,0));
    skycols.push_back(Colour(0,0,0));

}

void Asteroids::update(PicoDisplay &pico_display)
{

	//pico_display.set_pen(0, 0, 0);
	//pico_display.clear();

	pico_display.gradientRect(Rect(0,0,sw,sh),skycols);
	
	//Ship update and controls
	/*
	adc_select_input(1);
	int16_t adc = (adc_read() >> 6) - 32;// / 4095.f - 0.5f;

	if (abs(adc) > 2) {
		if (abs(adc)>10) adc = (adc<0) ? -10 : 10;
		myship.rot -= adc; 
	}
	* */

	
	if (pico_display.is_pressed(pico_display.L)) {
		myship.rot += 5;
	}
	
	if (pico_display.is_pressed(pico_display.R)) {
		myship.rot -= 5;
	}
	
	if (pico_display.is_pressed(pico_display.U)) {
		myship.jet(0.5f);
	}
	myship.update(pico_display);

	//Update asteroid field ...
	astroField.update(pico_display);


	//Missiles update and controls
	if (!start && (pico_display.is_pressed(pico_display.X)) && GetTicks()>firetime && myship.visible) {
		if (!missiles[currentMissile].visible) {
			missiles[currentMissile].fire(myship.pos, myship.rot, myship.radius);
		}
		currentMissile = (currentMissile + 1) % maxMissiles;
		firetime = GetTicks() + fireRate;
	}
	
	//Check if missiles hit asteroids and level up if needed ...
	bool newLevel = false;
	pico_display.set_pen(255,128,128);
	//Loop through all visible missiles ...
	for (uint32_t i = 0; i < maxMissiles; i++) {
		missiles[i].update(pico_display, 2.f);
		//If a missile is visible, check if it hits an asteroid ...
		if (missiles[i].visible && astroField.hit(missiles[i].pos, missiles[i].radius)) {
			// Hit! - missile disappears. If all asteroids have been destroyed, level up
			score += 10;
			missiles[i].visible = false;
			if (astroField.asteroidCount == 0) astroField.create(astroField.asteroidLevel + levelUp);
		}
	}

	if (ufo.update(pico_display) && fast_rand(30)<10) {
		int m=0;
		while (!ufomissiles[m].visible && m < maxMissiles) m++;
		if (m < maxMissiles) {
			//ufomissiles[m].fire(ufo.pos, fast_rand(360), ufo.radius);
		}
	}
	
	//Loop through all visible ufo missiles ...
	for (uint32_t i = 0; i < maxMissiles; i++) {
		//ufomissiles[i].update(pico_display, 2.f);
		//If a missile is visible, check if it hits an asteroid ...
		//if (ufomissiles[i].visible && ufomissiles[i].hit(myship.pos, myship.radius)) {
		//	ufomissiles[i].visible = false;
		//	myship.fieldStrength -= 0.5f;
		//}
	}
	
	//Check if any asteroid hits the ship ...
	if (!gameOver && !start && astroField.hit(myship.pos, myship.radius)) {
		myship.fieldStrength -= 0.1f;
		if (myship.fieldStrength <= 0.f) {
			myship.visible = false;
			//playSound(sndExp3);
			gameOver = true;
		}
	}

	pico_display.set_pen(0,0,255);
	float barwidth = sw / 5;
	float bar = barwidth*myship.fieldStrength;
	pico_display.rectangle(Rect((sw - barwidth) / 2, 10, bar, 5));

	pico_display.set_pen(0,255,255);
	vecart::drawString(pico_display, "SCORE:"+std::to_string(score), Point(10, 10), Point(10, 10)); //+", "+std::to_string(astroField.asteroidCount)
	vecart::drawString(pico_display, "HIGH:"+std::to_string(highscore), Point(10, sh-20), Point(10, 10)); //+", "+std::to_string(astroField.asteroidCount)

	//Show Game Over
	if (gameOver) {
		gocol += goinc; if (gocol > 254 || gocol < 1) { goinc = -goinc; gocol += goinc; }
		pico_display.set_pen(0,gocol,0);
		vecart::drawString(pico_display, "GAME OVER", Point((sw - 270) / 2, (sh - 60) / 2), Point(30, 30));
		vecart::drawString(pico_display, "Press 'B' to start", Point((sw - 160) / 2, (sh + 20) / 2), Point(10, 10));
		if (highscore < score) {
			pico_display.set_pen(255-gocol,0,gocol);
			vecart::drawString(pico_display, "YOU GOT THE HIGH SCORE!!", Point((sw - 270) / 2, (sh + 60) / 2), Point(12, 12));
		}
		if (pico_display.is_pressed(pico_display.B)) {
		   while (!pico_display.is_pressed(pico_display.B));
		   if (highscore < score) highscore = score;
		   init(pico_display);
		}
	}
	else if (start) {
		pico_display.set_pen(255,255,255);
		vecart::drawString(pico_display, "ASTEROIDS", Point((sw - 270) / 2, (sh - 60) / 2), Point(30, 30));
		vecart::drawString(pico_display, "Press 'A' to start", Point((sw - 160) / 2, (sh + 20) / 2), Point(10, 10));
		if (pico_display.is_pressed(pico_display.A)) {
		   while (!pico_display.is_pressed(pico_display.A));
		   start = false;
		}
	}

}



