#include <rock/string/isSpecialChar.h>

namespace rocksdb {

bool isSpecialChar(const char c) {
  if (c == '\\' || c == '#' || c == ':' || c == '\r' || c == '\n') {
    return true;
  }
  return false;
}

}
