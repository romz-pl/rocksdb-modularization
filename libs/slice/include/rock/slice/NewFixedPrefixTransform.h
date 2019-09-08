#pragma once

#include <cstdlib>

namespace rocksdb {

class SliceTransform;

extern const SliceTransform* NewFixedPrefixTransform(size_t prefix_len);

}
