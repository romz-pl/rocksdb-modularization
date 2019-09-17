#pragma once

#include <rock/slice/Slice.h>
#include <rock/coding/coding.h>

namespace rocksdb {

inline uint64_t ExtractInternalKeyFooter(const Slice& internal_key) {
  assert(internal_key.size() >= 8);
  const size_t n = internal_key.size();
  return DecodeFixed64(internal_key.data() + n - 8);
}

}
