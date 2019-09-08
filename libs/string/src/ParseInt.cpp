#include <rock/string/ParseInt.h>

namespace rocksdb {

int ParseInt(const std::string& value) {
  size_t endchar;
#ifndef CYGWIN
  int num = std::stoi(value.c_str(), &endchar);
#else
  char* endptr;
  int num = std::strtoul(value.c_str(), &endptr, 0);
  endchar = endptr - value.c_str();
#endif

  if (endchar < value.length()) {
    char c = value[endchar];
    if (c == 'k' || c == 'K')
      num <<= 10;
    else if (c == 'm' || c == 'M')
      num <<= 20;
    else if (c == 'g' || c == 'G')
      num <<= 30;
  }

  return num;
}

}
