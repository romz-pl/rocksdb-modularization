#include <rock/string/fromHex.h>

namespace rocksdb {

// most of the code is for validation/error check
int fromHex(char c) {
  // toupper:
  if (c >= 'a' && c <= 'f') {
    c -= ('a' - 'A');  // aka 0x20
  }
  // validation
  if (c < '0' || (c > '9' && (c < 'A' || c > 'F'))) {
    return -1;  // invalid not 0-9A-F hex char
  }
  if (c <= '9') {
    return c - '0';
  }
  return c - 'A' + 10;
}

}
