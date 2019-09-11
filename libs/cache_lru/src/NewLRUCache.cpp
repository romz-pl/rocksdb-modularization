#include <rock/cache/Cache.h>

#include <rock/cache_lru/LRUCache.h>
#include <rock/cache_sharded/GetDefaultCacheShardBits.h>
#include <rock/cache_lru/LRUCacheOptions.h>

namespace rocksdb {

std::shared_ptr<Cache> NewLRUCache(
    size_t capacity,
    int num_shard_bits,
    bool strict_capacity_limit,
    double high_pri_pool_ratio,
    std::shared_ptr<MemoryAllocator> memory_allocator,
    bool use_adaptive_mutex) {
  if (num_shard_bits >= 20) {
    return nullptr;  // the cache cannot be sharded into too many fine pieces
  }
  if (high_pri_pool_ratio < 0.0 || high_pri_pool_ratio > 1.0) {
    // invalid high_pri_pool_ratio
    return nullptr;
  }
  if (num_shard_bits < 0) {
    num_shard_bits = GetDefaultCacheShardBits(capacity);
  }
  return std::make_shared<LRUCache>(capacity, num_shard_bits,
                                    strict_capacity_limit, high_pri_pool_ratio,
                                    std::move(memory_allocator),
                                    use_adaptive_mutex);
}


std::shared_ptr<Cache> NewLRUCache(const LRUCacheOptions& cache_opts) {
  return NewLRUCache(cache_opts.capacity,
                     cache_opts.num_shard_bits,
                     cache_opts.strict_capacity_limit,
                     cache_opts.high_pri_pool_ratio,
                     cache_opts.memory_allocator,
                     cache_opts.use_adaptive_mutex);
}
}
