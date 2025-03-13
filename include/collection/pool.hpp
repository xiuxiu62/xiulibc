#pragma once

#include "common.hpp"
#include "common/types.hpp"

struct Pool {
    u8 *data;            // Holds the underlying data
    u8 *validity_bitmap; // Tracks the validity of indicies
    usize *slots;        // Tracks the availability of interlaced indicies
    usize stride;        // Size of an item
    usize capacity;      // Size of the pool
    usize len;           // Number of live items
    usize extent;        // The maximum possible index of the used bounding region
    usize slot_count;    // Number of dead indices
};

struct Handle {
    isize index;
};

PoolStatus pool_create(Pool &pool, usize stride, usize initial_capacity);
void pool_destroy(Pool &pool);

PoolStatus pool_reserve(Pool &pool, usize capacity);
PoolStatus pool_shrink_to_fit(Pool &pool);

Handle pool_alloc(Pool &pool);
void pool_free(Pool &pool, Handle handle);

inline u8 *pool_get_unchecked(const Pool &pool, Handle handle);
u8 *pool_get(const Pool &pool, Handle handle);
