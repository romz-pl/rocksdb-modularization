#pragma once

namespace rocksdb {

class Comparator;

// Return a builtin comparator that uses reverse lexicographic byte-wise
// ordering.
extern const Comparator* ReverseBytewiseComparator();

}
