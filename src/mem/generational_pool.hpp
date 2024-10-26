#pragma once

#include "../common/types.hpp"

struct GenerationalPool {
    u8 *data;            // Holds the underlying data
    u8 *validity_bitmap; // Tracks the validity of indicies
    usize *slots;        // Tracks the availability of interlaced indicies
    usize *generations;  // Tracks the generation of indicies for handles to
    usize stride;        // Size of an item
    usize capacity;      // Size of the pool
    usize len;           // Number of live items
    usize extent;        // The maximum possible index of the used bounding region
    usize slot_count;    // Number of dead indices
};

struct GenHandle {
    usize index;
    usize generation;
};
