#pragma once

#include "../common/types.hpp"

enum PoolStatus {
    OK = 0,
    CREATED = 1,
    DESTROYED = 2,
    UNINIT = 3,
    RESIZE_FAILED = 4,
    UNCHANGED = 5,
};

inline bool get_validitiy_bit(const u8 *bitmap, usize index) {
    return (bitmap[index / 8] >> (index % 8)) & 1;
}

inline void set_validitiy_bit(u8 *bitmap, usize index, bool value) {
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
