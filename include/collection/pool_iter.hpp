#pragma once

#include "collection/generational_pool.hpp"
#include "collection/pool.hpp"
#include "common/types.hpp"

struct PoolIter {
    const u8 *validity_bitmap;
    usize stride;
    usize current;
    usize end;
};

PoolIter pool_iter(Pool &pool);
PoolIter pool_iter(GenerationalPool &pool);

bool pool_iter_next(PoolIter &iter, usize &index);
