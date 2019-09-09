#include <rock/slice/BytewiseComparator.h>

#include <rock/slice/BytewiseComparatorImpl.h>

namespace rocksdb {

const Comparator* BytewiseComparator() {
  static BytewiseComparatorImpl bytewise;
  return &bytewise;
}

}
