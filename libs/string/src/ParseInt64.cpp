#include <rock/string/ParseInt64.h>


namespace rocksdb {

int64_t ParseInt64(const std::string& value) {
  size_t endchar;
#ifndef CYGWIN
  int64_t num = std::stoll(value.c_str(), &endchar);
#else
  char* endptr;
  int64_t num = std::strtoll(value.c_str(), &endptr, 0);
  endchar = endptr - value.c_str();
#endif

  if (endchar < value.length()) {
    char c = value[endchar];
    if (c == 'k' || c == 'K')
      num <<= 10LL;
    else if (c == 'm' || c == 'M')
      num <<= 20LL;
    else if (c == 'g' || c == 'G')
      num <<= 30LL;
    else if (c == 't' || c == 'T')
      num <<= 40LL;
  }

  return num;
}

}
