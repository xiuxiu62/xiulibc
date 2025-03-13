#pragma once

#include "common/types.hpp"

enum class PoolStatus {
    Ok = 0,
    Created = 1,
    Destroyed = 2,
    Uninit = 3,
    ResizeFailed = 4,
    Unchanged = 5,
};

inline bool get_validity_bit(const u8 *bitmap, usize index) {
    return (bitmap[index / 8] >> (index % 8)) & 1;
}

inline void set_validity_bit(u8 *bitmap, usize index, bool value) {
    if (value) {
        bitmap[index / 8] |= (1 << (index % 8));
    } else {
        bitmap[index / 8] &= ~(1 << (index % 8));
    }
}

// Round to the next power of 2
inline usize ceil_pow_2(usize value) {
    value -= 1;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    return value + 1;
}

// Round to the next power of 2
inline usize ceil_pow_2(u32 value) {
    value -= 1;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value + 1;
}
