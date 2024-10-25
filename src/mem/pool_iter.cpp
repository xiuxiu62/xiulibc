#include "pool_iter.hpp"

PoolIter pool_iter(Pool &pool) {
  return {
      .validity_bitmap = pool.validity_bitmap,
      .stride = pool.stride,
      .current = 0,
      .end = pool.extent,
  };
}

bool pool_iter_next(PoolIter &iter, usize &index) {
  while (iter.current < iter.end) {
    if (get_validitiy_bit(iter.validity_bitmap, iter.current)) {
      index = iter.current++;
      return true;
    }
    iter.current++;
  }

  return false;
}
