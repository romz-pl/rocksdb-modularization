#include <rock/string/ParseUint32.h>

#ifndef ROCKSDB_LITE

#include <rock/string/ParseUint64.h>

#include <stdexcept>

namespace rocksdb {

uint32_t ParseUint32(const std::string& value) {
  uint64_t num = ParseUint64(value);
  if ((num >> 32LL) == 0) {
    return static_cast<uint32_t>(num);
  } else {
    throw std::out_of_range(value);
  }
}

}

#endif
