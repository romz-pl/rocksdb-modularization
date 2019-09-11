//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//

#pragma once

#include <rock/memory_allocator/CustomDeleter.h>

#include <memory>

namespace rocksdb {

using CacheAllocationPtr = std::unique_ptr<char[], CustomDeleter>;

inline CacheAllocationPtr AllocateBlock(size_t size,
                                        MemoryAllocator* allocator) {
  if (allocator) {
    auto block = reinterpret_cast<char*>(allocator->Allocate(size));
    return CacheAllocationPtr(block, allocator);
  }
  return CacheAllocationPtr(new char[size]);
}

}  // namespace rocksdb
