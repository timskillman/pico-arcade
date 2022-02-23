#pragma once

#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>

#include "pico_display.hpp"

using namespace pimoroni;

struct vectorArt {
public:
    static int16_t SHEILD[];
    static int16_t CASTLE_TURRET[];
    static int16_t MAN1[];
    static int16_t MAN2[];
};

class VectorMap {
public:

    VectorMap() {}

    VectorMap(PicoDisplay& pico_display) {
        init(pico_display);
    }

    void init(PicoDisplay& pico_display);
    void update(PicoDisplay& pico_display);

private:

    void drawHouse(PicoDisplay& pico_display, int32_t x, int32_t z, int32_t y, int32_t w, int32_t h, int32_t d, int32_t roofHeight, int32_t slope, int8_t windows);
    void drawWall(PicoDisplay& pico_display, int32_t x, int32_t z, int32_t y, int32_t w, int32_t h, bool leftWall, Pen col);
    void drawRoof(PicoDisplay& pico_display, int32_t x, int32_t z, int32_t y, int32_t w, int32_t h, int32_t d, int32_t slope, Pen roofCol, Pen endCol);
    void drawTile(PicoDisplay& pico_display,int32_t x, int32_t y, int32_t sz);
    void drawShield(PicoDisplay& pico_display, int32_t x, int32_t y, int32_t w, int32_t h, Pen col);
    void drawFace(PicoDisplay& pico_display, int32_t x, int32_t y, int32_t w, int32_t h, Pen facecol, Pen haircol);
    void drawTree(PicoDisplay& pico_display, int32_t x, int32_t z, int32_t y, int32_t w, Pen leaves, Pen bark);
    void drawMan(PicoDisplay& pico_display, int mandir, int32_t x, int32_t y, int32_t pw, int32_t walk, Pen clothes);
    void drawManX(PicoDisplay& pico_display, int32_t x, int32_t z, int32_t y, int32_t w, int32_t h, int32_t d, Pen clothes);
    void drawManZ(PicoDisplay& pico_display, int32_t x, int32_t z, int32_t y, int32_t w, int32_t h, int32_t d, Pen clothes);
    Point calc3D(int32_t x, int32_t y, int32_t z)
    {
        return Point((x + z)*sc, ((-x + z) / 2 + y)*sc);
    }

    void drawTriangle(PicoDisplay& pico_display, Point p1, Point p2, Point p3)
    {
        pico_display.triangle(Point(p1.x, PicoDisplay::HEIGHT - p1.y), Point(p2.x, PicoDisplay::HEIGHT - p2.y), Point(p3.x, PicoDisplay::HEIGHT - p3.y));
    }

    void drawGrass(PicoDisplay& pico_display, Point p1)
    {
        for (int i = 0; i < 10; i++) {
            Point p = Point(p1.x + fast_rand() % 20, PicoDisplay::HEIGHT - p1.y + fast_rand() % 20);
            pico_display.line(p, Point(p.x, p.y + 5));
        }
    }

    int g_seed = 12345;
    int fast_rand();
    
    PicoDisplay* pico_display = nullptr;
    
    Pen skin;
    Pen blue;
    Pen grey;
    Pen red;
    
    int32_t x = 20, y = 20, z = 20;
    int32_t w = 25, d = 50, h = 15;
    int32_t mapsize = 200;
    int32_t tilesize = 10;
    int32_t maplimit = mapsize * (tilesize / 2);
    
    std::vector<int8_t> map;
    
    float sc = 2;
    int mandir = 0, walk =0, wd = 1;
};
