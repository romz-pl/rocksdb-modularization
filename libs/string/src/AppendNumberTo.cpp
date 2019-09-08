#include <rock/string/AppendNumberTo.h>

#include <cstdio>
#include <inttypes.h>

namespace rocksdb {

void AppendNumberTo(std::string* str, uint64_t num) {
  char buf[30];
  snprintf(buf, sizeof(buf), "%" PRIu64, num);
  str->append(buf);
}

}
