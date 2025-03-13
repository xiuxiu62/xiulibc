#include "collection/pool.hpp"
#include "collection/common.hpp"
#include "common/types.hpp"

#include <cstdlib>
#include <cstring>

template <typename T> Pool<T> Pool<T>::create(usize initial_capacity) {
    Pool<T> pool;
    pool.capacity = initial_capacity;
    pool.data = static_cast<T *>(malloc(initial_capacity * sizeof(T)));
    pool.slots = static_cast<usize *>(calloc(initial_capacity, sizeof(usize)));
    pool.validity_bitmap = static_cast<u8 *>(calloc((initial_capacity + 7) / 8, 1));
    return pool;
}

template <typename T> Pool<T> Pool<T>::create(PoolStatus &status, usize initial_capacity) {
    Pool<T> pool;
    pool.capacity = initial_capacity;
    pool.data = static_cast<T *>(malloc(initial_capacity * sizeof(T)));
    pool.slots = static_cast<usize *>(calloc(initial_capacity, sizeof(usize)));
    pool.validity_bitmap = static_cast<u8 *>(calloc((initial_capacity + 7) / 8, 1));

    if (!pool.data || !pool.slots || !pool.validity_bitmap) {
        status = PoolStatus::Uninit;
    } else {
        status = PoolStatus::Created;
    }

    return pool;
}

template <typename T> void Pool<T>::destroy() {
    free(data);
    free(slots);
    free(validity_bitmap);
}

template <typename T> PoolStatus Pool<T>::reserve(usize new_capacity) {
    // Return early if we already match this capacity
    if (new_capacity <= capacity) return PoolStatus::Ok;

    // Resize buffers
    data = static_cast<T *>(realloc(data, new_capacity * sizeof(T)));
    slots = static_cast<usize *>(realloc(slots, new_capacity * sizeof(usize)));
    validity_bitmap = static_cast<u8 *>(realloc(validity_bitmap, (new_capacity + 7) / 8));

    if (!data || !slots || !validity_bitmap) return PoolStatus::ResizeFailed;

    // Zero out new memory regions
    memset(slots + capacity, 0, (new_capacity - capacity) * sizeof(usize));
    memset(validity_bitmap + ((capacity + 7) / 8), 0, ((new_capacity + 7) / 8) - ((capacity + 7) / 8));

    capacity = new_capacity;

    return PoolStatus::Ok;
}

template <typename T> PoolStatus Pool<T>::shrink_to_fit() {
    if (extent == 0) return PoolStatus::Unchanged;
    usize min_capacity = ceil_pow_2(extent);

    // Don't shrink if we're using most of the capacity (wasting less than 25%)
    if (capacity - min_capacity < (capacity / 4)) return PoolStatus::Unchanged;

    data = static_cast<T *>(realloc(data, min_capacity * sizeof(T)));
    slots = static_cast<usize *>(realloc(slots, min_capacity * sizeof(usize)));
    validity_bitmap = static_cast<u8 *>(realloc(validity_bitmap, (min_capacity + 7) / 8));

    if (!data || !slots || !validity_bitmap) return PoolStatus::ResizeFailed;

    capacity = min_capacity;

    return PoolStatus::Ok;
}

template <typename T> PoolHandle Pool<T>::alloc() {
    PoolHandle handle = -1;
    PoolStatus status;

    // Double the buffer size if necessary
    if (len >= capacity) {
        status = reserve(capacity * 2);
        if (status != PoolStatus::Ok) return handle;
    }

    usize index;
    if (slot_count > 0) {
        index = slots[len];
        slot_count--;
    } else {
        index = len;
    }
    handle = index;

    set_validity_bit(validity_bitmap, index, true);
    len++;

    if (index >= extent) extent = index + 1;

    return handle;
}

template <typename T> PoolHandle Pool<T>::push(T value) {
    PoolHandle handle = alloc();

    // If allocation failed, return invalid handle
    if (handle == INVALID_HANDLE) return INVALID_HANDLE;

    // Set the value
    data[handle] = value;

    return handle;
}

template <typename T> void Pool<T>::remove(PoolHandle handle) {
    if (handle < 0) return;

    // Ensure handle is valid
    bool out_of_bounds = handle > extent;
    bool is_valid = !get_validity_bit(validity_bitmap, handle);
    if (out_of_bounds || !is_valid) return;

    // Clear index validity
    set_validity_bit(validity_bitmap, handle, false);

    // Add to free list
    slots[len - 1] = handle;
    len--;
    slot_count++;

    // Update extent if freeing the last element
    if (handle == extent - 1) extent--;
}

template <typename T> T *Pool<T>::get_unchecked(PoolHandle handle) {
    return data[handle];
}

template <typename T> const T *Pool<T>::get_unchecked(PoolHandle handle) const {
    return data[handle];
}

template <typename T> T *Pool<T>::get(PoolHandle handle) {
    if (handle >= extent || !get_validity_bit(validity_bitmap, handle)) {
        return nullptr;
    } else {
        return data[handle];
    }
}

template <typename T> const T *Pool<T>::get(PoolHandle handle) const {
    if (handle >= extent || !get_validity_bit(validity_bitmap, handle)) {
        return nullptr;
    } else {
        return data[handle];
    }
}
