#pragma once

#include <memory>

namespace rocksdb {

class Cache;

// Similar to NewLRUCache, but create a cache based on CLOCK algorithm with
// better concurrent performance in some cases. See util/clock_cache.cc for
// more detail.
//
// Return nullptr if it is not supported.
extern std::shared_ptr<Cache> NewClockCache(size_t capacity,
                                            int num_shard_bits = -1,
                                            bool strict_capacity_limit = false);

}
