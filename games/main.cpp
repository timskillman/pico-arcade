#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>

#include "pico_display.hpp"

#ifdef GAME_ROLLERBALL
#include "games/RollerBall/RollerBall.h"
#endif

#ifdef GAME_BREAKOUT
#include "games/BrickBash/BrickBash.h"
#endif

#ifdef GAME_SCREENSAVERS
#include "games/ScreenSavers/ScreenSaver.h"
#endif

//#include "games/Fire/Fire.h"
//#include "games/FlappyBird/PicoBird.h"

#ifdef GAME_UPRISING
#include "games/VectorMap/Vector.h"
#endif

#ifdef GAME_MISSILEBOMBER
#include "games/MissileBomber/MissileBomber.h"
#endif

#ifdef GAME_ASTEROIDS
#include "games/Asteroids/Asteroids.h"
#endif

#ifdef GAME_STARHOOPER
#include "games/StarHooper/StarHooper.h"
#endif

//#include "games/Pool/Pool.h"

using namespace pimoroni;

uint16_t buffer[PicoDisplay::WIDTH * PicoDisplay::HEIGHT];
PicoDisplay pico_display(buffer);

uint16_t max_games = 0;

int main(int argc, char* argv[]) {

  //Pi3C pi3c = Pi3C("PicoWindow", 800, 600);
  //pico_display.init(pi3c);
  


  pico_display.init();
  pico_display.set_backlight(255);
  
  uint32_t i = 0;
  std::vector<std::string> Games;
  
#ifdef GAME_ROLLERBALL
  RollerBall rollerball(pico_display);
  Games.push_back("ROLLERBALL");
#endif

#ifdef GAME_BREAKOUT
  BrickBash brickbash(pico_display);
  Games.push_back("BREAKOUT");
#endif
  
  //Fire fire(pico_display);
  //PicoBird picobird(pico_display);
#ifdef GAME_UPRISING
  VectorMap vectormap(pico_display);
  Games.push_back("UPRISING");
#endif

#ifdef GAME_SCREENSAVERS
  ScreenSaver screensaver(pico_display);
  Games.push_back("SCREENSAVERS");
#endif

#ifdef GAME_ASTEROIDS
  Asteroids asteroids(pico_display);
  Games.push_back("ASTEROIDS");
#endif

#ifdef GAME_MISSILEBOMBER
  MissileBomber missile(pico_display);
  Games.push_back("MISSILEBOMBER");
#endif

#ifdef GAME_STARHOOPER
  StarHooper starhooper(pico_display);
  Games.push_back("STARHOOPER");
#endif

  //Pool pool(pico_display);

  uint32_t max_games=Games.size();
  int game = 0; //GAME_ROLLERBALL; //GAME_BRICKBASH;
  
  //while(pi3c.is_running()) {
  while(true) {
    
    //float led_step = fmod(i / 20.0f, M_PI * 2.0f);
    //int r = (sin(led_step) * 25.0f) + 25.0f;
    //pico_display.set_led(r, r / 1.2f, r);

    if (pico_display.is_pressed(pico_display.Y)) {
      game = (game+1) % max_games;
      while (pico_display.is_pressed(pico_display.Y));
    }

    //switch (game) {
#ifdef GAME_BREAKOUT
    if (Games[game]=="BREAKOUT") brickbash.update(pico_display);
#endif
    //case GAME_FIRE:       fire.update(pico_display);      break;
    //case GAME_PICOBIRD:   picobird.update(pico_display);  break;
#ifdef GAME_UPRISING
    if (Games[game]=="UPRISING") vectormap.update(pico_display);
#endif

#ifdef GAME_SCREENSAVERS
    if (Games[game]=="SCREENSAVERS") screensaver.update(pico_display);
#endif

#ifdef GAME_ASTEROIDS
    if (Games[game]=="ASTEROIDS") asteroids.update(pico_display);
#endif

#ifdef GAME_MISSILEBOMBER
    if (Games[game]=="MISSILEBOMBER") missile.update(pico_display);
#endif

#ifdef GAME_ROLLERBALL
    if (Games[game]=="ROLLERBALL") rollerball.update(pico_display);
#endif

#ifdef GAME_STARHOOPER
    if (Games[game]=="STARHOOPER") starhooper.update(pico_display);
#endif

    //case GAME_POOL:       pool.update(pico_display);   break;

    
    //pico_display.showBattery();
    
    //pico_display.update(pi3c);
    pico_display.update();
    i++;
  }

  return 0;
}
