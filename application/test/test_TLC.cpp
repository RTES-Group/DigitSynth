#include "TLC59711.h"
#include <cstdio>

class TLC59711Test {
public:

    static int test_toGS_zero() {
        return TLC59711::toGS(0.0f) == 0 ? 0 : -1;
    }

    static int test_toGS_full() {
        return TLC59711::toGS(1.0f) == 0xFFFF ? 0 : -1;
    }

    static int test_toGS_clamp_negative() {
        return TLC59711::toGS(-1.0f) == 0 ? 0 : -1;
    }

    static int test_toGS_clamp_over() {
        return TLC59711::toGS(2.0f) == 0xFFFF ? 0 : -1;
    }

    static int test_toGS_gamma() {
        // 0.5 with γ=2.2 → 0.5^2.2 * 65535 ≈ 0x7BEF
        // just verify it's less than the linear midpoint (0x7FFF)
        // — that's the whole point of gamma correction
        const uint16_t gs = TLC59711::toGS(0.5f);
        return (gs > 0 && gs < 0x7FFF) ? 0 : -1;
    }

    // FRAME_TO_GS mapping sanity
    static int test_channel_map_reds() {
        // R1=GS[3], R2=GS[6], R3=GS[9], R4=GS[0]
        return (TLC59711::FRAME_TO_GS[0] == 3 &&
                TLC59711::FRAME_TO_GS[1] == 6 &&
                TLC59711::FRAME_TO_GS[2] == 9 &&
                TLC59711::FRAME_TO_GS[3] == 0) ? 0 : -1;
    }

    static int test_channel_map_greens() {
        // G1=GS[4], G2=GS[7], G3=GS[10]
        return (TLC59711::FRAME_TO_GS[4] == 4 &&
                TLC59711::FRAME_TO_GS[5] == 7 &&
                TLC59711::FRAME_TO_GS[6] == 10) ? 0 : -1;
    }

    static int test_channel_map_blues() {
        // B1=GS[5], B2=GS[8], B3=GS[11]
        return (TLC59711::FRAME_TO_GS[7] ==  5 &&
                TLC59711::FRAME_TO_GS[8] ==  8 &&
                TLC59711::FRAME_TO_GS[9] == 11) ? 0 : -1;
    }

};


static int run(const char* name, int(*test)()) {
    const int result = test();
    printf("[%s] %s\n", result == 0 ? "PASS" : "FAIL", name);
    return result;
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    int status = 0;
    status |= run("toGS_zero",          TLC59711Test::test_toGS_zero);
    status |= run("toGS_full",          TLC59711Test::test_toGS_full);
    status |= run("toGS_clamp_negative",TLC59711Test::test_toGS_clamp_negative);
    status |= run("toGS_clamp_over",    TLC59711Test::test_toGS_clamp_over);
    status |= run("toGS_gamma",         TLC59711Test::test_toGS_gamma);
    status |= run("channel_map_reds",   TLC59711Test::test_channel_map_reds);
    status |= run("channel_map_greens", TLC59711Test::test_channel_map_greens);
    status |= run("channel_map_blues",  TLC59711Test::test_channel_map_blues);

    return status == 0 ? 0 : -1;
}