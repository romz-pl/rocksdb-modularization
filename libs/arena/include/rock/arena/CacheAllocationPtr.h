#pragma once

#include <rock/memory_allocator/CustomDeleter.h>

#include <memory>

namespace rocksdb {

using CacheAllocationPtr = std::unique_ptr<char[], CustomDeleter>;

}
