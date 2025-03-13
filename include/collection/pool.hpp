#pragma once

#include "collection/common.hpp"
#include "common/types.hpp"
#include <iterator>

typedef isize PoolHandle;

template <typename T> struct Pool {
    struct iterator;
    struct const_iterator;

    static const PoolHandle INVALID_HANDLE = -1;

    T *data;              // Holds the underlying data
    u8 *validity_bitmap;  // Tracks the validity of indicies
    usize *slots;         // Tracks the availability of interlaced indicies
    usize capacity;       // Size of the pool
    usize len = 0;        // Number of live items
    usize extent = 0;     // The maximum possible index of the used bounding region
    usize slot_count = 0; // Number of dead indices

    static Pool create(usize initial_capacity);
    static Pool create(PoolStatus &status, usize initial_capacity);
    void destroy();

    PoolHandle alloc();
    PoolHandle push(T value);
    void remove(PoolHandle handle);

    T *get_unchecked(PoolHandle handle);
    const T *get_unchecked(PoolHandle handle) const;

    T *get(PoolHandle handle);
    const T *get(PoolHandle handle) const;

    PoolStatus reserve(usize new_capacity);
    PoolStatus shrink_to_fit();

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
};

template <typename T> struct Pool<T>::iterator {
  private:
    Pool<T> &pool;
    PoolHandle current;

    void advance_to_valid() {
        while (current < pool.extent && !get_validity_bit(pool.validity_bitmap, current)) {
            current++;
        }
    }

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    iterator(Pool<T> *p, PoolHandle start) : pool(p), current(start) {
        advance_to_valid();
    }

    T &operator*();
    T *operator->();

    iterator &operator++();
    iterator operator++(int);

    bool operator==(const iterator &other) const;
    bool operator!=(const iterator &other) const;

    PoolHandle handle() const;
};

template <typename T> struct Pool<T>::const_iterator {
  private:
    const Pool<T> &pool;
    PoolHandle current;

    void advance_to_valid() {
        while (current < pool->extent && !get_validity_bit(pool->validity_bitmap, current)) {
            current++;
        }
    }

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    const_iterator(const Pool<T> *p, PoolHandle start) : pool(p), current(start) {
        advance_to_valid();
    }

    const T &operator*() const;
    const T *operator->() const;

    const_iterator &operator++();
    const_iterator operator++(int);

    bool operator==(const const_iterator &other) const;
    bool operator!=(const const_iterator &other) const;

    PoolHandle handle() const;
};
