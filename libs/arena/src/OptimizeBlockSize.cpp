#include <rock/arena/OptimizeBlockSize.h>

#include <rock/arena/Arena.h>

namespace rocksdb {

size_t OptimizeBlockSize(size_t block_size) {
  // Make sure block_size is in optimal range
  block_size = std::max(Arena::kMinBlockSize, block_size);
  block_size = std::min(Arena::kMaxBlockSize, block_size);

  // make sure block_size is the multiple of Arena::kAlignUnit
  if (block_size % Arena::kAlignUnit != 0) {
    block_size = (1 + block_size / Arena::kAlignUnit) * Arena::kAlignUnit;
  }

  return block_size;
}

}
