#pragma once

#include "common/types.hpp"
#include "math/vector.hpp"

#include <cmath>

struct Rot2 {
    f32 scalar, bivector;

    static inline constexpr Rot2 IDENTITY() {
        return {1, 0};
    }

    // static inline constexpr Rot2 create() {
    // }

    // Rot2() : scalar(1), bivector(0) {
    // }

    // constexpr Rot2(f32 scalar, f32 bivector) : scalar(scalar),
    // bivector(bivector) {
    // }

    // Constructor from rotation angle
    static inline Rot2 from_angle(float angle) {
        f32 half_angle = angle * 0.5f;
        return {std::cos(half_angle), std::sin(half_angle)};
    }

    Vec2 rotate(const Vec2 &v) const {
        f32 x = v.x * (scalar * scalar - bivector * bivector) + v.y * (-2 * scalar * bivector);
        f32 y = v.x * (2 * scalar * bivector) + v.y * (scalar * scalar - bivector * bivector);
        return {x, y};
    }

    Rot2 operator*(const Rot2 &other) const {
        return {
            scalar * other.scalar - bivector * other.bivector,
            scalar * other.bivector + bivector * other.scalar,
        };
    }
};
