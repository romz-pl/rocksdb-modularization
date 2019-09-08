#pragma once

#include <string>

namespace rocksdb {

template <typename T>
inline std::string ToString(T value) {
#if !(defined OS_ANDROID) && !(defined CYGWIN) && !(defined OS_FREEBSD)
  return std::to_string(value);
#else
  // Andorid or cygwin doesn't support all of C++11, std::to_string() being
  // one of the not supported features.
  std::ostringstream os;
  os << value;
  return os.str();
#endif
}

}

