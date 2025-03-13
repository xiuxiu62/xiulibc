#include "collection/pool.hpp"
#include "collection/common.hpp"

#include <cstdlib>
#include <cstring>

PoolStatus pool_create(Pool &pool, usize stride, usize initial_capacity) {
    pool.stride = stride;
    pool.capacity = initial_capacity;
    pool.len = 0;
    pool.extent = 0;
    pool.slot_count = 0;

    pool.data = static_cast<u8 *>(malloc(initial_capacity * stride));
    pool.slots = static_cast<usize *>(calloc(initial_capacity * sizeof(usize), 0));
    pool.validity_bitmap = static_cast<u8 *>(calloc((initial_capacity + 7) / 8, 0));

    if (!pool.data || !pool.slots || !pool.validity_bitmap)
        return PoolStatus::UNINIT;
    else
        return PoolStatus::CREATED;
}

void pool_destroy(Pool &pool) {
    free(pool.data);
    free(pool.slots);
    free(pool.validity_bitmap);
}

PoolStatus pool_reserve(Pool &pool, usize capacity) {
    if (capacity <= pool.capacity) return PoolStatus::OK;

    u8 *data = static_cast<u8 *>(realloc(pool.data, capacity * pool.stride));
    usize *slots = static_cast<usize *>(realloc(pool.slots, capacity * sizeof(usize)));
    u8 *validity_bitmap = static_cast<u8 *>(realloc(pool.validity_bitmap, (capacity + 7) / 8));

    if (!data || !slots || !validity_bitmap) return PoolStatus::RESIZE_FAILED;

    memset(slots + pool.capacity, 0, (capacity - pool.capacity) * sizeof(usize));
    memset(validity_bitmap + ((pool.capacity + 7) / 8), 0, ((capacity + 7) / 8) - ((pool.capacity + 7) / 8));

    pool.data = data;
    pool.slots = slots;
    pool.validity_bitmap = validity_bitmap;
    pool.capacity = capacity;

    return PoolStatus::OK;
}

PoolStatus pool_shrink_to_fit(Pool &pool) {
    if (pool.extent == 0) return PoolStatus::UNCHANGED;
    usize min_capacity = ceil_pow_2(pool.extent);

    // Don't shrink if we're using most of the capacity
    if (pool.capacity - min_capacity < (pool.capacity / 4)) return PoolStatus::UNCHANGED; // Less than 25% waste

    u8 *data = static_cast<u8 *>(realloc(pool.data, min_capacity * pool.stride));
    usize *slots = static_cast<usize *>(realloc(pool.slots, min_capacity * sizeof(usize)));
    u8 *validity_bitmap = static_cast<u8 *>(realloc(pool.validity_bitmap, (min_capacity + 7) / 8));

    if (!data || !slots || !validity_bitmap) return PoolStatus::RESIZE_FAILED;

    pool.data = data;
    pool.slots = slots;
    pool.validity_bitmap = validity_bitmap;
    pool.capacity = min_capacity;

    return PoolStatus::OK;
}

Handle pool_alloc(Pool &pool) {
    Handle handle{-1};
    PoolStatus status;

    if (pool.len >= pool.capacity) {
        status = pool_reserve(pool, pool.capacity * 2);
        if (status == PoolStatus::UNINIT) return handle;
    }

    usize index;
    if (pool.slot_count > 0) {
        index = pool.slots[pool.len];
        pool.slot_count--;
    } else {
        index = pool.len;
    }
    handle = Handle{static_cast<isize>(index)};

    set_validitiy_bit(pool.validity_bitmap, index, true);
    pool.len++;

    if (index >= pool.extent) pool.extent = index + 1;

    return handle;
}

void pool_free(Pool &pool, Handle handle) {
    if (handle.index < 0) return;
    usize index = static_cast<usize>(handle.index);

    // Ensure handle is valid
    bool out_of_bounds = index > pool.extent;
    bool is_valid = !get_validitiy_bit(pool.validity_bitmap, index);
    if (out_of_bounds || !is_valid) return;

    // Clear index validity
    set_validitiy_bit(pool.validity_bitmap, index, false);

    // Add to free list
    pool.slots[pool.len - 1] = index;
    pool.len--;
    pool.slot_count++;

    // Update extent if freeing the last element
    if (index == pool.extent - 1) pool.extent--;
}

u8 *pool_get_unchecked(const Pool &pool, Handle handle) {
    if (handle.index < 0) return nullptr;

    return pool.data + (handle.index * pool.stride);
}

u8 *pool_get(const Pool &pool, Handle handle) {
    if (handle.index < 0) return nullptr;

    usize index = static_cast<usize>(handle.index);

    bool in_bounds = index < pool.extent;
    bool is_valid = get_validitiy_bit(pool.validity_bitmap, index);
    if (!in_bounds || !is_valid) return nullptr;

    return pool.data + (handle.index * pool.stride);
}
