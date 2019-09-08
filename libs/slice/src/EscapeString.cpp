#include <rock/slice/EscapeString.h>

#include <rock/slice/AppendEscapedStringTo.h>

namespace rocksdb {

std::string EscapeString(const Slice& value) {
  std::string r;
  AppendEscapedStringTo(&r, value);
  return r;
}

}
