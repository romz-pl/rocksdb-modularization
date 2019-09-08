#include <rock/string/ParseInt32.h>

#ifndef ROCKSDB_LITE

#include <rock/string/ParseInt64.h>
#include <rock/numeric_limits/numeric_limits.h>

#include <stdexcept>

namespace rocksdb {

int32_t ParseInt32(const std::string& value) {
  int64_t num = ParseInt64(value);
  if (num <= port::kMaxInt32 && num >= port::kMinInt32) {
    return static_cast<int32_t>(num);
  } else {
    throw std::out_of_range(value);
  }
}

}

#endif
