#pragma once
#include <cmath>

#include "SDL3/SDL_rect.h"

namespace utils {
inline constexpr float SQR_2 = 1.4142136f;

inline SDL_FPoint normalize(const SDL_FPoint& point) {
    const auto mod = std::sqrt(point.x * point.x + point.y * point.y);
    return {point.x / mod, point.y / mod};
}

inline SDL_FPoint difference(const SDL_FPoint& a, const SDL_FPoint& b) {
   return { a.x - b.x, a.y - b.y };
}

inline float lenght(const SDL_FPoint& point) {
    return std::sqrt(point.x * point.x + point.y * point.y);
}
}
