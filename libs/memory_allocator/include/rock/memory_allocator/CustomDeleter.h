#pragma once

#include <rock/memory_allocator/MemoryAllocator.h>

namespace rocksdb {

struct CustomDeleter {
  CustomDeleter(MemoryAllocator* a = nullptr) : allocator(a) {}

  void operator()(char* ptr) const {
    if (allocator) {
      allocator->Deallocate(reinterpret_cast<void*>(ptr));
    } else {
      delete[] ptr;
    }
  }

  MemoryAllocator* allocator;
};

}
