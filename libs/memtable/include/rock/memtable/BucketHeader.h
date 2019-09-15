#pragma once

#include <rock/memtable/Pointer.h>

namespace rocksdb {

// A data structure used as the header of a link list of a hash bucket.
struct BucketHeader {
  Pointer next;
  std::atomic<uint32_t> num_entries;

  explicit BucketHeader(void* n, uint32_t count)
      : next(n), num_entries(count) {}

  bool IsSkipListBucket() {
    return next.load(std::memory_order_relaxed) == this;
  }

  uint32_t GetNumEntries() const {
    return num_entries.load(std::memory_order_relaxed);
  }

  // REQUIRES: called from single-threaded Insert()
  void IncNumEntries() {
    // Only one thread can do write at one time. No need to do atomic
    // incremental. Update it with relaxed load and store.
    num_entries.store(GetNumEntries() + 1, std::memory_order_relaxed);
  }
};


}
