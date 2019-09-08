#include <rock/string/AppendHumanBytes.h>

#include <cstdio>
#include <inttypes.h>

namespace rocksdb {

// for sizes >=10TB, print "XXTB"
// for sizes >=10GB, print "XXGB"
// etc.
// append file size summary to output and return the len
int AppendHumanBytes(uint64_t bytes, char* output, int len) {
  const uint64_t ull10 = 10;
  if (bytes >= ull10 << 40) {
    return snprintf(output, len, "%" PRIu64 "TB", bytes >> 40);
  } else if (bytes >= ull10 << 30) {
    return snprintf(output, len, "%" PRIu64 "GB", bytes >> 30);
  } else if (bytes >= ull10 << 20) {
    return snprintf(output, len, "%" PRIu64 "MB", bytes >> 20);
  } else if (bytes >= ull10 << 10) {
    return snprintf(output, len, "%" PRIu64 "KB", bytes >> 10);
  } else {
    return snprintf(output, len, "%" PRIu64 "B", bytes);
  }
}

}
