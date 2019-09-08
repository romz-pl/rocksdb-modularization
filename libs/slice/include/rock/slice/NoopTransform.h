#pragma once

#include <rock/slice/SliceTransform.h>
#include <rock/slice/Slice.h>

namespace rocksdb {


class NoopTransform : public SliceTransform {
 public:
  explicit NoopTransform() { }

  const char* Name() const override { return "rocksdb.Noop"; }

  Slice Transform(const Slice& src) const override { return src; }

  bool InDomain(const Slice& /*src*/) const override { return true; }

  bool InRange(const Slice& /*dst*/) const override { return true; }

  bool SameResultWhenAppended(const Slice& /*prefix*/) const override {
    return false;
  }
};


}
