#include <rock/string/NumberToString.h>

#include <rock/string/AppendNumberTo.h>

namespace rocksdb {

std::string NumberToString(uint64_t num) {
  std::string r;
  AppendNumberTo(&r, num);
  return r;
}

}
