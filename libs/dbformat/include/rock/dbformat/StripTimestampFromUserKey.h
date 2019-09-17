#pragma once

#include <rock/slice/Slice.h>

namespace rocksdb {

inline Slice StripTimestampFromUserKey(const Slice& user_key, size_t ts_sz) {
  assert(user_key.size() >= ts_sz);
  return Slice(user_key.data(), user_key.size() - ts_sz);
}

}
