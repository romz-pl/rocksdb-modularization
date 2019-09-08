#include <rock/slice/NewFixedPrefixTransform.h>

#include <rock/slice/FixedPrefixTransform.h>

namespace rocksdb {

const SliceTransform* NewFixedPrefixTransform(size_t prefix_len) {
  return new FixedPrefixTransform(prefix_len);
}

}
