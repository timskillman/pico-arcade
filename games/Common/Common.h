#pragma once
#include "hardware/timer.h"

static unsigned int g_seed = 123435;

// Used to seed the generator.           
inline void fast_rand_seed(int seed) {
    g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
inline int fast_rand() {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

inline int fast_rand(int32_t range) {
    g_seed = (214013 * g_seed + 2531011);
    return ((g_seed >> 16) & 0x7FFF) % range;
}

inline int32_t GetTicks() {
    return timer_hw->timelr;
}

/*
static uint16_t cos90[90] = { 
    0xFFFF, 0xFFF6, 0xFFD8, 0xFFA6, 0xFF60, 0xFF06, 0xFE98, 0xFE17, 0xFD82, 0xFCD9, 0xFC1C, 0xFB4B, 0xFA67, 0xF970, 0xF865, 
    0xF746, 0xF615, 0xF4D0, 0xF378, 0xF20D, 0xF08F, 0xEEFF, 0xED5B, 0xEBA6, 0xE9DE, 0xE803, 0xE617, 0xE418, 0xE208, 0xDFE7, 
    0xDDB3, 0xDB6F, 0xD919, 0xD6B3, 0xD43B, 0xD1B3, 0xCF1B, 0xCC73, 0xC9BB, 0xC6F3, 0xC41B, 0xC134, 0xBE3E, 0xBB39, 0xB826, 
    0xB504, 0xB1D5, 0xAE97, 0xAB4C, 0xA7F3, 0xA48D, 0xA11B, 0x9D9B, 0x9A10, 0x9679, 0x92D5, 0x8F27, 0x8B6D, 0x87A8, 0x83D9, 
    0x7FFF, 0x7C1C, 0x782F, 0x7438, 0x7039, 0x6C30, 0x681F, 0x6406, 0x5FE6, 0x5BBD, 0x578E, 0x5358, 0x4F1B, 0x4AD8, 0x4690, 
    0x4241, 0x3DEE, 0x3996, 0x3539, 0x30D8, 0x2C74, 0x280C, 0x23A0, 0x1F32, 0x1AC2, 0x164F, 0x11DB, 0x0D65, 0x08EF, 0x0477 };

inline int32_t fastcos(int16_t degree) {
  switch (((degree>=360) ? degree % 360 : degree) / 90) {
    case 0: return (int32_t)cos90[degree % 90];
    case 1: return -(int32_t)cos90[90 - (degree % 90)];
    case 2: return -(int32_t)cos90[degree % 90];
    case 3: return (int32_t)cos90[90 - (degree % 90)];
  }
}

inline int32_t fastsin(int16_t degree) {
  switch (((degree>=360) ? degree % 360 : degree) / 90) {
    case 0: return (int32_t)cos90[90 - (degree % 90)];
    case 1: return (int32_t)cos90[degree % 90];
    case 2: return -(int32_t)cos90[90 - (degree % 90)];
    case 3: return -(int32_t)cos90[degree % 90];
  }
}
*/

