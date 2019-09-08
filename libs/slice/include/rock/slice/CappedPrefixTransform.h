#pragma once

#include <rock/slice/SliceTransform.h>
#include <rock/slice/Slice.h>
#include <rock/string/ToString.h>

namespace rocksdb {


class CappedPrefixTransform : public SliceTransform {
 private:
  size_t cap_len_;
  std::string name_;

 public:
  explicit CappedPrefixTransform(size_t cap_len)
      : cap_len_(cap_len),
        // Note that if any part of the name format changes, it will require
        // changes on options_helper in order to make RocksDBOptionsParser work
        // for the new change.
        // TODO(yhchiang): move serialization / deserializaion code inside
        // the class implementation itself.
        name_("rocksdb.CappedPrefix." + ToString(cap_len_)) {}

  const char* Name() const override { return name_.c_str(); }

  Slice Transform(const Slice& src) const override {
    assert(InDomain(src));
    return Slice(src.data(), std::min(cap_len_, src.size()));
  }

  bool InDomain(const Slice& /*src*/) const override { return true; }

  bool InRange(const Slice& dst) const override {
    return (dst.size() <= cap_len_);
  }

  bool FullLengthEnabled(size_t* len) const override {
    *len = cap_len_;
    return true;
  }

  bool SameResultWhenAppended(const Slice& prefix) const override {
    return prefix.size() >= cap_len_;
  }
};

}
