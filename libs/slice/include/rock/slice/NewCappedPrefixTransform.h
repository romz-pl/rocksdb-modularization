#pragma once

#include <cstdlib>

namespace rocksdb {

class SliceTransform;

extern const SliceTransform* NewCappedPrefixTransform(size_t cap_len);

}
