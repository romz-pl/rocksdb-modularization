#include <rock/string/EscapeOptionString.h>

#include <rock/string/isSpecialChar.h>
#include <rock/string/EscapeChar.h>

namespace rocksdb {

std::string EscapeOptionString(const std::string& raw_string) {
  std::string output;
  for (auto c : raw_string) {
    if (isSpecialChar(c)) {
      output += '\\';
      output += EscapeChar(c);
    } else {
      output += c;
    }
  }

  return output;
}

}
