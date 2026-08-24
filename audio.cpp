#include "audio.h"

#include <cmath>

void audioCallback(ma_device* device, void* outputData, const void*, ma_uint32 frameCount) {
    auto* melody = static_cast<Melody*>(device->pUserData);
    auto* output = static_cast<float*>(outputData);

    for (ma_uint32 index = 0; index < frameCount; ++index, ++melody->sample) {
        const int position = melody->sample % (melody->toneSamples + melody->pauseSamples);
        const int toneIndex = melody->sample / (melody->toneSamples + melody->pauseSamples) % melody->toneCount;
        output[index] = position < melody->toneSamples
            ? 0.3f * std::sin(2 * M_PI * melody->tones[toneIndex] * position / 44100.0)
            : 0.0f;
    }
}