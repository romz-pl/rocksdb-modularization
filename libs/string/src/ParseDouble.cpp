#include <rock/string/ParseDouble.h>

namespace rocksdb {

double ParseDouble(const std::string& value) {
#ifndef CYGWIN
  return std::stod(value);
#else
  return std::strtod(value.c_str(), 0);
#endif
}

}
