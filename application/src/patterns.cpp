#include "patterns.h"
#include <cmath>
#include <chrono>
#include <thread>

void pattern_fade(TLC59711& tlc) {
    static constexpr int FADE_DELAY_MS = 40;

    TLC59711::Channels channels{};

    // Fade in
    for (int i = 0; i <= 100; i++) {
        float brightness = i / 100.0f;
        for (int j = 0; j < 10; j++) channels[j] = brightness;
        tlc.update(channels);
        std::this_thread::sleep_for(std::chrono::milliseconds(FADE_DELAY_MS));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Fade out
    for (int i = 100; i >= 0; i--) {
        float brightness = i / 100.0f;
        for (int j = 0; j < 10; j++) channels[j] = brightness;
        tlc.update(channels);
        std::this_thread::sleep_for(std::chrono::milliseconds(FADE_DELAY_MS));
    }
}

void pattern_ripple(TLC59711& tlc) {
    static constexpr float TWO_PI      = 6.28318f;
    static constexpr int   N_FINGERS   = 10;
    static constexpr float SPEED       = 1;
    static constexpr int   SLEEP_TIME_MS = 40;

    const float phase_step = (TWO_PI / 3.0f) / N_FINGERS;

    const auto t_start = std::chrono::steady_clock::now();

    while (true) {
        const float t_secs = std::chrono::duration<float>(
            std::chrono::steady_clock::now() - t_start).count();

        TLC59711::Channels channels{};
        for (int f = 0; f < N_FINGERS; ++f) {
            const float raw = std::sin(TWO_PI * SPEED * t_secs + f * phase_step);
            channels[f] = (raw + 1.0f) / 2.0f;
        }

        tlc.update(channels);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS));
    }
}