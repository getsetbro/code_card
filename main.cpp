#include "audio.h"
#include <cstdlib>
#include <string>
#include <thread>

int main() {
    const char* paragraph = R"(13-ish years ago I showed you some web language code. You wondered why the code was made to do what it does but that didn’t stop you from learning it. That day you put the new knowledge to good use making a web-app that wished me a happy birthday with animation. That code was then printed on paper and stuck to my birthday cake. I loved it!
People have talents. Some get paid for their talents. Few put their talents to use for others - it is very special when they do. I think you are an uber special guy who uses your talents for awesome and not evil. I hope you can feel that during this season of your life.)";
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
