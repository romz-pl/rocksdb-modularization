#include <rock/slice/ExtractUserKey.h>

#include <rock/slice/Slice.h>

namespace rocksdb {

// Returns the user key portion of an internal key.
Slice ExtractUserKey(const Slice& internal_key) {
  assert(internal_key.size() >= 8);
  return Slice(internal_key.data(), internal_key.size() - 8);
}

}
