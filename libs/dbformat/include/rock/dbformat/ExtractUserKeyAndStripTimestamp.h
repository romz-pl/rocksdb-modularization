#pragma once

#include <rock/slice/Slice.h>

namespace rocksdb {

inline Slice ExtractUserKeyAndStripTimestamp(const Slice& internal_key,
                                             size_t ts_sz) {
  assert(internal_key.size() >= 8 + ts_sz);
  return Slice(internal_key.data(), internal_key.size() - 8 - ts_sz);
}

}
