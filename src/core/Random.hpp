#pragma once
#include <cstdint>
#include <ctime>

namespace rnd {
    inline static uint64_t x = time(nullptr);

    inline uint64_t Get64() {
        uint64_t z = (x += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }

    inline bool GetBit() {
        return Get64() & 1;
    }

    inline uint32_t Get32() {
        return static_cast<uint32_t>(Get64());
    }

    inline float GetFP32() {
        union {
            uint32_t i;
            float f;
        } u{};
        u.i = (Get32() >> 9) | 0x3F800000;
        return u.f - 1.0f;
    }

    inline double GetFP64() {
        union {
            uint64_t i;
            double d;
        } u{};
        u.i = (Get64() >> 12) | 0x3FF0000000000000;
        return u.d - 1.0;
    }

    inline int GetInt() {
        return static_cast<int>(Get64());
    }
}
