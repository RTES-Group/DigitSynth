#pragma once
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include "types.h"

struct LEDFrame {
    Brightness channels[10];   // indices 0–9 map to R1 R2 R3 R4 G1 G2 G3 B1 B2 B3
};

class TLC59711 {
public:
    TLC59711(int data_pin, int clk_pin, int num_drivers = 1);
    ~TLC59711();



    // start() opens the pipe and spawns the worker thread.
    void start();
    
    //signals the worker to exit and joins — safe to call from destructor.
    void stop();


    // Writes one LEDFrame into the pipe. Returns immediately.
    void update(const LEDFrame& frame);

    // Global hardware brightness scaler
    void setBrightness(uint8_t brightness);

private:

    // Blocks on pipe read and is woken by update().
    void worker();

    // ── Packet assembly ──────────────────────────────────────────────────────
    // Converts _pwm[] into a 28-byte TLC59711 frame per driver.
    void buildPacket(std::vector<uint8_t>& buf) const;

    // ── Bit-bang SPI ─────────────────────────────────────────────────────────
    // CPOL=0 CPHA=0, MSB first. Latch by silence after final byte.
    void shiftOut(const std::vector<uint8_t>& buf) const;

    // ── Gamma correction ─────────────────────────────────────────────────────
    // Maps linear Brightness (0.0–1.0) → 16-bit GS value with γ=2.2.
    static uint16_t toGS(Brightness b);


    // LEDFrame index → TLC59711 GS array index
    // Frame:  [R1 R2 R3 R4 | G1 G2 G3 | B1 B2 B3]
    // GS[]:   [R0 G0 B0 R1 G1 B1 R2 G2 B2 R3 G3 B3]  (per chip, 0-indexed)
    //
    // R1→GS[3]  R2→GS[6]  R3→GS[9]   R4→GS[0] (uses LED0 red)
    // G1→GS[4]  G2→GS[7]  G3→GS[10]
    // B1→GS[5]  B2→GS[8]  B3→GS[11]
    //
    // GS[1]=G0 and GS[2]=B0 are unused (no LED wired).
    static constexpr int FRAME_TO_GS[10] = {
        3, 6, 9, 0,   // R1 R2 R3 R4
        4, 7, 10,     // G1 G2 G3
        5, 8, 11      // B1 B2 B3
    };

    static constexpr int CHANNELS_PER_DRIVER = 12;
    static constexpr int NUM_LEDS            = 10;
    static constexpr long HALF_PERIOD_NS     = 5000L;   // ~100 kHz

    std::thread       _thread;
    std::atomic<bool> _running{false};

    int _gpio_handle{-1};
    int _data_pin{0};
    int _clk_pin{0};
    int _num_drivers{1};

    int _pipe_fd[2]{-1, -1};   // [0]=read end (worker), [1]=write end (update)

    uint8_t _brightness{127};   

    // Owned exclusively by worker thread after start()
    std::vector<uint16_t> _pwm;
};