// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// A Cache is an interface that maps keys to values.  It has internal
// synchronization and may be safely accessed concurrently from
// multiple threads.  It may automatically evict entries to make room
// for new entries.  Values have a specified charge against the cache
// capacity.  For example, a cache where the values are variable
// length strings, may use the length of the string as the charge for
// the string.
//
// A builtin cache implementation with a least-recently-used eviction
// policy is provided.  Clients may use their own implementations if
// they want something more sophisticated (like scan-resistance, a
// custom eviction policy, variable cache sizing, etc.)

#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <rock/memory_allocator/MemoryAllocator.h>
#include <rock/slice/Slice.h>
#include <rock/statistics/Statistics.h>
#include <rock/status/Status.h>

namespace rocksdb {

class Cache;

extern const bool kDefaultToAdaptiveMutex;

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

// Create a new cache with a fixed size capacity. The cache is sharded
// to 2^num_shard_bits shards, by hash of the key. The total capacity
// is divided and evenly assigned to each shard. If strict_capacity_limit
// is set, insert to the cache will fail when cache is full. User can also
// set percentage of the cache reserves for high priority entries via
// high_pri_pool_pct.
// num_shard_bits = -1 means it is automatically determined: every shard
// will be at least 512KB and number of shard bits will not exceed 6.
extern std::shared_ptr<Cache> NewLRUCache(
    size_t capacity, int num_shard_bits = -1,
    bool strict_capacity_limit = false, double high_pri_pool_ratio = 0.5,
    std::shared_ptr<MemoryAllocator> memory_allocator = nullptr,
    bool use_adaptive_mutex = kDefaultToAdaptiveMutex);

extern std::shared_ptr<Cache> NewLRUCache(const LRUCacheOptions& cache_opts);



}  // namespace rocksdb
