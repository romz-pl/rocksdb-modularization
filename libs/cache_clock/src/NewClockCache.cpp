#include <rock/cache_clock/NewClockCache.h>


#ifndef SUPPORT_CLOCK_CACHE

namespace rocksdb {

std::shared_ptr<Cache> NewClockCache(size_t /*capacity*/, int /*num_shard_bits*/,
                                     bool /*strict_capacity_limit*/) {
  // Clock cache not supported.
  return nullptr;
}

}  // namespace rocksdb

#else

namespace rocksdb {

std::shared_ptr<Cache> NewClockCache(size_t capacity, int num_shard_bits,
                                     bool strict_capacity_limit) {
  if (num_shard_bits < 0) {
    num_shard_bits = GetDefaultCacheShardBits(capacity);
  }
  return std::make_shared<ClockCache>(capacity, num_shard_bits,
                                      strict_capacity_limit);
}

}  // namespace rocksdb

#endif
