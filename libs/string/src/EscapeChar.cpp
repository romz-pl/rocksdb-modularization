#include <rock/string/EscapeChar.h>

#include <algorithm>

namespace rocksdb {


namespace {
using CharMap = std::pair<char, char>;
}



char EscapeChar(const char c) {
  static const CharMap convert_map[] = {{'\n', 'n'}, {'\r', 'r'}};

  auto iter = std::find_if(std::begin(convert_map), std::end(convert_map),
                           [c](const CharMap& p) { return p.first == c; });

  if (iter == std::end(convert_map)) {
    return c;
  }
  return iter->second;
}

}
