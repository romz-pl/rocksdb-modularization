#include <rock/slice/NewCappedPrefixTransform.h>

#include <rock/slice/CappedPrefixTransform.h>

namespace rocksdb {

const SliceTransform* NewCappedPrefixTransform(size_t cap_len) {
  return new CappedPrefixTransform(cap_len);
}

}
