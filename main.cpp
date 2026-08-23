#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <cmath>
#include <cstdlib>
#include <string>
#include <thread>

struct Melody {
    const int* tones;
    int toneCount;
    int sample = 0;
    int toneSamples;
    int pauseSamples;
};

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

int main() {
    const char* paragraph = "Happy birthday to you.Happy birthday to you.Happy birthday to you.Happy ";
    const int tones[] = {262, 262, 294, 262, 349, 330};
    const int sampleRate = 44100;
    const int toneSamples = sampleRate / 3;
    const int pauseSamples = sampleRate / 20;
    Melody melody{tones, 6, 0, toneSamples, pauseSamples};

    std::string command = "/usr/bin/say -v Alex \"" + std::string(paragraph) + "\"";

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = sampleRate;
    config.dataCallback = audioCallback;
    config.pUserData = &melody;

    ma_device device;
    if (ma_device_init(nullptr, &config, &device) != MA_SUCCESS) return 1;
    if (ma_device_start(&device) != MA_SUCCESS) {
        ma_device_uninit(&device);
        return 1;
    }
    std::thread voice([&command]() {
        system(command.c_str());
    });
    voice.join();
    ma_device_uninit(&device);
}
