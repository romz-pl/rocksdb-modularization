#pragma once

#include <rock/arena/CacheAllocationPtr.h>

namespace rocksdb {

inline CacheAllocationPtr AllocateBlock(size_t size,
                                        MemoryAllocator* allocator) {
  if (allocator) {
    auto block = reinterpret_cast<char*>(allocator->Allocate(size));
    return CacheAllocationPtr(block, allocator);
  }
  return CacheAllocationPtr(new char[size]);
}

}

