#include <rock/slice/ReverseBytewiseComparator.h>

#include <rock/slice/ReverseBytewiseComparatorImpl.h>

namespace rocksdb {

const Comparator* ReverseBytewiseComparator() {
  static ReverseBytewiseComparatorImpl rbytewise;
  return &rbytewise;
}

}
