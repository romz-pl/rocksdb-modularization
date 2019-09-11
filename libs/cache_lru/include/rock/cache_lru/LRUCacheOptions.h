#pragma once

#include <rock/cache_lru/kDefaultToAdaptiveMutex.h>

#include <memory>

namespace rocksdb {

class MemoryAllocator;

struct LRUCacheOptions {
  // Capacity of the cache.
  size_t capacity = 0;

  // Cache is sharded into 2^num_shard_bits shards,
  // by hash of key. Refer to NewLRUCache for further
  // information.
  int num_shard_bits = -1;

  // If strict_capacity_limit is set,
  // insert to the cache will fail when cache is full.
  bool strict_capacity_limit = false;

  // Percentage of cache reserved for high priority entries.
  // If greater than zero, the LRU list will be split into a high-pri
  // list and a low-pri list. High-pri entries will be insert to the
  // tail of high-pri list, while low-pri entries will be first inserted to
  // the low-pri list (the midpoint). This is refered to as
  // midpoint insertion strategy to make entries never get hit in cache
  // age out faster.
  //
  // See also
  // BlockBasedTableOptions::cache_index_and_filter_blocks_with_high_priority.
  double high_pri_pool_ratio = 0.5;

  // If non-nullptr will use this allocator instead of system allocator when
  // allocating memory for cache blocks. Call this method before you start using
  // the cache!
  //
  // Caveat: when the cache is used as block cache, the memory allocator is
  // ignored when dealing with compression libraries that allocate memory
  // internally (currently only XPRESS).
  std::shared_ptr<MemoryAllocator> memory_allocator;

  // Whether to use adaptive mutexes for cache shards. Note that adaptive
  // mutexes need to be supported by the platform in order for this to have any
  // effect. The default value is true if RocksDB is compiled with
  // -DROCKSDB_DEFAULT_TO_ADAPTIVE_MUTEX, false otherwise.
  bool use_adaptive_mutex = kDefaultToAdaptiveMutex;

  LRUCacheOptions() {}
  LRUCacheOptions(size_t _capacity, int _num_shard_bits,
                  bool _strict_capacity_limit, double _high_pri_pool_ratio,
                  std::shared_ptr<MemoryAllocator> _memory_allocator = nullptr,
                  bool _use_adaptive_mutex = kDefaultToAdaptiveMutex)
      : capacity(_capacity),
        num_shard_bits(_num_shard_bits),
        strict_capacity_limit(_strict_capacity_limit),
        high_pri_pool_ratio(_high_pri_pool_ratio),
        memory_allocator(std::move(_memory_allocator)),
        use_adaptive_mutex(_use_adaptive_mutex) {}
};


}
