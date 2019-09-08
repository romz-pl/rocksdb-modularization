#include <rock/string/ParseSizeT.h>

#include <rock/string/ParseUint64.h>

namespace rocksdb {

size_t ParseSizeT(const std::string& value) {
  return static_cast<size_t>(ParseUint64(value));
}

}
