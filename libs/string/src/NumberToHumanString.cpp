#include <rock/string/NumberToHumanString.h>

#include <cstdio>
#include <inttypes.h>

namespace rocksdb {

std::string NumberToHumanString(int64_t num) {
  char buf[19];
  int64_t absnum = num < 0 ? -num : num;
  if (absnum < 10000) {
    snprintf(buf, sizeof(buf), "%" PRIi64, num);
  } else if (absnum < 10000000) {
    snprintf(buf, sizeof(buf), "%" PRIi64 "K", num / 1000);
  } else if (absnum < 10000000000LL) {
    snprintf(buf, sizeof(buf), "%" PRIi64 "M", num / 1000000);
  } else {
    snprintf(buf, sizeof(buf), "%" PRIi64 "G", num / 1000000000);
  }
  return std::string(buf);
}

}
