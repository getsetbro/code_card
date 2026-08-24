#pragma once

#include "miniaudio.h"

struct Melody {
    const int* tones;
    int toneCount;
    int sample = 0;
    int toneSamples;
    int pauseSamples;
};

void audioCallback(ma_device* device, void* outputData, const void*, ma_uint32 frameCount);