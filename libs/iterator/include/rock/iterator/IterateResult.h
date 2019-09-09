#pragma once

#include <rock/slice/Slice.h>

namespace rocksdb {

struct IterateResult {
  Slice key;
  bool may_be_out_of_upper_bound;
};

}
