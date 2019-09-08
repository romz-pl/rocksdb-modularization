#pragma once

#include <rock/slice/SliceTransform.h>
#include <rock/slice/Slice.h>
#include <rock/string/ToString.h>

namespace rocksdb {

class FixedPrefixTransform : public SliceTransform {
 private:
  size_t prefix_len_;
  std::string name_;

 public:
  explicit FixedPrefixTransform(size_t prefix_len)
      : prefix_len_(prefix_len),
        // Note that if any part of the name format changes, it will require
        // changes on options_helper in order to make RocksDBOptionsParser work
        // for the new change.
        // TODO(yhchiang): move serialization / deserializaion code inside
        // the class implementation itself.
        name_("rocksdb.FixedPrefix." + ToString(prefix_len_)) {}

  const char* Name() const override { return name_.c_str(); }

  Slice Transform(const Slice& src) const override {
    assert(InDomain(src));
    return Slice(src.data(), prefix_len_);
  }

  bool InDomain(const Slice& src) const override {
    return (src.size() >= prefix_len_);
  }

  bool InRange(const Slice& dst) const override {
    return (dst.size() == prefix_len_);
  }

  bool FullLengthEnabled(size_t* len) const override {
    *len = prefix_len_;
    return true;
  }

  bool SameResultWhenAppended(const Slice& prefix) const override {
    return InDomain(prefix);
  }
};


}
