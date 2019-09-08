#include <rock/string/UnescapeOptionString.h>

#include <rock/string/UnescapeChar.h>

namespace rocksdb {

std::string UnescapeOptionString(const std::string& escaped_string) {
  bool escaped = false;
  std::string output;

  for (auto c : escaped_string) {
    if (escaped) {
      output += UnescapeChar(c);
      escaped = false;
    } else {
      if (c == '\\') {
        escaped = true;
        continue;
      }
      output += c;
    }
  }
  return output;
}

}
