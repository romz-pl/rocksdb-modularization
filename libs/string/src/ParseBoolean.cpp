#include <rock/string/ParseBoolean.h>

#ifndef ROCKSDB_LITE

#include <stdexcept>

namespace rocksdb {

bool ParseBoolean(const std::string& type, const std::string& value) {
  if (value == "true" || value == "1") {
    return true;
  } else if (value == "false" || value == "0") {
    return false;
  }
  throw std::invalid_argument(type);
}

}

#endif
