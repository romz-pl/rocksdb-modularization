#pragma once

#include <rock/slice/BytewiseComparatorImpl.h>

#include <cassert>

namespace rocksdb {

class ReverseBytewiseComparatorImpl : public BytewiseComparatorImpl {
 public:
  ReverseBytewiseComparatorImpl() { }

  const char* Name() const override {
    return "rocksdb.ReverseBytewiseComparator";
  }

  int Compare(const Slice& a, const Slice& b) const override {
    return -a.compare(b);
  }

  void FindShortestSeparator(std::string* start,
                             const Slice& limit) const override {
    // Find length of common prefix
    size_t min_length = std::min(start->size(), limit.size());
    size_t diff_index = 0;
    while ((diff_index < min_length) &&
           ((*start)[diff_index] == limit[diff_index])) {
      diff_index++;
    }

    assert(diff_index <= min_length);
    if (diff_index == min_length) {
      // Do not shorten if one string is a prefix of the other
      //
      // We could handle cases like:
      //     V
      // A A 2 X Y
      // A A 2
      // in a similar way as BytewiseComparator::FindShortestSeparator().
      // We keep it simple by not implementing it. We can come back to it
      // later when needed.
    } else {
      uint8_t start_byte = static_cast<uint8_t>((*start)[diff_index]);
      uint8_t limit_byte = static_cast<uint8_t>(limit[diff_index]);
      if (start_byte > limit_byte && diff_index < start->size() - 1) {
        // Case like
        //     V
        // A A 3 A A
        // A A 1 B B
        //
        // or
        //     v
        // A A 2 A A
        // A A 1 B B
        // In this case "AA2" will be good.
#ifndef NDEBUG
        std::string old_start = *start;
#endif
        start->resize(diff_index + 1);
#ifndef NDEBUG
        assert(old_start >= *start);
#endif
        assert(Slice(*start).compare(limit) > 0);
      }
    }
  }

  void FindShortSuccessor(std::string* /*key*/) const override {
    // Don't do anything for simplicity.
  }

  bool CanKeysWithDifferentByteContentsBeEqual() const override {
    return false;
  }

  int CompareWithoutTimestamp(const Slice& a, const Slice& b) const override {
    return -a.compare(b);
  }
};

}
