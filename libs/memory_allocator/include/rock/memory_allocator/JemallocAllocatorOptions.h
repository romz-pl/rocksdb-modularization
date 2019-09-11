#pragma once

#include <cstdlib>

namespace rocksdb {

struct JemallocAllocatorOptions {
  // Jemalloc tcache cache allocations by size class. For each size class,
  // it caches between 20 (for large size classes) to 200 (for small size
  // classes). To reduce tcache memory usage in case the allocator is access
  // by large number of threads, we can control whether to cache an allocation
  // by its size.
  bool limit_tcache_size = false;

  // Lower bound of allocation size to use tcache, if limit_tcache_size=true.
  // When used with block cache, it is recommneded to set it to block_size/4.
  size_t tcache_size_lower_bound = 1024;

  // Upper bound of allocation size to use tcache, if limit_tcache_size=true.
  // When used with block cache, it is recommneded to set it to block_size.
  size_t tcache_size_upper_bound = 16 * 1024;
};


}
