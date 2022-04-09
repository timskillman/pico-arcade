#include "ScreenSaver.h"


#define screenTime 50000000 //10 seconds

void ScreenSaver::init(PicoDisplay& pico_display)
{
	
	nextScreenTime = GetTicks() + screenTime;
	
	metaballs = MetaBalls(pico_display);
	plasma = Plasma(pico_display);
	rotozoom = RotoZoom(pico_display);
	starfield = StarField(pico_display);
	bouncingballs = BouncingBalls(pico_display);
	strings = Strings(pico_display);
	triangles = Triangles(pico_display);
	gfx = GfxTest(pico_display);
}

void ScreenSaver::update(PicoDisplay& pico_display)
{
	switch (currentScreen) {
	case 5: bouncingballs.update(pico_display); break;
	case 1: plasma.update(pico_display); break;
	case 2: rotozoom.update(pico_display); break;
	case 3: starfield.update(pico_display); break;
	case 4: metaballs.update(pico_display); break;
	case 6: strings.update(pico_display); break;
	case 7: gfx.update(pico_display); break;
	case 0: triangles.update(pico_display); break;
	}
	
	if (GetTicks() > nextScreenTime || pico_display.is_pressed(pico_display.X)) {
		while (pico_display.is_pressed(pico_display.X));
		currentScreen = (currentScreen + 1) % maxScreens;
		nextScreenTime = GetTicks() + screenTime;
	}
}
