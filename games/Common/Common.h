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

