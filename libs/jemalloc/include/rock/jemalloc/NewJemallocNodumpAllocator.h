#pragma once

#include <rock/status/Status.h>

#include <memory>

namespace rocksdb {

class MemoryAllocator;
struct JemallocAllocatorOptions;

// Generate memory allocators which allocates through Jemalloc and utilize
// MADV_DONTDUMP through madvice to exclude cache items from core dump.
// Applications can use the allocator with block cache to exclude block cache
// usage from core dump.
//
// Implementation details:
// The JemallocNodumpAllocator creates a delicated jemalloc arena, and all
// allocations of the JemallocNodumpAllocator is through the same arena.
// The memory allocator hooks memory allocation of the arena, and call
// madvice() with MADV_DONTDUMP flag to exclude the piece of memory from
// core dump. Side benefit of using single arena would be reduce of jemalloc
// metadata for some workload.
//
// To mitigate mutex contention for using one single arena, jemalloc tcache
// (thread-local cache) is enabled to cache unused allocations for future use.
// The tcache normally incur 0.5M extra memory usage per-thread. The usage
// can be reduce by limitting allocation sizes to cache.
extern Status NewJemallocNodumpAllocator(
    JemallocAllocatorOptions& options,
    std::shared_ptr<MemoryAllocator>* memory_allocator);

}
